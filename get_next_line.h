/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 02:00:21 by WTower            #+#    #+#             */
/*   Updated: 2023/06/21 20:23:05 by dlacuey          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# define BUFFER_SIZE 1
# define STORAGE_SIZE 1024
# define STORAGE_NEWLINES 1

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>

typedef struct storage {
	char		*storage;
	int			size;
	int			capacity;
	int			index;
	int			newline_found;
	int			malloc_failed;
	int			key;
	int 		*newlines;
	int			newlines_index;
	int			newlines_capacity;
	int			newlines_counted;
	int			storage_full;
	int			endoffile;
}			t_storage;

typedef struct buffer
{
	char	*buffer;
	int		bytesread;
}			t_buffer;

typedef struct line_infos
{
	char	*string;
	int		size;
	int		last;
}			t_line;

void	free_everything(t_storage *storage);
char	*get_next_line(int fd);
void	free_storage(t_storage *storage);
int		new_value_incorrect(int new_size, t_storage *storage,
								char **new_storage);
void	delet_line_in_storage(t_storage *storage);
char	*last_fill_line(t_storage *storage);
char	*fill_line(t_storage *storage);

#endif
