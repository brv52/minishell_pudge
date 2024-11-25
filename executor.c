#include "executor.h"

int	exec_command(t_ast_node *op_node)
{
	pid_t	pid;
	int		status;

	printf("EXECUTING COMMAND\n");
	printf("cmd: [%s]\n", op_node->t_command.argv[0]);
	printf("args: [");
	size_t	tmp = 1;
	while (op_node->t_command.argc > 1 && op_node->t_command.argv[1] && op_node->t_command.argv[tmp] != NULL)
	{
		printf("%s ", op_node->t_command.argv[tmp]);
		tmp += 1;
	}
	printf("]\n");
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

int	exec_redir(t_ast_node *redir_node)
{
	printf(" REDIRECTION\n");
	int	saved_stdin = dup(STDIN_FILENO);
	int	saved_stdout = dup(STDOUT_FILENO);
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
		printf("HEREDOC TODO\n");
		return (0);
	}
	else
	{
		printf("Wrong op_type\n");
		return (-1);
	}
	int	res = execute_ast_tree(redir_node->t_operator.left);
	reset_std(saved_stdin, saved_stdout);
	return (res);
}

int exec_pipe(t_ast_node *pipe_node)
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

		exit(execute_ast_tree(pipe_node->t_operator.left));
	}
	pid[1] = fork();
	if (pid[1] == 0)
	{
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);

		exit(execute_ast_tree(pipe_node->t_operator.right));
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid[0], &status[0], 0);
	waitpid(pid[1], &status[1], 0);
	return (status[0] && status[1]);
}

int	exec_operator(t_ast_node *op_node)
{
	printf("EXECUTING OPERATOR: ");
	if (op_node->t_operator.op_type == PIPE)
		return (exec_pipe(op_node));
	else if (is_redir(op_node->t_operator.op_type))
		return (exec_redir(op_node));
	else
	{
		printf("op_error");
		return (-1);
	}
}

int	execute_ast_tree(t_ast_node *node)
{
	if (!node)
		return (-1);
	if (node->node_type == OPERATOR)
		return (exec_operator(node));
	else if (node->node_type == COMMAND)
		return (exec_command(node));
	else
	{
		printf("execute_ast_tree error\n");
		return (-1);
	}
}
