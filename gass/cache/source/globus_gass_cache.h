/******************************************************************************
globus_gass_cache.h
 
Description:
    Header of the GASS CACHE MANAGEMENT API.

    CVS Information:
 
    $Source$
    $Date$
    $Revision$
    $Author$

******************************************************************************/
#ifndef _GLOBUS_GASS_INCLUDE_GLOBUS_GASS_CACHE_H_
#ifndef SWIG
#define _GLOBUS_GASS_INCLUDE_GLOBUS_GASS_CACHE_H_

#ifndef EXTERN_C_BEGIN
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
#endif

#include <sys/param.h>

#include "globus_common.h"
#include "globus_gass_common.h"

EXTERN_C_BEGIN


/******************************************************************************
Codes returned by globus_gass_cache module
******************************************************************************/

#define GLOBUS_GASS_CACHE_ADD_NEW                       1
#define GLOBUS_GASS_CACHE_URL_NOT_FOUND                 2
#define GLOBUS_GASS_CACHE_ADD_EXISTS                    3

#define GLOBUS_GASS_CACHE_ERROR_NO_HOME                -1
#define GLOBUS_GASS_CACHE_ERROR_CAN_NOT_CREATE         -2
#define GLOBUS_GASS_CACHE_ERROR_NAME_TOO_LONG          -3
#define GLOBUS_GASS_CACHE_ERROR_LOCK_ERROR             -4
#define GLOBUS_GASS_CACHE_ERROR_LOCK_TIME_OUT          -5 /* not used in a first impl. */
#define GLOBUS_GASS_CACHE_ERROR_OPEN_STATE             -6
#define GLOBUS_GASS_CACHE_ERROR_STATE_F_CORRUPT        -7
#define GLOBUS_GASS_CACHE_ERROR_NO_MEMORY              -8
#define GLOBUS_GASS_CACHE_ERROR_CAN_NOT_CREATE_DATA_F  -9
#define GLOBUS_GASS_CACHE_ERROR_URL_NOT_FOUND          -10 /* only for "done" or delete */
#define GLOBUS_GASS_CACHE_ERROR_CAN_NOT_DEL_LOCK       -11
#define GLOBUS_GASS_CACHE_ERROR_WRONG_TAG              -12
#define GLOBUS_GASS_CACHE_ERROR_ALREADY_DONE           -13
#define GLOBUS_GASS_CACHE_ERROR_CAN_NOT_WRITE          -14
#define GLOBUS_GASS_CACHE_ERROR_CAN_NOT_READ           -15
#define GLOBUS_GASS_CACHE_ERROR_CAN_NOT_DELETE_DATA_F  -16
#define GLOBUS_GASS_CACHE_ERROR_CACHE_NOT_OPENED       -17
#define GLOBUS_GASS_CACHE_ERROR_CACHE_ALREADY_OPENED   -18
#define GLOBUS_GASS_CACHE_ERROR_INVALID_PARRAMETER     -19
#define GLOBUS_GASS_CACHE_ERROR_INVALID_VERSION        -20


#if !defined(PATH_MAX) && defined(MAXPATHLEN)
#   define PATH_MAX MAXPATHLEN
#endif

/******************************************************************************
Other definitions
******************************************************************************/

#define COMMENT_LENGHT       80

#define GLOBUS_GASS_CACHE_STATE_FILE_FORMAT_VERSION     1

#endif

#ifdef SWIG
%subsection "Structures", before, pre, chop_left=0,chop_top=2,chop_bottom=1
#endif

/******************************************************************************
   Structure: globus_gass_cache_t

   Data structure used to store informations concerning an open cache
   directory. This structure MUST NOT be modified directly, but passed to
   the globus_gass_cache functions
******************************************************************************/

