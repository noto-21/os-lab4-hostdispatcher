/* string_utils.h */

/**
 * This header file contains declarations for functions that perform
 * trimming operations on strings. Trimming is the process of removing
 * leading and trailing whitespace characters from a string.
 * 
 * The functions provided are:
 * - ltrim: Trims leading whitespace.
 * - rtrim: Trims trailing whitespace.
 * - trim: Trims both leading and trailing whitespace.
 *
 * Usage of these functions is recommended when processing strings that
 * might contain unwanted whitespace, particularly user input, or text
 * retrieved from files or networks.
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * Trims leading whitespace characters from a string.
 *
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the first non-whitespace character in the string.
 */
char* ltrim(char *s);

/**
 * Trims trailing whitespace characters from a string.
 *
 * @param s Pointer to the string to be trimmed.
 * @return The same pointer passed in as the parameter, now pointing to a string
 *         with trailing whitespace removed.
 */
char* rtrim(char *s);

/**
 * Trims both leading and trailing whitespace characters from a string.
 *
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the trimmed string. If the string is all whitespace,
 *         the returned pointer will point to the terminating null character.
 */
char* trim(char *s);

#endif /* STRING_UTILS_H */
