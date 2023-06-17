/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: WTower <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 02:00:21 by WTower            #+#    #+#             */
/*   Updated: 2023/06/17 03:47:18 by WTower           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

//# define BUFFER_SIZE 1024

# include <stdlib.h>
# include <unistd.h>
# include <string.h> // test
# include <stdio.h>  // test
# include <fcntl.h>  // test

typedef struct storage {
	char	*storage;
	int		size;
	int		capacity;
	int		newline_found;
	int		malloc_failed;
}			t_storage;

typedef struct buffer
{
	char	buffer[BUFFER_SIZE];
	int		bytesread;
}			t_buffer;

char	*get_next_line(int fd);

#endif
