#include <kern/tests.h>

//define the white-space symbols
#define WHITESPACE "\t\r\n "

void TestAssignment2()
{
//	cprintf("\n========================\n");
//	cprintf("Automatic Testing of Q1:\n");
//	cprintf("========================\n");
//	TestAss2Q1();
//	cprintf("\n========================\n");
//	cprintf("Automatic Testing of Q2:\n");
//	cprintf("========================\n");
//	TestAss2Q2();
//	cprintf("\n========================\n");
//	cprintf("Automatic Testing of Q3:\n");
//	cprintf("========================\n");
//	TestAss2Q3();
//	cprintf("\n========================\n");
//	cprintf("Automatic Testing of Q4:\n");
//	cprintf("========================\n");
//	TestAss2Q4();
	cprintf("\n===========================\n");
	cprintf("Automatic Testing of BONUS:\n");
	cprintf("===========================\n");
	TestAss2BONUS();
}

int TestAss2Q1()
{
	int retValue = 1;
	int i = 0;
	//Create first account
	char cr1[100] = "csa Ali 2 1 2 3";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int* ptr1 = CreateAccount(numOfArgs, args) ;
	assert(ptr1 == (int*)0xF1000000);

	//Check elements of 1st array
	int expectedArr1[] = {1, 2, 3};
	if (!CheckArrays(expectedArr1, ptr1, 3))
	{
		cprintf("[EVAL] #1 CreateAccount: Failed\n");
		return retValue;
	}

	//Create second account
	char cr2[100] = "csa Mona 1 4 5 7 8";
	numOfArgs = 0;
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	int* ptr2 = CreateAccount(numOfArgs,args) ;
	assert(ptr2 == (int*)0xF100000C);

	//Check elements of 2nd account
	int expectedArr2[] = {4, 5, 7, 8};
	if (!CheckArrays(expectedArr2, ptr2, 4))
	{
		cprintf("[EVAL] #2 CreateAccount: Failed\n");
		return retValue;
	}

	//Check elements of 1st account
	if (!CheckArrays(expectedArr1, ptr1, 3))
	{
		cprintf("[EVAL] #3 CreateAccount: Failed\n");
		return retValue;
	}

	//Create third account
	char cr3[100] = "csa Lara 4 8 9";
	numOfArgs = 0;
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	int* ptr3 = CreateAccount(numOfArgs,args) ;
	assert(ptr3 == (int*)0xF100001C);

	//Check elements of 3rd account
	int expectedArr3[] = {8, 9};
	if (!CheckArrays(expectedArr3, ptr3, 2))
	{
		cprintf("[EVAL] #4 CreateAccount: Failed\n");
		return retValue;
	}
	//Check elements of 2nd account
	if (!CheckArrays(expectedArr2, ptr2, 4))
	{
		cprintf("[EVAL] #5 CreateAccount: Failed\n");
		return retValue;
	}

	//Check elements of 1st array
	if (!CheckArrays(expectedArr1, ptr1, 3))
	{
		cprintf("[EVAL] #6 CreateAccount: Failed\n");
		return retValue;
	}

	cprintf("[EVAL] CreateAccount: Succeeded\n");

	return retValue;
}

int TestAss2Q2()
{
	//Create first array
	char cr1[100] = "csa Ahmed 2 10 20 30 40";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	CreateAccount(numOfArgs, args) ;

	//Create second array
	char cr2[100] = "csa Alaa 4 1 3 2 5 6";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	CreateAccount(numOfArgs, args) ;

	//Get (Exist)
	char f1[100] = "gnc Ahmed";
	strsplit(f1, WHITESPACE, args, &numOfArgs) ;
	int ret = GetNumberOfCourses(args) ;
	if (ret != 4)
	{
		cprintf("[EVAL] #1 GetNumberOfCourses: Failed\n");
		return 1;
	}

	//Get (Exist)
	char f2[100] = "gnc Alaa";
	strsplit(f2, WHITESPACE, args, &numOfArgs) ;
	ret = GetNumberOfCourses(args) ;
	if (ret != 5)
	{
		cprintf("[EVAL] #2 GetNumberOfCourses: Failed\n");
		return 1;
	}

	//Create third array
	char cr3[100] = "csa Omar 1 4 5 6";
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	CreateAccount(numOfArgs, args) ;

	//Get (Exist)
	char f3[100] = "gnc Omar";
	strsplit(f3, WHITESPACE, args, &numOfArgs) ;
	ret = GetNumberOfCourses(args) ;
	if (ret != 3)
	{
		cprintf("[EVAL] #3 GetNumberOfCourses: Failed\n");
		return 1;
	}

	//Get from the three arrays
	char f4[100] = "gnc Ahmed";
	strsplit(f4, WHITESPACE, args, &numOfArgs) ;
	ret = GetNumberOfCourses(args) ;
	if (ret != 4)
	{
		cprintf("[EVAL] #4 GetNumberOfCourses: Failed\n");
		return 1;
	}

	char f5[100] = "gnc Alaa";
	strsplit(f5, WHITESPACE, args, &numOfArgs) ;
	ret = GetNumberOfCourses(args) ;
	if (ret != 5)
	{
		cprintf("[EVAL] #5 GetNumberOfCourses: Failed\n");
		return 1;
	}
	char f6[100] = "gnc Omar";
	strsplit(f6, WHITESPACE, args, &numOfArgs) ;
	ret = GetNumberOfCourses(args) ;
	if (ret != 3)
	{
		cprintf("[EVAL] #6 GetNumberOfCourses: Failed\n");
		return 1;
	}

	//check un-existing account
	char f7[100] = "gnc Hala";
	strsplit(f7, WHITESPACE, args, &numOfArgs) ;
	ret = GetNumberOfCourses(args) ;
	if (ret != 0)
	{
		cprintf("[EVAL] #7 GetNumberOfCourses: Failed\n");
		return 1;
	}

	cprintf("[EVAL] GetNumberOfCourses: Succeeded\n");

	return 1;
}

