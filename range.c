
#include "mylist.h"


#define qwerty 10
#define erty 2
//static int q = 3,e = 2; //|Q|, |E|


/* Тестовый автоматик */
static unsigned int ar[qwerty][erty];
int tiny[qwerty*erty];

/* Добавляет новое множество состояний, 
 * также надо проверять, было ли уже такое множество 
 * текущее множество - под индексом i, последнее - под j 
 * w - буква, по которой переходим*/
int new_state_set(unsigned int i, unsigned int j, unsigned int w) {
	struct state *pos;
	unsigned int q_tmp;
	list_for_each_entry(pos, &state_list[i], list) {
		q_tmp = tiny[pos->q * 2 + w];
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
	unsigned int i,j,k,l;
	int flag;
	int range = qwerty;
	unsigned int w, current, stuff; 
	father[1] = 0;
	current = 1;
	stuff = 1;	//на последнее не пустое множество в массиве
	/* Now, MAGIC! */
	while(current != 0) {
		flag = 0;
		for(j=0; j<erty; j++) {
			if(new_state_set(current, stuff,j)) {
				stuff++;
				father[stuff]=current;
				flag = 1;
				if(sizeof_set(stuff) < range)
					range = sizeof_set(stuff);
				if(range == 1)
					goto mark;
			}
		}
		if(flag)
			current++;
		else
			current = father[current];
	}
mark:
	return range;
}

/* Генерирует случайный автомат*/
void ar_random() {
	unsigned int i,j;
//	srand(time(NULL));
	for(i=0; i<qwerty; i++)
		for(j=0; j<erty; j++) {
			tiny[i*2 + j] = rand() % qwerty;
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
	while(1) {
		init();
		ar_random();
		printf("%d\n", range());
		cleanup();
	}

	return 0;	
}

