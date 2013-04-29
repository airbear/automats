#include <stdio.h>
#include "list.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

/* Размер массива */
#define LIST_SIZE 100000

static int q = 3,e = 2; //|Q|, |E|

/* Указатель на массив */
struct list_head *state_list;

/* Описание структуры */
struct state {
	int q;
	struct list_head list;
};

/* печатает все элементы массива */
void state_list_print() {
	unsigned int i;
	struct state *pos;
	for (i = 0; i < LIST_SIZE; i++) 
		if (! list_empty(&state_list[i])) {
			printf("state_list[%u]: ", i);
			list_for_each_entry(pos, &state_list[i], list) 
				printf("%d ", pos->q);
			printf("\n");
		}
}

/* выделяет память под новое состояние */
struct state *__state_new() {
	struct state *tmp;
	tmp = (struct state *) malloc( sizeof(struct state));
	if (!tmp)
		return NULL;
	memset(tmp, sizeof(struct state), 0);
	return tmp;
}

/* добавляет новое состояние в список */
struct state *state_add(int num, int q) {
	struct state *st;
	st = __state_new();
	if (!st)
		return NULL;
	st->q = q;
	list_add( &(st->list), &(state_list[num]));
	return st;	
}

/* Вычищает состояние из памяти */
int __state_erase(struct state *st) {
	if (st)
		free(st);
	return 0;
}

/* Удаляет состояние из списка */
int state_del(struct state *st) {
	list_del(&(st->list));
	__state_erase(st);
}

/* Удаляет все списки из памяти */
int state_list_clean() {
	unsigned int i;
	struct state *pos, *n;
	for (i = 0; i < LIST_SIZE; i++)
		if (!list_empty(&state_list[i]))
			list_for_each_entry_safe(pos, n, &state_list[i], list)
				state_del(pos);
}

int main() {
	unsigned int i;
	state_list = (struct list_head *) malloc(sizeof(*state_list) * LIST_SIZE);
	if (!state_list) 
		return 3;
	for (i = 0; i < LIST_SIZE; i++)
		INIT_LIST_HEAD(&state_list[i]);
	state_add(1, 10);
	state_add(1, 5);
	state_add(1, 3);
	state_add(2, 11);
	state_list_print();
	state_list_clean();
	state_list_print();
	return 0;	
}

#if 0
int main(void) {
	int ar[q][e];
	int i,j,k;
	int w;
	int tmp_z;
	int *z;
	int x;
	int flag = 0;
	ar[0][0]=2;
	ar[0][1]=1;
	ar[1][0]=2;
	ar[1][1]=0;
	ar[2][0]=1;
	ar[2][1]=2;
	//тестовый автоматик ^
	
	//search:
	for (k=0; k<e; k++)  //проход по букве
		for(i=0; i<q; i++) //проход по парам
			for(j=0; j<q; j++) {
				if( (i!=j) && (ar[i][k] == ar[j][k])) {
					w = k; 
					flag = 1;
					goto mark;

				}
			}
mark:
	if(flag) { // была сжимаемая пара, надо ужать автомат по идее, но это я допишу, когда разберусь с листами ^^
		//goto search;
		printf("we have pair \n");
	}
	else { //сжать уже ничего нельзя 
		printf("we're fucked up\n");
	}
	free(state_list);
	return 0;
}
#endif

