#include "ft_ls.h"

char *format_perm(mode_t perms)
{
    char *str = malloc(11);
    if (!str)
        return NULL;

    str[0] = (perms & S_IRUSR) ? 'r' : '-';
    str[1] = (perms & S_IWUSR) ? 'w' : '-';
    str[2] = (perms & S_IXUSR) ? 'x' : '-';
    str[3] = (perms & S_IRGRP) ? 'r' : '-';
    str[4] = (perms & S_IWGRP) ? 'w' : '-';
    str[5] = (perms & S_IXGRP) ? 'x' : '-';
    str[6] = (perms & S_IROTH) ? 'r' : '-';
    str[7] = (perms & S_IWOTH) ? 'w' : '-';
    str[8] = (perms & S_IXOTH) ? 'x' : '-';
    str[9] = '\0';

    return str;
}

char *format_time(time_t t)
{
    char *raw = ctime(&t);
    if (!raw)
        return NULL;
        
    char *formatted = malloc(20);
    if (!formatted)
        return NULL;
    
    if (ft_strlen(raw) < 24) {
        ft_strlcpy(formatted, "Unknown", 20);
        return formatted;
    }
    char month[4] = {0};
    char day[3] = {0};
    char time_part[6] = {0};
    
    for (int i = 0; i < 3; i++) {
        month[i] = raw[4 + i];
    }
    for (int i = 0; i < 2; i++) {
        day[i] = raw[8 + i];
    }
    for (int i = 0; i < 5; i++) {
        time_part[i] = raw[11 + i];
    }
    char *day_ptr = day;
    if (day[0] == ' ') {
        day_ptr = day + 1;
    }
    ft_strlcpy(formatted, month, 20);
    ft_strlcat(formatted, " ", 20);
    ft_strlcat(formatted, day_ptr, 20);
    ft_strlcat(formatted, " ", 20);
    ft_strlcat(formatted, time_part, 20);
    
    return formatted;
}

char *format_number_right(int num, int width)
{
    char *result = malloc(width + 1);    
    char *num_str = ft_itoa(num);
    int len = ft_strlen(num_str);
    int spaces = width - len;
    int i, j = 0;
    
    while (i < spaces) {
        result[i] = ' ';
        i++;
    }    
    while (j < len) {
        result[i + j] = num_str[j];
        j++;
    }
    
    result[width] = '\0';
    free(num_str);
    return result;
}

char *format_string_left(char *str, int width)
{
    char *result = malloc(width + 1);
    if (!result)
        return NULL;
    
    int len = ft_strlen(str);    
    int i = 0;
    while (i < len && i < width) {
        result[i] = str[i];
        i++;
    }
    while (i < width) {
        result[i] = ' ';
        i++;
    }
    result[width] = '\0';
    return result;
}