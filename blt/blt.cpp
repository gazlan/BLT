/* ******************************************************************** **
** @@ Baby Language Translator
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Dscr   :
** ******************************************************************** */

/* ******************************************************************** **
**                uses pre-compiled headers
** ******************************************************************** */

#include <stdafx.h>

#include "..\shared\file_find.h"

/* ******************************************************************** **
** @@                   internal defines
** ******************************************************************** */

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef NDEBUG
#pragma optimize("gsy",on)
#pragma comment(linker,"/FILEALIGN:512 /MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR /IGNORE:4078")
#endif 

#define ASCII_SIZE      (256)

/* ******************************************************************** **
** @@                   internal prototypes
** ******************************************************************** */

/* ******************************************************************** **
** @@                   external global variables
** ******************************************************************** */

extern DWORD   dwKeepError = 0;

/* ******************************************************************** **
** @@                   static global variables
** ******************************************************************** */
                  
static int  pXLIT[ASCII_SIZE];

/* ******************************************************************** **
** @@                   real code
** ******************************************************************** */

/* ******************************************************************** **
** @@ Cyrillic_2_BLT()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void Cyrillic_2_BLT(char* pszDst,const char* const pszSrc)
{
   *pszDst = 0;  // Reset

   int   iLen = strlen(pszSrc);

   for (int ii = 0; ii < iLen; ++ii)
   {
      int   iRet = pXLIT[(BYTE)pszSrc[ii]];

      if (iRet != -1)
      {
         strcat(pszDst,(char*)&iRet); 
      }
   }
}

/* ******************************************************************** **
** @@ Init()
** @  Copyrt :
** @  Author :
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void Init()
{   
   // Initialize
   for (int ii = 0; ii < ASCII_SIZE; ++ii)
   {
      pXLIT[ii] = (BYTE)ii;
   }

   // Cyrillic Upper case
   pXLIT[(BYTE)'À'] |= (BYTE)'Ñ' << 8 | (BYTE)'À' << 16;
   pXLIT[(BYTE)'Å'] |= (BYTE)'Ñ' << 8 | (BYTE)'Å' << 16;
   pXLIT[(BYTE)'¨'] |= (BYTE)'Ñ' << 8 | (BYTE)'¨' << 16;
   pXLIT[(BYTE)'È'] |= (BYTE)'Ñ' << 8 | (BYTE)'È' << 16;
   pXLIT[(BYTE)'Î'] |= (BYTE)'Ñ' << 8 | (BYTE)'Î' << 16;
   pXLIT[(BYTE)'Ó'] |= (BYTE)'Ñ' << 8 | (BYTE)'Ó' << 16;
   pXLIT[(BYTE)'Û'] |= (BYTE)'Ñ' << 8 | (BYTE)'Û' << 16;
   pXLIT[(BYTE)'Ý'] |= (BYTE)'Ñ' << 8 | (BYTE)'Ý' << 16;
   pXLIT[(BYTE)'Þ'] |= (BYTE)'Ñ' << 8 | (BYTE)'Þ' << 16;
   pXLIT[(BYTE)'ß'] |= (BYTE)'Ñ' << 8 | (BYTE)'ß' << 16;

   // Cyrillic Lower case
   pXLIT[(BYTE)'à'] |= (BYTE)'ñ' << 8 | (BYTE)'à' << 16;
   pXLIT[(BYTE)'å'] |= (BYTE)'ñ' << 8 | (BYTE)'å' << 16;
   pXLIT[(BYTE)'¸'] |= (BYTE)'ñ' << 8 | (BYTE)'¸' << 16;
   pXLIT[(BYTE)'è'] |= (BYTE)'ñ' << 8 | (BYTE)'è' << 16;
   pXLIT[(BYTE)'î'] |= (BYTE)'ñ' << 8 | (BYTE)'î' << 16;
   pXLIT[(BYTE)'ó'] |= (BYTE)'ñ' << 8 | (BYTE)'ó' << 16;
   pXLIT[(BYTE)'û'] |= (BYTE)'ñ' << 8 | (BYTE)'û' << 16;
   pXLIT[(BYTE)'ý'] |= (BYTE)'ñ' << 8 | (BYTE)'ý' << 16;
   pXLIT[(BYTE)'þ'] |= (BYTE)'ñ' << 8 | (BYTE)'þ' << 16;
   pXLIT[(BYTE)'ÿ'] |= (BYTE)'ñ' << 8 | (BYTE)'ÿ' << 16;
}

/* ******************************************************************** **
** @@ Proceed()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void Proceed(const char* const pszFileName)
{
   char     pszDstName[_MAX_PATH];
   char     pszDrive  [_MAX_DRIVE];
   char     pszDir    [_MAX_DIR];
   char     pszFName  [_MAX_FNAME];
   char     pszExt    [_MAX_EXT];

   _splitpath(pszFileName,pszDrive,pszDir,pszFName,pszExt);
   _makepath( pszDstName,pszDrive,pszDir,pszFName,"blt");

   FILE*    pIn = fopen(pszFileName,"rt");

   if (!pIn)
   {
      perror("\a\nOpen Input File Error !\n");
      return;
   }

   FILE*    pOut = fopen(pszDstName,"wt");

   if (!pOut)
   {
      perror("\a\nOpen Output File Error !\n");
      fclose(pIn);
      pIn = NULL;
      return;
   }

   const DWORD    BUF_SIZE = (0x01 << 20);   // About 1 Mb !

   char*    pBuf = new char[BUF_SIZE];

   if (!pBuf)
   {
      fclose(pIn);
      pIn = NULL;
      fclose(pOut);
      pOut = NULL;
      return;
   }

   char*    pOutBuf = new char[BUF_SIZE];

   if (!pOutBuf)
   {
      delete pBuf;
      pBuf = NULL;
      fclose(pIn);
      pIn = NULL;
      fclose(pOut);
      pOut = NULL;
      return;
   }

   while (fgets(pBuf,BUF_SIZE,pIn))
   {
      pBuf[BUF_SIZE - 1] = 0; // ASCIIZ !

      Cyrillic_2_BLT(pOutBuf,pBuf);

      if (*pOutBuf)
      {
         fprintf(pOut,"%s",pOutBuf);
      }
   }

   delete pBuf;
   pBuf = NULL;

   delete pOutBuf;
   pOutBuf = NULL;

   fclose(pIn);
   pIn = NULL;

   fclose(pOut);
   pOut = NULL;
}

/* ******************************************************************** **
** @@ ShowHelp()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

static void ShowHelp()
{
   const char  pszCopyright[] = "-*-   BLT 1.0   *   Copyright (c) Gazlan, 2015   -*-";
   const char  pszDescript [] = "Baby Language Translator";
   const char  pszE_Mail   [] = "complains_n_suggestions direct to gazlan@yandex.ru";

   printf("%s\n\n",pszCopyright);
   printf("%s\n\n",pszDescript);
   printf("Usage: blt.com [wildcards]\n\n");
   printf("%s\n",pszE_Mail);
}

/* ******************************************************************** **
** @@ main()
** @  Copyrt : 
** @  Author : 
** @  Modify :
** @  Update :
** @  Notes  :
** ******************************************************************** */

int main(int argc,char** argv)
{
   if ((argc < 2) || (argc > 4))
   {
      ShowHelp();
      return 0;
   }

   if (argc == 2 && ((!strcmp(argv[1],"?")) || (!strcmp(argv[1],"/?")) || (!strcmp(argv[1],"-?")) || (!stricmp(argv[1],"/h")) || (!stricmp(argv[1],"-h"))))
   {
      ShowHelp();
      return 0;
   }

   Init();
         
   FindFile   FF;

   FF.SetMask(argv[1]);

   int      iTotal = 0;

   while (FF.Fetch())
   {
      if ((FF._w32fd.dwFileAttributes | FILE_ATTRIBUTE_NORMAL) && !(FF._w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
         Proceed(FF._w32fd.cFileName);
      }
   }

   printf("\n[i]: Processed %d file(s) total.\n",iTotal);

   return 0;
}

/* ******************************************************************** **
** @@                   End of File
** ******************************************************************** */
