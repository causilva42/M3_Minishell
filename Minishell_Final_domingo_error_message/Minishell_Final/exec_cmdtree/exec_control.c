/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myivanov <myivanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:10:47 by causilva          #+#    #+#             */
/*   Updated: 2025/11/30 14:38:58 by myivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmdtree.h"
#include "../minishell.h"

int	exec_parentheses(t_cmdtree *cmdtree, t_vars *vars)
{
	if (redirect(cmdtree, vars) == -1)
		return (127);
	return (exec_cmdtree(cmdtree->child1, vars));
}

int	exec_and(t_cmdtree *cmdtree, t_vars *vars)
{
	int	temp;

	temp = exec_cmdtree(cmdtree->child1, vars);
	if (temp == 0)
		temp = exec_cmdtree(cmdtree->child2, vars);
	return (temp);
}

int	exec_or(t_cmdtree *cmdtree, t_vars *vars)
{
	int	temp;

	temp = exec_cmdtree(cmdtree->child1, vars);
	if (temp != 0)
		temp = exec_cmdtree(cmdtree->child2, vars);
	return (temp);
}
