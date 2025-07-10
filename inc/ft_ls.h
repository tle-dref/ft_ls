#pragma once
 
# include <sys/stat.h>
# include "libft.h"
# include <stdbool.h>
# include <dirent.h>

typedef struct s_listls {
    char *name;
    char *path;           // full path of the file or directory
    struct stat stat;           // name of the file or directory
    struct s_listls *next; // pointer to the next element in the list
} t_listls;


// typedef struct s_flags{
    //     bool l; // use a long listing format | print
    //     bool a; // hidden files || print
    //     bool Z; // add security context | print
    //     bool f; // brut, -aU enabled | print
    //     bool g; // -l but without the owner | print prioritary on -l
    //     bool F; // add / to dir, @to link, etc | print
    //     bool t; // time sort | sort
    //     bool r; // reverse | sort not order affected
    //     bool U; // not sorted | sort order affected
    //     bool R; // recursive | calculus
    //     bool d; // specify a directory | calculus
    //     bool colors;
    // } t_flags;

typedef struct s_flags {
    bool l, a, Z, f, g, F, t, r, U, R, d, colors;
} t_flags;
    
typedef struct s_flag_map {
    char c;
    bool *field;
} t_flag_map;

typedef struct s_ls{
    char *dir;
    t_flags *flags;
}t_ls;
// last sort flag is the one done, r is not undoable if he's here he stays


#define MUTEX_FLAGS(flag1, flag2, current, flags) \
    do { \
        if (current == flag1) flags->t = false; \
        else if (current == flag2) flags->U = false; \
    } while(0)

t_ls *parsing(char **args);
