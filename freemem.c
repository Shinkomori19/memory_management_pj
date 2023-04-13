
/*
  Copyright [2022] <Tatsuhiko Araki, Shin Komori>
  freemem.c
  implements freemem (free) for memory system
  CSE 374 HW6
*/

#include <assert.h>
#include "mem_impl.h"
#include "mem.h"

/* This code will be filled in by the students.  Your primary job is to
   implement freemem functions.  */

/* Define your functions below: */
void freemem(void* p) {
  check_heap();
  if (p) {
    freeNode* prev = NULL;
    freeNode* cur = freelist;
    freeNode* assign_block = (freeNode*) ((uintptr_t) p - sizeof(freeNode));
    freeNode* end = (freeNode*) ((uintptr_t) assign_block
    + assign_block->size + sizeof(freeNode));

    // Calculate total bytes in free list
    total_free += assign_block->size + sizeof(freeNode);

    // Traverses through the FreeList
    while (cur && (end > cur)) {
      prev = cur;
      cur = cur->next;
    }

    /* if end = freelist merge with first block in storage
      else insert block to the beginning of storage*/
    if (prev == NULL) {
      if (end == freelist) {
        assign_block->next = freelist->next;
        assign_block->size = assign_block->size
        + freelist->size + sizeof(freeNode);
      } else {
        assign_block->next = freelist;
        nFreeBlocks++;
      }
      freelist = assign_block;
      return;
    }
    freeNode* end_last_block = (freeNode*) ((uintptr_t) prev
                                    + prev->size
                                    + sizeof(freeNode));
    if (cur == NULL) {
      prev->next = assign_block;
      assign_block->next = NULL;
      nFreeBlocks++;
      return;
    }

    // connection with the right part
    if (end == cur && assign_block == end_last_block) {
      // Connections with adjacent blocks
      prev->size = prev->size + assign_block->size + cur->size + 32;
      prev->next = cur->next;
      nFreeBlocks--;
    } else if (assign_block == end_last_block) {
      // Conection with bottom of the left block
      prev->size = prev->size + assign_block->size + 16;
    } else if (end == cur) {
      // Conection with top of the right block
      assign_block->next = cur->next;
      prev->next = assign_block;
      assign_block->size = assign_block->size + cur->size + 16;
    } else {
      // Conection with middle of the two blocks
      assign_block->next = cur;
      prev->next = assign_block;
      nFreeBlocks++;
    }
  }
  check_heap();
}
