#ifndef STAT_DATA_DUMP_H
#define STAT_DATA_DUMP_H

#include "stat_data.h"

typedef struct stat_data_dump
{
    stat_data* array;
    unsigned long num_elements;
} stat_data_dump;

#endif // STAT_DATA_DUMP_H