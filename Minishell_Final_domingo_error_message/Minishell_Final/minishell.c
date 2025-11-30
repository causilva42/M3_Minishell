/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myivanov <myivanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 13:36:06 by causilva          #+#    #+#             */
/*   Updated: 2025/11/30 16:57:45 by myivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_vars(t_vars *vars, char **envp);

int	main(int argc, char **argv, char **envp)
{
	t_vars	vars;

	(void)argv;
	if (argc != 1)
		return (1);
	if (init_vars(&vars, envp) == -1)
		return (1);
	while (1)
	{
		vars.cmdtree = get_cmdtree(&vars);
		if (vars.cmdtree)
			exec_cmdtree(vars.cmdtree, &vars);
		free_cmdtree(vars.cmdtree);
		vars.cmdtree = NULL;
	}
	return (0);
}

static int	init_vars(t_vars *vars, char **envp)
{
	vars->cmdtree = NULL;
	vars->envp = malloc(1 * sizeof(char *));
	if (!vars->envp)
		return (-1);
	vars->envp[0] = NULL;
	builtin_export(envp, 1, vars);
	vars->heredocs = malloc(1 * sizeof(int));
	if (!vars->heredocs)
		return (free(vars->envp), -1);
	vars->heredocs[0] = -1;
	vars->last_exit_status = 0;
	return (0);
}

