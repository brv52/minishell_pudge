#include "utilities.h"

void	free_memo(void *memo)
{
	if (memo)
		free(memo);
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
