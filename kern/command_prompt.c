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

		//helper commands
		//===============
		{ "cvp", "Connect virtual address to physical address", command_cvp},
		{ "dvp", "DisConnect virtual address from its physical one", command_dvp},

		//Assignment3 commands
		//====================
		{ "fpa", "Find the EXACT pa correspond to the given va", command_fpa},
		{ "srp", "Share 2 virtual ranges with the given permission", command_srp},
		{ "fv", "Find virtual address of the given frame number", command_fv},

		//Assignment3.BONUS command
		//=========================
		{ "cpf", "Connect page number to frame number with permissions", command_cpf},

		//TODO: LAB2 Hands-on: add the commands here


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
	//CAUTION: DON'T CHANGE OR COMMENT THESE LINE======
	if (firstTime)
	{
		firstTime = 0;
		TestAssignment3();
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

//===========================================================================
//Helper Commands
//===============
/*DON'T change this function*/
int command_cvp(int number_of_arguments, char **arguments)
{
	uint32 va = strtol(arguments[1], NULL, 16);
	uint32 pa = strtol(arguments[2], NULL, 16);
	map_frame(ptr_page_directory, to_frame_info(pa), (void*)va, 3);
	return 0;
}

/*DON'T change this function*/
int command_dvp(int number_of_arguments, char **arguments)
{
	uint32 va = strtol(arguments[1], NULL, 16);
	uint32 *ptr_table;
	get_page_table(ptr_page_directory, (void*) va, 0, &ptr_table);
	if (ptr_table!=NULL)
		ptr_table[PTX(va)] &= ~PERM_PRESENT;

	return 0;
}

/****************************************************************/

//========================================================
/*ASSIGNMENT-3 [MAIN QUESTIONS] */
//========================================================




//Q1:Find EXACT Physical Address
//=========================================
/*DON'T change this function*/
int command_fpa(int number_of_arguments, char **arguments )
{
	//DON'T WRITE YOUR LOGIC HERE, WRITE INSIDE THE FindPhysicalAddress() FUNCTION
	int ret = FindPhysicalAddress(arguments);
	if (ret == -1)
		cprintf("pa not exists!");
	else
		cprintf("pa = %u\n", (uint32)ret);
	return 0;
}
/*---------------------------------------------------------*/

/*FILL this function
 * arguments[1]: virtual address of the page in HEXADECIMAL
 * Return:
 * 	If the page exists, return the EXACT physical address (i.e. including the offset).
 * 	Else, return -1
 */
int getDirectoryIndex(uint32 virtualAddress){
	return PDX(virtualAddress);
}
int getPageTableIndex(uint32 virtualAddress){
	return PTX(virtualAddress);
}
uint32* getPageTableVA(const void *virtual_address, int write){
	uint32* pageTableVirtualAddress=NULL;
	get_page_table(ptr_page_directory, virtual_address,write,&pageTableVirtualAddress);
	return pageTableVirtualAddress;
}
int getPhysicalAddress(int virtualAddress){
	int offset=virtualAddress&(0xFFF);
	uint32 *PageTableVA=getPageTableVA((void *)virtualAddress,0);
	int pageTableIndex = getPageTableIndex(virtualAddress);
	if(PageTableVA!=NULL){
		if((PageTableVA[pageTableIndex] & PERM_PRESENT)>0)
		{
			int phyicalAddress=(PageTableVA[pageTableIndex]>>12)*PAGE_SIZE +offset;
			cprintf("Phyiscal Address of %x is %x\n",virtualAddress,phyicalAddress);
			return phyicalAddress;
		}
	}
	return -1;
}
int FindPhysicalAddress(char** arguments)
{
	//TODO: Assignment3.Q1
	//put your logic here
	//...
	int virtualAddress=strtol(arguments[1],NULL,16);
	return getPhysicalAddress(virtualAddress);
}
//========================================================

//Q2:Share Range with Permissions
//========================================

/*DON'T change this function*/
int command_srp(int number_of_arguments, char **arguments )
{
	//DON'T WRITE YOUR LOGIC HERE, WRITE INSIDE THE ShareRangeWithPermissions() FUNCTION
	ShareRangeWithPermissions(arguments) ;
	return 0;
}
/*---------------------------------------------------------*/

/*FILL this function
 * arguments[1]: start virtual address of the range to be shared (in HEX)
 * arguments[2]: start virtual address of the second range (in HEX)
 * arguments[3]: size of the sharing range (in MB)
 * arguments[4]: <r/w>: 'r' for read-only permission, 'w' for read/write permission
 */

void setWritePremission(uint32 * pageTable, int pageTableIndex){
	pageTable[pageTableIndex] |= PERM_WRITEABLE;
}
void setReadPremission(uint32 * pageTable, int pageTableIndex){
	pageTable[pageTableIndex] &= ~PERM_WRITEABLE;
}
void setPremission(char premission,uint32 * pageTable,int pageTableIndex){
	if(premission =='w'){
		setWritePremission(pageTable,pageTableIndex);
	}
	else{
		setReadPremission(pageTable,pageTableIndex);
	}
}
void ShareRangeWithPermissions(char** arguments)
{
	uint32 virutalAddress1=strtol(arguments[1],NULL,16);
	uint32 virutalAddress2=strtol(arguments[2],NULL,16);
	int size=strtol(arguments[3],NULL,10);
	char premission=arguments[4][0];

	int toShare =(size*1024)/4 + (size%4==0?0:1);

	uint32 * pageTable1Va=getPageTableVA((void*)virutalAddress1,0);
	uint32 * pageTable2Va=getPageTableVA((void*)virutalAddress2,1);

	if(pageTable1Va==NULL||pageTable2Va==NULL)
		return ;

	int tmpVar1=virutalAddress1;
	int tmpVar2=virutalAddress2;

	uint32 * pageTable1;
	uint32 * pageTable2;
	for(int i=0;i<toShare;i++){
		get_page_table(ptr_page_directory,(int*)tmpVar1,0,&pageTable1);
		get_page_table(ptr_page_directory,(int*)tmpVar2,1,&pageTable2);

		pageTable2[getPageTableIndex(tmpVar2)]=pageTable1[getPageTableIndex(tmpVar1)];

		setPremission(premission,pageTable2,getPageTableIndex(tmpVar2));

		tmpVar1+=PAGE_SIZE;
		tmpVar2+=PAGE_SIZE;
	}

}
//========================================================

//Q3: Find virtual address of the given frame number
//=============================================================

/*DON'T change this function*/
int command_fv(int number_of_arguments, char **arguments )
{
	//DON'T WRITE YOUR LOGIC HERE, WRITE INSIDE THE FindVirtualOfFrameNum() FUNCTION
	int ret = FindVirtualOfFrameNum(arguments) ;
//	if (ret == -1)
//	{
//		cprintf("not exists!\n");
//	}
//	else
//	{
//		cprintf("va of the first page that's connected frame #%s = %x\n", arguments[1], ret);
//	}
	return 0;
}
/*---------------------------------------------------------*/

/*FILL this function
 * arguments[1]: frame number
 * Return:
 * 	If there's one or more pages connected to the given frame number, return the virtual address of the FIRST one.
 * 	Else, return -1.
 */
//fv <frame number>
int getpageFrameNumber(uint32 virutalAddress ){
	uint32* pageTableVA=NULL;
	get_page_table(ptr_page_directory,(int*)virutalAddress,0,&pageTableVA);
	if(pageTableVA!=NULL){
		uint32 pageEntry=pageTableVA[PTX(virutalAddress)];
		if((pageEntry & PERM_PRESENT)){
			int pageFrameNumber=pageEntry>>12;
			return pageFrameNumber;
		}
	}
	return -1;
}
int FindVirtualOfFrameNum(char** arguments)
{
	//TODO: Assignment3.Q3
	uint32 virutalAddress=0;
	int pageFrameNumber=strtol(arguments[1],NULL,10);
//	int offset=pageFrameNumber<<20;
//	cprintf("Offset : %d\n",offset);
//	int frameNo=pageFrameNumber>>12;
//	cprintf("# frame: %d\n",frameNo);
	uint32 address=0;
	for(int i=0;i<4*1024*1024;i++){
		if(getpageFrameNumber(address)==pageFrameNumber){
			cprintf("frame: %d is Found at address: %x\n",pageFrameNumber,address);
			return (int)address;
		}
		address+=PAGE_SIZE;
	}
	cprintf("frame: %d is not Found\n",pageFrameNumber);
	return -1;
}
//========================================================


//========================================================

//========================================================
/*ASSIGNMENT-3 [BONUS QUESTION] */
//========================================================

//BONUS: Connect page number to frame number with permissions
//======================================================================

/*DON'T change this function*/
int command_cpf(int number_of_arguments, char **arguments )
{
	//DON'T WRITE YOUR LOGIC HERE, WRITE INSIDE THE ConnectPageToFrame() FUNCTION
	uint32 tableEntry = ConnectPageToFrame(arguments);
	cprintf("The table entry after connection = %08x\n", tableEntry);

	return 0;
}
/*---------------------------------------------------------*/

/*FILL this function
 * arguments[1]: page number
 * arguments[2]: frame number
 * arguments[3]: <r/w>: 'r' for read-only permission, 'w' for read/write permission
 * arguments[4]: <s/u>: 's' for supervisor permission, 'u' for user permission
 * Return:
 * 	page table ENTRY of the given page after applying the connection
 */
uint32 ConnectPageToFrame(char** arguments)
{
	//TODO: Assignment3.BONUS
	//put your logic here
	//...

	return 0;
}


