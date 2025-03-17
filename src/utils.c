/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:56:22 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/17 01:42:38 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	find_path(char *command, char **envp, char **pathname)
{
	int		i;
	char	**directories;

	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	if (!envp[i])
		return ;
	directories = ft_split(envp[i] + 5, ':');
	if (!directories)
		ft_perror(FAIL_ALLOC);
	i = -1;
	while (directories[++i])
	{
		*pathname = so_strjoin(directories[i], "/");
		if (!*pathname)
			ft_exit_free(EXIT_FAILURE, FAIL_ALLOC, directories);
		*pathname = ft_strjoin(*pathname, command);
		if (!*pathname)
			ft_exit_free(EXIT_FAILURE, FAIL_ALLOC, directories);
		if (access(*pathname, F_OK) == 0)
			return (free_array(directories));
		free(*pathname);
	}
	return (free_array(directories));
}

void	execute_command(char *argv, char **envp)
{
	char	**command;
	char	*pathname;

	command = ft_split(argv, ' ');
	if (!command)
		ft_perror(FAIL_ALLOC);
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

void	set_info(t_info *info)
{
	info->i = 0;
	info->fd[0] = 0;
	info->fd[1] = 0;
	info->fd_tmp[0] = 0;
	info->fd_tmp[1] = 0;
	info->pre_fd = 0;
	info->pid = 0;
	info->pid_tmp = 0;
}

void	set_info_tmp(t_info *info)
{
	info->fd_tmp[0] = 0;
	info->fd_tmp[1] = 0;
	info->pid_tmp = 0;
	info->i += 1;
}
