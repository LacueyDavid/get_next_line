/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 01:52:24 by WTower            #+#    #+#             */
/*   Updated: 2023/06/18 08:01:18 by WTower           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/////// free storage ///////////////////////////////////////////////////////////

static void free_storage(t_storage *storage)
{
	storage->size = 0;
	storage->capacity = 0;
	storage->newline_found = 0;
	storage->malloc_failed = 0;
	free(storage->storage);
	storage->storage = NULL;
}

/////// delet line in storage //////////////////////////////////////////////////

static int new_value_incorrect(int new_size, t_storage *storage, char **new_storage)
{
	if (new_size == 0)
	{
		free(storage->storage);
		storage->storage = NULL;
		storage->capacity = new_size;
		storage->size = new_size;
		return 1;
	}
	*new_storage = malloc(new_size);
	if (*new_storage == NULL)
	{
		storage->malloc_failed = 1;
		return 1;
	}
	return 0;
}

static void delet_line_in_storage(t_storage *storage)
{
	int index;
	int deleter;
	int new_size;
	char *new_storage;

	index = 0;
	deleter = 0;
	while (storage->storage[index] != '\n' && index < storage->size)
		index++;
	if (storage->storage[index] == '\n')
		index++;
	new_size = storage->size - index;
	if (new_value_incorrect(new_size, storage, &new_storage))
		return;
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

static char *special_fill_line(t_storage *storage)
{
	int index;
	int filler;
	char *line;

	index = 0;
	filler = 0;
	while (index < storage->size)
		index++;
	line = malloc(index + 1);
	if (line == NULL)
		return NULL;
	while (filler < index)
	{
		line[filler] = storage->storage[filler];
		filler++;
	}
	line[filler] = '\0';
	storage->size = 0;
	return line;
}

static char *fill_line(t_storage *storage)
{
	int index;
	int filler;
	char *line;

	index = 0;
	filler = 0;
	while (storage->storage[index] != '\n' && index < storage->size - 1)
		index++;
	if (storage->storage[index] == '\n')
		index++;
	line = malloc(index + 1);
	if (line == NULL)
		return NULL;
	while (filler < index)
	{
		line[filler] = storage->storage[filler];
		filler++;
	}
	line[filler] = '\0';
	storage->newline_found--;
	delet_line_in_storage(storage);
	return line;
}

/////// test storage //////////////////////////////////////////////////////////

static void test_storage(t_storage *storage)
{
	int index;

	index = 0;
	while (index < storage->size)
	{
		if (storage->storage[index] == '\n')
			storage->newline_found++;
		index++;
	}
}

/////// fill storage //////////////////////////////////////////////////////////

static void double_storage_size(t_storage *storage, t_buffer buffer)
{
	char *new_storage;
	int index;

	storage->capacity *= 2;
	if (storage->capacity < buffer.bytesread + storage->size)
		storage->capacity = buffer.bytesread + storage->size;
	index = 0;
	new_storage = malloc(storage->capacity);
	if (new_storage == NULL)
	{
		storage->malloc_failed = 1;
		return;
	}
	while (index < storage->size)
	{
		new_storage[index] = storage->storage[index];
		index++;
	}
	free(storage->storage);
	storage->storage = new_storage;
}

static void fill_storage(t_storage *storage, t_buffer buffer)
{
	int	index;

	index = 0;
	if (storage->capacity == 0)
	{
		storage->storage = malloc(buffer.bytesread);
		if (storage->storage == NULL)
			storage->malloc_failed = 1;
		while (index < buffer.bytesread)
		{
			storage->storage[index] = buffer.buffer[index];
			index++;
		}
		storage->size = buffer.bytesread;
		storage->capacity = buffer.bytesread;
		return;
	}
	else if (storage->capacity < storage->size + buffer.bytesread)
		double_storage_size(storage, buffer);
	while (index < buffer.bytesread)
	{
		storage->storage[storage->size + index] = buffer.buffer[index];
		index++;
	}
	storage->size += buffer.bytesread;
}

/////// get next line /////////////////////////////////////////////////////////

static char *the_next_line(int fd, t_buffer buffer, t_storage *storage)
{
	while (!storage->newline_found)
	{
		buffer.bytesread = read(fd, buffer.buffer, BUFFER_SIZE);
		if (buffer.bytesread == -1 || buffer.bytesread == 0)
		{
			free(buffer.buffer);
			if (storage->size > 0)
				return special_fill_line(storage);
			free_storage(storage);
			return (NULL);
		}
		fill_storage(storage, buffer);
		if (storage->malloc_failed == 1)
		{
			free(buffer.buffer);
			free_storage(storage);
			return NULL;
		}
		test_storage(storage);
	}
	free(buffer.buffer);
	return fill_line(storage);
}

char	*get_next_line(int fd)
{
	t_buffer buffer;
	static t_storage storage;

	buffer.buffer = malloc(BUFFER_SIZE);
	if (buffer.buffer == NULL)
		return (NULL);
	if (fd < 0 || BUFFER_SIZE < 1 || storage.malloc_failed == 1)
	{
		free(buffer.buffer);
		return (NULL);
	}
	return the_next_line(fd, buffer, &storage);
}