int TestAss2Q3()
{
	int retValue = 1;
	int i = 0;
	//Create first account
	char cr1[100] = "csa Ola 2 1 2 3 4";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int* ptr1 = CreateAccount(numOfArgs,args) ;
	assert(ptr1 >= (int*)0xF1000000);

	//Create second account
	char cr2[100] = "csa Amged 1 5 6 7 8";
	numOfArgs = 0;
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	int* ptr2 = CreateAccount(numOfArgs,args) ;
	assert(ptr2 >= (int*)0xF1000000);

	//Create third account
	char cr3[100] = "csa Nora 2 9 10 11 12";
	numOfArgs = 0;
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	int* ptr3 = CreateAccount(numOfArgs,args) ;
	assert(ptr3 >= (int*)0xF1000000);

	//Switch: Test1
	char cr4[100] = "scs Ola Amged";
	numOfArgs = 0;
	strsplit(cr4, WHITESPACE, args, &numOfArgs) ;

	SwitchCourses(args) ;

	int expectedArr1[] = {5, 6, 7, 8};
	cprintf("Pointer 1 address : %x\n",ptr1);
	if (!CheckArrays(expectedArr1, ptr1, 4))
	{
		cprintf("[EVAL] #1 SwitchCourses: Failed\n");
		return 1;
	}

	int expectedArr1_1[] = {1, 2, 3, 4};
	if (!CheckArrays(expectedArr1_1, ptr2, 4))
	{
		cprintf("[EVAL] #2 SwitchCourses: Failed\n");
		return 1;
	}

	//Switch: Test2
	char cr5[100] = "scs Amged Nora";
	numOfArgs = 0;
	strsplit(cr5, WHITESPACE, args, &numOfArgs) ;

	SwitchCourses(args) ;

	int expectedArr2[] = {9, 10, 11, 12};
	if (!CheckArrays(expectedArr2, ptr2, 4))
	{
		cprintf("[EVAL] #3 SwitchCourses: Failed\n");
		return 1;
	}

	int expectedArr2_1[] = {1, 2, 3, 4};
	if (!CheckArrays(expectedArr2_1, ptr3, 4))
	{
		cprintf("[EVAL] #4 SwitchCourses: Failed\n");
		return 1;
	}

	//Switch: Test3
	char cr6[100] = "scs Ola Nora";
	numOfArgs = 0;
	strsplit(cr6, WHITESPACE, args, &numOfArgs) ;

	SwitchCourses(args) ;

	int expectedArr3[] = {5, 6, 7, 8};
	if (!CheckArrays(expectedArr3, ptr3, 4))
	{
		cprintf("[EVAL] #5 SwitchCourses: Failed\n");
		return 1;
	}

	int expectedArr3_1[] = {1, 2, 3, 4};
	if (!CheckArrays(expectedArr3_1, ptr1, 4))
	{
		cprintf("[EVAL] #6 SwitchCourses: Failed\n");
		return 1;
	}


	cprintf("[EVAL] SwitchCourses: Succeeded\n");

	return retValue;}

