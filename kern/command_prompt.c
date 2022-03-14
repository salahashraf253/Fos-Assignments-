/*	Simple command-line kernel prompt useful for
	controlling the kernel and exploring the system interactively.


KEY WORDS
==========
CONSTANTS:	WHITESPACE, NUM_OF_COMMANDS
VARIABLES:	Command, commands, name, description, function_to_execute, number_of_arguments, arguments, command_string, command_line, command_found
FUNCTIONS:	readline, cprintf, execute_command, run_command_prompt, command_kernel_info, command_help, strcmp, strsplit, start_of_kernel, start_of_uninitialized_data_section, end_of_kernel_code_section, end_of_kernel
=====================================================================================================================================================================================================
 */

#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/memlayout.h>
#include <inc/assert.h>
#include <inc/x86.h>


#include <kern/console.h>
#include <kern/command_prompt.h>
#include <kern/memory_manager.h>
#include <kern/trap.h>
#include <kern/kdebug.h>
#include <kern/user_environment.h>
#include <kern/tests.h>


//TODO:LAB3.Hands-on: declare start address variable of "My int array"

//=============================================================

//Structure for each command
struct Command
{
	char *name;
	char *description;
	// return -1 to force command prompt to exit
	int (*function_to_execute)(int number_of_arguments, char** arguments);
};

//Functions Declaration
int command_writemem(int number_of_arguments, char **arguments);
int command_readmem(int number_of_arguments, char **arguments);
int command_meminfo(int , char **);

//Lab2.Hands.On
//=============
//TODO: LAB2 Hands-on: declare the command function here


//Lab4.Hands.On
//=============
int command_show_mapping(int number_of_arguments, char **arguments);
int command_set_permission(int number_of_arguments, char **arguments);
int command_share_range(int number_of_arguments, char **arguments);

//Lab5.Examples
//=============
int command_nr(int number_of_arguments, char **arguments);
int command_ap(int , char **);
int command_fp(int , char **);

//Lab5.Hands-on
//=============
int command_asp(int, char **);
int command_cfp(int, char **);

//Lab6.Examples
//=============
int command_run(int , char **);
int command_kill(int , char **);
int command_ft(int , char **);


//Array of commands. (initialized)
struct Command commands[] =
{
		{ "help", "Display this list of commands", command_help },	//don't need arguments
		{ "kernel_info", "Display information about the kernel", command_kernel_info },	//don't need arguments
		{ "wum", "writes one byte to specific location" ,command_writemem},	//need arguments
		{ "rum", "reads one byte from specific location" ,command_readmem},	//need arguments
		{ "ver", "Print the FOS version" ,command_ver},//don't need arguments
		{ "add", "Add two integers" ,command_add},//need arguments

		//Assignment2 commands
		//====================
		{"csa", "Creat account for a student with the given student name, gpa and list of courses", command_csa},
		{"gnc", "Get number of courses a student enrolled in", command_gnc},
		{"scs", "Switch courses between two students", command_scs},
		{"ie", "Check whether the student is enrooled in the given course or not", command_ie},
		//Assignment2.BONUS command
		//=========================
		{ "dnia", "Delete named account and shift down all above accounts (if any)", command_dnia},

		//TODO: LAB2 Hands-on: add the commands here
		{"print","fds",command_print_student_data},
		{"print_index","fdf",command_print_with_index},

		//LAB4: Hands-on
		{ "sm", "Lab4.HandsOn: display the mapping info for the given virtual address", command_show_mapping},
		{ "sp", "Lab4.HandsOn: set the desired permission to a given virtual address page", command_set_permission},
		{ "sr", "Lab4.HandsOn: shares the physical frames of the first virtual range with the 2nd virtual range", command_share_range},

		//LAB5: Examples
		{ "nr", "Lab5.Example: show the number of references of the physical frame" ,command_nr},
		{ "ap", "Lab5.Example: allocate one page [if not exists] in the user space at the given virtual address", command_ap},
		{ "fp", "Lab5.Example: free one page in the user space at the given virtual address", command_fp},

		//LAB5: Hands-on
		{ "asp", "Lab5.HandsOn: allocate 2 shared pages with the given virtual addresses" ,command_asp},
		{ "cfp", "Lab5.HandsOn: count the number of free pages in the given range", command_cfp},

		//LAB6: Examples
		{ "ft", "Lab6.Example: Free table", command_ft},
		{ "run", "Lab6.Example: Load and Run User Program", command_run},
		{ "kill", "Lab6.Example: Kill User Program", command_kill},

};

