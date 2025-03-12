/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 00:34:15 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/04 22:42:32 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_array(char **arr)
{
	int	i;

	if (!arr || !*arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_list(t_list **stack)
{
	t_list	*tmp;

	tmp = NULL;
	if (!stack || !*stack)
		return ;
	while (*stack)
	{
		tmp = (*stack)->next;
		free((*stack)->content);
		free(*stack);
		*stack = tmp;
	}
}

static void	free_all(t_list **stack, char **arr)
{
	free_list(stack);
	free_array(arr);
}

void	ft_exit_free(int EXIT, t_list **stack, char *message, char **arr)
{
	free_all(stack, arr);
	if (EXIT == EXIT_FAILURE)
		ft_perror(message);
	else
	{
		ft_printf(message);
		exit(EXIT);
	}
}
