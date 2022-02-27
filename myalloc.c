// Ryan Earl

#include "myalloc.h"

struct block *head = NULL;

void print_data(void) {
    struct block *b = head;

    if (b == NULL) {
        printf("[empty]\n");
        return;
    }

    while (b != NULL) {
        // Uncomment the following line if you want to see the pointer values
        //printf("[%p:%d,%s]", b, b->size, b->in_use? "used": "free");
        printf("[%d,%s]", b->size, b->in_use? "used": "free");
        if (b->next != NULL) {
            printf(" -> ");
        }

        b = b->next;
    }

    printf("\n");
}

void *myalloc(size_t size) {

	if (head == NULL) { 
    	head = sbrk(1024);
    	head->next = NULL;
    	head->size = 1024 - block_padded_size;
    	head->in_use = 0;
    }

    (void)size;

    int padded_size = PADDED_SIZE(size);

    struct block *cur = head;
    while (cur != NULL) {
        // if this node is big enough and not in use:
        if (!cur->in_use && cur->size >= padded_size) {
            split_space(cur, size);	
            // mark it in use
            cur->in_use = 1;
            // printf("Found one!\n");

            // return pointer to node data
            return PTR_OFFSET(cur, block_padded_size);
        }
        cur = cur->next;
    }
    return NULL;
}

void split_space (struct block *s, size_t size) {
    int required_space = PADDED_SIZE(size) + block_padded_size + 16;
    
    if (s->size >= required_space) {
        struct block *split_node = PTR_OFFSET(s, PADDED_SIZE(size) + block_padded_size); // The data space requested by the user
        split_node->in_use = 0;
        split_node->size = s->size - (PADDED_SIZE(size) + (block_padded_size)); // A new struct block (padded)
        split_node->next = s->next;
        s->next = split_node;
        s->size = PADDED_SIZE(size);
    }
}

void myfree (void *p) { 
    struct block *b = p - (block_padded_size);
    b->in_use = 0;
	struct block *cur = head; // start cur at head
	while (cur->next != NULL) { // while cur->next isn't NULL:
		if ((cur->in_use == 0) && (cur->next->in_use == 0)) {// 	if cur is not in_use and next node not in use:
			cur->size += block_padded_size + (cur->next)->size;	// 		add the next node's region's size to cur's
			cur->next = cur->next->next;	// 		make cur's next pointer skip the next node
		}
		else { // 	else
			cur = cur->next;	// 		move cur to next node
		}		
		
	}
}

int main(void) {
    void *p, *q, *r, *s;

p = myalloc(10); print_data();
q = myalloc(20); print_data();
r = myalloc(30); print_data();
s = myalloc(40); print_data();

myfree(q); print_data();
myfree(p); print_data();
myfree(s); print_data();
myfree(r); print_data();
}