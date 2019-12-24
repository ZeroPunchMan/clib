#include "version.h"
#include "flash_layout.h"

#define VERSION_ADDR  (APP_START_ADDR + 1024 * 5)

const Version_t version __attribute__((at(VERSION_ADDR)))={
    .head[0] = 'T',
    .head[1] = 'E',
    .head[2] = 'S',
    .head[3] = 'T',
    .major = 0,
    .minor = 0,
    .revision = 1,};

    
