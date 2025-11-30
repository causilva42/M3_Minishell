/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 14:32:04 by causilva          #+#    #+#             */
/*   Updated: 2025/11/26 13:03:33 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int			init_vars(t_vars *vars);
void				free_vars(t_vars vars);
void				print_cmdtree(t_cmdtree *cmdtree);
static t_cmdtree	*mini_get_cmdtree(char *str);
static int			fill_heredoc(t_cmdtree *cmdtree);
void				free_cmdtree(t_cmdtree *cmdtree);
void				close_heredocs(t_list *tokens, int jump_parens);
t_list				*jump_parenthesis(t_list *node);

int	main(int argc, char **argv)
{
	t_vars		vars;
	t_cmdtree	*cmdtree;
	int			func_return;

	if (init_vars(&vars) == -1)
		return (1);
	if (argc != 2)
		return (1);
	cmdtree = mini_get_cmdtree(argv[1]);
	if (!cmdtree)
		return (1);
	print_cmdtree(cmdtree);
	func_return = expand(&cmdtree->tokens);
	if (func_return == -1)
		return (free_cmdtree(cmdtree), 1);
	print_cmdtree(cmdtree);
	func_return = redirect(cmdtree);
	if (func_return == -1)
		return (free_cmdtree(cmdtree), 1);
	print_cmdtree(cmdtree);
	free_cmdtree(cmdtree);
	free_vars(vars);
	return (0);
}

static int	init_vars(t_vars *vars)
{
	vars->envp = malloc(1 * sizeof(char *));
	if (!vars->envp)
		return (-1);
	vars->envp[0] = NULL;
	vars->heredocs = malloc(1 * sizeof(int));
	if (!vars->heredocs)
		return (free(vars->envp), -1);
	vars->heredocs[0] = 0;
	vars->last_exit_status = 0;
	return (0);
}

void	free_vars(t_vars vars)
{
	int	i;
	
	i = 0;
	if (vars.envp)
		while (vars.envp[i])
			free(vars.envp[i++]);
	free(vars.envp);
	i = 0;
	if (vars.heredocs)
		while (vars.heredocs[i])
			close(vars.heredocs[i++]);
	free(vars.heredocs);
}

void	print_cmdtree(t_cmdtree *cmdtree)
{
	t_list	*token_node;
	t_token	*token;

	printf("\n---------------------\n");
	printf("CmdTree:\n");
	printf("Tokens:");
	token_node = cmdtree->tokens;
	while (token_node)
	{
		token = token_node->content;
		printf("(%d)(%d)(%s)->", token->control, token->fd_heredoc, token->str);
		token_node = token_node->next;
	}
	printf("(NULL)\n");
	printf("Fd_input: %d\n", cmdtree->fd_input);
	printf("Fd_output: %d\n", cmdtree->fd_output);
	printf("Is_local_fd_input: %d\n", cmdtree->is_local_fd_input);
	printf("Is_local_fd_output: %d\n", cmdtree->is_local_fd_output);
	printf("---------------------\n");
}

static t_cmdtree	*mini_get_cmdtree(char *str)
{
	t_cmdtree	*cmdtree;

	cmdtree = malloc(sizeof(t_cmdtree));
	cmdtree->tokens = NULL;
	cmdtree->type = 4;
	cmdtree->fd_input = 0;
	cmdtree->fd_pipe = 0;
	cmdtree->fd_output = 1;
	cmdtree->is_local_fd_input = 0;
	cmdtree->is_local_fd_output = 0;
	cmdtree->exit_status = 0;
	cmdtree->exec_status = 0;
	cmdtree->parent = NULL;
	cmdtree->child1 = NULL;
	cmdtree->child2 = NULL;
	errno = 0;
	cmdtree->tokens = tokenize(str);
	if (!cmdtree->tokens && errno)
		return (free_cmdtree(cmdtree), NULL);
	if (fill_heredoc(cmdtree) == -1)
		return (free_cmdtree(cmdtree), NULL);
	return (cmdtree);
}

static int	fill_heredoc(t_cmdtree *cmdtree)
{
	t_list		*node;
	t_token		*token;

	node = cmdtree->tokens;
	while (node)
	{
		token = node->content;
		if (token->control == 6)
		{
			node = node->next;
			token = node->content;
			token->fd_heredoc = open("../notas", O_RDONLY);
			if (token->fd_heredoc < 0)
				return (token->fd_heredoc = 0, -1);
		}
		node = node->next;
	}
	return (0);
}

void	free_cmdtree(t_cmdtree *cmdtree)
{
	if (!cmdtree)
		return ;
	free_cmdtree(cmdtree->child1);
	free_cmdtree(cmdtree->child2);
	if (cmdtree->type == 0 || cmdtree->type == 4)
		close_heredocs(cmdtree->tokens, (cmdtree->type == 0));
	ft_lstclear(&cmdtree->tokens, free_token);
	if (cmdtree->fd_input != 0 && cmdtree->is_local_fd_input)
		close(cmdtree->fd_input);
	if (cmdtree->fd_output != 1 && cmdtree->is_local_fd_output)
		close(cmdtree->fd_output);
	if (cmdtree->fd_pipe)
		close(cmdtree->fd_pipe);
	free(cmdtree);
}

void	close_heredocs(t_list *tokens, int jump_parens)
{
	t_list		*node;
	t_token		*token;

	node = tokens;
	if (jump_parens)
		node = jump_parenthesis(node);
	while (node)
	{
		token = node->content;
		if (token->fd_heredoc)
		{
			close(token->fd_heredoc);
			token->fd_heredoc = 0;
		}
		node = node->next;
	}
}

t_list	*jump_parenthesis(t_list *node)
{
	t_token	*token;
	int		count;

	if (!node)
		return (NULL);
	token = node->content;
	while (token->control != 1)
	{
		node = node->next;
		if (!node)
			return (NULL);
		token = node->content;
	}
	count = 1;
	while (count > 0)
	{
		node = node->next;
		if (!node)
			return (NULL);
		token = node->content;
		count += (token->control == 1) - (token->control == 2);
	}
	return (node);
}
