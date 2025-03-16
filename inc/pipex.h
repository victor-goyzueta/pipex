/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:27:08 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/16 20:27:34 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <sys/types.h>
# include <sys/wait.h>

# define USAGE			"./pipex file1 first_command middle_commands(optional) last_command file2"
# define FAIL_PIPE		"Failed pipe function"
# define FAIL_FORK		"Failed fork function"
# define FAIL_WAIT		"Failed waitpid function"
# define FAIL_OPEN_FD	"Failed to open fd"
# define FAIL_CLOSE_FD	"Failed to close fd"
# define FAIL_CHILD		"Failure to redirect flows in child process"
# define FAIL_PARENT	"Failure to redirect flows in parent process"
# define FAIL_ALLOC		"Failed memory allocation"

typedef struct s_info
{
	int		i;
	int		fd[2];
	int		fd_tmp[2];
	pid_t	pid;
	pid_t	pid_tmp;
}	t_info;

void	execute_command(char *argv, char **envp);
void	set_info(t_info *info);

#endif
