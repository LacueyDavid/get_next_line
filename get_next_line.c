/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 01:52:24 by WTower            #+#    #+#             */
/*   Updated: 2023/06/21 19:52:26 by dlacuey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/////// fill storage //////////////////////////////////////////////////////////

static void double_newlines_size(t_storage *storage)
{
	int *new_newlines;
	int index;

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

static void	double_storage_size(t_storage *storage, t_buffer *buffer)
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

static void	fill_storage(t_storage *storage, t_buffer *buffer)
{
	int index;
	int storage_size;

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

/////// get next line /////////////////////////////////////////////////////////

static char	*the_next_line(int fd, t_buffer *buffer, t_storage *storage)
{
	buffer->bytesread = storage->key;
	storage->storage_full = 0;
	while (buffer->bytesread)
	{
		storage->key = 0;
		buffer->bytesread = read(fd, buffer->buffer, BUFFER_SIZE);
		if (buffer->bytesread < 0)
		{
			free(buffer->buffer);
			return (NULL);
		}
		fill_storage(storage, buffer);
		if (storage->malloc_failed == 1)
		{
			free(buffer->buffer);
			return (NULL);
		}
		storage->storage_full = 1;
	}
	if (storage->newlines_counted == 0)
	{
		storage->newlines_counted = 1;
		storage->newlines[0] = storage->size - 1;
	}
	if (storage->storage_full == 1)
		free(buffer->buffer);
	return (fill_line(storage));
}

char	*get_next_line(int fd)
{
	t_buffer			buffer;
	static t_storage	storage;
	char				*line;

	if (storage.storage == NULL)
	{
		buffer.buffer = malloc(BUFFER_SIZE);
		storage.storage = malloc(STORAGE_SIZE);
		storage.newlines = malloc(STORAGE_NEWLINES * sizeof(int));
		storage.newlines_capacity = STORAGE_NEWLINES;
		storage.capacity = STORAGE_SIZE;
		storage.key = 1;
		if (buffer.buffer == NULL || fd < 0
			|| storage.storage == NULL || storage.newlines == NULL
			|| BUFFER_SIZE < 1)
		{
			free(buffer.buffer);
			free_everything(&storage);
			return (NULL);
		}
	}
	if (storage.malloc_failed == 1)
	{
		free_everything(&storage);
		return (NULL);
	}
	line = the_next_line(fd, &buffer, &storage);
	if (storage.malloc_failed == 1 || line == NULL)
		free_everything(&storage);
	return line;
}
