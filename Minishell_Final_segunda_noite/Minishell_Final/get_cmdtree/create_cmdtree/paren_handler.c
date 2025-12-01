/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paren_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:31:25 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/12/01 20:18:02 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../parse_line.h"

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

int double_paren_sequence(t_list *node)
{
    if (!node || !node->next)
        return (-1);
    if (ft_strcmp(((t_token*)node->content)->str, "(") != 0 ||
        ft_strcmp(((t_token*)node->next->content)->str, "(") != 0)
        return (-1);
    return (1);
}

int has_double_outer_parens(t_list *tokens)
{
    t_list *node;
    int paren_level;
	t_token *tok;

	node = tokens;
	paren_level = 0;
    if (!node || !node->next)
        return (0);
    if (double_paren_sequence(node) == -1)
		return (0);
    node = node->next->next;
    paren_level = 2;
    while (node)
    {
        tok = node->content;
		update_paren_level(tok->str, &paren_level);
        if (paren_level == 0)
        {
            if (node->next != NULL)
                return (0);
            return (1);
        }
        node = node->next;
    }
    return (0);
}


t_list *dup_inner_tokens(t_list *tokens)
{
    t_list *new;
    t_list *node;
    t_token *tok;
    int paren;

	new = NULL;
	node = tokens->next;
	paren = 1;
    while (node)
    {
        tok = node->content;

        if (tok->str[0] == '(')
            paren++;
        else if (tok->str[0] == ')')
            paren--;
        if (paren == 0)
            break;
        t_token *dup = dup_token(tok);
        ft_lstadd_back(&new, ft_lstnew(dup));
        node = node->next;
    }
    return new;
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