typedef struct
{
#ifndef SWIG
    void*          init;   /* dirty hack to know if this cache has
			      been opened/init.    */
    char           comment[COMMENT_LENGHT];
    int            version;  /* version number read out of the state file */
    
    char           cache_directory_path[PATH_MAX+1];
    
    /* just to not have to biuld them every time: */
    char           state_file_path[PATH_MAX+1];
    int            state_file_fd;
    char           state_file_lock_path[PATH_MAX+1];   
    char           temp_file_path[PATH_MAX+1];
    int            temp_file_fd;
    FILE*          log_FILE;
    int            nb_entries;
/* endif SWIG */
#endif
} globus_gass_cache_t;      /* cache handle */

/******************************************************************************
   Structure: globus_gass_cache_tag_t

   Define an entry for the array of tag returned by  "globus_gass_cache_list()"
******************************************************************************/
typedef struct
{
#ifdef SWIG
    globus_gass_cache_tag_t();
    ~globus_gass_cache_tag_t();
/* endif SWIG */
#endif
    char *           tag;
    int              count;
} globus_gass_cache_tag_t;
/******************************************************************************
   Structure: globus_gass_cache_entry_t

  Define an entry of the cache, as return by the function "globus_gass_cache_list()"
******************************************************************************/
typedef struct
{
#ifdef SWIG
    globus_gass_cache_entry_t();
    ~globus_gass_cache_entry_t();
/* endif SWIG */
#endif
    char *                    url;
    char *                    filename;
    unsigned long             timestamp;  /* modification timestamp          */
                                          /* (seconds since the epoch)       */
    char *                    lock_tag;   /* The tag that has acquired this  */
                                          /* entry's lock, or GLOBUS_NULL    */
                                          /* if the entry is unlocked        */
    unsigned long             num_tags;   /* number of tags in the tag array */
    globus_gass_cache_tag_t*   tags;             /* array of tags for this entry    */
} globus_gass_cache_entry_t;

/******************************************************************************

  FUNCTIONS

******************************************************************************/

#ifdef SWIG
%subsection "Functions", before, pre, chop_left=0,chop_top=2,chop_bottom=1
#endif

/******************************************************************************
   Function: globus_gass_cache_open()

   Description:
    Open the cache specified by the cache_directory_path argument, and return
    a cache handle that can be used in subsequent cache calls. 

    If cache_directory_path is NULL, then use the value contained in the
    GLOBUS_GASS_CACHE_DEFAULT environment variable if it is defined,
    otherwise use ~/.globus_gass_cache.

    The cache_directory_path must be a directory. If it is a file, this call
    will fail with a non-0 return value.

    If the specified directory does not exist, then this call will create the
    directory.

   Parameters: input:    
                cache_directory_path: Path to the cache directory to open.
		    Can be NULL (see above)
		cache_handle->is_init: checked and return an error if 
		    cache_handle has already been used.
		    
	    output: 
	      	cache_handle: Structure containning all the necessary
		information to access the cache (file names, descriptor,...)
		(see globus_gass_gache.h) Some files are also opened:
		globus_gass_cache_close() must be called subsequently to close those
		files.
		
   Returns:    
            BLOBUS_SUCCESS or error code:	
            GLOBUS_GASS_CACHE_ERROR_CACHE_ALREADY_OPENED
            GLOBUS_GASS_CACHE_ERROR_NAME_TOO_LONG if the cache directory path is
	        too long
	    GLOBUS_GASS_CACHE_ERROR_NO_HOME if cache_directory_path is NULL and
	        the env. variable GLOBUS_GASS_CACHE_DEFAULT is empty and
		the env. variable $HOME is not defined !
	    GLOBUS_GASS_CACHE_ERROR_CAN_NOT_CREATE if the cache directory or any
	         necessary file can not be created.
	 
******************************************************************************/
extern int 
globus_gass_cache_open(char*          cache_directory_path,
		globus_gass_cache_t*  cache_handle);




