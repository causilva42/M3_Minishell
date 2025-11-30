/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: myivanov <myivanov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:21:51 by causilva          #+#    #+#             */
/*   Updated: 2025/11/30 17:41:34 by myivanov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_cmdtree.h"
#include "../minishell.h"

char	*get_cmd_path(char *cmd, char **envp);
char	*try_paths(char **paths, char *cmd);
char	*get_path_from_env(char **envp);
void		memory_cleanup(t_vars vars);

int	exec_cmd(t_cmdtree *cmdtree, t_vars *vars)
{
	char	**argv;
	char	*path;
	int		temp;
	int		id;

	if (redirect(cmdtree, vars) == -1)
		return (127);
	argv = get_argv_tokenlst(cmdtree->tokens);
	temp = 0;
	if (is_builtin(argv[0]))
	{
		temp = builtin(argv, cmdtree->fd_output, vars);
		return (free(argv), temp);
	}
	id = fork();
	if (id == -1)
		return (127);
	if (id == 0)
	{
		dup2(cmdtree->fd_input, STDIN_FILENO);
		dup2(cmdtree->fd_output, STDOUT_FILENO);
		io_cleanup(*vars);
		path = get_cmd_path(argv[0], vars->envp);
		if (path)
			if (execve(path, argv, vars->envp) == -1)
				exit((free(argv), memory_cleanup(*vars), 127));
	}
	if (id > 0)
		if (waitpid(id, &temp, 0) == -1)
			return (free(argv), 127);
	return (free(argv), temp);
}

char	*get_cmd_path(char *cmd, char **envp)
{
	char	*path_str;
	char	**paths;
	char	*result;

	if (cmd[0] == '/' || cmd[0] == '.')
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	path_str = get_path_from_env(envp);
	if (!path_str)
		return (NULL);
	paths = ft_split(path_str, ':');
	if (!paths)
		return (NULL);
	result = try_paths(paths, cmd);
	return (result);
}

char	*try_paths(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*full;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		full = ft_strjoin(temp, cmd);
		free (temp);
		if (access(full, X_OK) == 0)
		{
			free_array_split(paths);
			return (full);
		}
		free(full);
		i++;
	}
	free_array_split(paths);
	printf("%s: command not found\n", cmd);
	return (NULL);
}

char	*get_path_from_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}
