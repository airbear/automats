#include <stdio.h>
#include "list.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

/* Размер массива */
#define LIST_SIZE 100000

#define qwerty 3
#define erty 2
//static int q = 3,e = 2; //|Q|, |E|

/* Указатель на массив */
struct list_head *state_list;
unsigned int father[LIST_SIZE];
struct list_head *pair_list;
	/* Тестовый автоматик */
static unsigned int ar[qwerty][erty];

/* Описание структуры состояния*/
struct state {
	int q;
	struct list_head list;
};

/* Описание структуры для "использованных" пар */
struct pair {
	int p;
	int q;
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
			printf("(%d,%d) from state %d", pos->p, pos->q, pos->state);
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
	memset(tmp, sizeof(struct pair), 0);
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
struct pair *pair_add(int p, int q, int s) {
	struct pair *st;
	st = __pair_new();
	if(!st)
		return NULL;
	st->p = p;
	st->q = q;
	st->state = s;
	list_add( &(st->list), pair_list);
	return st;
}

/* Вычищает состояние из памяти */
int __state_erase(struct state *st) {
	if (st)
		free(st);
	return 0;
}
/* Вычищает пару из памяти  */
int __pair_erase(struct pair *st) {
	if(st)
		free(st);
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

/* Проверяет, принадлежат ли состояния a, b, c множеству d */
int check_state(int a, int b, int c, int d) {
	struct state *pos;
	int flag1, flag2, flag3;
	if(!list_empty(&state_list[d])) {
		list_for_each_entry(pos, &state_list[d], list) { 
			if(pos->q == a)
				flag1 = 1;	
			if(pos->q == b)
				flag2 = 1;
			if(pos->q == c)
				flag3 = 1;
		}
	}
	if( flag1 && flag2 && flag3 )
		return 1;
	else
		return 0;
}

/* Добавляет новое множество состояний, 
 * также надо проверять, было ли уже такое множество 
 * текущее множество - под индексом i, последнее - под j */
int new_state_set(unsigned int i, unsigned int j, unsigned int w) {
	struct state *pos;
	unsigned int q_tmp;
	list_for_each_entry(pos, &state_list[i], list) {
		q_tmp = ar[pos->q][w];
		if (!check_state(q_tmp, q_tmp, q_tmp, j+1))
				state_add(j+1, q_tmp);
	}
state_list_print();
	if(check_eq(j))
		return 1;
	else {
		state_list_del(j);
		return 0;
	}
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
 * при том, что они нсортированы, способ таксе*/
int check_eq_set(int i, int j) {
	struct state *posI;
	struct state *posJ;
	int flag = 0;
	if(sizeof_set(i) != sizeof_set(j))
		return 0;
	list_for_each_entry(posI, &state_list[i], list) {
		list_for_each_entry(posJ, &state_list[j], list)
			if(posI->q == posJ->q)
				flag = 1;
		if(!flag)
			return 0;
		flag = 0;
	}
	return 1;
			
}

/* Проверяет, было ли уже такое множество */
int check_eq(int s) {
	unsigned int i;
	for(i=0; i<LIST_SIZE; i++) {
		if(!list_empty(&state_list[i]))
			if(check_eq_set(i,s))
				return 0;
	}
	return 1;
}

/* Проверяет, была ли уже такая пара 
 * возвращает 1, если пары ещё не было, тюе можно по ней пройти
 * мб не оче логично */
int check_pair(int q, int p, int s) {
	struct pair *pos;
	if(!list_empty(pair_list))
		list_for_each_entry(pos, pair_list, list) 
			if((pos->state == s) && 
					(((pos->p == p) && (pos->q==q)) ||
					 ((pos->p == q) && (pos->q==p))))
				return 0;
	return 1;
}
/* Все проверки для пары в кучу */
int pair_approved(int q, int p, int letter, int state) {
	if(arr[q][letter] != arr[p][letter] ) return 0; // пара сжимается
	if(!check_state(q,p,arr[p][letter],state)) return 0;// пара и то, во что сжимается, принадлежат текущему S
	if(!check_pair(q,p,state)) return 0; //переходили ли из этого S по этой паре раньше
	return 1;
}

int main() {
	unsigned int i,j,k;
	int flag;
	unsigned int w, current_set, stuff; 
/*	ar[0][0]=2;
	ar[0][1]=1;
	ar[1][0]=2;
	ar[1][1]=0;
	ar[2][0]=1;
	ar[2][1]=2;
*/
	ar[0][0]=1;
	ar[0][1]=1;
	ar[1][0]=2;
	ar[1][1]=2;
	ar[2][0]=0;
	ar[2][1]=0;
	/* Создаем напока пустой массив под множества состояний */
	state_list = (struct list_head *) malloc(sizeof(*state_list) * LIST_SIZE);
	pair_list = (struct list_head *) malloc(sizeof(*pair_list));

	if (!pair_list) 
		return 5;
	INIT_LIST_HEAD(pair_list);
	if (!state_list) 
		return 3;
	for (i = 0; i < LIST_SIZE; i++)
		INIT_LIST_HEAD(&state_list[i]);
	
	/* Создаем полное множество состояний, для начала - будет первым элементом массива */
	for (i=0; i<qwerty; i++)
		state_add(1, i);
	father[1] = 0;
	current_set = 1;
	stuff = 1;	

	/* Now, MAGIC! */

	/*
	while(current_set != 0) {
		flag = 0;
		for (k=0; k<erty; k++) //обходим по букве
			for (i=0; i<qwerty; i++)
				for(j=0; j< qwerty; j++) // обходим по парам
	}
*/
/*
	state_list_print();
	flag = 0;
	for (k=0; k<e; k++)  //проход по букве
		for(i=0; i<q; i++) //проход по парам
			for(j=0; j<q; j++) {
				if( (i!=j) && (ar[i][k] == ar[j][k]) && (check_state(i,j,k, 1))) {
				//проверили, что по букве k пара (i,j) принадлежащая
				//к текущему множесву состояний, сжимается к подходящему состоянию
					flag = 1;
					// надо сделать переход к новому множеству состояний
					w = k;			
				}
			}
*/
	/*
	state_list_print();
	state_list_clean();
	state_list_print();
	*/
	return 0;	
}

