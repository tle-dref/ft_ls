#include "ft_ls.h"
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>

void printlist(t_listls *list, t_ls *ls)
{
    if (!list)
        return;
    
    if (ls->flags->l) {
        print_list_l(list, ls);
        return;
    }
        
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


void print_list_l(t_listls *list, t_ls *ls)
{
    (void)ls;
    if (!list)
        return;
    
    int max_nlink_width = 0;
    int max_user_width = 0;
    int max_group_width = 0;
    int max_size_width = 0;
    
    long total_blocks = 0;
    t_listls *curr = list;
    while (curr) {
        total_blocks += curr->stat.st_blocks;        
        int nlink_val = (int)curr->stat.st_nlink;
        int nlink_len = (nlink_val == 0) ? 1 : 0;
        int temp_val = nlink_val;
        while (temp_val > 0) {
            nlink_len++;
            temp_val /= 10;
        }
        if (nlink_len > max_nlink_width) max_nlink_width = nlink_len;
        
        struct passwd *pwd = getpwuid(curr->stat.st_uid);
        char *username = (pwd && pwd->pw_name) ? pwd->pw_name : "unknown";
        int user_len = ft_strlen(username);
        if (user_len > max_user_width) max_user_width = user_len;
        
        struct group *grp = getgrgid(curr->stat.st_gid);
        char *groupname = (grp && grp->gr_name) ? grp->gr_name : "unknown";
        int group_len = ft_strlen(groupname);
        if (group_len > max_group_width) max_group_width = group_len;
        int size_val = (int)curr->stat.st_size;
        int size_len = (size_val == 0) ? 1 : 0;
        temp_val = size_val;
        while (temp_val > 0) {
            size_len++;
            temp_val /= 10;
        }
        if (size_len > max_size_width) max_size_width = size_len;
        
        curr = curr->next;
    }
    
    ft_printf("total %d\n", (int)(total_blocks / 2));
    
    curr = list;
    while (curr) {
        char type = '-';
        if (S_ISDIR(curr->stat.st_mode)) type = 'd';
        else if (S_ISLNK(curr->stat.st_mode)) type = 'l';
        else if (S_ISCHR(curr->stat.st_mode)) type = 'c';
        else if (S_ISBLK(curr->stat.st_mode)) type = 'b';
        else if (S_ISFIFO(curr->stat.st_mode)) type = 'p';
        else if (S_ISSOCK(curr->stat.st_mode)) type = 's';
        
        char *perms = format_perm(curr->stat.st_mode);
        if (!perms) {
            curr = curr->next;
            continue;
        } 

        struct passwd *pwd = getpwuid(curr->stat.st_uid);
        struct group *grp = getgrgid(curr->stat.st_gid);
        
        char username[64];
        char groupname[64];
        
        if (pwd && pwd->pw_name) {
            ft_strlcpy(username, pwd->pw_name, sizeof(username));
        } else {
            ft_strlcpy(username, "unknown", sizeof(username));
        }
        if (grp && grp->gr_name) {
            ft_strlcpy(groupname, grp->gr_name, sizeof(groupname));
        } else {
            ft_strlcpy(groupname, "unknown", sizeof(groupname));
        }
        
        char *formatted_time = format_time(curr->stat.st_mtime);
        if (!formatted_time) {
            free(perms);
            curr = curr->next;
            continue;
        }
        
        ft_printf("%c%s ", type, perms);
        
        char *nlink_str = format_number_right((int)curr->stat.st_nlink, max_nlink_width);
        if (nlink_str) {
            ft_printf("%s ", nlink_str);
            free(nlink_str);
        }
        char *user_str = format_string_left(username, max_user_width);
        char *group_str = format_string_left(groupname, max_group_width);
        if (user_str && group_str) {
            ft_printf("%s %s ", user_str, group_str);
            free(user_str);
            free(group_str);
        }

        char *size_str = format_number_right((int)curr->stat.st_size, max_size_width);
        if (size_str) {
            ft_printf("%s ", size_str);
            free(size_str);
        }
        ft_printf("%s ", formatted_time);
        if (S_ISLNK(curr->stat.st_mode)) {
            char linkbuf[1024];
            ssize_t linklen = readlink(curr->path, linkbuf, sizeof(linkbuf) - 1);
            if (linklen != -1) {
                linkbuf[linklen] = '\0';
                ft_printf("%s -> %s\n", curr->name, linkbuf);
            } else {
                ft_printf("%s\n", curr->name);
            }
        } else {
            ft_printf("%s\n", curr->name);
        }
        
        free(perms);
        free(formatted_time);
        curr = curr->next;
    }
}