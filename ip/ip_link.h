/*
 * @Author: cpu_code
 * @Date: 2020-05-01 17:41:48
 * @LastEditTime: 2020-05-01 22:25:58
 * @FilePath: \Linux_net_router_project\ip\ip_link.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#ifndef IP_LINK_H
#define IP_LINK_H

typedef struct ip_link
{
	unsigned char ip[4];	//ip地址四个字节
	struct ip_link *next;
}IP_LINK;

extern IP_LINK *ip_head;

/**
 * @function: 插入ip过滤链表
 * @parameter: 	IP_LINK *head ip过滤链表头  IP_LINK* p 待插入节点
 * @return: IP_LINK *找到的节点
 * @note: 
 */
extern IP_LINK *inner_ip_link(IP_LINK *ip_head,IP_LINK* p);

/**
 * @function: 打印ip过滤链表
 * @parameter: 	IP_LINK *head ip过滤链表头
 * @return: 
 * @note: 
 */
extern void printf_ip_link(IP_LINK *ip_head);

/**
 * @function: 查找ip过滤链表
 * @parameter: IP_LINK *head ip过滤链表头
 * 				unsigned char *ip 待查找IP
 * @return: IP_LINK *找到的节点
 * @note: 
 */
extern IP_LINK *find_ip(IP_LINK *ip_head, unsigned char *ip);

/**
 * @function: 释放ip过滤链表
 * @parameter: IP_LINK *head ip过滤链表头
 * @return: 
 * @note: 
 */
extern void free_ip_link(IP_LINK *ip_head);

/**
 * @function: 删除ip过滤链表节点
 * @parameter: IP_LINK *head ip过滤链表头
 * 				unsigned char *ip 待删除IP
 * @return: IP_LINK *head ip过滤链表头
 * @note: 
 */
extern IP_LINK *del_ip_for_link(IP_LINK *head,unsigned char *ip);

/**
 * @function: 读取配置文件数据到链表
 * @parameter: 
 * @return: 
 * @note: 
 */
extern void init_ip_link();

/**
 * @function: 保存链表数据到配置文件
 * @parameter: 
 * @return: 
 * @note: 
 */
extern void save_ip_link();

#endif

