#include "string_utils.h"

/**
 * Trims leading whitespace characters from a string.
 * 
 * This function moves the pointer to the first non-whitespace character
 * in the string, effectively "trimming" the whitespace from the start.
 * 
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the first non-whitespace character in the string.
 */
char* ltrim(char *s) {
    while (isspace((unsigned char)*s)) {
        s++;
    }
    return s;
}

/**
 * Trims trailing whitespace characters from a string.
 * 
 * This function iterates from the end of the string moving backwards
 * and places a null terminator '\0' after the last non-whitespace
 * character, effectively "trimming" the whitespace from the end.
 * 
 * @param s Pointer to the string to be trimmed.
 * @return The same pointer passed in as the parameter, now pointing to a string
 *         with trailing whitespace removed.
 */
char* rtrim(char *s) {
    char* back = s + strlen(s);
    while (isspace((unsigned char)*--back));
    *(back + 1) = '\0';
    return s;
}

/**
 * Trims both leading and trailing whitespace characters from a string.
 * 
 * This function first trims the leading whitespace by calling `ltrim`
 * and then trims the trailing whitespace by calling `rtrim`.
 * 
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the trimmed string. If the string is all whitespace,
 *         the returned pointer will point to the terminating null character.
 */
char* trim(char *s) {
    return rtrim(ltrim(s));  
}
