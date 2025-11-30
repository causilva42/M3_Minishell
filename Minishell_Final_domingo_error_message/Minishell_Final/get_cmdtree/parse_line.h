/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_line.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myivanov <myivanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/04 14:46:05 by mykytaivano       #+#    #+#             */
/*   Updated: 2025/11/30 15:10:49 by myivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_LINE_H
# define PARSE_LINE_H

# include <stdlib.h>
# include <stdio.h>
# include "../minishell.h"


typedef struct s_split_result
{
	t_list	*left;
	t_list	*right;
}	t_split_result;



// quote_flags 0 = normal
// quote_flags 39 (') = literal
// quote_flags 34 (") = half literal

t_list		*tokenize(char const *line);

//t_cmdtree	*create_cmdtree(t_list *tokens);
//int			expand_envs(t_cmdtree *cmdtree);
//int			expand_wilds(t_cmdtree *cmdtree);
//int			remove_quotes(t_cmdtree *cmdtree);






//tokenize
t_list	*tokenize(char const *line);

//read_and_accumulate_helper.c
t_list *add_hyst_return(char **full, t_list *all);
void	ctrld_intercepted(char *line, t_list **all, char **full);
t_list	*tokenize_and_append(t_list **all, char *line);
int	handle_line_init(t_list **all, char **full, char *line);

//here_doc.c
int    handle_heredoc(t_list *tokens);

//get_cmdtree.c
void	append_token_list(t_list **dst, t_list *src);
int	is_line_complete(t_list *tokens);
t_list	*read_and_accumulate_tokens(t_vars *vars);
t_cmdtree	*get_cmdtree(t_vars *vars);



//calc_and_split.c
int	cmdtree_calc_type(t_cmdtree *cmdtree);
t_list	*get_split_point(t_list *tokens, int type);
t_list	*get_tokens_right(t_list *split_point);
t_list	*get_tokens_left(t_list *tokens, t_list *split_point);
t_split_result	split_tokens(t_list *tokens, int type);


//create_cmdtree.c
t_cmdtree	*create_cmdtree(t_list *tokens);
int	init_children(t_cmdtree *cmd, t_split_result split);
int	expand_children(t_cmdtree *cmd);
int	process_operator_node(t_cmdtree *cmd);
int	cmdtree_expand(t_cmdtree *cmd);


//paren_handler.c
void	update_paren_level(char *str, int *level);
int	has_outer_parens(t_list *tokens);
int	has_double_outer_parens(t_list *tokens);
t_list	*dup_inner_tokens(t_list *tokens);
t_list	*ft_lstremove_outer_parens(t_list *tokens);


//tree_helpers.c
t_cmdtree	*cmdtree_new_node(t_list *tokens);
void	cmdtree_clear(t_cmdtree *tree);
//int	process_leaf_node(t_cmdtree *cmd);


//operator_syntax.c
int	is_operator_str(char *str);
int	line_starts_with_op(t_list *line_tokens);
int     has_invalid_operator_sequence(t_list *tokens);


//paren_syntax
int	has_unmatched_closing_paren(t_list *tokens);
int	parens_are_fully_closed(t_list *tokens);
int	is_invalid_open_paren(t_token *prev, t_token *curr, t_token *next);
int	is_invalid_close_paren(t_token *curr, t_token *next);
int	has_invalid_paren_sequences(t_list *tokens);

//redirection_syntax.c
int	is_redirection_token(char *str);
int is_word_token(char *s);
int redir_followup_status(t_list *node);
int has_invalid_redirection_sequences(t_list *tokens);

//syntax_checks.c
int	has_fatal_syntax_error(t_list *tokens);
int	process_syntax_checks(t_list **all);

//parse_line.c
t_cmdtree *parse_line(t_list *tokens);

#endif