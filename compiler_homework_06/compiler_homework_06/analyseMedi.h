#pragma once
#include "parsing.h"
#include "head.h"

FILE* fmedi;
FILE* fmips;

SymbolTable_t* global_vars;
SymbolTable_t* local_vars;

int gpAddr;
int gpOffset;
int fpAddr;
int fpOffset;
int spAddr;
int spOffset;

int init_analyseMedi();
int beforeAnalyse();
int analyseMedi();