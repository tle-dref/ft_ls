#pragma once
 

# include "libft.h"
# include <stdbool.h>



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

t_ls *parsing(char **args);
