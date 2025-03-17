/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:04:14 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/17 21:12:06 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**get_command(char *argv)
{
	char	**command;
	size_t	len;
	size_t	word;

	if (!argv)
		return (NULL);
	while (*argv && ft_is_space_tab(*argv))
		argv++;
	command = NULL;
	len = 0;
	while (argv[len] && (!ft_is_space_tab(argv[len]) && argv[len] != 39))
		len++;
	word = 0;
	command[word] = ft_substr(argv, 0, len + 1);
	if (!command[word])
		return (NULL); //free?
	while (*argv)
	{
		len = 0;
		while(*argv && ft_is_space_tab(*argv))
			argv++;
		if (*argv && *argv == 39)
		{
			command[++word] = ft_substr(argv, 1, 2);
			argv += 2;
		}
		else if (*argv && *argv != 39)
		{
			while (argv[len] && (!ft_is_space_tab(argv[len]) && argv[len] != 39))
				len++;
			command[++word] = ft_substr(argv, 0, len + 1);
			if (!command[word])
				return (NULL); //free
		}
	}
	command[++word] = NULL;
	command = ft_calloc(word + 1, sizeof(char *));
	if (!command)
		return (NULL);
	command = &command[0];
	return(command);
}

void	find_path(char *command, char **envp, char **pathname)
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
		ft_exit_free(EXIT_FAILURE, FAIL_ALLOC, directories);
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
