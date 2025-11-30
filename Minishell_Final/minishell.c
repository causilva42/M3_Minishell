/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 13:36:06 by causilva          #+#    #+#             */
/*   Updated: 2025/11/29 17:52:25 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	init_vars(t_vars *vars, char **envp);
void		memory_cleanup(t_vars vars);
void		free_cmdtree(t_cmdtree *cmdtree);
void		io_cleanup(t_vars vars);
void		io_cleanup_cmdtree(t_cmdtree *cmdtree);

int	main(int argc, char **argv, char **envp)
{
	t_vars	vars;

	if (argc != 1)
		return (1);
	if (init_vars(&vars, envp) == -1)
		return (1);
	while (1)
	{
		vars.cmdtree = get_cmdtree(vars);
		if (vars.cmdtree)
			exec_cmdtree(vars.cmdtree, &vars);
		free_cmdtree(vars.cmdtree);
		vars.cmdtree = NULL;
	}
	return (0);
}

static int	init_vars(t_vars *vars, char **envp)
{
	int	i;

	vars->cmdtree = NULL;
	vars->envp = malloc(1 * sizeof(char *));
	if (!vars->envp)
		return (-1);
	vars->envp[0] = NULL;
	i = 0;
	while (envp[i])
		add_envp(&vars->envp, envp[i++]);
	vars->heredocs = malloc(1 * sizeof(int));
	if (!vars->heredocs)
		return (free(vars->envp), -1);
	vars->heredocs[0] = -1;
	vars->last_exit_status = 0;
	return (0);
}

void	memory_cleanup(t_vars vars)
{
	int	i;

	free_cmdtree(vars.cmdtree);
	i = 0;
	if (vars.envp)
		while (vars.envp[i])
			free(vars.envp[i++]);
	free(vars.envp);
	free(vars.heredocs);
	rl_clear_history();
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

void	io_cleanup(t_vars vars)
{
	int	i;

	i = 0;
	while (vars.heredocs[i] != -1)
	{
		close(vars.heredocs[i]);
		vars.heredocs[i] = -1;
		i++;
	}
	io_cleanup_cmdtree(vars.cmdtree);
}

void	io_cleanup_cmdtree(t_cmdtree *cmdtree)
{
	if (!cmdtree)
		return ;
	io_cleanup_cmdtree(cmdtree->child1);
	io_cleanup_cmdtree(cmdtree->child2);
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
