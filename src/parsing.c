#include "ft_ls.h"

void check_flag(char flag, t_flags *flags)
{
    t_flag_map flag_table[] = {
        {'l', &flags->l}, {'a', &flags->a}, {'Z', &flags->Z}, {'f', &flags->f},
        {'g', &flags->g}, {'F', &flags->F}, {'t', &flags->t}, {'r', &flags->r},
        {'U', &flags->U}, {'R', &flags->R}, {'d', &flags->d},
    };
    size_t n = sizeof(flag_table) / sizeof(flag_table[0]);

    for (size_t i = 0; i < n; ++i) {
        if (flag_table[i].c == flag) {
            MUTEX_FLAGS('U', 't', flag, flags);
            *(flag_table[i].field) = true;
            
            return;
        }
    }
    ft_printf("invalid args");
    exit(1);
}

char *check_dir(char *dir_name)
{
    DIR *dir = opendir(dir_name);
    if (!dir)
        return NULL;
    closedir(dir);
    return dir_name;
}

t_ls *parsing(char **args)
{
    t_flags *flags = ft_calloc(1, sizeof(t_flags));
    t_ls *ls = ft_calloc(1, sizeof(t_ls));
    ls->flags = flags;
    int i = 1;
    while (args[i]) {
        if (args[i][0] == '-') {
            int j = 1;
            while(args[i][j] != '\0') {
                check_flag(args[i][j], flags);
                j++;
            }
        }
        else if (args[i][0] != '-') {
            ls->dir = check_dir(args[i]);
            if (!ls->dir) {
                ft_printf("ls: %s: No such file or directory\n", args[i]);
                return NULL;
            }
        }
        i++;
    }
    if (!ls->dir)
        ls->dir = ".";
    return (ls);
}