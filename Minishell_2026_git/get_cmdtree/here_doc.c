/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:52:02 by mykytaivano       #+#    #+#             */
/*   Updated: 2026/02/11 16:32:00 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*here_doc_expanded_env(char *line, t_vars *vars);

static void	remove_quotes_token(t_token *token)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(token->str);
	while (token->str[i])
	{
		if (((token->str[i] == '\'') || (token->str[i] == '\"'))
			&& (token->quoted[i] == 0))
		{
			ft_memmove(&token->str[i], &token->str[i + 1], len - i);
			ft_memmove(&token->quoted[i], &token->quoted[i + 1], len - i);
			len--;
		}
		else
			i++;
	}
}

static	t_token	*get_eof(t_list *tokens)
{
	t_list	*node;
	t_token	*tok;

	node = tokens;
	while (node)
	{
		tok = (t_token *)node->content;
		if (ft_strcmp(tok->str, "<<") == 0)
		{
			if (node->next == NULL)
				return (NULL);
			return ((t_token *)node->next->content);
		}
		node = node->next;
	}
	return (NULL);
}

void	heredoc_loop(int *pipefd, t_token *duped, int need_exp, t_vars *vs)
{
	char	*line;
	char	*expanded_line;

	while (1)
	{
		line = readline("heredoc> ");
		if (!line)
			break ;
		if (ft_strcmp(line, duped->str) == 0)
		{
			free(line);
			break ;
		}
		if (need_exp == 1)
		{
			expanded_line = here_doc_expanded_env(line, vs);
			if (!expanded_line)
				continue ;
			free(line);
			line = expanded_line;
		}
		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}
}

int	handle_heredoc(t_list *tokens, t_vars *vars)
{
	int		pipefd[2];
	t_token	*eof;
	t_token	*duped;
	int		need_expantion;

	eof = get_eof(tokens);
	if (!eof)
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	duped = dup_token(eof);
	if (!duped)
		return (-1);
	remove_quotes_token(duped);
	need_expantion = 1;
	if (ft_strcmp(eof->str, duped->str) != 0)
		need_expantion = 0;
	heredoc_loop(pipefd, duped, need_expantion, vars);
	close(pipefd[1]);
	free_token(duped);
	return (pipefd[0]);
}
