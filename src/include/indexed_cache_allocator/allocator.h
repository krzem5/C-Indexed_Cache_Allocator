#ifndef _INDEXED_CACHE_ALLOCATOR_ALLOCATOR_H_
#define _INDEXED_CACHE_ALLOCATOR_ALLOCATOR_H_ 1



typedef unsigned short int allocator_index_t;



typedef unsigned long long int allocator_key_t;



typedef struct _ALLOCATOR_NODE{
	allocator_key_t key;
	void* value;
	allocator_index_t key_at_index;
	allocator_index_t prev;
	allocator_index_t next;
} allocator_node_t;



typedef struct _ALLOCATOR{
	allocator_index_t size;
	allocator_index_t first;
	allocator_index_t last;
	allocator_node_t* nodes;
} allocator_t;



void allocator_init(allocator_index_t size,allocator_t* out);



void* allocator_get(allocator_t* allocator,allocator_key_t key);



_Bool allocator_set(allocator_t* allocator,allocator_key_t key,void* value);



void allocator_deinit(allocator_t* allocator);



#endif
