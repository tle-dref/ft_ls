#include "libft.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ft_ls.h"


void ft_ls(char *name, t_listls **head, t_ls *ls)
{
    struct dirent *d;
    DIR *dir = opendir(name);
    if (dir == NULL) {
        ft_printf("Error: Cannot open directory %s\n", name);
        return;
    }
    
    while ((d = readdir(dir)) != NULL)
    {
        if (ft_strcmp(d->d_name, ".") == 0 || ft_strcmp(d->d_name, "..") == 0)
            continue;
            
        if (d->d_name[0] == '.' && !ls->flags->a)
            continue;
            
        char *str = ft_strjoin(name, "/");
        char *full_path = clean_join(str, d->d_name);
        
        t_listls *new_node = malloc(sizeof(t_listls));
        if (!new_node) {
            free(str);
            free(full_path);
            continue;
        }
        
        new_node->name = ft_strdup(d->d_name);
        new_node->path = ft_strdup(full_path);
        if (!new_node->name || !new_node->path) {
            free(new_node->name);
            free(new_node->path);
            free(new_node);
            free(str);
            free(full_path);
            continue;
        }
        
        new_node->stat = (struct stat){0};
        if (stat(full_path, &new_node->stat) != 0) {
            free(new_node->name);
            free(new_node->path);
            free(new_node);
            free(str);
            free(full_path);
            continue;
        }
        new_node->isdir = S_ISDIR(new_node->stat.st_mode);
        
        new_node->next = *head;
        *head = new_node;
        
        free(full_path);
    }
    closedir(dir);
}







void printlist(t_listls *list, t_ls *ls)
{
    (void)ls;
    t_listls *curr = list;

    while (curr)
    {
        ft_printf("%s", curr->name);
        if (curr->next)
            ft_printf(" ");
        curr = curr->next;
    }
    ft_printf("\n");
}

void free_list(t_listls *list)
{
    t_listls *tmp;
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

void ft_ls_recursive(char *dir_name, t_ls *ls)
{
    t_listls *file_list = NULL;
    
    ft_ls(dir_name, &file_list, ls);
    
    if (ls->flags->t)
        file_list = sortbytime(file_list);
    else
        file_list = sort_alphabet(file_list);
    
    if (ls->flags->r)
        file_list = reverselist(file_list);
    
    ft_printf("%s:\n", dir_name);
    printlist(file_list, ls);
    
    if (ls->flags->R) {
        t_listls *curr = file_list;
        while (curr) {
            if (curr->isdir) {
                ft_printf("\n");
                ft_ls_recursive(curr->path, ls);
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
    if(!ls)
    {
        return 1;
    }
    
    if (ls->flags->R) {
        ft_ls_recursive(ls->dir, ls);
    }
    else {
        t_listls *file_list = NULL;
        
        ft_ls(ls->dir, &file_list, ls);
        
        if (ls->flags->t)
            file_list = sortbytime(file_list);
        else
            file_list = sort_alphabet(file_list);
        
        if (ls->flags->r)
            file_list = reverselist(file_list);

        printlist(file_list, ls);
        
        free_list(file_list);
    }
    
    // Clean up
    // if (ls->colors)
    //     free_colors(ls->colors);
    free(ls->flags);
    free(ls);
    
    return 0;
}
