#include "minishell.h"

static void	free_shell_data(t_shell_data *sh_data)
{
	destroy_str(&sh_data->u_promt);
}

static void	readline_cleanup()
{
	rl_clear_history();
}

static void	print_type(e_token_type type)
{
	if (type == WORD)
		printf("type: WORD");
	if (type == PIPE)
		printf("type: PIPE");
	if (type == REDIR_IN)
		printf("type: REDIR_IN");
	if (type == REDIR_OUT)
		printf("type: REDIR_OUT");
	if (type == HEREDOC)
		printf("type: HEREDOC");
	if (type == APPEND)
		printf("type: APPEND");
	if (type == S_QUOTE)
		printf("type: S_QUOTE");
	if (type == D_QUOTE)
		printf("type: D_QUOTE");

}

static void	print_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		printf("token got: [%s] ", current->data.data);
		print_type(current->type);
		printf("\n");
		current = next;
	}
}

void	print_ast(t_ast_node *node, int indent) {
	if (node == NULL) return;

	for (int i = 0; i < indent; i++) printf("  ");

	if (node->node_type == COMMAND) {
		printf("Command: ");
		for (size_t i = 0; i < node->t_command.argc; i++) {
			printf("%s ", node->t_command.argv[i].data);
		}
		printf("\n");
	}
	else if (node->node_type == OPERATOR) {
		printf("Operator: ");
		switch (node->t_operator.op_type) {
			case PIPE:
				printf("|\n");
				break;
			case REDIR_IN:
				printf("<\n");
				break;
			case REDIR_OUT:
				printf(">\n");
				break;
			case APPEND:
				printf(">>\n");
				break;
			case HEREDOC:
				printf("<<\n");
				break;
			default:
				printf("Unknown operator\n");
				break;
		}
		print_ast(node->t_operator.left, indent + 1);
		print_ast(node->t_operator.right, indent + 1);
	}
}

int	main()
{
	t_shell_data sh_data;
	while (1)
	{
		read_input(&sh_data.u_promt);
		printf("line_got: [%s]\n", sh_data.u_promt.data);
		if (sh_data.u_promt.data[0] == 'e')
			break;
		sh_data.tokens = tokenize(&sh_data.u_promt);
		print_tokens(sh_data.tokens);
		sh_data.ast_tree = parse_tokens(sh_data.tokens);
		print_ast(sh_data.ast_tree, 0);
		destroy_tokens(sh_data.tokens);
		destroy_ast_tree(sh_data.ast_tree);
		destroy_str(&sh_data.u_promt);
	}
	free_shell_data(&sh_data);
	readline_cleanup();
}
