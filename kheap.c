#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>

//2022: NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
#define maxKernelHeapPages ((KERNEL_HEAP_MAX - KERNEL_HEAP_START) / PAGE_SIZE) +1

struct KernelHeapPages{
	uint32 startAddress;
	bool isEmpty;
}kernelHeapPages[maxKernelHeapPages];
uint32 lastAllocatedKHpage = KERNEL_HEAP_START;
bool isKHpageEmpty = 1;

void initializaKernelHeapPages(){
//	cprintf("Hello in Kmalloc\n");
	int heapPageIndex;
	for(int i=KERNEL_HEAP_START;i<KERNEL_HEAP_MAX;i+=PAGE_SIZE){
		heapPageIndex=(i-KERNEL_HEAP_START)/PAGE_SIZE;
		kernelHeapPages[heapPageIndex].startAddress = i;
		kernelHeapPages[heapPageIndex].isEmpty=1;
	}
	//return it false to keep the Kernel heap pages the same without zeros
	isKHpageEmpty=0;
}
void *getStartAddress(uint32 start,uint32 end,uint32 size){
	uint32 freeSize=0;
	while(start<end){
		if (kernelHeapPages[(start - KERNEL_HEAP_START) / PAGE_SIZE].isEmpty){
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
	//loop from the lastAllocated Kernel heap page to kernel heap max
	uint32 startAddress=(uint32)getStartAddress(lastAllocatedKHpage,KERNEL_HEAP_MAX,size);
	if(startAddress!=0){
		return (void*)startAddress;
	}
	//loop from the Kernel heap start to last allocated Kernel heap page
	startAddress=(uint32)getStartAddress(KERNEL_HEAP_START,lastAllocatedKHpage,size);
	if(startAddress!=0){
		return (void*)startAddress;
	}
	return 0;	//there is no enough space in the heap
}
struct Location{
	uint32 startAddress;
	uint32 endAddress;
	int freeFrames;
};
void *bestFit(uint32 size){
	uint32 freeSize=0;
	struct Location bestLocation;
	bestLocation.freeFrames=maxKernelHeapPages;
	bestLocation.startAddress=0;
	bestLocation.endAddress=0;
	int freeFrames=0;
	struct Location block;
	block.freeFrames=-1;
	block.startAddress=0;
	block.endAddress=0;
	int targetFrames=size/PAGE_SIZE;
//	cprintf("Target frames:%d\n",targetFrames);
	uint32 start=KERNEL_HEAP_START;
	while(start<KERNEL_HEAP_MAX){
		if (kernelHeapPages[(start - KERNEL_HEAP_START) / PAGE_SIZE].isEmpty){
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
	return (block.freeFrames < targetFrames)? 0:(void*)(bestLocation.startAddress +(PAGE_SIZE *targetFrames)) - size ;
}
void* kmalloc(unsigned int size)
{
	//TODO: [PROJECT 2022 - [1] Kernel Heap] kmalloc()
	// Write your code here, remove the panic and write your code
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");

	//===================================================================================================
	//NOTE: Allocation using NEXTFIT strategy
	//NOTE: All kernel heap allocations are multiples of PAGE_SIZE (4KB)
	//refer to the project presentation and documentation for details
	size = ROUNDUP(size, PAGE_SIZE);
	uint32 startAddress=0;
	if(isKHpageEmpty){
		initializaKernelHeapPages();//initialize kernel heap pages
	}
	if(isKHeapPlacementStrategyNEXTFIT()){
		//kmalloc with next fit
		 startAddress = (uint32)nextFit(size);
	}
	//kmalloc with best fit
	else{
//		cprintf("Run with Best fit\n");
		startAddress = (uint32)bestFit(size);
	}

//	cprintf("Start Address: %x\n",startAddress);
	if (!startAddress){
//		cprintf("Successful kmalloc with no space \n");
		return 0;	//there is no enough space
	}
	uint32 tempStartAddress = startAddress;
	struct Frame_Info *ptr_frame_info ;
	int kernelHeapPageIndex;
	int numberOfPages=size/PAGE_SIZE;
	while(numberOfPages--){
		ptr_frame_info=0;
		allocate_frame(&ptr_frame_info);
		map_frame(ptr_page_directory, ptr_frame_info, (void*)tempStartAddress, PERM_PRESENT | PERM_WRITEABLE);
		kernelHeapPageIndex=(tempStartAddress - KERNEL_HEAP_START) / PAGE_SIZE;
		kernelHeapPages[kernelHeapPageIndex].startAddress = startAddress;
		kernelHeapPages[kernelHeapPageIndex].isEmpty=0;
		tempStartAddress += PAGE_SIZE;
	}
	lastAllocatedKHpage = startAddress + PAGE_SIZE * (size/PAGE_SIZE);
//	cprintf("Successful kmalloc with Start Address: %x\n",startAddress);
	return (void*)startAddress;
}

void kfree(void* virtual_address)
{
	//TODO: [PROJECT 2022 - [2] Kernel Heap] kfree()
	// Write your code here, remove the panic and write your code
//	panic("kfree() is not implemented yet...!!");

	uint32 Start = (uint32)virtual_address;
	while(Start<KERNEL_HEAP_MAX){
		uint32 heapPageIndex=(Start -KERNEL_HEAP_START )/ PAGE_SIZE;
		if(kernelHeapPages[heapPageIndex].isEmpty == 0
				&& kernelHeapPages[heapPageIndex].startAddress==(uint32)virtual_address){
			unmap_frame(ptr_page_directory, (void*)Start);
			kernelHeapPages[heapPageIndex].startAddress = Start;
			kernelHeapPages[heapPageIndex].isEmpty=1;
			Start+=PAGE_SIZE;
		}
		else return;
	}
	//you need to get the size of the given allocation using its address
	//refer to the project presentation and documentation for details

}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [PROJECT 2022 - [3] Kernel Heap] kheap_virtual_address()
	// Write your code here, remove the panic and write your code
//	panic("kheap_virtual_address() is not implemented yet...!!");

	//return the virtual address corresponding to given physical_address
	//refer to the project presentation and documentation for details

	//change this "return" according to your answer
	struct Frame_Info* frameInfo ;
	uint32* ptr_page_table;
	for(uint32 i = KERNEL_HEAP_START ; i<=lastAllocatedKHpage ; i+=PAGE_SIZE){
		frameInfo= NULL;
		frameInfo = get_frame_info(ptr_page_directory, (void*)i, &ptr_page_table);
		if(frameInfo != NULL && to_physical_address(frameInfo)==physical_address){
			return i;
		}
	}
	return 0;
}
unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [PROJECT 2022 - [4] Kernel Heap] kheap_physical_address()
	// Write your code here, remove the panic and write your code
//	panic("kheap_physical_address() is not implemented yet...!!");

	//return the physical address corresponding to given virtual_address
	//refer to the project presentation and documentation for details
	//change this "return" according to your answer

	uint32* pageTableVA = NULL;
	get_page_table(ptr_page_directory, (void*)virtual_address, &pageTableVA);
	if(pageTableVA != NULL){
		//physical address= frame number * page size
		return (pageTableVA[PTX(virtual_address)]>>12) * PAGE_SIZE;
	}
	return 0;
}
