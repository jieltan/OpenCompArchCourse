///////////////////////////////////
// basic_malloc.c
// basic test for tj_malloc
// Tom Brady is the GOAT
//   - Jielun Tan, 02/2019
///////////////////////////////////
#ifndef DEBUG
extern void exit();
#endif
#include "tj_malloc.h"
#include <stdio.h>

typedef struct example_s{
	int brady;
	uint16_t rings;
	bool goat;
	int gronk;
	int edelman;
} example_t;

int main() {
	example_t* pats = (example_t*)tj_malloc(sizeof(example_t));
#ifdef DEBUG
	printf("pts is %i\n", (int)pats);
	printf("what\n");
#endif
	pats->brady = 12; // as of 02/2019
	pats->rings = 6;
	pats->goat = 1; // hands down the goat
	pats->gronk = 87; // tide pods
	pats->edelman = 11; // the ironman, superbowl 53 mvp
#ifdef DEBUG
	printf("%i\n", pats->edelman);
#endif
	example_t* another = (example_t*)tj_malloc(sizeof(example_t));
#ifdef DEBUG
	printf("pts is %i\n", (int)another);
#endif
	tj_free(pats);
	example_t* third = (example_t*)tj_malloc(sizeof(example_t));
	another->rings = 2003;
	third->rings = 2004;
#ifdef DEBUG
	printf("created third %i\n", (int)third);
#endif
	//let's make this test calloc
	example_t* latest = (example_t*)tj_calloc(sizeof(example_t));
	latest->rings = 2019;
	if (latest->goat != 0) exit(2);
	tj_free(another);
	tj_free(latest);
	return 0;
}
