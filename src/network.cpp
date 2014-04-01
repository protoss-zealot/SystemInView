
/************************************************************
 * Name: Netcard information collecting program
 *
 * Description: Implementation of functions declared in
 *				file network.h.
 *
 * Usage:
 * 1.Used as an interface to collect all the information of
 *	 netcards in Linux OS and judge the status of networks.
 * 2.Used as a single program to display information of
 *   netcards.
 *
 * Copyright 2013~ shadowxiaoh@163.com
 * Visit more : http://www.cnblogs.com/XiaoHDeBlog/
 *
 ************************************************************
 * THIS IS A TEST PROGRAM, MAYBE SOME PLACES SHOULD BE  	*
 * OPTIMIZED LATER.	    								    *
 ************************************************************
 *
 * $Id: network.c version 1.1 2013/11/9 Edited by shadowxiaoh $
 */


#include "network.h"


/********************* A Sample Usage ************************/
/*
int main(void)
{
        int totalNum,index;

        struct netCardInfo netcards[MAXINTERFACES];
        totalNum = getNetList(netcards);
        while(true)
        {
                sleep(3);

                for(index=0;index<totalNum;index++)
                {
                        printf("NAME : %s\n",netcards[index].Name);
                        printf("MODE : %d\n",netcards[index].mode);
                        printf("STATUS : %d\n",netcards[index].status);
                        printf("IP : %s\n",netcards[index].IP);
                        printf("MAC : %s\n",netcards[index].MacAdd);
                        printf("MTU : %d\n",netcards[index].MTU);
                        if(getDDX(&netcards[index])==0)
                                printf("BYTES RCV : %u\nBYTES TRANS : %u\nPACKET RECV : %u\nPACKET TRANS : %u\n", netcards[index].BytesRcv,netcards[index].BytesTrans,netcards[index].PackRcv,netcards[index].PackTrans);
                        printf("********************************************\n");
                }
        }
        return 0;
}
*/


/************************************************************
 *
 *	Name：getNetList
 *
 *  Description：Collect all the ipv4 netcards information and
 *               put them into an arrayList.
 *
 *	Parameters： 1.struct netCardInfo a[]:
 *					To store the result.
 *
 *  Return: If finished successfully then return the number of
 *			netcards,else return -1.
 *
 ************************************************************
 */

int getNetList(struct netCardInfo a[])
{
        int fd,intrface;
        int i = 0,num = 0,retn = 0;	//some parameters are used for debug

        struct ifreq buf[MAXINTERFACES];
        //struct arpreq arp;    //如果有必要的话还可以打印出arp表
        struct ifconf ifc;      //接口信息数据结构

        if((fd = socket(AF_INET,SOCK_DGRAM,0))>=0)
        {
                ifc.ifc_len = sizeof(buf);
                ifc.ifc_buf = (caddr_t)buf;
                /*	获取所有接口信息，以ioctrl函数的不同参数调用	*/
                if(!ioctl(fd,SIOCGIFCONF,(char *)&ifc))
                {
                        intrface = ifc.ifc_len/sizeof(struct ifreq);
                        //printf("interface num is intrface=%d\n\n\n",intrface);
                        //num = intrface;
                        while(intrface-->0)
                        {
                                //printf("net device %s/n",buf[intrface].ifr_name);
                                memset(a[i].Name,0,sizeof(a[i].Name));
                                sprintf(a[i].Name,"%s",buf[intrface].ifr_name);

                                /*	Step1: Jugde whether the net card status is promisc	*/
                                if(!(ioctl(fd,SIOCGIFFLAGS,(char *)&buf[intrface])))
                                {
                                        if(buf[intrface].ifr_flags & IFF_PROMISC)
                                        {
                                                //puts("the interface is PROMISC");
                                                a[i].mode = 1;
                                                //retn++;
                                        }
                                        else
                                                a[i].mode = 0;
                                }
                                else//SIOCGIFFLAGS
                                {
                                        //char str[256];
                                        //sprintf(str,"cpm: ioctl device %s",buf[intrface].ifr_name);
                                        a[i].mode = 0;
                                        //perror(str);
                                }//END SIOCGIFFLAGS

                                /*	Step2: Jugde whether the net card status is up    */
                                if(buf[intrface].ifr_flags & IFF_UP)
                                {
                                        //puts("the interface status is UP");
                                        a[i].status = 1;
                                }
                                else
                                {
                                        //puts("the interface status is DOWN");
                                        a[i].status = 0;
                                }

                                /*	Step3: Get IP of the net card	*/
                                if(!(ioctl(fd,SIOCGIFADDR,(char *)&buf[intrface])))
                                {
                                        //puts("IP address is:");
                                        /*
                                        puts(inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
                                        */
                                        //puts("");
                                        //puts(buf[intrface].ifr_addr.sa_data);
                                        memset(a[i].IP,0,sizeof(a[i].IP));
                                        sprintf(a[i].IP,"%s",inet_ntoa(((struct sockaddr_in *)(&buf[intrface].ifr_addr))->sin_addr));
                                }
                                else//SIOCGIFADDR
                                {
                                        /*
                                        char    str[256];
                                        sprintf(str,"cpm: ioctl device %s",buf[intrface].ifr_name);
                                        perror(str);
                                        */
                                        memset(a[i].IP,0,sizeof(a[i].IP));
                                        sprintf(a[i].IP,"%s","0.0.0.0");
                                }//END SIOCGIFADDR


                                /*	Step4: Get HW ADDRESS of the net card	*/
                                memset(a[i].MacAdd,0,sizeof(a[i].MacAdd));
                                if(!(ioctl(fd,SIOCGIFHWADDR,(char *)&buf[intrface])))
                                {    //puts("HW address is:");
                                        sprintf(a[i].MacAdd,"%02x:%02x:%02x:%02x:%02x:%02x",
                                        (unsigned char)buf[intrface].ifr_hwaddr.sa_data[0],
                                        (unsigned char)buf[intrface].ifr_hwaddr.sa_data[1],
                                        (unsigned char)buf[intrface].ifr_hwaddr.sa_data[2],
                                        (unsigned char)buf[intrface].ifr_hwaddr.sa_data[3],
                                        (unsigned char)buf[intrface].ifr_hwaddr.sa_data[4],
                                        (unsigned char)buf[intrface].ifr_hwaddr.sa_data[5]);
                                }
                                else//SIOCGIFHWADDR
                                {
                                        /*
                                        char str[256];
                                        sprintf(str,"cpm: ioctl device %s",buf[intrface].ifr_name);
                                        perror(str);
                                        */
                                        sprintf(a[i].MacAdd,"%s","00:00:00:00:00:00");
                                }//END SIOCGIFHWADDR

                                /*	Step5: Get MTU	*/
                                if(!(ioctl(fd,SIOCGIFMTU,(char *)&buf[intrface])))
                                {
                                        a[i].MTU = buf[intrface].ifr_mtu;
                                }
                                else
                                {
                                a[i].MTU = 0;
                                }//END SIOCGFHWADDR

                                i++;

                        }//END WHILE
                }//SIOCGCONFIG
                else
                        return -1;
                //END SIOCGCONFIG
        }//SOCK
        else
                return -1;
        //END SOCK
        close(fd);

        return i;
}

