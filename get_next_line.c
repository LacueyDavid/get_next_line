/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 01:52:24 by WTower            #+#    #+#             */
/*   Updated: 2023/06/22 15:29:45 by dlacuey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static void	eof(t_storage *storage)
{
	if (storage->newlines_counted == 0)
	{
		storage->newlines_counted = 1;
		storage->newlines[0] = storage->size - 1;
	}
	else if (storage->size > storage->index
		&& storage->newlines_index >= storage->newlines_counted)
	{
		storage->newlines_counted++;
		storage->newlines[storage->newlines_counted - 1] = storage->size - 1;
	}
}

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
	eof(storage);
	if (storage->storage_full == 1)
		free(buffer->buffer);
	return (fill_line(storage));
}

static t_storage *actual_fd_storage(int fd, t_storages *storages)
{
	int i;

	if (storages->size == 0)
	{
		i = 0;
		while (i < MAP_SIZE)
		{
			storages->map[i].key = -1;
			i++;
		}
	}
	i = 0;
	while (i < MAP_SIZE)
	{
		if (storages->map[i].key == fd)
			return &(storages->map[i].storage);
		i++;
	}
	i = 0;
	while (i < storages->size)
		i++;
	storages->map[i].key = fd;
	storages->size += 1;
	return &(storages->map[i].storage);
}

char	*get_next_line(int fd)
{
	t_buffer			buffer;
	static	t_storages	storages;
	t_storage			*storage;
	char				*line;

	if (fd < 0)
		return NULL;
	storage = actual_fd_storage(fd, &storages);
	if (storage->storage == NULL)
	{
		buffer.buffer = malloc(BUFFER_SIZE);
		storage->storage = malloc(STORAGE_SIZE);
		storage->newlines = malloc(STORAGE_NEWLINES * sizeof(int));
		storage->newlines_capacity = STORAGE_NEWLINES;
		storage->capacity = STORAGE_SIZE;
		storage->key = 1;
		if (buffer.buffer == NULL || fd < 0 || storage->storage == NULL
			|| storage->newlines == NULL || BUFFER_SIZE < 1)
		{
			free(buffer.buffer);
			free_everything(&storages);
			return (NULL);
		}
	}
	line = the_next_line(fd, &buffer, storage);
	if (storage->malloc_failed == 1 || line == NULL)
		free_everything(&storages);
	return (line);
}
