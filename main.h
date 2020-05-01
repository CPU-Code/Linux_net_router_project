/*
 * @Author: cpu_code
 * @Date: 2020-05-01 15:33:57
 * @LastEditTime: 2020-05-01 22:27:41
 * @FilePath: \Linux_net_router_project\main.h
 * @Gitee: https://gitee.com/cpu_code
 * @CSDN: https://blog.csdn.net/qq_44226094
 */
#ifndef MIAN_H
#define MIAN_H

int raw_sock_fd;

#define RECV_SIZE 2048
typedef struct recv_data
{
	ssize_t data_len ;
	unsigned char data[RECV_SIZE];
}RECV_DATA;

#endif   