int TestAss2Q4()
{
	int retValue = 1;
	int i = 0;
	//Create first account
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	char cr1[100] = "csa Mariam 3 2 5 6";
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;
	int* ptr1 = CreateAccount(numOfArgs, args) ;
	assert(ptr1 >= (int*)0xF1000000);

	//check the course
	char ie1[100] = "ie Mariam 2";
	strsplit(ie1, WHITESPACE, args, &numOfArgs) ;

	int is_enrolled = IsEnrolled(args) ;
	if (is_enrolled != 1)
	{
		cprintf("[EVAL] #1 IsEnrolled: Failed\n");
		return retValue;
	}

	char ie2[100] = "ie Mariam 1";
	strsplit(ie2, WHITESPACE, args, &numOfArgs) ;

	is_enrolled = IsEnrolled(args);
	if (is_enrolled != 0)
	{
		cprintf("[EVAL] #2 IsEnrolled: Failed\n");
		return retValue;
	}

	//Create second account
	char cr2[100] = "csa Farah 4 1 3 2 ";
	numOfArgs = 0;
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	int* ptr2 = CreateAccount(numOfArgs, args) ;
	assert(ptr2 >= (int*)0xF1000000);

	//check the course
	char ie3[100] = "ie Farah 4";
	strsplit(ie3, WHITESPACE, args, &numOfArgs) ;

	is_enrolled = IsEnrolled(args) ;
	if (is_enrolled != 0)
	{
		cprintf("[EVAL] #3 IsEnrolled: Failed\n");
		return retValue;
	}

	char ie4[100] = "ie Farah 3";
	strsplit(ie4, WHITESPACE, args, &numOfArgs) ;

	is_enrolled = IsEnrolled(args) ;
	if (is_enrolled != 1)
	{
		cprintf("[EVAL] #4 IsEnrolled: Failed\n");
		return retValue;
	}

	//Create third account
	char cr3[100] = "csa Farag 4 9 10";
	numOfArgs = 0;
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	int* ptr3 = CreateAccount(numOfArgs, args) ;
	assert(ptr3 >= (int*)0xF1000000);

	//check the course
	char ie5[100] = "ie Farag 10";
	strsplit(ie5, WHITESPACE, args, &numOfArgs) ;

	is_enrolled = IsEnrolled(args) ;
	if (is_enrolled != 1)
	{
		cprintf("[EVAL] #5 IsEnrolled: Failed\n");
		return retValue;
	}

	//check the course
	char ie6[100] = "ie Farag 1";
	strsplit(ie6, WHITESPACE, args, &numOfArgs) ;

	is_enrolled = IsEnrolled(args) ;
	if (is_enrolled != 0)
	{
		cprintf("[EVAL] #6 IsEnrolled: Failed\n");
		return retValue;
	}

	//check the course
	char ie7[100] = "ie Farag 9";
	strsplit(ie7, WHITESPACE, args, &numOfArgs) ;

	is_enrolled = IsEnrolled(args) ;
	if (is_enrolled != 1)
	{
		cprintf("[EVAL] #7 IsEnrolled: Failed\n");
		return retValue;
	}

	//check un-existing account
	char ie8[100] = "ie Mona 9";
	strsplit(ie8, WHITESPACE, args, &numOfArgs) ;

	is_enrolled = IsEnrolled(args) ;
	if (is_enrolled != 0)
	{
		cprintf("[EVAL] #8 IsEnrolled: Failed\n");
		return retValue;
	}

	cprintf("[EVAL] IsEnrolled: Succeeded\n");
	return 1;
}