//Number of commands = size of the array / size of command structure
#define NUM_OF_COMMANDS (sizeof(commands)/sizeof(commands[0]))

int firstTime = 1;

//invoke the command prompt
void run_command_prompt()
{
	//----------------------------------------------------Main Function -----------------------
	//CAUTION: DON'T CHANGE OR COMMENT THESE LINE======
	if (firstTime)
	{
		firstTime = 0;
		TestAssignment2();
	}
	else
	{
		cprintf("Test failed.\n");
	}
	//================================================

	char command_line[1024];

	while (1==1)
	{
		//get command line
		readline("FOS> ", command_line);

		//parse and execute the command
		if (command_line != NULL)
			if (execute_command(command_line) < 0)
				break;
	}
}

/***** Kernel command prompt command interpreter *****/

//define the white-space symbols
#define WHITESPACE "\t\r\n "

//Function to parse any command and execute it
//(simply by calling its corresponding function)
int execute_command(char *command_string)
{
	// Split the command string into whitespace-separated arguments
	int number_of_arguments;
	//allocate array of char * of size MAX_ARGUMENTS = 16 found in string.h
	char *arguments[MAX_ARGUMENTS];


	strsplit(command_string, WHITESPACE, arguments, &number_of_arguments) ;
	if (number_of_arguments == 0)
		return 0;

	// Lookup in the commands array and execute the command
	int command_found = 0;
	int i ;
	for (i = 0; i < NUM_OF_COMMANDS; i++)
	{
		if (strcmp(arguments[0], commands[i].name) == 0)
		{
			command_found = 1;
			break;
		}
	}

	if(command_found)
	{
		int return_value;
		return_value = commands[i].function_to_execute(number_of_arguments, arguments);
		return return_value;
	}
	else
	{
		//if not found, then it's unknown command
		cprintf("Unknown command '%s'\n", arguments[0]);
		return 0;
	}
}

/***** Implementations of basic kernel command prompt commands *****/
/***************************************/
/*DON'T change the following functions*/
/***************************************/
//print name and description of each command
int command_help(int number_of_arguments, char **arguments)
{
	int i;
	for (i = 0; i < NUM_OF_COMMANDS; i++)
		cprintf("%s - %s\n", commands[i].name, commands[i].description);

	cprintf("-------------------\n");

	return 0;
}

/*DON'T change this function*/
//print information about kernel addresses and kernel size
int command_kernel_info(int number_of_arguments, char **arguments )
{
	extern char start_of_kernel[], end_of_kernel_code_section[], start_of_uninitialized_data_section[], end_of_kernel[];

	cprintf("Special kernel symbols:\n");
	cprintf("  Start Address of the kernel 			%08x (virt)  %08x (phys)\n", start_of_kernel, start_of_kernel - KERNEL_BASE);
	cprintf("  End address of kernel code  			%08x (virt)  %08x (phys)\n", end_of_kernel_code_section, end_of_kernel_code_section - KERNEL_BASE);
	cprintf("  Start addr. of uninitialized data section 	%08x (virt)  %08x (phys)\n", start_of_uninitialized_data_section, start_of_uninitialized_data_section - KERNEL_BASE);
	cprintf("  End address of the kernel   			%08x (virt)  %08x (phys)\n", end_of_kernel, end_of_kernel - KERNEL_BASE);
	cprintf("Kernel executable memory footprint: %d KB\n",
			(end_of_kernel-start_of_kernel+1023)/1024);
	return 0;
}


/*DON'T change this function*/
int command_readmem(int number_of_arguments, char **arguments)
{
	unsigned int address = strtol(arguments[1], NULL, 16);
	unsigned char *ptr = (unsigned char *)(address ) ;

	cprintf("value at address %x = %c\n", ptr, *ptr);

	return 0;
}

/*DON'T change this function*/
int command_writemem(int number_of_arguments, char **arguments)
{
	unsigned int address = strtol(arguments[1], NULL, 16);
	unsigned char *ptr = (unsigned char *)(address) ;

	*ptr = arguments[2][0];

	return 0;
}

/*DON'T change this function*/
int command_meminfo(int number_of_arguments, char **arguments)
{
	cprintf("Free frames = %d\n", calculate_free_frames());
	return 0;
}

//===========================================================================
//Lab1 Examples
//=============
/*DON'T change this function*/
int command_ver(int number_of_arguments, char **arguments)
{
	cprintf("FOS version 0.1\n") ;
	return 0;
}

