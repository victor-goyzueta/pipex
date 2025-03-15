/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vgoyzuet <vgoyzuet@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 17:27:08 by vgoyzuet          #+#    #+#             */
/*   Updated: 2025/03/15 19:56:57 by vgoyzuet         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft.h"
# include <sys/types.h>
# include <sys/wait.h>

# define USAGE			"./pipex file1 command1 command2 file2"
# define FAIL_PIPE		"Pipe function return error"
# define FAIL_FORK		"Fork function return error"
# define FAIL_WAIT		"Waitpid function return error"
# define FAIL_OPEN_FD	"Failed to open fd"
# define FAIL_CLOSE_FD	"Failed to close fd"
# define FAIL_CHILD		"Failure to redirect flows in child process"
# define FAIL_PARENT	"Failure to redirect flows in parent process"
# define FAIL_ALLOC		"Failed memory allocation"

#endif
