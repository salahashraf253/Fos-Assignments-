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

int command_print_student_data(int number_of_arguments, char **arguments);
/*ASSIGNMENT2*/
int* CreateAccount(int , char**);
int GetNumberOfCourses(char**);
void SwitchCourses(char**);
int IsEnrolled(char**);

int command_csa(int , char**);

int command_gnc(int , char**);
int command_scs(int , char**);
int command_ie(int , char**);

/*ASSIGNMENT2 - BONUS*/
void DeleteAccount(char** arguments);
int command_dnia(int , char **);
#endif	// !FOS_KERN_MONITOR_H
