#include "ft_ls.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>

static void	calculate_list_metrics(t_listls *list, int *max_len, int *count)
{
	t_listls	*curr;
	int			len;

	*max_len = 0;
	*count = 0;
	curr = list;
	while (curr)
	{
		len = ft_strlen(curr->name);
		if (len > *max_len)
			*max_len = len;
		(*count)++;
		curr = curr->next;
	}
}

static int	get_terminal_width(void)
{
	return (80);
}

static void	print_simple_list(t_listls *list, t_ls *ls)
{
	t_listls	*curr;

	curr = list;
	while (curr)
	{
		ft_printfcolor(curr, ls->colors, "%s\n", curr->name);
		curr = curr->next;
	}
}

static t_listls	**create_files_array(t_listls *list, int count)
{
	t_listls	**files;
	t_listls	*curr;
	int			i;

	files = malloc(count * sizeof(t_listls*));
	if (!files)
		return (NULL);
	curr = list;
	i = 0;
	while (i < count)
	{
		files[i] = curr;
		curr = curr->next;
		i++;
	}
	return (files);
}

static void	print_columnar_format(t_listls **files, int count, int rows, int num_cols, t_ls *ls)
{
	int	row;
	int	col;
	int	idx;

	row = 0;
	while (row < rows)
	{
		col = 0;
		while (col < num_cols)
		{
			idx = row + col * rows;
			if (idx < count)
			{
				ft_printfcolor(files[idx], ls->colors, "%s", files[idx]->name);
				if (col < num_cols - 1 && idx + rows < count)
					ft_printf("  ");
			}
			col++;
		}
		ft_printf("\n");
		row++;
	}
}

void	printlist(t_listls *list, t_ls *ls)
{
	int			max_len;
	int			count;
	int			term_width;
	int			col_width;
	int			num_cols;
	int			rows;
	t_listls	**files;

	if (!list)
		return ;
	if (ls->flags->l || ls->flags->g)
	{
		print_list_l(list, ls);
		return ;
	}
	calculate_list_metrics(list, &max_len, &count);
	term_width = get_terminal_width();
	col_width = max_len + 2;
	num_cols = term_width / col_width;
	if (num_cols < 1)
		num_cols = 1;
	files = create_files_array(list, count);
	if (!files)
	{
		print_simple_list(list, ls);
		return ;
	}
	rows = (count + num_cols - 1) / num_cols;
	print_columnar_format(files, count, rows, num_cols, ls);
	free(files);
}


static int	calculate_number_width(int num)
{
	int	width;

	if (num == 0)
		return (1);
	width = 0;
	while (num > 0)
	{
		width++;
		num /= 10;
	}
	return (width);
}

static void	calculate_column_widths(t_listls *list, int *max_nlink_width,
		int *max_user_width, int *max_group_width, int *max_size_width)
{
	t_listls		*curr;
	struct passwd	*pwd;
	struct group	*grp;
	int				len;

	*max_nlink_width = 0;
	*max_user_width = 0;
	*max_group_width = 0;
	*max_size_width = 0;
	curr = list;
	while (curr)
	{
		len = calculate_number_width((int)curr->stat.st_nlink);
		if (len > *max_nlink_width)
			*max_nlink_width = len;
		pwd = getpwuid(curr->stat.st_uid);
		len = ft_strlen((pwd && pwd->pw_name) ? pwd->pw_name : "unknown");
		if (len > *max_user_width)
			*max_user_width = len;
		grp = getgrgid(curr->stat.st_gid);
		len = ft_strlen((grp && grp->gr_name) ? grp->gr_name : "unknown");
		if (len > *max_group_width)
			*max_group_width = len;
		len = calculate_number_width((int)curr->stat.st_size);
		if (len > *max_size_width)
			*max_size_width = len;
		curr = curr->next;
	}
}

static long	calculate_total_blocks(t_listls *list)
{
	long		total_blocks;
	t_listls	*curr;

	total_blocks = 0;
	curr = list;
	while (curr)
	{
		total_blocks += curr->stat.st_blocks;
		curr = curr->next;
	}
	return (total_blocks);
}

static char	get_file_type(mode_t mode)
{
	if (S_ISDIR(mode))
		return ('d');
	else if (S_ISLNK(mode))
		return ('l');
	else if (S_ISCHR(mode))
		return ('c');
	else if (S_ISBLK(mode))
		return ('b');
	else if (S_ISFIFO(mode))
		return ('p');
	else if (S_ISSOCK(mode))
		return ('s');
	else
		return ('-');
}

