/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 08:14:13 by WTower            #+#    #+#             */
/*   Updated: 2023/06/22 15:22:58 by dlacuey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	free_everything(t_storages *storages)
{
	int i;

	i = 0;
	while (i < MAP_SIZE)
	{
		if (storages->map[i].key != -1)
		{
			free(storages->map[i].storage.storage);
			free(storages->map[i].storage.newlines);
			storages->map[i].storage.storage = NULL;
			storages->map[i].storage.newlines = NULL;
			storages->map[i].storage.size = 0;
			storages->map[i].storage.index = 0;
			storages->map[i].storage.capacity = 0;
			storages->map[i].storage.newline_found = 0;
			storages->map[i].storage.malloc_failed = 0;
			storages->map[i].storage.newlines_index = 0;
			storages->map[i].storage.newlines_counted = 0;
			storages->map[i].storage.newlines_capacity = 0;
		}
		i++;
	}
}

void	double_newlines_size(t_storage *storage)
{
	int	*new_newlines;
	int	index;

	index = 0;
	new_newlines = malloc(sizeof(int) * (storage->newlines_capacity * 2));
	if (new_newlines == NULL)
	{
		storage->malloc_failed = 1;
		return ;
	}
	while (index < storage->newlines_counted)
	{
		new_newlines[index] = storage->newlines[index];
		index++;
	}
	free(storage->newlines);
	storage->newlines = new_newlines;
	storage->newlines_capacity *= 2;
}

void	double_storage_size(t_storage *storage, t_buffer *buffer)
{
	char	*new_storage;
	int		index;

	storage->capacity *= 2;
	if (storage->capacity < buffer->bytesread + storage->size)
		storage->capacity = buffer->bytesread + storage->size;
	index = 0;
	new_storage = malloc(storage->capacity);
	if (new_storage == NULL)
	{
		storage->malloc_failed = 1;
		return ;
	}
	while (index < storage->size)
	{
		new_storage[index] = storage->storage[index];
		index++;
	}
	free(storage->storage);
	storage->storage = new_storage;
}

void	fill_storage(t_storage *storage, t_buffer *buffer)
{
	int	index;
	int	storage_size;

	index = 0;
	storage_size = storage->size;
	if (storage->capacity < storage->size + buffer->bytesread)
		double_storage_size(storage, buffer);
	if (storage->malloc_failed == 1)
		return ;
	while (index < buffer->bytesread)
	{
		storage->storage[storage_size + index] = buffer->buffer[index];
		if (storage->storage[storage_size + index] == '\n')
		{
			storage->newlines[storage->newlines_counted] = storage_size + index;
			storage->newlines_counted++;
			if (storage->newlines_counted >= storage->newlines_capacity)
				double_newlines_size(storage);
			if (storage->malloc_failed == 1)
				return ;
		}
		index++;
	}
	storage->size += buffer->bytesread;
}

char	*fill_line(t_storage *storage)
{
	t_line	line;

	line.i = 0;
	line.string = NULL;
	if (storage->newlines_index < storage->newlines_counted)
	{
		line.line_begin = storage->index;
		storage->index = storage->newlines[storage->newlines_index] + 1;
		if (storage->index - line.line_begin < 1)
			return (NULL);
		line.string = malloc(storage->index - line.line_begin + 1);
		if (line.string == NULL)
		{
			storage->malloc_failed = 1;
			return (NULL);
		}
		while (line.i < storage->index - line.line_begin)
		{
			line.string[line.i] = storage->storage[line.line_begin + line.i];
			line.i++;
		}
		line.string[line.i] = '\0';
		storage->newlines_index++;
	}
	return (line.string);
}
