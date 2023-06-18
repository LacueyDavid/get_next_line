/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 08:14:13 by WTower            #+#    #+#             */
/*   Updated: 2023/06/18 08:16:51 by WTower           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/////// free storage ///////////////////////////////////////////////////////////

void	free_storage(t_storage *storage)
{
	storage->size = 0;
	storage->capacity = 0;
	storage->newline_found = 0;
	storage->malloc_failed = 0;
	free(storage->storage);
	storage->storage = NULL;
}

/////// delet line in storage //////////////////////////////////////////////////

int	new_value_incorrect(int new_size, t_storage *storage,
	char **new_storage)
{
	if (new_size == 0)
	{
		free(storage->storage);
		storage->storage = NULL;
		storage->capacity = new_size;
		storage->size = new_size;
		return (1);
	}
	*new_storage = malloc(new_size);
	if (*new_storage == NULL)
	{
		storage->malloc_failed = 1;
		return (1);
	}
	return (0);
}

void	delet_line_in_storage(t_storage *storage)
{
	int		index;
	int		deleter;
	int		new_size;
	char	*new_storage;

	index = 0;
	deleter = 0;
	while (storage->storage[index] != '\n' && index < storage->size)
		index++;
	if (storage->storage[index] == '\n')
		index++;
	new_size = storage->size - index;
	if (new_value_incorrect(new_size, storage, &new_storage))
		return ;
	while (index < storage->size)
	{
		new_storage[deleter] = storage->storage[index];
		deleter++;
		index++;
	}
	free(storage->storage);
	storage->storage = new_storage;
	storage->capacity = new_size;
	storage->size = new_size;
}

/////// fill line /////////////////////////////////////////////////////////////

char	*special_fill_line(t_storage *storage)
{
	int		index;
	int		filler;
	char	*line;

	index = 0;
	filler = 0;
	while (index < storage->size)
		index++;
	line = malloc(index + 1);
	if (line == NULL)
		return (NULL);
	while (filler < index)
	{
		line[filler] = storage->storage[filler];
		filler++;
	}
	line[filler] = '\0';
	storage->size = 0;
	return (line);
}

char	*fill_line(t_storage *storage)
{
	int		index;
	int		filler;
	char	*line;

	index = 0;
	filler = 0;
	while (storage->storage[index] != '\n' && index < storage->size - 1)
		index++;
	if (storage->storage[index] == '\n')
		index++;
	line = malloc(index + 1);
	if (line == NULL)
		return (NULL);
	while (filler < index)
	{
		line[filler] = storage->storage[filler];
		filler++;
	}
	line[filler] = '\0';
	storage->newline_found--;
	delet_line_in_storage(storage);
	return (line);
}
