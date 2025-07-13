#include "libft.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "ft_ls.h"


char *add_extra_link(char *path)
{
    struct stat statlink;
    stat(path, &statlink);
    if (S_ISDIR(statlink.st_mode) == 1 )
    {
         path = ft_strjoin(path, "/");
    }
    else if (S_ISFIFO(statlink.st_mode)== 1)
    {
        path = ft_strjoin(path, "|");
    }
    else if (S_ISSOCK(statlink.st_mode)== 1)
    {
        path = ft_strjoin(path, "=");
    }
    else if ((S_IXUSR & (statlink.st_mode)) || (S_IXGRP & (statlink.st_mode)) || (S_IXOTH & (statlink.st_mode)))
    {
        path = ft_strjoin(path, "*");
    }
    return path;
}

void add_extra_info(t_listls *list, t_ls *ls)
{
    if (!list || !ls || !ls->flags->F)
        return;
    t_listls *curr = list;
    while (curr) {
        if (S_ISDIR(curr->stat.st_mode) == 1 )
        {
            curr->name = clean_join(curr->name, "/");
        }
        else if (S_ISLNK(curr->stat.st_mode)== 1)
        {
            if (ls->flags->l)
            {
                curr = curr->next; 
                continue;
            }
            curr->name = clean_join(curr->name, "@");
        }
        else if (S_ISFIFO(curr->stat.st_mode)== 1)
        {
            curr->name = clean_join(curr->name, "|");
        }
        else if (S_ISSOCK(curr->stat.st_mode)== 1)
        {
            curr->name = clean_join(curr->name, "=");
        }
        else if ((S_IXUSR & (curr->stat.st_mode)) || (S_IXGRP & (curr->stat.st_mode)) || (S_IXOTH & (curr->stat.st_mode)))
        {
            curr->name = clean_join(curr->name, "*");
        }
        curr = curr->next;
    }
}

void ft_ls(char *name, t_listls **head, t_ls *ls)
{
    struct dirent *d;
    bool is_file = false;
    // t_listls **start = NULL;
    // bool s_start = false;
    DIR *dir = opendir(name);
    if (dir == NULL) {
        is_file = true;
        if (errno == ENOENT && !ls->flags->R)
        {
            ft_printf_e("ls: %s: No such file or directory\n", name);
            return;
        }
    }
    
    while (!is_file && !ls->flags->d && (d = readdir(dir)) != NULL)
    {
        if (!ls->flags->a && (ft_strcmp(d->d_name, ".") == 0 || ft_strcmp(d->d_name, "..") == 0))
            continue;
            
        if (d->d_name[0] == '.' && !ls->flags->a)
            continue;
            
        char *str;
        size_t name_len = ft_strlen(name);
        if (name_len > 0 && name[name_len - 1] == '/') {
            str = ft_strdup(name);
        } else {
            str = ft_strjoin(name, "/");
        }
        if (!str) {
            continue;
        }
        char *full_path = clean_join(str, d->d_name);
        if (!full_path) {
            continue;
        }
        
        t_listls *new_node = malloc(sizeof(t_listls));
        if (!new_node) {
            free(full_path);
            continue;
        }
        
        new_node->name = ft_strdup(d->d_name);
        new_node->path = ft_strdup(full_path);
        if (!new_node->name || !new_node->path) {
            free(new_node->name);
            free(new_node->path);
            free(new_node);
            free(full_path);
            continue;
        }
        
        new_node->stat = (struct stat){0};
        if (lstat(full_path, &new_node->stat) != 0) {
            free(new_node->name);
            free(new_node->path);
            free(new_node);
            free(full_path);
            continue;
        }
        new_node->isdir = S_ISDIR(new_node->stat.st_mode);
        
        new_node->next = NULL;

        if (*head == NULL) {
            *head = new_node;
        } else {
            t_listls *curr = *head;
            while (curr->next != NULL) {
                curr = curr->next;
            }
            curr->next = new_node;
        }
        // if (!s_start) {
        //     start = head;
        //     s_start = true;
        // }
        free(full_path);
    }
    if (ls->flags->d || is_file) {
        t_listls *new_node = malloc(sizeof(t_listls));
        if (!new_node) {
            closedir(dir);
            return;
        }
        
        new_node->name = ft_strdup(name);
        new_node->path = ft_strdup(name);
        if (!new_node->name || !new_node->path) {
            free(new_node->name);
            free(new_node->path);
            free(new_node);
            closedir(dir);
            return;
        }
        
        new_node->stat = (struct stat){0};
        if (lstat(name, &new_node->stat) != 0) {
            free(new_node->name);
            free(new_node->path);
            free(new_node);
            closedir(dir);
            return;
        }
        new_node->isdir = S_ISDIR(new_node->stat.st_mode);
        
        new_node->next = *head;
        *head = new_node;
        // add_extra_info(*head, ls);
    }
    // if (start)
    add_extra_info(*head, ls);
    closedir(dir);
}




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
void ft_ls_recursive(char *dir_name, t_ls *ls)
{
    t_listls *file_list = NULL;
    
    DIR *test_dir = opendir(dir_name);
    if (!test_dir) {
        perror(dir_name);
        return;
    }
    closedir(test_dir);

    ft_ls(dir_name, &file_list, ls);
    
    if (ls->flags->t)
        file_list = sortbytime(file_list);
    else if (!ls->flags->U)
        file_list = sort_alphabet(file_list);

    
    if (ls->flags->r)
        file_list = reverselist(file_list);
    
    ft_printf("%s:\n", dir_name);
    printlist(file_list, ls);
    
    if (ls->flags->R) {
        t_listls *curr = file_list;
        while (curr) {
            if (curr->isdir) {
                if (ft_strcmp(curr->name, ".") != 0 && ft_strcmp(curr->name, "..") != 0) {
                    struct stat link_stat;
                    if (lstat(curr->path, &link_stat) == 0 && !S_ISLNK(link_stat.st_mode)) {
                        ft_printf("\n");
                        ft_ls_recursive(curr->path, ls);
                    }
                }
            }
            curr = curr->next;
        }
    }
    free_list(file_list);
}

int main(int ac, char **argv)
{
    (void)ac;
    t_ls *ls = parsing(argv);
    test_colors(ls->colors);
    if (!ls)
    {
        return 1;
    }
    if (ls->flags->R)
        printf("LE CACA\n");
    if (ls->flags->R) {
        ft_ls_recursive(ls->dir, ls);
    }
    else {
        t_listls *file_list = NULL;
        
        ft_ls(ls->dir, &file_list, ls);
        
        if (ls->flags->t)
            file_list = sortbytime(file_list);
        else if (!ls->flags->U)
            file_list = sort_alphabet(file_list);
        
        if (ls->flags->r)
            file_list = reverselist(file_list);

        printlist(file_list, ls);
        
        free_list(file_list);
    }
    // char *leak = malloc(1000);
    // (void)leak;
    // Clean up
    // if (ls->colors)
    //     free_colors(ls->colors);
    free(ls->flags);
    free(ls);
    
    return 0;
}
