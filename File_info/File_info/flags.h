#pragma once
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