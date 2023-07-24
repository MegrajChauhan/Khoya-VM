/*!
 * @file errors.h
 * @brief Error handling logic for the Khoya VM.
 *
 * This file defines the Error struct and provides functions for error handling and reporting.
 */

#ifndef K_ERROR
#define K_ERROR

#include <stdio.h>
#include <stdlib.h>

#define KNO_ERROR                        0  /*!< No error */
#define KINTERNAL_ERROR_MEM_ALLOC_FAILED 50 /*!< Internal error: Memory allocation failed */
#define KMEM_INIT_FAILED                 51 /*!< Memory initialization failed */
#define KMEM_ACCESS_OUT_OF_BOUNDS        52 /*!< Memory access out of bounds */
#define KMEM_MEMORY_SIZE_INCREASE_FAILED 53 /*!< Failed to increase memory size */
#define KSTACK_INITIALIZATION_FAILED     54 /*!< Stack initialization failed */
#define KSTACK_OVERFLOW                  55 /*!< Stack overflow */
#define KSTACK_UNDERFLOW                 56 /*!< Stack underflow */
#define KMEM_FULL                        57 /*!< Memory is full */
#define KDIV_BY_ZERO                     58 /*!< Division by zero*/

/*!
 * @struct Error
 * @brief Error structure for the Khoya VM.
 *
 * This struct represents an error and its details.
 */
typedef struct Error
{
    int __status;     /*!< Error status: 1 indicates an error has occurred, 0 means no error */
    int __err_code;   /*!< Error code */
    char *_err_msg;   /*!< Error message */
} Error;

/*!
 * @brief Converts the error code to a string representation.
 * @param error The Error object.
 * @return The string representation of the error code.
 */
static const char *err_code_to_str(Error *error)
{
    if (error->__status != 1)
        return "No error";
    switch (error->__err_code)
    {
    case 50:
    case 51:
    case 53:
    case 54:
        return "Internal error"; // the message after this " Memory allocation failed" should be in _err_msg
    case 52:
        return "Access Error";
    case 55:
        return "Stack Overflow"; 
    case 56:
        return "Stack Underflow";  
    case 57:
        return "Memory Full"; 
    case 58:
        return "Logical error";
    default:
        return "Unknown Error";
    }
}

/*!
 * @brief Sets the error flag and error message in the Error object. This is a wrapper macro.
 * @param err The Error object.
 * @param err_code The error code to set.
 * @param err_msg The error message.
 */
#define SET_ERR(err, err_code, err_msg) \
    do                                  \
    {                                   \
        err->__status = 1;              \
        err->__err_code = err_code;     \
        err->_err_msg = err_msg;        \
    } while (0);

/*!
 * @brief Unsets the error flag in the Error object. A wrapper macro
 * @param err The Error object.
 */
#define UNSET_ERR(err)               \
    do                               \
    {                                \
        err->__status = 0;           \
        err->__err_code = KNO_ERROR; \
    } while (0);

/*!
 * @brief Prints the error message.
 * @param err The Error object.
 */
void print_err(Error *err);

/*!
 * @brief Frees the memory allocated for the Error object.
 * @param err The Error object to free.
 */
void free_error(Error *err);

#endif
/*
This is a note: All of that conversion of error code to string to print it seems like a lot of overhead instead of directly printing the error
. Well, it is but i still haven't changed it because it helps in understanding what really is going on
*/