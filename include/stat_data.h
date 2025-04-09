#ifndef STAT_DATA_H
#define STAT_DATA_H

typedef struct stat_data
{
    long id;
    int count;
    float cost;
    unsigned int primary : 1;
    unsigned int mode : 3;
} stat_data;

#endif // STAT_DATA_H