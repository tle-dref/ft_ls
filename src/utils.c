#include "ft_ls.h"

void	free_colors(t_colors *colors)
{
	if (!colors)
		return ;
	if (colors->directory)
		free(colors->directory);
	if (colors->executable)
		free(colors->executable);
	if (colors->regular)
		free(colors->regular);
	if (colors->symlink)
		free(colors->symlink);
	if (colors->pipe)
		free(colors->pipe);
	if (colors->socket)
		free(colors->socket);
	if (colors->block_device)
		free(colors->block_device);
	if (colors->char_device)
		free(colors->char_device);
	free(colors);
}


int	ft_strcasecmp(const char *s1, const char *s2)
{
	char	c1;
	char	c2;

	while (*s1 == '.')
		s1++;
	while (*s2 == '.')
		s2++;
	while (*s1 && *s2)
	{
		c1 = ft_tolower((unsigned char)*s1);
		c2 = ft_tolower((unsigned char)*s2);
		if (c1 != c2)
			return ((unsigned char)c1 - (unsigned char)c2);
		s1++;
		s2++;
	}
	return ((unsigned char)ft_tolower(*s1) - (unsigned char)ft_tolower(*s2));
}


void	free_list(t_listls *list)
{
	t_listls	*tmp;

	while (list != NULL)
	{
		tmp = list;
		list = list->next;
		if (tmp->name)
			free(tmp->name);
		if (tmp->path)
			free(tmp->path);
		free(tmp);
	}
}

char	*add_extra_link(char *path)
{
	struct stat	statlink;

	if (stat(path, &statlink) != 0)
		return (path);
	if (S_ISDIR(statlink.st_mode))
		return (ft_strjoin(path, "/"));
	else if (S_ISFIFO(statlink.st_mode))
		return (ft_strjoin(path, "|"));
	else if (S_ISSOCK(statlink.st_mode))
		return (ft_strjoin(path, "="));
	else if (statlink.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
		return (ft_strjoin(path, "*"));
	return (path);
}

void	add_extra_info(t_listls *list, t_ls *ls)
{
	t_listls	*curr;

	if (!list || !ls || !ls->flags->F)
		return ;
	curr = list;
	while (curr)
	{
		if (S_ISDIR(curr->stat.st_mode))
			curr->name = clean_join(curr->name, "/");
		else if (S_ISLNK(curr->stat.st_mode))
		{
			if (!ls->flags->l)
				curr->name = clean_join(curr->name, "@");
		}
		else if (S_ISFIFO(curr->stat.st_mode))
			curr->name = clean_join(curr->name, "|");
		else if (S_ISSOCK(curr->stat.st_mode))
			curr->name = clean_join(curr->name, "=");
		else if (curr->stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
			curr->name = clean_join(curr->name, "*");
		curr = curr->next;
	}
}