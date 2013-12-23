#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef __LOGGER_H_
#define __LOGGER_H_

/* Functions as a printf, (msg is the format string), but prepends information
    and additionally writes to a log file. */
int write_log(int loglevel, char *msg, ...);
#endif

