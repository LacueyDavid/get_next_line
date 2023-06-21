/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/18 08:14:13 by WTower            #+#    #+#             */
/*   Updated: 2023/06/21 21:05:49 by dlacuey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/////// free everything ////////////////////////////////////////////////////////

void	free_everything(t_storage *storage)
{
	free(storage->storage);
	free(storage->newlines);
	storage->storage = NULL;
	storage->newlines = NULL;
	storage->size = 0;
	storage->index = 0;
	storage->capacity = 0;
	storage->newline_found = 0;
	storage->malloc_failed = 0;
	storage->newlines_index = 0;
	storage->newlines_counted = 0;
	storage->newlines_capacity = 0;
}

/////// fill line /////////////////////////////////////////////////////////////

char	*fill_line(t_storage *storage)
{
	char *line;
	int line_begin;
	int line_end;
	int line_size;
	int index;

	index = 0;
	line = NULL;
	if (storage->newlines_index < storage->newlines_counted)
	{
		line_begin = storage->index;
		line_end = storage->newlines[storage->newlines_index] + 1;
		line_size = line_end - line_begin;
		if (line_size < 1)
			return NULL;
		line = malloc(line_size + 1);
		if (line == NULL)
		{
			storage->malloc_failed = 1;
			return (NULL);
		}
		while (index < line_size)
		{
			line[index] = storage->storage[line_begin + index];
			index++;
		}
		line[index] = '\0';
		storage->index = line_end;
		storage->newlines_index++;
	}
	return (line);
}
