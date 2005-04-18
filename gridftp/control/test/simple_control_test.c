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

#include "globus_ftp_control_test.h"

#define TEST_ITERATIONS  8

void
simple_control_response_callback(
    void *                                      callback_arg,
    globus_ftp_control_handle_t *               handle,
    globus_object_t *                           error,
    globus_ftp_control_response_t *             ftp_response);

void
block_response_callback(
    void *                                      callback_arg,
    globus_ftp_control_handle_t *               handle,
    globus_object_t *                           error,
    globus_ftp_control_response_t *             ftp_response);

static globus_ftp_control_test_monitor_t        monitor;

globus_bool_t
simple_control_test(
    globus_ftp_control_handle_t *               handle)
{
    int                                         ctr;
    globus_result_t                             result;

    globus_mutex_init(&monitor.mutex, GLOBUS_NULL);
    globus_cond_init(&monitor.cond, GLOBUS_NULL);
    monitor.done = GLOBUS_FALSE;
    monitor.rc = GLOBUS_FALSE;

    for(ctr = 0; ctr < TEST_ITERATIONS; ctr++)
    {
        /* try to send a command  and wait for a response */
        monitor.done = GLOBUS_FALSE;
        result = globus_ftp_control_send_command(
                     handle,
                     "PWD\r\n",
                     simple_control_response_callback,
                     GLOBUS_NULL);
        if(result != GLOBUS_SUCCESS)
        {   
            return GLOBUS_FALSE;
        }
        globus_mutex_lock(&monitor.mutex);
        {
            while(!monitor.done)
            { 
                globus_cond_wait(&monitor.cond, &monitor.mutex);
            }
        }
        globus_mutex_unlock(&monitor.mutex);
    }

    return monitor.rc;
}

void
simple_control_response_callback(
    void *                                      callback_arg,
    globus_ftp_control_handle_t *               handle,
    globus_object_t *                           error,
    globus_ftp_control_response_t *             ftp_response)
{
    if(ftp_response->code == 257)
    {
        globus_mutex_lock(&monitor.mutex);
        {
            monitor.done = GLOBUS_TRUE;
            monitor.rc = GLOBUS_TRUE;
            globus_cond_signal(&monitor.cond);
        }
        globus_mutex_unlock(&monitor.mutex);
    }
}
