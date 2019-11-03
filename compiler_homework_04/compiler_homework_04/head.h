#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "linkList.h"
#include "uthash-master/uthash.h"
#include "symbolTable.h"

#define TOKENSIZE 0x80
#define LINESIZE (TOKENSIZE*TOKENSIZE)
#define _NORMAL_EXIT 1
#define _ERROR -1
#define _ERROR_GET -2 

extern int lineNum;			// �к�
extern int isMinuLine;		// ���ʱlineNum�費��Ҫ-1



