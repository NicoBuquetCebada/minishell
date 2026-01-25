/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbuquet- <nbuquet-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 11:27:40 by nbuquet-          #+#    #+#             */
/*   Updated: 2026/01/25 23:35:05 by nbuquet-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "minishell.h"

static int	change_directory(t_exec_ctx *ctx, char *path);
static void	update_wd(t_exec_ctx *ctx, char *wd);

int	ft_cd(t_exec_ctx *ctx, char **argv)
{
	char	*path;

	if (!argv || !argv[1] || ft_isspace(argv[1][0]) || argv[1][0] == '\0'
		|| ft_strncmp(argv[1], "--", 3) == 0)
	{
		path = get_env(ctx->envp, "HOME");
		if (!path || *path == '\0' || ft_isspace(*path))
			return (ft_error_msg("cd", NULL, "HOME not set", 1));
		return (!change_directory(ctx, path));
	}
	if (argv[2])
		return (ft_error_msg("cd", NULL, "too many arguments", 1));
	if (ft_strncmp(argv[1], "-", 2) == 0)
	{
		path = get_env(ctx->envp, "OLDPWD");
		if (!path)
			return (ft_error_msg("cd", NULL, "OLDPWD not set", 1));
		return (!change_directory(ctx, path));
	}
	return (!change_directory(ctx, argv[1]));
}

char	*get_env(char **envp, const char *key)
{
	int		i;
	char	*tmp;

	tmp = ft_strjoin(key, "=");
	if (!tmp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(tmp, envp[i], ft_strlen(tmp)) == 0)
		{
			free(tmp);
			return (ft_strchr(envp[i], '=') + 1);
		}
		i++;
	}
	free(tmp);
	return (NULL);
}

static int	change_directory(t_exec_ctx *ctx, char *path)
{
	char	*dir;
	char	*tmp;
	char	cwd[4096];

	dir = NULL;
	if (chdir(path) != 0)
		return (cd_error(path));
	dir = getcwd(cwd, 4096);
	if (!dir)
	{
		ft_error_msg("cd: error retrieving current directory",
			"getcwd: cannot access parent directories", strerror(errno), errno);
		dir = ft_strjoin(ctx->wd, "/");
		tmp = dir;
		dir = ft_strjoin(tmp, path);
		free(tmp);
	}
	else
		dir = ft_strdup(cwd);
	update_wd(ctx, dir);
	return (1);
}

static void	update_wd(t_exec_ctx *ctx, char *wd)
{
	char	*pwd_v;
	char	*pwd_env;

	pwd_v = get_env(ctx->envp, "PWD");
	if (pwd_v)
	{
		pwd_env = ft_strjoin("OLDPWD=", pwd_v);
		set_kv(ctx, pwd_env);
		free(pwd_env);
	}
	pwd_env = ft_strjoin("PWD=", wd);
	set_kv(ctx, pwd_env);
	free(pwd_env);
	if (ctx->old_wd)
		free(ctx->old_wd);
	ctx->old_wd = ft_strdup(ctx->wd);
	if (ctx->wd)
		free(ctx->wd);
	ctx->wd = ft_strdup(wd);
	free(wd);
}
