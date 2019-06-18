/* 
 *  malloc package written by Ezra Davis (edavis@wpi.edu)
 *
 *
 * A malloc package that uses binning and immediate boundary tag coalescing.
 *
 * Blocks must be aligned to 8 bytes. The minimum block size is 16 bytes.
 *
 * Binning is a method of approaching best-fit searching for
 *    blocks to place payloads in while running in near-constant time. 
 *
 *
 * There are 256 bins.
 *   Each bin corresponds to a different size of block,
 *     with a special bin for all blocks over 256 bytes.
 *	 The beginning of each bin is located in contiguous memory at the binsp pointer.
 *     The special large-allocation bin is at the pointer,
 *          the next is unused, and the remaining ones start counting up by a byte,
 *          starting at blocks of 2 words (minimum block size).
 *     The large-allocation bin uses first-fit searching to find blocks large enough for a given payload.
 *
 *
 * Blocks consist of a header, a footer and the body.
 * 		The header and footer have two parts:
 *			Last bit:  Whether the block is allocated (1) or not (0).
 *			Rest of the word: Size of allocated block, including header and footer.
 *
 * In the first 8 bytes of a free node, there are pointers for one of the bins' doubly linked lists.
 *		The first pointer is to the next free node.
 *		These are NULL if this node is the beginning or ending of the list.
 *
 * 
 *
 *	The base code for this implementation is from our textbook's website and is located here:
 * 		http://csapp.cs.cmu.edu/public/ics2/code/vm/malloc/mm.c
 *    In fact, if the #define BINS statement is removed, it will function very similarly.
 *
 * This implementation has been written for 32-bit machines. It may work for 64-bit machines, but that is unlikely.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mm.h"
#include "memlib.h"


/* Author information */
team_t team = {
    /* Team name */
    "No-Team",
    /* First member's full name */
    "Ezra Philip Darius Davis",
    /* First member's email address */
    "edavis@wpi.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};



/* 
	If VERBOSE is defined, print out debug data.
		If it is defined as != 0, it prints out much more debug information.
 */
#define VERBOSEx 0

/* 
	If BINS is defined, malloc will use binning.
		Otherwise it will use first-fit or next-fit algorithms.
 */
#define BINS

#ifndef BINS
/*
 * If NEXT_FIT defined use next fit search, else use first fit search.
 *   BINS will override NEXT_FIT
 */
#define NEXT_FIT

#endif

/* Basic constants and macros */
#define WSIZE       4       /* Word and header/footer size (bytes) */
#define DSIZE       8       /* Doubleword size (bytes) */
#define MIN_BLKSIZE (2*DSIZE)/* Size of smallest possible block */
#define CHUNKSIZE  (1<<12)  /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc)  ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)       (*(unsigned int *)(p))
#define PUT(p, val)  (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)       ((char *)(bp) - WSIZE)
#define FTRP(bp)       ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
#define BINP_NEXT(bp)  ((char **)(bp) + 0)
#define BINP_PREV(bp)  ((char **)(bp) + 1)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)  ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp)  ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))


/* Global variables */
static char *heap_listp = 0;  /* Pointer to first block */

#ifdef NEXT_FIT
static char *rover;           /* Next fit rover */
#endif

#ifdef BINS

#define BINCOUNT 256 /* Number of bins (size categories). */
/* Get the correct bin number. Large sizes go in the 0th bin */
#define GET_BIN_NUMBER(asize)       (((asize)/8 >= BINCOUNT) ? 0 : (asize/8))

static char **binsp; /* Pointer to the first bin */

#endif

/* Function prototypes for internal helper routines */
static void *extend_heap(size_t words);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void *coalesce(void *bp);
static void printblock(void *bp); 
static void mm_checkheap(int verbose);
static void checkblock(void *bp);

#ifdef BINS
static void put_in_bin(void *bp, size_t asize);
static void take_from_bin(void *bp, size_t asize);
static void printbins();
static void validate_bins();
static void validate_bin(int bin_num);
#endif

/* 
 * mm_init - Initialize the memory manager.
 *   Create bins, prologue, and epilogue.
 */
int mm_init(void)
{

#ifdef BINS
	/* Creating bins and setting them to NULL */
	if((binsp = mem_sbrk(sizeof(char *) * BINCOUNT))  == (void *)-1)
		return -1;
	int i;
	for(i = 0; i < BINCOUNT; i++) {
		binsp[i] = NULL;
	}
#endif

	/* Create the initial empty heap */
	if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
	   return -1;
	PUT(heap_listp, 0);                          /* Alignment padding */
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */ 
	PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */ 
	PUT(heap_listp + (3*WSIZE), PACK(0, 1));     /* Epilogue header */
	heap_listp += (2*WSIZE);

#ifdef NEXT_FIT
	rover = heap_listp;
#endif

	/* Extend the empty heap with a free block of CHUNKSIZE bytes */
	if (extend_heap(CHUNKSIZE/WSIZE) == NULL) 
	   return -1;
	return 0;
}

/* 
 * mm_malloc - Allocate a block with at least size bytes of payload.
 *     Attempts to find the best fit using binning.
 */
void *mm_malloc(size_t size) 
{
#ifdef VERBOSE
	printf("malloc\n");
#endif
	size_t asize;      /* Adjusted block size */
	size_t extendsize; /* Amount to extend heap if no fit */
	char *bp;

	if (heap_listp == 0){
	   mm_init();
	}
	/* Ignore spurious requests */
	if (size == 0)
	   return NULL;

	/* Adjust block size to include overhead and alignment reqs. */
	if (size <= DSIZE)
	   asize = 2*DSIZE;
	else
	   asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

	/* Search the free list for a fit */
	if ((bp = find_fit(asize)) != NULL) {
		place(bp, asize);
		return bp;
	}

	/* No fit found. Get more memory and place the block */
	extendsize = MAX(asize,CHUNKSIZE);
	if ((bp = extend_heap(extendsize/WSIZE)) == NULL) {
#ifdef BINS
		printbins();
#endif
		return NULL;                      
	}
	place(bp, asize);                   
	return bp;
} 

/* 
 * mm_free - Free a block.
 *     Also puts it into the correct bin's free list.
 *     Boundary tag coalescing is immediate.
 */
void mm_free(void *bp)
{
#ifdef VERBOSE
	printf("free\n");
#endif

	if(bp == 0) 
		return;

	size_t size = GET_SIZE(HDRP(bp));
	if (heap_listp == 0) {
		mm_init();
	}

	PUT(HDRP(bp), PACK(size, 0));
	PUT(FTRP(bp), PACK(size, 0));

#ifdef BINS
	put_in_bin(bp, size);
#endif

	coalesce(bp);

}

/*
 * coalesce - Boundary tag and bin coalescing. Return ptr to coalesced block
 * 			  Only use on free blocks.
 *            Also manages bin locations for these blocks.
 */
static void *coalesce(void *bp) 
{
#ifdef VERBOSE
	printf("coalesce\n");
#endif

	size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
	size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
	size_t size = GET_SIZE(HDRP(bp));

	if (prev_alloc && next_alloc) {            /* Case 1 */
#ifdef BINS
		//No changes due to bins.
#endif
		return bp;
	}

	else if (prev_alloc && !next_alloc) {      /* Case 2 */
#ifdef BINS
		take_from_bin(bp, GET_SIZE(HDRP(bp)));
		take_from_bin(NEXT_BLKP(bp), GET_SIZE(HDRP(NEXT_BLKP(bp))));
#endif
		size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		PUT(HDRP(bp), PACK(size, 0));
		PUT(FTRP(bp), PACK(size,0));
	}

	else if (!prev_alloc && next_alloc) {      /* Case 3 */

#ifdef BINS /* Remove the previous & this block from bins */
		take_from_bin(bp, GET_SIZE(HDRP(bp)));
		take_from_bin(PREV_BLKP(bp), GET_SIZE(HDRP(PREV_BLKP(bp))));
#endif

	size += GET_SIZE(HDRP(PREV_BLKP(bp)));
	PUT(FTRP(bp), PACK(size, 0));
	PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
	bp = PREV_BLKP(bp);
	}

	else {                                     /* Case 4 */
#ifdef BINS /* Remove all the blocks being coalesced from their bins */
		take_from_bin(PREV_BLKP(bp), GET_SIZE(HDRP(PREV_BLKP(bp))));
		take_from_bin(bp, GET_SIZE(HDRP(bp)));
		take_from_bin(NEXT_BLKP(bp), GET_SIZE(HDRP(NEXT_BLKP(bp))));
#endif
	size += GET_SIZE(HDRP(PREV_BLKP(bp))) + 
		GET_SIZE(FTRP(NEXT_BLKP(bp)));
	PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
	PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
	bp = PREV_BLKP(bp);
	}

#ifdef NEXT_FIT
	/* Make sure the rover isn't pointing into the free block */
	/* that we just coalesced */
	if ((rover > (char *)bp) && (rover < NEXT_BLKP(bp))) 
	rover = bp;
#endif

#ifdef BINS
		put_in_bin(bp, GET_SIZE(HDRP(bp)));
#endif

	return bp;
}
/*
 * mm_realloc - Resizes (and may move) the block pointed to by 'ptr' to size 'size'.
 *
 *		In the current implementation,
 *         if the block fits, it will always stay in the same location.
 */
void *mm_realloc(void *ptr, size_t size)
{
#ifdef VERBOSE
	printf("realloc\n");
#endif
	size_t oldsize;
	void *newptr;

	/* If size == 0 then this is just free, and we return NULL. */
	if(size == 0) {
		mm_free(ptr);
		return NULL;
	}

	/* If oldptr is NULL, then this is just malloc. */
	if(ptr == NULL) {
		return mm_malloc(size);
	}

	/* If the new block-size requested is smaller than the old block */
	if(GET_SIZE(HDRP(ptr)) >= size + DSIZE) // DSIZE == size of header (and footer)
	{
		place(ptr, size + DSIZE);
		return ptr;
	}


	newptr = mm_malloc(size);

	/* If realloc() fails the original block is left untouched  */
	if(!newptr) {
		return 0;
	}

	/* Copy the old data. */
	oldsize = GET_SIZE(HDRP(ptr));
	if(size < oldsize) oldsize = size;
	memcpy(newptr, ptr, oldsize);

	/* Free the old block. */
	mm_free(ptr);

	return newptr;
}


/*
	Checks the heap for consistency and prints out any problems:
		Examines prologue and epilogue.
		Makes sure that the header and footers match.
		Makes sure the total memory used is the total memory allocated.
		Validates the bins.
*/
void mm_checkheap(int verbose)
{
	char *bp = heap_listp;
	size_t total_mem = DSIZE;
#ifdef BINS
	/* Remembering space used by the bin pointers */
	total_mem += BINCOUNT * sizeof(int *);
#endif

	if(verbose)
		printf("Checking Heap (%p):\n", heap_listp);

	//Checking prologue.
	size_t size = (GET_SIZE(HDRP(heap_listp)));

	if( (size != DSIZE) || !GET_ALLOC(HDRP(heap_listp)) )
		printf("Error: Bad prologue header\n");
	checkblock(heap_listp);

	//Checking all the blocks
	for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
		if (verbose)
			printblock(bp);
		checkblock(bp);
		total_mem += GET_SIZE(HDRP(bp));
	}

	//Checking epilogue
	if (verbose)
		printblock(bp);
	size = GET_SIZE(HDRP(bp));
	if (( 0 != size ) || !(GET_ALLOC(HDRP(bp))))
		printf("Error: Bad epilogue header\n");

	//Making sure that the heap is the same size as the system thinks it is
	if(mem_heapsize() != total_mem)
		printf("Error: Heap size is wrong.\tSystem: 0x%x, mm: 0x%x", mem_heapsize(), total_mem);

#ifdef BINS
	validate_bins();
#endif

}

