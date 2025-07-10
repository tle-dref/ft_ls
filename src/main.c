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
    
    if (ls->dir) {
        ft_printf("Directory: %s\n", ls->dir);
    } else {
        ft_printf("Directory: current directory (.)\n");
    }
    free(ls->flags);
    free(ls);
    return 0;
}