/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmdtree.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myivanov <myivanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 14:16:42 by causilva          #+#    #+#             */
/*   Updated: 2025/11/30 14:36:30 by myivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmdtree.h"
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

/*static void	fill_io(t_cmdtree *cmdtree)
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
}*/

static void fill_io(t_cmdtree *cmd)
{
    cmd->is_local_fd_input = 0;
    cmd->is_local_fd_output = 0;

    // defaults
    cmd->fd_input = STDIN_FILENO;
    cmd->fd_output = STDOUT_FILENO;
    cmd->fd_pipe = -1;

    if (!cmd->parent)
        return;

    // inherit normally
    cmd->fd_input = cmd->parent->fd_input;
    cmd->fd_output = cmd->parent->fd_output;

    // pipe rules
    if (cmd->parent->type == 3)          // PIPE
    {
        if (cmd == cmd->parent->child1)  // left side writes
        {
            cmd->fd_output = cmd->parent->fd_pipe;
            cmd->is_local_fd_output = 0; // NEVER close pipe inherited
        }
        if (cmd == cmd->parent->child2)  // right side reads
        {
            cmd->fd_input = cmd->parent->fd_pipe;
            cmd->is_local_fd_input = 0;  // NEVER close inherited
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
