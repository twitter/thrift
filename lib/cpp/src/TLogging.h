#ifndef _THRIFT_LOGGING_H
#define _THRIFT_LOGGING_H 1

/**
 * Contains utility macros for debugging and logging.
 *
 * @author Aditya Agarwal
 */

#include "time.h"
#include <stdint.h>

/**
 * GLOBAL_DEBUGGING_LEVEL = 0: all debugging turned off, debug macros undefined
 * GLOBAL_DEBUGGING_LEVEL = 1: all debugging turned on  
 */
#define GLOBAL_DEBUGGING_LEVEL 0


/**
 * GLOBAL_LOGGING_LEVEL = 0: all logging turned off, logging macros undefined
 * GLOBAL_LOGGING_LEVEL = 1: all logging turned on  
 */
#define GLOBAL_LOGGING_LEVEL   1


/** 
 * Standard wrapper around fprintf what will prefix the file name and line
 * number to the line. Uses GLOBAL_DEBUGGING_LEVEL to control whether it is
 * turned on or off.
 *
 * @param format_string 
 */
#if GLOBAL_DEBUGGING_LEVEL > 0
  #define DEBUG(format_string,...)                                        \
    if (GLOBAL_DEBUGGING_LEVEL > 0) {                                     \
      fprintf(stderr,"[%s,%d] " #format_string " \n", __FILE__, __LINE__,##__VA_ARGS__); \
  } 
#else
  #define DEBUG(format_string,...)
#endif


/** 
 * analagous to DEBUG but also prints the time 
 *
 * @param string  format_string input: printf style format string
 */
#if GLOBAL_DEBUGGING_LEVEL > 0
  #define DEBUG_T(format_string,...)                                      \
    {                                                                     \
      if (GLOBAL_DEBUGGING_LEVEL > 0) {                                   \
        time_t now;                                                       \
        char dbgtime[26] ;                                                \
        time(&now);                                                       \
        ctime_r(&now, dbgtime);                                           \
        dbgtime[24] = '\0';                                               \
        fprintf(stderr,"[%s,%d] [%s] " #format_string " \n", __FILE__, __LINE__,dbgtime,##__VA_ARGS__); \
      }                                                                   \
    }
#else
  #define DEBUG_T(format_string,...)
#endif


/** 
 * analagous to DEBUG but uses input level to determine whether or not the string
 * should be logged.
 *
 * @param int     level: specified debug level
 * @param string  format_string input: format string
 */
#define DEBUG_L(level, format_string,...)                               \
  if ((level) > 0) {                                                    \
    fprintf(stderr,"[%s,%d] " #format_string " \n", __FILE__, __LINE__,##__VA_ARGS__); \
  }


/** 
 * Explicit error logging. Prints time, file name and line number
 *
 * @param string  format_string input: printf style format string
 */
#define ERROR(format_string,...)                                        \
  {                                                                     \
    time_t now;                                                         \
    char dbgtime[26] ;                                                  \
    time(&now);                                                         \
    ctime_r(&now, dbgtime);                                             \
    dbgtime[24] = '\0';                                                 \
    fprintf(stderr,"[%s,%d] [%s] ERROR: " #format_string " \n", __FILE__, __LINE__,dbgtime,##__VA_ARGS__); \
  }


/** 
 * Analagous to ERROR, additionally aborting the process.
 * WARNING: macro calls abort(), ending program execution
 *
 * @param string  format_string input: printf style format string
 */
#define ERROR_ABORT(format_string,...)                                  \
  {                                                                     \
    time_t now;                                                         \
    char dbgtime[26] ;                                                  \
    time(&now);                                                         \
    ctime_r(&now, dbgtime);                                             \
    dbgtime[24] = '\0';                                                 \
    fprintf(stderr,"[%s,%d] [%s] ERROR: Going to abort " #format_string " \n", __FILE__, __LINE__,dbgtime,##__VA_ARGS__); \
    exit(1);                                                            \
  }


/** 
 * Log input message
 *
 * @param string  format_string input: printf style format string
 */
#if GLOBAL_LOGGING_LEVEL > 0
  #define LOG_OPER(format_string,...)                                     \
    {                                                                     \
      if (GLOBAL_LOGGING_LEVEL > 0) {                                     \
        time_t now;                                                       \
        char dbgtime[26] ;                                                \
        time(&now);                                                       \
        ctime_r(&now, dbgtime);                                           \
        dbgtime[24] = '\0';                                               \
        fprintf(stderr,"[%s] " #format_string " \n", dbgtime,##__VA_ARGS__); \
      }                                                                   \
    } 
#else
  #define LOG_OPER(format_string,...)
#endif

#endif	// _THRIFT_LOGGING_H