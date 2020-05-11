#include <iostream>
#include <string>
#include "img.h"
using namespace std;
#define DEBUG
#define MAIN

#if defined (DEBUG)
static void DebugPrint(string pszMsg);
#define DEBUG_PRINT(pszMsg) DebugPrint(pszMsg)
#else
#define DEBUG_PRINT(pszMsg) NULL
#endif

#ifdef TEST
void main() {

}
#endif // TEST


#ifdef MAIN
int main(int agrc, char* agrv[]) {
    string a = "a.img";
    bool temp = CreateImageFile(a, 20);
    DEBUG_PRINT("aaaaaaa");
    return temp;
}
#endif // MAIN

#if defined (DEBUG)
/*--------------------------------------------------------------------
The DebugPrint function prints a string to STDOUT.

Parameters
   pszMsg
      The string to be printed to STDOUT.
--------------------------------------------------------------------*/
void DebugPrint(string pszMsg){
    printf_s("%s \n", pszMsg.c_str());
}
#endif
