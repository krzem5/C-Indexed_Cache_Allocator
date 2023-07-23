#include <indexed_cache_allocator/allocator.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



#define TEST_SIZE 16384
#define ALLOCATOR_SIZE_SHIFT 8



int _get_random_number(int min,int max){
	return rand()%(max-min+1)+min;
}



int main(void){
	unsigned int seed=(unsigned int)time(NULL);
	printf("Seed: %.8x\n",seed);
	srand(seed);
	allocator_t allocator;
	allocator_init((1<<ALLOCATOR_SIZE_SHIFT)-1,&allocator);
	// Random access
	unsigned int cache_miss_count=0;
	for (unsigned int i=0;i<TEST_SIZE;i++){
		unsigned int j=_get_random_number(0,TEST_SIZE-1);
		if (!allocator_get(&allocator,j,NULL)){
			allocator_set(&allocator,j,(void*)1);
			cache_miss_count++;
		}
	}
	printf("%u/%u -> %.2f\n",cache_miss_count,TEST_SIZE,((float)cache_miss_count)/TEST_SIZE);
	// Localized access
	cache_miss_count=0;
	unsigned int j=TEST_SIZE>>1;
	for (unsigned int i=0;i<TEST_SIZE;i++){
		if (!allocator_get(&allocator,j,NULL)){
			allocator_set(&allocator,j,(void*)1);
			cache_miss_count++;
		}
		j+=_get_random_number(-4,4);
	}
	printf("%u/%u -> %.2f\n",cache_miss_count,TEST_SIZE,((float)cache_miss_count)/TEST_SIZE);
	allocator_deinit(&allocator);
	return 0;
}
