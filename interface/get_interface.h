#ifndef GET_INTERFACE_H
#define GET_INTERFACE_H

#define MAXINTERFACES 16    /* 最大接口数 */

typedef struct interface{
	char name[20];		//接口名字
	unsigned char ip[4];		//IP地址
	unsigned char mac[6];		//MAC地址
	unsigned char netmask[4];	//子网掩码
	unsigned char br_ip[4];		//广播地址
	int  flag;			//状态
}INTERFACE;

int interface_num = 0;        //接口数量
INTERFACE net_interface[MAXINTERFACES];//接口数据

extern INTERFACE net_interface[MAXINTERFACES];//接口数据

/*
 *function:	获取接口信息
 */
extern void getinterface();

/*
 *function:	获取接口数量
 *return:   接口数量
 */
int get_interface_num();

#endif
