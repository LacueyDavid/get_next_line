/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 02:00:21 by WTower            #+#    #+#             */
/*   Updated: 2023/06/22 15:51:47 by dlacuey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# define MAP_SIZE 20
# define STORAGE_SIZE 1024
# define STORAGE_NEWLINES 1

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct storage
{
	char		*storage;
	int			size;
	int			capacity;
	int			index;
	int			newline_found;
	int			malloc_failed;
	int			key;
	int			*newlines;
	int			newlines_index;
	int			newlines_capacity;
	int			newlines_counted;
	int			storage_full;
	int			endoffile;
}			t_storage;

typedef struct map
{
	int			key;
	t_storage	storage;
}			t_map;

typedef struct storages
{
	t_map		map[MAP_SIZE];
	int			size;
}			t_storages;

typedef struct buffer
{
	char	*buffer;
	int		bytesread;
}			t_buffer;

typedef struct line_infos
{
	char	*string;
	int		line_begin;
	int		i;
}			t_line;

void	double_newlines_size(t_storage *storage);
void	double_storage_size(t_storage *storage, t_buffer *buffer);
void	fill_storage(t_storage *storage, t_buffer *buffer);
void	free_everything(t_storage *storage);
char	*get_next_line(int fd);
int		new_value_incorrect(int new_size, t_storage *storage,
			char **new_storage);
void	delet_line_in_storage(t_storage *storage);
char	*last_fill_line(t_storage *storage);
char	*fill_line(t_storage *storage);

#endif
