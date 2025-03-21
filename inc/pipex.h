/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:27:08 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/21 17:00:55 by vgoyzuet         ###   ########.fr       */
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

typedef struct s_info
{
	int		i;
	int		fd[2];
	int		pre_fd;
	int		infile;
	int		outfile;
	char	*limiter;
	pid_t	pid;
}	t_info;

void	set_info(t_info *info);
void	execute_command(char *argv, char **envp);
void	put_here_doc(char *argv, t_info info);

#endif
