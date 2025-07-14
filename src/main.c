#include "ft_ls.h"

char *add_extra_link(char *path)
{
    struct stat statlink;
    
    if (stat(path, &statlink) != 0)
        return path;
        
    if (S_ISDIR(statlink.st_mode))
        return ft_strjoin(path, "/");
    else if (S_ISFIFO(statlink.st_mode))
        return ft_strjoin(path, "|");
    else if (S_ISSOCK(statlink.st_mode))
        return ft_strjoin(path, "=");
    else if (statlink.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
        return ft_strjoin(path, "*");
    
    return path;
}

void add_extra_info(t_listls *list, t_ls *ls)
{
    if (!list || !ls || !ls->flags->F)
        return;
        
    t_listls *curr = list;
    while (curr) {
        if (S_ISDIR(curr->stat.st_mode))
            curr->name = clean_join(curr->name, "/");
        else if (S_ISLNK(curr->stat.st_mode)) {
            if (!ls->flags->l)
                curr->name = clean_join(curr->name, "@");
        }
        else if (S_ISFIFO(curr->stat.st_mode))
            curr->name = clean_join(curr->name, "|");
        else if (S_ISSOCK(curr->stat.st_mode))
            curr->name = clean_join(curr->name, "=");
        else if (curr->stat.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
            curr->name = clean_join(curr->name, "*");
            
        curr = curr->next;
    }
}

static t_listls *create_file_node(const char *name, const char *full_path)
{
    t_listls *new_node = malloc(sizeof(t_listls));
    if (!new_node)
        return NULL;
        
    new_node->name = ft_strdup(name);
    new_node->path = ft_strdup(full_path);
    if (!new_node->name || !new_node->path) {
        free(new_node->name);
        free(new_node->path);
        free(new_node);
        return NULL;
    }
    
    new_node->stat = (struct stat){0};
    if (lstat(full_path, &new_node->stat) != 0) {
        free(new_node->name);
        free(new_node->path);
        free(new_node);
        return NULL;
    }
    
    new_node->isdir = S_ISDIR(new_node->stat.st_mode);
    new_node->next = NULL;
    
    return new_node;
}

static void add_node_to_list(t_listls **head, t_listls *new_node)
{
    if (*head == NULL) {
        *head = new_node;
    } else {
        t_listls *curr = *head;
        while (curr->next != NULL)
            curr = curr->next;
        curr->next = new_node;
    }
}

static char *build_full_path(const char *dir_name, const char *file_name)
{
    char *str;
    size_t name_len = ft_strlen(dir_name);
    
    if (name_len > 0 && dir_name[name_len - 1] == '/')
        str = ft_strdup(dir_name);
    else
        str = ft_strjoin(dir_name, "/");
        
    if (!str)
        return NULL;
        
    char *full_path = clean_join(str, (char *)file_name);
    return full_path;
}

void ft_ls(char *name, t_listls **head, t_ls *ls)
{
    struct dirent *d;
    bool is_file = false;
    DIR *dir = opendir(name);
    
    if (dir == NULL) {
        is_file = true;
        if (errno == ENOENT && !ls->flags->R) {
            ft_printf_e("ls: %s: No such file or directory\n", name);
            return;
        }
    }
    
    while (!is_file && !ls->flags->d && (d = readdir(dir)) != NULL) {
        if (!ls->flags->a && (ft_strcmp(d->d_name, ".") == 0 || ft_strcmp(d->d_name, "..") == 0))
            continue;
            
        if (d->d_name[0] == '.' && !ls->flags->a)
            continue;
            
        char *full_path = build_full_path(name, d->d_name);
        if (!full_path)
            continue;
        
        t_listls *new_node = create_file_node(d->d_name, full_path);
        if (new_node)
            add_node_to_list(head, new_node);
            
        free(full_path);
    }
    
    if (ls->flags->d || is_file) {
        t_listls *new_node = create_file_node(name, name);
        if (new_node) {
            new_node->next = *head;
            *head = new_node;
        }
    }
    
    add_extra_info(*head, ls);
    if (dir)
        closedir(dir);
}

static void process_file_list(t_listls **file_list, t_ls *ls)
{
    if (ls->flags->t)
        *file_list = sortbytime(*file_list);
    else if (!ls->flags->U)
        *file_list = sort_alphabet(*file_list);
    
    if (ls->flags->r)
        *file_list = reverselist(*file_list);
}

void ft_ls_recursive(char *dir_name, t_ls *ls)
{
    DIR *test_dir = opendir(dir_name);
    if (!test_dir) {
        perror(dir_name);
        return;
    }
    closedir(test_dir);

    t_listls *file_list = NULL;
    ft_ls(dir_name, &file_list, ls);
    process_file_list(&file_list, ls);
    
    ft_printf("%s:\n", dir_name);
    printlist(file_list, ls);
    
    if (ls->flags->R) {
        t_listls *curr = file_list;
        while (curr) {
            if (curr->isdir && ft_strcmp(curr->name, ".") != 0 && ft_strcmp(curr->name, "..") != 0) {
                struct stat link_stat;
                if (lstat(curr->path, &link_stat) == 0 && !S_ISLNK(link_stat.st_mode)) {
                    ft_printf("\n");
                    ft_ls_recursive(curr->path, ls);
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
    if (!ls)
        return 1;

    if (ls->flags->R) {
        ft_ls_recursive(ls->dir, ls);
    } else {
        t_listls *file_list = NULL;
        ft_ls(ls->dir, &file_list, ls);
        process_file_list(&file_list, ls);
        printlist(file_list, ls);
        free_list(file_list);
    }

    // Cleanup
    if (ls->colors)
        free_colors(ls->colors);
    free(ls->flags);
    free(ls);
    
    return 0;
}
