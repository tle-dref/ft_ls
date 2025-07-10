#include "ft_ls.h"

void print_flags(t_flags *flags)
{
    ft_printf("Flags active:\n");
    if (flags->l) ft_printf("  -l: long format\n");
    if (flags->a) ft_printf("  -a: show hidden files\n");
    if (flags->Z) ft_printf("  -Z: security context\n");
    if (flags->f) ft_printf("  -f: brut format\n");
    if (flags->g) ft_printf("  -g: long format without owner\n");
    if (flags->F) ft_printf("  -F: classify files\n");
    if (flags->t) ft_printf("  -t: sort by time\n");
    if (flags->r) ft_printf("  -r: reverse order\n");
    if (flags->U) ft_printf("  -U: no sorting\n");
    if (flags->R) ft_printf("  -R: recursive\n");
    if (flags->d) ft_printf("  -d: directory itself\n");
    if (flags->colors) ft_printf(" --colors\n");
}

void test_colors(t_colors *colors)
{
    if (!colors) {
        ft_printf("No colors configuration found.\n");
        return;
    }
    
    ft_printf("\n=== COLORS TEST ===\n");
    ft_printf("Testing color codes:\n");
    
    if (colors->directory)
        ft_printf("Directory: \033[%sm[DIR]\033[0m (%s)\n", colors->directory, colors->directory);
    if (colors->executable)
        ft_printf("Executable: \033[%sm[EXE]\033[0m (%s)\n", colors->executable, colors->executable);
    if (colors->symlink)
        ft_printf("Symlink: \033[%sm[LINK]\033[0m (%s)\n", colors->symlink, colors->symlink);
    if (colors->pipe)
        ft_printf("Pipe: \033[%sm[PIPE]\033[0m (%s)\n", colors->pipe, colors->pipe);
    if (colors->socket)
        ft_printf("Socket: \033[%sm[SOCK]\033[0m (%s)\n", colors->socket, colors->socket);
    if (colors->block_device)
        ft_printf("Block device: \033[%sm[BLK]\033[0m (%s)\n", colors->block_device, colors->block_device);
    if (colors->char_device)
        ft_printf("Char device: \033[%sm[CHR]\033[0m (%s)\n", colors->char_device, colors->char_device);
    if (colors->regular)
        ft_printf("Regular file: \033[%sm[REG]\033[0m (%s)\n", colors->regular, colors->regular);
}

int main(int ac, char **av)
{
    
    for (int i = 0; i < ac; i++) {
        ft_printf("arg[%d]: %s\n", i, av[i]);
    }
    
    t_ls *ls = parsing(av);
    if (!ls)
        return 1;
    
    ft_printf("\n=== PARSING RESULTS ===\n");
    print_flags(ls->flags);
    
    if (ls->flags->colors && ls->colors) {
        test_colors(ls->colors);
    }
    
    if (ls->dir) {
        ft_printf("Directory: %s\n", ls->dir);
    } else {
        ft_printf("Directory: current directory (.)\n");
    }
    
    free(ls->flags);
    if (ls->colors)
        free_colors(ls->colors);
    free(ls);
    return 0;
}