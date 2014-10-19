#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/processor_info.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>


processor_array_t processor_list = NULL;
mach_msg_type_number_t processor_listCnt = 0;

int main ()
{
    int res, i;
    mach_port_t host_privileged_port;
    device_t device_privileged_port;
    mach_msg_type_number_t processor_infoCnt = 0;
    processor_info_t processor_info_data;
    mach_port_t* host;

    // Get privileged ports
    res = get_privileged_ports(&host_privileged_port, &device_privileged_port);
    if (res != KERN_SUCCESS) {
        printf ("Error getting privileged ports (0x%x), %s\n", res,
                mach_error_string(res));
        exit(1);
    }

    printf ("privileged ports: host 0x%x  devices 0x%x\n",
            host_privileged_port, device_privileged_port);

    printf ("Getting processors at array 0x%x\n", processor_list);

    // Get list of processors
    res = host_processors(host_privileged_port,
                          &processor_list, &processor_listCnt);
    if (res != KERN_SUCCESS) {
        printf ("Error getting host_processors (0x%x), %s\n", res,
                mach_error_string(res));
        exit(1);
    }

    printf ("        processors at array 0x%x\n", processor_list);
    printf ("processor_listCnt %d\n", processor_listCnt);

    processor_info_data = (processor_info_t) malloc(PROCESSOR_INFO_MAX*sizeof(int));
    host = (mach_port_t *) malloc(10*sizeof(mach_port_t));

    // For each processor get processor info
    for (i=0; i < processor_listCnt; i++) {

        processor_info_data = sizeof(int)*PROCESSOR_INFO_MAX;

        res = processor_info(processor_list[i], PROCESSOR_BASIC_INFO,
                             host, processor_info_data,
                             &processor_infoCnt)

        if (res != KERN_SUCCESS) {
            printf ("Error getting processor info (0x%x), %s\n", res,
                    mach_error_string(res));
            exit(1);
        }

        processor_basic_info_t proc_basicInfo = (processor_basic_info_t) &processor_info_data[i];

        printf("CPU %d\n", i);
        printf("CPU Type:\t\t%d\n", proc_basicInfo->cpu_type);
        printf("CPU Subtype:\t\t%d\n", proc_basicInfo->cpu_subtype);

        if(proc_basicInfo->running)
            printf("Running?:\t\tYES\n");
        else
            printf("Running?:\t\tNO\n");

        printf("Slot number:\t\t%d\n", proc_basicInfo->slot_num);

        if(proc_basicInfo->is_master)
            printf("Master?:\t\tYES\n");
        else
            printf("Master?:\t\tNO\n");

        printf("\n\n")


    }
}






