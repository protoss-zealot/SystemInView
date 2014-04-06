#
# Downloaded from Internet 
# 
# Stored here ,aims to create a benchmark module 
# for the SystemInView.
#


Dhrystone 2 uses dhry_1.c, dhry_2.c and dhry.h
Dhrystone 1 uses dhry1.c

The source code is as used for the first versions of this collection 
to compile to run via DOS, Windows and OS/2. The revised versions use 
QueryPerformanceCounter and QueryPerformanceFrequency functions for
better timer resolution and faster running time with appropriate
changes to calibration variables.

Folder MSC6 contains the latest versions of the programs which
compile with Microsoft Visual C++ 6.0. The programs check numeric
results for correct values and floating point answers can be 
different. Those set as defaults are as found for the MS compiler
but the ones for Watcom are included. These programs also use asm 
for assembly instructions instead of #pragma (see below).See listings 
in this folder for descriptions.

The only real change to code is for calibration in LLLOOPS.CPP as
follows. This prevents what appears to be non-ending calibration
time when one loop is excessively slow (only seen when compiled
for a DEC Alpha CPU).  

   if (RunTime[section][i] < 0.07)
   {
       least = 0.0;
       RunTime[section][i] = 0.05;
   }
 
and

  extra_loops[section][i] = (long)(5.0 / RunTime[section][i]
                                       * lmult) +1;

are changed to (also note run time changes):

    if (RunTime[section][i] < 0.001)
    {
       least = 0.0;
       RunTime[section][i] = 0.0008;
       extra_loops[section][i] = extra_loops[section][i] * 2;
    }

and

  extra_loops[section][i] = (long)(runSecs / RunTime[section][i]
                  * (double)extra_loops[section][i] * lmult) +1;


Additional code is as follows:

 WhatConfig();

 fprintf(Ap,"%s\n", configData1);
 fprintf(Ap,"%s\n", configData2);
 fprintf(Ap,"%s\n\n", configData3);

produces the following (example):

 Windows NT Version 5.0, build 2195, Service Pack 2
 CPU AuthenticAMD, Features Code 0183F9FF, Model Code 00000630, 950 MHz
 Memory 523760 KB, Free 435540 KB



#include <windows.h>

double secs;
LARGE_INTEGER starttime;
char    timeday[30];
char    configData1[200];
char    configData2[200];
char    configData3[200];
char    configData4[200];
int     hasMMX;
double runSecs = 1;
unsigned int megaHz;
DWORD   ramsize;
DWORD   freesize;
char idString[20];
unsigned int eaxCode;
unsigned int eaxCode1;
unsigned int edxCode;
unsigned int startCount;
unsigned int endCount;

 #define MM_EXTENSION 0x00800000

 // Following assembly required for Watcom 10.5
 // other benchmarks use the same assemby code
 // with asm declaration and no .586. 
 
 #pragma aux getCPUstring =     \
        ".586"                  \
        "lea edi, idString"     \
        "mov  eax, 0"           \
        "cpuid"                 \
        "mov[edi], ebx"         \
        "mov[edi+4], edx"       \
        "mov[edi+8], ecx"       \
        "mov[edi+12], 0"        \
        "mov eaxCode1, eax"     \
        modify [eax edi];

 #pragma aux getCPUDetails =    \
        ".586"                  \
        "mov  eax, 1"           \
        "cpuid"                 \
        "mov eaxCode, eax"      \
        "mov edxCode, edx"      \
        modify [eax edx];

 #pragma aux getStart =         \
        ".586"                  \
        "rdtsc"                 \
        "mov startCount, eax"   \   
        modify [eax];

 #pragma aux getEnd =           \
        ".586"                  \
         "mov edx, 1000"        \
     "lp: dec edx"              \  
         "jnz lp"               \
         "rdtsc"                \
         "mov endCount, eax"    \  
         modify [eax edx];

void start_time()
{
    QueryPerformanceCounter(&starttime);
}

