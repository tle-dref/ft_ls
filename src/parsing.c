#include "ft_ls.h"

static void	check_l_flag(char *flag, t_flags *flags)
{
	t_long_flag_map	flag_table[] = {
		{"colors", &flags->colors}
	};
	size_t			n;
	size_t			i;

	n = sizeof(flag_table) / sizeof(flag_table[0]);
	i = 0;
	while (i < n)
	{
		if (ft_strcmp(flag_table[i].str, flag) == 0)
		{
			*(flag_table[i].field) = true;
			return ;
		}
		i++;
	}
	ft_printf_e("error: unexpected argument '--%s' found\n", flag);
	exit(1);
}

static void	apply_flag_logic(char flag, t_flags *flags)
{
	MUTEX_FLAGS('U', 't', flag, flags);
	// MUTEX_calcFLAGS('F', 'p', flag, flags);
	if (flags->d)
		flags->R = false;
	if (flags->f)
	{
		flags->a = true;
		flags->U = true;
		flags->colors = false;
	}
}

static void	check_flag(char flag, t_flags *flags)
{
	t_flag_map	flag_table[] = {
		{'l', &flags->l}, {'a', &flags->a}, {'Z', &flags->Z},
		{'f', &flags->f}, {'g', &flags->g}, {'F', &flags->F},
		{'t', &flags->t}, {'r', &flags->r}, {'U', &flags->U},
		{'R', &flags->R}, {'d', &flags->d}, {'S', &flags->S},
		{'s', &flags->s}, {'p', &flags->p}, {'o', &flags->o},
		{'N', &flags->N},
	};
	size_t		n;
	size_t		i;

	n = sizeof(flag_table) / sizeof(flag_table[0]);
	i = 0;
	while (i < n)
	{
		if (flag_table[i].c == flag)
		{
			*(flag_table[i].field) = true;
			apply_flag_logic(flag, flags);
			return ;
		}
		i++;
	}
	ft_printf_e("error: unexpected argument '-%c' found\n", flag);
	exit(1);
}

static void	parse_color_entry(char *entry, t_colors *colors)
{
	char	**key_value;

	key_value = ft_split(entry, '=');
	if (!key_value || !key_value[0] || !key_value[1])
		return ;
	if (ft_strcmp(key_value[0], "di") == 0)
		colors->directory = ft_strdup(key_value[1]);
	else if (ft_strcmp(key_value[0], "ex") == 0)
		colors->executable = ft_strdup(key_value[1]);
	else if (ft_strcmp(key_value[0], "ln") == 0)
		colors->symlink = ft_strdup(key_value[1]);
	else if (ft_strcmp(key_value[0], "pi") == 0)
		colors->pipe = ft_strdup(key_value[1]);
	else if (ft_strcmp(key_value[0], "so") == 0)
		colors->socket = ft_strdup(key_value[1]);
	else if (ft_strcmp(key_value[0], "bd") == 0)
		colors->block_device = ft_strdup(key_value[1]);
	else if (ft_strcmp(key_value[0], "cd") == 0)
		colors->char_device = ft_strdup(key_value[1]);
	free(key_value[0]);
	free(key_value[1]);
	free(key_value);
}

static t_colors	*parse_colors(void)
{
	char		*ls_colors;
	t_colors	*colors;
	char		**entries;
	int			i;

	ls_colors = getenv("LS_COLORS");
	if (!ls_colors)
		return (NULL);
	colors = ft_calloc(1, sizeof(t_colors));
	if (!colors)
		return (NULL);
	entries = ft_split(ls_colors, ':');
	if (!entries)
		return (colors);
	i = 0;
	while (entries[i])
	{
		parse_color_entry(entries[i], colors);
		free(entries[i]);
		i++;
	}
	free(entries);
	colors->regular = ft_strdup("0");
	return (colors);
}

static char	*check_dir(char *dir_name)
{
	DIR	*dir;

	dir = opendir(dir_name);
	if (!dir)
	{
		if (errno == ENOTDIR)
			return (dir_name);
	}
	else
		closedir(dir);
	return (dir_name);
}

static void	parse_arguments(char **args, t_ls *ls)
{
	int	i;
	int	j;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && args[i][1] == '-')
			check_l_flag(args[i] + 2, ls->flags);
		else if (args[i][0] == '-')
		{
			j = 1;
			while (args[i][j] != '\0')
			{
				check_flag(args[i][j], ls->flags);
				j++;
			}
		}
		else if (args[i][0] != '-')
		{
			ls->dir = check_dir(args[i]);
			if (!ls->dir)
			{
				ft_printf_e("ls: %s: No such file or directory\n", args[i]);
				exit(1);
			}
		}
		i++;
	}
}

t_ls	*parsing(char **args)
{
	t_flags	*flags;
	t_ls	*ls;

	flags = ft_calloc(1, sizeof(t_flags));
	if (!flags)
		return (NULL);
	ls = ft_calloc(1, sizeof(t_ls));
	if (!ls)
	{
		free(flags);
		return (NULL);
	}
	ls->flags = flags;
	ls->flags->colors = true;
	parse_arguments(args, ls);
	if (!ls->dir)
		ls->dir = ".";
	if (ls->flags->colors)
		ls->colors = parse_colors();
	return (ls);
}