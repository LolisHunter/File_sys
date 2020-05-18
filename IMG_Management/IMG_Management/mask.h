#pragma once
const uint8_t zero = 0b0;
#define ZERO		zero

// Volume FLAGS
const uint8_t deflt = 0b00001001;
#define DEFAULT		deflt
#define RECYCLE		0b00000001
#define HIDDEN		0b00000010
#define PASSWORD	0b00000100
#define END			0b00001000

// Volume CONST
#define vENTRY_SIZE	10