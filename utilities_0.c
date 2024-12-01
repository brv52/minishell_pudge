#include "utilities.h"

void	free_memo(void **memo)
{
	if (*memo)
	{
		free(*memo);
		*memo = NULL;
	}
}

size_t	ft_strlen(const char *str)
{
	size_t	res;

	res = 0;
	if (str)
	{
		while (*str++)
			res += 1;
	}
	return (res);
}

int	is_alpha(char c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}
int	is_num(char c)
{
	return (c >= '0' && c <= '9');
}

int	is_al_num(char c)
{
	return (is_alpha(c) || is_num(c));
}
t_string	str_cat(const char *a, const char *b)
{
	t_string	buffer;
	size_t		len_a;
	size_t		len_b;
	size_t		c_pos;

	len_a = ft_strlen(a);
	len_b = ft_strlen(b);
	c_pos = 0;
	buffer.data = (char *)malloc((len_a + len_b + 1) * sizeof(char));
	buffer.size = len_a + len_b;
	buffer.data[len_a + len_b] = '\0';
	while (a && a[c_pos])
	{
		buffer.data[c_pos] = a[c_pos];
		c_pos += 1;
	}
	while (b && b[c_pos - len_a])
	{
		buffer.data[c_pos] = b[c_pos - len_a];
		c_pos += 1;
	}
	return (buffer);
}
