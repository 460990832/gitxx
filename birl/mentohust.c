/* -*- Mode: C; tab-width: 4; -*- */


/*
Birl is Ruijie Linux

作者：JessonChan（chinachendongpo@gmail.com//OR//i@moreblue.cn）

*/


#define HAVE_ICONV_H

#include "mentohust.h"
#include "myconfig.h"
#include "mystate.h"
#include "myfunc.h"
#include "dlfunc.h"
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>

#ifdef HAVE_ICONV_H
#include <iconv.h>
#endif

#define NO_NOTIFY
extern pcap_t *hPcap;
extern volatile int state;
extern u_char *fillBuf;
extern const u_char *capBuf;
extern unsigned startMode, dhcpMode, maxFail;
extern u_char destMAC[];
extern int lockfd;
static int connetFlag = 1;
#ifndef NO_ARP
extern u_int32_t rip, gateway;
extern u_char gateMAC[];
#endif

static void exit_handle(void);		/* 退出回调 */
static void sig_handle(int sig);	/* 信号回调 */
static void pcap_handle(u_char *user, const struct pcap_pkthdr *h, const u_char *buf);	/* pcap_loop回调 */
static void showRuijieMsg(const u_char *buf, unsigned bufLen);				/* 显示锐捷服务器提示信息 */
static void keepOnLine(u_char *user, const struct pcap_pkthdr *h, const u_char *buf);	/* pcap_loop回调 */
static char ruijieMsg[1024];
void startConnet();
void keepConnet();
char* getServerMsg();
int statasConnet()
{
return connetFlag;
}
int mentohust()
{
	atexit(exit_handle);
	signal(SIGALRM, sig_handle);		/* 定时器 */
	signal(SIGHUP, sig_handle);		/* 注销时 */
	signal(SIGTERM, sig_handle);		/* 被结束时 */
	if(connetFlag)
	{
		printf("%s\n",getServerMsg());
		keepConnet();
		ticktick();
	}
	else
		printf("%s\n",getServerMsg());
	exit(EXIT_FAILURE);
}

void  startConnet()
{
	/* 开始认证 */
	if(sendStartPacket()!=0)
	{
		strcpy(ruijieMsg,"无法找到服务器，请检查网线或网卡\n");
		connetFlag  = 0;
	}
	if (-1 == pcap_loop(hPcap, 3, pcap_handle, NULL)) {	 /* 开始捕获数据包 */
		strcpy(ruijieMsg,"网络连接失败，请检查网络\n");
		connetFlag  = 0;
	}
}
void keepConnet()
{
	if (-1 == pcap_loop(hPcap, 2, keepOnLine, NULL)) { /* 捕获数据包 */
		strcpy(ruijieMsg,"网络失去连接，请检查网络\n");
		connetFlag  = 0;
	}
}
char* getServerMsg()
{
	return ruijieMsg;
}
static void exit_handle(void)
{
	if (state != ID_DISCONNECT)
		switchState(ID_DISCONNECT);
	if (hPcap != NULL)
		pcap_close(hPcap);
	if (fillBuf != NULL)
		free(fillBuf);
	if (lockfd > -1)
		close(lockfd);
#ifndef NO_DYLOAD
	free_libpcap();
#endif
}

static void sig_handle(int sig)
{
	if (sig == SIGALRM)	
		keepConnet();
	else
	{
		pcap_breakloop(hPcap);
		exit(EXIT_SUCCESS);
	}
}

