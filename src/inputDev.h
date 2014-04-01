#ifndef INPUTDEV_H
#define INPUTDEV_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define MAXIDEVNUM 32


struct inputDevInfo{
        char name[50];
        char phys[50];
        int  type;
        /*
        *	type=0	: keyboard device
        *	type=1	: joystick
        *	type=2	: mouse
        *	type=3	: pcspeaker
        *	type=4	: other
        */
};


static void remove_quotes(char * str);
static char *strend(char * str, char chr);
extern void getInputDev(struct inputDevInfo idevInfo[], int *totalNum);


#endif // INPUTDEV_H
