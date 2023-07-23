#include <indexed_cache_allocator/allocator.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



int _get_random_number(int min,int max){
	return rand()%(max-min+1)+min;
}



int main(void){
	unsigned int seed=(unsigned int)time(NULL);
	printf("Seed: %.8x\n",seed);
	srand(seed);
	allocator_t allocator;
	allocator_init((1<<11)-1,&allocator);
	// Random access
	unsigned int cache_miss_count=0;
	for (unsigned int i=0;i<4096;i++){
		unsigned int j=_get_random_number(0,4095);
		if (!allocator_get(&allocator,j,NULL)){
			allocator_set(&allocator,j,(void*)1);
			cache_miss_count++;
		}
	}
	printf("%u/%u -> %.2f\n",cache_miss_count,4096,cache_miss_count/4096.0f);
	// Localized access
	cache_miss_count=0;
	int j=2048;
	for (unsigned int i=0;i<4096;i++){
		if (!allocator_get(&allocator,j,NULL)){
			allocator_set(&allocator,j,(void*)1);
			cache_miss_count++;
		}
		j+=_get_random_number(-4,4);
	}
	printf("%u/%u -> %.2f\n",cache_miss_count,4096,cache_miss_count/4096.0f);
	allocator_deinit(&allocator);
	return 0;
}
