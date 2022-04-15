#include <kern/tests.h>
#include <kern/memory_manager.h>

//define the white-space symbols
#define WHITESPACE "\t\r\n "

void TestAssignment3()
{
	cprintf("\n========================\n");
	cprintf("Automatic Testing of Q1:\n");
	cprintf("========================\n");
	TestAss3Q1();
	cprintf("\n========================\n");
	cprintf("Automatic Testing of Q2:\n");
	cprintf("========================\n");
	TestAss3Q2();
	cprintf("\n========================\n");
	cprintf("Automatic Testing of Q3:\n");
	cprintf("========================\n");
	TestAss3Q3();
	cprintf("\n===========================\n");
	cprintf("Automatic Testing of BONUS:\n");
	cprintf("===========================\n");
	TestAss3QB();
}

int TestAss3Q1()
{
	int kilo = 1024 ;
	int mega = 1024*1024 ;


	char cmd1[100] = "fpa 0xF0001000";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cmd1, WHITESPACE, args, &numOfArgs) ;

	int pa = FindPhysicalAddress(args) ;
	if (pa != 0x1000)
	{
		cprintf("[EVAL] #1 FindPhysicalAddress Failed.\n");
		return 0;
	}

	char cmd2[100] = "fpa 0xF0100000";
	strsplit(cmd2, WHITESPACE, args, &numOfArgs) ;

	pa = FindPhysicalAddress(args) ;
	if (pa != 0x100000)
	{
		cprintf("[EVAL] #2 FindPhysicalAddress Failed.\n");
		return 0;
	}

	char cmd3[100] = "fpa 0x100";
	strsplit(cmd3, WHITESPACE, args, &numOfArgs) ;

	pa = FindPhysicalAddress(args) ;
	if (pa != -1)
	{
		cprintf("[EVAL] #3 FindPhysicalAddress Failed.\n");
		return 0;
	}



	char cmd4[100] = "fpa 0xF0000005";
	strsplit(cmd4, WHITESPACE, args, &numOfArgs) ;

	 pa = FindPhysicalAddress(args) ;
	if (pa != 5)
	{
		cprintf("[EVAL] #4 FindPhysicalAddress Failed.\n");
		return 0;
	}

	char cmd5[100] = "fpa 0xF0100555";
	strsplit(cmd5, WHITESPACE, args, &numOfArgs) ;

	pa = FindPhysicalAddress(args) ;
	if (pa != 0x100555)
	{
		cprintf("[EVAL] #5 FindPhysicalAddress Failed. \n");
		return 0;
	}

	char acmd1[100] = "cvp 0x0 0x100000";
	execute_command(acmd1);

	char cmd6[100] = "fpa 0x100";
	strsplit(cmd6, WHITESPACE, args, &numOfArgs) ;

	pa = FindPhysicalAddress(args) ;
	if (pa != 0x100100)
	{
		cprintf("[EVAL] #6 FindPhysicalAddress Failed.\n");
		return 0;
	}


	char acmd2[100] = "dvp 0xF0F00000";
	execute_command(acmd2);

	char cmd7[100] = "fpa 0xF0F00F00";
	strsplit(cmd7, WHITESPACE, args, &numOfArgs) ;

	pa = FindPhysicalAddress(args) ;
	if (pa != -1)
	{
		cprintf("[EVAL] #7 FindPhysicalAddress Failed. \n");
		return 0;
	}

	cprintf("[EVAL] Q1 Automatic Test: Succeeded. \n");

	return 0;
}

