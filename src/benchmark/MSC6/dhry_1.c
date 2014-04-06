/*
 *************************************************************************
 *
 *                   "DHRYSTONE" Benchmark Program
 *                   -----------------------------
 *
 *  Version:    C, Version 2.1
 *
 *  File:       dhry_1.c (part 2 of 3)
 *
 *  Date:       May 25, 1988
 *
 *  Author:     Reinhold P. Weicker
 *
 *************************************************************************
 *
 *     #define options not used
 */

 #include <conio.h>
 #include <time.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <windows.h>
 #include "dhry.h"
 
 #ifdef CNNT
    #define options   "Non-optimised"
    #define opt "0"
 #else
//    #define options   "Optimised"
    #define options   " "
    #define opt "1"
 #endif
 

/* Global Variables: */
 
Rec_Pointer     Ptr_Glob,
                 Next_Ptr_Glob;
int             Int_Glob;
 Boolean         Bool_Glob;
 char            Ch_1_Glob,
                 Ch_2_Glob;
 int             Arr_1_Glob [50];
 int             Arr_2_Glob [50] [50];
 
 char Reg_Define[] = "Register option      Selected.";
 
// extern char     *malloc ();
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


 Enumeration Func_1 (Capital_Letter Ch_1_Par_Val,
                                           Capital_Letter Ch_2_Par_Val);
   /* 
   forward declaration necessary since Enumeration may not simply be int
   */
 
 #ifndef ROPT
 #define REG
         /* REG becomes defined as empty */
         /* i.e. no register variables   */
 #else
 #define REG register
 #endif

 void Proc_1 (REG Rec_Pointer Ptr_Val_Par);
 void Proc_2 (One_Fifty *Int_Par_Ref);
 void Proc_3 (Rec_Pointer *Ptr_Ref_Par);
 void Proc_4 (); 
 void Proc_5 ();
 void Proc_6 (Enumeration Enum_Val_Par, Enumeration *Enum_Ref_Par);
 void Proc_7 (One_Fifty Int_1_Par_Val, One_Fifty Int_2_Par_Val,
                                              One_Fifty *Int_Par_Ref);
 void Proc_8 (Arr_1_Dim Arr_1_Par_Ref, Arr_2_Dim Arr_2_Par_Ref,
                               int Int_1_Par_Val, int Int_2_Par_Val);
                               
 Boolean Func_2 (Str_30 Str_1_Par_Ref, Str_30 Str_2_Par_Ref);
 void start_time();
 double end_time();
 void local_time();
 void calculateMHz(void); 
 void WhatConfig();

 


 /* variables for time measurement: */
 
 #define Too_Small_Time 2
                 /* Measurements should last at least 2 seconds */
 
 double         User_Time;
 
 double          Microseconds,
                 Dhrystones_Per_Second,
                 Vax_Mips;
 
 /* end of variables for time measurement */
 
 
 void main (int argc, char *argv[])
 /*****/
 
   /* main program, corresponds to procedures        */
   /* Main and Proc_0 in the Ada version             */
 {
 
         One_Fifty   Int_1_Loc;
   REG   One_Fifty   Int_2_Loc;
         One_Fifty   Int_3_Loc;
   REG   char        Ch_Index;
         Enumeration Enum_Loc;
         Str_30      Str_1_Loc;
         Str_30      Str_2_Loc;
   REG   int         Run_Index;
   REG   int         Number_Of_Runs; 
         int         endit, count = 10;
         FILE        *Ap;
         int         errors = 0;

 
   /* Initializations */
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
 
   Next_Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
   Ptr_Glob = (Rec_Pointer) malloc (sizeof (Rec_Type));
 
   Ptr_Glob->Ptr_Comp                    = Next_Ptr_Glob;
   Ptr_Glob->Discr                       = Ident_1;
   Ptr_Glob->variant.var_1.Enum_Comp     = Ident_3;
   Ptr_Glob->variant.var_1.Int_Comp      = 40;
   strcpy (Ptr_Glob->variant.var_1.Str_Comp, 
           "DHRYSTONE PROGRAM, SOME STRING");       
   strcpy (Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING");
 
   Arr_2_Glob [8][7] = 10;
         /* Was missing in published program. Without this statement,   */
         /* Arr_2_Glob [8][7] would have an undefined value.            */
         /* Warning: With 16-Bit processors and Number_Of_Runs > 32000, */
         /* overflow may occur for this array element.                  */
 
   printf ("\n");
   printf ("Dhrystone Benchmark, Version 2.1 (Language: C or C++)\n");
   printf ("\n");
   
   printf ("Optimisation    %s\n", options);
   #ifdef ROPT
       printf ("Register option selected\n\n");
   #else
       printf ("Register option not selected\n\n");
       strcpy(Reg_Define, "Register option      Not selected.");
   #endif

 /*  
   if (Reg)
   {
     printf ("Program compiled with 'register' attribute\n");
     printf ("\n");
   }
   else
   {
     printf ("Program compiled without 'register' attribute\n");
     printf ("\n");
   }

   printf ("Please give the number of runs through the benchmark: ");
   {
     int n;
     scanf ("%d", &n);
     Number_Of_Runs = n;
   }   
   printf ("\n"); 
   printf ("Execution starts, %d runs through Dhrystone\n",
                                                 Number_Of_Runs);
 */

   Number_Of_Runs = 5000;

   do
   {

       Number_Of_Runs = Number_Of_Runs * 2;
       count = count - 1;
       Arr_2_Glob [8][7] = 10;
        
       /***************/
       /* Start timer */
       /***************/
  
       start_time();
   
       for (Run_Index = 1; Run_Index <= Number_Of_Runs; ++Run_Index)
       {
 
         Proc_5();
         Proc_4();
           /* Ch_1_Glob == 'A', Ch_2_Glob == 'B', Bool_Glob == true */
         Int_1_Loc = 2;
         Int_2_Loc = 3;
         strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING");
         Enum_Loc = Ident_2;
         Bool_Glob = ! Func_2 (Str_1_Loc, Str_2_Loc);
           /* Bool_Glob == 1 */
         while (Int_1_Loc < Int_2_Loc)  /* loop body executed once */
         {
           Int_3_Loc = 5 * Int_1_Loc - Int_2_Loc;
             /* Int_3_Loc == 7 */
           Proc_7 (Int_1_Loc, Int_2_Loc, &Int_3_Loc);
             /* Int_3_Loc == 7 */
           Int_1_Loc += 1;
         }   /* while */
            /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
         Proc_8 (Arr_1_Glob, Arr_2_Glob, Int_1_Loc, Int_3_Loc);
           /* Int_Glob == 5 */
         Proc_1 (Ptr_Glob);
         for (Ch_Index = 'A'; Ch_Index <= Ch_2_Glob; ++Ch_Index)
                              /* loop body executed twice */
         {
           if (Enum_Loc == Func_1 (Ch_Index, 'C'))
               /* then, not executed */
             {
               Proc_6 (Ident_1, &Enum_Loc);
               strcpy (Str_2_Loc, "DHRYSTONE PROGRAM, 3'RD STRING");
               Int_2_Loc = Run_Index;
               Int_Glob = Run_Index;
             }
         }
           /* Int_1_Loc == 3, Int_2_Loc == 3, Int_3_Loc == 7 */
         Int_2_Loc = Int_2_Loc * Int_1_Loc;
         Int_1_Loc = Int_2_Loc / Int_3_Loc;
         Int_2_Loc = 7 * (Int_2_Loc - Int_3_Loc) - Int_1_Loc;
           /* Int_1_Loc == 1, Int_2_Loc == 13, Int_3_Loc == 7 */
         Proc_2 (&Int_1_Loc);
           /* Int_1_Loc == 5 */
 
       }   /* loop "for Run_Index" */
 
       /**************/
       /* Stop timer */
       /**************/
 
       end_time();
       User_Time = secs;
             
       printf ("%12.0f runs %6.2f seconds \n",(double) Number_Of_Runs, User_Time);
       if (User_Time > 2)
       {
             count = 0;
       }
       else
       {
             if (User_Time < 0.05)
             {
                  Number_Of_Runs = Number_Of_Runs * 5;
             }
       }
   }   /* calibrate/run do while */
   while (count >0);
 
   printf ("\n");
   printf ("Final values (* implementation-dependent):\n");
   printf ("\n");
   printf ("Int_Glob:      ");
   if (Int_Glob == 5)  printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d  ", Int_Glob);
      
   printf ("Bool_Glob:     ");
   if (Bool_Glob == 1) printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d\n", Bool_Glob);
      
   printf ("Ch_1_Glob:     ");
   if (Ch_1_Glob == 'A')  printf ("O.K.  ");               
   else                   printf ("WRONG ");
   printf ("%c  ", Ch_1_Glob);
         
   printf ("Ch_2_Glob:     ");
   if (Ch_2_Glob == 'B')  printf ("O.K.  ");
   else                   printf ("WRONG ");
   printf ("%c\n",  Ch_2_Glob);
   
   printf ("Arr_1_Glob[8]: ");
   if (Arr_1_Glob[8] == 7)  printf ("O.K.  ");
   else                     printf ("WRONG ");
   printf ("%d  ", Arr_1_Glob[8]);
            
   printf ("Arr_2_Glob8/7: ");
   if (Arr_2_Glob[8][7] == Number_Of_Runs + 10)
                          printf ("O.K.  ");
   else                   printf ("WRONG ");
   printf ("%10d\n", Arr_2_Glob[8][7]);
   
   printf ("Ptr_Glob->            ");
   printf ("  Ptr_Comp:       *    %d\n", (int) Ptr_Glob->Ptr_Comp);
   
   printf ("  Discr:       ");
   if (Ptr_Glob->Discr == 0)  printf ("O.K.  ");
   else                       printf ("WRONG ");
   printf ("%d  ", Ptr_Glob->Discr);
            
   printf ("Enum_Comp:     ");
   if (Ptr_Glob->variant.var_1.Enum_Comp == 2)
                        printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d\n", Ptr_Glob->variant.var_1.Enum_Comp);
      
   printf ("  Int_Comp:    ");
   if (Ptr_Glob->variant.var_1.Int_Comp == 17)  printf ("O.K.  ");
   else                                         printf ("WRONG ");
   printf ("%d ", Ptr_Glob->variant.var_1.Int_Comp);
      
   printf ("Str_Comp:      ");
   if (strcmp(Ptr_Glob->variant.var_1.Str_Comp,
                        "DHRYSTONE PROGRAM, SOME STRING") == 0)
                        printf ("O.K.  ");
   else                printf ("WRONG ");   
   printf ("%s\n", Ptr_Glob->variant.var_1.Str_Comp);
   
   printf ("Next_Ptr_Glob->       "); 
   printf ("  Ptr_Comp:       *    %d", (int) Next_Ptr_Glob->Ptr_Comp);
   printf (" same as above\n");
   
   printf ("  Discr:       ");
   if (Next_Ptr_Glob->Discr == 0)
                        printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d  ", Next_Ptr_Glob->Discr);
   
   printf ("Enum_Comp:     ");
   if (Next_Ptr_Glob->variant.var_1.Enum_Comp == 1)
                        printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
   
   printf ("  Int_Comp:    ");
   if (Next_Ptr_Glob->variant.var_1.Int_Comp == 18)
                        printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d ", Next_Ptr_Glob->variant.var_1.Int_Comp);
   
   printf ("Str_Comp:      ");
   if (strcmp(Next_Ptr_Glob->variant.var_1.Str_Comp,
                        "DHRYSTONE PROGRAM, SOME STRING") == 0)
                        printf ("O.K.  ");
   else                printf ("WRONG ");   
   printf ("%s\n", Next_Ptr_Glob->variant.var_1.Str_Comp);
   
   printf ("Int_1_Loc:     ");
   if (Int_1_Loc == 5)
                        printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d  ", Int_1_Loc);
      
   printf ("Int_2_Loc:     ");
   if (Int_2_Loc == 13)
                        printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d\n", Int_2_Loc);
   
   printf ("Int_3_Loc:     ");
   if (Int_3_Loc == 7)
                        printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d  ", Int_3_Loc);
   
   printf ("Enum_Loc:      ");
   if (Enum_Loc == 1)
                        printf ("O.K.  ");
   else                printf ("WRONG ");
   printf ("%d\n", Enum_Loc);
   
   printf ("Str_1_Loc:                             ");
   if (strcmp(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING") == 0)
                        printf ("O.K.  ");
   else                printf ("WRONG ");   
   printf ("%s\n", Str_1_Loc);
   
   printf ("Str_2_Loc:                             ");
   if (strcmp(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING") == 0)
                        printf ("O.K.  ");
   else                printf ("WRONG ");   
   printf ("%s\n", Str_2_Loc);
         
   printf ("\n");
    
 
   if (User_Time < Too_Small_Time)
   {
     printf ("Measured time too small to obtain meaningful results\n");
     printf ("Please increase number of runs\n");
     printf ("\n");
   }
   else
   {
     Microseconds = User_Time * Mic_secs_Per_Second 
                         / (double) Number_Of_Runs;
     Dhrystones_Per_Second = (double) Number_Of_Runs / User_Time;
     Vax_Mips = Dhrystones_Per_Second / 1757.0;
 
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
   fprintf (Ap, " Dhrystone Benchmark  Version 2.1 %s via C/C++ %s\n", options, timeday);
   fprintf (Ap, " VAX MIPS rating:      %12.2lf\n\n",Vax_Mips);

//   fprintf(Ap, " Classic Benchmark Ratings for CPUSpeed.txt where 100 MHz Pentium = 100\n");
/*
   if (strtol(opt, NULL, 10) == 1)
   {
       fprintf(Ap, " Integer Dhry2 Opt %d\n\n", (int)(Vax_Mips / 130 * 100)); 
   }
   else
   {
       fprintf(Ap, " Integer Dhry2 NoOpt %d\n\n", (int)(Vax_Mips / 32 * 100)); 
   }
*/   

   if (Int_Glob != 5)
   {
       fprintf (Ap, " Wrong result Int_Glob Was %d Should be 5\n", Int_Glob);
       errors = errors + 1;
   }
   if (Bool_Glob != 1)
   {
       fprintf (Ap, " Wrong result Bool_Glob Was %d Should be 1\n", Bool_Glob);
       errors = errors + 1;
   }
   if (Ch_1_Glob != 'A')
   {
       fprintf (Ap, " Wrong result Ch_1_Glob Was %c Should be A\n", Ch_1_Glob);
       errors = errors + 1;
   }
   if (Ch_2_Glob != 'B')
   {
       fprintf (Ap, " Wrong result Ch_2_Glob Was %c Should be B\n", Ch_2_Glob);
       errors = errors + 1;
   }         
   if (Arr_1_Glob[8] != 7)
   {
       fprintf (Ap, " Wrong result Arr_1_Glob[8] Was %d Should be 7\n", Arr_1_Glob[8]);
       errors = errors + 1;
   }
   if (Arr_2_Glob[8][7] != Number_Of_Runs + 10)
   {
       fprintf (Ap, " Wrong result Arr_2_Glob[8][7] Was %d Should be %d\n", Arr_2_Glob[8][7], Number_Of_Runs + 10);
       errors = errors + 1;
   }
   if (Ptr_Glob->Discr != 0)
   {
       fprintf (Ap, " Wrong result Ptr_Glob->Discr Was %d Should be 0\n", Ptr_Glob->Discr);
       errors = errors + 1;
   }
   if (Ptr_Glob->variant.var_1.Enum_Comp != 2)
   {
       fprintf (Ap, " Wrong result Ptr_Glob->variant.var_1.Enum_Comp Was %d Should be 2\n", Ptr_Glob->variant.var_1.Enum_Comp);
       errors = errors + 1;
   }
   if (Ptr_Glob->variant.var_1.Int_Comp != 17)
   {
       fprintf (Ap, " Wrong result Ptr_Glob->variant.var_1.Int_Comp Was %d Should be 17\n", Ptr_Glob->variant.var_1.Int_Comp);
       errors = errors + 1;
   }
   if (strcmp(Ptr_Glob->variant.var_1.Str_Comp,  "DHRYSTONE PROGRAM, SOME STRING") != 0)
   {
       fprintf (Ap, " Wrong result Ptr_Glob->variant.var_1.Str_Comp Was %s Should be DHRYSTONE PROGRAM, SOME STRING\n", Ptr_Glob->variant.var_1.Str_Comp);
       errors = errors + 1;
   }
   if (Next_Ptr_Glob->Discr != 0)
   {
       fprintf (Ap, " Wrong result Next_Ptr_Glob->Discr Was %d Should be 0\n", Next_Ptr_Glob->Discr);
       errors = errors + 1;
   }
   if (Next_Ptr_Glob->variant.var_1.Enum_Comp != 1)
   {
       fprintf (Ap, " Wrong result Next_Ptr_Glob->variant.var_1.Enum_Comp Was %d Should be 1\n", Next_Ptr_Glob->variant.var_1.Enum_Comp);
       errors = errors + 1;
   }
   if (Next_Ptr_Glob->variant.var_1.Int_Comp != 18)
   {
       fprintf (Ap, " Wrong result Next_Ptr_Glob->variant.var_1.Int_Comp Was %d Should be 18\n", Next_Ptr_Glob->variant.var_1.Int_Comp);
       errors = errors + 1;
   }
   if (strcmp(Next_Ptr_Glob->variant.var_1.Str_Comp,  "DHRYSTONE PROGRAM, SOME STRING") != 0)
   {
       fprintf (Ap, " Wrong result Next_Ptr_Glob->variant.var_1.Str_Comp Was %s Should be DHRYSTONE PROGRAM, SOME STRING\n", Next_Ptr_Glob->variant.var_1.Str_Comp);
       errors = errors + 1;
   }
   if (Int_1_Loc != 5)
   {
       fprintf (Ap, " Wrong result Int_1_Loc Was %d Should be 5\n", Int_1_Loc);
       errors = errors + 1;
   }
   if (Int_2_Loc != 13)
   {
       fprintf (Ap, " Wrong result Int_2_Loc Was %d Should be 13\n", Int_2_Loc);
       errors = errors + 1;
   }
   if (Int_3_Loc != 7)
   {
       fprintf (Ap, " Wrong result Int_3_Loc Was %d Should be 7\n", Int_3_Loc);
       errors = errors + 1;
   }
   if (Enum_Loc != 1)
   {
       fprintf (Ap, " Wrong result Enum_Loc Was %d Should be 1\n", Enum_Loc);
       errors = errors + 1;
   }
   if (strcmp(Str_1_Loc, "DHRYSTONE PROGRAM, 1'ST STRING") != 0)
   {
       fprintf (Ap, " Wrong result Str_1_Loc Was %s Should be DHRYSTONE PROGRAM, 1'ST STRING\n", Str_1_Loc);
       errors = errors + 1;
   }
   if (strcmp(Str_2_Loc, "DHRYSTONE PROGRAM, 2'ND STRING") != 0)
   {
       fprintf (Ap, " Wrong result Str_2_Loc Was %s Should be DHRYSTONE PROGRAM, 2'ND STRING\n", Str_2_Loc);
       errors = errors + 1;
   }
   if (errors == 0)
   {
       fprintf (Ap, " Numeric results were correct\n\n");
   }
   else
   {
       fprintf (Ap, "\n");
   }
              
   WhatConfig();

   fprintf(Ap,"%s\n", configData1);
   fprintf(Ap,"%s\n", configData2);
   fprintf(Ap,"%s\n\n", configData3);
   
   fclose(Ap);
   }
 }
 
 void Proc_1 (REG Rec_Pointer Ptr_Val_Par)
 /******************/
 
     /* executed once */
 {
   REG Rec_Pointer Next_Record = Ptr_Val_Par->Ptr_Comp;  
                                         /* == Ptr_Glob_Next */
   /* Local variable, initialized with Ptr_Val_Par->Ptr_Comp,    */
   /* corresponds to "rename" in Ada, "with" in Pascal           */
   
   structassign (*Ptr_Val_Par->Ptr_Comp, *Ptr_Glob);
   Ptr_Val_Par->variant.var_1.Int_Comp = 5;
   Next_Record->variant.var_1.Int_Comp 
         = Ptr_Val_Par->variant.var_1.Int_Comp;
   Next_Record->Ptr_Comp = Ptr_Val_Par->Ptr_Comp;
   Proc_3 (&Next_Record->Ptr_Comp);
     /* Ptr_Val_Par->Ptr_Comp->Ptr_Comp 
                         == Ptr_Glob->Ptr_Comp */
   if (Next_Record->Discr == Ident_1)
     /* then, executed */
   {
     Next_Record->variant.var_1.Int_Comp = 6;
     Proc_6 (Ptr_Val_Par->variant.var_1.Enum_Comp, 
            &Next_Record->variant.var_1.Enum_Comp);
     Next_Record->Ptr_Comp = Ptr_Glob->Ptr_Comp;
     Proc_7 (Next_Record->variant.var_1.Int_Comp, 10, 
            &Next_Record->variant.var_1.Int_Comp);
   }
   else /* not executed */
     structassign (*Ptr_Val_Par, *Ptr_Val_Par->Ptr_Comp);
 } /* Proc_1 */
 
 
 void Proc_2 (One_Fifty *Int_Par_Ref)
 /******************/
     /* executed once */
     /* *Int_Par_Ref == 1, becomes 4 */
 
 {
   One_Fifty  Int_Loc;
   Enumeration   Enum_Loc;
 
   Int_Loc = *Int_Par_Ref + 10;
   do /* executed once */
     if (Ch_1_Glob == 'A')
       /* then, executed */
     {
       Int_Loc -= 1;
       *Int_Par_Ref = Int_Loc - Int_Glob;
       Enum_Loc = Ident_1;
     } /* if */
   while (Enum_Loc != Ident_1); /* true */
 } /* Proc_2 */
 
 
 void Proc_3 (Rec_Pointer *Ptr_Ref_Par)
 /******************/
     /* executed once */
     /* Ptr_Ref_Par becomes Ptr_Glob */
 
 {
   if (Ptr_Glob != Null)
     /* then, executed */
     *Ptr_Ref_Par = Ptr_Glob->Ptr_Comp;
   Proc_7 (10, Int_Glob, &Ptr_Glob->variant.var_1.Int_Comp);
 } /* Proc_3 */
 
 
void Proc_4 () /* without parameters */
 /*******/
     /* executed once */
 {
   Boolean Bool_Loc;
 
   Bool_Loc = Ch_1_Glob == 'A';
   Bool_Glob = Bool_Loc | Bool_Glob;
   Ch_2_Glob = 'B';
 } /* Proc_4 */
 
 
 void Proc_5 () /* without parameters */
 /*******/
     /* executed once */
 {
   Ch_1_Glob = 'A';
   Bool_Glob = false;
 } /* Proc_5 */
 
 
         /* Procedure for the assignment of structures,          */
         /* if the C compiler doesn't support this feature       */
 #ifdef  NOSTRUCTASSIGN
 memcpy (d, s, l)
 register char   *d;
 register char   *s;
 register int    l;
 {
         while (l--) *d++ = *s++;
 }
 #endif


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




