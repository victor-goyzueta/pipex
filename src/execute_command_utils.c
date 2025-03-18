/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 20:04:14 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/18 19:09:33 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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
