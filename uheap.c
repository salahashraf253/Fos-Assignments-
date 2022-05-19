
#include <inc/lib.h>

// malloc()
//	This function use NEXT FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.


//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
#define userHeapMaxPages (USER_HEAP_MAX - USER_HEAP_START) / PAGE_SIZE
struct KernelHeapPages{
	uint32 startAddress;
	bool isEmpty;
	uint32 size;
}userHeapPages[userHeapMaxPages];
uint32 lastAllocatedUHpage = USER_HEAP_START;
struct Location{
	uint32 startAddress;
	uint32 endAddress;
	int freeFrames;
};
bool isUserHeapEmpty=1;
void initializaKernelHeapPages(){
//	cprintf("Hello in malloc in user heap\n");
	int userHeapPageIndex;
	for(int i=USER_HEAP_START;i<USER_HEAP_MAX;i+=PAGE_SIZE){
		userHeapPageIndex=(i-USER_HEAP_START)/PAGE_SIZE;
		userHeapPages[userHeapPageIndex].startAddress = i;
		userHeapPages[userHeapPageIndex].isEmpty=1;
		userHeapPages[userHeapPageIndex].size=0;
	}
	//return it false to keep the User heap pages the same without zeros
	isUserHeapEmpty=0;
}
void *getStartAddress(uint32 start,uint32 end,uint32 size){
	uint32 freeSize=0;
	while(start<end){
		if (userHeapPages[(start - USER_HEAP_START) / PAGE_SIZE].isEmpty){
			freeSize += PAGE_SIZE;
			if (freeSize == size){
				start = (start+PAGE_SIZE)-size;
				return (void*) start;
			}
		}
		else{
			freeSize = 0;
		}
		start += PAGE_SIZE;
	}
	return 0;
}
void *nextFit(uint32 size){
	//loop from the lastAllocated User heap page to kernel heap max
	uint32 startAddress=(uint32)getStartAddress(lastAllocatedUHpage,USER_HEAP_MAX,size);
	if(startAddress!=0){
		return (void*)startAddress;
	}
	//loop from the Kernel heap start to last allocated User heap page
	startAddress=(uint32)getStartAddress(USER_HEAP_START,lastAllocatedUHpage,size);
	if(startAddress!=0){
		return (void*)startAddress;
	}
	return 0;	//there is no enough space in the User Kernel
}
void *bestFit(uint32 size){
	uint32 freeSize=0;
	struct Location bestLocation;
	bestLocation.freeFrames=(USER_HEAP_MAX - USER_HEAP_START )/ PAGE_SIZE+ 1;
	bestLocation.startAddress=0;
	bestLocation.endAddress=0;
	int freeFrames=0;
	struct Location block;
	block.freeFrames=-1;
	block.startAddress=0;
	block.endAddress=0;
	bool inElse=0;
	int targetFrames=size/PAGE_SIZE;
//	cprintf("Target frames:%d\n",targetFrames);
	uint32 start=USER_HEAP_START;
	while(start<USER_HEAP_MAX){
		if (userHeapPages[(start - USER_HEAP_START) / PAGE_SIZE].isEmpty){
			if(block.freeFrames==-1){
				block.startAddress=start;
				block.freeFrames=0;
			}
			block.freeFrames++;
		}
		else{
			if(block.freeFrames >= targetFrames && bestLocation.freeFrames > block.freeFrames){
				bestLocation=block;
			}
			block.freeFrames=-1;
		}
		start += PAGE_SIZE;
	}
	if(block.freeFrames >= targetFrames && bestLocation.freeFrames > block.freeFrames){
		bestLocation=block;
	}
	if(	block.freeFrames < targetFrames){
//		cprintf("no space : best fit\n");
		return 0;
	}
	uint32 retAddress=bestLocation.startAddress;
//	cprintf("return of best fit  : %x\n",(bestLocation.startAddress +(PAGE_SIZE *targetFrames)) - size);
	return  (void*)(bestLocation.startAddress +(PAGE_SIZE *targetFrames)) - size;
//	return (void*)((bestLocation.startAddress +(PAGE_SIZE *targetFrames))) ;
//	cprintf("return of best fit is : %x\n",bestLocation.startAddress+PAGE_SIZE);
//	return (void*)((bestLocation.startAddress+PAGE_SIZE));
}
void* malloc(uint32 size)
{
	//TODO: [PROJECT 2022 - [9] User Heap malloc()] [User Side]
	// Write your code here, remove the panic and write your code
	//panic("malloc() is not implemented yet...!!");

	// Steps:
	//	1) Implement NEXT FIT strategy to search the heap for suitable space
	//		to the required allocation size (space should be on 4 KB BOUNDARY)
	//	2) if no suitable space found, return NULL
	//	 Else,
	//	3) Call sys_allocateMem to invoke the Kernel for allocation
	// 	4) Return pointer containing the virtual address of allocated space,
	//

	//This function should find the space of the required range
	// ******** ON 4KB BOUNDARY ******************* //

	//Use sys_isUHeapPlacementStrategyNEXTFIT() and
	//sys_isUHeapPlacementStrategyBESTFIT() for the bonus
	//to check the current strategy

	if(isUserHeapEmpty){
		initializaKernelHeapPages();
	}
	size=ROUNDUP(size,PAGE_SIZE);
//	cprintf("Size in malloc : %d\n",size);
	uint32 start;
	if(sys_isUHeapPlacementStrategyBESTFIT()){
		/*to test using best fit write command: uhbestfit at the begining of run*/
//		cprintf("Malloc in user heap using best fit\n");
		start=(uint32)bestFit(size);
	}
	else if(sys_isUHeapPlacementStrategyNEXTFIT()){
//		cprintf("Malloc in user heap using NEXT fit\n");
		start=(uint32)nextFit(size);
	}
	if(!start){
//		cprintf("Successful malloc with no space \n");
		return 0;
	}
	sys_allocateMem(start, size); /*3) Call sys_allocateMem to invoke the Kernel for allocation*/
	uint32 tmpstart = start;
	uint32 tmpsize = size;
	uint32 userHeapPageIndex;
	while (tmpsize > 0){
		userHeapPageIndex=(tmpstart - USER_HEAP_START) / PAGE_SIZE;
		userHeapPages[userHeapPageIndex].startAddress = start;
		userHeapPages[userHeapPageIndex].isEmpty = 0;
		userHeapPages[userHeapPageIndex].size=size;
		tmpsize -= PAGE_SIZE;
		tmpstart += PAGE_SIZE;
	}
	lastAllocatedUHpage=start + PAGE_SIZE * (size/PAGE_SIZE);
//	cprintf("return of malloc : %x\n",start);
	return (void*)(start);

}

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	panic("smalloc() is not required ..!!");
	return NULL;
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	panic("sget() is not required ..!!");
	return 0;
}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address)
{
	//TODO: [PROJECT 2022 - [11] User Heap free()] [User Side]
	// Write your code here, remove the panic and write your code
	//panic("free() is not implemented yet...!!");

	//you shold get the size of the given allocation using its address
	//you need to call sys_freeMem()
	//refer to the project presentation and documentation for details
	uint32 va = (uint32)virtual_address;
	uint32 size = userHeapPages[((uint32)virtual_address - USER_HEAP_START) / PAGE_SIZE].size;
	sys_freeMem(va, size);
	uint32 userHeapPageIndex;
	struct KernelHeapPages emptyPage;
	emptyPage.size=0;
	emptyPage.isEmpty=1;
	while(size){
		userHeapPageIndex=(va - USER_HEAP_START) / PAGE_SIZE;
		emptyPage.startAddress=va;
		userHeapPages[userHeapPageIndex]=emptyPage;
		va += PAGE_SIZE;
		size-=PAGE_SIZE;
	}
	//------------------

}

void sfree(void* virtual_address)
{
	panic("sfree() is not requried ..!!");
}


//===============
// [2] realloc():
//===============

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_moveMem(uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		which switches to the kernel mode, calls moveMem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
//		in "memory_manager.c", then switch back to the user mode here
//	the moveMem function is empty, make sure to implement it.

void *realloc(void *virtual_address, uint32 new_size)
{
	//TODO: [PROJECT 2022 - BONUS3] User Heap Realloc [User Side]
	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");

	return NULL;
}
