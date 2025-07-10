#include "libft.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/ft_ls.h"

void printtamere(char *name, t_listls **head)
{
    struct dirent *d;
    DIR *dir = opendir(name);
    
    if (dir == NULL) {
        ft_printf("Erreur: Impossible d'ouvrir le répertoire %s\n", name);
        return;
    }
    
    while ((d = readdir(dir)) != NULL)
    {
        if (ft_strcmp(d->d_name, ".") == 0 || ft_strcmp(d->d_name, "..") == 0 || ft_strcmp(d->d_name, ".git") == 0)
            continue;
            
        char *str = ft_strjoin(name, "/");
        char *tho = ft_strdup(d->d_name);
        char *full_path = clean_join(str, d->d_name);

        t_listls *new_node = malloc(sizeof(t_listls));
        
        if (ft_strncmp(tho, ".", 1) != 0) {
            new_node->name = ft_strdup(tho);
            new_node->path = ft_strdup(full_path);        
        }
        
        if (!new_node->name || !new_node->path) {
            
            continue;
        }
        
        new_node->stat = (struct stat){0};
        if (tho && full_path && stat(full_path, &new_node->stat) != 0) {
            continue;
        }
        
        // Ajouter en tête de liste
        new_node->next = *head;
        *head = new_node;
        
        if (S_ISDIR(new_node->stat.st_mode) && false) // rajouter la verif -R
        {
            printtamere(full_path, head);
        }
        
        // Libérer les ressources temporaires
        free(tho);
        free(full_path);
    }
    closedir(dir);
}

t_listls* reverselist(t_listls *list){
    t_listls *prev = NULL;
    t_listls *current = list;
    t_listls *next = NULL;

    while (current != NULL) {
        next = current->next; 
        current->next = prev; 
        prev = current;       
        current = next;
    }
    return prev;
}

t_listls* sortbytime(t_listls *list)
{
    if (!list || !list->next)
        return list;
    
    int swapped;
    t_listls *current, *prev, *next;
    
    do {
        swapped = 0;
        current = list;
        prev = NULL;
        
        while (current && current->next)
        {
            if (current->stat.st_atime < current->next->stat.st_atime)
            {

                next = current->next;
                current->next = next->next;
                next->next = current;
                
                if (prev == NULL)
                    list = next;
                else
                    prev->next = next;
                
                prev = next;
                swapped = 1;
            }
            else
            {
                prev = current;
                current = current->next;
            }
        }
    } while (swapped);
    
    return list;
}

void printlist(t_listls *list)
{
    // -g retire mochamsa
    while (list != NULL)
    {
        printf("%s | %s | %ld | %ld\n", list->name, list->path, 
                list->stat.st_size, list->stat.st_atime);
        list = list->next;
    }
}

void free_list(t_listls *list)
{
    t_listls *tmp;
    while (list != NULL)
    {
        tmp = list;
        list = list->next;
        free(tmp->name);
        free(tmp->path);
        free(tmp);
    }
}

int main(int ac, char **argv)
{
    (void)ac;
    (void)argv;
    
    t_listls *file_list = NULL;
    
    // Parcourir le répertoire et créer la liste
    printtamere(".", &file_list);
    
    ft_printf("=== Liste des fichiers ===\n");
    printlist(file_list);
    // Inverser la liste
    file_list = reverselist(file_list);
    printf("\n");
    printlist(file_list);
    // Trier par date d'accès
    file_list = sortbytime(file_list);
    printf("\n");
    printlist(file_list);
    // Libérer la mémoire
    free_list(file_list);
    
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