int TestAss2BONUS()
{
	int retValue = 1;
	int i = 0;
	/*CASE1: Create 1 array then delete it*/
	//Create first array
	char cr1[100] = "csa ali 4 1 2 3";
	int numOfArgs = 0;
	char *args[MAX_ARGUMENTS] ;
	strsplit(cr1, WHITESPACE, args, &numOfArgs) ;

	int* ptr1 = CreateAccount(numOfArgs, args) ;
	assert(ptr1 >= (int*)0xF1000000);

	//Delete it
	char dl1[100] = "dnia ali";
	strsplit(dl1, WHITESPACE, args, &numOfArgs) ;
	DeleteAccount(args);

	//Create another array (should overwrite the 1st one!)
	char cr2[100] = "csa clara 3 60 61 62 63";
	strsplit(cr2, WHITESPACE, args, &numOfArgs) ;

	int* ptr2 = CreateAccount(numOfArgs, args) ;

	//Access 1st element of 1st array (it should retrieve the one of the 2nd array)
	if (ptr1[0] != 60 || ptr1 != ptr2 )
	{
		cprintf("[EVAL] #1 DeleteAccount: Failed\n");
		return retValue;
	}

	//Get element from 1st array and from 2nd array
	char f1[100] = "gnc ali";
	strsplit(f1, WHITESPACE, args, &numOfArgs) ;
	int ret = GetNumberOfCourses(args) ;
	if (ret != 0)
	{
		cprintf("[EVAL] #2 DeleteAccount: Failed\n");
		return 1;
	}
	char f2[100] = "ie clara 62";
	strsplit(f2, WHITESPACE, args, &numOfArgs) ;
	ret = IsEnrolled(args);
	if (ret != 1)
	{
		cprintf("[EVAL] #3 DeleteAccount: Failed\n");
		return 1;
	}

	char f3[100] = "gnc clara";
	strsplit(f3, WHITESPACE, args, &numOfArgs) ;
	ret = GetNumberOfCourses(args) ;
	if (ret != 4)
	{
		cprintf("[EVAL] #4 DeleteAccount: Failed\n");
		return 1;
	}


	/*CASE2: Create three arrays then delete one of them*/
	//Create second array
	char cr3[100] = "csa Lina 3 1 9 5 50";
	numOfArgs = 0;
	strsplit(cr3, WHITESPACE, args, &numOfArgs) ;

	int* ptr3 = CreateAccount(numOfArgs, args) ;

	//Create third array
	char cr4[100] = "csa Ramy 7 10 2 3 4 5 6 7";
	numOfArgs = 0;
	strsplit(cr4, WHITESPACE, args, &numOfArgs) ;

	int* ptr4 = CreateAccount(numOfArgs, args) ;

	//Delete the 1st array
	char dl2[100] = "dnia clara";
	strsplit(dl2, WHITESPACE, args, &numOfArgs) ;
	DeleteAccount(args);

	//Access 1st element of 1st and 2nd arrays (it should retrieve the one of the 2nd and 3rd arrays)
	if (ptr2[0] != 1 || ptr2[1] != 9 || ptr2[2] != 5 || ptr2[3] != 50)
	{
		cprintf("[EVAL] #5 DeleteAccount: Failed\n");
		return retValue;
	}
	if(ptr3[0] != 10 || ptr3[1] != 2 || ptr3[2] != 3 || ptr3[3] != 4 || ptr3[4] != 5 || ptr3[5] != 6 || ptr3[6] != 7)
	{
		cprintf("[EVAL] #6 DeleteAccount: Failed\n");
		return retValue;
	}

	//Get element from 1st (deleted), 2nd and 3rd arrays
	char f4[100] = "ie clara 11";
	strsplit(f4, WHITESPACE, args, &numOfArgs) ;
	ret = IsEnrolled(args) ;
	if (ret != 0)
	{
		cprintf("[EVAL] #7 DeleteAccount: Failed\n");
		return 1;
	}
	char f5[100] = "gnc Ramy";
	strsplit(f5, WHITESPACE, args, &numOfArgs) ;
	ret = GetNumberOfCourses(args) ;
	if (ret != 7)
	{
		cprintf("[EVAL] #8 DeleteAccount: Failed\n");
		return 1;
	}
	char f6[100] = "ie Ramy 2";
	strsplit(f6, WHITESPACE, args, &numOfArgs) ;
	ret = IsEnrolled(args) ;
	if (ret != 1)
	{
		cprintf("[EVAL] #9 DeleteAccount: Failed\n");
		return 1;
	}
	char f7[100] = "gnc Lina";
	strsplit(f7, WHITESPACE, args, &numOfArgs) ;
	ret = GetNumberOfCourses(args) ;
	if (ret != 4)
	{
		cprintf("[EVAL] #10 DeleteAccount: Failed\n");
		return 1;
	}

	char f8[100] = "ie Lina 50";
	strsplit(f8, WHITESPACE, args, &numOfArgs) ;
	ret = IsEnrolled(args) ;
	if (ret != 1)
	{
		cprintf("[EVAL] #11 DeleteAccount: Failed\n");
		return 1;
	}
	char f9[100] = "ie Lina 62";
	strsplit(f9, WHITESPACE, args, &numOfArgs) ;
	ret = IsEnrolled(args) ;
	if (ret != 0)
	{
		cprintf("[EVAL] #12 DeleteAccount: Failed\n");
		return 1;
	}
	cprintf("[EVAL][BONUS] DeleteAccount: Succeeded\n");

	return retValue;
}

//========================================================
int CheckArrays(int *expectedArr, int *actualArr, int N)
{
	int equal = 1 ;
	cprintf("ActualArray : ");
	for(int i=0;i<N;i++){
		cprintf("%d , ",actualArr[i]);
	}
	cprintf("\n");
	for(int i = 0; i < N; i++)
	{
		cprintf(" %d:%d \n",expectedArr[i],actualArr[i]);
		if(expectedArr[i] != actualArr[i])
			return 0;
	}
	return equal;
}

