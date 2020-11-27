#include "linked_list.h"
#include "revo_os.h"

void list_zero(list_t *list)
{
	memset(list, 0, sizeof(list_t));
}

list_t *list_initialize(void)
{
	list_t *new_list;
	new_list = (list_t *)revo_malloc(sizeof(list_t));
	list_zero(new_list);
	
	return new_list;
}

static void list_append_no_malloc(list_t *list, void *content, list_element_t *new_element, size_t size)
{
	new_element->content = content;
	new_element->next = NULL;
	new_element->prev = list->last;
	if (list->first == NULL)
		list->first = new_element;
	else
		list->last->next = new_element;
	list->last = new_element;
	++(list->count);
	list->size += size;
}

revo_s32 list_append_item(list_t *list, void *content, size_t size)
{
	list_element_t *new_element = revo_malloc(sizeof(list_element_t));
	if(new_element == NULL)
	{
		return REVO_FAIL;
	}
	
	memset(new_element, 0, sizeof(list_element_t));
	list_append_no_malloc(list, content, new_element, size);

	return REVO_SUCCESS;
}

static revo_s32 list_unlink_current(list_t* list, revo_s32 free_content)
{
	list_element_t* prev = NULL;

	if(list->current == NULL)
		return REVO_FAIL;

	if (list->current->prev == NULL)
	{
		/* so this is the first element, and we have to update the "first" pointer */
		list->first = list->current->next;
		if(list->first)
			list->first->prev = NULL;
	}else{
		list->current->prev->next = list->current->next;
	}

	if (list->current->next == NULL)
	{
		list->last = list->current->prev;
		if(list->last)
			list->last->next = NULL;
	}else{
		list->current->next->prev = list->current->prev;
	}

	prev = list->current->prev;
	if (free_content)
    {
		revo_free(list->current->content);
		list->current->content = NULL;
	}
	revo_free(list->current);

	list->current = prev;
	--(list->count);
	
	return REVO_SUCCESS; /* successfully removed item */
}

static revo_s32 list_unlink(list_t* list, void* content, revo_s32(*callback)(void*, void*), revo_s32 free_content)
{
	list_element_t* next = NULL;
	list_element_t* saved = list->current;
	revo_s32 saveddeleted = 0;

	if (!list_find_item(list, content, callback))
		return REVO_FAIL; /* false, did not remove item */

	if (list->current->prev == NULL)
	{
		/* so this is the first element, and we have to update the "first" pointer */
		list->first = list->current->next;
		if(list->first)
			list->first->prev = NULL;
	}else{
		list->current->prev->next = list->current->next;
	}

	if (list->current->next == NULL)
	{
		list->last = list->current->prev;
		if(list->last)
			list->last->next = NULL;
	}else{
		list->current->next->prev = list->current->prev;
	}

	next = list->current->next;
	if (free_content)
    {
		revo_free(list->current->content);
		list->current->content = NULL;
	}
	
	if (saved == list->current)
		saveddeleted = 1;
	
	revo_free(list->current);
	if (saveddeleted)
		list->current = next;
	else
		list->current = saved;
	
	--(list->count);
	
	return REVO_SUCCESS; /* successfully removed item */
}

revo_s32 list_remove_item(list_t *list, void *content, revo_s32(*callback)(void*, void*))
{ /* remove from list and free the content */
	return list_unlink(list, content, callback, 1);
}

revo_s32 list_remove_item_cur(list_t *list)
{
	return list_unlink_current(list, 1);
}

revo_s32 list_remove_item_first(list_t *list)
{
	list_element_t *tmp = NULL;

	if(list->first == NULL)
		return REVO_FAIL;
	
	tmp = list->first;
	if(list->current == tmp)
		list->current = tmp->next;

	list->first = tmp->next;
	if(list->first)
		list->first->prev = NULL;
	else
		list->last = NULL;
	
	//if(free_content)
		revo_free(tmp->content);

	revo_free(tmp);
	--(list->count);
	
	return REVO_SUCCESS;
}

list_element_t* list_find_item(list_t *list, void *content, revo_s32(*callback)(void*, void*))
{
	list_element_t* rc = NULL;

	list_element_t* current = NULL;

	/* find the content */
	while (list_next_element(list, &current) != NULL)
	{
		if (callback == NULL)
		{
			if (current->content == content)
			{
				rc = current;
				break;
			}
		}
		else
		{
			if (callback(current->content, content))
			{
				rc = current;
				break;
			}
		}
	}
	if (rc != NULL)
		list->current = rc;

	return rc;
}

list_element_t *list_next_element(list_t *list, list_element_t **list_element_pos)
{
	return *list_element_pos = (*list_element_pos) ? (*list_element_pos)->next : list->first;
}

list_element_t *list_prev_element(list_t *list, list_element_t **list_element_pos)
{
	return *list_element_pos = (*list_element_pos) ? (*list_element_pos)->prev : list->last;
}

revo_s32 int_compare(void* a, void* b)
{
	return *((int*)a) == *((int*)b);
}

revo_s32 str_compare(void* a, void* b)
{
	return strcmp((char*)a, (char*)b) == 0;
}