/* 
 * The remaining routines are internal helper routines 
 */

/* 
 * extend_heap - Extend heap with free block and return its block pointer
 *		Makes a simple call to mem_sbrk and puts the returned block into the correct free block bin.
 */
static void *extend_heap(size_t words)
{
#ifdef VERBOSE
	printf("extend_heap\n");
#endif
	char *bp;
	size_t size;

	/* Allocate an even number of words to maintain alignment */
	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == -1)  
		return NULL;                            

	/* Initialize free block header/footer and the epilogue header */
	PUT(HDRP(bp), PACK(size, 0));         /* Free block header */
	PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */  
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */

#ifdef BINS
	put_in_bin(bp, size);
#endif


	/* Coalesce if the previous block was free */
	return coalesce(bp);                                       
}

/* 
 * place - Place block of asize bytes at start of free block bp 
 *         and split if remainder would be at least minimum block size
 */
static void place(void *bp, size_t asize)
{
#ifdef VERBOSE
	mm_checkheap(VERBOSE);

	printf("place: bp=%p, %x\n", bp, asize);
#endif

	size_t csize = GET_SIZE(HDRP(bp));   

#ifdef BINS
	take_from_bin(bp, csize);
#endif

	if ((csize - asize) >= MIN_BLKSIZE) { 
		PUT(HDRP(bp), PACK(asize, 1));
		PUT(FTRP(bp), PACK(asize, 1));
		bp = NEXT_BLKP(bp);
		PUT(HDRP(bp), PACK(csize-asize, 0));
		PUT(FTRP(bp), PACK(csize-asize, 0));
#ifdef BINS
		put_in_bin(bp, csize-asize);
#endif
	}
	else { 
		PUT(HDRP(bp), PACK(csize, 1));
		PUT(FTRP(bp), PACK(csize, 1));
	}
}

