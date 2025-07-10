#include "ft_ls.h"

void free_colors(t_colors *colors)
{
    if (!colors)
        return;
        
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
	while (*s1 && *s2) {
		char c1 = ft_tolower((unsigned char)*s1);
		char c2 = ft_tolower((unsigned char)*s2);
		if (c1 != c2)
			return ((unsigned char)c1 - (unsigned char)c2);
		s1++;
		s2++;
	}
	return ((unsigned char)ft_tolower(*s1) - (unsigned char)ft_tolower(*s2));
}
