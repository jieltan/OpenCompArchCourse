//////////////////////////////////////////////////////
// tj_malloc.h
// this provides a pseudo malloc that we can use
// this implementation of malloc is O(n^2)
// and free is O(n), so not the most efficient malloc
// 
// since we are allocating everything statically
// we can only keep track of so many mallocs,
// and the default number is 16
// Jielun Tan and James Connolly, 02/2019
// **************************************************
// Update 08/2019: So scratch everything above, I've
// reimplemented these functions following the example
// provided from the C Programming Language book
// The current implementation uses a linked list 
// to keep track of free blocks and allocates the
// free blocks first before grabbing more memory from
// the heap. Both malloc and free should now be
// O(n), since the only one search is required for
// each operation. Also added calloc, which is just
// malloc plus memset - Jielun Tan
/////////////////////////////////////////////////////

#include<stdbool.h>
#include<stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define HEAP_SIZE 16*1024
static unsigned char heap[HEAP_SIZE]; //reserve 16 KiB for malloc
static void* next_index = (void *)heap; //the next place to be allocated
static unsigned int avail_mem = sizeof(heap); //the most CONTIGUOUS memory available

typedef struct header { //block header
	struct header *next; //next block if on free list
	unsigned int size; //size of this block
} Header;
static Header base; //empty list to get started
static Header *freep = NULL; //start of the free list


void tj_free(void *mem) {
	//sanity check, we don't want to free memory that's not
	//in the heap
	if (mem < (void *)heap || mem > (void *)(heap + sizeof(heap)))
		exit(1);

	Header *bp, *p;
	bp = (Header *)mem - 1; //point to block header
	//scan the free list to see where the current block should sit in between
	for (p = freep; !(bp > p && bp < p->next); p = p->next) 
		// self wrapped free list with only one entry
		//                  or you are just at the very beginning/end
		if (p >= p->next && (bp > p || bp < p->next))
			break; //freed block at start of end of the arena
		//we can merge the 2 free blocks if they are adjacent to each other
		//or we just can append a new entry into the free list
	if (bp + bp->size == p->next) { //join to upper nbr
		//merge if exactly adjacent
		bp->size += p->next->size;
		bp->next = p->next->next;
	} else
		bp->next = p->next; //insert bp after p in the linked list
		//if p is freep which is base, then this will make the newly
		//allocated block point to base

	if (p + p->size == bp) { //join to lower nbr
		//merge if exactly adjacent
		p->size += bp->size;
		p->next = bp->next;
	} else //or just append to linked list
		p->next = bp; //again, if the free list is just the base
		//then effectively we just created a new entry
		//and make it point to the base which has a size of 0
	freep = p;
}

static Header* getmoremem(unsigned int total_size) {
	if (avail_mem < total_size) return NULL; //gg, we ran out of mem :P
	Header* up = (Header *)next_index;
	next_index += total_size; //allocate the block
	avail_mem -= total_size; //deduct from avail mem;
	up->size = total_size - sizeof(Header); //set the size right
	tj_free((void *)(++up)); //append the new block to the free list first
	return freep;
}

void *tj_malloc(unsigned int size) {
	//sanity check, so that you don't blow the memory space
	if (size > sizeof(heap)) return NULL;
	//we want strict word alignment just to make things easier
	//and so that we don't have improper alignment issues
	if ((size & 3) != 0) {
		size = size + 4 - (size & 3);
	}

	//we want to build a linked list of the existing blocks and free blocks
	Header *p, *prevp; //iterators
	
	unsigned int total_size = size + sizeof(Header); //need to include the header size as well
	//check the linked list
	prevp = freep;
	//if there's no linked list yet
	if (prevp == NULL) {
		prevp = &base;
		freep = prevp;
		base.next = freep;
		base.size = 0;
	}
	//traverse through the linked list, note there's no stopping condition
	for (p = prevp->next; ;prevp = p, p = p->next) {
		if (p->size >= size) { //big enough
			if (p->size == size) // exact size
				prevp->next = p->next; // just return that block
			else {
				p->size -= total_size; //break up the block
				p += p->size;
				p->size = size;
			}
			freep = prevp;
#ifdef DEBUG
			printf("returned pointer is %i\n", (int)p + 1);
#endif
			return (void *)(++p);
		}
		if (p == freep) //wrapped around free list
			if ((p = getmoremem(total_size)) == NULL) //if the heap runs out
				return NULL; //well, you got nothing left, gg
	}
}

void *tj_calloc(unsigned int size) {
	void *mem = tj_malloc(size);
	memset(mem, 0, size);
	return mem;
}
