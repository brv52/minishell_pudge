/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environmentals_operators_1.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: borov <borov@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 20:48:17 by borov             #+#    #+#             */
/*   Updated: 2024/12/24 23:08:53 by borov            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "environmentals.h"

int	env_get_status(t_env_map *map)
{
	t_env	*status;

	status = env_get(map, "?");
	return (ft_atoi(status->key_val[1].data));
}

void	env_update(t_env_map *map, char *key, char *val)
{
	t_env	*u_env;

	u_env = env_get(map, key);
	if (!u_env)
	{
		write(STDERR_FILENO, "env_update_error: no such variable\n", 35);
		return ;
	}
	destroy_str(&u_env->key_val[1]);
	u_env->key_val[1] = create_string(val, ft_strlen(val));
}
