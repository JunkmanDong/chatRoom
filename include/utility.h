/*************************************************************************
	> File Name: utility.h
	> Author: RED STREAM red-stream@foxmail.com
	> Created Time: Thu 05 Dec 2019 10:05:23 CST
 ************************************************************************/

#ifndef CHAT_UTILITY_H
#define CHAT_UTILITY_H

//#include <iostream>
#include <list>
using namespace std;

#include "clientTdef/clientTdef.h"
//clients_list save all the clients's socket
extern list<clientTdef> clients_list;

/***** macro defintion *****/
//server ip
//#define SERVER_IP "127.0.0.1"
#define SERVER_IP INADDR_ANY

//server port
#define SERVER_PORT 16888

//epoll size
#define EPOLL_SIZE 5000

//message buffer size
#define BUF_SIZE (4096 * 8)

#define SERVER_WELCOME "Welcome you join to the chat room! Your chat ID is: Client #%d"

#define SERVER_MESSAGE "ClientID %d say >> %s"

//exit
#define EXIT "EXIT"

#define CAUTION "There is only ont int the char root!"


/****** some function *****/
/**
 *设置非阻塞
 */
int setnonblockint(int sockfd);

/**
 * 将文件描述符 fd 添加到 epollfd 标示的内核事件表中,
 * 并注册 EPOLLIN 和 EPOOLET 事件,
 * EPOLLIN 是数据可读事件；EPOOLET 表明是 ET 工作方式。
 * 最后将文件描述符设置非阻塞方式
 * @param epollfd:epoll句柄
 * @param fd:文件描述符
 * @param enable_et:enable_et = true,
 * 是否采用epoll的ET(边缘触发)工作方式；否则采用LT(水平触发)工作方式
 */
void addfd(int epollfd, int fd, bool enable_et);

//发送广播
int sendBroadcastmessage(int sockfd);

#endif //CHAT_UTILITY_H
