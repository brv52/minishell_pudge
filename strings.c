#include "utilities.h"

size_t	get_data_strlen(const char *str)
{
	size_t	res;

	res = 0;
	while (str && str[res])
		res += 1;
	return (res);
}

char	*cp_str_data(const char *ref_data)
{
	char	*res;
	size_t	ref_size;

	if (ref_data != NULL)
	{
		ref_size = get_data_strlen(ref_data);
		res = (char *)malloc((ref_size + 1) * sizeof(char));
		while (ref_size > 0)
		{
			res[ref_size] = ref_data[ref_size];
			ref_size -= 1;
		}
		res[0] = ref_data[0];
	}
	else
		res = NULL;
	return (res);
}

static char	*crt_str_data(const char *ref_data, size_t size)
{
	char	*res;
	size_t	c_pos;

	c_pos = 0;
	if (ref_data != NULL && size > 0)
	{
		res = (char *)malloc((size + 1) * sizeof(char));
		while (ref_data[c_pos] && c_pos < size)
		{
			res[c_pos] = ref_data[c_pos];
			c_pos += 1;
		}
		res[c_pos] = '\0';
	}
	else
		res = NULL;
	return (res);
}

t_string	create_string(const char *ref_data, size_t size)
{
	t_string	res;

	res.data = crt_str_data(ref_data, size);
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
	free_memo((void **)&str->data);
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
	int	res;

	res = 0;
	while (str && str[res])
	{
		if (str[res] == c)
			return (res);
		res += 1;
	}
	return (-1);
}
char	*chr_in_str_pos(char c, char *str)
{
	int	c_pos;

	c_pos = str_chr(c, str);
	if (c_pos == -1)
		return (NULL);
	return (&str[c_pos]);
}
