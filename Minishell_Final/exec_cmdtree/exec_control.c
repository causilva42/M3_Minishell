/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 16:10:47 by causilva          #+#    #+#             */
/*   Updated: 2025/11/29 13:51:24 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_parentheses(t_cmdtree *cmdtree, t_vars *vars)
{
	if (redirect(cmdtree, vars) == -1)
		return (127);
	return (exec_tree(cmdtree->child1, vars));
}

int	exec_and(t_cmdtree *cmdtree, t_vars *vars)
{
	int	temp;

	temp = exec_tree(cmdtree->child1, vars);
	if (temp == 0)
		temp = exec_tree(cmdtree->child2, vars);
	return (temp);
}

int	exec_or(t_cmdtree *cmdtree, t_vars *vars)
{
	int	temp;

	temp = exec_tree(cmdtree->child1, vars);
	if (temp != 0)
		temp = exec_tree(cmdtree->child2, vars);
	return (temp);
}
