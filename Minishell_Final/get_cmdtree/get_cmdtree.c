/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmdtree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:38:41 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/11/29 17:53:40 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

t_list	*read_and_accumulate_tokens(void)
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
		line = readline("> ");
	}
	ctrlD_intecepted(line, &full, &all);
	return (add_hyst_return(&full, all));
}

t_vars	*get_cmdtree(t_vars *vars)
{
	t_list	*all_tokens;

	all_tokens = read_and_accumulate_tokens();
	if (!all_tokens)
		return (NULL);
	vars->cmdtree = parse_line(all_tokens);
	ft_lstclear(&all_tokens, free_token);
	return (vars->cmdtree);
}
