#ifndef __STRING_UTIL_H__
#define __STRING_UTIL_H__

#include <stdio.h>

/**
 * Reads a line from the given file and returns it as a dynamically allocated
 * string.  The returned string does not include the newline character
 * and and trailing carriage returns are removed.  If there is no data to read
 * before EOF or if there is a memory allocation error then the returned
 * pointer is NULL.  Otherwise, it is the caller's responsibility to
 * free the returned string.
 *
 * @param in an input file
 */
char *read_line(FILE *in);

#endif
