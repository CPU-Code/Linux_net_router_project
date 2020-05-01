#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arp_pthread.h"
#include "arp_link.h"

/*
 *function:	ARP包处理线程
 *		将源MAC和源IP保存
 */
void *arp_pthread(void *arg)
{
	ARP_LINK *p= (ARP_LINK *)arg;
    
    //添加到链表中
	arp_head = inner_arp_link(arp_head, p);
	//ARP回应--
	return NULL;
}