#include "linklist.h"
#include <cstddef>

/* Allocate new list. */
struct list*
	list_new(void)
{
	return (struct list*)calloc(1, sizeof(struct list));
}

/* Free list. */
void
list_free(struct list* l)
{
	free(l);
}

/* Allocate new listnode.  Internal use only. */
static struct listnode*
listnode_new(void)
{
	return (struct listnode*)calloc(1, sizeof(struct listnode));
}

/* Free listnode. */
static void
listnode_free(struct listnode* node)
{
	free(node);
}

/* Add new data to the list. */
void
listnode_add(struct list* list, void* val)
{
	struct listnode* node;

	assert(val != NULL);

	node = listnode_new();

	node->prev = list->tail;
	node->data = val;

	if (list->head == NULL)
		list->head = node;
	else
		list->tail->next = node;
	list->tail = node;

	list->count++;
}

/*
 * Add a node to the list.  If the list was sorted according to the
 * cmp function, insert a new node with the given val such that the
 * list remains sorted.  The new node is always inserted; there is no
 * notion of omitting duplicates.
 */
void
listnode_add_sort(struct list* list, void* val)
{
	struct listnode* n;
	struct listnode* new;

	assert(val != NULL);

	new = listnode_new();
	new->data = val;

	if (list->cmp)
	{
		for (n = list->head; n; n = n->next)
		{
			if ((*list->cmp) (val, n->data) < 0)
			{
				new->next = n;
				new->prev = n->prev;

				if (n->prev)
					n->prev->next = new;
				else
					list->head = new;
				n->prev = new;
				list->count++;
				return;
			}
		}
	}

	new->prev = list->tail;

	if (list->tail)
		list->tail->next = new;
	else
		list->head = new;

	list->tail = new;
	list->count++;
}

void
listnode_add_after(struct list* list, struct listnode* pp, void* val)
{
	struct listnode* nn;

	assert(val != NULL);

	nn = listnode_new();
	nn->data = val;

	if (pp == NULL)
	{
		if (list->head)
			list->head->prev = nn;
		else
			list->tail = nn;

		nn->next = list->head;
		nn->prev = pp;

		list->head = nn;
	}
	else
	{
		if (pp->next)
			pp->next->prev = nn;
		else
			list->tail = nn;

		nn->next = pp->next;
		nn->prev = pp;

		pp->next = nn;
	}
	list->count++;
}


/* Delete specific date pointer from the list. */
void
listnode_delete(struct list* list, void* val)
{
	struct listnode* node;

	assert(list);
	for (node = list->head; node; node = node->next)
	{
		if (node->data == val)
		{
			if (node->prev)
				node->prev->next = node->next;
			else
				list->head = node->next;

			if (node->next)
				node->next->prev = node->prev;
			else
				list->tail = node->prev;

			list->count--;
			listnode_free(node);
			return;
		}
	}
}

/* Return first node's data if it is there.  */
void*
listnode_head(struct list* list)
{
	struct listnode* node;

	assert(list);
	node = list->head;

	if (node)
		return node->data;
	return NULL;
}

/* Delete all listnode from the list. */
void
list_delete_all_node(struct list* list)
{
	struct listnode* node;
	struct listnode* next;

	assert(list);
	for (node = list->head; node; node = next)
	{
		next = node->next;
		if (list->del)
			(*list->del) (node->data);
		listnode_free(node);
	}
	list->head = list->tail = NULL;
	list->count = 0;
}

/* Delete all listnode then free list itself. */
void
list_delete(struct list* list)
{
	assert(list);
	list_delete_all_node(list);
	list_free(list);
}

/* Lookup the node which has given data. */
struct listnode*
	listnode_lookup(struct list* list, void* data)
{
	struct listnode* node;

	assert(list);
	for (node = listhead(list); node; node = listnextnode(node))
		if (data == listgetdata(node))
			return node;
	return NULL;
}

/* Delete the node from list.. */
void
list_delete_node(struct list* list, struct listnode* node)
{
	if (node->prev)
		node->prev->next = node->next;
	else
		list->head = node->next;
	if (node->next)
		node->next->prev = node->prev;
	else
		list->tail = node->prev;
	list->count--;
	listnode_free(node);
}


void
list_add_node_prev(struct list* list, struct listnode* current, void* val)
{
	struct listnode* node;

	assert(val != NULL);

	node = listnode_new();
	node->next = current;
	node->data = val;

	if (current->prev == NULL)
		list->head = node;
	else
		current->prev->next = node;

	node->prev = current->prev;
	current->prev = node;

	list->count++;
}

void
list_add_node_next(struct list* list, struct listnode* current, void* val)
{
	struct listnode* node;

	assert(val != NULL);

	node = listnode_new();
	node->prev = current;
	node->data = val;

	if (current->next == NULL)
		list->tail = node;
	else
		current->next->prev = node;

	node->next = current->next;
	current->next = node;

	list->count++;
}

void
list_add_list(struct list* l, struct list* m)
{
	struct listnode* n;

	for (n = listhead(m); n; n = listnextnode(n))
		listnode_add(l, n->data);
}