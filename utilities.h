#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

#define __BEG_PROMT "minishell> "
#define __METACHARS "|&<>()"

typedef struct str
{
	char	*data;
	size_t	size;
}	t_string;

t_string	create_string(const char *ref_data, size_t size);
void		init_str(t_string *str);
void		free_memo(void **memo);
size_t		ft_strlen(const char *str);
void		destroy_str(t_string *str);
int			cmp_str_data(const char *ref, const char *cmp_to);
int			str_chr(char c, const char *str);
char		*cp_str_data(const char *ref_data);
char		*chr_in_str_pos(char c, char *str);
int			is_alpha(char c);
int			is_num(char c);
int			is_al_num(char c);
t_string	str_cat(const char *a, const char *b);
#endif
