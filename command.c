/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liton <livbrandon@outlook.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/27 20:47:18 by liton             #+#    #+#             */
/*   Updated: 2017/08/02 00:07:26 by liton            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		command_env(char **env, char *cmd, char *builtins)
{
	if (ft_strcmp(cmd, builtins) == 0)
		ft_display_tab(env);
}

void		command_ls(char **env, char *cmd, char *builtins)
{
	pid_t	pid;
	char	**av;

	pid = fork();
	(void)builtins;
	if (pid > 0)
		wait(NULL);
	else
	{
		av = ft_strsplit(cmd, ' ');
		execve("/bin/ls", av, env);
	}
}
