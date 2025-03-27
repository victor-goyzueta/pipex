/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:27:08 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/27 16:28:28 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <sys/types.h>
# include <sys/wait.h>

# define USAGE			"./pipex file1 first_command ... last_command file2"
# define FAIL_PIPE		"Failed pipe function"
# define FAIL_FORK		"Failed fork function"
# define FAIL_WAIT		"Failed waitpid function"
# define FAIL_OPEN_FD	"Failed to open fd"
# define FAIL_CLOSE_FD	"Failed to close fd"
# define FAIL_FLOW		"Failure to redirect flows"
# define FAIL_CHILD		"Failure to redirect flows in child process"
# define FAIL_MID		"Failure to redirect flows in middle process"
# define FAIL_PARENT	"Failure to redirect flows in parent process"
# define FAIL_ALLOC		"Failed memory allocation"
# define FAIL_EXEC		"Failed execve function"
# define FAIL_COM		"Failed command"

typedef struct s_info
{
	int		i;
	int		fd[2];
	int 	fd_tmp[2];
	int 	pre_fd;
	int		infile;
	int		outfile;
	pid_t	pid;
	pid_t 	pid_tmp;
	bool	check;
} t_info;

void	set_info(t_info *info);
void	execute_command(char *argv, char **envp);
void	put_here_doc(int *fd, char *limiter);
void	middle_process(char **argv, char **envp, t_info info, int pre_fd);

#endif
