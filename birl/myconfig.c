/* -*- Mode: C; tab-width: 4; -*- */

/*
Birl is Ruijie Linux

作者：JessonChan（chinachendongpo@gmail.com//OR//i@moreblue.cn）

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
static const char *VERSION = "0.0.1";
static const char *PACKAGE_BUGREPORT = "http://linuxasking.appspot.com";
#endif

#include "myconfig.h"
#include "myfunc.h"
#include "dlfunc.h"
#include "types.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/stat.h>

#define ACCOUNT_SIZE		65	/* 用户名密码长度*/
#define NIC_SIZE		16	/* 网卡名最大长度 */
#define MAX_PATH		255	/* FILENAME_MAX */
#define D_TIMEOUT		8	/* 默认超时间隔 */
#define D_ECHOINTERVAL		30	/* 默认心跳间隔 */
#define D_RESTARTWAIT		15	/* 默认重连间隔 */
#define D_STARTMODE		1	/* 默认组播模式 */
#define D_DHCPMODE		1	/* 默认DHCP模式 */
#define D_DAEMONMODE		0	/* 默认daemon模式 */
#define D_MAXFAIL		0	/* 默认允许失败次数 */
#define FILE_SIZE               350

static const char *D_DHCPSCRIPT = "dhclient >/dev/null";			/* 默认DHCP脚本 */
static const char *CFG_FILE = "/opt/birl/birl.conf";		/* 配置文件 */
static const char *LOG_FILE = "/opt/birl/bril.log";		/* 日志文件 */
static const char *LOCK_FILE = "/var/run/mentohust.pid";	/* 锁文件 */
#define LOCKMODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)	/* 创建掩码 */

#ifndef NO_NOTIFY
#define D_SHOWNOTIFY		5	/* 默认Show Notify模式 */
int showNotify = D_SHOWNOTIFY;		/* 显示通知 */
#endif

extern int bufType;			/*0内置xrgsu 1内置Win 2仅文件 3文件+校验*/
extern u_char version[];		/* 版本 */
static char *iniBuf;			/*这里是我加的，用来读取配置文件*/
char userName[ACCOUNT_SIZE] = "";	/* 用户名 */
char password[ACCOUNT_SIZE] = "";	/* 密码 */
char nic[NIC_SIZE] = "";		/* 网卡名 */
char dataFile[MAX_PATH] = "";		/* 数据文件 */
char dhcpScript[MAX_PATH] = "";		/* DHCP脚本 */
u_int32_t ip = 0;			/* 本机IP */
u_int32_t mask = 0;			/* 子网掩码 */
u_int32_t gateway = 0;			/* 网关 */
u_int32_t dns = 0;			/* DNS */
u_int32_t pingHost = 0;			/* ping */
u_char localMAC[6];			/* 本机MAC */
u_char destMAC[6];			/* 服务器MAC */
unsigned timeout = D_TIMEOUT;		/* 超时间隔 */
unsigned echoInterval = D_ECHOINTERVAL;	/* 心跳间隔 */
unsigned restartWait = D_RESTARTWAIT;	/* 失败等待 */
unsigned startMode = D_STARTMODE;	/* 组播模式 */
unsigned dhcpMode = D_DHCPMODE;		/* DHCP模式 */
unsigned maxFail = D_MAXFAIL;		/* 允许失败次数 */
pcap_t *hPcap = NULL;			/* Pcap句柄 */
int lockfd = -1;			/* 锁文件描述符 */
static int daemonMode = D_DAEMONMODE;	/* 是否后台运行 */
static pcap_if_t *alldevs;		/*网卡信息*/

/*得到所有网卡，并存放在字串中，数量为int*/
void getNicInfo(CSTRING,int*);
/*选择网卡*/
void selectNic(int);
/*得到配置信息，这是一个分离字符串函数*/
char* getIniStr()
{
	iniBuf=strtok(NULL,"\n");
	if(iniBuf)
	{
		iniBuf=strstr(iniBuf,"=");
		iniBuf++;
	}
	return iniBuf;
}

void itoa_cat(char *buf,int num)
{
	char a[20];
	sprintf(a,"%d",num);
	strcat(buf,a);
}
char *loadFile(const char *fileName)
{
	FILE *fp = NULL;
	long size = 0;
	char *buf = NULL;
	if ((fp=fopen(fileName, "rb")) == NULL)
		return NULL;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	buf = (char *)malloc(size+1);
	buf[size] = '\0';
	if (fread(buf, size, 1, fp) < 1)
	{
		free(buf);
		buf = NULL;
	}
	fclose(fp);
	return buf;
}

int saveFile(const char *buf, const char *fileName)
{
	FILE *fp;
	int result;

	if ((fp=fopen(fileName, "wb")) == NULL)
		return -1;
	result = fwrite(buf, strlen(buf), 1, fp)<1 ? -1 : 0;
	fclose(fp);
	return result;
}