/*	实际实现是读取设备文件，并处理字符串的方式	*/
int getDDX(struct netCardInfo *netcard)
{
        char text[201];
        char data[201];
        FILE *fp;
        char seps[] = " \t";
        char *token;
        int  i;
        char net[20];

/************************************************************
 *
 *	Name：linux virtual filesystem "/proc/net/dev"
 *
 *	Description：This is a sample format of the file,displays
 *				 information about data of send and recv.We
 *				 can read this file and store them into the
 *				 arrayList.
 *
 *	/proc/net/dev  sample Format:
 *
 *	Inter-|   Receive                                                |  Transmit
 *   face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed
 *      lo:  112200    1769    0    0    0     0          0         0   112200    1769    0    0    0     0       0          0
 *    eth0:  498914    4917    0    0    0     0          0         0   157164    1443    0    0    0     0       0          0
 *
 ************************************************************
 */

        fp = fopen("/proc/net/dev","r");
        if(!fp)
                return -1;
        memset(text,0,201);
        while(fgets(text,200,fp))
        {
                memset(net,0,20);
                strcat(net,netcard->Name);
                strcat(net,":\0");
                token = strtok(text,seps);	//分割字符串函数，第一个必须以源字符串为参数，后面的以NULL为参数
                memset(data,0,201);
                if(strcmp(token,net)==0)
                {
                        token = strtok(NULL,seps);
                        if(token==NULL)
                                return -1;
                        strcat(data,token);
                        strcat(data,"    ");
                        token = strtok(NULL,seps);
                        if(token==NULL)
                                return -1;
                        strcat(data,token);
                        for(i=0;i<6;i++)
                        {
                                token = strtok(NULL,seps);
                                if(token==NULL)
                                        return -1;
                        }
                        strcat(data,"    ");
                        token = strtok(NULL,seps);
                        if(token==NULL)
                                return -1;
                        strcat(data,token);
                        strcat(data,"    ");
                        token = strtok(NULL,seps);
                        if(token==NULL)
                                return -1;
                        strcat(data,token);
                        sscanf(data,"%u%u%u%u",&(netcard->BytesRcv),&(netcard->PackRcv),&(netcard->BytesTrans),&(netcard->PackTrans));  //
                }
                memset(text,0,201);
        }
        fclose(fp);
        return 0;
}

/************************************************************
 *
 *	Name：getNetCardInfo
 *
 *	Description：This function calls the getNetList function
 *				 and getDDX function.Basic infomation are filled
 *				 into the datastructure netCardInfo except the
 *				 Bytes and Packet info.
 *
 *  Parameters: 1.struct netCardInfo netcards[]:
 *					Store basic netcard info.
 *				2.int *num:
 *					Store the number of card detected.
 *
 *	Return: If finished successfully,then return 0,else
 *          return -1.
 *
 ************************************************************
 */
int getNetCardInfo(struct netCardInfo netcards[],int *num)
{
        int totalNum,index;

        totalNum = getNetList(netcards);
        if(totalNum<0)
        {
                *num = 0;
                perror("Error getNetList.");
                return -1;
        }
        for(index=0;index<totalNum;index++)
        {
                if(getDDX(&netcards[index])!=0)
                {
                        perror("Error getDDX message.");
                        return -1;
                }
        }
        *num = totalNum;
        return 0;
}

