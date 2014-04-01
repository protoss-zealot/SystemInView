#ifndef FS_H
#define FS_H


#include <stdio.h>
#include <string.h>
#include <sys/vfs.h>
//#include "hardinfo.h"
//#include "computer.h"

#define MAXDISKNUM 32

struct fsInfo{
        char filesystem[50];
        char fsName[50];
        char mountAs[50];
        char mountPt[50];
        char size[50];
        char avail[50];
};


extern void getFSinfo(struct fsInfo fsi[],int *totalNum);

#endif // FS_H
