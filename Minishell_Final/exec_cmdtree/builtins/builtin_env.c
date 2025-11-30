/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 15:13:27 by causilva          #+#    #+#             */
/*   Updated: 2025/11/29 17:32:52 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	builtin_env(char **envp, int fd_output)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		write(fd_output, envp[i], ft_strlen(envp[i]));
		write(fd_output, "\n", 1);
		i++;
	}
	return (0);
}

int	special_env(char **envp, int fd_output)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		write(fd_output, "declare -x ", 11);
		write(fd_output, envp[i], ft_strlen(envp[i]));
		write(fd_output, "\n", 1);
		i++;
	}
	return (0);
}
