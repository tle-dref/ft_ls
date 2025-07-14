#include "ft_ls.h"

char *format_perm(mode_t perms)
{
	char *str = malloc(10);
	if (!str)
		return (NULL);
	
	str[0] = (perms & S_IRUSR) ? 'r' : '-';
	str[1] = (perms & S_IWUSR) ? 'w' : '-';
	str[2] = (perms & S_IXUSR) ? 'x' : '-';
	str[3] = (perms & S_IRGRP) ? 'r' : '-';
	str[4] = (perms & S_IWGRP) ? 'w' : '-';
	str[5] = (perms & S_IXGRP) ? 'x' : '-';
	str[6] = (perms & S_IROTH) ? 'r' : '-';
	str[7] = (perms & S_IWOTH) ? 'w' : '-';
	str[8] = (perms & S_IXOTH) ? 'x' : '-';
	str[9] = '\0';
	
	return (str);
}

static char	*parse_time_components(char *raw, char *formatted)
{
	char	month[4] = {0};
	char	day[3] = {0};
	char	time_part[6] = {0};
	char	*day_ptr;
	int		i;

	i = 0;
	while (i < 3)
	{
		month[i] = raw[4 + i];
		i++;
	}
	i = 0;
	while (i < 2)
	{
		day[i] = raw[8 + i];
		i++;
	}
	i = 0;
	while (i < 5)
	{
		time_part[i] = raw[11 + i];
		i++;
	}
	day_ptr = (day[0] == ' ') ? day + 1 : day;
	ft_strlcpy(formatted, month, 20);
	ft_strlcat(formatted, " ", 20);
	ft_strlcat(formatted, day_ptr, 20);
	ft_strlcat(formatted, " ", 20);
	ft_strlcat(formatted, time_part, 20);
	return (formatted);
}

char *format_time(time_t t)
{
	char	*raw;
	char	*formatted;

	raw = ctime(&t);
	if (!raw)
		return (NULL);
	formatted = malloc(20);
	if (!formatted)
		return (NULL);
	if (ft_strlen(raw) < 24)
	{
		ft_strlcpy(formatted, "Unknown", 20);
		return (formatted);
	}
	return (parse_time_components(raw, formatted));
}

char *format_number_right(int num, int width)
{
	char	*result;
	char	*num_str;
	int		len;
	int		spaces;
	int		i;
	int		j;

	result = malloc(width + 1);
	if (!result)
		return (NULL);
	num_str = ft_itoa(num);
	if (!num_str)
	{
		free(result);
		return (NULL);
	}
	len = ft_strlen(num_str);
	spaces = width - len;
	i = 0;
	while (i < spaces)
		result[i++] = ' ';
	j = 0;
	while (j < len)
	{
		result[i + j] = num_str[j];
		j++;
	}
	result[width] = '\0';
	free(num_str);
	return (result);
}

char *format_string_left(char *str, int width)
{
	char	*result;
	int		len;
	int		i;

	result = malloc(width + 1);
	if (!result)
		return (NULL);
	len = ft_strlen(str);
	i = 0;
	while (i < len && i < width)
	{
		result[i] = str[i];
		i++;
	}
	while (i < width)
	{
		result[i] = ' ';
		i++;
	}
	result[width] = '\0';
	return (result);
}