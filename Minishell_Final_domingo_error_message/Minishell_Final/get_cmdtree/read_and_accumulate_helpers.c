/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_and_accumulate_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myivanov <myivanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:31:25 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/11/29 15:50:31 by myivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse_line.h"
#include "../minishell.h"

int	handle_line_init(t_list **all, char **full, char *line)
{
	char	*tmp;

	if (!*line && !*all)
		return (free(line), free(*full), -1);
	tmp = ft_strjoin(*full, line);
	if (!tmp)
		return (free(line), free(*full), -1);
	free(*full);
	*full = tmp;
	tmp = ft_strjoin(*full, "");
	if (!tmp)
		return (free(*full), free(line), -1);
	free(*full);
	*full = tmp;
	return (0);
}

t_list	*tokenize_and_append(t_list **all, char *line)
{
	t_list	*tokens;

	tokens = tokenize(line);
	free(line);
	if (!tokens)
	{
		ft_lstclear(all, free_token);
		return (NULL);
	}
	if (line_starts_with_op(tokens))
	{
		printf("minishell: syntax error near unexpected token `%s'\n",
			((t_token *)tokens->content)->str);
		ft_lstclear(&tokens, free_token);
		ft_lstclear(all, free_token);
		return (NULL);
	}
	append_token_list(all, tokens);
	return (tokens);
}

void	ctrld_intercepted(char *line, t_list **all, char **full)
{
	if (!line)
	{
		if (!is_line_complete(*all))
		{
			ft_putendl_fd("Minishell: syntax error: unexpected end of file", 2);
			printf("exit\n");
			ft_lstclear(all, free_token);
			free(*full);
			exit(2);
		}
		else
		{
			printf("exit\n");
			ft_lstclear(all, free_token);
			free(*full);
			exit(0);
		}
	}
}

t_list	*add_hyst_return(char **full, t_list *all)
{
	if (**full)
		add_history(*full);
	free (*full);
	return (all);
}