/******************************************************************************
Function: globus_gass_cache_close()

Description:
  Close (NOT delete) a previously opened cache:
  - close the opened files and 
  - free the memory allocated for the cache_handle.
  - mark the handle as "not initialized".
  
  Parameters: input:    
		cache_handle: Handler to the opened cahe directory to use.    
		    
	    output: 
	      	cache_handle->is_init set to "not initialized" and all the
		    files opened bu globus_gass_cache_open are closed . 

Returns:    GLOBUS_SUCCESS or error code:
            GLOBUS_GASS_CACHE_ERROR_CACHE_NOT_OPENED
	 
******************************************************************************/
extern int
globus_gass_cache_close(globus_gass_cache_t *  cache_handle);

/******************************************************************************
Function: globus_gass_cache_add()

Description: Create a new cache file or add a tag on it.

    If the URL is already in the cache but is locked, then this call will block
    until the cache entry is unlocked, then will proceed with the subsequent
    operations.

    If the URL is already in the cache and unlocked, then add the tag to the
    cache entry's tag list, return the local cache filename in *local_filename,
    return the entry's current timestamp in *timestamp, lock the cache entry,
    and return GLOBUS_GASS_CACHE_ADD_EXISTS.

    If the URL is not in the cache, and create==GLOBUS_TRUE, then create a new
    unique empty local cache file, add it to the cache with the specified tag,
    return the filename in *local_filename, return *timestamp set to
    GLOBUS_GASS_TIMESTAMP_UNKNOWN, lock the cache entry, and
    return GLOBUS_GASS_CACHE_ADD_NEW.

    If the URL is not in the cache, and create==GLOBUS_FALSE, then do not
    add it to the cache, and return GLOBUS_GASS_CACHE_URL_NOT_FOUND. 

    If this function returns GLOBUS_GASS_CACHE_ADD_EXISTS or GLOBUS_GASS_CACHE_ADD_NEW,
    then globus_gass_cache_add_done() or globus_gass_cache_delete() must be subsequently
    called to unlock the cache entry. 

    Subsequent calls to globus_gass_cache_add() and globus_gass_cache_delete_start() on the
    same cache and url, made either from this process or another, will block
    until the cache entry is unlocked.

    If tag==NULL, then a tag with the value "null" will be added to the cache
    entry's tag list.

    The same tag can be used multiple times, in which case this tag will be
    added to the entry's tag list multiple times.

    Note: It is recommended that proglobus_grams started via GLOBUS_GRAM pass a tag value
    of getenv("GLOBUS_GRAM_JOB_CONTACT"), since upon completion of a job GLOBUS_GRAM will
    automatically cleanup entries with this tag.

    Important Note: the local_filename MUST be free by the user in a
    subsequent operation, using globus_free()
    
Parameters: input:    
                cache_handle: Handler to the opened cahe directory to use.
		url : url of the file to be cached. It is used as the main
		    key to the cache entries.
		tag : tag specifying which job is/are using the cache. This
		    is usually the GLOBUS_GRAM_JOB_CONTACT. Can be NULL or empty; the
		    tag "null" is then used.
		create: Tells if the cache entry should be created if it is
		    not already existing.
		
	    output: 
	      	timestamp: time stamp of the cached file, set by
		    globus_gass_cache_done(), (or globus_gass_cache_delete() ).
		local_filename: Path the the local file caching the file
		    specified by "url". NULL if "url" not yet cached and
		    creation not requested (create false). 

Returns:    
            GLOBUS_GASS_CACHE_URL_NOT_FOUND
	    GLOBUS_GASS_CACHE_ADD_NEW
	    GLOBUS_GASS_CACHE_ADD_EXISTS
	    or any of the defined gass error code.
	    
******************************************************************************/
extern int
globus_gass_cache_add(globus_gass_cache_t *  cache_handle,
	       char*           url,
	       char*           tag,
	       globus_bool_t   create,
	       unsigned long*  timestamp,
	       char**          local_filename);