static void	get_user_group_names(t_listls *curr, char *username, char *groupname)
{
	struct passwd	*pwd;
	struct group	*grp;

	pwd = getpwuid(curr->stat.st_uid);
	grp = getgrgid(curr->stat.st_gid);
	if (pwd && pwd->pw_name)
		ft_strlcpy(username, pwd->pw_name, 64);
	else
		ft_strlcpy(username, "unknown", 64);
	if (grp && grp->gr_name)
		ft_strlcpy(groupname, grp->gr_name, 64);
	else
		ft_strlcpy(groupname, "unknown", 64);
}

static void	print_file_info(t_listls *curr, t_ls *ls, int max_nlink_width,
		int max_user_width, int max_group_width, int max_size_width)
{
	char	type;
	char	*perms;
	char	*formatted_time;
	char	username[64];
	char	groupname[64];
	char	*nlink_str;
	char	*user_str;
	char	*group_str;
	char	*size_str;

	type = get_file_type(curr->stat.st_mode);
	perms = format_perm(curr->stat.st_mode);
	if (!perms)
		return ;
	get_user_group_names(curr, username, groupname);
	formatted_time = format_time(curr->stat.st_mtime);
	if (!formatted_time)
	{
		free(perms);
		return ;
	}
	ft_printf("%c%s ", type, perms);
	nlink_str = format_number_right((int)curr->stat.st_nlink, max_nlink_width);
	if (nlink_str)
	{
		ft_printf("%s ", nlink_str);
		free(nlink_str);
	}
	user_str = format_string_left(username, max_user_width);
	group_str = format_string_left(groupname, max_group_width);
	if (user_str && group_str)
	{
		if (!ls->flags->g)
			ft_printf("%s %s ", user_str, group_str);
		else
			ft_printf("%s ", group_str);
		free(user_str);
		free(group_str);
	}
	size_str = format_number_right((int)curr->stat.st_size, max_size_width);
	if (size_str)
	{
		ft_printf("%s ", size_str);
		free(size_str);
	}
	ft_printf("%s ", formatted_time);
	free(perms);
	free(formatted_time);
}

static void	print_symlink(t_listls *curr, t_ls *ls)
{
	char		linkbuf[1024];
	ssize_t		linklen;
	char		*readedlink;
	t_listls	*link_list;

	linklen = readlink(curr->path, linkbuf, sizeof(linkbuf) - 1);
	if (linklen != -1)
	{
		linkbuf[linklen] = '\0';
		link_list = malloc(sizeof(t_listls));
		if (!link_list)
		{
			ft_printfcolor(curr, ls->colors, "%s\n", curr->name);
			return ;
		}
		link_list->next = NULL;
		link_list->path = linkbuf;
		stat(link_list->path, &link_list->stat);
		if (ls->flags->F)
		{
			readedlink = add_extra_link(linkbuf);
			ft_printfcolor(curr, ls->colors, "%s", curr->name);
			ft_printf(" -> ");
			ft_printfcolor(link_list, ls->colors, "%s\n", readedlink);
		}
		else
		{
			ft_printfcolor(curr, ls->colors, "%s", curr->name);
			ft_printf(" -> ");
			ft_printfcolor(link_list, ls->colors, "%s\n", linkbuf);
		}
		free(link_list);
	}
	else
		ft_printfcolor(curr, ls->colors, "%s\n", curr->name);
}

static void	print_filename(t_listls *curr, t_ls *ls)
{
	if (S_ISLNK(curr->stat.st_mode))
		print_symlink(curr, ls);
	else
		ft_printfcolor(curr, ls->colors, "%s\n", curr->name);
}

void	print_list_l(t_listls *list, t_ls *ls)
{
	int			max_nlink_width;
	int			max_user_width;
	int			max_group_width;
	int			max_size_width;
	long		total_blocks;
	t_listls	*curr;

	if (!list)
		return ;
	calculate_column_widths(list, &max_nlink_width, &max_user_width,
		&max_group_width, &max_size_width);
	total_blocks = calculate_total_blocks(list);
	ft_printf("total %d\n", (int)(total_blocks / 2));
	curr = list;
	while (curr)
	{
		print_file_info(curr, ls, max_nlink_width, max_user_width,
			max_group_width, max_size_width);
		print_filename(curr, ls);
		curr = curr->next;
	}
}