/* 
 * find_fit - Find a fit for a block with asize bytes.
 *     Finds this fit with binning (see comment at beginning of file).
 *
 */
static void *find_fit(size_t asize)
{
#ifdef VERBOSE
	printf("find_fit: %x\n", asize);
	mm_checkheap(VERBOSE);
#endif
#ifdef BINS

	int bin_num = GET_BIN_NUMBER(asize);
	void *bp = binsp[bin_num];

	if(bp == NULL && bin_num == 0)
		return NULL;
	else if(bp == NULL)
		//If this bin is empty, check the next largest bin for a fit.
		return find_fit(asize + MIN_BLKSIZE);

	if(bin_num != 0)
	{
		take_from_bin(bp, GET_SIZE(HDRP(bp)));

		return bp;
	} else { //Asize is large
		while(bp != NULL)
		{
			if(GET_SIZE(HDRP(bp)) >= asize) {
				take_from_bin(bp, GET_SIZE(HDRP(bp)));
				return bp;
			}
			bp = *BINP_NEXT(bp);
		}
		return NULL; /* No fit in extra large bin. */
	}

#else


#ifdef NEXT_FIT 
	/* Next fit search */
	char *oldrover = rover;

	/* Search from the rover to the end of list */
	for ( ; GET_SIZE(HDRP(rover)) > 0; rover = NEXT_BLKP(rover))
	if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
		return rover;

	/* search from start of list to old rover */
	for (rover = heap_listp; rover < oldrover; rover = NEXT_BLKP(rover))
	if (!GET_ALLOC(HDRP(rover)) && (asize <= GET_SIZE(HDRP(rover))))
		return rover;

	return NULL;  /* no fit found */
#else
	/* First fit search */
	void *bp;

	for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
	if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp)))) {
		return bp;
	}
	}
	return NULL; /* No fit */
