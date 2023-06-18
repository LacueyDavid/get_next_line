/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 02:00:21 by WTower            #+#    #+#             */
/*   Updated: 2023/06/18 08:20:23 by WTower           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

//# define BUFFER_SIZE 1024

# include <stdlib.h>
# include <unistd.h>

typedef struct storage {
	char	*storage;
	int		size;
	int		capacity;
	int		newline_found;
	int		malloc_failed;
}			t_storage;

typedef struct buffer
{
	char	*buffer;
	int		bytesread;
}			t_buffer;

char	*get_next_line(int fd);
void	free_storage(t_storage *storage);
int		new_value_incorrect(int new_size, t_storage *storage,
								char **new_storage);
void	delet_line_in_storage(t_storage *storage);
char	*special_fill_line(t_storage *storage);
char	*fill_line(t_storage *storage);

#endif
