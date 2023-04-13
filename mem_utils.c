/*
  Copyright [2022] <Tatsuhiko Araki, Shin Komori>
  mem_utils.c utilities to support memory management system.
  HW6, CSE 374
*/

#include "mem.h"
#include "mem_impl.h"

/* initialize global variables? */
freeNode *freelist = NULL;
uintptr_t totalmalloc = 0;
uintptr_t totalFree = 0;
uintptr_t nFreeBlocks = 0;
void check_heap() {
  if (!freelist) return;
  freeNode* currentNode = freelist;
  uintptr_t minsize = currentNode->size;

  while (currentNode != NULL) {
    if (currentNode->size < minsize) {
      minsize = currentNode->size;
    }
    if (currentNode->next != NULL) {
      assert((uintptr_t)currentNode <(uintptr_t)(currentNode->next));
      assert((uintptr_t)currentNode + currentNode->size + NODESIZE
              <(uintptr_t)(currentNode->next));
    }
    currentNode = currentNode->next;
  }
  // go through free list and check for all the things
  if (minsize == 0) print_heap( stdout);
  assert(minsize >= MINCHUNK);
}
void get_mem_stats(uintptr_t* total_size, uintptr_t* total_free,
                   uintptr_t* n_free_blocks) {
  *total_size = totalmalloc;
  *total_free = 0;
  *n_free_blocks = 0;

  freeNode* currentNode = freelist;
  while (currentNode) {
    *n_free_blocks = *n_free_blocks + 1;
    *total_free = *total_free + (currentNode->size + NODESIZE);
    currentNode = currentNode->next;
  }
}

void print_heap(FILE *f) {
  printf("Printing the heap\n");
  freeNode* currentNode = freelist;
  while (currentNode !=NULL) {
    fprintf(f, "%" PRIuPTR, (uintptr_t)currentNode);
    fprintf(f, ", size: %" PRIuPTR, currentNode->size);
    fprintf(f, ", next: %" PRIuPTR, (uintptr_t)currentNode->next);
    fprintf(f, "\n");
    currentNode = currentNode->next;
  }
}


void insert(freeNode * next_Node) {
  // Check to make sure insert function
  if (!freelist || !next_Node) {
    return;
  }

  uintptr_t next_NodeAddress = (uintptr_t) next_Node;

  // Move specified block to free list
  if (next_NodeAddress < (uintptr_t) freelist) {
    next_Node->next = freelist;
    freelist = next_Node;
    return;
  }

  // Move the specified block to the middle of the free list
  freeNode* cur = freelist;
  freeNode* nextNode = freelist->next;
  while (cur && nextNode) {
    if (next_Node > cur && next_Node < nextNode) {
      next_Node->next = nextNode;
      cur->next = next_Node;
      return;
    } else {
      cur = nextNode;
      nextNode = nextNode->next;
    }
  }

  // Move the specified block to the end of the free list
  cur->next = next_Node;
  return;
}

freeNode * split(freeNode * cur, uintptr_t size) {
    uintptr_t headerSize = 2 * sizeof(uintptr_t);
    uintptr_t next_size = cur->size - size - headerSize;
    uintptr_t next_address = ((uintptr_t) cur) + headerSize + size;

    freeNode * new_cur = (freeNode *) next_address;
    new_cur->size = next_size;
    new_cur->next = cur->next;

    cur->size = size;
    cur->next = NULL;

    return new_cur;
}

uintptr_t align(uintptr_t address) {
  uintptr_t bound = 2 * sizeof(uintptr_t);
  if (address % bound == 0) {
    return address;
  } else {
    return address + (bound - address % bound);
  }
}
