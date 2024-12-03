#include "expander.h"

int	expand_args(char **argv, e_token_type *types, t_env_map *envs)
{
	size_t	c_arg;
	t_env	*env_got;

	if (!argv || !argv[0] || !argv[1])
		return (0);
	c_arg = 1;
	while (argv[c_arg])
	{
		printf("expanding argv[%ld]: %s %d", c_arg, argv[c_arg], types[c_arg]);
		if (argv[c_arg][0] == '$' && types[c_arg] != S_QUOTE)
		{
			env_got = env_get(envs, &argv[c_arg][1]);
			if (!env_got)
			{
				printf("expander: no such variable\n");
				return (-1);
			}
			printf("changing %s to %s\n", argv[c_arg], env_got->key_val[1].data);
			free_memo((void **)&argv[c_arg]);
			argv[c_arg] = cp_str_data(env_got->key_val[1].data);
		}
		c_arg += 1;
	}
	return (0);
}
