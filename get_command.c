/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:16:03 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/18 15:37:33 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	count_words(char *argv)
{
	int	count;

	count = 0;
	while (*argv)
	{
		if (!ft_is_space_tab(*argv))
		{
			count++;
			if (*argv == 39)
			{
				argv++;
				while (*argv && *argv != 39)
					argv++;
				if (*argv)
					argv++;
			}
			else
			{
				//
				argv++;
			}
		}
		//
		argv++;
	}
}

char	**get_command(char *argv)
{
	char	**command;
	int		count;

	if (!argv)
		return (NULL);
	if (!ft_strchr(argv, 39))
	{
		command = ft_split(argv, ' ');
		return (command);
	}
	count = count_words(argv);
	//
}

/*test*/
//tr ' ' '\n'
//cut -d' ' -f2
/*end*/