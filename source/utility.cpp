#include "utility.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cstring>

#include <string>

#include "clientTdef/clientTdef.h"

#include <iostream>

int setnonblockint(int sockfd) {
    fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0) | O_NONBLOCK);
    return 0;
}

void addfd(int epollfd, int fd, bool enable_et) {
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    if (enable_et) {
        ev.events = EPOLLIN | EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);
    setnonblockint(fd);
    printf("fd added to epoll!\n\n");
}


int sendBroadcastmessage(int sockfd) {
//    char buf[BUF_SIZE];
//    unsigned char message[BUF_SIZE];
    char buf[4096 * 8];
    unsigned char message[4096 * 8];
    bzero(buf, 4096 * 8);
    bzero(message, 4096 * 8);

	if (clients_list.empty()) {
		return -1;
	}
	list<clientTdef>::iterator clientfd;
	for (clientfd = clients_list.begin(); clientfd != clients_list.end(); ++clientfd) {
		if ( (*clientfd).socket == sockfd ) {
			break;
		}
	}

	if (clientfd != clients_list.end()) {
	} else {
		return -1;
	}

    printf("read from client(clientID = %d)\n", (*clientfd).socket);
    int len = recv( (*clientfd).socket, buf, 4096 * 8, 0);

    // 处理断开链接的用户
    if (0 == len) {
        close( (*clientfd).socket);
        clients_list.remove(*clientfd);
        printf("ClientID = %d closed.\n now there are %d client in the char room\n",
        (*clientfd).socket, (int)clients_list.size());
    } else {
        /*
        // 聊天室只有单用户提示
        if (1 == clients_list.size()) {
            send(clientfd, CAUTION, strlen(CAUTION), 0);
            return 0;
        }
        sprintf(message, SERVER_MESSAGE, clientfd, buf);
        list<int>::iterator it;
        for (it = clients_list.begin(); it != clients_list.end(); ++it) {
            if (*it != clientfd) {
                if (send(*it, message, BUF_SIZE, 0) < 0) {
                    perror("error");
                    exit(-1);
                }
            }
        }
        */
		if ( (*clientfd).role == W_NULL) {
			if (len > 20) {
				return len;
			}
			std::string str(buf, len);
			std::string::size_type index = str.find(',');
			if (index != std::string::npos) {
				std::string head(str, 0, index);
				if (head == "classA") {
					(*clientfd).role = Wship;
				} else if (head == "classB") {
					(*clientfd).role = Wremote;
				} else {
					(*clientfd).role = W_NULL;
					(*clientfd).status = WEMRW;
					return len;
				}
				str = str.erase(0, index + 1);
				index = str.find(';');
				if (index != std::string::npos) {
					head.clear();
					head = std::string(str, 0, index);
					if (head == "R" || head == "r") {
						(*clientfd).status = WEMonlyR;
					} else if (head == "W" || head == "w") {
						(*clientfd).status = WEMonlyW;
					} else if (head == "RW") {
						(*clientfd).status = WEMRW;
					} else {
						(*clientfd).status = WEMRW;
						(*clientfd).role = W_NULL;
						return len;
					}
				}
				printf("客户端标记完成！\r\n");
			}
		} else {
			if ( (*clientfd).status == WEMonlyR) {
				return len;
			}
			if ( (*clientfd).role == Wship) {
				list<clientTdef>::iterator it;
				for (it = clients_list.begin(); it != clients_list.end(); ++it) {
					if ( it != clientfd) {
						if ( (*it).role == Wremote && (*it).status != WEMonlyW) {
            				if (send( (*it).socket, buf, len, 0) < 0) {
            				    perror("error");
								list<clientTdef>::iterator itp;
								for (itp = clients_list.begin(); itp != clients_list.end(); ++itp) {
									close( (*itp).socket);
								}
								clients_list.clear();
            				    exit(-1);
            				}
						}
					}
				}
			} else if ( (*clientfd).role == Wremote) {
				list<clientTdef>::iterator it;
				for (it = clients_list.begin(); it != clients_list.end(); ++it) {
					if (it != clientfd) {
						if ( (*it).role == Wship && (*it).status != WEMonlyW) {
            				if (send( (*it).socket, buf, len, 0) < 0) {
            				    perror("error");
								list<clientTdef>::iterator itp;
								for (itp = clients_list.begin(); itp != clients_list.end(); ++itp) {
									close( (*itp).socket);
								}
								clients_list.clear();
            				    exit(-1);
            				}
						}
					}
				}
				
			}
		}
		

    }
    return len;
}



