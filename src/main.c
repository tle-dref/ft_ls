#include "ft_ls.h"

int	main(int ac, char **argv)
{
	t_ls		*ls;
	t_listls	*file_list;

	(void)ac;
	ls = parsing(argv);
	if (!ls)
		return (1);
	if (ls->flags->R)
	{
		ft_ls_recursive(ls->dir, ls);
	}
	else
	{
		file_list = NULL;
		ft_ls(ls->dir, &file_list, ls);
		if (ls->flags->t)
			file_list = sortbytime(file_list);
		else if (!ls->flags->U)
			file_list = sort_alphabet(file_list);
		if (ls->flags->S)
			file_list = sortbysize(file_list);
		if (ls->flags->s)
			// file_list = sortbyblock(file_list);
		if (ls->flags->r)
			file_list = reverselist(file_list);
		if (ls->flags->N)
			file_list = foreach_list_trunc_N(file_list, ft_strtrim);
		printlist(file_list, ls);
		free_list(file_list);
	}
	if (ls->colors)
		free_colors(ls->colors);
	free(ls->flags);
	free(ls);
	return (0);
}
