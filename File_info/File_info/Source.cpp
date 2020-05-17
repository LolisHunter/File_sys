#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <string>
#include "flags.h"
using namespace std;
#pragma warning(disable : 4996)
#define DEBUG


#if defined (DEBUG)
static void DebugPrint(string pszMsg);
#define DEBUG_PRINT(pszMsg) DebugPrint(pszMsg)
#else
#define DEBUG_PRINT(pszMsg) NULL
#endif

void PrintHelp(string pszAppName);
void PrintStat(struct stat& fileInfo);
void Run(uint8_t flag, struct stat& fileInfo);
auto main(int argc, char* argv[]){

    bool flag[10];
    uint8_t c;
    struct stat fileInfo;

    if (argc == 2) {

    }
    else{
        if (argc != 3){
            PrintHelp(argv[0]);
            return 0;
        }
        c = Flag(argv[1]);
        //cout << argv[2] << endl;

        if (stat(argv[2], &fileInfo) != 0) {  // Use stat() to get the info
            char* buff = new char[200];
            cerr << "Error: " << strerror_s(buff, 200, errno) << '\n';
            cout << buff;
            delete[] buff;
            return(EXIT_FAILURE);
        }
    }

    Run(c, fileInfo);
    return EXIT_SUCCESS;
}

void PrintHelp(string pszAppName) {
    cout << "Usage: File_info -s <file name>   status file\n";
}

void PrintStat(struct stat& fileInfo) {
    //#define _S_IFMT   0xF000 // File type mask
    //#define _S_IFDIR  0x4000 // Directory
    //#define _S_IFCHR  0x2000 // Character special
    //#define _S_IFIFO  0x1000 // Pipe
    //#define _S_IFREG  0x8000 // Regular
    //#define _S_IREAD  0x0100 // Read permission, owner
    //#define _S_IWRITE 0x0080 // Write permission, owner
    //#define _S_IEXEC  0x0040 // Execute/search permission, owner
    cout << "Type:         : " << (((fileInfo.st_mode & S_IFMT) == S_IFDIR) ? "Directory\n" : "File\n"); // From sys/types.h
    cout << "Size          : " << fileInfo.st_size << '\n';               // Size in bytes
    cout << "Device        : " << (char)(fileInfo.st_dev + 'A') << '\n';  // Device number
    cout << "Created       : " << ctime(&fileInfo.st_ctime);         // Creation time
    cout << "Modified      : " << ctime(&fileInfo.st_mtime);         // Last modify time
}

void Run(uint8_t flag, struct stat& fileInfo) {
    switch (flag)
    {
    case 1:
        PrintStat(fileInfo);
        break;
    default:
        break;
    }
}

#if defined (DEBUG)
/*--------------------------------------------------------------------
The DebugPrint function prints a string to STDOUT.

Parameters
   pszMsg
      The string to be printed to STDOUT.
--------------------------------------------------------------------*/
void DebugPrint(string pszMsg) {
    printf_s("%s \n", pszMsg.c_str());
}
#endif
