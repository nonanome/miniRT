/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbageCollector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qhahn <qhahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 17:40:51 by qhahn             #+#    #+#             */
/*   Updated: 2025/04/03 17:51:46 by qhahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "garbageCollector.h"

void	*get_adress_of_list(void)
{
	static t_list	list = {0};

	return (&list);
}

void	delete_lst_data(t_list **lst, void *data)
{
	t_list	*current;
	t_list	*prev;

	current = (*lst)->next;
	prev = *lst;
	while (current)
	{
		if (current->content == data)
		{
			prev->next = current->next;
			free(current->content);
			current->content = NULL;
			free(current);
			current = NULL;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

void	FREE(void *data)
{
	t_list	*list;

	list = get_adress_of_list();
	delete_lst_data(&list, data);
}

void	*MALLOC(int size)
{
	t_list	*list;
	void	*content;
	t_list	*new_node;

	content = malloc(size);
	if (!content)
		return (NULL);
	list = get_adress_of_list();
	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(content);
		content = NULL;
		return (NULL);
	}
	new_node->content = content;
	new_node->next = list->next;
	list->next = new_node;
	return (content);
}

void	*rt_realloc(void *ptr, int new_size)
{
	t_list	*lst;
	t_list	*current;
	void	*new_ptr;

	lst = get_adress_of_list();
	current = lst->next;
	if (!ptr)
		return (MALLOC(new_size));
	while (current)
	{
		if (current->content == ptr)
		{
			new_ptr = realloc(ptr, new_size);
			if (new_ptr)
				current->content = new_ptr;
			return (new_ptr);
		}
		current = current->next;
	}
	return (realloc(ptr, new_size));
}

void	__attribute__((destructor)) free_list(void)
{
	t_list	*list;
	t_list	*current;
	t_list	*next;

	list = get_adress_of_list();
	current = list->next;
	while (current)
	{
		next = current->next;
		if (current->content)
		{
			free(current->content);
			current->content = NULL;
		}
		free(current);
		current = NULL;
		current = next;
	}
	list->next = NULL;
}
