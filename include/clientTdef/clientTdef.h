#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H


enum WRW_Tdef_EM {
	WEMonlyR,
	
	WEMonlyW,

	WEMRW
};

enum Wrole_Tdef_EM {
	W_NULL,
	
	Wship,

	Wremote
};

class clientTdef {
	public:
		clientTdef();

		clientTdef(int sockfd);
		
		~clientTdef();
		bool operator == (const clientTdef& fd) {
			return (socket == fd.socket);
		}

	public:
		int socket;

		WRW_Tdef_EM status;

		Wrole_Tdef_EM role;
};


#endif

