/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:32:54 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/17 13:22:59 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	middle_process(char **argv, char **envp, t_info info, int pre_fd)
{
	if (dup2(pre_fd, STDIN_FILENO) == -1
		|| dup2(info.fd_tmp[1], STDOUT_FILENO) == -1)
		ft_perror(FAIL_MID);
	if (close(pre_fd) == -1 || close(info.fd_tmp[0]) == -1
		|| close(info.fd[1]) == -1)
		ft_perror(FAIL_CLOSE_FD);
	execute_command(argv[info.i], envp);
	exit(EXIT_SUCCESS);
}

static void	check_process(char **argv, char **envp, t_info *info, int argc)
{
	if (argc < 6)
		return ;
	if (!info)
		ft_perror(FAIL_ALLOC);
	info->i = 2;
	info->pre_fd = info->fd[0];
	while (info->i < argc - 2)
	{
		set_info_tmp(info);
		if (pipe(info->fd_tmp) == -1)
			ft_perror(FAIL_PIPE);
		info->pid_tmp = fork();
		if (info->pid_tmp == -1)
			ft_perror(FAIL_FORK);
		if (info->pid_tmp == 0)
			middle_process(argv, envp, *info, info->pre_fd);
		else
		{
			if (close(info->fd_tmp[1]) == -1 || close(info->pre_fd) == -1)
				ft_perror(FAIL_CLOSE_FD);
			info->pre_fd = info->fd_tmp[0];
		}
	}
	info->fd[0] = info->pre_fd;
}

static void	child_process(char **argv, char **envp, t_info info)
{
	int	filein;

	if (info.pid != 0)
		return ;
	filein = open(argv[1], O_RDONLY);
	if (filein == -1)
		ft_perror(FAIL_OPEN_FD);
	if (dup2(info.fd[1], STDOUT_FILENO) == -1
		|| dup2(filein, STDIN_FILENO) == -1)
	{
		if (close(filein) == -1 || close(info.fd[0]) == -1
			|| close(info.fd[1]) == -1)
			ft_perror(FAIL_CLOSE_FD);
		ft_perror(FAIL_CHILD);
	}
	if (close(filein) == -1 || close(info.fd[0]) == -1
		|| close(info.fd[1]) == -1)
		ft_perror(FAIL_CLOSE_FD);
	execute_command(argv[2], envp);
	exit(EXIT_SUCCESS);
}

static void	parent_process(char **argv, char **envp, t_info info)
{
	int	i;
	int	fileout;

	if (info.pid == 0)
		return ;
	if (waitpid(info.pid, NULL, 0) == -1)
		ft_perror(FAIL_WAIT);
	i = 0;
	while (argv[i])
		i++;
	fileout = open(argv[i - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
		ft_perror(FAIL_OPEN_FD);
	if (dup2(info.fd[0], STDIN_FILENO) == -1
		|| dup2(fileout, STDOUT_FILENO) == -1)
	{
		if (close(fileout) == -1 || close(info.fd[0]) == -1
			|| close(info.fd[1]) == -1)
			ft_perror(FAIL_CLOSE_FD);
		ft_perror(FAIL_PARENT);
	}
	if (close(fileout) == -1 || close(info.fd[0]) == -1
		|| close(info.fd[1]) == -1)
		ft_perror(FAIL_CLOSE_FD);
	execute_command(argv[i - 2], envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_info	info;

	if (argc < 5)
		ft_perror(USAGE);
	set_info(&info);
	if (pipe(info.fd) == -1)
		ft_perror(FAIL_PIPE);
	info.pid = fork();
	if (info.pid == -1)
		ft_perror(FAIL_FORK);
	child_process(argv, envp, info);
	check_process(argv, envp, &info, argc);
	parent_process(argv, envp, info);
	return (0);
}
