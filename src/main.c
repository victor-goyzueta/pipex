/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:32:54 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/16 17:22:10 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	middle_process(char **argv, char **envp, t_info info, int argc)
{
	(void)argv, (void)envp, (void)info, (void)argc;
}

static void	child_process(char **argv, char **envp, t_info info)
{
	int	filein;

	if (info.pid != 0)
		return ;
	filein = open(argv[1], O_RDONLY);
	if (filein == -1)
		ft_perror(FAIL_OPEN_FD);
	if (dup2(info.fd[1], STDOUT_FILENO) == -1 || dup2(filein, STDIN_FILENO) == -1)
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
}

static void	parent_process(char **argv, char **envp, t_info info)
{
	int	fileout;
	int	argc;

	if (info.pid == 0)
		return ;
	if (waitpid(info.pid, NULL, 0) == -1)
		ft_perror(FAIL_WAIT);
	argc = 0;
	while (argv[argc])
		argc++;	
	fileout = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fileout == -1)
		ft_perror(FAIL_OPEN_FD);
	if (dup2(info.fd[0], STDIN_FILENO) == -1 || dup2(fileout, STDOUT_FILENO) == -1)
	{
		if (close(fileout) == -1 || close(info.fd[0]) == -1
			|| close(info.fd[1]) == -1)
			ft_perror(FAIL_CLOSE_FD);
		ft_perror(FAIL_PARENT);
	}
	if (close(fileout) == -1 || close(info.fd[0]) == -1
		|| close(info.fd[1]) == -1)
		ft_perror(FAIL_CLOSE_FD);
	execute_command(argv[argc - 2], envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_info	info;
	// int		fd[2];
	// pid_t	pid;

	if (argc < 5)
		ft_perror(USAGE);
	if (pipe(info.fd) == -1)
		ft_perror(FAIL_PIPE);
	info.pid = fork();
	if (info.pid == -1)
		ft_perror(FAIL_FORK);
	child_process(argv, envp, info);
	while (--argc >= 5)
		middle_process(argv, envp, info, argc);
	parent_process(argv, envp, info);
	return (0);
}
