/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myivanov <myivanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 13:31:25 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/11/30 17:52:50 by myivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include "../parse_line.h"

int	has_fatal_syntax_error(t_list *tokens)
{
	if (has_unmatched_closing_paren(tokens))
		return (1);
	if (has_invalid_operator_sequence(tokens))
		return (1);
	if (has_invalid_paren_sequences(tokens))
		return (1);
	return (0);
}

int	process_syntax_checks(t_list **all)
{
	int	rstat;

	if (has_fatal_syntax_error(*all))
	{
		ft_lstclear(all, free_token);
		return (-1);
	}
	rstat = has_invalid_redirection_sequences(*all);
	if (rstat)
	{
		if (rstat == 2)
			printf("syntax error near unexpected token `newline'\n");
		else
			printf("syntax error\n");
		ft_lstclear(all, free_token);
		return (-1);
	}
	return (0);
}