/******************************************************************************
Function: globus_gass_cache_add_done()

Description:

  globus_gass_cache_add_done() MUST be called after globus_gass_cache_add(), to
  set the timestamp in the cache entry for the URL, and then unlock the
  cache entry. (The only case it does not need to be called is if
  globus_gass_cache_add() has returned GLOBUS_GASS_CACHE_URL_NOT_FOUND, of course.
  
Parameters: input:    
                cache_handle: Handler to the opened cahe directory to use.
		url : url of the cached file to set as "done" (unlock)
		tag : tag specifying which job has locked the cache and must
		    therfor be unlocked.It is an error to call this function
		    with a tag which does not currently own the cache lock.
		timestamp: time stamp of the cached file.
		
	    output: 
                none (well, the state file is updated and the cache file
		lock removed)
Returns:    GLOBUS_SUCCESS or error code:
	    or any of the defined gass error code.

******************************************************************************/
extern int
globus_gass_cache_add_done(globus_gass_cache_t *  cache_handle,
		    char *          url,
		    char *          tag,
 		    unsigned long   timestamp);

/******************************************************************************
Function: globus_gass_cache_delete_start()

Description:
    Lock the cache entry for the URL, and return the cache entry's current
    timestamp in *timestamp.
    This function will block if the data file is already locked, until it is
    unlocked.

    Parameters: input:    
                cache_handle: Handler to the opened cahe directory to use.
		url : url of the cached file to set as "done" (unlock)
		tag : tag specifying which job has locked the cache and must
		    therfor be unlocked.It is an error to call this function
		    with a tag which does not currently own the cache lock.
		
	    output: 
	      	timestamp: time stamp of the cached file, set by
		    globus_gass_cache_done(), (or globus_gass_cache_delete() ).

Returns:    GLOBUS_SUCCESS or error code:
	    or any of the defined gass error code.

******************************************************************************/
extern int
globus_gass_cache_delete_start(globus_gass_cache_t *  cache_handle,
			char *          url,
			char *          tag,
			unsigned long * timestamp);

/******************************************************************************
Function: globus_gass_cache_delete()

Description:
    Remove one instance of the tag from the cache entry's tag list.

    If there are no more tags in the tag list, then remove this cache
    entry and delete the associated local cache file.

    Otherwise, update the timestamp to the passed value.
    
    This call will leave the cache entry unlocked.

    If is_locked==GLOBUS_TRUE, then this cache entry was locked during a
    previous call to globus_gass_cache_add() or globus_gass_cache_delete_start(). The cache
    file should be locked by the corresponding url/tag, or an error is
    returned. If it is locked by the corresponding url/tag, then the normal
    operation occur, whithout blocking (remove one instance from the tag
    list, update the timestamp and unlock the cache).

    If is_locked==GLOBUS_FALSE, eventually wait the cache is not locked any
    more, and then proceed with the normal operations.(remove one instance
    from the tag list and update the timestamp).

Parameters: input:    
                cache_handle: Handler to the opened cahe directory to use.
		url : url of the file to be cached. It is used as the main
		    key to the cache entries.
		tag : tag specifying which job is/are using the cache. This
		    is usually the GLOBUS_GRAM_JOB_CONTACT. Can be NULL or empty; the
		    tag "null" is then used.
		timestamp: time stamp of the cached file.
        	is_locked: indicate if this cache entry was locked during a
		    previous call to globus_gass_cache_add() or
		    globus_gass_cache_delete_start().
	    output:
	        none 
		
Returns:    GLOBUS_SUCCESS or error code:
	    or any of the defined gass error code.   

******************************************************************************/
extern int
globus_gass_cache_delete(globus_gass_cache_t *  cache_handle,
		  char *          url,
		  char *          tag,
		  unsigned long   timestamp,
		  globus_bool_t   is_locked);

