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

#if !defined(TEST_COMMON_H)
#define TEST_COMMON_H 1

#include "globus_xio.h"
#include "test_common.h"
#include "globus_common.h"
#include "globus_xio_test_transport.h"
#include "globus_xio_test_transport.h"

typedef struct test_info_s
{
    int                                     write_count;
    int                                     read_count;

    /* always points to nothing */
    globus_byte_t *                         buffer;
    globus_size_t                           buffer_length;
    globus_size_t                           chunk_size;

    globus_size_t                           nwritten;
    globus_size_t                           nread;
    globus_size_t                           total_write_bytes;
    globus_size_t                           total_read_bytes;

    int                                     failure;
    int                                     closed;
    globus_bool_t                           write_done;
    globus_bool_t                           read_done;

    globus_bool_t                           server;

    globus_reltime_t                        delay;

    globus_mutex_t                          mutex;
} test_info_t;

extern test_info_t                         globus_l_test_info;

void
failed_exit(
    char *                                  fmt,
    ...);
void
test_res(
    int                                     location,
    globus_result_t                         res,
    int                                     line,
    char *                                  filename);

void
parse_parameters(
    int                                     argc,
    char **                                 argv,
    globus_xio_stack_t                      stack,
    globus_xio_attr_t                       attr);

int
unload_main(
    int                                     argc,
    char **                                 argv);

int
read_barrier_main(
    int                                     argc,
    char **                                 argv);

int
close_barrier_main(
    int                                     argc,
    char **                                 argv);

int
close_barrier2_main(
    int                                     argc,
    char **                                 argv);

int
framework_main(
    int                                     argc,
    char **                                 argv);

int
timeout_main(
    int                                     argc,
    char **                                 argv);

int
cancel_main(
    int                                     argc,
    char **                                 argv);

int
attr_main(
    int                                     argc,
    char **                                 argv);

int
close_cancel_main(
    int                                     argc,
    char **                                 argv);

int
space_main(
    int                                     argc,
    char **                                 argv);

int
server2_main(
    int                                     argc,
    char **                                 argv);

int
block_barrier_main(
    int                                     argc,
    char **                                 argv);

int
blocking_dd_main(
    int                                     argc,
    char **                                 argv);

int
stack_main(
    int                                     argc,
    char **                                 argv);

void
test_common_end();


#endif
