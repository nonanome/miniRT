#include "garbageCollector.h"



void *get_adress_of_list()
{
	static t_list list;

	return &list;
}

void	delete_lst_data(t_list *list, void *data)
{
	void	*next;

	if (list == 0)
		return ;
	while(list)
	{
		next = list->next;
		if (list->content != NULL && &list->content == data)
		{
			free(list->content);
			list->content = NULL;
		}
		list = next;
	}

}

void	FREE(void *data)
{
	void	*ptrlist;
	void	*next;

	ptrlist = get_adress_of_list();
	delete_lst_data(ptrlist, data);
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

void __attribute__ ((destructor)) free_list(void)
{
    t_list *ptrlist= get_adress_of_list();
    if (ptrlist != NULL) {
        ft_lstclear(&ptrlist, free);
    }
}
