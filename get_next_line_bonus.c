/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:28:01 by rapohlen          #+#    #+#             */
/*   Updated: 2025/11/10 17:54:00 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

// This fills the line we are building with the buffer
//	- to_copy	tells us how many bytes to copy
//	- line		the line we are building
//	- line_len	current size of that line (before copy, for realloc)
//	- d->buf	buffer we sent to read and are copying from
// We fill the line through successive reallocs
// Every realloc widens the line by BUFFER_SIZE bytes, except at last call
// After every fill
//	- update d->index (where we are in the buffer) for subsequent gnl calls
//	- update line_len
// The if statement during copy is there to handle null bytes for binary files
//	(null bytes are replaced by 1 so binary files can output like cat would)
static int	gnl_fill2(char **line, size_t *line_len, t_gnl *d, int to_copy)
{
	int	i;

	*line = ft_realloc(*line, *line_len, to_copy);
	if (!*line)
		return (1);
	i = 0;
	while (i < to_copy)
	{
		(*line)[*line_len + i] = d->buf[d->index + i];
		if (!d->buf[d->index + i])
			(*line)[*line_len + i] = 1;
		i++;
	}
	(*line)[*line_len + i] = 0;
	*line_len += to_copy;
	d->index += to_copy;
	return (0);
}

// Called after every read (and once right after function call before 1st read)
// This checks for \n or eof
//	- if \n is found, we add up to and including the \n to the line and ret 1
//	- if no \n is found but len < BSIZ (reached EOF), fill line and ret 1
//	- if no \n is found and len == BSIZ, fill line and ret 0 (continue reading)
// If by chance we read exactly BUFFER_SIZE bytes but it would be our last read,
//	then we have no way of knowing it's our last read and we need to read again
static int	gnl_fill(char **line, size_t *line_len, t_gnl *d)
{
	int		i;

	i = 0;
	while (d->index + i < d->len && d->buf[d->index + i] != '\n')
		i++;
	if (d->index + i < d->len && d->buf[d->index + i] == '\n')
	{
		gnl_fill2(line, line_len, d, i + 1);
		return (1);
	}
	else if (i > 0)
		if (gnl_fill2(line, line_len, d, i) || d->len < BUFFER_SIZE)
			return (1);
	return (0);
}

// Main gnl function that first checks for leftovers, then calls read
//	successively until gnl_fill returns 1 (line is complete)
//	or read fails (or is done reading)
// gnl_fill does the line filling and checking for \n or EOF
// At return, line might be NULL if nothing was copied
static char	*gnl_read(t_gnl *d, int fd)
{
	char	*line;
	size_t	line_len;

	line = NULL;
	line_len = 0;
	while (1)
	{
		if (gnl_fill(&line, &line_len, d))
			break ;
		d->len = read(fd, d->buf, BUFFER_SIZE);
		d->index = 0;
		if (d->len == -1)
		{
			free(line);
			return (NULL);
		}
		if (d->len == 0)
			break ;
	}
	return (line);
}

// Static variable is a chained list with one node per fd
// If fd is unknown, create node and init index & len values
// Remove node when done reading completely or upon read/malloc error
// If program is ended before end of read, there will be still reachables
char	*get_next_line(int fd)
{
	static t_gnl	*lst = NULL;
	t_gnl			*data;
	char			*line;

	data = lst_find_fd(lst, fd);
	if (!data)
		data = lst_add_fd(&lst, fd);
	if (!data)
		return (NULL);
	line = gnl_read(data, fd);
	if (!line)
		lst_remove_fd(&lst, fd);
	return (line);
}
