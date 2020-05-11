#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <string>
#pragma warning(disable : 4996)

int main(int argc, char** argv)
{
    struct stat fileInfo;

    if (argc < 2) {
        std::cout << "Usage: fileinfo <file name>\n";
        return(EXIT_FAILURE);
    }

    if (stat(argv[1], &fileInfo) != 0) {  // Use stat() to get the info
        char* buff = new char[200];
        std::cerr << "Error: " << strerror_s(buff, 200, errno) << '\n';
        std::cout << buff;
        delete[] buff;
        return(EXIT_FAILURE);
    }

    std::cout << "Type:         : ";
    if ((fileInfo.st_mode & S_IFMT) == S_IFDIR) { // From sys/types.h
        std::cout << "Directory\n";
    }
    else {
        std::cout << "File\n";
    }
    std::cout << "Size          : " <<
        fileInfo.st_size << '\n';               // Size in bytes
    std::cout << "Device        : " <<
        (char)(fileInfo.st_dev + 'A') << '\n';  // Device number
    std::cout << "Created       : " <<
        ctime(&fileInfo.st_ctime);         // Creation time
    std::cout << "Modified      : " <<
        ctime(&fileInfo.st_mtime);         // Last mod time

    //std::string temp(ctime(&fileInfo.st_ctime));
    int x = fileInfo.st_ctime;
    //std::cout << x;
    fileInfo.st_ctime = 0;
    std::cout << "Created again       : " <<
        ctime(&fileInfo.st_ctime);
    fileInfo.st_ctime = x;
    std::cout << "Created again       : " <<
        ctime(&fileInfo.st_ctime);
    return 0;
}