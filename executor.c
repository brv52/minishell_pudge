#include "executor.h"

int	is_bin(t_ast_node *node)
{
	if (cmp_str_data(node->t_command.argv[0], "echo") == 0)
		return (1);
	else if (cmp_str_data(node->t_command.argv[0], "cd") == 0)
		return (2);
	else if (cmp_str_data(node->t_command.argv[0], "pwd") == 0)
		return (3);
	else if (cmp_str_data(node->t_command.argv[0], "export") == 0)
		return (4);
	else if (cmp_str_data(node->t_command.argv[0], "unset") == 0)
		return (5);
	else if (cmp_str_data(node->t_command.argv[0], "env") == 0)
		return (6);
	else if (cmp_str_data(node->t_command.argv[0], "exit") == 0)
		return (7);
	return (0);
}

int	exec_bin(t_ast_node *node, t_env_map *envs, int bin_id)
{
	printf("EXEC_BIN: bin_got [%s]\n", node->t_command.argv[0]);
	if (bin_id == 1)
		return (bin_echo(node->t_command.argv, envs));
	else if (bin_id == 2)
		return (bin_cd(node->t_command.argv, envs));
	else if (bin_id == 3)
		return (bin_pwd(envs));
	else if (bin_id == 4)
		return (bin_export(node->t_command.argv, envs));
	else if (bin_id == 5)
		return (bin_unset(node->t_command.argv, envs));
	else if (bin_id == 6)
		return (bin_env(node->t_command.argv, envs));
	else if (bin_id == 7)
		return (bin_exit(node->t_command.argv));
	return (0);
}

int	exec_command(t_ast_node *op_node, t_env_map *envs)
{
	pid_t	pid;
	int		status;
	int		bin_id;

	printf("EXECUTING COMMAND\n");
	if (expand_args(op_node->t_command.argv, op_node->t_command.types, envs) < 0)
		return (-1);
	printf("cmd: [%s]\n", op_node->t_command.argv[0]);
	printf("args: [");
	size_t	tmp = 1;
	bin_id = is_bin(op_node);
	while (op_node->t_command.argc > 1 && op_node->t_command.argv[1] && op_node->t_command.argv[tmp] != NULL)
	{
		printf("%s ", op_node->t_command.argv[tmp]);
		tmp += 1;
	}
	printf("]\n");
	if (bin_id != 0)
		return (exec_bin(op_node, envs, bin_id));
	pid = fork();
	if (pid == 0)
	{
		execve(op_node->t_command.argv[0], op_node->t_command.argv, NULL);
		perror("execve failed");
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		return (status);
	}
	return (-1);
}

int	reset_std(int saved_stdin, int saved_stdout)
{
	printf("RESET\n");
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (1);
}

int	exec_redir(t_ast_node *redir_node, t_env_map *envs)
{
	printf(" REDIRECTION\n");
	int	saved_stdin = dup(STDIN_FILENO);
	int	saved_stdout = dup(STDOUT_FILENO);
	printf("saved std: %d %d", saved_stdin, saved_stdout);
	if (saved_stdin < 0 || saved_stdout < 0)
	{
		perror("Error saving stdin/stdout");
		return (-1);
	}
	printf("filename got: [%s]\n", redir_node->t_operator.right->t_command.argv[0]);
	int	fd;
	if (redir_node->t_operator.op_type == REDIR_IN)
	{
		fd = open(redir_node->t_operator.right->t_command.argv[0], O_RDONLY);
		if (fd < 0)
		{
			perror("redir_in error");
			return (-1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	else if (redir_node->t_operator.op_type == REDIR_OUT)
	{
		fd = open(redir_node->t_operator.right->t_command.argv[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd < 0)
		{
			perror("redir_out error");
			return (-1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir_node->t_operator.op_type == APPEND)
	{
		fd = open(redir_node->t_operator.right->t_command.argv[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("redir_append error");
			return (-1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
	else if (redir_node->t_operator.op_type == HEREDOC)
	{
		char	*u_input;
		int		pipe_fd[2];

		if (pipe(pipe_fd) < 0)
		{
			printf("pipe error\n");
			return (-1);
		}
		while (1)
		{
			u_input = readline("> ");
			if (!u_input)
			{
				printf("heredoc: eliminated by EOF\n");
				break ;
			}
			if (cmp_str_data(u_input, redir_node->t_operator.right->t_command.argv[0]) == 0)
			{
				free(u_input);
				break ;
			}
			write(pipe_fd[1], u_input, ft_strlen(u_input));
			write(pipe_fd[1], "\n", 1);
			free(u_input);
		}
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
	else
	{
		printf("Wrong op_type\n");
		return (-1);
	}
	int	res = execute_ast_tree(redir_node->t_operator.left, envs);
	reset_std(saved_stdin, saved_stdout);
	return (res);
}

int exec_pipe(t_ast_node *pipe_node, t_env_map *envs)
{
	printf("PIPE\n");
	int		pipe_fd[2];
	pid_t	pid[2];
	int		status[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe creation");
		return(-1);
	}
	pid[0] = fork();
	if (pid[0] == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);

		exit(execute_ast_tree(pipe_node->t_operator.left, envs));
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);

		exit(execute_ast_tree(pipe_node->t_operator.right, envs));
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid[0], &status[0], 0);
	waitpid(pid[1], &status[1], 0);
	return (status[0] && status[1]);
}

int	exec_operator(t_ast_node *op_node, t_env_map *envs)
{
	printf("EXECUTING OPERATOR: ");
	if (op_node->t_operator.op_type == PIPE)
		return (exec_pipe(op_node, envs));
	else if (is_redir(op_node->t_operator.op_type))
		return (exec_redir(op_node, envs));
	else
	{
		printf("op_error");
		return (-1);
	}
}

int	execute_ast_tree(t_ast_node *node, t_env_map *envs)
{
	if (!node)
		return (-1);
	if (node->node_type == OPERATOR)
		return (exec_operator(node, envs));
	else if (node->node_type == COMMAND)
		return (exec_command(node, envs));
	else
	{
		printf("execute_ast_tree error\n");
		return (-1);
	}
}
