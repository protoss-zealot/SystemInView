/*
 *     C/C++ Whetstone Benchmark Single or Double Precision
 *     
 *  #define Precision and Version not used
 *     
 *  Different compilers can produce different floating point numeric
 *  results, probably due to compiling instructions in a different
 *  sequence. As the program checks these, they may need to be changed.
 *  The log file indicates non-standard results and these values can
 *  be copied and pasted into this program. See // Values near the
 *  end of main(). These values are for optimised compiling and non-
 *  optimised results might be different.
 *
 *  Different compilers do not optimise the code in the same way.
 *  This can lead to wide variations in benchmark speeds. See results
 *  with MS6 compiler ID and compare with those from same CPUs from
 *  the Watcom compiler generated code.
 *
 */
 

 #include <math.h>       /* for sin, exp etc.           */
 #include <stdio.h>      /* standard I/O                */
 #include <conio.h>      /* for getch  - 2 occurrences  */ 
 #include <string.h>     /* for strcpy - 1 occurrence   */
 #include <stdlib.h>     /* for exit   - 1 occurrence   */
 #include <windows.h>
 #include <time.h>

 
 // PRECISION
 #ifdef DP 
    #define SPDP double
    #define Precision "Double Precision"
    #define SorD "2"
 #else
    #define SPDP float
//    #define Precision "Single Precision"
    #define Precision " "
   #define SorD "1"
 #endif

 // VERSION
 #ifdef CNNT
    #define options   "Non-optimised"
    #define opt "0"
 #else
