#include "clientTdef/clientTdef.h"

clientTdef::clientTdef()
{
	this->socket = -1;
	this->status = WEMRW;
	this->role = W_NULL;
}

clientTdef::clientTdef(int sockfd) : socket(sockfd) {
	this->status = WEMRW;
	this->role = W_NULL;
}

clientTdef::~clientTdef()
{
}





