/*
 * @Author: cpu_code
 * @Date: 2020-05-02 09:53:21
 * @LastEditTime: 2020-05-02 10:02:03
 * @FilePath: \Linux_net_router_project\ip\ip_pthread.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>		//htons
#include <sys/ioctl.h>		//ioctl SIOCGIFINDEX
#include <net/if.h>			//struct ifreq
#include <netinet/ether.h>	//ETH_P_ALL
#include <netpacket/packet.h>
#include "get_interface.h"
#include "ip_pthread.h"
#include "arp_link.h"
#include "main.h"

int netmask_num = 3;		//默认3个255 掩码的长度

//ARP结构体						
unsigned char arp_buf[42] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // dst MAC	[0]-->[5]
		0, 0, 0, 0, 0, 0, 					// src MAC	[6]-->[11]
		0x08, 0x06, 						// ARP	[12]-->[13]
		0x00, 0x01, 						// Ethernet  [14]-->[15]
		0x08, 0x00, 						// IP  [16]-->[17]
		0x06,								// HW address size	[18]
		0x04,								// protocol(IP here) address size  [19]
		0x00, 0x01, 						// opcode : 1.arp request 2.arp answer 3.rarp request 4.rarp answer  [20]-->[21]
		0, 0, 0, 0, 0, 0, 					// src MAC	[22]-->[27]
		0, 0, 0, 0,							// src ip : 192.168.1.254  [28]-->[31]
		0, 0, 0, 0, 0, 0,					// dst MAC	[32]-->[37]
		0, 0, 0, 0							// dst ip :   [38]-->[41]
		};

/**
 * @function: 根据目的ip查找对应网段
 * @parameter: 	unsigned char *ip 目的IP
 * @return: 网段编号
 * @note: 
 */
int find_network_segment(unsigned char *ip)
{
	int i = 0;
    int j = get_interface_num();

	for(i = 0; i < j; i++)
	{
		//比较掩码覆盖的前面几位
		if(memcmp(net_interface[i].ip, ip, netmask_num) == 0)
		{
			break;
		}
	}
	return i;
}

/**
 * @function: 根据网卡编号将对应的数据转发出去
 * @parameter: int network_num 网卡标号
 *              unsigned char *msg_data 转发的数据
 *              unsigned long msg_data_len 数据的长度
 * @return: 网段编号
 * @note: 
 */
int send_msg(char *eth_name, unsigned char *msg_data, unsigned long msg_data_len)
{
	struct ifreq ethrep;
	strncpy(ethrep.ifr_name, eth_name, IFNAMSIZ);

	if(-1 == ioctl(raw_sock_fd, SIOGIFINDEX, &ethrep))
	{
		perror("send_msg-ioctl");
		close(raw_sock_fd);
		_exit(-1);
	}
	struct sockaddr_ll sll;
	bzero(&sll, sizeof(sll));

	sll.sll_ifindex = ethrep.ifr_ifindex;
	int len = sendto(raw_sock_fd, 
                     msg_data, 
					 msg_data_len, 
					 0,
					 (struct sockaddr*)&sll, 
					 sizeof(sll));
	return len;
}

/**
 * @function: 组件ARP报文
 * @parameter: unsigned char *ip 目的IP
 *              int network_num 网段编号
 * @return: 
 * @note: 
 */
void creat_arp_msg(unsigned char *ip,int network_num)
{
	memcpy(arp_buf + 38, ip, 4); 							//目的IP
	memcpy(arp_buf + 28, net_interface[network_num].ip, 4);  //源ip
	memcpy(arp_buf + 6, net_interface[network_num].mac, 6); //源mac
	memcpy(arp_buf + 22, net_interface[network_num].mac, 6); //源mac
}

/**
 * @function: IP处理线程
 *              根据对应的网段选择发送的网卡
 *              没有目的IP的MAC地址则发送ARP请求
 * @parameter: void *arg 待转发的数据
 * @return: 
 * @note: 
 */
void *ip_pthread(void *arg)
{
	RECV_DATA *msg_data = (RECV_DATA *)arg;

	//获取目的ip对应的网卡出口
	int network_num = find_network_segment((msg_data->data) + 30);

	//当前路由器还不支持这个网段，涉及到路由表
	if(network_num == get_interface_num())
	{
		printf("ip_pthread:network_num=%d no this network_segment\n",network_num);
		//network_num=0;//默认发送给0网卡
		return NULL;
	}
	
	//广播数据一律不转发
	if(msg_data->data[33] == 255)
	{
		return NULL;
	}

	//本地回环不转发，会死机
	if(strcmp(net_interface[network_num].name, "lo") == 0)
	{
		return NULL;
	}

	ARP_LINK * pb = find_arp_from_ip(arp_head, (msg_data->data) + 30);

	//发送arp包
	if(pb == NULL)
	{
		int j;

		//发j次arp请求包,未找到则放弃转发
		for(j = 0; j < 3; j++)
		{
			creat_arp_msg((msg_data->data) + 30, network_num);

			int ret = send_msg(net_interface[network_num].name,arp_buf, sizeof(arp_buf));//发送arp消息
			if(ret < 0)
			{
				perror("sendmsg1");
			}

			pb = find_arp_from_ip(arp_head, (msg_data->data) + 30);
			if(pb != NULL)
			{
				break;
			}
		}

		if(pb == NULL)
		{
			printf("%d.%d.%d.%d arp error\n",msg_data->data[30],
										 	 msg_data->data[31],
											 msg_data->data[32],
											 msg_data->data[33]);
			goto IP_PTHREAD_END;
		}
		else
		{
			printf("%d.%d.%d.%d arp OK\n",msg_data->data[30],
										  msg_data->data[31],
										  msg_data->data[32],
										  msg_data->data[33]);
		}
	}
	//转发
	memcpy((msg_data->data), pb->mac, 6);		//修改源MAC
	memcpy((msg_data->data)+6, net_interface[network_num].mac, 6);	//修改目的MAC

	//发送消息
	int ret = send_msg(net_interface[network_num].name,
					   msg_data->data,
					   msg_data->data_len);
	if(ret < 0)
	{
		printf("msg_data.data_len=%d\n",msg_data->data_len);
		perror("sendmsg2");
	}
	
IP_PTHREAD_END:
	return NULL;
}
