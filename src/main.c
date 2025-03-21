/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 19:46:56 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/21 17:04:57 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	child_process(char **argv, char **envp, t_info info)
{
	if (pipe(info.fd) == -1)
		ft_perror(FAIL_PIPE);
	info.pid = fork();
	if (info.pid == -1)
		ft_perror(FAIL_FORK);
	if (info.pid != 0)
		return ;
	info.infile = open(argv[1], O_RDONLY);
	if (info.infile == -1)
		ft_perror(FAIL_OPEN_FD);
	if (dup2(info.fd[1], STDOUT_FILENO) == -1
		|| dup2(info.infile, STDIN_FILENO) == -1)
	{
		if (close(info.infile) == -1 || close(info.fd[0]) == -1
			|| close(info.fd[1]) == -1)
			ft_perror(FAIL_CLOSE_FD);
		ft_perror(FAIL_CHILD);
	}
	if (close(info.infile) == -1 || close(info.fd[0]) == -1
		|| close(info.fd[1]) == -1)
		ft_perror(FAIL_CLOSE_FD);
	execute_command(argv[2], envp);
	exit(EXIT_SUCCESS);
}

static void	here_doc_process(char **argv, char **envp, t_info info)
{
	char	*here_doc;

	if (pipe(info.fd) == -1)
		ft_perror(FAIL_PIPE);
	info.pid = fork();
	if (info.pid == -1)
		ft_perror(FAIL_FORK);
	if (info.pid != 0)
		return ;
	info.limiter = ft_strdup(argv[2]);
	if (!info.limiter)
		ft_perror(FAIL_ALLOC);
	put_here_doc(argv, info);
	execute_command(argv[3], envp);
	exit(EXIT_SUCCESS);
}

static void pipex(int argc, char **argv, char **envp, t_info *info)
{
	check_here_doc(argv, envp, info);
	while (info->i < argc - 2)
	{
		if (pipe(info->fd) == -1)
			ft_perror(FAIL_PIPE);
		info->pid = fork();
		if (info->pid == -1)
			ft_perror(FAIL_FORK);
		if (info->pid == 0)
		{
			if (info->i == 2)
			{
				if (dup2(info->infile, STDIN_FILENO) == -1)
					ft_perror(FAIL_FLOW);
				if (close(info->infile) == -1)
					ft_perror(FAIL_CLOSE_FD);
			}
			else
			{
				if (dup2(info->pre_fd, STDIN_FILENO) == -1)
					ft_perror(FAIL_FLOW);
			}
			if (dup2(info->fd[1], STDOUT_FILENO) == -1)
				ft_perror(FAIL_FLOW);
			if (close(info->fd[0]) == -1 || close(info->fd[1]) == -1)
				ft_perror(FAIL_CLOSE_FD);
			execute_command(argv[info->i], envp);
		}
		else
		{
			if (close(info->fd[1]) == -1)
				ft_perror(FAIL_CLOSE_FD);
			if (info->i > 2)
			{
				if (close(info->pre_fd) == -1)
					ft_perror(FAIL_CLOSE_FD);
				info->pre_fd = info->fd[0];
			}
		}
		info->i++;
	}
	info->pid = fork();
	if (info->pid == -1)
		ft_perror(FAIL_FORK);
	if (info->pid == 0)
	{
		if (dup2(info->pre_fd, STDIN_FILENO) == -1)
			ft_perror(FAIL_FLOW);
		if (close(info->pre_fd) == -1)
			ft_perror(FAIL_CLOSE_FD);
		info->outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (info->outfile == -1)
			ft_perror(FAIL_OPEN_FD);
		if (dup2(info->outfile, STDOUT_FILENO) == -1)
			ft_perror(FAIL_FLOW);
		if (close(info->outfile) == -1)
			ft_perror(FAIL_CLOSE_FD);
		execute_command(argv[argc - 2], envp);
	}
	else
	{
		if (close(info->pre_fd) == -1)
			ft_perror(FAIL_CLOSE_FD);
		wait(NULL);
	}
}

static void	check_here_doc(char **argv, char **envp, t_info *info)
{
	if (ft_strncmp(argv[1], "here_doc", 8) != 0)
	{
		info->i = 2;
		child_process(argv, envp, *info);
	}
	else
	{
		info->i = 3;
		here_doc_process(argv, envp, *info);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_info	info;

	if (argc < 5)
		ft_perror(USAGE);
	set_info(&info);
	pipex(argc, argv, envp, &info);
	return (0);
}
