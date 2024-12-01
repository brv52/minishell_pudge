#include "built_in.h"

int	bin_echo(char **argv, t_env_map *envs)
{
	size_t	c_arg;
	int		n_flag;

	if (!argv || !argv[0] || !envs)
		return (-1);

	if (argv[1] && cmp_str_data(argv[1], "-n") == 0)
		n_flag = 1;
	else
		n_flag = 0;
	c_arg = n_flag + 1;
	while (argv[c_arg] != NULL)
	{
		printf("%s", argv[c_arg]);
		if (argv[c_arg] && argv[c_arg + 1] != NULL)
			printf(" ");
		c_arg += 1;
	}
	if (n_flag == 0)
		printf("\n");
	return(1);
}

static int	upd_pwd(t_env_map *envs)
{
	t_env	*path;
	char	*n_pwd;

	path = env_get(envs, "PWD");
	n_pwd = getcwd(NULL, 0);
	if (path != NULL)
	{
		destroy_str(&path->key_val[1]);
		path->key_val[1] = create_string(n_pwd, ft_strlen(n_pwd));
		free(n_pwd);
	}
	else
	{
		perror("upd_pwd error\n");
		free(n_pwd);
		return (-1);
	}
	return (0);
}

int	bin_cd(char **argv, t_env_map *envs)
{
	char	*dir;

	if (!argv || (argv[1] && argv[2]))
	{
		perror("cd: error\n");
		return (-1);
	}
	if (!argv[1])
	{
		printf("cd: no args\n");
		dir = (env_get(envs, "HOME"))->key_val[1].data;
		if (!dir)
		{
			perror("env HOME not set\n");
			return (-1);
		}
	}
	else
		dir = argv[1];
	if (chdir(dir))
	{
		perror("cd error");
		return (-1);
	}
	return (upd_pwd(envs));
}

int	bin_pwd(t_env_map *envs)
{
	t_env	*cwd;

	cwd = env_get(envs, "PWD");
	if (!cwd)
	{
		perror("upd_pwd error\n");
		return (-1);
	}
	printf("PWD: %s\n", cwd->key_val[1].data);
	return (0);
}

int	bin_export(char **argv, t_env_map *envs)
{
	char	*key;
	char	*val;

	if (!argv || !argv[0] || !argv[1] || (argv[1] && argv[2]))
	{
		printf("invalid argument\n");
		return (-1);
	}
	key = cp_str_data(argv[1]);
	val = chr_in_str_pos('=', key);
	if (!val)
	{
		printf("export: invalid format, expexted key=value\n");
		free(key);
		return (-1);
	}
	*val = '\0';
	val += 1;
	if (!is_alpha(key[0]) && key[0] != '_')
	{
		printf("export: invalid key\n");
		free(key);
		return (-1);
	}
	char	*key_cp;
	key_cp = key;
	while (key_cp && *key_cp)
	{
		if (!is_al_num(*key_cp) && *key_cp != '_')
		{
			printf("export: invalid format, expexted key=value\n");
			free(key);
			return (-1);
		}
		key_cp += 1;
	}
	int res = env_add(envs, key, val);
	free(key);
	return (res);
}

int	bin_unset(char **argv, t_env_map *envs)
{
	t_env	*u_env;

	if (!argv || !argv[0] || !argv[1] || (argv[1] && argv[2]))
	{
		printf("unset: arguments error\n");
		return (-1);
	}
	u_env = env_get(envs, argv[1]);
	if (!u_env)
	{
		printf("unset: no such variable\n");
		return (-1);
	}
	return (env_remove(envs, u_env->key_val[0].data));
}

int	bin_env(char **argv, t_env_map *envs)
{
	t_env	*g_env;

	if (!argv || !argv[0] || !argv[1] || (argv[1] && argv[2]))
	{
		printf("env: arguments error\n");
		return (-1);
	}
	print_map(envs);
	g_env = env_get(envs, argv[1]);
	if (!g_env)
	{
		printf("env: env not found\n");
		return (-1);
	}
	printf("%s=%s\n", g_env->key_val[0].data, g_env->key_val[1].data);
	return (0);
}

int	bin_exit(char **argv)
{
	if (!argv || !argv[0] || argv[1])
	{
		printf("unset: arguments error\n");
		return (-1);
	}
	exit(0);
	return (0);
}
