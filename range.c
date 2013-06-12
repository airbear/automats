
#include "mylist.h"


#define qwerty 14
#define erty 2
//static int q = 3,e = 2; //|Q|, |E|


/* Тестовый автоматик */
static unsigned int ar[qwerty][erty];


/* Добавляет новое множество состояний, 
 * также надо проверять, было ли уже такое множество 
 * текущее множество - под индексом i, последнее - под j */
int new_state_set(unsigned int i, unsigned int j, unsigned int w) {
	struct state *pos;
	unsigned int q_tmp;
	list_for_each_entry(pos, &state_list[i], list) {
		q_tmp = ar[pos->q][w];
		if (!check_state(q_tmp, j+1))
			state_add(j+1, q_tmp);
	}
	if(check_eq(j+1))
		return 1;
	else {
		state_list_del(j+1);
		return 0;
	}
}
/* Подготовительное */
void init() {
	unsigned int i;
	/* Создаем напока пустой массив под множества состояний */
	state_list = (struct list_head *) malloc(sizeof(*state_list) * LIST_SIZE);
	//	if (!state_list) 
	//return 3;
	for (i = 0; i < LIST_SIZE; i++)
		INIT_LIST_HEAD(&state_list[i]);
	/* Пустой список пар */
	pair_list = (struct list_head *) malloc(sizeof(*pair_list));
	
	//	if (!pair_list) 
	//return 5;
	INIT_LIST_HEAD(pair_list);

	/* Создаем полное множество состояний, для начала - будет первым элементом массива */
	for (i=0; i<qwerty; i++)
		state_add(1, i);
}

/* Считает ранг, возвращает его  */
int range() {
	unsigned int i,j,k;
	int flag;
	int range = qwerty;
	unsigned int w, current_set, stuff; 
	father[1] = 0;
	current_set = 1;
	stuff = 1;	//на последнее не пустое множество в массиве

	/* Now, MAGIC! */

mark:
	while(current_set != 0) {
		flag = 0;
		for (k=0; k<erty; k++) {
			for (i=0; i<qwerty; i++) { // обходим по парам
				if(pair_approved(k,current_set)) {
					if(new_state_set(current_set, stuff, k)) {
						stuff++;
						pair_add(k,current_set);	
						father[stuff] = current_set;
						current_set = stuff;
						flag = 1;
						goto mark;
					}
				}
			}
		}
		if(!flag) {
			current_set = father[current_set];
		}
	}

	for(i=1; i<LIST_SIZE; i++)
		if(!list_empty(&state_list[i]))
			if(sizeof_set(i) < range)
				range = sizeof_set(i);
	return range;
}
/* Генерирует случайный автомат*/
void ar_random() {
	unsigned int i,j;
//	srand(time(NULL));
	for(i=0; i<qwerty; i++)
		for(j=0; j<erty; j++) {
			ar[i][j] = rand() % qwerty;
		}

}


void print_automat() {
	unsigned int i,j;
	for(i = 0; i< qwerty; i++ ) {
		for(j=0; j<erty; j++) 
			printf("%d",ar[i][j]);
		printf("\n");
	}
	printf("\n");
}
int main() {
	unsigned int i;
	srand(time(NULL));
	for (i = 0; i < 3; i++) {
		init();
		ar_random();
		printf("%d\n", range());
		cleanup();
	}

	return 0;	
}

