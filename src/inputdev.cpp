
#include "inputDev.h"

void remove_quotes(char * str)
{
    if (!str)
                return;

    while (*str == '"')
                *(str++) = ' ';

    strend(str,'"');
}

char *strend(char * str, char chr)
{
    if (!str)
                return NULL;

    char *p;
    if ((p = strchr(str, chr)))
                *p = 0;

    return str;
}


void getInputDev(struct inputDevInfo idevInfo[], int *totalNum)
{
        //struct inputDevInfo idevInfo[MAXIDEVNUM];

    FILE *dev;
    char buffer[128];
    char *tmp, *name = NULL, *phys = NULL;
    int bus = 0, vendor, product, version;
    int d = 0;
        int i = 0;
        *totalNum = 0;

    dev = fopen("/proc/bus/input/devices", "r");
    if (!dev)
                return;


    while (fgets(buffer, 128, dev))
        {
                tmp = buffer;

                switch (*tmp)
                {
                case 'N':
                        name = strdup(tmp + strlen("N: Name="));// copy string to buffer name
                        remove_quotes(name);
                        //printf("name is : %s\n",name);
                        sprintf(idevInfo[i].name,"%s",name);
                        break;
                case 'P':
                phys = strdup(tmp + strlen("P: Phys="));// phys ends with a '\n' char
                        //printf("phys is : %s\n",phys);
                        sprintf(idevInfo[i].phys,"%s",phys);
                break;
                case 'I':
                sscanf(tmp, "I: Bus=%x Vendor=%x Product=%x Version=%x", &bus, &vendor, &product, &version);
                break;
                //dev class
                case 'H':
                if (strstr(tmp, "kbd"))	//power button is a kind of keyboard device?
                        {
                                d = 0;		//INPUT_KEYBOARD;
                                idevInfo[i].type = d;
                        }
                else if (strstr(tmp, "js"))
                        {
                                d = 1;		//INPUT_JOYSTICK;
                                idevInfo[i].type = d;
                        }
                else if (strstr(tmp, "mouse"))
                        {
                                d = 2;		//INPUT_MOUSE;
                                idevInfo[i].type = d;
                        }
                        else
                        {
                                d = 4;		//INPUT_UNKNOWN;
                                idevInfo[i].type = d;
                        }
                break;
                case '\n':
                if (name && strstr(name, "PC Speaker")) {
                          d = 3;		// INPUT_PCSPKR
                                idevInfo[i].type = d;
                }
                i++;
                (*totalNum)++;
                //free(tmp);
            //free(phys);
            //free(name);

                }

                //printf("string name is : %s\n",tmp);
        }
        /*
        for(i=0;i<*totalNum;i++)
        {
                printf("%s\n",idevInfo[i].name);
                printf("%s",idevInfo[i].phys);
                printf("type number : %d\n",idevInfo[i].type);
        }
        */
    fclose(dev);
}





