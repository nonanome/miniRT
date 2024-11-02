#include "garbageCollector.h"

void *get_adress_of_list()
{
	static t_list list;

	return &list;
}

void *MALLOC(int size)
{
	void *ptrlist;
	void *content;
	t_list *new_node;

	content = malloc(size);
	ptrlist = get_adress_of_list();
	new_node = ft_lstnew(content);
	ft_lstadd_front(ptrlist, new_node);
	return content;
}

void __attribute__ ((destructor))	free_list(void)
{
	void	*ptrlist;

	ptrlist = get_adress_of_list();
	ft_lstclear(ptrlist, free);
}
