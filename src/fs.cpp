/*
 *    HardInfo - Displays System Information
 *    Copyright (C) 2003-2006 Leandro A. F. Pereira <leandro@hardinfo.org>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, version 2.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * Some code from xfce4-mount-plugin, version 0.4.3
 *  Copyright (C) 2005 Jean-Baptiste jb_dul@yahoo.com
 *  Distributed under the terms of GNU GPL 2.
 */


#include "fs.h"



void getFSinfo(struct fsInfo fsi[],int *totalNum)
{
    FILE *mtab;
    char buf[1024];
        char data[1024];
    struct statfs sfs;
    int index = 0;
        int i;
        int lineNum = 0;
        char sep[] = " ";
        char *token;

        //struct fsInfo fsi[MAXDISKNUM];
    mtab = fopen("/etc/mtab", "r");
    if (!mtab)
                return;

    while (fgets(buf, 1024, mtab))
        {
                float size, used, avail;

                memset(data,0,1024);

                token = strtok(buf, sep);
               // printf("first token is : %s\n",token);
                strcat(data,token);	//fsName
                strcat(data,"    ");

                token = strtok(NULL,sep);//mount point
                if(token==NULL)
                        return;
               // printf("second token is : %s\n",token);

                if (!statfs(token, &sfs)) // 第二个项是目录名称
                {
                        strcat(data,token);
                        strcat(data,"    ");

                        //char *strsize = size_human_readable(size),
                    //  *stravail = size_human_readable(avail),
                    //  *strused = size_human_readable(used);

                        //格式：
                        ///dev/mapper/VolGroup-lv_root / ext4 rw 0 0

                        //devname mountpoint filesystem mountas

                        token = strtok(NULL,sep);//filesystem
                        if(token==NULL)
                                return;
                     //   printf("third token is : %s\n",token);
                        strcat(data,token);
                        strcat(data,"    ");

                        token = strtok(NULL,sep);//mount as
                        if(token==NULL)
                                return;
                      //  printf("fourth token is : %s\n",token);
                        strcat(data,token);

                        sscanf(data,"%s%s%s%s",&(fsi[index].fsName),&(fsi[index].mountPt),&(fsi[index].filesystem),&(fsi[index].mountAs));

                        float use_ratio;

                        size = (float) sfs.f_bsize * (float) sfs.f_blocks;
                        avail = (float) sfs.f_bsize * (float) sfs.f_bavail;
                        used = size - avail;

                        /*
                        if (size == 0.0f) {
                                continue;
                        }

                        if (avail == 0.0f) {
                                use_ratio = 100.0f;
                        } else {
                                use_ratio = 100.0f * (used / size);
                        }
                        */
                        sprintf(fsi[index].size,"%f",size);

                       // printf("size : %s\n",fsi[index].size);
                        sprintf(fsi[index].avail,"%f",avail);
                       // printf("avail : %s\n",fsi[index].avail);

                }
                memset(buf,0,1024);
                lineNum++;
                index++;
                /*
                        strreplacechr(tmp[0], "#", '_');
                        strhash = g_strdup_printf("[%s]\n"
                                          "Filesystem=%s\n"
                                          "Mounted As=%s\n"
                                          "Mount Point=%s\n"
                                          "Size=%s\n"
                                          "Used=%s\n"
                                          "Available=%s\n",
                                          tmp[0],
                                          tmp[2],
                                          strstr(tmp[3], "rw") ? "Read-Write" :
                                          "Read-Only", tmp[1], strsize, strused,
                                          stravail);
               char *key = strdup_printf("FS%d", ++count);
                           moreinfo_add_with_prefix("COMP", key, strhash);
                        */
        }
        /*
        for(i=0;i<lineNum;i++)
        {
                printf("%s\n",fsi[i].fsName);
                printf("%s\n",fsi[i].filesystem);
                printf("%s\n",fsi[i].mountAs);
                printf("%s\n",fsi[i].mountPt);
                printf("%s\n",fsi[i].size);
                printf("%s\n",fsi[i].avail);
                printf("************************************\n");
        }
        */
    *totalNum = lineNum;
    fclose(mtab);
}