/******************************************************************************
Function: globus_gass_cache_cleanup_tag()

Description:
    Remove all instances of the tag from the cache entry's tag list.
    If there are no more tags in the tag list, then remove this cache entry
    and delete the associated local cache file.

    If the cache entry is locked with the same tag as is passed to this
    function, then the entry is unlocked after removing the tags.
    Otherwise, the cache entry's lock is left untouched.

    This function never block. 

    Note: The GLOBUS_GRAM job manager will automatically call this function with a
    tag of getenv("GLOBUS_GRAM_JOB_CONTACT") upon completion of a job.
    
Parameters: input:    
                cache_handle: Handler to the opened cahe directory to use.
		url : url of the file to be cached. It is used as the main
		    key to the cache entries.
		tag : tag specifying which job is/are using the cache. This
		    is usually the GLOBUS_GRAM_JOB_CONTACT. Can be NULL or empty; the
		    tag "null" is then used.

	    output:
	        none 
		
Returns:    GLOBUS_SUCCESS or error code:
	    or any of the defined gass error code.   

******************************************************************************/
extern int
globus_gass_cache_cleanup_tag(globus_gass_cache_t *  cache_handle,
		   char *          url,
		   char *          tag);

/******************************************************************************
Function: globus_gass_cache_cleanup_file()

Description:
    Remove the cache entry and delete the associated local cache file,
    regardless of the tags in tag list, and regardless of whether or not
    the cache entry is locked.

    This function never block.
    
Parameters: input:    
                cache_handle: Handler to the opened cahe directory to use.
		url : url of the file to be cached. It is used as the main
		    key to the cache entries.

	    output:
	        none 
		
Returns:    GLOBUS_SUCCESS or error code:
	    or any of the defined gass error code.   
******************************************************************************/
extern int
globus_gass_cache_cleanup_file(globus_gass_cache_t *  cache_handle,
			char *          url);


/******************************************************************************
Function: globus_gass_cache_add_list()

Description:
    Return the entries of the cache in *entries as an array of
    globus_gass_cache_entry_t structures, and return the number of elements in
    this array in *size.

    The function globus_gass_cache_list_free() must be called subsequently to free
    the *entrie array allocated by globus_gass_cache_list();
Parameters: input:    
                cache_handle: Handler to the opened cahe directory to use.

	    output:
	        entries : array of globus_gass_cache_entry_t structure describing
		    eache cache entry.
		size : size of the "entries" array, (nb of entries)
		
Returns:    GLOBUS_SUCCESS or error code:
	    or any of the defined gass error code.      

******************************************************************************/
extern int
globus_gass_cache_list(globus_gass_cache_t *         cache_handle,
		globus_gass_cache_entry_t **  entry,
		int *                  size);

/******************************************************************************
Function: globus_gass_cache_add_list_free()

Description:
  Free the cache entries previously returned by globus_gass_cache_list().

Parameters: input:
               entries : array of globus_gass_cache_entry_t structure describing
		    eache cache entry.
	       size : size of the "entries" array, (nb of entries)

Returns:    GLOBUS_SUCCESS

******************************************************************************/
extern int
globus_gass_cache_list_free(globus_gass_cache_entry_t *  entry,
		     int                   size);

/******************************************************************************
Function: globus_gass_cache_error_string()

Description:
  Return a pointer on an error description string.

Parameters: input:    
               error_code: error code returned by a previously called
	       globus_gass_cache function.
	    output:
	       none
Returns:   Pointer to an error message, or NULL if invalide error code.

******************************************************************************/
extern const char *
globus_gass_cache_error_string(int error_code);

#ifndef SWIG
EXTERN_C_END

/******************************************************************************
 *                    Module Definition
 *****************************************************************************/

#define GLOBUS_GASS_CACHE_MODULE (&globus_gass_cache_module)

static globus_module_descriptor_t globus_gass_cache_module =
{
    "globus_gass_cache",
    GLOBUS_NULL,
    GLOBUS_NULL,
    GLOBUS_NULL
};


#endif

#endif   /* _GLOBUS_GASS_INCLUDE_GLOBUS_GASS_CACHE_H */
