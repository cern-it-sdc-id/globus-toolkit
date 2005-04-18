/*
 * Portions of this file Copyright 1999-2005 University of Chicago
 * Portions of this file Copyright 1999-2005 The University of Southern California.
 *
 * This file or a portion of this file is licensed under the
 * terms of the Globus Toolkit Public License, found at
 * http://www.globus.org/toolkit/download/license.html.
 * If you redistribute this file, with or without
 * modifications, you must include this notice in the file.
 */

#ifndef GLOBUS_LOGGING_H
#define GLOBUS_LOGGING_H 1

#include "globus_common_include.h"
#include "globus_time.h"

#define GLOBUS_LOGGING_INLINE           0x08000000

typedef struct globus_l_logging_handle_s * globus_logging_handle_t;

typedef enum
{
    GLOBUS_LOGGING_ERROR_PARAMETER,
    GLOBUS_LOGGING_ERROR_ALLOC
} globus_logging_error_type_t;

typedef void
(*globus_logging_open_func_t)(
    void *                              user_arg);

typedef void
(*globus_logging_write_func_t)(
    globus_byte_t *                     buf,
    globus_size_t                       length,
    void *                              user_arg);

typedef void
(*globus_logging_close_func_t)(
    void *                              user_arg);

typedef void
(*globus_logging_header_func_t)(
    char *                              buffer,
    globus_size_t *                     buf_len);

typedef struct globus_logging_module_s
{
    globus_logging_open_func_t          open_func;
    globus_logging_write_func_t         write_func;
    globus_logging_close_func_t         close_func;
    globus_logging_header_func_t        header_func;
} globus_logging_module_t;


globus_result_t
globus_logging_init(
    globus_logging_handle_t *           out_handle,
    globus_reltime_t *                  flush_period,
    int                                 buffer_length,
    int                                 log_type,
    globus_logging_module_t *           module,
    void *                              user_arg);

globus_result_t
globus_logging_write(
    globus_logging_handle_t             handle,
    int                                 type,
    const char *                        fmt,
    ...);

globus_result_t
globus_logging_vwrite(
    globus_logging_handle_t             handle,
    int                                 type,
    const char *                        fmt,
    va_list                             ap);

globus_result_t
globus_logging_flush(
    globus_logging_handle_t             handle);

globus_result_t
globus_logging_destroy(
    globus_logging_handle_t             handle);

extern globus_logging_module_t          globus_logging_stdio_module;
extern globus_logging_module_t          globus_logging_syslog_module;

#endif
