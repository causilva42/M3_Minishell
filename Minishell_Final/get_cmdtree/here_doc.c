/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/19 12:52:02 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/11/29 18:01:26 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	here_parent_routine(int pipefd[2], pid_t pid)
{
	int	status;

	close (pipefd[1]);
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		close(pipefd[0]);
		return (-1);
	}
	return (pipefd[0]);
}

static void	write_and_free(char *line, int pipefd[2])
{
	write(pipefd[1], line, ft_strlen(line));
	write(pipefd[1], "\n", 1);
	free(line);
}

int	handle_heredoc(t_list *tokens)
{
	int		pipefd[2];
	pid_t	pid;
	t_token	*eof;
	char	*line;

	eof = get_eof(tokens);
	pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(pipefd[0]);
		while (1)
		{
			line = readline("heredoc> ");
			if (!line || ft_strcmp(line, eof->str) == 0)
				break ;
			write_and_free(line, pipefd);
		}
		free (line);
		close(pipefd[1]);
		exit (0);
	}
	return (here_parent_routine(pipefd, pid));
}
