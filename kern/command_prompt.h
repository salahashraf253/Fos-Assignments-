#ifndef FOS_KERN_MONITOR_H
#define FOS_KERN_MONITOR_H
#ifndef FOS_KERNEL
# error "This is a FOS kernel header; user programs should not #include it"
#endif

#include <inc/types.h>

// Function to activate the kernel command prompt
void run_command_prompt();
int execute_command(char *command_string);

// Declaration of functions that implement command prompt commands.
int command_help(int , char **);
int command_kernel_info(int , char **);
int command_ver(int number_of_arguments, char **arguments);
int command_add(int number_of_arguments, char **arguments);

/*ASSIGNMENT3*/
int FindPhysicalAddress(char** arguments);
void ShareRangeWithPermissions(char** arguments);
int FindVirtualOfFrameNum(char** arguments);
uint32 ConnectPageToFrame(char** arguments);

int command_fpa(int , char **);
int command_srp(int , char **);
int command_fv(int , char **);
int command_cpf(int , char **);

int command_cvp(int , char **);
int command_dvp(int , char **);


#endif	// !FOS_KERN_MONITOR_H