#endif
#endif
}

/*
	Checks a single block for proper header and footer values.
*/
static void checkblock(void *bp)
{
	if ((size_t)bp % 8)
		printf("Error: Block %p not aligned.", bp);
	if (GET(HDRP(bp)) != GET(FTRP(bp)))
		printf("Error: Header doesn't equal footer in block %p\n\tHeader: %x\n\tFooter: %x",
		 bp, GET(HDRP(bp)), GET(FTRP(bp)));
	if (GET(HDRP(bp)) & 6)
		printf("Error: Header has incorrect values in block %p. Header: %x\n", bp, GET(HDRP(bp)));
}


/*
	Prints out information about a block.
*/
static void printblock(void *bp)
{
	size_t hsize, halloc;

	hsize = GET_SIZE(HDRP(bp));
	halloc = GET_ALLOC(HDRP(bp));
	//fsize = GET_SIZE(FTRP(bp));
	//falloc = GET_ALLOC(FTRP(bp));

	if(hsize == 0) {
		printf("\tBlock %p: Epilogue\n", bp);
	}
	else
	{
		printf("\tBlock %p:\tSize: 0x%x, %s\n",
			bp,
			hsize,
			halloc ? "Full" : "Free");
	}

}

///////////// Helper functions explicitly related to bins /////////////
#ifdef BINS
/*
	Puts a pointer to bp in the correct bin.
	asize is the size of the space pointed to by bp. Should be bigger than MIN_BLKSIZE (also bigger than 8)
*/
static void put_in_bin(void *bp, size_t asize)
{
#ifdef VERBOSE
	printf("Placing in bin: bp=%p, %x\n", bp, asize);
#endif

	int bin_num = GET_BIN_NUMBER(asize);

	char **temp;

	//bp is going into the front of the bin's list.
	temp = BINP_NEXT(bp);
	*temp = binsp[bin_num];

	temp = BINP_PREV(bp);
	*temp = NULL;


	if((int)*BINP_NEXT(bp) & 1)
		printf("Error: block not aligned in put_in_bin\n");

	if(binsp[bin_num] != NULL) //If there is an item in the bin
	{
		temp = BINP_PREV(binsp[bin_num]);
		*temp = bp; //This pointer is it's previous block.
	}

	binsp[bin_num] = bp;

#ifdef VERBOSE
	mm_checkheap(VERBOSE);
#endif
}

