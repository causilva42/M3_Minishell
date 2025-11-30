/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmdtree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myivanov <myivanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:38:41 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/11/30 16:24:29 by myivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_line.h"
#include "../minishell.h"

int	is_redirection_token(char *str);

void	append_token_list(t_list **dst, t_list *src)
{
	t_list	*tmp;

	if (!dst || !src)
		return ;
	if (!*dst)
	{
		*dst = src;
		return ;
	}
	tmp = *dst;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = src;
}

int	is_line_complete(t_list *tokens)
{
	t_list	*last;
	t_token	*tk;

	if (!tokens)
		return (1);
	last = tokens;
	while (last->next)
		last = last->next;
	tk = (t_token *)last->content;
	if (!tk || !tk->str)
		return (1);
	if (is_operator_str(tk->str) || ft_strcmp(tk->str, "(") == 0)
		return (0);
	if (!parens_are_fully_closed(tokens))
		return (0);
	return (1);
}

int	add_heredocs(int **heredocs, int heredoc)
{
	int	*new_heredocs;
	int	len;
	int	i;

	len = 0;
	while((*heredocs)[len] != -1)
		len++;
	new_heredocs = malloc((len + 2) * sizeof(int));
	if (!new_heredocs)
		return (-1);
	i = -1;
	while (++i < len)
		new_heredocs[i] = (*heredocs)[i];
	new_heredocs[i] = heredoc;
	new_heredocs[i + 1] = -1;
	free(*heredocs);
	*heredocs = new_heredocs;
	return (0);
}

int 	parse_heredocs(t_list *tokens, t_vars *vars)
{
	t_token *tk;
	int	res;

	while (tokens)
	{
		tk = tokens->content;
		if (tk->control == 6)
		{
			res = handle_heredoc(tokens);
			if (!res)
				return (-1);
			tokens = tokens->next;
			tk = tokens->content;
			tk->fd_heredoc = res;
			if (add_heredocs(&vars->heredocs, res) == -1)
				return (-1);
		}
		tokens = tokens->next;
	}
	return (0);
}

t_list	*read_and_accumulate_tokens(t_vars *vars)
{
	t_list	*all;
	char	*full;
	t_list	*tokens;
	char	*line;

	all = NULL;
	full = ft_strdup("");
	if (!full)
		return (NULL);
	line = readline(">> ");
	while (line)
	{
		if (handle_line_init(&all, &full, line) == -1)
			return (ft_lstclear(&all, free_token), NULL);
		tokens = tokenize_and_append(&all, line);
		if (!tokens)
			return (free(full), NULL);
		if (process_syntax_checks(&all) == -1)
			return (free(full), NULL);
		if (is_line_complete(all))
			break ;
		full = ft_strjoin(full, " ");
		line = readline("> ");
	}
	if (parse_heredocs(all, vars) == -1)
		return (free(full), NULL);
	ctrld_intercepted(line, &all, &full);
	return (add_hyst_return(&full, all));
}

t_cmdtree	*get_cmdtree(t_vars *vars)
{
	t_list	*all_tokens;

	all_tokens = read_and_accumulate_tokens(vars);
	if (!all_tokens)
		return (NULL);
	vars->cmdtree = parse_line(all_tokens);
	//ft_lstclear(&all_tokens, free_token);
	return (vars->cmdtree);
}
