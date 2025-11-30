/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmdtree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 14:16:42 by causilva          #+#    #+#             */
/*   Updated: 2025/11/29 18:32:58 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	fill_io(t_cmdtree *cmdtree);
static void	close_io(t_cmdtree *cmdtree);

int	exec_cmdtree(t_cmdtree *cmdtree, t_vars *vars)
{
	if (!cmdtree)
		return (127);
	if (expand(&cmdtree->tokens, vars) == -1)
		return (127);
	fill_io(cmdtree);
	if (cmdtree->type == 0)
		cmdtree->exit_status = exec_parentheses(cmdtree, vars);
	else if (cmdtree->type == 1)
		cmdtree->exit_status = exec_and(cmdtree, vars);
	else if (cmdtree->type == 2)
		cmdtree->exit_status = exec_or(cmdtree, vars);
	else if (cmdtree->type == 3)
		cmdtree->exit_status = exec_pipe(cmdtree, vars);
	else if (cmdtree->type == 4)
		cmdtree->exit_status = exec_cmd(cmdtree, vars);
	close_io(cmdtree);
	vars->last_exit_status = cmdtree->exit_status;
	return (cmdtree->exit_status);
}

static void	fill_io(t_cmdtree *cmdtree)
{
	cmdtree->fd_input = STDIN_FILENO;
	cmdtree->fd_pipe = -1;
	cmdtree->fd_output = STDOUT_FILENO;
	if (cmdtree->parent)
	{
		cmdtree->fd_input = cmdtree->parent->fd_input;
		cmdtree->fd_output = cmdtree->parent->fd_output;
		if (cmdtree->parent->type == 3)
		{
			if (cmdtree == cmdtree->parent->child1)
				cmdtree->fd_output = cmdtree->parent->fd_pipe;
			if (cmdtree == cmdtree->parent->child2)
				cmdtree->fd_input = cmdtree->parent->fd_pipe;
		}
	}
}

static void	close_io(t_cmdtree *cmdtree)
{
	if (cmdtree->is_local_fd_input && cmdtree->fd_input != -1)
		close(cmdtree->fd_input);
	if (cmdtree->fd_pipe != -1)
		close(cmdtree->fd_pipe);
	if (cmdtree->is_local_fd_output && cmdtree->fd_output != -1)
		close(cmdtree->fd_output);
	cmdtree->fd_input = -1;
	cmdtree->fd_pipe = -1;
	cmdtree->fd_output = -1;
	cmdtree->is_local_fd_input = 0;
	cmdtree->is_local_fd_output = 0;
}
