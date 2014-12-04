/*
Birl is Ruijie Linux

作者：JessonChan（chinachendongpo@gmail.com//OR//i@moreblue.cn）

*/


#ifndef HUSTMOON_MYSTATE_H
#define HUSTMOON_MYSTATE_H

#define ID_DISCONNECT		0	/* 断开状态 */
#define ID_START		1	/* 寻找服务器 */
#define ID_IDENTITY		2	/* 发送用户名 */
#define ID_CHALLENGE		3	/* 发送密码 */
#define ID_ECHO			4	/* 发送心跳包 */
#define ID_DHCP			5	/* 更新IP */
#define ID_WAITECHO		6	/* 等待心跳包 */

int switchState(int type);	/* 改变状态 */
int restart();	/* 重启认证 */

#endif
