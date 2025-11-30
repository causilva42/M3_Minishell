/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myivanov <myivanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:52:02 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/11/30 16:12:56 by myivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static t_token	*get_eof(t_list *tokens)
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



int	handle_heredoc(t_list *tokens)
{
	int		pipefd[2];
	char	*line;
	t_token	*eof;

	eof = get_eof(tokens);
	if (!eof)
		return (-1);

	if (pipe(pipefd) == -1)
		return (-1);

	// shell escreve no pipefd[1]
	// comando futuro vai ler de pipefd[0]

	while (1)
	{
		line = readline("heredoc> ");
		if (!line) // CTRL-D → igual ao bash
			break;

		if (ft_strcmp(line, eof->str) == 0)
		{
			free(line);
			break;
		}

		write(pipefd[1], line, ft_strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}

	close(pipefd[1]); // só deixamos o read-end aberto
	return pipefd[0];
}


