#pragma once
const uint8_t zero = 0b0;
#define ZERO		zero
const uint64_t _zero = 0b0;
#define _ZERO		_zero
const uint64_t usize = 512;
#define	UNIT_SIZE	usize

// Volume FLAGS
const uint8_t deflt = 0b10000000;
#define DEFAULT		deflt
#define RECYCLE		0b10000000
#define USING		0b01000000 
#define PASSWORD	0b00100000
#define HIDDEN		0b00010000

// Volume CONST
#define vENTRY_SIZE	10

//entry flags
#define WRITABLE    0b00000000
#define DELETED     0b00000001
#define HIDDEN      0b00000010
#define PASSWORD    0b00000100
#define READ_ONLY   0b00001000
#define SUB_ENTRY   0b00010000
#define ROOT        0b00100000
#define END         0b01000000