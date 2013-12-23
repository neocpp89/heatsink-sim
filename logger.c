#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>

#include "logger.h"

int write_log(int loglevel, char *msg, ...)
{
    va_list ap;
    va_list apcopy;

    FILE *logfile = NULL;
    char hostname[256];
    char logfilename[260];
    char timebuffer[32];
    time_t current;
    struct tm *timeinfo;

    va_start(ap, msg);
    va_start(apcopy, msg);

    current = time(NULL);
    timeinfo = localtime(&current);
    strftime(timebuffer,32,"%X", timeinfo);
    if (gethostname(hostname, 256) != 0) {
        fprintf(stderr, "Error getting hostname.\n");
        return -1;
    }

    snprintf(logfilename, 260, "%s.log", hostname);
    logfile = fopen(logfilename, "a");
    if (logfile != NULL) {
        fprintf(logfile, "[%s %s:%d]: ", timebuffer, hostname, loglevel);
        vfprintf(logfile, msg, ap);
    }
    fprintf(stderr, "[%s %s:%d]: ", timebuffer, hostname, loglevel);
    vfprintf(stderr, msg, apcopy);

    fflush(logfile);
    fflush(stderr);

    return 0;
}

