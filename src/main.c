#include "libft.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ft_ls.h"

void printtamere(char *name, t_listls **head, t_ls *ls)
{
    struct dirent *d;
    DIR *dir = opendir(name);
    if (dir == NULL) {
        ft_printf("Erreur: Impossible d'ouvrir le répertoire %s\n", name);
        return;
    }
    
    while ((d = readdir(dir)) != NULL)
    {
        // Ignorer . et .. pour éviter la récursion infinie
        if (ft_strcmp(d->d_name, ".") == 0 || ft_strcmp(d->d_name, "..") == 0)
            continue;
            
        // Ignorer les fichiers cachés si -a n'est pas activé
        if (d->d_name[0] == '.' && !ls->flags->a)
            continue;
            
        // Construire le chemin complet
        char *str = ft_strjoin(name, "/");
        char *full_path = clean_join(str, d->d_name);
        
        // Créer un nouveau nœud
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
        
        // Obtenir les informations du fichier
        new_node->stat = (struct stat){0};
        if (stat(full_path, &new_node->stat) != 0) {
            free(new_node->name);
            free(new_node->path);
            free(new_node);
            free(str);
            free(full_path);
            continue;
        }
        if (S_ISDIR(new_node->stat.st_mode))
        {
            new_node->isdir = true;
        }
        else
        {
            new_node->isdir = false;
        }
        
        // Ajouter en tête de liste
        new_node->next = *head;
        *head = new_node;
        
        // Récursion sur les répertoires si -R est activé
        if (S_ISDIR(new_node->stat.st_mode) && ls->flags->R) {
            printtamere(full_path, head, ls);
        }
        
        // Libérer les ressources temporaires
        // free(str);
        free(full_path);
    }
    
    closedir(dir);
}

t_listls* reverselist(t_listls *list)
{
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
            if (current->stat.st_mtime < current->next->stat.st_mtime)
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

void printlist(t_listls *list, t_ls *ls)
{
    t_listls *curr;
    curr = list;

    // 1) Affichage des noms dans le répertoire courant
    if (!ls->flags->R)
    {
    
        while (curr)
        {
            ft_printf("%s", curr->name);
            if (curr->next)
                ft_printf(" ");
            curr = curr->next;
        }
        ft_printf("\n");
    }
    // 2) Descente récursive dans les sous-dossiers si -R
            

    if (ls->flags->R)
    {
        if (ls->flags->r) 
        {
            curr = reverselist(list);
            ft_printf("\n.:\n");
            t_listls *sublist2 = NULL;
            ls ->flags->R = false;
            printtamere(".", &sublist2, ls);
            if (sublist2)
            {
                printlist(sublist2, ls);
            }
        }
        while (curr != NULL)
        {
            if (S_ISDIR(curr->stat.st_mode))
            {
                ft_printf("\n%s:\n", curr->path);

                t_listls *sublist = NULL;
                ls->flags->R = false;
                printtamere(curr->path, &sublist, ls);
                if (sublist)
                {
                    printlist(sublist, ls);
                }
            }
            curr = curr->next;
        }
        if (!ls->flags->r) 
        {
            curr = reverselist(list);
            ft_printf("\n.:\n");
            t_listls *sublist2 = NULL;
            ls ->flags->R = false;
            printtamere(".", &sublist2, ls);
            if (sublist2)
            {
                printlist(sublist2, ls);
            }
        }
    }
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

int main(int ac, char **argv)
{
    (void)ac;
    
    t_listls *file_list = NULL;
    
    t_ls *ls = parsing(argv);
    if(!ls)
    {
        return 1;
    }
    
    // Parcourir le répertoire et créer la liste
    printtamere(ls->dir, &file_list, ls);
    
    if (ls->flags->t)
    {
        // Trier par date de modification
        file_list = sortbytime(file_list);
    }
    if (ls->flags->r)
    {
        // Inverser la liste
        file_list = reverselist(file_list);
    }
    
    printlist(file_list, ls);
    
    // Libérer la mémoire
    free_list(file_list);
    free(ls->flags);
    free(ls);
    
    return 0;
}
