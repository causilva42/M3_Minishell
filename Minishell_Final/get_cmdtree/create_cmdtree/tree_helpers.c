/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:31:25 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/11/29 17:59:22 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdtree	*cmdtree_new_node(t_list *tokens)
{
	t_cmdtree	*node;

	if (!tokens)
		return (NULL);
	node = malloc(sizeof(t_cmdtree));
	if (!node)
		return (NULL);
	node->tokens = tokens;
	node->type = -1;
	node->fd_input = -1;
	node->fd_output = -1;
	node->fd_pipe = -1;
	node->exit_status = 0;
	node->parent = NULL;
	node->child1 = NULL;
	node->child2 = NULL;
	return (node);
}

void	free_cmdtree(t_cmdtree *cmdtree)
{
	if (!cmdtree)
		return ;
	free_cmdtree(cmdtree->child1);
	free_cmdtree(cmdtree->child2);
	ft_lstclear(&cmdtree->tokens, free_token);
	free(cmdtree);
}

int	process_leaf_node(t_cmdtree *cmd, t_vars *vars)
{
	int	result;

	result = exec_cmd(cmd, vars);
	if (!result)
		return (-1);
	return (0);
}