/*DON'T change this function*/
int command_add(int number_of_arguments, char **arguments)
{
	int n1 = strtol(arguments[1], NULL, 10);
	int n2 = strtol(arguments[2], NULL, 10);

	int res = n1 + n2 ;
	cprintf("res=%d\n", res);

	return 0;
}

//===========================================================================
//Lab2.Hands.On
//=============
//TODO: LAB2 Hands-on: write the command function here


//===========================================================================
//Lab4.Hands.On
//=============
int command_show_mapping(int number_of_arguments, char **arguments)
{
	//TODO: LAB4 Hands-on: fill this function. corresponding command name is "sm"
	//Comment the following line
	panic("Function is not implemented yet!");

	return 0 ;
}

int command_set_permission(int number_of_arguments, char **arguments)
{
	//TODO: LAB4 Hands-on: fill this function. corresponding command name is "sp"
	//Comment the following line
	panic("Function is not implemented yet!");

	return 0 ;
}

int command_share_range(int number_of_arguments, char **arguments)
{
	//TODO: LAB4 Hands-on: fill this function. corresponding command name is "sr"
	//Comment the following line
	panic("Function is not implemented yet!");

	return 0;
}

//===========================================================================
//Lab5.Examples
//==============
//[1] Number of references on the given physical address
int command_nr(int number_of_arguments, char **arguments)
{
	//TODO: LAB5 Example: fill this function. corresponding command name is "nr"
	//Comment the following line
	panic("Function is not implemented yet!");

	return 0;
}

//[2] Allocate Page: If the given user virtual address is mapped, do nothing. Else, allocate a single frame and map it to a given virtual address in the user space
int command_ap(int number_of_arguments, char **arguments)
{
	//TODO: LAB5 Example: fill this function. corresponding command name is "ap"
	//Comment the following line
	panic("Function is not implemented yet!");

	return 0 ;
}

//[3] Free Page: Un-map a single page at the given virtual address in the user space
int command_fp(int number_of_arguments, char **arguments)
{
	//TODO: LAB5 Example: fill this function. corresponding command name is "fp"
	//Comment the following line
	panic("Function is not implemented yet!");

	return 0;
}

//===========================================================================
//Lab5.Hands-on
//==============
//[1] Allocate Shared Pages
int command_asp(int number_of_arguments, char **arguments)
{
	//TODO: LAB5 Hands-on: fill this function. corresponding command name is "asp"
	//Comment the following line
	panic("Function is not implemented yet!");

	return 0;
}


//[2] Count Free Pages in Range
int command_cfp(int number_of_arguments, char **arguments)
{
	//TODO: LAB5 Hands-on: fill this function. corresponding command name is "cfp"
	//Comment the following line
	panic("Function is not implemented yet!");

	return 0;
}

//===========================================================================
//Lab6.Examples
//=============
/*DON'T change this function*/
int command_run(int number_of_arguments, char **arguments)
{
	//[1] Create and initialize a new environment for the program to be run
	struct UserProgramInfo* ptr_program_info = env_create(arguments[1]);
	if(ptr_program_info == 0) return 0;

	//[2] Run the created environment using "env_run" function
	env_run(ptr_program_info->environment);
	return 0;
}

/*DON'T change this function*/
int command_kill(int number_of_arguments, char **arguments)
{
	//[1] Get the user program info of the program (by searching in the "userPrograms" array
	struct UserProgramInfo* ptr_program_info = get_user_program_info(arguments[1]) ;
	if(ptr_program_info == 0) return 0;

	//[2] Kill its environment using "env_free" function
	env_free(ptr_program_info->environment);
	ptr_program_info->environment = NULL;
	return 0;
}

int command_ft(int number_of_arguments, char **arguments)
{
	//TODO: LAB6 Example: fill this function. corresponding command name is "ft"
	//Comment the following line

	return 0;
}
/****************************************************************/

//========================================================
/*ASSIGNMENT-2 [MAIN QUESTIONS] */
struct Students{
	char name[20];
	int gpa;
	int*addressOfFirstCourse;
	int courseCounter;
};
//========================================================
//Q1:Create Student Account [1.5 MARKS]
//======================================
/*DON'T change this function*/
int command_csa(int number_of_arguments, char **arguments )
{
	//cprintf("gsfg\n");
	//DON'T WRITE YOUR LOGIC HERE, WRITE INSIDE THE CreateAccount() FUNCTION
	int *s_va = CreateAccount(number_of_arguments, arguments);
	return 0;
}
/*---------------------------------------------------------*/

