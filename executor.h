#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "parser.h"
#include "enviromentals.h"
#include "built_in.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int	execute_ast_tree(t_ast_node *node, t_env_map *envs);

#endif
