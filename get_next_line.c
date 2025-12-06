/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:44:35 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/06 21:42:21 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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

static char	*get_line(t_gnl_buf **buf, int end_len)
{
	t_gnl_buf	*cur;
	char		*line;

	line = malloc(get_len(*buf, end_len) + 1);
	if (!line)
		return (gnl_clear_buf(*buf));
	cur = fill_line(*buf, line, end_len);
	if (cur->index + end_len == cur->len)
	{
		free(cur);
		cur = NULL;
	}
	else
		cur->index += end_len;
	*buf = cur;
	return (line);
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

static char	*gnl_read_loop(t_gnl *data)
{
	t_gnl_buf	*cur;
	t_gnl_buf	*last;
	int			end_len;

	cur = data->buf;
	while (1)
	{
		if (cur && reached_end(cur, &end_len))
			return (get_line(&data->buf, end_len));
		last = cur;
		cur = malloc(sizeof(*cur));
		if (!cur)
			return (gnl_clear_buf(data->buf));
		if (!data->buf)
			data->buf = cur;
		else
			last->next = cur;
		cur->next = NULL;
		cur->len = read(data->fd, cur->buf, BUFFER_SIZE);
		if ((!cur->len && cur == data->buf) || cur->len == -1)
			return (gnl_clear_buf(data->buf));
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
	line = gnl_read_loop(current);
	if (!line)
		lst_remove_fd(&lst, fd);
	return (line);
}
