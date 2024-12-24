/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: borov <borov@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 21:57:29 by borov             #+#    #+#             */
/*   Updated: 2024/12/24 22:51:30 by borov            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

static int	throw_expander_error(void)
{
	write (STDERR_FILENO, "expander_error: no such variable\n", 33);
	return (1);
}

int	expand_args(char **argv, t_token_type *types, t_env_map *envs)
{
	size_t	c_arg;
	t_env	*env_got;

	if (!argv || !argv[0])
		return (1);
	c_arg = 1;
	while (argv[c_arg])
	{
		if (argv[c_arg][0] == '$' && types[c_arg] != S_QUOTE)
		{
			env_got = env_get(envs, &argv[c_arg][1]);
			if (!env_got)
				return (throw_expander_error());
			free_memo((void **)&argv[c_arg]);
			argv[c_arg] = cp_str_data(env_got->key_val[1].data);
		}
		c_arg += 1;
	}
	return (0);
}
