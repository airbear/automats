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

/* Проверяет, принадлежит ли состояние а текущему множеству состояний */
int check_state(int a, int b, int c, int d) {
	struct state *pos;
	int flag1, flag2, flag3;
	list_for_each_entry(pos, &state_list[d], list) { 
		if(pos->q == a)
			flag1 = 1;	
		if(pos->q == b)
			flag2 = 1;
		if(pos->q == c)
			flag3 = 1;
	}
	if( flag1 && flag2 && flag3 )
		return 1;
	else
		return 0;
}
int main() {
	unsigned int i,j,k;
	int flag;
	/* Создаем напока пустой массив под множества состояний */
	state_list = (struct list_head *) malloc(sizeof(*state_list) * LIST_SIZE);
	if (!state_list) 
		return 3;
	for (i = 0; i < LIST_SIZE; i++)
		INIT_LIST_HEAD(&state_list[i]);
	/* Создаем полное множество состояний, для начала - будет первым элементом массива */
	for (i=0; i<q; i++)
		state_add(1, i);
	/* Тестовый автоматик */
	int ar[q][e];
	ar[0][0]=2;
	ar[0][1]=1;
	ar[1][0]=2;
	ar[1][1]=0;
	ar[2][0]=1;
	ar[2][1]=2;
	/* Таки пробуем */
	flag = 0;
	for (k=0; k<e; k++)  //проход по букве
		for(i=0; i<q; i++) //проход по парам
			for(j=0; j<q; j++) {
				if( (i!=j) && (ar[i][k] == ar[j][k]) && (check_state(i,j,k, 1))) {
				//проверили, что по букве k пара (i,j) принадлежащая
				//к текущему множесву состояний, сжимается к подходящему состоянию
					flag = 1;
					// надо сделать переход к новому множеству состояний
					// и по идее оно могло как расшириться так и сузиться
				}
			}
	if (flag) printf("пара найдена, работает вроде как ");

	/*
	state_list_print();
	state_list_clean();
	state_list_print();
	*/
	return 0;	
}

#if 0
int main(void) {
	int i,j,k;
	int w;
	int tmp_z;
	int *z;
	int x;
	int flag = 0;
	//тестовый автоматик ^
	
	//search:
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

