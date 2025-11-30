/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paren_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:31:25 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/11/29 18:00:13 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_paren_level(char *str, int *level)
{
	if (ft_strcmp(str, "(") == 0)
		(*level)++;
	else if (ft_strcmp(str, ")") == 0)
		(*level)--;
}

int	has_outer_parens(t_list *tokens)
{
	t_list	*node;
	t_token	*tok;
	int		depth;

	node = tokens;
	tok = (t_token *)node->content;
	if (!tok || ft_strcmp(tok->str, "(") != 0)
		return (0);
	depth = 0;
	while (node)
	{
		tok = (t_token *)node->content;
		if (ft_strcmp(tok->str, "(") == 0)
			depth++;
		else if (ft_strcmp(tok->str, ")") == 0)
			depth--;
		if (depth == 0 && node->next)
			return (0);
		node = node->next;
	}
	return (depth == 0);
}

int	has_double_outer_parens(t_list *tokens)
{
	t_token	*tok1;
	t_token	*tok2;
	t_list	*node;
	t_list	*before_last;

	if (!tokens || !tokens->next || !tokens->next->next)
		return (0);
	tok1 = (t_token *)tokens->content;
	tok2 = (t_token *)tokens->next->content;
	if (!tok1 || !tok2 || ft_strcmp(tok1->str, "(") != 0 
		|| ft_strcmp(tok2->str, "(") != 0)
		return (0);
	node = tokens;
	while (node && node->next && node->next->next)
		node = node->next;
	before_last = node;
	if (!before_last || !before_last->next)
		return (0);
	tok1 = (t_token *)before_last->content;
	tok2 = (t_token *)before_last->next->content;
	if (!tok1 || !tok2 || ft_strcmp(tok1->str, ")") != 0
		|| ft_strcmp(tok2->str, ")") != 0)
		return (0);
	return (1);
}

t_list	*dup_inner_tokens(t_list *tokens)
{
	t_list	*new;
	t_list	*node;
	t_token	*dup;
	t_list	*node_new;

	new = NULL;
	node = tokens->next;
	while (node && node->next)
	{
		dup = dup_token((t_token *)node->content);
		if (!dup)
			return (ft_lstclear(&new, free_token), NULL);
		node_new = ft_lstnew(dup);
		if (!node_new)
		{
			free_token(dup);
			ft_lstclear(&new, free_token);
			return (NULL);
		}
		ft_lstadd_back(&new, node_new);
		node = node->next;
	}
	return (new);
}

t_list	*ft_lstremove_outer_parens(t_list *tokens)
{
	t_list	*new;

	if (!tokens || !tokens->next)
		return (tokens);
	if (!has_outer_parens(tokens) || has_double_outer_parens(tokens))
		return (tokens);
	new = dup_inner_tokens(tokens);
	ft_lstclear(&tokens, free_token);
	return (new);
}