static int readFile();	/* 读取配置文件来初始化 */
static int getAdapter();	/* 查找网卡名 */
static int openPcap();	/* 初始化pcap、设置过滤器 */
void saveConfig(const char *,const char *);	/* 保存参数 */
static void checkRunning();	/* 检测是否已运行 */

int getUserInfo(char *pUserName,char *pPasswd)
{
	readFile();
	strcpy(pUserName,userName);
	strcpy(pPasswd,password);
	return 0;
}
void initConfig(char *pName,char *pPasswd)
{
	int exitFlag = 0;	/* 0Nothing 1退出 2重启 */
	strcpy(userName,pName);
	strcpy(password,pPasswd);
#ifndef NO_DYLOAD
	if (load_libpcap() == -1) {
		/*初始化libpcap()失败*/
		exit(EXIT_FAILURE);
	}
#endif
	checkRunning(exitFlag);
	getAdapter();
	if (dhcpScript[0] == '\0')	/* 未填写DHCP脚本？ */
		strcpy(dhcpScript, D_DHCPSCRIPT);
	newBuffer();
	if (fillHeader()==-1 || openPcap()==-1) {	/* 获取IP、MAC，打开网卡 */
		exit(EXIT_FAILURE);
	}
}

static int readFile()
{
	char tmp[16];
	char *buf = loadFile(CFG_FILE);
	if (buf == NULL)
		return -1;
	iniBuf=strtok(buf,"\n");
	strcpy(userName,getIniStr());
	strcpy(password,getIniStr());
	strcpy(nic,getIniStr());

	ip= inet_addr(getIniStr());
	mask=inet_addr(getIniStr());
	gateway=inet_addr(getIniStr());
	dns=inet_addr(getIniStr());
	pingHost=inet_addr(getIniStr());

	timeout=atoi(getIniStr());
	echoInterval=atoi(getIniStr());
	restartWait=atoi(getIniStr());
	maxFail=atoi(getIniStr());
	startMode=atoi(getIniStr());
	dhcpMode=atoi(getIniStr());
	daemonMode=atoi(getIniStr());
#ifndef NO_NOTIFY
	showNotify=atoi(getIniStr());
#endif
	strcpy(version,getIniStr());
	strcpy(dataFile,getIniStr());
	strcpy(dhcpScript,getIniStr());
	startMode = 1;
	dhcpMode = 1;
	free(buf);
	return 0;
}

void getNicInfo(CSTRING nicInfoStr,int *nicNum)
{
	pcap_if_t *d;
	*nicNum =0;
	char errbuf[PCAP_ERRBUF_SIZE];
	if (pcap_findalldevs(&alldevs, errbuf)==-1 || alldevs==NULL)
	{
		/*查找网卡失败*/
		*nicNum = 0;
	}
	for (d=alldevs; d!=NULL; d=d->next)
		if (!(d->flags & PCAP_IF_LOOPBACK) && strcmp(d->name, "any")!=0)
		{
			sprintf(nicInfoStr[*nicNum],"%s\n\0",d->name);
			(*nicNum)++;
		}
}
static int getAdapter()
{
	int num = 0;
	int se = 1;
	CSTRING nicInfo;
	getNicInfo(nicInfo,&num);
	//{
	//	int i = 0;
	//	for(i=0;i<num;i++)
	//	{
	//		printf("[%d]\t%s",i,nicInfo[i]);
	//	}
	//}
	//printf("select nic\n");
	//scanf("%d",&se);
	selectNic(se);
	return 0;
}

/*selectNum是从1开始*/
void selectNic(int selectNum)
{
	pcap_if_t *d;
	for (d=alldevs; selectNum>1; d=d->next,selectNum--);
	strncpy(nic, d->name, sizeof(nic)-1);
	pcap_freealldevs(alldevs);
}

static int openPcap()
{
	char buf[PCAP_ERRBUF_SIZE], *fmt;
	struct bpf_program fcode;
	if ((hPcap = pcap_open_live(nic, 2048, 1, 1000, buf)) == NULL)
	{
//		printf("!! 打开网卡%s失败: %s\n", nic, buf);
		return -1;
	}
	fmt = formatHex(localMAC, 6);
#ifndef NO_ARP
	sprintf(buf, "((ether proto 0x888e and (ether dst %s or ether dst 01:80:c2:00:00:03)) "
			"or ether proto 0x0806) and not ether src %s", fmt, fmt);
#else
	sprintf(buf, "ether proto 0x888e and (ether dst %s or ether dst 01:80:c2:00:00:03) "
			"and not ether src %s", fmt, fmt);
#endif
	if (pcap_compile(hPcap, &fcode, buf, 0, 0xffffffff) == -1
			|| pcap_setfilter(hPcap, &fcode) == -1)
	{
//		printf("!! 设置pcap过滤器失败: %s\n", pcap_geterr(hPcap));
		return -1;
	}
	pcap_freecode(&fcode);
	return 0;
}