void end_time()
{
    LARGE_INTEGER liDiff;
    LARGE_INTEGER liFreq;

    QueryPerformanceCounter(&liDiff);

    liDiff.QuadPart -= starttime.QuadPart;

    (void)QueryPerformanceFrequency(&liFreq);

    secs = (double)liDiff.QuadPart / (double) liFreq.QuadPart;
}

void local_time()
{
    time_t time_of_day;
    time_of_day = time( NULL ); 
    strcpy (timeday, ctime(&time_of_day)); /* timeday = date and time */
    return;
}

void WhatConfig()
{
    OSVERSIONINFO vers;
    char szosBuild[20] = "";
    char os[100];
    SYSTEM_INFO info;
    BOOL  OldSys;    
    MEMORYSTATUS lpBuffer;

    hasMMX = 0;

    // **************** OS *********************

    vers.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&vers);
    
    if (vers.dwPlatformId == VER_PLATFORM_WIN32s)
    {
        strcpy(os, " Windows 3.1");
    }
    else if (vers.dwPlatformId == 1) // VER_PLATFORM_WIN32_WINDOWS)
    {
        strcpy(os, " Windows 95");
        if (vers.dwMinorVersion > 0) strcpy(os, " Windows 98");
        sprintf(szosBuild, "build %d,", (DWORD)(LOWORD(vers.dwBuildNumber)));
    }
    else if (vers.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        strcpy(os, " Windows NT");
        sprintf(szosBuild, "build %d,", vers.dwBuildNumber);

    }
    else
    {
        strcpy(os, "OS?");
    }
    sprintf (configData1, "%s Version %d.%d, %s %s",
                        os, vers.dwMajorVersion,
                        vers.dwMinorVersion, szosBuild, vers.szCSDVersion);

    // **************** CPU *********************

    lpBuffer.dwLength = sizeof(MEMORYSTATUS);

    GetSystemInfo(&info);
        
    OldSys = FALSE;
    if (info.dwProcessorType == PROCESSOR_INTEL_386)
    {
        strcpy(configData2, " CPU Type 80386");
        OldSys = TRUE;
    }
    if (info.dwProcessorType == PROCESSOR_INTEL_486)
    {
        strcpy(configData2, " CPU Type 80486");
        OldSys = TRUE;
    }

    if (!OldSys)
    {
       getCPUstring();
       getCPUDetails();
       calculateMHz();
       if (edxCode & MM_EXTENSION) hasMMX = 1;
       sprintf(configData2, " CPU %s, Features Code %8.8X, Model Code %8.8X, %d MHz",
                           idString, edxCode, eaxCode, megaHz);
    } 
    GlobalMemoryStatus(&lpBuffer);
    ramsize = lpBuffer.dwTotalPhys;
    freesize = lpBuffer.dwAvailPhys;
    sprintf(configData3, " Memory %d KB, Free %d KB", ramsize / 1024, freesize / 1024);
    sprintf(configData4, " Memory Load %d. Paging MB %d, Free %d. User Virtual MB %d, Free %d.",
                           lpBuffer.dwMemoryLoad,
                           lpBuffer.dwTotalPageFile/1024/1024,
                           lpBuffer.dwAvailPageFile/1024/1024,
                           lpBuffer.dwTotalVirtual/1024/1024,
                           lpBuffer.dwAvailVirtual/1024/1024); 
    
}


void calculateMHz(void)
{
    unsigned int i, max;
    unsigned int cycleCount = 0;

    startCount = 0;
    endCount = 0;
    max = 0;
    for (i=0; i<10; i++)
    {
        start_time();
        getStart();    
        do
        {
            getEnd();
            end_time();
        }
        while (secs < 0.01);
        
        cycleCount = endCount - startCount;
        megaHz = (unsigned int)((double)cycleCount / 1000000.0 / secs + 0.5);
        if (megaHz > max) max = megaHz;
    }
    megaHz = max;
}







