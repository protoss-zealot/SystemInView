/*
 *      "DHRYSTONE" Benchmark Program for PCs
 *      #define Version not used
 */
 // VERSION
               
 #ifdef CNNT
    #define options   "Non-optimised"
 #else
//    #define options   "Optimised"
    #define options   " "
 #endif

#define structassign(d, s)      d = s

typedef enum    {Ident1, Ident2, Ident3, Ident4, Ident5} Enumeration;

typedef int     OneToThirty;
typedef int     OneToFifty;
typedef char    CapitalLetter;
typedef char    String30[31];
typedef int     Array1Dim[51];
typedef unsigned long     Array2Dim[51][51];

struct  Record
{
        struct Record           *PtrComp;
        Enumeration             Discr;
        Enumeration             EnumComp;
        OneToFifty              IntComp;
        String30                StringComp;
};

typedef struct Record   RecordType;
typedef RecordType *    RecordPtr;
typedef int             booleans;

/* #define      NULL            0 */
#define TRUE            1
#define FALSE           0


void Proc0();
void Proc1(RecordPtr PtrParIn);
void Proc2(OneToFifty *IntParIO);
void Proc3(RecordPtr *PtrParOut);
void Proc4();
void Proc5();
void Proc6(Enumeration EnumParIn, Enumeration *EnumParOut);
void Proc7(OneToFifty IntParI1, OneToFifty IntParI2, OneToFifty *IntParOut);
void Proc8(Array1Dim Array1Par, Array2Dim Array2Par, OneToFifty IntParI1,
                                                OneToFifty IntParI2);
extern Enumeration  Func1(CapitalLetter CharPar1, CapitalLetter CharPar2);
extern booleans      Func2(String30 StrParI1, String30 StrParI2);
booleans Func3(Enumeration EnumParIn);
void start_time();
double end_time();
void local_time();
void calculateMHz(void); 
void WhatConfig();


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h> 


 
/*
 * Package 1
 */
int             IntGlob;
booleans         BoolGlob;
char            Char1Glob;
char            Char2Glob;
Array1Dim       Array1Glob;
Array2Dim       Array2Glob;
RecordPtr       PtrGlb;
RecordPtr       PtrGlbNext;

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
 
void getCPUstring(void)
{
    __asm
    {
        lea  edi, idString
        mov  eax, 0
        CPUID
        mov[edi],   ebx
        mov[edi+4], edx
        mov[edi+8], ecx
        mov[edi+12], 0
        mov eaxCode1, eax
    }
}
    
void getCPUDetails(void)
{
    __asm
    {
        mov  eax, 1
        CPUID
        mov eaxCode, eax  ; Features Code
        mov edxCode, edx  ; family/model/stepping
    }
}

main(int argc, char *argv[])
{
    if (argc > 1)
     {
        switch (argv[1][0])
         {
             case 'N':
                break;
             case 'n':
                break;
         }
      }
 
    Proc0();
    exit(0);
}

