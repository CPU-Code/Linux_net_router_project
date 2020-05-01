/*
 * @Author: cpu_code
 * @Date: 2020-05-01 17:41:35
 * @LastEditTime: 2020-05-01 22:22:02
 * @FilePath: \Linux_net_router_project\ip\ip_link.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ip_link.h"

#define ip_config_name "ip_config"

IP_LINK *ip_head = NULL;

/**
 * @function: 保存链表数据到配置文件
 * @parameter: 
 * @return: 
 * @note: 
 */
void save_ip_link()
{
	FILE *ip_config = fopen(ip_config_name,"wb+");
	char buff[20] = "";
	if(ip_config == NULL)
    {
		perror("!!!configure file,in main.c");
		_exit(1);
	}

	IP_LINK *pb = ip_head;
	while(pb != NULL)
    {
		sprintf(buff,"%d.%d.%d.%d\n",pb->ip[0], pb->ip[1],
                                     pb->ip[2], pb->ip[3]);
		fputs(buff, ip_config);
		pb = pb->next;
	}
	fclose(ip_config);
}

/**
 * @function: 读取配置文件数据到链表
 * @parameter: 
 * @return: 
 * @note: 
 */
void init_ip_link()
{
	FILE *ip_config = NULL;

	ip_config = fopen(ip_config_name,"rb+");
	if(ip_config == NULL)
    {
		perror("!!!configure file,in main.c");
		_exit(1);
	}

	while(1)
	{
		char buff[500] = "";
		bzero(buff, sizeof(buff));
		int ip;

		if(fgets(buff, sizeof(buff), ip_config) == NULL)
		{
			break;
		}
		buff[strlen(buff) - 1] = 0;	//注意文件中存在\r

		inet_pton(AF_INET, buff, &ip);
		IP_LINK *pb = (IP_LINK *)malloc(sizeof(IP_LINK));	
		memcpy(pb->ip, &ip, 4);
		ip_head = inner_ip_link(ip_head, pb);
	}

	printf_ip_link(ip_head);
	fclose(ip_config);
}

/**
 * @function: 释放ip过滤链表
 * @parameter: IP_LINK *head ip过滤链表头
 * @return: 
 * @note: 
 */
void free_ip_link(IP_LINK *head)
{
	IP_LINK *pb = head;

	while(head)
    {
		pb = head->next;
		free(head);
		head = pb;
	}
}

/**
 * @function: 打印ip过滤链表
 * @parameter: IP_LINK *head ip过滤链表头
 * @return: 
 * @note: 
 */
void printf_ip_link(IP_LINK *head)
{
	printf("\n---------ip_link_start----------\n");
	IP_LINK *pb = head;

	while(pb != NULL)
	{
		printf("%d.%d.%d.%d\n", pb->ip[0], pb->ip[1], pb->ip[2], pb->ip[3]);
		pb = pb->next;
	}
	printf("---------ip_link_end----------\n\n");
}

/**
 * @function: 查找ip过滤链表
 * @parameter: IP_LINK *head ip过滤链表头
 * 				unsigned char *ip 待查找IP
 * @return:  IP_LINK *找到的节点
 * @note: 
 */
IP_LINK *find_ip(IP_LINK *head, unsigned char *ip)
{
	IP_LINK *pb = head;

	while(pb)
	{
		if(memcmp(pb->ip, ip, 4) == 0)
		{
			break;
		}
		pb = pb->next;
	}
	return pb;
}

/**
 * @function: 插入ip过滤链表
 * @parameter: IP_LINK *head ip过滤链表头
 * 				IP_LINK* p 待插入节点
 * @return: IP_LINK *head 找到的节点
 * @note: 
 */
IP_LINK *inner_ip_link(IP_LINK *head, IP_LINK* p)
{
	IP_LINK *pb = find_ip(head, p->ip);		//查找是否有该记录

	// 未查找到，插入链表，直接插入表头方便
	if(pb == NULL)
	{
		p->next = head;
		head = p;
	}
	else
	{
		free(p);
	}
	return head;
}

/**
 * @function: 删除ip过滤链表节点
 * @parameter: IP_LINK *head ip过滤链表头
 * 				unsigned char *ip 待删除IP
 * @return: IP_LINK *head ip过滤链表头
 * @note: 
 */
IP_LINK *del_ip_for_link(IP_LINK *head, unsigned char *ip)
{
	IP_LINK *pf, *pb;
	pf = head;
    pb = head;

	while(pb)
    {
		if(memcmp(pb->ip, ip, 4) == 0)
        {
			break;
		}
		pf = pb;
		pb = pb->next;
	}
	if(pb!=NULL)
    {
		if(pb == head)
        {
			head = head->next;
		}
        else
        {
			pf->next = pb->next;
		}
		free(pb);
		pb = NULL;
	}
	return head;
}

