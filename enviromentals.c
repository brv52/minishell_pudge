#include "enviromentals.h"

size_t	hash(const char *str)
{
	size_t	djb_hashcode;
	size_t	idx;

	idx = 0;
	djb_hashcode = 5381;
	while (str && str[idx])
	{
		djb_hashcode = ((djb_hashcode << 5) + djb_hashcode) + str[idx];
		idx += 1;
	}
	return (djb_hashcode);
}

int	init_map(t_env_map *map, size_t buckets)
{
	size_t		c_bucket;

	c_bucket = 0;
	map->buckets = buckets;
	map->data = (t_env **)malloc(map->buckets * sizeof(t_env*));
	while (c_bucket < map->buckets)
	{
		map->data[c_bucket] = NULL;
		c_bucket += 1;
	}
	env_add(map, "PWD", getenv("PWD"));
	env_add(map, "HOME", getenv("HOME"));
	return (1);
}
int		env_add(t_env_map *map, const char *key, const char *val)
{
	size_t	key_idx;
	t_env	*current;
	t_env	*n_node;

	if(!key || !val || !map)
	{
		printf("env_add: invalid arguments\n");
		return (-1);
	}
	key_idx = hash(key) % map->buckets;
	current = map->data[key_idx];
	while (current)
	{
		if (cmp_str_data(current->key_val[0].data, key) == 0)
		{
			printf("env_add: enviromental already exists\n");
			destroy_str(&current->key_val[1]);
			current->key_val[1] = create_string(val, ft_strlen(val));
			return (0);
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	n_node = (t_env *)malloc(sizeof(t_env));
	n_node->key_val[0] = create_string(key, ft_strlen(key));
	n_node->key_val[1] = create_string(val, ft_strlen(val));
	n_node->next = NULL;
	if (current)
		current->next = n_node;
	else
		map->data[key_idx] = n_node;
	return (0);
}
t_env	*env_get(t_env_map *map, const char *key)
{
	char	*res;
	t_env	*stored;

	res = getenv(key);
	stored = map->data[hash(key) % map->buckets];
	while (stored && cmp_str_data(stored->key_val[0].data, key) != 0)
		stored = stored->next;
	if (stored && cmp_str_data(stored->key_val[0].data, key) != 0)
			stored = NULL;
	if (res && !stored)
	{
		env_add(map, key, res);
		return (env_get(map, key));
	}
	return (stored);
}
int	env_remove(t_env_map *map, const char *key)
{
	t_env	*del_env;
	t_env	*prev;
	size_t	idx;

	idx = hash(key) % map->buckets;
	del_env = map->data[idx];
	prev = NULL;
	while (del_env)
	{
		if (cmp_str_data(del_env->key_val[0].data, key) == 0)
		{
			if (prev)
				prev->next = del_env->next;
			else
				map->data[idx] = map->data[idx]->next;
			destroy_str(&del_env->key_val[0]);
			destroy_str(&del_env->key_val[1]);
			free_memo((void **)&del_env);
			return (0);
		}
		prev = del_env;
		del_env = del_env->next;
	}
	return (-1);
}
void	destroy_map(t_env_map *map)
{
	t_env	*entry;
	t_env	*tmp;
	size_t	c_bucket;

	c_bucket = 0;
	while (c_bucket < map->buckets)
	{
		entry = map->data[c_bucket];
		while (entry)
		{
			tmp = entry;
			entry = entry->next;
			destroy_str(&tmp->key_val[0]);
			destroy_str(&tmp->key_val[1]);
			free_memo((void **)&tmp);
		}
		c_bucket += 1;
	}
	free_memo((void **)&map->data);
	map->buckets = 0;
}
void	print_map(t_env_map *map)
{
	t_env	*current;
	size_t	c_bucket;

	c_bucket = 0;
	while (c_bucket < map->buckets)
	{
		current = map->data[c_bucket];
		while (current)
		{
			printf("%s=%s\n", current->key_val[0].data, current->key_val[1].data);
			current = current->next;
		}
		c_bucket += 1;
	}
}
