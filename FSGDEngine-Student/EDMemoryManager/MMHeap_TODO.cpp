#include <fstream>
#include <chrono>
#include <iomanip>

#include "MMHeap.h"

namespace EDMemoryManager
{	
	Header * Heap::FindBlock(unsigned int allocSize)
	{
		//return FindBlockSolution(allocSize);
		Header* aux = Heap::firstHeader;
		while (true)
		{
			if (aux->size >= allocSize)
				return aux;
			aux = aux->next;

			if (aux == Heap::firstHeader)
				return nullptr;
		}
		
	}

	void Heap::Init(unsigned int poolSize, unsigned int _synchronize)
	{
		//synchronize = _synchronize;
		//InitializeCriticalSection(&criticalSection);
		//MM_HEAP_LOCK;

		// TODO-STUDENT - comment out the solution version and write your own, you can ignore the synchronization code
		//InitSolution(poolSize, _synchronize);

		Heap::pool = ((char*)malloc(poolSize));
		Heap::firstHeader = ((Header*)Heap::pool);
		//header stuff
		Heap::firstHeader->next = Heap::firstHeader;
		Heap::firstHeader->prev = Heap::firstHeader;
		Heap::firstHeader->size = poolSize - sizeof(Header) - sizeof(Footer);
		Heap::firstHeader->size &= ~(1 << 31); //free memory block

		Heap::lastFooter = ((Footer*)(((char*)Heap::pool) + poolSize - sizeof(Footer)));
		//foot stuff
		Heap::lastFooter->size = Heap::firstHeader->size;

		Heap::freeListEntry = Heap::firstHeader;
		Heap::poolSizeTotal = poolSize;
		Heap::synchronize = _synchronize;
		//MM_HEAP_UNLOCK;
	}

	void* Heap::Allocate(unsigned int allocSize)
	{
		// MM_HEAP_LOCK;

		//Header* free_head = FindBlockSolution(allocSize);
		//Footer* free_foot = nullptr;
		Header* free_head = FindBlock(allocSize);
		Footer* used_foot = ((Footer*)(((char*)free_head) + free_head->size + sizeof(Header)));

		Header* used_head = nullptr;
		Footer* free_foot = nullptr;
		void* data = nullptr;

		// TODO-STUDENT - comment out the solution version and write your own, you can ignore the synchronization code
		//if (AllocateSplitSolution(free_head, &data, allocSize) == false)
		//{
		//	AllocateNoSplitSolution(free_head, &data);
		//}

		if (free_head != nullptr)
		{
			//split logic
			if (free_head->size - allocSize >= (sizeof(Header) + sizeof(Footer) + 1))
			{
				//still free 
				free_head->size = free_head->size - allocSize - sizeof(Header) - sizeof(Footer);
				free_foot = (Footer*)(((char*)free_head) + sizeof(Header) + free_head->size);
				free_foot->size = free_head->size;
				//used
				used_head = ((Header*)(((char*)free_foot) + sizeof(Footer)));
				used_head->size = allocSize;
				used_head->size |= (1 << 31); //set to used
				used_foot->size = used_head->size;
				
				data = (char*)(((char*)used_head) + sizeof(Header));
			}
			else //don't split
			{
				free_head->next->prev = free_head->prev;
				free_head->prev->next = free_head->next;

				free_head->size |= (1 << 31);

				data = (char*)(((char*)free_head) + sizeof(Header) );
			}
		}

		// MM_HEAP_UNLOCK;
		return data;
	}

	void Heap::DeAllocate(void * data)
	{
		// MM_HEAP_LOCK;
		bool merged_left = false;
		bool merged_right = false;

		// TODO-STUDENT - comment out the solution version and write your own, you can ignore the synchronization code
		//merged_right = DeAllocateRightSolution(data);
		//merged_left = DeAllocateLeftSolution(data);
		/*if (merged_left != true)
		{
			DeAllocateMiddleSolution(data);
		}*/

		Header* myHead = (Header*)((char*)data - sizeof(Header));
		myHead->size &= ~(1 << 31);
		Footer* myFoot = (Footer*)((char*)myHead + sizeof(Header) + myHead->size);
		myFoot->size &= ~(1 << 31);

		if (myFoot != lastFooter)
		{
			Header* righthead = (Header*)((char*)myFoot + sizeof(Footer));
		
			if (!(righthead->size & (1 << 31))) //free
			{
				Footer* rightfoot = (Footer*)((char*)righthead + righthead->size + sizeof(Header));
				rightfoot->size += myFoot->size + sizeof(Header) + sizeof(Footer);
				myFoot = rightfoot;
				myHead->size = myFoot->size;

				merged_right = true;

				righthead->next->prev = righthead->prev;
				righthead->prev->next = righthead->next;
			}
		}

		if (myHead != firstHeader)
		{
			Footer* leftfoot = (Footer*)((char*)myHead - sizeof(Footer));
			
			if ( !(leftfoot->size & (1 << 31)))//free
			{
				Header* lefthead = ((Header*)(((char*)leftfoot) - leftfoot->size - sizeof(Header)));
				lefthead->size += myHead->size + sizeof(Header) + sizeof(Footer);;
				myHead = lefthead;
				myFoot->size = myHead->size;

				merged_left = true;
			}
		}

		if (merged_left == false)
		{
			if (freeListEntry == nullptr)
			{
				myHead->next = myHead;
				myHead->prev = myHead;

				freeListEntry = myHead;
			}
			else
			{
				myHead->next = freeListEntry->next;
				myHead->prev = freeListEntry;

				freeListEntry->next->prev = myHead;
				freeListEntry->next = myHead;
			}

			
		}

		// MM_HEAP_UNLOCK;
	}
	
	void Heap::CoreDump()
	{
		// MM_HEAP_LOCK;

		// TODO - comment out the solution version and write your own, you can ignore the synchronization code
		CoreDumpSolution();

		// kick start
		//std::ofstream file("..\\Logs\\coredump.txt", std::ios_base::out | std::ios_base::trunc);
		//if (file.is_open())
		//{
		//	std::time_t time = std::time(nullptr);
		//	std::tm local_time = *std::localtime(&time);
		//	file << "Core Dump on " << std::put_time(&local_time, "%m-%d-%Y %I:%M") << "\n\n";
		//	file << "Mem Address\tData";
		//	file << std::hex;

		//	//some sort of looping mechanism, until it works just print out a few hundred bytes or so
		//	//		print out the memory address - 0x000012ab
		//	//		print out data at the memory address chopped up into bytes		44 12 ab 6e
		//	//		print out the chopped up data interpeted as a character			0=a1
		//	//		0x000012ab		44 12 ab 6e			0=a1

		//	file.close();
		//}

		// MM_HEAP_UNLOCK;
	}
}
