#include "ft_ls.h"

void check_l_flag(char *flag, t_flags *flags)
{
    t_long_flag_map flag_table[] = {
        {"colors", &flags->colors}
    };
    size_t n = sizeof(flag_table) / sizeof(flag_table[0]);

    for (size_t i = 0; i < n; ++i) {
        if (ft_strcmp(flag_table[i].str, flag) == 0) {
            *(flag_table[i].field) = true;
            return;
        }
    }
    ft_printf_e("error: unexpected argument '--%s' found", flag);
    exit(1);
}

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
    ft_printf_e("error: unexpected argument '-%c' found", flag);
    exit(1);
}

t_colors *parse_colors()
{
    char *ls_colors = getenv("LS_COLORS");
    if (!ls_colors)
        return NULL;
    
    t_colors *colors = ft_calloc(1, sizeof(t_colors));
    
    char **entries = ft_split(ls_colors, ':');
    if (!entries)
        return NULL;
    for (int i = 0; entries[i]; i++) {
        char **key_value = ft_split(entries[i], '=');
        if (!key_value || !key_value[0] || !key_value[1])
            continue;
            
        if (ft_strcmp(key_value[0], "di") == 0) {        
            colors->directory = ft_strdup(key_value[1]);
        }
        else if (ft_strcmp(key_value[0], "ex") == 0) {  
            colors->executable = ft_strdup(key_value[1]);
        }
        else if (ft_strcmp(key_value[0], "ln") == 0) { 
            colors->symlink = ft_strdup(key_value[1]);
        }
        else if (ft_strcmp(key_value[0], "pi") == 0) {
            colors->pipe = ft_strdup(key_value[1]);
        }
        else if (ft_strcmp(key_value[0], "so") == 0) {
            colors->socket = ft_strdup(key_value[1]);
        }
        else if (ft_strcmp(key_value[0], "bd") == 0) {
            colors->block_device = ft_strdup(key_value[1]);
        }
        else if (ft_strcmp(key_value[0], "cd") == 0) {
            colors->char_device = ft_strdup(key_value[1]);
        }
        free(key_value[0]);
        free(key_value[1]);
        free(key_value);
    }
    for (int i = 0; entries[i]; i++)
        free(entries[i]);
    free(entries);
    colors->regular = ft_strdup("0");
    return colors;
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
        if (args[i][0] == '-' && args[i][1] == '-') {
            check_l_flag(args[i] + 2, flags);
        }
        else if (args[i][0] == '-') {
            int j = 1;
            while(args[i][j] != '\0') {
                check_flag(args[i][j], flags);
                j++;
            }
        }
        else if (args[i][0] != '-') {
            ls->dir = check_dir(args[i]);
            if (!ls->dir) {
                ft_printf_e("ls: %s: No such file or directory\n", args[i]);
                return NULL;
            }
        }
        i++;
    }
    if (!ls->dir) ls->dir = ".";
    if (ls->flags->colors) {
        ls->colors = parse_colors();
    }
    return (ls);
}