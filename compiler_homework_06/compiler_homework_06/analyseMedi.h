#pragma once
#include "parsing.h"
#include "head.h"
#include "medi_hash.h"

FILE* fmedi;
FILE* fmips;

MediHashTable_t* global_pool;
MediHashGrandpa_t* func_pool;

int gpOffset;
int spOffset;
int fpOffset;

int init_analyseMedi();
int beforeAnalyse();
int analyseMedi();