void saveConfig(const char *pName,const char *pPasswd)
{
	char *	buf = (char*)malloc(FILE_SIZE);
	buf[0]='\0';
	buf[FILE_SIZE]='\0';

	strcat(buf,"\n[BIRL]\n");

	strcat(buf,"UserName=");
	strcat(buf,pName);
	strcat(buf,"\n");

	strcat(buf,"Password=");
	strcat(buf,pPasswd?pPasswd:"");
	strcat(buf,"\n");

	strcat(buf,"Nic=");
	strcat(buf,nic);
	strcat(buf,"\n");

	strcat(buf,"IP=");
	strcat(buf,formatIP(ip));
	strcat(buf,"\n");

	strcat(buf,"Mask=");
	itoa_cat(buf,mask);
	strcat(buf,"\n");

	strcat(buf,"Gateway=");
	strcat(buf,formatIP(gateway));
	strcat(buf,"\n");

	strcat(buf,"DNS=");
	strcat(buf,formatIP(dns));
	strcat(buf,"\n");

	strcat(buf,"PingHost=");
	strcat(buf,formatIP(pingHost));
	strcat(buf,"\n");

	strcat(buf,"Timeout=");
	itoa_cat(buf,timeout);
	strcat(buf,"\n");

	strcat(buf,"EchoInterval=");
	itoa_cat(buf,echoInterval);
	strcat(buf,"\n");

	strcat(buf,"RestartWait=");
	itoa_cat(buf,restartWait);
	strcat(buf,"\n");

	strcat(buf,"MaxFail=");
	itoa_cat(buf,maxFail);
	strcat(buf,"\n");

	strcat(buf,"StartMode=");
	itoa_cat(buf,startMode);
	strcat(buf,"\n");

	strcat(buf,"DhcpMode=");
	itoa_cat(buf,dhcpMode);
	strcat(buf,"\n");


	strcat(buf,"DaemonMode=");
	itoa_cat(buf,daemonMode);
	strcat(buf,"\n");

#ifndef NO_NOTIFY
	strcat(buf,"ShowNotify=");
	itoa_cat(buf,showNotify);
	strcat(buf,"\n");
#endif

	strcat(buf,"Version=");
	if (bufType != 0) {
		char ver[10];
		sprintf(ver, "%u.%u", version[0], version[1]);
		strcat(buf,ver);
	} else
	strcat(buf,"0.0.0");
	strcat(buf,"\n");

	strcat(buf,"DataFile=");
	strcat(buf,dataFile);
	strcat(buf,"\n");

	strcat(buf,"DhcpScript=");
	strcat(buf,dhcpScript);
	strcat(buf,"\n");

	saveFile(buf, CFG_FILE);
	free(buf);
}

static void checkRunning(int exitFlag)
{
	struct flock fl;
	lockfd = open (LOCK_FILE, O_RDWR|O_CREAT, LOCKMODE);
	if (lockfd < 0) {
		perror("!! 打开锁文件失败");	/* perror真的很好啊，以前没用它真是太亏了 */
		goto error_exit;
	}
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	fl.l_type = F_WRLCK;
	if (fcntl(lockfd, F_GETLK, &fl) < 0) {
		perror("!! 获取文件锁失败");
		goto error_exit;
	}
	if (exitFlag) {
		if (fl.l_type != F_UNLCK) {
			printf(">> 已发送退出信号给MentoHUST进程(PID=%d).\n", fl.l_pid);
			if (kill(fl.l_pid, SIGINT) == -1)
				perror("!! 结束进程失败");
		}
		else
			printf("!! 没有MentoHUST正在运行！\n");
		if (exitFlag == 1)
			exit(EXIT_SUCCESS);
	}
	else if (fl.l_type != F_UNLCK) {
		printf("!! MentoHUST已经运行(PID=%d)!\n", fl.l_pid);
		exit(EXIT_FAILURE);
	}
	if (daemonMode) {	/* 貌似我过早进入后台模式了，就给个选项保留输出或者输出到文件吧 */
		printf(">> 进入后台运行模式，使用参数-k可退出认证。\n");
		if (daemon(0, (daemonMode+1)%2))
			perror("!! 后台运行失败");
		else if (daemonMode == 3) {
			freopen(LOG_FILE, "w", stdout);
			freopen(LOG_FILE, "a", stderr);
		}
	}
	fl.l_type = F_WRLCK;
	fl.l_pid = getpid();
	if (fcntl(lockfd, F_SETLKW, &fl) < 0) {
		perror("!! 加锁失败");
		goto error_exit;
	}
	return;

error_exit:
	exit(EXIT_FAILURE);
}

