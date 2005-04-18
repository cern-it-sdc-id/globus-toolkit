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

#ifndef GLOBUS_COMMON_PRIORITY_Q_H
#define GLOBUS_COMMON_PRIORITY_Q_H

/********************************************************************
 *
 *  This file defines the a priority queue for globus
 *  It is implemented using a binary heap (minheap) and does NOT have
 *  a fifo fallback for like priorities.  If you need fifo fallback,
 *  you should use a compound priority with the primary priority being
 *  the 'real' priority and the secondary being a serial number.
 *
 ********************************************************************/

#include "globus_common_include.h"
#include "globus_memory.h"


EXTERN_C_BEGIN

/*
 * if priority_1 comes after priority_2, return > 0
 * else if priority_1 comes before priority_2, return < 0
 * else return 0
 */
 
typedef int (*globus_priority_q_cmp_func_t)(
    void *                                  priority_1,
    void *                                  priority_2);

typedef struct globus_priority_q_s
{
    struct globus_l_priority_q_entry_s **   heap;
    int                                     next_slot;
    size_t                                  max_len;
    globus_memory_t                         memory;
    globus_priority_q_cmp_func_t            cmp_func;
} globus_priority_q_t;

int
globus_priority_q_init(
    globus_priority_q_t *               priority_q,
    globus_priority_q_cmp_func_t        cmp_func);

int
globus_priority_q_destroy(
    globus_priority_q_t *               priority_q);

globus_bool_t
globus_priority_q_empty(
    globus_priority_q_t *               priority_q);

int
globus_priority_q_size(
    globus_priority_q_t *               priority_q);

int
globus_priority_q_enqueue(
    globus_priority_q_t *               priority_q,
    void *                              datum,
    void *                              priority);

void *
globus_priority_q_remove(
    globus_priority_q_t *               priority_q,
    void *                              datum);

/*
 * it is acceptable to modify the priority already stored within the queue
 * before making this call.  The old priority is not looked at
 */
void *
globus_priority_q_modify(
    globus_priority_q_t *               priority_q,
    void *                              datum,
    void *                              new_priority);

void *
globus_priority_q_dequeue(
    globus_priority_q_t *               priority_q);

void *
globus_priority_q_first(
    globus_priority_q_t *               priority_q);

void *
globus_priority_q_first_priority(
    globus_priority_q_t *               priority_q);

EXTERN_C_END

#endif /* GLOBUS_COMMON_PRIORITY_Q_H */
