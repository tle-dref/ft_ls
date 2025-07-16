#include "ft_ls.h"
#include <stdarg.h>

char *get_color_for_file(t_listls *file, t_colors *colors)
{
    if (!file || !colors)
        return NULL;
        
    mode_t mode = file->stat.st_mode;
    
    if (S_ISDIR(mode))
        return colors->directory;
    else if (S_ISLNK(mode))
        return colors->symlink;
    else if (S_ISFIFO(mode))
        return colors->pipe;
    else if (S_ISSOCK(mode))
        return colors->socket;
    else if (S_ISBLK(mode))
        return colors->block_device;
    else if (S_ISCHR(mode))
        return colors->char_device;
    else if (S_ISREG(mode) && (mode & S_IXUSR))
        return colors->executable;
    else
        return colors->regular;
}

int ft_printfcolor(t_listls *file, t_colors *colors, const char *format, ...)
{
    va_list args;
    int result = 0;
    char *color_code;
    char *str_arg;
    long arg;

    if (!format)
        return 0;
        
    va_start(args, format);
    
    if (ft_strcmp(format, "%s") == 0 || ft_strcmp(format, "%s\n") == 0) {
        str_arg = va_arg(args, char*);
        
        if (file && colors && (color_code = get_color_for_file(file, colors))) {
            if (ft_strcmp(format, "%s\n") == 0) {
                result = ft_printf("\033[%sm%s\033[0m\n", color_code, str_arg);
            } else {
                result = ft_printf("\033[%sm%s\033[0m", color_code, str_arg);
            }
        } else {
            result = ft_printf(format, str_arg);
        }
    }
    else if(ft_strcmp(format, "%d") == 0 || ft_strcmp(format, "%d\n") == 0)
    {
        arg = va_arg(args, long);
        result = ft_printf(format, arg);
        ft_printf(" ");
        ft_printfcolor(file, colors, "%s", file->name);
    }
    else {
        result = ft_printf("ERROR: Unsupported format in ft_printfcolor\n");
    }
    
    va_end(args);
    return result;
}
