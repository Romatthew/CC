#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	struct Node* pNext;
	struct Node* pPrev;
	char data;
} Node;

typedef struct List {
	struct Node* begin;
	struct Node* end;
	size_t size;
	int words_count;
} List;

List* createList() {
	List* tmp = (List*)malloc(sizeof(List));
	tmp->size = 0;
	tmp->begin = tmp->end = NULL;
	return tmp;
}

void destructor(List** list) {
	Node* tmp = (*list)->begin;
	Node* next = NULL;
	while (tmp) {
		next = tmp->pNext;
		free(tmp);
		tmp = next;
	}
	free(*list);
	(*list) = NULL;
}

	Node* get(List* list, size_t index) {
		Node* tmp = NULL;
		size_t i;

		if(index < list->size/2) {
			i = 0;
			tmp = list->begin;
			while (tmp && i < index) {
				tmp = tmp -> pNext;
				i++;
			}
		}
		else {
			i = list->size - 1;
			tmp = list->end;
			while (tmp && i > index) {
				tmp = tmp->pPrev;
				i--;
			}
		}

		return tmp;
	}

	void push_back(List* list, char data) {
		Node* tmp = (Node*)malloc(sizeof(Node));
		if (tmp == NULL) {
			exit(1);
		}
		tmp->data = data;
		tmp->pNext = NULL;
		tmp->pPrev = list->end;
		if (list->end) {
			list->end->pNext = tmp;
		}
		list->end = tmp;
		
		if (list->begin == NULL) {
			list->begin = tmp;
		}

		list->size++;

	}

	void pop_front(List* list) {
		Node* prev;
		if (list->begin == NULL)
			exit(2);

		prev = list->begin;
		list->begin = list->begin->pNext;
		if (list->begin) {
			list->begin->pPrev = NULL;
		}
		if (prev == list->end) {
			list->end = NULL;
		}
		free(prev);
		list->size--;
	}
	
	void pop_back(List* list) {
		Node* next;
		if(list->end == NULL)
			exit(3);
		next = list->end;
		list->end = list->end->pPrev;
		if (list->end) {
			list->end->pNext = NULL;
		}
		if (next == list->begin) {
			list->begin = NULL;
		}
		free(next);
		list->size--;
	}

	void delete_i(List* list, size_t i) {
		Node* elm = NULL;
		elm = get(list,i);

		if(elm == NULL) {
			exit(4);
		}
		if(elm->pPrev) {
			elm->pPrev->pNext = elm->pNext;
		}

		if(elm->pNext) {
			elm->pNext->pPrev = elm->pPrev;
		}
		if(!elm->pPrev) {
			list->begin = elm->pNext;
		}
		if(!elm->pNext) {
			list->end = elm->pPrev;
		}

		free(elm);
		list->size--;
	}

	void getword(List* list, size_t index, size_t* st_res, size_t* fin_res) {
		Node* tmp = list->begin;
		int curr = 0;
		int Sprev = 0;
		int Snext = 0;

		while (Sprev != index + 1) {
			if (tmp->data == ' ') {
				Sprev++;
			}
			tmp = tmp->pNext;
			curr++;
		}

		Snext = Sprev;
		Sprev = curr -1;

		while (Snext != index + 2) {
			if (tmp->data == ' ') {
				Snext++;
			}
			tmp = tmp->pNext;
			curr++;
		}
		Snext = curr -1;
		*st_res = Sprev;
		*fin_res = Snext;
	}


	void deleteword(List* list, size_t index) {
		size_t st;
		size_t fin;
		getword(list,index,&st,&fin);
		int count = 0;
		while(count < (fin - st)) {
			delete_i(list,st);
			count++;
		}

		list->words_count--;
	}

	void fill_array(char*common_char,List* list, int* char_count) {
		int count;
		int max = 0;
		char first;
		char second;

		for (int i = 0; i < list->size; ++i) {
			count = 0;
			first = get(list,i)->data;
			if(first != ' ') {
				for (int j = 0; j < list->size; ++i) {
					if (i != j) {
						second = get(list,j)->data;
					}
					if(first == second) {
						count++;
					}
				}
			}
			if (count >= max)
				max = count;
		}

		
		int index_common  = 0;
		for (int i = 0; i < list->size; ++i) {
			count = 0;
			first = get(list,i)->data;
			if(first != ' ') {
				for (int j = 0; j < list->size; ++j) {
					if (i != j) {
						second = get(list,j)->data;
					}
					if(first == second) {
						count++;
					}
				}
			}
			if (count == max) {
				int check = 1;
				for (int j = 0; j < index_common; ++j) {
					if(first == common_char[j]) {
						check = 0;
					}
				}
				if (check) {
					common_char[index_common] = first;
					index_common++;
				}
			}
		}
		*char_count = index_common;
	}
		void removeword(char* common_char, List* list, int char_count) {
			char curr_char;
			size_t st;
			size_t fin;
			int ind;
			
			for (int i = 0; i < char_count; ++i) {
				curr_char = common_char[i];
				ind = 0;

				while (ind < list->words_count) {
					getword(list, ind, &st, &fin);
					int flag = 1;

					for (int j = st; j < fin; ++j) {
						if (flag && get(list,j)->data == curr_char) {
							deleteword(list,ind);
							flag = 0;
						}
					}

					if (flag) {
						ind++;
					}
				}
			}
		}

		void printlist(List* list) {
			Node* tmp = list->begin;
			while(tmp) {
				printf("%c", tmp->data);
				tmp = tmp->pNext;
			}
			printf("\n");
		}

		int main() {
			int f = 1;
			while(f == 1) {
				List* tmp = createList();
				tmp->words_count = 0;
				push_back(tmp,' ');

				char symb = getchar();
				while (symb != '\n') {
					push_back(tmp, symb);
					symb = getchar();
				}

				push_back(tmp,' ');

				for (int i = 0; i < tmp->size; ++i) {
					char c = get(tmp,i)->data;
					if (c == '\t') {
						get(tmp,i)->data = ' ';
					}
				}

				int index = 0;
				while (index < tmp->size - 1) {
					if((get(tmp,index)->data == ' ')&& (get(tmp, index + 1)->data == ' ')) {
						delete_i(tmp,index + 1);
					}
					else
						index++;
				}

				for (int i = 0; i < tmp->size; ++i) {
					if(get(tmp,i)->data == ' ') {
						tmp->words_count++;
					}
				}
				tmp->words_count -= 1;

				char* common_char;
				common_char = (char*)malloc(sizeof(char) * tmp->size);
				int char_count;
				fill_array(common_char, tmp, &char_count);

				printf("\nStart :\n");
				printlist(tmp);
				removeword(common_char, tmp, char_count);

				printf("Result :\n");
				printlist(tmp);

				free(common_char);
				destructor(&tmp);
			}	
			return 0;
		}





















