int TestAss3Q2()
{
	char *ptr1, *ptr2, *ptr3;


	char cmd1[100] = "srp F0000000 40000000 256 w";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cmd1, WHITESPACE, args, &numOfArgs) ;

	ShareRangeWithPermissions(args) ;
	ptr1 = (char*)0xF0000000; *ptr1 = 'A' ;
	ptr2 = (char*)0x40000000;
	if (CB(0x40000000,0) <= 0)
	{
		cprintf("[EVAL] Test Failed #1. \n");
		return 0;
	}

	if(CheckFrameNumbersInRange(0xF0000000, 0x40000000, 256) < 0)
	{
		cprintf("[EVAL] Test Failed #1. \n");
		return 0;
	}

	if ((*ptr1) != 'A' || (*ptr2) != 'A')
	{
		cprintf("[EVAL] #1 ShareRangeWithPermissions: Failed.\n");
		return 0;
	}

	if (CB(0x41000000,0) <= 0 || CB(0x41000000,1) <= 0)
	{
		cprintf("[EVAL] Test Failed #2. \n");
		return 0;
	}

	ptr1 = (char*)0x41000000; *ptr1 = 'C' ;
	ptr2 = (char*)0xF1000000;

	if ((*ptr1) != 'C' || (*ptr2) != 'C')
	{
		cprintf("[EVAL] #2 ShareRangeWithPermissions: Failed.\n");

		return 0;
	}


	char cmd2[100] = "srp F0000000 80000000 128 r";
	strsplit(cmd2, WHITESPACE, args, &numOfArgs) ;

	ShareRangeWithPermissions(args) ;
	ptr1 = (char*)0xF0000000;
	ptr2 = (char*)0x81000000;

	if (CB(0x81000000,0) <= 0)
	{
		cprintf("[EVAL] Test Failed #3. \n");
		return 0;
	}

	if(CheckFrameNumbersInRange(0xF0000000, 0x80000000, 128) < 0)
	{
		cprintf("[EVAL] Test Failed #3. \n");
		return 0;
	}

	if ((*ptr1) != 'A' || (*ptr2) != 'C')
	{
		cprintf("[EVAL] #3 ShareRangeWithPermissions: Failed. \n");
		return 0;
	}

	if (CB(0x81800000, 1) != 0 || CB(0x81000000, 1) != 0)
	{
		cprintf("[EVAL] #4 ShareRangeWithPermissions: Failed. \n");
		return 0;
	}

	cprintf("[EVAL] Q2 Automatic Test: Succeeded. \n");
	return 0;
}

int TestAss3Q3()
{
	int kilo = 1024 ;
	int mega = 1024*1024 ;
	ClearUserSpace();

	char cmd1[100] = "fv 1";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cmd1, WHITESPACE, args, &numOfArgs) ;

	int va = FindVirtualOfFrameNum(args) ;
	if (va  != 0xF0001000)
	{
		cprintf("[EVAL] #1 FindVirtualOfFrameNum: Failed. \n");
		return 0;
	}


	char acmd1[100] = "cvp 0x00004000 0x1000";
	execute_command(acmd1);

	char cmd2[100] = "fv 1";
	strsplit(cmd2, WHITESPACE, args, &numOfArgs) ;

	va = FindVirtualOfFrameNum(args) ;
	if (va  != 0x00004000)
	{
		cprintf("[EVAL] #2 FindVirtualOfFrameNum: Failed. \n");
		return 0;
	}

	char cmd3[100] = "fv 100";
	strsplit(cmd3, WHITESPACE, args, &numOfArgs) ;

	va = FindVirtualOfFrameNum(args) ;
	if (va  != 0xF0064000)
	{
		cprintf("[EVAL] #3 FindVirtualOfFrameNum: Failed. \n");
		return 0;
	}

	char acmd2[100] = "dvp 0xF0064000";
	execute_command(acmd2);

	char cmd4[100] = "fv 100";
	strsplit(cmd4, WHITESPACE, args, &numOfArgs) ;


	va = FindVirtualOfFrameNum(args) ;
	if (va  != -1)
	{
		cprintf("Wrong address is %x\n",va);
		cprintf("[EVAL] #4 FindVirtualOfFrameNum: Failed. \n");
		return 0;
	}

	cprintf("[EVAL] Q3 Automatic Test: Succeeded.\n");

	return 0;
}