/*FILL this function
 * arguments[1]: student name
 * arguments[2]: student gpa
 * arguments[3...(K+3)]: k courses that the student enrolled in
 * Return:
 * 		Start address of the FIRST COURSE in the created array
 *
 * Example:
 * 	FOS> csa   Ali  4  1  2  3
 * 				^
 * 				|
 * arguments	[1]	[2]	[3]	...
 * Create account named "Ali", with gpa 1.4 and 3 courses: 1, 2, 3
 * It should return the start address of the FIRST COURSE in the created array
 */
#define MaxStudents 30
#define MaxCourses 50
struct  Students stud[30];

int index=0;
int *coursePointer=(int*)0xF1000000;

int* CreateAccount(int numOfArgs, char** arguments)
{
	//TODO: Assignment2.Q1
	//put your logic here
	//...
	int*startVirtualAddress=coursePointer;
	cprintf("Address : %x\n",startVirtualAddress);
	if(index < MaxStudents){
		stud[index].addressOfFirstCourse=startVirtualAddress;

		for(int i=0;i<strlen(arguments[1]);i++){
			stud[index].name[i]=arguments[1][i];
		}
		//stud[index].name=arguments[1];
		cprintf("Name : %s\n",stud[index].name);

		stud[index].gpa=strtol(arguments[2],NULL,10);
		stud[index].courseCounter=0;

		for(int i=3;i<numOfArgs;i++){
			int courseNumber=strtol(arguments[i],NULL,10);

			*coursePointer=courseNumber;
			coursePointer++;

			stud[index].courseCounter++;
		}
		index++;
	}
	//cprintf("%d",index);
	//return 0;
	return startVirtualAddress;
}
//========================================================

//Q2:Get Number of Courses [1.5 MARKS]
//=============================
/*DON'T change this function*/
int command_gnc(int number_of_arguments, char **arguments )
{
	//DON'T WRITE YOUR LOGIC HERE, WRITE INSIDE THE GetNumberOfCourses() FUNCTION
	int numOfCourses = GetNumberOfCourses(arguments) ;
	cprintf("%s is enrolled in %d courses.\n", arguments[1], numOfCourses);
	return 0;
}
/*---------------------------------------------------------*/

/*FILL this function
 * arguments[1]: student name
 * Return:
 * 		The number of courses that the student is enrolled in.
 */
int GetNumberOfCourses(char** arguments)
{
	//TODO: Assignment2.Q2
	//put your logic here
	//...
	for(int i=0;i<MaxStudents;i++){
		if(strcmp(stud[i].name,arguments[1])==0){
			return stud[i].courseCounter;
		}
	}
	return 0;
}
//========================================================

//Q3:Copy courses from One account to Another [1.5 MARKS]
//======================================================
/*DON'T change this function*/
int command_scs(int number_of_arguments, char **arguments )
{
	//DON'T WRITE YOUR LOGIC HERE, WRITE INSIDE THE SwitchCourses() FUNCTION
	SwitchCourses(arguments) ;
	return 0;
}
/*---------------------------------------------------------*/

/*FILL this function
 * arguments[1]: first student name
 * arguments[2]: second student name
 */
void swap(int*x,int*y){
	int temp=*x;
	*x=*y;
	*y=temp;
}
int getIndexOfStudent(char *name){
	for(int i=0;i<index;i++){
		if(strcmp(stud[i].name,name)==0){
			return i;
		}
	}
	cprintf("this name (%s) is not Found\n",name);
	return -1;
}
void swapAddr(int* a, int* b) {
    int* temp = a;
    a = b;
    b = temp;
}
void SwitchCourses(char** arguments)
{
	//TODO: Assignment2.Q3
	//put your logic here
	//...
	char *firstName=arguments[1];
	char *secondName = arguments[2];
	int firstStudentIndex=getIndexOfStudent(firstName);
	int secondStudentIndex=getIndexOfStudent(secondName);

//	cprintf("Before swapping : %x and %x\n",stud[firstStudentIndex].address,stud[secondStudentIndex].address);
//
//	int *temp=stud[firstStudentIndex].address;
//	stud[firstStudentIndex].address=stud[secondStudentIndex].address;
//	stud[secondStudentIndex].address=temp;
//
//	//swapAddr(&stud[firstStudentIndex].address,stud[&secondStudentIndex].address);
//	cprintf("After swapping : %x and %x\n",stud[firstStudentIndex].address,stud[secondStudentIndex].address);

	int* address1=stud[firstStudentIndex].addressOfFirstCourse;
	int *address2=stud[secondStudentIndex].addressOfFirstCourse;
	for(int i=0;i<stud[firstStudentIndex].courseCounter;i++){
		int temp=address1[i];
		address1[i]=address2[i];
		address2[i]=temp;
//		address1++,address2++;
	}
}
//========================================================

