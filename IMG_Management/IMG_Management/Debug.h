#pragma once
#include <string>
#include <iostream>
using namespace std;
#define DEBUG
#if defined (DEBUG)
static void DebugPrint(string pszMsg);
#define DEBUG_PRINT(pszMsg) DebugPrint(pszMsg)
#else
#define DEBUG_PRINT(pszMsg) NULL
#endif

#if defined (DEBUG)
/*--------------------------------------------------------
The DebugPrint function prints a string to STDOUT.

Parameters
   pszMsg
      The string to be printed to STDOUT.
--------------------------------------------------------*/
void DebugPrint(string plain_text) {
	cout << "[Log] : " << plain_text << endl;
}
#endif