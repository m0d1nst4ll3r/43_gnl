/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:44:35 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/15 19:55:46 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

static t_gnl_buf	*fill_line(t_gnl_buf *cur, char *line, int end_len)
{
	t_gnl_buf	*last;
	int			i;
	int			j;

	i = 0;
	while (cur->next)
	{
		j = cur->index;
		while (j < cur->len)
			line[i++] = cur->buf[j++];
		last = cur;
		cur = cur->next;
		free(last);
	}
	j = cur->index;
	while (j < cur->index + end_len)
		line[i++] = cur->buf[j++];
	line[i] = 0;
	return (cur);
}

static int	get_line(t_gnl_buf **buf, int end_len, char **line)
{
	t_gnl_buf	*cur;

	*line = malloc(get_len(*buf, end_len) + 1);
	if (!*line)
		return (gnl_clear_buf(*buf, line));
	cur = fill_line(*buf, *line, end_len);
	if (cur->index + end_len == cur->len)
	{
		free(cur);
		*buf = NULL;
		return (1);
	}
	cur->index += end_len;
	*buf = cur;
	return (0);
}

static int	reached_end(t_gnl_buf *buf, int *end_len)
{
	int	i;

	i = 0;
	while (buf->index + i < buf->len && buf->buf[buf->index + i] != '\n')
		i++;
	if (buf->len < BUFFER_SIZE || buf->index + i < buf->len)
	{
		*end_len = i;
		if (buf->index + i < buf->len && buf->buf[buf->index + i] == '\n')
			(*end_len)++;
		return (1);
	}
	return (0);
}

static int	gnl_read_loop(t_gnl *data, char **line)
{
	t_gnl_buf	*cur;
	t_gnl_buf	*last;
	int			end_len;

	cur = data->buf;
	while (1)
	{
		if (cur && reached_end(cur, &end_len))
			return (get_line(&data->buf, end_len, line));
		last = cur;
		cur = malloc(sizeof(*cur));
		if (!cur)
			return (gnl_clear_buf(data->buf, line));
		if (!data->buf)
			data->buf = cur;
		else
			last->next = cur;
		cur->next = NULL;
		cur->len = read(data->fd, cur->buf, BUFFER_SIZE);
		if ((!cur->len && cur == data->buf) || cur->len == -1)
			return (gnl_clear_buf(data->buf, line));
		cur->index = 0;
	}
}

char	*get_next_line(int fd)
{
	static t_gnl	*lst;
	t_gnl			*current;
	char			*line;

	current = lst_find_fd(lst, fd);
	if (!current)
		current = lst_add_fd(&lst, fd);
	if (!current)
		return (NULL);
	if (gnl_read_loop(current, &line))
		lst_remove_fd(&lst, fd);
	return (line);
}
