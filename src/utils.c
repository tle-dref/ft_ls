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