//Q4:Is enrolled? [1.5 MARKS]
//==============================
/*DON'T change this function*/
int command_ie(int number_of_arguments, char **arguments )
{
	//DON'T WRITE YOUR LOGIC HERE, WRITE INSIDE THE IsEnrolled() FUNCTION
	IsEnrolled(arguments);
	return 0;
}
/*---------------------------------------------------------*/

/*FILL this function
 * arguments[1]: array name
 * arguments[2]: order type ("a": ascending,"d": descending)
 */
int IsEnrolled(char** arguments)
{
	//Assignment2.Q4
	//put your logic here
	//...
	int position=getIndexOfStudent((arguments[1]));
	int courseNumber=strtol(arguments[2],NULL,10);
	int *address=stud[position].addressOfFirstCourse;
	for(int i=0;i<stud[position].courseCounter;i++){
		if(*address == courseNumber)
			return 1;
		address++;
	}
	return 0;
}
//========================================================

//BONUS:Delete Student Account [2 MARKS]
/*DON'T change this function*/
int command_dnia(int number_of_arguments, char **arguments )
{
	//DON'T WRITE YOUR LOGIC HERE, WRITE INSIDE THE DeleteAccount() FUNCTION
	DeleteAccount(arguments);
	return 0;
}
/*---------------------------------------------------------*/

/*FILL this function
 * arguments[1]: account name
 * It should delete the previously created <account> from the memory.
 * This is done by moving down all allocated accounts that are located after the deleted one.
 */
void copyNames(char *name1,char *name2,int size){
	for(int i=0;i<size;i++){
		name1[i]=name2[i];
	}
}
//size =2
//0 1 2
//1 2
void DeleteAccount(char** arguments)
{
	//Assignment2.BONUS
	//put your logic here
	//...
	if(index==1){
		stud[0].courseCounter=0;
		coursePointer=(int*)0xF1000000;
		return;
	}

	char *studentName=arguments[1];
	int studentIndex=getIndexOfStudent(studentName);
	//stud[studentIndex].addressOfFirstCourse=stud[studentIndex+1].addressOfFirstCourse;

	for(int i=studentIndex;i<index-1;i++){

		int* address1=stud[i].addressOfFirstCourse;
		int *address2=stud[i+1].addressOfFirstCourse;
		for(int i=0;i<stud[i+1].courseCounter;i++){
			int temp=address1[i];
			address1[i]=address2[i];
			address2[i]=temp;
		}


		copyNames(stud[i].name,stud[i+1].name,strlen(stud[i+1].name));

		//copy GPA
		stud[i].gpa=stud[i+1].gpa;


		//copy Coureses
//		int* address1=stud[i].address;
//		int *address2=stud[i+1].address;

//		int*tmp;
//		tmp=stud[i].addressOfFirstCourse;
//		stud[i].addressOfFirstCourse=stud[i+1].addressOfFirstCourse;
//		stud[i+1].addressOfFirstCourse=tmp;
//
//		stud[i].courseCounter=stud[i+1].courseCounter;

		//for(int j=0;i<stud[i+1].courseCounter;j++){
//			stud[i].address[j]=stud[i+1].address[j];
//		}
	}
	index--;

}

//..............................................
int command_print_with_index(int number_of_arguments, char **arguments){
	int position=strtol(arguments[1],NULL,16);

	cprintf("Name : %s\nGpa: %d\n",stud[position].name,stud[position].gpa);

	int *address=stud[position].addressOfFirstCourse;
	cprintf("Courses Address is at %x\n",address);

	cprintf("Courses %d : ",stud[position].courseCounter);

	for(int i=0;i<stud[position].courseCounter;i++){
		cprintf(" %d ",*address);
		address++;
	}
	cprintf("\n=================================================\n");
	return 0;
}
int command_print_student_data(int number_of_arguments, char **arguments){
	int position = getIndexOfStudent(arguments[1]);
	if(position==-1)return 0;
	cprintf("Name : %s\nGpa: %d\n",stud[position].name,stud[position].gpa);

	int *address=stud[position].addressOfFirstCourse;
	cprintf("Courses Address is at %x\n",address);

	cprintf("Courses %d : ",stud[position].courseCounter);

	for(int i=0;i<stud[position].courseCounter;i++){
		cprintf(" %d ",*address);
		address++;
	}
	cprintf("\n==============================================\n");
	return 0;
}
