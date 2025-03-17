/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:56:22 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/17 21:23:11 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	execute_command(char *argv, char **envp)
{
	char	**command;
	char	*pathname;

	command = ft_split(argv, ' '); //or get_command
	if (!command)
		ft_perror(FAIL_ALLOC); //free?
	pathname = NULL;
	find_path(command[0], envp, &pathname);
	if (!pathname)
	{
		free(pathname);
		ft_exit_free(EXIT_FAILURE, NULL, command);
	}
	if (execve(pathname, command, envp) == -1)
	{
		free(pathname);
		ft_exit_free(EXIT_FAILURE, NULL, command);
	}
}

void	set_info(t_info *info, bool check)
{
	info->fd_tmp[0] = 0;
	info->fd_tmp[1] = 0;
	info->pid_tmp = 0;
	if (check)
	{
		info->i = 0;
		info->fd[0] = 0;
		info->fd[1] = 0;
		info->pre_fd = 0;
		info->pid = 0;
	}
	else
		info->i += 1;
}

void	middle_process(char **argv, char **envp, t_info info, int pre_fd)
{
	if (dup2(pre_fd, STDIN_FILENO) == -1
		|| dup2(info.fd_tmp[1], STDOUT_FILENO) == -1)
		ft_perror(FAIL_MID);
	if (close(pre_fd) == -1 || close(info.fd_tmp[0]) == -1
		|| close(info.fd_tmp[1]) == -1 || close(info.fd[1]) == -1)
		ft_perror(FAIL_CLOSE_FD);
	execute_command(argv[info.i], envp);
	exit(EXIT_SUCCESS);
}
