/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 16:56:22 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/27 16:28:10 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	set_info(t_info *info)
{
	info->i = 0;
	info->pre_fd = 0;
	info->infile = 0;
	info->outfile = 0;
	info->pid = 0;
	info->pid_tmp = 0;
}

void	put_here_doc(int *fd, char *limiter)
{
	char	*line;
	int		pid;

	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		while (1)
		{
			ft_putstr_fd("> ", STDOUT_FILENO);
			line = get_next_line(STDIN_FILENO);
			if ((ft_strnstr(line, limiter, ft_strlen(limiter))
				&& ft_strlen(line) - 1 == ft_strlen(limiter)) || !line)
			{
				if (line)
					free(line);
				break ;
			}
			write(fd[1], line, ft_strlen(line));
			free(line);
		}
		if (close(fd[1]) == -1)
			ft_perror(FAIL_CLOSE_FD);
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (waitpid(pid, NULL, 0) == -1)
			ft_perror(FAIL_WAIT);
		if (close(fd[1]) == -1)
			ft_perror(FAIL_CLOSE_FD);
	}
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
}

static void	find_path(char *command, char **envp, char **path)
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
		return ;
	i = -1;
	while (directories[++i])
	{
		*path = so_strjoin(directories[i], "/");
		if (!*path)
			return (free_array(directories));
		*path = ft_strjoin(*path, command);
		if (!*path)
			return (free_array(directories));
		if (access(*path, F_OK) == 0)
			return (free_array(directories));
		free(*path);
	}
	return (free_array(directories));
}

void	execute_command(char *argv, char **envp)
{
	char	**command;
	char	*path;

	command = ft_split(argv, ' ');
	if (!command)
		ft_perror(FAIL_ALLOC);
	find_path(command[0], envp, &path);
	if (!path)
		ft_exit_free(EXIT_FAILURE, FAIL_COM, command);
	if (execve(path, command, envp) == -1)
	{
		free(path);
		ft_exit_free(EXIT_FAILURE, FAIL_EXEC, command);
	}
	free(path);
	free_array(command);
}
