#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <string>
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
int _tmain(int argc, char* argv[]){

    if (argc != 3)
    {
        PrintHelp(argv[0]);
        return 0;
    }

    struct stat fileInfo;

    if (argc < 2) {
        cout << "Usage: fileinfo <file name>\n";
        return(EXIT_FAILURE);
    }

    if (stat(argv[1], &fileInfo) != 0) {  // Use stat() to get the info
        char* buff = new char[200];
        cerr << "Error: " << strerror_s(buff, 200, errno) << '\n';
        cout << buff;
        delete[] buff;
        return(EXIT_FAILURE);
    }

    cout << "Type:         : ";
    if ((fileInfo.st_mode & S_IFMT) == S_IFDIR) { // From sys/types.h
        cout << "Directory\n";
    }
    else {
        cout << "File\n";
    }
    cout << "Size          : " <<
        fileInfo.st_size << '\n';               // Size in bytes
    cout << "Device        : " <<
        (char)(fileInfo.st_dev + 'A') << '\n';  // Device number
    cout << "Created       : " <<
        ctime(&fileInfo.st_ctime);         // Creation time
    cout << "Modified      : " <<
        ctime(&fileInfo.st_mtime);         // Last modify time

    cout << (char)(fileInfo.st_dev+'A') << endl;
    cout << fileInfo.st_mode << endl;
    cout << fileInfo.st_size << endl;
    cout << ctime(&fileInfo.st_mtime) << endl;
    cout << ctime(&fileInfo.st_ctime) << endl;
    return EXIT_SUCCESS;
}

void PrintHelp(string pszAppName) {
 //   cout << 
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
