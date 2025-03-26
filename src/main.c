/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:46:56 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/26 19:22:53 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	here_doc_process(char **argv, char **envp, t_info *info)
{
	int fd[2];
	
	if (info->pid != 0 || ft_strncmp(argv[1], "here_doc", 8) != 0)
		return ;
	if (pipe(fd) == -1)
		ft_perror(FAIL_PIPE);
	info->i = 3;
	info->limiter = ft_strdup(argv[2]);
	if (!info->limiter)
		ft_perror(FAIL_ALLOC);
	info->len = ft_strlen(info->limiter);
	put_here_doc(info, fd);
	if (dup2(fd[0], 0) == -1 || dup2(info->fd[1], 1) == -1)
	{	
		if (close(fd[0]) == -1 || close(info->fd[1]) == -1)
			ft_perror(FAIL_CLOSE_FD);
		ft_perror(FAIL_CHILD);
	}
	if (close(fd[0]) == -1)
		ft_perror(FAIL_CLOSE_FD);
	execute_command(argv[info->i], envp);
	exit(EXIT_SUCCESS);
}

static void	child_process(char **argv, char **envp, t_info *info)
{
	if (info->pid != 0 || ft_strncmp(argv[1], "here_doc", 8) == 0)
		return ;
	info->i = 2;
	info->infile = open(argv[1], O_RDONLY);
	if (info->infile == -1)
		ft_perror(FAIL_OPEN_FD);
	if (dup2(info->fd[1], STDOUT_FILENO) == -1
		|| dup2(info->infile, STDIN_FILENO) == -1)
	{
		if (close(info->fd[0]) == -1 || close(info->fd[1]) == -1
			|| close(info->infile) == -1)
			ft_perror(FAIL_CLOSE_FD);
		ft_perror(FAIL_CHILD);
	}
	if (close(info->fd[0]) == -1 || close(info->fd[1]) == -1
		|| close(info->infile) == -1)
		ft_perror(FAIL_CLOSE_FD);
	execute_command(argv[info->i], envp);
	exit(EXIT_SUCCESS);
}

void	check_process(char **argv, char **envp, t_info *info, int argc)
{
	if (argc < 6 || info->pid == 0 || ft_strncmp(argv[1], "here_doc", 8) == 0)
		return ;
	if (!info)
		ft_perror(FAIL_ALLOC);
	info->i = 2;
	info->pre_fd = info->fd[0];
	while (info->i < argc - 2)
	{
		set_info(info);
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

static void	parent_process(char **argv, char **envp, t_info *info)
{
	int	i;
	if (info->pid == 0)
		return ;
	i = 0;
	while (argv[i])
		i++;
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		info->outfile = open(argv[i - 1], O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		info->outfile = open(argv[i - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (info->outfile == -1)
		ft_perror(FAIL_OPEN_FD);
	if (dup2(info->fd[0], STDIN_FILENO) == -1
		|| dup2(info->outfile, STDOUT_FILENO) == -1)
	{
		if (close(info->fd[0]) == -1 || close(info->fd[1]) == -1
			|| close(info->outfile) == -1)
			ft_perror(FAIL_CLOSE_FD);
		ft_perror(FAIL_PARENT);
	}
	if (close(info->fd[0]) == -1 || close(info->fd[1]) == -1
		|| close(info->outfile) == -1)
		ft_perror(FAIL_CLOSE_FD);
	if (waitpid(info->pid, NULL, 0) == -1)
		ft_perror(FAIL_WAIT);
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
	here_doc_process(argv, envp, &info);
	child_process(argv, envp, &info);
	check_process(argv, envp, &info, argc);
	parent_process(argv, envp, &info);
	return (0);
}
