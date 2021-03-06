/*
 * Copyright (c) 2013 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "vmm_defs.h"
#include "vmm_dbg.h"
#include "memory_allocator.h"
#include "vmm_objects.h"
#include "vmcs_api.h"
#include "vmcs_sw_object.h"
#include "vmcs_actual.h"
#include "vmcs_hierarchy.h"
#include "file_codes.h"
#define VMM_DEADLOOP()          VMM_DEADLOOP_LOG(VMCS_HIERARCHY_C)
#define VMM_ASSERT(__condition) VMM_ASSERT_LOG(VMCS_HIERARCHY_C, __condition)
#ifdef JLMDEBUG
#include "jlmdebug.h"
#endif

typedef struct {
    VMCS_OBJECT    *vmcs;
    LIST_ELEMENT    list[1];
} VMCS_1_DESCRIPTOR;
VMCS_1_DESCRIPTOR *vmcs_hierarchy_vmcs1_lkup(VMCS_HIERARCHY *obj, VMCS_OBJECT *vmcs);


VMM_STATUS vmcs_hierarchy_create(VMCS_HIERARCHY *obj, GUEST_CPU_HANDLE gcpu)
{
    VMM_STATUS status;

#ifdef JLMDEBUG
    bprint("vmcs_hierarchy_create\n");
#endif
    VMM_ASSERT(obj);
    VMCS_OBJECT * v= vmcs_act_create(gcpu);
    if(NULL==v) {
#ifdef JLMDEBUG
        bprint("vmcs_act_creat retuns NULL\n");
        LOOP_FOREVER
#endif
    }
    obj->vmcs[VMCS_MERGED] = v;
    obj->vmcs[VMCS_LEVEL_0] = v;
    if (NULL == obj->vmcs[VMCS_LEVEL_0]) {
        VMM_LOG(mask_anonymous, level_trace,"Failed to create merged VMCS\n");
#ifdef JLMDEBUG
        bprint("failed to create merged VMCS\n");
        LOOP_FOREVER
#endif
        status = VMM_ERROR;
    }
    else {
        obj->vmcs[VMCS_LEVEL_1] = NULL;
        list_init(obj->vmcs_1_list);
        status = VMM_OK;
    }
    return status;
}

#ifdef INCLUDE_UNUSED_CODE
VMM_STATUS vmcs_hierarchy_add_vmcs( VMCS_HIERARCHY * obj, GUEST_CPU_HANDLE gcpu, ADDRESS gpa)
{
    VMM_STATUS status = VMM_ERROR;
    VMCS_1_DESCRIPTOR *desc;

    VMM_ASSERT(obj);
    VMM_ASSERT(obj->vmcs[VMCS_MERGED]);
    do { 
        desc = vmm_malloc(sizeof(*desc));
        if (NULL == desc) {
            VMM_LOG(mask_anonymous, level_trace,"Failed to create VMCS-1\n");
            break;
        }
        desc->vmcs = vmcs_1_create(gcpu, gpa);
        if (NULL == desc->vmcs) {
            VMM_LOG(mask_anonymous, level_trace,"Failed to create VMCS-1\n");
            vmm_mfree(desc);
            break;
        }
        // create VMCS-0 if required
        if (list_is_empty(obj->vmcs_1_list)) {
            obj->vmcs[VMCS_LEVEL_0] = vmcs_0_create(obj->vmcs[VMCS_MERGED]);
            if (NULL == obj->vmcs[VMCS_LEVEL_0]) {
                VMM_LOG(mask_anonymous, level_trace,"Failed to create VMCS-0\n");
                vmcs_destroy(desc->vmcs);
                vmm_mfree(desc);
                break;
            }
        }
        // here all objects were successfully created
        // we can add new VMCS-1 to the list
        list_add(obj->vmcs_1_list, desc->list);
        // newly created VMCS-1 becomes the current
        obj->vmcs[VMCS_LEVEL_1] = desc->vmcs;
        status = VMM_OK;
    } while (0);
    return status;
}

VMM_STATUS vmcs_hierarchy_remove_vmcs(VMCS_HIERARCHY *obj, VMCS_OBJECT *vmcs_1)
{
    VMM_STATUS status = VMM_ERROR;
    VMM_ASSERT(obj);

    do {
        VMCS_1_DESCRIPTOR   *desc;

        desc = vmcs_hierarchy_vmcs1_lkup(obj, vmcs_1);
        if (NULL == desc) {
            VMM_LOG(mask_anonymous, level_trace,"Cannot remove VMCS-1 %P. Not found\n");
            break;
        }
        // we found proper VMCS-1. Remove it.
        status = VMM_OK;
        list_remove(desc->list);
        vmcs_destroy(desc->vmcs);
        vmm_mfree(desc);
        // if there is no level-1 vmcs, then remove vmcs-0 also
        if (list_is_empty(obj->vmcs_1_list)) {
            vmcs_destroy(obj->vmcs[VMCS_LEVEL_0]);
            obj->vmcs[VMCS_LEVEL_0] = obj->vmcs[VMCS_MERGED];
        }
        else {
            // select current VMCS-1
            desc = LIST_NEXT(obj->vmcs_1_list,VMCS_1_DESCRIPTOR, list);
            obj->vmcs[VMCS_LEVEL_1] = desc->vmcs;
        }
    } while (0);
    return status;
}
#endif


VMCS_OBJECT* vmcs_hierarchy_get_vmcs(VMCS_HIERARCHY *obj, VMCS_LEVEL level)
{
    VMCS_OBJECT *vmcs= NULL;

#ifdef JLMDEBUG1
    bprint("vmcs_hierarchy_get_vmcs %p %d, ", obj, level);
    if(level>4)
        return NULL;
#endif
    VMM_ASSERT(obj);
    if (((int)level)>=VMCS_LEVEL_0 && ((int)level)<VMCS_LEVELS) {
        vmcs = obj->vmcs[level];
    }
    else {
        VMM_LOG(mask_anonymous, level_trace,"Invalid VMCS level\n");
        VMM_ASSERT(0);
#ifdef JLMDEBUG1
        bprint("vmcs_hierarchy_get_vmcs returning NULL\n");
        LOOP_FOREVER
#endif
        vmcs = NULL;
    }
#ifdef JLMDEBUG1
    bprint(" returning %p\n", vmcs);
#endif
    return vmcs;
}

#ifdef INCLUDE_UNUSED_CODE
VMCS_OBJECT * vmcs_hierarchy_get_next_vmcs_1(VMCS_HIERARCHY *obj)
{
    VMM_ASSERT(obj);

    if (NULL != obj->vmcs[VMCS_LEVEL_1]) {
        VMCS_1_DESCRIPTOR *desc = vmcs_hierarchy_vmcs1_lkup(obj, obj->vmcs[VMCS_LEVEL_1]);
        VMM_ASSERT(desc);

        desc = LIST_NEXT(desc->list, VMCS_1_DESCRIPTOR, list);
        obj->vmcs[VMCS_LEVEL_1] = desc->vmcs;
    }
    return  obj->vmcs[VMCS_LEVEL_1];
}

VMCS_OBJECT * vmcs_hierarchy_select_vmcs_1(VMCS_HIERARCHY *obj, VMCS_OBJECT *vmcs)
{
    VMCS_1_DESCRIPTOR   *desc;

    VMM_ASSERT(obj);
    desc = vmcs_hierarchy_vmcs1_lkup(obj, vmcs);
    if (NULL != desc) {
        // found
        obj->vmcs[VMCS_LEVEL_1] = desc->vmcs;
    }
    else {
        VMM_LOG(mask_anonymous, level_trace,"Failed to select VMCS-1. Should be added first.\n");
        VMM_ASSERT(0);
    }
    return obj->vmcs[VMCS_LEVEL_1];
}

VMCS_1_DESCRIPTOR *vmcs_hierarchy_vmcs1_lkup(VMCS_HIERARCHY *obj, VMCS_OBJECT *vmcs)
{
    LIST_ELEMENT      *iter;
    VMCS_1_DESCRIPTOR *desc;    // output

    VMM_ASSERT(obj);
    LIST_FOR_EACH(obj->vmcs_1_list, iter) {
        desc = LIST_ENTRY(iter, VMCS_1_DESCRIPTOR, list);
        if (vmcs == desc->vmcs) {
            // math
            return desc;
        }
    }
    return NULL;    // not found
}
#endif

