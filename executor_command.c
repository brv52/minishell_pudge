/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: borov <borov@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 21:01:23 by borov             #+#    #+#             */
/*   Updated: 2024/12/24 22:28:49 by borov            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

static int	exec_child(t_ast_node *op_node)
{
	int	exit_status;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	exit_status = execve(op_node->s_command.argv[0],
			op_node->s_command.argv, NULL);
	perror("Execve failed");
	exit(exit_status != 0);
}

static int	exec_parent(int pid, int *status, t_ast_node *op_node)
{
	int	sig;

	signal(SIGINT, second_hand);
	waitpid(pid, status, 0);
	if (WIFSIGNALED(*status))
	{
		sig = WTERMSIG(*status);
		if (sig == SIGQUIT)
			printf("Quit    %s\n", op_node->s_command.argv[0]);
		else if (sig == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
	}
	signal(SIGINT, handle_signal);
	signal(SIGQUIT, SIG_IGN);
	return (WEXITSTATUS(*status));
}

int	exec_command(t_ast_node *op_node, t_env_map *envs)
{
	pid_t	pid;
	int		status;
	int		bin_id;

	if (expand_args(op_node->s_command.argv,
			op_node->s_command.types, envs) != 0)
		return (1);
	bin_id = is_bin(op_node);
	if (bin_id != 0)
		return (exec_bin(op_node, envs, bin_id));
	pid = fork();
	if (pid == 0)
		exec_child(op_node);
	else
		return (exec_parent(pid, &status, op_node));
	return (1);
}
