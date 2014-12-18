/*
 * Copyright 1999-2014 University of Chicago
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file context/pre_connect.c
 * @brief globus_net_manager_context_pre_connect()
 */

#include "globus_net_manager_context.h"

globus_result_t
globus_net_manager_context_pre_connect(
    globus_net_manager_context_t        context,
    const char                         *task_id,
    const char                         *transport,
    const char                         *remote_contact,
    const globus_net_manager_attr_t    *attr_array,
    char                              **remote_contact_out,
    globus_net_manager_attr_t         **attr_array_out)
{
    globus_i_net_manager_context_t *    ctx = context;
    globus_list_t *                     list;
    globus_result_t                     result = GLOBUS_SUCCESS;
    globus_net_manager_attr_t *         tmp_attr_array = NULL;
    char *                              tmp_remote_contact = NULL;
    globus_i_net_manager_context_entry_t * ent;
    
    if(!ctx || !task_id || !transport || !attr_array || !attr_array_out ||
        !remote_contact || !remote_contact_out)
    {
        result = GLOBUS_FAILURE;
        goto error_bad_args;
    }

    for(list = ctx->managers; 
        !globus_list_empty(list) && result == GLOBUS_SUCCESS; 
        list = globus_list_rest(list))
    {            
        ent = globus_list_first(list);
        
        if(ent->manager->pre_connect)
        {   
            globus_net_manager_attr_t *     ret_attr_array = NULL;
            char *                          ret_remote_contact = NULL;
            
            result = ent->manager->pre_connect(
                ent->manager,
                ent->attrs,
                task_id,
                transport,
                tmp_remote_contact ? tmp_remote_contact : remote_contact,
                tmp_attr_array ? tmp_attr_array : attr_array,
                &ret_remote_contact,
                &ret_attr_array);
                
            if(ret_attr_array != NULL)
            {
                globus_net_manager_attr_array_delete(tmp_attr_array);
                tmp_attr_array = ret_attr_array;
            }
            if(ret_remote_contact != NULL)
            {
                if(tmp_remote_contact)
                {
                    globus_free(tmp_remote_contact);
                }
                tmp_remote_contact = ret_remote_contact;
            }
        }
    }
    
    *attr_array_out = tmp_attr_array;
    *remote_contact_out = tmp_remote_contact;

    return result;

error_bad_args:
    return result;
}