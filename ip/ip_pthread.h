/*
 * @Author: cpu_code
 * @Date: 2020-05-02 09:53:21
 * @LastEditTime: 2020-05-02 09:55:36
 * @FilePath: \Linux_net_router_project\ip\ip_pthread.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#ifndef IP_PTHREAD_H
#define IP_PTHREAD_H

/**
 * @function: IP处理线程
 *            根据对应的网段选择发送的网卡
 *            没有目的IP的MAC地址则发送ARP请求
 * @parameter: void *arg 待转发的数据
 * @return: 
 * @note: 
 */
extern void *ip_pthread(void *arg);

#endif