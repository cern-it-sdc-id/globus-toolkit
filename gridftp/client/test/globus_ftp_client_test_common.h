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

void
test_parse_args(int argc, 
		char *argv[],
		globus_ftp_client_handleattr_t    * handleattr,
		globus_ftp_client_operationattr_t * operationattr,
		char **src,
		char **dst);

void
test_remove_arg(int *argc, char **argv, int *start, int num_of_options);

extern int test_abort_count;

