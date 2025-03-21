/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 19:01:03 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/18 19:07:33 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	count_words(char *argv)
{
	int	count;

	count = 0;
	while (*argv)
	{
		if (*argv == 39)
		{
			count++;
			argv++;
			while(*argv && *argv != 39)
				argv++;
			argv++;
		}
		else if (!ft_is_space_tab(*argv))
		{
			count++;
			while(*argv && (!ft_is_space_tab(*argv) && *argv != 39))
				argv++;
		}
		else
			argv++;
	}
	return (count);
}

static char	*allocate1(char *argv, char **command, int *word)
{
	int	len;

	len = 0;
	while(argv[len] && argv[len] != 39)
			len++;
	command[*word] = ft_substr(argv, 0, len + 1);
	if (!command[*word])
	{
		free_array(command);
		return (NULL);
	}
	*word = *word + 1;
	return (argv += len);
}

static char	*allocate2(char *argv, char **command, int *word)
{
	int	len;

	len = 0;
	while(argv[len] && (!ft_is_space_tab(argv[len]) && argv[len] != 39))
		len++;
	command[*word] = ft_substr(argv, 0, len + 1);
	if (!command[*word])
	{
		free_array(command);
		return (NULL);
	}
	*word = *word + 1;
	return (argv += len);
}

char	**get_command(char *argv)
{
	char	**command;
	int		count;
	int		word;

	if (!argv)
		return (NULL);
	count = count_words(argv);
	command = ft_calloc(count + 1, sizeof(char *));
	if (!command)
		return (NULL);
	command[count] = NULL;
	word = 0;
	while(*argv && word < count)
	{
		if (*argv == 39)
		{
			argv++;
			argv = allocate1(argv, command, &word);
		}
		else if (!ft_is_space_tab(*argv))
			argv = allocate2(argv, command, &word);
		else
			argv++;
	}
	return (command);
}
