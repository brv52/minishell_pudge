#ifndef ENVIROMENTALS_H
#define ENVIROMENTALS_H

#include "utilities.h"

typedef struct enviromental
{
	t_string			key_val[2];
	struct enviromental	*next;
}	t_env;

typedef struct env_map
{
	t_env	**data;
	size_t	buckets;
}	t_env_map;

size_t	hash(const char *str);
int	init_map(t_env_map *map, size_t buckets);
int		env_add(t_env_map *map, const char *key, const char *val);
t_env	*env_get(t_env_map *map, const char *key);
int		env_remove(t_env_map *map, const char *key);
void	destroy_map(t_env_map *map);
void	print_map(t_env_map *map);

#endif
