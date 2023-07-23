#include <indexed_cache_allocator/allocator.h>
#include <stdio.h>



int main(void){
	allocator_t allocator;
	allocator_init((1<<11)-1,&allocator);
	allocator_deinit(&allocator);
	return 0;
}
