#include "list.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Размер массива */
#define LIST_SIZE 100000

/* Указатель на массив */
struct list_head *state_list;
unsigned int father[LIST_SIZE];
struct list_head *pair_list;

/* Описание структуры состояния*/
struct state {
	int q;
	struct list_head list;
};

/* Описание структуры для "использованных" пар */
struct pair {
	int p;
	int state; // из какого S переходили по этой паре 
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
/* Печатает все пары */
void pair_list_print() {
	struct pair *pos;
	if(! list_empty(pair_list)) 
		list_for_each_entry(pos, pair_list, list) {
			printf("%d from state %d", pos->p, pos->state);
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
/* Выделяет память под новую пару */
struct pair *__pair_new() {
	struct pair *tmp;
	tmp = (struct pair *) malloc( sizeof(struct pair));
	if(!tmp)
		return NULL;
	memset(tmp, 0, sizeof(struct pair));
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
/* добавляет новую пару в список */
struct pair *pair_add(int p, int s) {
	struct pair *st;
	st = __pair_new();
	if(!st)
		return NULL;
	st->p = p;
	st->state = s;
	list_add( &(st->list), pair_list);
	return st;
}

/* Вычищает состояние из памяти */
int __state_erase(struct state *st) {
	if (st) {
		free(st);
	}
	return 0;
}
/* Вычищает пару из памяти  */
int __pair_erase(struct pair *st) {
	if(st) {
		free(st);
	}
	return 0;
}

/* Удаляет состояние из списка */
int state_del(struct state *st) {
	list_del(&(st->list));
	__state_erase(st);
}

/* Удаляет пару из списка */
int pair_del(struct pair *st) {
	list_del(&(st->list));
	__pair_erase(st);
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

/* Удаляет i-е множество состояний из массива  */
int state_list_del(unsigned int i) {
	struct state *pos, *n;
	if(!list_empty(&state_list[i]))
		list_for_each_entry_safe(pos, n, &state_list[i], list)
			state_del(pos);
}

/* Удаляет все пары из списка */
int pair_list_clean() {
	struct pair *pos, *n;
	if(!list_empty(pair_list))
		list_for_each_entry_safe(pos, n, pair_list, list)
			pair_del(pos);
}

/* Проверяет, принадлежит ли состояние а множеству d */
int check_state(int a, int d) {
	struct state *pos;
	if(!list_empty(&state_list[d])) {
		list_for_each_entry(pos, &state_list[d], list) { 
			if(pos->q == a)
				return 1;
		}
	}
	return 0;
}

/* Находит размер множества */
int sizeof_set(unsigned int i) {
	int k = 0;
	struct state *pos;
	list_for_each_entry(pos, &state_list[i], list)
		k++;
	return k;
}



/* Проверяет, одинаковы ли два множества
 * возвращает 0, если да*/
int check_eq_set(int i, int j) {
	struct state *posI;
	struct state *posJ;
	int flag = 0;
	if(sizeof_set(i) != sizeof_set(j))
		return 1;
	list_for_each_entry(posI, &state_list[i], list) {
		list_for_each_entry(posJ, &state_list[j], list)
			if(posI->q == posJ->q)
				flag = 1;
		if(!flag)
			return 1;
		flag = 0;
	}
	return 0;

}

/* Проверяет, было ли уже такое множество 
 * возвращает 0, если было*/
int check_eq(int s) {
	unsigned int i;
	for(i=0; i<LIST_SIZE; i++) {
		if(!list_empty(&state_list[i]))
			if((i != s) && (!check_eq_set(i,s)))
				return 0;
	}
	return 1;
}

/* Проверяет, была ли уже такая пара 
 * возвращает 1, если пары ещё не было, т.е можно по ней пройти */
int check_pair(int p, int s) {
	struct pair *pos;
	if(!list_empty(pair_list))
		list_for_each_entry(pos, pair_list, list) 
			if((pos->state == s) && (pos->p == p))
				return 0;
	return 1;

}
/* Все проверки для пары в кучу */
int pair_approved(int p, int state) { 
	if(!check_state(p,state)) {return 0;} // пара  принадлежаит текущему S
	if(!check_pair(p,state)) { return 0;} //переходили ли из этого S по этой паре раньше
	return 1;
}


/* Подчищает */
void cleanup() {
	unsigned int i;
	state_list_clean();
	pair_list_clean();
	for(i=0; i<LIST_SIZE; i++)
		father[i] = 0;
	free(state_list);
	free(pair_list);
}
