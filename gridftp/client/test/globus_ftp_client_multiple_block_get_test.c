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

/*
 * Multiple block get.
 *
 * Tests for errors handle more data blocks than
 * the size of the file we're transferring.
 */
#include "globus_ftp_client.h"
#include "globus_ftp_client_test_common.h"

static globus_mutex_t lock;
static globus_cond_t cond;
static globus_bool_t done;
static globus_bool_t error = GLOBUS_FALSE;

#define BUFSIZE 100

static
void
done_cb(
	void *					user_arg,
	globus_ftp_client_handle_t *		handle,
	globus_object_t *			err)
{
    char * tmpstr;

    if(err) tmpstr = " an";
    else    tmpstr = "out";

    if(err) { printf("done with%s error\n", tmpstr); 
              error = GLOBUS_TRUE; }

    globus_mutex_lock(&lock);
    done = GLOBUS_TRUE;
    globus_cond_signal(&cond);
    globus_mutex_unlock(&lock);
       
}

static
void
data_cb(
    void *					user_arg,
    globus_ftp_client_handle_t *		handle,
    globus_object_t *				err,
    globus_byte_t *				buffer,
    globus_size_t				length,
    globus_off_t				offset,
    globus_bool_t				eof)
{
    globus_result_t				result;
    fwrite(buffer, 1, length, stdout);
    if(!eof)
    {
	result = globus_ftp_client_register_read(handle,
					buffer,
					BUFSIZE,
					data_cb,
					0);
    }
    else
    {
        globus_libc_free(buffer);
    }
}

int main(int argc, char **argv)
{
    globus_ftp_client_handle_t			handle;
    globus_ftp_client_operationattr_t		attr;
    globus_byte_t *				buffer;
    globus_result_t				result;
    int						i;
    globus_ftp_client_handleattr_t		handle_attr;
    char *					src;
    char *					dst;

    globus_module_activate(GLOBUS_FTP_CLIENT_MODULE);
    globus_ftp_client_handleattr_init(&handle_attr);
    globus_ftp_client_operationattr_init(&attr);

    globus_mutex_init(&lock, GLOBUS_NULL);
    globus_cond_init(&cond, GLOBUS_NULL);

    test_parse_args(argc,
		    argv,
		    &handle_attr,
		    &attr,
		    &src,
		    &dst);

    globus_ftp_client_handle_init(&handle,  &handle_attr);

    done = GLOBUS_FALSE;
    result = globus_ftp_client_get(&handle,
				   src,
				   &attr,
				   GLOBUS_NULL,
				   done_cb,
				   0);
    if(result != GLOBUS_SUCCESS)
    {
	error = GLOBUS_TRUE;
	done = GLOBUS_TRUE;
    }
    else
    {
	for(i = 0; i < 5; i++)
	{
	    buffer = globus_libc_malloc(BUFSIZE);
	    globus_ftp_client_register_read(
		&handle,
		buffer,
		BUFSIZE,
		data_cb,
		0);
	}
    }
    globus_mutex_lock(&lock);
    while(!done)
    {
	globus_cond_wait(&cond, &lock);
    }
    globus_mutex_unlock(&lock);

    globus_ftp_client_handle_destroy(&handle);
    globus_module_deactivate_all();

    if(test_abort_count && error)
    {
	return 0;
    }
    return error;
}
