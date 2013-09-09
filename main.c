#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"

int main(int argc, char **argv)
{
    write_log(1, "daddy would you like some sausage? %x %c\n", 29, 'y');
    return 0;
}

