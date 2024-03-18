#ifndef UTIL_H_
#define UTIL_H_

/**
 * Compares two processes based on their arrival times for sorting purposes.
 *
 * This function is designed to be used with sorting algorithms that require
 * a comparator, for our usecase qsort. It takes pointers to two process_t structures
 * and compares their arrival times. The goal is to sort processes in
 * ascending order of their arrival times, meaning earlier processes come first.
 * 
 * @param a A void pointer to the first process for comparison.
 * @param b A void pointer to the second process for comparison.
 * @return An integer less than, equal to, or greater than zero if the arrival time
 *         of the first process is found, respectively, to be less than, to match,
 *         or be greater than the arrival time of the second process.
 */
extern int arrival_time(const void* a, const void* b);

#endif /* UTIL_H_ */