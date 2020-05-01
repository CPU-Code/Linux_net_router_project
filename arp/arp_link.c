/*
 * @Author: cpu_code
 * @Date: 2020-05-01 15:33:57
 * @LastEditTime: 2020-05-01 22:10:43
 * @FilePath: \Linux_net_router_project\arp\arp_link.c
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arp_link.h"

ARP_LINK *arp_head = NULL;

/**
 * @function: 释放ARP链表
 * @parameter: ARP_LINK *head ARP链表头
 * @return: 
 * @note: 
 */
void free_arp_link(ARP_LINK *head)
{
	ARP_LINK *pb = head;

	while(head)
	{
		pb = head->next;
		free(head);
		head = pb;
	}
}

/**
 * @function: 打印ARP链表
 * @parameter: ARP_LINK *head ARP链表头
 * @return: 
 * @note: 
 */
void printf_arp_link(ARP_LINK *head)
{
	printf("\n\n---------arp_link_start----------\n");
	ARP_LINK *pb = head;

	while(pb)
	{
		printf("%d.%d.%d.%d-->", pb->ip[0], pb->ip[1], pb->ip[2], pb->ip[3]);
		printf("%02x:%02x:%02x:%02x:%02x:%02x\n",pb->mac[0], pb->mac[1],
												 pb->mac[2], pb->mac[3],
												 pb->mac[4], pb->mac[5]);
		pb = pb->next;
	}
	printf("---------arp_link_end----------\n\n");
}

/**
 * @function: 根据IP查找MAC
 * @parameter: ARP_LINK *head ARP链表头
 * 		  		unsigned char *ip IP地址
 * @return: 	ARP_LINK *查找到的节点
 * @note: 
 */
ARP_LINK *find_arp_from_ip(ARP_LINK *head, unsigned char *ip)
{
	ARP_LINK *pb = head;
	
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
 * @function: 插入ARP链表
 * @parameter: ARP_LINK *head ARP链表头
 * 				unsigned char *ip IP地址
 * @return: ARP_LINK *head ARP链表头
 * @note: 
 */
ARP_LINK *inner_arp_link(ARP_LINK *head, ARP_LINK* p)
{
	//查找是否有该记录
	ARP_LINK *pb = find_arp_from_ip(head, p->ip);
	//未查找到，插入链表，直接插入表头方便
	if(pb == NULL)
	{
		p->next = head;
		head = p;
	}
	else	    //查找到，修改链表
	{
		memcpy(pb->mac, p->mac, 6);	 //拷贝mac地址
	}
	return head;
}

