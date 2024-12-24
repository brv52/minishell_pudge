/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: borov <borov@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 21:03:58 by borov             #+#    #+#             */
/*   Updated: 2024/12/24 22:29:02 by borov            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

int	throw_exec_error(int err_code)
{
	if (err_code == 0)
		write(STDERR_FILENO,
			"executor_error: cannot save file descriptors\n", 45);
	else if (err_code == 1)
		write(STDERR_FILENO, "executor_redir_in: file open error\n", 35);
	else if (err_code == 2)
		write(STDERR_FILENO, "executor_redir_out: file open error\n", 36);
	else if (err_code == 3)
		write(STDERR_FILENO, "executor_redir_append: file open error\n", 39);
	else if (err_code == 4)
		write(STDERR_FILENO,
			"executor_redir_heredoc: pipe creation error\n", 44);
	else if (err_code == 5)
		write(STDERR_FILENO, "executor_redir: unknown redirection type\n", 41);
	else if (err_code == 6)
		write(STDERR_FILENO, "executor_operator: unknown operator type\n", 41);
	else if (err_code == 7)
		write(STDERR_FILENO, "executor_tree: unknown node type\n", 33);
	return (1);
}

int	reset_std(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (0);
}

int	exec_operator(t_ast_node *op_node, t_env_map *envs)
{
	if (op_node->s_operator.op_type == PIPE
		|| op_node->s_operator.op_type == OR
		|| op_node->s_operator.op_type == AND)
		return (exec_connector(op_node, envs));
	else if (is_redir(op_node->s_operator.op_type))
		return (exec_redir(op_node, envs));
	else
		return (throw_exec_error(6));
}

int	execute_ast_tree(t_ast_node *node, t_env_map *envs)
{
	if (!node)
		return (1);
	if (node->node_type == OPERATOR)
		return (exec_operator(node, envs));
	else if (node->node_type == COMMAND)
		return (exec_command(node, envs));
	else
		return (throw_exec_error(7));
}
