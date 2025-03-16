/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 17:32:54 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/16 22:22:34 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	middle_process(char **argv, char **envp, t_info info, int i)
{
	//int		filemid;
	(void)argv, (void)envp, (void)i;

	if (info.pid != 0 || info.pid_tmp != 0)
		return ;
	// if (waitpid(info.pid_tmp, NULL, 0) == -1)
	// 	ft_perror(FAIL_WAIT);
	// if (dup2(info.fd_tmp[1], STDOUT_FILENO) == -1
	// 	|| dup2(filemid, STDIN_FILENO) == -1)
	// {
	// 	if (close(filemid) == -1 || close(info.fd_tmp[0]) == -1
	// 		|| close(info.fd_tmp[1]) == -1)
	// 		ft_perror(FAIL_CLOSE_FD);
	// 	ft_perror(FAIL_CHILD);
	// }
	// if (close(filemid) == - 1 || close(info.fd_tmp[0]) == -1
	// 	|| close(info.fd_tmp[1]) == -1)
	// 	ft_perror(FAIL_CLOSE_FD);
	// execute_command(argv[i], envp);
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
	execute_command(argv[i - 2], envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_info	info;

	if (argc < 5)
		ft_perror(USAGE);
	ft_memset(&info, 0, sizeof(t_info));
	if (pipe(info.fd) == -1)
		ft_perror(FAIL_PIPE);
	info.pid = fork();
	if (info.pid == -1)
		ft_perror(FAIL_FORK);
	/*test*/
	if (info.pid != 0)
		printf("Parent PID: %d\n", getpid());
	else
		printf("Child PID: %d\n", getpid());
	/*end*/
	child_process(argv, envp, info);
	info.i = 2;
	while (--argc >= 5 && info.pid_tmp == 0)
	{
		printf("In loop\n");
		set_info(&info);
		if (pipe(info.fd_tmp) == -1)
			ft_perror(FAIL_PIPE);
		info.pid_tmp = fork();
		if (info.pid_tmp == -1)
			ft_perror(FAIL_FORK);
		/*test*/
		if (info.pid_tmp == 0)
		{
			printf("In\n");
			printf("PID: %d\n", getpid());
		}
		/*end*/
		middle_process(argv, envp, info, info.i);
	}
	parent_process(argv, envp, info);
	return (0);
}