void Proc0()
{
    OneToFifty              IntLoc1;
    OneToFifty              IntLoc2;
    OneToFifty              IntLoc3;
    char                    CharIndex;
    Enumeration             EnumLoc;
    String30                String1Loc;
    String30                String2Loc;
    unsigned long         Loops;
    
    double                  Microseconds,
                            Dhrystones_Per_Second,
                            Vax_Mips;
                      
    double                  benchtime;
        
    register unsigned long  i;

    int         endit;
    unsigned long count = 10;
    FILE        *Ap;

     /* Initializations */
 
    if ((Ap = fopen("Dhry.txt","a+")) == NULL)
     {
        printf("Can not open Dhry.txt\n\n");
        printf("Press any key\n");
        endit = getch();
        exit(1);
     }
     
/***********************************************************************
 *         Change for compiler and optimisation used                   *
 ***********************************************************************/
       
    PtrGlbNext = (RecordPtr) malloc(sizeof(RecordType));
    PtrGlb = (RecordPtr) malloc(sizeof(RecordType));
    PtrGlb->PtrComp = PtrGlbNext;
    PtrGlb->Discr = Ident1;
    PtrGlb->EnumComp = Ident3;
    PtrGlb->IntComp = 40;
    strcpy(PtrGlb->StringComp, "DHRYSTONE PROGRAM, SOME STRING");
    strcpy(String1Loc, "DHRYSTONE PROGRAM, 1'ST STRING");

    printf ("\n");
    printf ("Dhrystone Benchmark, Version 1.1 (Language: C or C++)\n");
    printf ("\n");

    printf ("Optimisation    %s\n", options);
    printf ("\n");
    
    Loops = 5000;

   do
   {

       Loops = Loops * 2;
       count = count - 1;
       Array2Glob[8][7] = 10;

        start_time();        
        for (i = 0; i < Loops; ++i)
        {
                Proc5();
                Proc4();
                IntLoc1 = 2;
                IntLoc2 = 3;
                strcpy(String2Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
                EnumLoc = Ident2;
                BoolGlob = ! Func2(String1Loc, String2Loc);
                while (IntLoc1 < IntLoc2)
                {
                        IntLoc3 = 5 * IntLoc1 - IntLoc2;
                        Proc7(IntLoc1, IntLoc2, &IntLoc3);
                        ++IntLoc1;
                }
                Proc8(Array1Glob, Array2Glob, IntLoc1, IntLoc3);
                Proc1(PtrGlb);
                for (CharIndex = 'A'; CharIndex <= Char2Glob; ++CharIndex)
                        if (EnumLoc == Func1(CharIndex, 'C'))
                                Proc6(Ident1, &EnumLoc);
                IntLoc3 = IntLoc2 * IntLoc1;
                IntLoc2 = IntLoc3 / IntLoc1;
                IntLoc2 = 7 * (IntLoc3 - IntLoc2) - IntLoc1;
                Proc2(&IntLoc1);
        }
        end_time();
        benchtime = secs;
        
        printf ("%12.0f runs %6.2f seconds \n",(double) Loops, benchtime);
        if (benchtime > 2)
        {
              count = 0;
        }
        else
        {
              if (benchtime < 0.05)
              {
                   Loops = Loops * 5;
              }
        }
   }   /* calibrate/run do while */
   while (count >0);
   
   for (i = 0; i < Loops; ++i)
        {
           count = count +1;
        }
        
    printf ("\n");
  
    printf ("Array2Glob8/7: ");
    if (Array2Glob[8][7] == count + 10)
                           printf ("O.K.  ");
    else                   printf ("WRONG ");
    printf ("%12.0f\n", (double) Array2Glob[8][7]);
       
    Microseconds = benchtime * 1000000 / (double) Loops;
    Dhrystones_Per_Second = (double) Loops / benchtime;
    Vax_Mips = Dhrystones_Per_Second / 1757.0;
    
    printf ("\n"); 
    printf ("Microseconds for one run through Dhrystone: ");
    printf ("%12.2lf \n", Microseconds);
    printf ("Dhrystones per Second:                      ");
    printf ("%10.0lf \n", Dhrystones_Per_Second);
    printf ("VAX  MIPS rating =                          ");
    printf ("%12.2lf \n",Vax_Mips);
    printf ("\n");

    
/************************************************************************
 *                Add results to output file Dhry.txt                   *
 ************************************************************************/
    local_time();

    fprintf (Ap, " ********************************************************\n\n");                     
    fprintf (Ap, " Dhrystone Benchmark  Version 1.1 %s via C/C++ %s\n", options, timeday);
    fprintf (Ap, " VAX MIPS rating:      %12.2lf\n\n",Vax_Mips);
   
   
   if (Array2Glob[8][7] == Loops + 10) fprintf (Ap, " Correct result\n\n");
   else                                fprintf (Ap, " WRONG result\n\n");

    WhatConfig();

    fprintf(Ap,"%s\n", configData1);
    fprintf(Ap,"%s\n", configData2);
    fprintf(Ap,"%s\n\n", configData3);

    fclose(Ap);    
}

void Proc1(RecordPtr PtrParIn)
{
#define NextRecord      (*(PtrParIn->PtrComp))

        structassign(NextRecord, *PtrGlb);
        PtrParIn->IntComp = 5;
        NextRecord.IntComp = PtrParIn->IntComp;
        NextRecord.PtrComp = PtrParIn->PtrComp;
        Proc3(&NextRecord.PtrComp);
        if (NextRecord.Discr == Ident1)
        {
                NextRecord.IntComp = 6;
                Proc6(PtrParIn->EnumComp, &NextRecord.EnumComp);
                NextRecord.PtrComp = PtrGlb->PtrComp;
                Proc7(NextRecord.IntComp, 10, &NextRecord.IntComp);
        }
        else
                structassign(*PtrParIn, NextRecord);

#undef  NextRecord
}

void Proc2(OneToFifty *IntParIO)
{
        OneToFifty              IntLoc;
        Enumeration             EnumLoc;

        IntLoc = *IntParIO + 10;
        for(;;)
        {
                if (Char1Glob == 'A')
                {
                        --IntLoc;
                        *IntParIO = IntLoc - IntGlob;
                        EnumLoc = Ident1;
                }
                if (EnumLoc == Ident1)
                        break;
        }
}

void Proc3(RecordPtr *PtrParOut)
{
        if (PtrGlb != NULL)
                *PtrParOut = PtrGlb->PtrComp;
        else
                IntGlob = 100;
        Proc7(10, IntGlob, &PtrGlb->IntComp);
}

void Proc4()
{
        booleans BoolLoc;

        BoolLoc = Char1Glob == 'A';
        BoolLoc |= BoolGlob;
        Char2Glob = 'B';
}

void Proc5()
{
        Char1Glob = 'A';
        BoolGlob = FALSE;
}

extern booleans Func3();

void Proc6(Enumeration EnumParIn, Enumeration *EnumParOut)
{
        *EnumParOut = EnumParIn;
        if (! Func3(EnumParIn) )
                *EnumParOut = Ident4;
        switch (EnumParIn)
        {
        case Ident1:    *EnumParOut = Ident1; break;
        case Ident2:    if (IntGlob > 100) *EnumParOut = Ident1;
                        else *EnumParOut = Ident4;
                        break;
        case Ident3:    *EnumParOut = Ident2; break;
        case Ident4:    break;
        case Ident5:    *EnumParOut = Ident3;
        }
}

void Proc7(OneToFifty IntParI1, OneToFifty IntParI2, OneToFifty *IntParOut)
{
        OneToFifty      IntLoc;

        IntLoc = IntParI1 + 2;
        *IntParOut = IntParI2 + IntLoc;
}

void Proc8(Array1Dim Array1Par, Array2Dim Array2Par, OneToFifty IntParI1,
                                                OneToFifty IntParI2)
{
        OneToFifty      IntLoc;
        OneToFifty      IntIndex;

        IntLoc = IntParI1 + 5;
        Array1Par[IntLoc] = IntParI2;
        Array1Par[IntLoc+1] = Array1Par[IntLoc];
        Array1Par[IntLoc+30] = IntLoc;
        for (IntIndex = IntLoc; IntIndex <= (IntLoc+1); ++IntIndex)
                Array2Par[IntLoc][IntIndex] = IntLoc;
        ++Array2Par[IntLoc][IntLoc-1];
        Array2Par[IntLoc+20][IntLoc] = Array1Par[IntLoc];
        IntGlob = 5;
}

Enumeration Func1(CapitalLetter CharPar1, CapitalLetter CharPar2)
{
        CapitalLetter   CharLoc1;
        CapitalLetter   CharLoc2;

        CharLoc1 = CharPar1;
        CharLoc2 = CharLoc1;
        if (CharLoc2 != CharPar2)
                return (Ident1);
        else
                return (Ident2);
}

booleans Func2(String30 StrParI1, String30 StrParI2)
{
        OneToThirty             IntLoc;
        CapitalLetter           CharLoc;

        IntLoc = 1;
        while (IntLoc <= 1)
                if (Func1(StrParI1[IntLoc], StrParI2[IntLoc+1]) == Ident1)
                {
                        CharLoc = 'A';
                        ++IntLoc;
                }
        if (CharLoc >= 'W' && CharLoc <= 'Z')
                IntLoc = 7;
        if (CharLoc == 'X')
                return(TRUE);
        else
        {
                if (strcmp(StrParI1, StrParI2) > 0)
                {
                        IntLoc += 7;
                        return (TRUE);
                }
                else
                        return (FALSE);
        }
}

booleans Func3(Enumeration EnumParIn)
{
        Enumeration     EnumLoc;

        EnumLoc = EnumParIn;
        if (EnumLoc == Ident3) return (TRUE);
        return (FALSE);
}

void start_time()
{
    QueryPerformanceCounter(&starttime);
}

double end_time()
{
    LARGE_INTEGER liDiff;
    LARGE_INTEGER liFreq;

    QueryPerformanceCounter(&liDiff);
     
    liDiff.QuadPart -= starttime.QuadPart;

    (void)QueryPerformanceFrequency(&liFreq);

    secs = (double)liDiff.QuadPart / (double) liFreq.QuadPart;
    return secs;
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
    unsigned int startCount = 0;
    unsigned int endCount = 0;
    unsigned int cycleCount = 0;

    max = 0;
    for (i=0; i<10; i++)
    {
        start_time();
        __asm
        {
            RDTSC                 // Pentium op code to obtain instruction cycle count
            mov startCount, eax   
        }
    
        while (end_time() < 0.01)
        {
            __asm
            {
                mov edx, 1000
              lp:
                dec edx            // misc instructions
                jnz lp
                RDTSC              // end instruction cycle count
                mov endCount, eax  
            }
        }
        cycleCount = endCount - startCount;
        megaHz = (unsigned int)((double)cycleCount / 1000000.0 / secs + 0.5);
        if (megaHz > max) max = megaHz;
    }
    megaHz = max;
}



