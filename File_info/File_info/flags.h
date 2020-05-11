#pragma once
#include <iostream>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>
#include <string>
using namespace std;
#pragma warning(disable : 4996)

uint8_t Flag(char*& flag) {
	switch (flag[0])
	{
	case '-':
		if (strlen(flag) > 2) {
			break;
		}
		switch (flag[1])
		{
		case 's':
			return 1;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return 0;
}