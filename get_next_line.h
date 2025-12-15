/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rapohlen <rapohlen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 18:41:35 by rapohlen          #+#    #+#             */
/*   Updated: 2025/12/15 19:54:54 by rapohlen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE	1024
# endif

# include <stdlib.h>
# include <unistd.h>

/*	Program structure and steps:
 *
 * 	The program uses two chained lists. One to handle multiple fds (one
 * 	 node per fd), another to handle buffer reading (each buffer has its
 * 	 own node).
 *
 *	1. List to hold memory for each fd
 *		- fd		which fd the node is using
 *		- buf		its memorized buffers
 *					these buffers are their own list (see below)
 *	2. List for buffer management
 *		- buf		the buffer sent to read
 *		- len		len returned by read
 *		- index		last pos in the buffer (for multi-newline buffers)
 *
 *	Steps:
 *
 *	1. Fetch fd's node if any, or create it
 *	2. Check buffer if any (leftover from last call)
 *		- We are checking for end condition (newline, or EOF)
 *		- EOF condition is recognized by len (read return) being less
 *		   than BUFFER_SIZE (meaning read was not full)
 *	3. Start reading in a loop, checking buffer every time
 *		- Every new read goes into its own node in the chained list
 *	4. Once end buffer is found, write the line and return
 *		- Line is written by parsing buffer list
 *		- After each buffer node is copied, it is freed
 *		- If last node is not copied completely, its index is updated
 *		   and it's placed at the start of the list for the next call
 *
 *	In case of read or malloc error, free buffer list and fd's node.
 *	After having read a file completely, free fd's node.
 *
 *	Note:
 *
 *	In this implementation, memory is not optimized for remainders
 *	 from full or partial reads.
 *
 *	  E.g, for BUFFER_SIZE of 1024:
 *
 *		- For a full read where a newline is found at index 999, there
 *		   only remain 24 bytes, but we are holding 1024 bytes in memory.
 *
 *		- For a partial read of 11 where a newline is found at index 9,
 *		   we are holding 1024 bytes for 1 remaining byte.
 *
 *	This amount of memory will be held until the next call for each
 *	 open fd. The higher BUFFER_SIZE is, the more wasted memory.
 *	Therefore, it is highly encouraged to either keep BUFFER_SIZE
 *	 reasonable or to make sure you are reading files completely.
*/
typedef struct s_gnl_buf
{
	char				buf[BUFFER_SIZE];
	int					len;
	int					index;
	struct s_gnl_buf	*next;
}	t_gnl_buf;

typedef struct s_gnl
{
	int				fd;
	t_gnl_buf		*buf;
	struct s_gnl	*next;
}	t_gnl;

// lst utils
int		gnl_clear_buf(t_gnl_buf *buf, char **line);
t_gnl	*lst_add_fd(t_gnl **lst, int fd);
t_gnl	*lst_find_fd(t_gnl *lst, int fd);
void	lst_remove_fd(t_gnl **lst, int fd);

// gnl utils
int		get_len(t_gnl_buf *buf, int end_len);

// core
char	*get_next_line(int fd);

#endif
