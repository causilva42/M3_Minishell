/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_expanded_env.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: causilva <@student.42lisboa.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 15:45:54 by causilva          #+#    #+#             */
/*   Updated: 2026/02/10 16:21:10 by causilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	replace_param(char **line, int i, t_vars *vars);
static char	*get_param_name(char *str);
static char	*ft_getenv(char **envp, char *name);
static int	insert_param(char **line, char *param_result, int i, int j);

char	*here_doc_expanded_env(char *line, t_vars *vars)
{
	char	*dup_line;
	int		i;
	int		temp;

	dup_line = ft_strdup(line);
	if (!dup_line)
		return (NULL);
	i = 0;
	while (dup_line[i])
	{
		if (dup_line[i] == '$')
		{
			temp = replace_param(&dup_line, i, vars);
			if (temp == -1)
				return (NULL);
			i += temp;
		}
		else
			i++;
	}
	return (dup_line);
}

static int	replace_param(char **line, int i, t_vars *vars)
{
	char	*param_name;
	char	*param_result;
	int		temp;

	param_name = get_param_name(*line);
	if (!param_name || !(*param_name))
		return (free(param_name), -1);
	if (ft_strcmp(param_name, "$") == 0)
		return (free(param_name), 1);
	if (ft_strcmp(param_name, "$?") == 0)
		param_result = ft_itoa(vars->last_exit_status);
	else
		param_result = ft_getenv(vars->envp, &param_name[1]);
	if (!param_result)
		param_result = "";
	temp = ft_strlen(param_result);
	if (insert_param(line, param_result, i, ft_strlen(param_name)) == -1)
		temp = -1;
	if (ft_strcmp(param_name, "$?") == 0 && param_result[0])
		free(param_result);
	return (free(param_name), temp);
}

static char	*get_param_name(char *str)
{
	int	len;

	if (!str || (*str != '$') || ft_isdigit(str[1]))
		return (NULL);
	len = 1;
	if (str[len] == '?')
		len++;
	else
		while (ft_isalnum(str[len]) || (str[len] == '_'))
			len++;
	return (ft_substr(str, 0, len));
}

static char	*ft_getenv(char **envp, char *name)
{
	int	i;
	int	j;

	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && name[j]
			&& envp[i][j] == name[j]
			&& envp[i][j] != '=')
			j++;
		if (envp[i][j] == '=' && name[j] == '\0')
			return (&envp[i][j + 1]);
		i++;
	}
	return (NULL);
}

static int	insert_param(char **old_line, char *param_result, int i, int j)
{
	char	*line;
	int		old_line_len;
	int		param_result_len;

	old_line_len = ft_strlen(*old_line);
	param_result_len = ft_strlen(param_result);
	line = malloc(old_line_len + param_result_len);
	ft_bzero(line, old_line_len + param_result_len);
	if (!line)
		return (-1);
	ft_memmove(line, *old_line, i);
	ft_memmove(&line[i], param_result, param_result_len);
	ft_memmove(&line[i + param_result_len], &(*old_line)[i + j],
		old_line_len - i - j);
	free(*old_line);
	*old_line = line;
	return (0);
}
