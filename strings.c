#include "utilities.h"

t_string	create_string(const char *ref_data, size_t size)
{
	t_string	res;
	size_t		c_pos;

	c_pos = 0;
	res.data = (char *)malloc((size + 1) * sizeof(char));
	if (ref_data != NULL)
	{
		while (ref_data[c_pos] && c_pos < size)
		{
			res.data[c_pos] = ref_data[c_pos];
			c_pos += 1;
		}
	}
	res.data[c_pos] = '\0';
	res.size = size;
	return (res);
}

void	init_str(t_string *str)
{
	str->data = NULL;
	str->size = 0;
}

void	destroy_str(t_string *str)
{
	free_memo((void *)str->data);
	str->size = 0;
}
int	cmp_str_data(const char *ref, const char *cmp_to)
{
	size_t	c_pos;

	c_pos = 0;
	while ((ref && cmp_to)
		&& (ref[c_pos] && cmp_to[c_pos]))
	{
		if (ref[c_pos] != cmp_to[c_pos])
			return (ref[c_pos] - cmp_to[c_pos]);
		c_pos += 1;
	}
	if (ref[c_pos] != cmp_to[c_pos])
			return (ref[c_pos] - cmp_to[c_pos]);
	return (0);
}
int	str_chr(char c, const char *str)
{
	while (str && *str)
	{
		if (*str == c)
			return (1);
		str += 1;
	}
	return (0);
}