/*
	Removes a block from its bin.
*/
static void take_from_bin(void *bp, size_t asize)
{
#ifdef VERBOSE
	printf("take_from_bin: bp=%p, %x\n", bp, asize);
	mm_checkheap(VERBOSE);
	printbins();
#endif


	if(* BINP_PREV(bp) == NULL)
		binsp[GET_BIN_NUMBER(asize)] = * BINP_NEXT(bp);
	else {
		char** previous_next =  BINP_NEXT( *BINP_PREV(bp));
		*previous_next = * BINP_NEXT(bp);
	}

	if(* BINP_NEXT(bp) != NULL) {
		char** nexts_previous = BINP_PREV( *BINP_NEXT(bp));
		*nexts_previous = * BINP_PREV(bp);
	}
}


/*
	Prints out data about all the bins.
*/
static void printbins()
{
	int i;
	for(i=0; i < BINCOUNT; i++) {
		int itemCount = 0;
		char *item = binsp[i];
		while(item != NULL)
		{
			itemCount++;
			item = * BINP_NEXT(item);
		}
		if(itemCount != 0)
			printf("\tBin %x: %x items\t", i, itemCount);
	}
	printf("\n");
}

/*
	Checks all bins for correctness.
		Makes sure all elements are aligned, and all links in the doubly linked lists match up.
 */
static void validate_bins()
{
	int i;
	for (i = 0; i < BINCOUNT; ++i)
	{
		validate_bin(i);
	}
}

/* 
	Checks the free nodes in a single bin for consistancy.
	Makes sure that they are aligned and the doubly linked list doesn't have inconsistencies.
*/
static void validate_bin(int bin_num)
{
	char *bp = binsp[bin_num];
	char *prev_item = NULL;
	int chain_length = 0;	
	while(bp != NULL)
	{
		if( (int)bp % 8)
			printf("Bin not aligned. Bin %x, item %x\n", bin_num, chain_length);

		if(* BINP_PREV(bp) != prev_item)
			printf("Previous BINP is wrong. Bin %x, item %x\n", bin_num, chain_length);

		prev_item = bp;
		bp = * BINP_NEXT(bp);
		chain_length++;
	}
}
#endif