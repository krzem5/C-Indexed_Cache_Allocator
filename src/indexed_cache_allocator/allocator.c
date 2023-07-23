#include <indexed_cache_allocator/allocator.h>
#include <stdio.h>
#include <stdlib.h>



void allocator_init(allocator_index_t size,allocator_t* out){
	if (size<2){
		printf("allocator size must be at least 2");
		return;
	}
	if (size&(size+1)){
		printf("allocator size must be 1 below a power of 2");
		return;
	}
	out->size=size;
	out->first=0;
	out->last=size-1;
	out->nodes=malloc(size*sizeof(allocator_node_t));
	for (allocator_index_t i=0;i<size;i++){
		(out->nodes+i)->key=i;
		(out->nodes+i)->value=NULL;
		(out->nodes+i)->key_at_index=i;
		(out->nodes+i)->prev=i-1;
		(out->nodes+i)->next=i+1;
	}
}



void* allocator_get(allocator_t* allocator,allocator_key_t key,void** out){
	allocator_index_t i=(allocator->size+1)>>1;
	allocator_index_t j=i-1;
	allocator_index_t k;
	allocator_node_t* node;
	do{
		k=(allocator->nodes+j)->key_at_index;
		node=allocator->nodes+k;
		if (node->key==key){
			if (k==allocator->first){
				return node->value;
			}
			if (k==allocator->last){
				allocator->last=node->prev;
			}
			else{
				(allocator->nodes+node->prev)->next=node->next;
				(allocator->nodes+node->next)->prev=node->prev;
			}
			(allocator->nodes+allocator->first)->prev=k;
			node->next=allocator->first;
			allocator->first=k;
			return node->value;
		}
		i>>=1;
		j=(key<node->key?j-i:j+i);
	} while (i);
	if (node->key<key&&j<allocator->size-1){
		j++;
	}
	k=allocator->last;
	if (k<j){
		for (i=k;i<j;i++){
			(allocator->nodes+i)->key_at_index=(allocator->nodes+i+1)->key_at_index;
		}
	}
	else{
		for (i=k;i>j;i--){
			(allocator->nodes+i)->key_at_index=(allocator->nodes+i-1)->key_at_index;
		}
	}
	(allocator->nodes+j)->key_at_index=k;
	node=allocator->nodes+k;
	if (out){
		*out=node->value;
	}
	node->key=key;
	node->value=NULL;
	node->next=allocator->first;
	(allocator->nodes+allocator->first)->prev=k;
	allocator->first=k;
	allocator->last=node->prev;
	return NULL;
}



_Bool allocator_set(allocator_t* allocator,allocator_key_t key,void* value){
	allocator_index_t i=(allocator->size+1)>>1;
	allocator_index_t j=i-1;
	do{
		allocator_node_t* node=allocator->nodes+(allocator->nodes+j)->key_at_index;
		if (node->key==key){
			node->value=value;
			return 1;
		}
		i>>=1;
		j=(key<node->key?j-i:j+i);
	} while (i);
	return 0;
}



void allocator_deinit(allocator_t* allocator){
	free(allocator->nodes);
}
