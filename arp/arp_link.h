/*
 * @Author: cpu_code
 * @Date: 2020-05-01 15:33:57
 * @LastEditTime: 2020-05-01 22:13:06
 * @FilePath: \Linux_net_router_project\arp\arp_link.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#ifndef ARP_LINK_H
#define ARP_LINK_H

typedef struct arp_link
{
	unsigned char ip[4];        //ip地址四个字节
	unsigned char mac[6];       //mac地址六个字节
	struct arp_link *next;
}ARP_LINK;

extern ARP_LINK *arp_head;

/**
 * @function: 	插入ARP链表
 * @parameter: ARP_LINK *head ARP链表头
 * 			   unsigned char *ip IP地址
 * @return: ARP_LINK *head ARP链表头
 * @note: 
 */
extern ARP_LINK *inner_arp_link(ARP_LINK *arp_head, ARP_LINK* p);

/**
 * @function: 打印ARP链表
 * @parameter: ARP链表头
 * @return: 
 * @note: 
 */
extern void printf_arp_link(ARP_LINK *arp_head);

/**
 * @function:  根据IP查找MAC
 * @parameter: ARP_LINK *head ARP链表头
 * 				unsigned char *ip IP地址
 * @return: ARP_LINK *查找到的节点
 * @note: 
 */
extern ARP_LINK *find_arp_from_ip(ARP_LINK *arp_head, unsigned char *ip);

/**
 * @function: 释放ARP链表
 * @parameter: ARP_LINK *head ARP链表头
 * @return: 
 * @note: 
 */
extern void free_arp_link(ARP_LINK *arp_head);

#endif