static void keepOnLine(u_char *user, const struct pcap_pkthdr *h, const u_char *buf)
{
	switchState(ID_ECHO);
}
static void pcap_handle(u_char *user, const struct pcap_pkthdr *h, const u_char *buf)
{
	static unsigned failCount = 0;
#ifndef NO_ARP
	if (buf[0x0c]==0x88 && buf[0x0d]==0x8e) {
#endif
		if (memcmp(destMAC, buf+6, 6)!=0 && startMode>2)	/* 服务器MAC地址不符 */
			return;
		capBuf = buf;
		if (buf[0x0F]==0x00 && buf[0x12]==0x01 && buf[0x16]==0x01) {	/* 验证用户名 */
			if (startMode < 3) {
				memcpy(destMAC, buf+6, 6);
				startMode += 3;	/* 标记为已获取 */
			}
			switchState(ID_IDENTITY);
		}
		else if (buf[0x0F]==0x00 && buf[0x12]==0x01 && buf[0x16]==0x04)	/* 验证密码 */
			switchState(ID_CHALLENGE);
		else if (buf[0x0F]==0x00 && buf[0x12]==0x03) {	/* 认证成功 */
			connetFlag = 1;	
			failCount = 0;
			if (!(startMode%3 == 2)) {
				getEchoKey(buf);
				showRuijieMsg(buf, h->caplen);
			}
			switchState(ID_DHCP);
		}
		else if (buf[0x0F]==0x00 && buf[0x12]==0x04) {  /* 认证失败或被踢下线 */
				connetFlag=0;
				showRuijieMsg(buf,h->caplen);
				return ;
		}
#ifndef NO_ARP
	} else if (gateMAC[0]!=0xFE && buf[0x0c]==0x08 && buf[0x0d]==0x06) {
		if (*(u_int32_t *)(buf+0x1c) == gateway) {
			char str[50];
			if (gateMAC[0] == 0xFF) {
				memcpy(gateMAC, buf+0x16, 6);
				printf("** 网关MAC:\t%s\n", formatHex(gateMAC, 6));
				fflush(stdout);
				sprintf(str, "arp -s %s %s", formatIP(gateway), formatHex(gateMAC, 6));
				system(str);
			} else if (buf[0x15]==0x02 && *(u_int32_t *)(buf+0x26)==rip
				&& memcmp(gateMAC, buf+0x16, 6)!=0) {
				printf("** ARP欺骗:\t%s\n", formatHex(buf+0x16, 6));
				fflush(stdout);
#ifndef NO_NOTIFY
				if (showNotify) {
					sprintf(str, "欺骗源: %s", formatHex(buf+0x16, 6));
					show_notify("MentoHUST - ARP提示", str);
				}
#endif
			}
		}
	}
#endif
}

static char *gbk2utf(char *src, size_t srclen)	/* GBK转UTF－8 */
{
	/* GBK一汉字俩字节，UTF-8一汉字3字节，二者ASCII字符均一字节
		 所以这样申请是足够的了，要记得释放 */
	size_t dstlen = srclen * 3 / 2 + 1;
	size_t left = dstlen;
	char *dst, *pdst;
	int res;
	iconv_t cd  = iconv_open("utf-8", "gbk");
	if (cd == (iconv_t)-1)
		return NULL;
	dst = (char *)malloc(dstlen);
	pdst = dst;
	res = iconv(cd, &src, &srclen, &pdst, &left);
	iconv_close(cd);
	if (res == -1)
	{
		free(dst);
		return NULL;
	}
	dst[dstlen-left] = '\0';
	return dst;
}

static void showRuijieMsg(const u_char *buf, unsigned bufLen)
{
	char *serverMsg;
	int length = buf[0x1b];
	if (length > 0)
	{
		for (serverMsg=(char *)(buf+0x1c); *serverMsg=='\r'||*serverMsg=='\n'; serverMsg++,length--);	/* 跳过开头的换行符 */
		if (strlen(serverMsg) < length)
			length = strlen(serverMsg);
		if (length>0 && (serverMsg=gbk2utf(serverMsg, length))!=NULL)
		{
			if(strlen(serverMsg))
				strcpy(ruijieMsg,serverMsg);
			free(serverMsg);
		}
	}
	if ((length=0x1c+buf[0x1b]+0x69+39) < bufLen)
	{
		serverMsg=(char *)(buf+length);
		if (buf[length-1]-2 > bufLen-length)
			length = bufLen - length;
		else
			length = buf[length-1]-2;
		for (; *serverMsg=='\r'||*serverMsg=='\n'; serverMsg++,length--);
		if (length>0 && (serverMsg=gbk2utf(serverMsg, length))!=NULL)
		{
			if(strlen(serverMsg))
				strcpy(ruijieMsg,serverMsg);
			free(serverMsg);
		}
	}
}