int TestAss3QB()
{
	//[1] Connect with write and user permissions
	char cmd1[100] = "cpf 0 768 w u";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cmd1, WHITESPACE, args, &numOfArgs) ;

	int ref1 = frames_info[768].references;
	uint32 entry = ConnectPageToFrame(args) ;
	char *ptr1, *ptr2, *ptr3;

	if (CB(0,0) <= 0)
	{
		cprintf("[EVAL] Test Failed #1. \n");
		return 0;
	}

	ptr1 = (char*)0x0; *ptr1 = 'A' ;
	int ref2 = frames_info[768].references;

	if ((ref2 - ref1) != 0)
	{
		cprintf("[EVAL] #1 ConnectPageToFrame: Failed. You should manually implement the connection logic using paging data structures ONLY. [DON'T update the references]. \n");
		return 0;
	}

	uint32 f = entry & 0xFFFFF000 ;
	uint32 p = entry & 0x00000FFF ;
	if (*ptr1 != 'A' || (f != 0x00300000) || (p != 0xE07))
	{
		cprintf("[EVAL] #2 ConnectPageToFrame: Failed. \n");
		return 0;
	}



	char cmd2[100] = "cpf 981000 768 w s";
	strsplit(cmd2, WHITESPACE, args, &numOfArgs) ;

	ref1 = frames_info[768].references;
	entry = ConnectPageToFrame(args) ;

	if (CB(0xEF808000,0) <= 0 || CB(0xEF808000,1) <= 0)
	{
		cprintf("[EVAL] Test Failed #2. \n");
		return 0;
	}

	ptr2 = (char*)0xEF808000;
	ref2 = frames_info[768].references;

	if ((ref2 - ref1) != 0)
	{
		cprintf("[EVAL] #3 ConnectPageToFrame: Failed. You should manually implement the connection logic using paging data structures ONLY. [DON'T update the references].\n");
		return 0;
	}

	f = entry & 0xFFFFF000 ;
	p = entry & 0x00000FFF ;
	if (*ptr1 != 'A' || *ptr2 != 'A' || (f != 0x00300000) || (p != 0xE03))
	{
		cprintf("[EVAL] #4 ConnectPageToFrame: Failed. \n");
		return 0;
	}

	*ptr2 = 'B';


	//[3] Connect with read and user permissions
	char cmd3[100] = "cpf 4 768 r u";
	strsplit(cmd3, WHITESPACE, args, &numOfArgs) ;

	ref1 = frames_info[768].references;
	entry = ConnectPageToFrame(args) ;

	if (CB(0x00004000,0) <= 0 )
	{
		cprintf("[EVAL] Test Failed #3. \n");
		return 0;
	}

	ptr3 = (char*)0x00004000;
	ref2 = frames_info[768].references;

	if ((ref2 - ref1) != 0)
	{
		cprintf("[EVAL] #5 ConnectPageToFrame: Failed. You should manually implement the connection logic using paging data structures ONLY. [DON'T update the references]. \n");
		return 0;
	}

	f = entry & 0xFFFFF000 ;
	p = entry & 0x00000FFF ;
	if (*ptr1 != 'B' || *ptr2 != 'B' || *ptr3 != 'B' || (f != 0x00300000) || (p != 0xE05))
	{
		cprintf("[EVAL] #6 ConnectPageToFrame: Failed. \n");
		return 0;
	}

	cprintf("[EVAL] BONUS Automatic Test: Succeeded. \n");

	return 0;
}

int CB(uint32 va, int bn)
{
	uint32 *ptr_table = NULL;
	uint32 mask = 1<<bn;
	get_page_table(ptr_page_directory, (void*)va, 0, &ptr_table);
	if (ptr_table == NULL) return -1;
	return (ptr_table[PTX(va)] & mask) == mask ? 1 : 0 ;
}

void ClearUserSpace()
{
	for (int i = 0; i < PDX(USER_TOP); ++i) {
		ptr_page_directory[i] = 0;
	}
}

int CheckFrameNumbersInRange(uint32 ptr1, uint32 ptr2, uint32 size)
{
	uint32 totalNumberOfPages = (size*1024*1024)/PAGE_SIZE;
	void* ptrTemp1 = (void*)ptr1;
	void* ptrTemp2 = (void*)ptr2;

	for(int i = 0;i<totalNumberOfPages;i++)
	{
		uint32* ptr_table1;
		uint32* ptr_table2;

		get_page_table(ptr_page_directory, ptrTemp1, 0, &ptr_table1);
		get_page_table(ptr_page_directory, ptrTemp2, 0, &ptr_table2);
		if (ptr_table1 == NULL)
		{
			cprintf("[EVAL] #1 ShareRangeWithPermissions: Failed. Table of address 1 = NULL\n");
			return -1;
		}
		if (ptr_table2 == NULL)
		{
			cprintf("[EVAL] #1 ShareRangeWithPermissions: Failed. Table of address 2 = NULL\n");
			return -1;
		}
		if(ptr_table1[PTX(ptrTemp1)] >> 12 != ptr_table2[PTX(ptrTemp2)] >> 12)
		{
			cprintf("[EVAL] #1 ShareRangeWithPermissions: Failed. Frame numbers not equal in the whole range\n");
			return -1;
		}
		ptrTemp1 += PAGE_SIZE;
		ptrTemp2 += PAGE_SIZE;
	}

	return 0;
}
