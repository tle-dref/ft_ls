#include "libft.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "ft_ls.h"


void ft_ls(char *name, t_listls **head, t_ls *ls)
{
    struct dirent *d;
    DIR *dir = opendir(name);
    if (dir == NULL) {
        if (!ls->flags->R) {
            ft_printf("Error: Cannot open directory %s\n", name);
        }
        return;
    }
    
    while ((d = readdir(dir)) != NULL)
    {
        if (ft_strcmp(d->d_name, ".") == 0 || ft_strcmp(d->d_name, "..") == 0)
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
        if (stat(full_path, &new_node->stat) != 0) {
            free(new_node->name);
            free(new_node->path);
            free(new_node);
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
    if (!list)
        return;
        
    int max_len = 0;
    int count = 0;
    t_listls *curr = list;
    
    while (curr) {
        int len = ft_strlen(curr->name);
        if (len > max_len)
            max_len = len;
        count++;
        curr = curr->next;
    }
    int term_width = 100;
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        term_width = w.ws_col;
        // printf("Terminal size: %d columns, %d rows\n", w.ws_col, w.ws_row);
    } else
        perror("ioctl");

    int col_width = max_len + 2;
    int num_cols = term_width / col_width;
    if (num_cols < 1) num_cols = 1;
    
    char **names = malloc(count * sizeof(char*));
    if (!names) {
        curr = list;
        while (curr) {
            ft_printf("%s\n", curr->name);
            curr = curr->next;
        }
        return;
    }
    
    curr = list;
    for (int i = 0; i < count; i++) {
        names[i] = curr->name;
        curr = curr->next;
    }
    
    int rows = (count + num_cols - 1) / num_cols;
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < num_cols; col++) {
            int idx = row + col * rows;
            if (idx < count) {
                ft_printf("%s", names[idx]);
                if (col < num_cols - 1 && idx + rows < count) {
                    ft_printf("  ");
                }
            }
        }
        ft_printf("\n");
    }
    
    free(names);
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
                struct stat link_stat;
                if (lstat(curr->path, &link_stat) == 0 && !S_ISLNK(link_stat.st_mode)) {
                    // if (access(curr->path, R_OK) == 0) {
                        ft_printf("\n");
                        ft_ls_recursive(curr->path, ls);
                    // }
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
