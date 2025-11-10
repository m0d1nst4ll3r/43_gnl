/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:28:19 by rapohlen          #+#    #+#             */
/*   Updated: 2025/11/10 17:55:00 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE	1337
# endif

// Chained list, each node is for a different fd
// index is where in our buffer we left off
// len is the return of our last read
typedef struct s_gnl
{
	int				fd;
	char			buf[BUFFER_SIZE];
	int				index;
	int				len;
	struct s_gnl	*next;
}t_gnl;

// Core
char	*get_next_line(int fd);

// Utils
char	*ft_realloc(char *old, size_t old_size, size_t to_add);
t_gnl	*lst_add_fd(t_gnl **lst, int fd);
void	lst_remove_fd(t_gnl **lst, int fd);
t_gnl	*lst_find_fd(t_gnl *lst, int fd);

#endif
