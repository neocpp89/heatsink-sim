#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef __LOGGER_H_
#define __LOGGER_H_
int write_log(int loglevel, char *msg, ...);
#endif