//    #define options   "Optimised"
    #define options   " "
    #define opt "1"
 #endif

  void whetstones(long xtra, long x100, int calibrate);  
  void pa(SPDP e[4], SPDP t, SPDP t2);
  void po(SPDP e1[4], long j, long k, long l);
  void p3(SPDP *x, SPDP *y, SPDP *z, SPDP t, SPDP t1, SPDP t2);
  void pout(char title[22], float ops, int type, SPDP checknum,
                  SPDP time, int calibrate, int section);
  void printIt(SPDP val);
  void start_time();
  double end_time();
  void local_time();
  void calculateMHz(void); 
  void WhatConfig();
  

  static SPDP loop_time[9];
  static SPDP loop_mops[9];
  static SPDP loop_mflops[9];
  static SPDP TimeUsed;
  static SPDP mwips;
  static char  headings[9][18];
  static SPDP Check;
  static SPDP results[9];
  FILE    *outfile;

  double secs;
  LARGE_INTEGER starttime;
  char    timeday[30];
  char    configData1[200];
  char    configData2[200];
  char    configData3[200];
  char    configData4[200];
  int     hasMMX;
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

 
  main()
  {       
       int count = 10, calibrate = 1;
       long xtra=1;
       int endit;
       int i;
       int errors;
       long x100 = 100;
       int duration = 10;
       double gmean1;
       double gmean2;
       char expect[9][30];
       char was[30];

       
       printf("\n %s C/C++ Whetstone Benchmark %s \n\n", Precision, options);
       local_time();
       
       outfile = fopen("Whets.txt","a+");
       if (outfile == NULL)
       {
           printf ("Cannot open results file \n\n");
           printf("Press any key\n");
           endit = getch();
           exit (0);
       }
                
       fprintf (outfile, " ********************************************************\n\n");                     
       fprintf (outfile, " Whetstone %s Benchmark %s via C/C++ %s\n",
                                                               Precision, options, timeday);
       printf("Calibrate\n");
       do
       {
            TimeUsed=0;
            
            whetstones(xtra,x100,calibrate);
            
            printf("%11.2f Seconds %10.0lf   Passes (x 100)\n",
                                     TimeUsed,(SPDP)(xtra));
            calibrate = calibrate + 1;
            count = count - 1;
            if (TimeUsed > 0.2)
               count = 0;
            else
               xtra = xtra * 5;
       }
       while (count > 0);
       
       if (TimeUsed > 0)
                     xtra = (long)((SPDP)(duration * xtra) / TimeUsed);
       if (xtra < 1) xtra = 1;
       
       calibrate = 0;
  
       printf("\nUse %d  passes (x 100)\n", xtra);

       printf("\n %s C/C++ Whetstone Benchmark %s\n\n", Precision, options);

       printf("\nLoop content                  Result              MFLOPS "
                                "     MOPS   Seconds\n\n");

       TimeUsed=0;
       whetstones(xtra,x100,calibrate);

       printf("\nMWIPS            ");
       if (TimeUsed>0)
         mwips=(float)(xtra) * (float)(x100) / (10 * TimeUsed);
       else
         mwips = 0;
       
       printf("%39.3f%19.3f\n\n",mwips,TimeUsed);
     
       if (Check == 0)
          printf("Wrong answer  ");
              


    //               Add results to output file Whets.txt

    gmean1 = pow((double)(loop_mflops[1] * loop_mflops[2] * loop_mflops[6]), 1.0/3.0);
    gmean2 = pow((double)(loop_mops[4] * loop_mops[3] * loop_mops[7]), 1.0/3.0) * 5.0;
                          
    fprintf (outfile, " MFLOPS    Vax  MWIPS MFLOPS MFLOPS MFLOPS    Cos    Exp  Fixpt     If  Equal\n");
    fprintf (outfile, "  Gmean   MIPS            1      2      3    MOPS   MOPS   MOPS   MOPS   MOPS\n");
    printIt((SPDP)gmean1);
    printIt((SPDP)gmean2);    
    printIt((SPDP)mwips);
    printIt((SPDP)loop_mflops[1]);
    printIt((SPDP)loop_mflops[2]);
    printIt((SPDP)loop_mflops[6]);
    printIt((SPDP)loop_mops[5]);
    printIt((SPDP)loop_mops[8]);
    printIt((SPDP)loop_mops[4]);
    printIt((SPDP)loop_mops[3]);
    printIt((SPDP)loop_mops[7]);    
    fprintf(outfile,"\n\n");
/*
    if (strtol(SorD, NULL, 10) == 1 && strtol(opt, NULL, 10) == 1)
    {
        fprintf(outfile, " Classic Benchmark Ratings for CPUSpeed.txt where 100 MHz Pentium = 100\n");
        fprintf(outfile, " Integer Whets %d, Float Whets %d\n\n", (int)(gmean2 / 97.0 * 100), (int)(gmean1 / 16.7 * 100)); 
    }
*/
/*
    // Values Watcom
    
    if (strtol(SorD, NULL, 10) == 1)  // SP
    {
        sprintf(expect[1], "%24.17f", -1.12475025653839100);
        sprintf(expect[2], "%24.17f", -1.12274754047393800);
        sprintf(expect[3], "%24.17f",  1.00000000000000000);
        sprintf(expect[4], "%24.17f", 12.00000000000000000);
        sprintf(expect[5], "%24.17f",  0.49904659390449520);
        sprintf(expect[6], "%24.17f",  0.99999988079071040);
        sprintf(expect[7], "%24.17f",  3.00000000000000000);
        sprintf(expect[8], "%24.17f",  0.75110864639282230);
    }
    else // DP
    {
        sprintf(expect[1], "%24.17f", -1.12398255667391900);
        sprintf(expect[2], "%24.17f", -1.12187079889284400);
        sprintf(expect[3], "%24.17f",  1.00000000000000000);
        sprintf(expect[4], "%24.17f", 12.00000000000000000);
        sprintf(expect[5], "%24.17f",  0.49902937281515140);
        sprintf(expect[6], "%24.17f",  0.99999987890802820);
        sprintf(expect[7], "%24.17f",  3.00000000000000000);
        sprintf(expect[8], "%24.17f",  0.75100163018457870);
    }
*/

    // Values MS Visual C++
    
    if (strtol(SorD, NULL, 10) == 1)  // SP
    {
        sprintf(expect[1], "%24.17f", -1.12441420555114750);
        sprintf(expect[2], "%24.17f", -1.12241148948669430);
        sprintf(expect[3], "%24.17f",  1.00000000000000000);
        sprintf(expect[4], "%24.17f", 12.00000000000000000);
        sprintf(expect[5], "%24.17f",  0.49907428026199341);
        sprintf(expect[6], "%24.17f",  0.99999988079071040);
        sprintf(expect[7], "%24.17f",  3.00000000000000000);
        sprintf(expect[8], "%24.17f",  0.75095528364181519);
    }
    else // DP
    {
        sprintf(expect[1], "%24.17f", -1.12398255667391100);
        sprintf(expect[2], "%24.17f", -1.12187079889295080);
        sprintf(expect[3], "%24.17f",  1.00000000000000000);
        sprintf(expect[4], "%24.17f", 12.00000000000000000);
        sprintf(expect[5], "%24.17f",  0.49902937281515491);
        sprintf(expect[6], "%24.17f",  0.99999987890802811);
        sprintf(expect[7], "%24.17f",  3.00000000000000000);
        sprintf(expect[8], "%24.17f",  0.75100163018453769);
    }

    errors = 0;
    
    for (i=1; i<9; i++)
    {
        sprintf(was, "%24.17f", results[i]); 

        if (strcmp (expect[i], was)  != 0)
        {
            fprintf(outfile, " Test %d Non-standard result was %s instead of %s\n",
                               i, was, expect[i]);  
            errors = errors + 1;
        }
    }
    if (errors == 0)
    {
        fprintf(outfile, " Numeric results were as expected\n");
    }

    fprintf(outfile, "\n");
    
    WhatConfig();

    fprintf(outfile,"%s\n", configData1);
    fprintf(outfile,"%s\n", configData2);
    fprintf(outfile,"%s\n\n", configData3);

    fclose (outfile);
    Sleep(1000);
    return 0;
  }

    void whetstones(long xtra, long x100, int calibrate)
    {
        long n1,n2,n3,n4,n5,n6,n7,n8,i,ix,n1mult;
        SPDP x,y,z;              
        long j,k,l;
        SPDP e1[4];
        double timeb;
                        
        SPDP t =  (SPDP)0.49999975;
        SPDP t0 = t;        
        SPDP t1 = (SPDP)0.50000025;
        SPDP t2 = 2.0;
                
        Check=0.0;
       
        n1 = 12*x100;
        n2 = 14*x100;
        n3 = 345*x100;
        n4 = 210*x100;
        n5 = 32*x100;
        n6 = 899*x100;
        n7 = 616*x100;
        n8 = 93*x100;
        n1mult = 10;

        /* Section 1, Array elements */

        e1[0] = 1.0;
        e1[1] = -1.0;
        e1[2] = -1.0;
        e1[3] = -1.0;
        start_time();
        {
            for (ix=0; ix<xtra; ix++)
              {
                for(i=0; i<n1*n1mult; i++)
                  {
                      e1[0] = (e1[0] + e1[1] + e1[2] - e1[3]) * t;
                      e1[1] = (e1[0] + e1[1] - e1[2] + e1[3]) * t;
                      e1[2] = (e1[0] - e1[1] + e1[2] + e1[3]) * t;
                      e1[3] = (-e1[0] + e1[1] + e1[2] + e1[3]) * t;
                  }
                t = (SPDP)1.0 - t;
              }
            t =  t0;                    
        }
        end_time();
        timeb = secs/(double)(n1mult);
        pout("N1 floating point\0",(float)(n1*16)*(float)(xtra),
                             1,e1[3],(SPDP)timeb,calibrate,1);

        /* Section 2, Array as parameter */

        start_time();
        {
            for (ix=0; ix<xtra; ix++)
              { 
                for(i=0; i<n2; i++)
                  {
                     pa(e1,t,t2);
                  }
                t = (SPDP)1.0 - t;
              }
            t =  t0;
        }
        end_time();
        timeb = secs;
        pout("N2 floating point\0",(float)(n2*96)*(float)(xtra),
                             1,e1[3],(SPDP)timeb,calibrate,2);

        /* Section 3, Conditional jumps */
        j = 1;
        start_time();
         {
            for (ix=0; ix<xtra; ix++)
              {
                for(i=0; i<n3; i++)
                  {
                     if(j==1)       j = 2;
                     else           j = 3;
                     if(j>2)        j = 0;
                     else           j = 1;
                     if(j<1)        j = 1;
                     else           j = 0;
                  }
              }
         }
        end_time();
        timeb = secs;
        pout("N3 if then else  \0",(float)(n3*3)*(float)(xtra),
                        2,(SPDP)(j),(SPDP)timeb,calibrate,3);

        /* Section 4, Integer arithmetic */
        j = 1;
        k = 2;
        l = 3;
        start_time();
        {
            for (ix=0; ix<xtra; ix++)
              {
                for(i=0; i<n4; i++)
                  {
                     j = j *(k-j)*(l-k);
                     k = l * k - (l-j) * k;
                     l = (l-k) * (k+j);
                     e1[l-2] = (SPDP)(j + k + l);
                     e1[k-2] = (SPDP)(j * k * l);
                  }
              }
        }
        end_time();
        timeb = secs;
        x = e1[0]+e1[1];
        pout("N4 fixed point   \0",(float)(n4*15)*(float)(xtra),
                                 2,x,(SPDP)timeb,calibrate,4);
     
        /* Section 5, Trig functions */
        x = 0.5;
        y = 0.5;
        start_time();
        {
            for (ix=0; ix<xtra; ix++)
              {
                for(i=1; i<n5; i++)
                  {
                     x = (SPDP)(t*atan(t2*sin(x)*cos(x)/(cos(x+y)+cos(x-y)-1.0)));
                     y = (SPDP)(t*atan(t2*sin(y)*cos(y)/(cos(x+y)+cos(x-y)-1.0)));
                  }
                t = (SPDP)1.0 - t;
              }
            t = t0;
        }
        end_time();
        timeb = secs;
        pout("N5 sin,cos etc.  \0",(float)(n5*26)*(float)(xtra),
                                 2,y,(SPDP)timeb,calibrate,5);
  
        /* Section 6, Procedure calls */
        x = 1.0;
        y = 1.0;
        z = 1.0;
        start_time();
        {
            for (ix=0; ix<xtra; ix++)
              {
                for(i=0; i<n6; i++)
                  {
                     p3(&x,&y,&z,t,t1,t2);
                  }
              }
        }
        end_time();
        timeb = secs;
        pout("N6 floating point\0",(float)(n6*6)*(float)(xtra),
                                1,z,(SPDP)timeb,calibrate,6);
  
        /* Section 7, Array refrences */
        j = 0;
        k = 1;
        l = 2;
        e1[0] = 1.0;
        e1[1] = 2.0;
        e1[2] = 3.0;
        start_time();
        {
            for (ix=0; ix<xtra; ix++)
              {
                for(i=0;i<n7;i++)
                  {
                     po(e1,j,k,l);
                  }
              }
        }
        end_time();
        timeb = secs;
        pout("N7 assignments   \0",(float)(n7*3)*(float)(xtra),
                            2,e1[2],(SPDP)timeb,calibrate,7);
        
        /* Section 8, Standard functions */
        x = 0.75;
        start_time();
        {
            for (ix=0; ix<xtra; ix++)
              {
                for(i=0; i<n8; i++)
                  {
                     x = (SPDP)(sqrt(exp(log(x)/t1)));
                  }
              }
        }
        end_time();
        timeb = secs;
        pout("N8 exp,sqrt etc. \0",(float)(n8*4)*(float)(xtra),
                                2,x,(SPDP)timeb,calibrate,8);

        return;
    }


    void pa(SPDP e[4], SPDP t, SPDP t2)
      {
         long j;
         for(j=0;j<6;j++)
            {
               e[0] = (e[0]+e[1]+e[2]-e[3])*t;
               e[1] = (e[0]+e[1]-e[2]+e[3])*t;
               e[2] = (e[0]-e[1]+e[2]+e[3])*t;
               e[3] = (-e[0]+e[1]+e[2]+e[3])/t2;
            }

         return;
      }

    void po(SPDP e1[4], long j, long k, long l)
      {
         e1[j] = e1[k];
         e1[k] = e1[l];
         e1[l] = e1[j];
         return;
      }

    void p3(SPDP *x, SPDP *y, SPDP *z, SPDP t, SPDP t1, SPDP t2)
      {
         *x = *y;
         *y = *z;
         *x = t * (*x + *y);
         *y = t1 * (*x + *y);
         *z = (*x + *y)/t2;
         return;
      }


    void pout(char title[18], float ops, int type, SPDP checknum,
              SPDP time, int calibrate, int section)
      {
        SPDP mops,mflops;

        Check = Check + checknum;
        loop_time[section] = time;
        strcpy (headings[section],title);
        TimeUsed =  TimeUsed + time;
        if (calibrate == 1)     
        {
              results[section] = checknum;
        }
        if (calibrate == 0)
          {              
            printf("%s %24.17f    ",headings[section],results[section]);    
       
            if (type == 1)
             {
                if (time>0)
                 {
                    mflops = ops/(1000000L*time);
                 }
                else
                 {
                   mflops = 0;
                 }
                loop_mops[section] = 99999;
                loop_mflops[section] = mflops;
                printf(" %9.3f          %9.3f\n",
                 loop_mflops[section], loop_time[section]);                
             }
            else
             {
                if (time>0)
                 {
                   mops = ops/(1000000L*time);
                 }
                else
                 {
                   mops = 0;
                 }
                loop_mops[section] = mops;
                loop_mflops[section] = 0;                 
                printf("           %9.3f%9.3f\n",
                 loop_mops[section], loop_time[section]);
             }
          }
          
        return;
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



void printIt(SPDP val)
{
    if (val > 100)
    {
        fprintf (outfile, "%7.0f", val);
    }
    else if (val > 10)
    {
        fprintf (outfile, "%7.1f", val);
    }
    else if (val > 1)
    {
        fprintf (outfile, "%7.2f", val);
    }
    else
    {
        fprintf (outfile, "%7.3f", val);
    }
}
        

