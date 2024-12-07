#include "input_handler.h"

static void	prep_promt_data(t_string *u_promt)
{
	free_memo((void **)&u_promt->data);
	init_str(u_promt);
}

void	read_input(t_string *u_promt)
{
	char	*input;

	prep_promt_data(u_promt);
	input = readline(__BEG_PROMT);
	
    if (!input)
    {
        init_str(u_promt);
        return ;
    }
    if (*input == '\0')
    {
        u_promt->data = malloc(1);
        u_promt->data[0] = '\0';
        u_promt->size = 0;
        free(input);
        return ;
    }
    if (input)
	{
		add_history(input);
		*u_promt = create_string(input, ft_strlen(input));
	}
	free(input);
}
