#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "revo_typedef.h"

typedef revo_u32 size_t;

struct list_element_struct{
	struct list_element_struct *next;
	struct list_element_struct *prev;
	void *content;
};

typedef struct list_element_struct list_element_t;

typedef struct{
	list_element_t *first;
	list_element_t *last;
	list_element_t *current;
	revo_u32 count;
	revo_u32 size;
}list_t;

void list_zero(list_t *list);
list_t *list_initialize(void);
revo_s32 list_append_item(list_t *list, void *content, size_t size);
revo_s32 list_remove_item(list_t *list, void *content, revo_s32(*callback)(void*, void*));
revo_s32 list_remove_item_first(list_t *list);
revo_s32 list_remove_item_cur(list_t *list);
list_element_t* list_find_item(list_t *list, void *content, revo_s32(*callback)(void*, void*));
list_element_t *list_next_element(list_t *list, list_element_t **list_element_pos);
list_element_t *list_prev_element(list_t *list, list_element_t **list_element_pos);
revo_s32 int_compare(void* a, void* b);
revo_s32 str_compare(void* a, void* b);

#endif