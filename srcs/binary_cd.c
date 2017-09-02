/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liton <livbrandon@outlook.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/01 23:43:56 by liton             #+#    #+#             */
/*   Updated: 2017/09/02 17:22:58 by liton            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		support_cd(char **av, char *dir, char **env, char **path)
{
	struct stat		buf;
	int				p;

	if (!av[1])
	{
		if ((p = search_v(env, "HOME")) == -1)
		{
			ft_putstr_fd("Variable HOME not set.\n", 2);
			free_cd(av, dir, *path, 0);
			return (-1);
		}
		*path = ft_strjoinfree(*path, env[p] + 5, 1);
		if (stat(*path, &buf) == -1)
		{
			if (env[p][5] == '\0')
				ft_putstr_fd("Variable HOME don't have value.\n", 2);
			else
				error_msg("cd: no such file or directory", env[p] + 5);
			free_cd(av, dir, *path, 0);
			return (-1);
		}
		if (!(buf.st_mode & S_IXUSR))
			error_msg("cd: permission denied:", env[p] + 5);
	}
	return (0);
}

static int		support_cd_3(char **av, char *dir, char **env, char ***path)
{
	int				p;
	struct stat		buf;

	if ((p = search_v(env, "OLDPWD")) == -1)
	{
		ft_putstr_fd("Variable OLDPWD not set.\n", 2);
		free_cd(av, dir, **path, 0);
		return (-1);
	}
	**path = ft_strjoinfree(**path, env[p] + 7, 1);
	if (stat(**path, &buf) == -1)
	{
		if (env[p][7] == '\0')
			ft_putstr_fd("Variable OLDPWD don't have value\n", 2);
		else
			error_msg("cd: no such file or directory", env[p] + 7);
		free_cd(av, dir, **path, 0);
		return (-1);
	}
	if (!(buf.st_mode & S_IXUSR))
		error_msg("cd: permission denied:", env[p] + 7);
	else
		ft_putendl(**path);
	return (0);
}

static int		support_cd_2(char **av, char *dir, char **env, char **path)
{
	if (!ft_strcmp(av[1], "-") && support_cd_3(av, dir, env, &path) == -1)
		return (-1);
	else if (av[1] && av[1][0] != '/' && ft_strcmp(av[1], "-") != 0)
	{
		ft_strdel(path);
		*path = ft_strdup("/");
		*path = ft_strjoinfree(dir, *path, 2);
		*path = ft_strjoinfree(*path, av[1], 1);
	}
	else if (av[1] && av[1][0] == '/')
		*path = ft_strjoinfree(*path, av[1], 1);
	return (0);
}

static void		modify_env(char ***env, char **dir)
{
	int		i;
	int		p;
	char	**new_env;

	new_env = NULL;
	i = search_v(*env, "PWD");
	if ((p = search_v(*env, "OLDPWD")) != -1)
		i >= 0 ? modify_v(*env, p, "OLDPWD", (*env)[i] + 4) : modify_v(*env, p, "OLDPWD", *dir);
	else
	{
		new_env = i >= 0 ? add_v(*env, "OLDPWD", (*env)[i] + 4, 0) : add_v(*env, "OLDPWD", *dir, 0);
		*env = new_env;
	}
	if ((p = search_v(*env, "PWD")) != -1)
	{
		ft_strdel(dir);
		modify_v(*env, p, "PWD", (*dir = getcwd(*dir, 100)));
	}
	else
	{
		ft_strdel(dir);
		new_env = add_v(*env, "PWD", (*dir = getcwd(*dir, 100)), 0);
		*env = new_env;
	}
}

void			binary_cd(char ***env, char *cmd)
{
	struct stat		buf;
	char			**av;
	char			*dir;
	char			*path;

	dir = NULL;
	av = ft_strsplit(cmd, ' ');
	dir = getcwd(dir, 100);
	path = ft_strnew(0);
	if ((support_cd(av, dir, *env, &path)) == -1)
		return ;
	if (!path[0] && (support_cd_2(av, dir, *env, &path)) == -1)
		return ;
	if (chdir(path))
	{
		if (!stat(path, &buf) && !(S_ISDIR(buf.st_mode)) && ft_strcmp(av[1], "-"))
			error_msg("cd: not a directory:", av[1]);
		else if (!stat(path, &buf) && !(buf.st_mode & S_IXUSR) && ft_strcmp(av[1], "-"))
			error_msg("cd: permission denied:", av[1]);	
		else if (av[1] && ft_strcmp(av[1], "-"))
			error_msg("cd: no such file or directory", av[1]);
		free_cd(av, dir, path, 0);
		return ;
	}
	free_cd(av, dir, path, 1);
	modify_env(env, &dir);
	ft_strdel(&dir);
}
