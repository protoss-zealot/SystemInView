
#ifndef _NETWORKTEST_H
#define _NETWORKTEST_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFERSIZE		20
#define MAXINTERFACES	16

#define true			 1



/*	数据结构定义，如需 MTU,ARP 可再增加	*/
struct netCardInfo
{
        char    		   Name[BUFFERSIZE];
        unsigned    int    mode;					//    it is promisc or normal mode
        unsigned    int    status;      			//    up or down flag
        char    		   IP[BUFFERSIZE];    		//    ip address
        char    		   MacAdd[BUFFERSIZE];		//	  mac address
        unsigned    int    MTU;
        unsigned    int    BytesRcv;
        unsigned    int    BytesTrans;
        unsigned    int    PackRcv;
        unsigned    int    PackTrans;
};

extern int getNetCardInfo(struct netCardInfo netcards[],int *num);
extern int getNetList(struct netCardInfo netcards[]);
extern int getDDX(struct netCardInfo *);

#endif

