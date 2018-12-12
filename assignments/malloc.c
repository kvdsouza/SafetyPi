/*
 * File: malloc.c
 * --------------
 * This is the simple "bump" allocator from lecture.
 * An allocation request is serviced by tacking on the requested
 * space to the end of the heap thus far.
 * It does not recycle memory (free is a no-op) so when all the
 * space set aside for the heap is consumed, it will not be able
 * to service any further requests.
 *
 * This code is given here just to show the very simplest of
 * approaches to dynamic allocation. You are to replace this code
 * with your own heap allocator implementation.
 */

#include "malloc.h"
#include <stddef.h> // for NULL
#include "strings.h"
#include "printf.h"

extern int __bss_end__;

// Simple macro to round up x to multiple of n.
// The efficient but tricky bitwise approach it uses
// works only if n is a power of two -- why?
#define roundup(x,n) (((x)+((n)-1))&(~((n)-1)))

#define TOTAL_HEAP_SIZE 0x1000000 // 16 MB


/* Global variables for the bump allocator
 *
 * `heap_start` tracks where heap segment begins in memory.
 * It is initialized to point to the address at end of data segment.
 * It uses symbol __bss_end__ from memmap to locate end.
 * `heap_used` tracks the number of bytes allocated thus far.
 * The next available memory begins at heap_start + heap_used.
 * `heap_max` is total number of bytes set aside for heap segment.
 */
static void *heap_start = NULL;

struct blockHeader { // REQUIRES 8 BYTES.
    size_t payload_size;
    int status;       // 0 if free, 1 if in use
};

/*
Malloc function.
*/
void *malloc(size_t nbytes)
{
		// initialize mallocInit();
    if (!heap_start) {
				heap_start = &__bss_end__;
				struct blockHeader *initialBh;
				initialBh = heap_start;
				initialBh->payload_size = TOTAL_HEAP_SIZE - sizeof(struct blockHeader);
        initialBh->status = 0;
    } // excellent!

    nbytes = roundup(nbytes, 8);
		// set a pointer - go all the way to the end to look for the any free space,
		// return null otherwise.
		char *currPtr = (char *)heap_start;
		//printf("curr: %p\n", currPtr);
		char *finalAddress = ((char *)heap_start) + TOTAL_HEAP_SIZE;
		//printf("finalAddress: %p\n", finalAddress);

		struct blockHeader *newHeader = NULL;
		while (currPtr < finalAddress) { 	// may need to deal with the -8 case -> need to check for this..
				newHeader = (struct blockHeader *)currPtr; // initialize a new Header.
				// if header is free, make the new header and change the old.
				if ((newHeader->status == 0) && (nbytes <= newHeader->payload_size)) {
					// if newHeader is big enough and is free
					break;
				}
				currPtr = currPtr + (sizeof(struct blockHeader) + newHeader->payload_size); // do i need to worry about casting here?
		}
		if (currPtr > finalAddress) {
			//printf("NULLLLL\n");
			return NULL;
		}
		struct blockHeader *nextHeader = (struct blockHeader *) (currPtr + sizeof(struct blockHeader) + nbytes);

		if ((char*) nextHeader < finalAddress - sizeof(struct blockHeader)){ // malloc 0? check spec.
			nextHeader->payload_size = newHeader->payload_size - nbytes - sizeof(struct blockHeader);
			nextHeader->status = 0;
		}
		//change newHeader to currPtr
		newHeader->payload_size = nbytes;
		newHeader->status = 1;

		currPtr += sizeof(struct blockHeader);
		//printf("About to return from malloc: %p\n", (void*)currPtr);
		return (void*)currPtr;
}

/*
A helper function for coalescence.
*/
void coalesce_blocks(struct blockHeader* ptr) {
	  if (ptr->status == 1) return;
		char *lastAddress = ((char *)heap_start) + TOTAL_HEAP_SIZE;
  	struct blockHeader* next = (struct blockHeader*) ((char*)ptr +
					ptr->payload_size + sizeof(struct blockHeader));

  	while (next < (struct blockHeader *)(lastAddress)) {
    		if (next->status == 1) {
      			return;
				}
    		ptr->payload_size += next->payload_size + sizeof(struct blockHeader);
    		next = (struct blockHeader*)((char*)ptr + ptr->payload_size
					+ sizeof(struct blockHeader));
    }
		return;
}


/*
Free Function
*/
void free(void *ptr)
{
		struct blockHeader* header = (struct blockHeader*) ((char*) ptr - sizeof(struct blockHeader));
		header->status = 0; // set it to 0.

		coalesce_blocks(header);


}

void *realloc (void *old_ptr, size_t new_size)
{
		new_size = roundup(new_size, 8);
		struct blockHeader* header = (struct blockHeader*)((char*)old_ptr -
									sizeof(struct blockHeader));
		struct blockHeader* next = (struct blockHeader*) (((char*)header) +
									header->payload_size + sizeof(struct blockHeader));
		size_t prev_size = (header->payload_size);

		// Case 1: if we need to realloc more data, coalesce the next blocks, then check if there is enough space!
		if (new_size > prev_size) {
				coalesce_blocks(next);
				size_t available_space = next->payload_size + header->payload_size + sizeof(struct blockHeader);
		// Case 1a: if there is available space, just extend the current block and create a new block if necessary
				if (available_space >= new_size) {
						header->payload_size = new_size;
						size_t space_diff = available_space - new_size;
						if (space_diff > sizeof(struct blockHeader)) {
						// if there is enough space left over, create the new block (i.e. >8 bytes)
						struct blockHeader* next_header = (struct blockHeader*)((char*)header +
											header->payload_size + sizeof(struct blockHeader));
						next_header->payload_size = space_diff - sizeof(struct blockHeader);
						next_header->status = 0;
						}
						// if all we've done is extend the existing block, just return the same pointer
						return old_ptr;
				} else {
						// Case 1b: if there isn't enough space in the next block, malloc a new pointer
						void* new_ptr = malloc(new_size);
						if (!new_ptr) return NULL;
						memcpy(new_ptr, old_ptr, new_size);
						free(old_ptr);
						return new_ptr;
				}
		}

		// Case 2: if the requested size is less than the previously allocated size and the next block is free,
		// keep the same pointer but extend the next block
		if (new_size < prev_size && next->status == 0) {
				header->payload_size = new_size;
				size_t size_diff = prev_size - new_size;
				size_t next_size = next->payload_size + size_diff;
				int next_status = next->status;
				struct blockHeader* new_next = (struct blockHeader*) (((char*) header)
								+ header->payload_size + sizeof(struct blockHeader));
				new_next->payload_size = next_size;
				new_next->status = next_status;
		}
		// Edge Case: return same ptr in previous if or if prev_size == new_size or (prev_size < new_size and next->status == 1)
		return old_ptr;
}


void heap_dump () {
	malloc(10);
	malloc(20);
	char *currPtr = (char *)heap_start;
	printf("curr: %p\n", currPtr);
	char *finalAddress = ((char *)heap_start) + TOTAL_HEAP_SIZE;
	printf("finalAddress: %p\n", finalAddress);

	struct blockHeader *newHeader = NULL;
	while (currPtr < finalAddress) { 	// may need to deal with the -8 case -> need to check for this..
			newHeader = (struct blockHeader *)currPtr; // initialize a new Header.
			// if header is free, make the new header and change the old.
			printf("payload size: %d\n", newHeader->payload_size);
			printf("address: %p\n", currPtr);
			printf("status: %d\n", newHeader->status);
			currPtr = currPtr + newHeader->payload_size;
	}
}
	// header, address, and size.
