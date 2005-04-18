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

/**********************************************************************
globus_utp.h

Public declarations for the Unnamed Timing Package (UTP).
**********************************************************************/

#ifndef GLOBUS_UTP_INCLUDE
#define GLOBUS_UTP_INCLUDE

#include "globus_common.h"

#ifndef EXTERN_C_BEGIN
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
#endif

EXTERN_C_BEGIN

/**********************************************************************
Publicly-accessible functions.
**********************************************************************/

extern int
globus_utp_init(unsigned numTimers, int mode);

extern void
globus_utp_write_file(const char *outFilename);

extern void
globus_utp_set_attribute(const char *keyStr,
			 const char *keyArg,
			 const char *valueStr,
			 ...);

extern void
globus_utp_start_timer(unsigned timerNumber);

extern void
globus_utp_stop_timer(unsigned timerNumber);

extern void
globus_utp_reset_timer(unsigned timerNumber);

extern void
globus_utp_disable_timer(unsigned timerNumber);

extern void
globus_utp_enable_timer(unsigned timerNumber);

extern void
globus_utp_disable_all_timers(void);

extern void
globus_utp_enable_all_timers(void);

extern const char *
globus_utp_name_timer(unsigned timerNumber,
		      const char *nameStr,
		      ...);

extern void
globus_utp_get_accum_time(unsigned timerNumber,
			  double *time,
			  int *precision);


/**********************************************************************
Publicly-accessible definitions.
**********************************************************************/

/*
 * Initialization modes.
 */
#define GLOBUS_UTP_MODE_SHARED  0	/* Pick one of these. */
#define GLOBUS_UTP_MODE_PRIVATE 1

/*
 * Strings for timer names and attribute keys and
 * values must be shorter than this.
 */
#define GLOBUS_UTP_MAX_NAME_LENGTH 240

#define GLOBUS_UTP_DEFAULT_TIMER_NAME ""

/******************************************************************************
			  Module activation structure
******************************************************************************/
extern globus_module_descriptor_t	globus_i_utp_module;

#define GLOBUS_UTP_MODULE (&globus_i_utp_module)


EXTERN_C_END
#endif /* #ifndef GLOBUS_UTP_INCLUDE */

