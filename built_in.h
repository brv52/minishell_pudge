#ifndef GUILT_IN_H
#define GUILT_IN_H

#include "utilities.h"
#include "enviromentals.h"

int	bin_echo(char **argv, t_env_map *envs);
int	bin_cd(char **argv, t_env_map *envs);
int	bin_pwd(t_env_map *envs);
int	bin_export(char **argv, t_env_map *envs);
int	bin_unset(char **argv, t_env_map *envs);
int	bin_env(char **argv, t_env_map *envs);
int	bin_exit(char **argv);

#endif
