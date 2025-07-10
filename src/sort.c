#include "ft_ls.h"

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


t_listls *merge(t_listls *a, t_listls *b) {
    if (!a) return b;
    if (!b) return a;

    if (ft_strcasecmp(a->name, b->name) <= 0) {
        a->next = merge(a->next, b);
        return a;
    } else {
        b->next = merge(a, b->next);
        return b;
    }
}

void split(t_listls *source, t_listls **front, t_listls **back) {
    t_listls *slow = source;
    t_listls *fast = source->next;

    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

t_listls *sort_alphabet(t_listls *list) {
    if (!list || !list->next)
        return list;

    t_listls *left;
    t_listls *right;

    split(list, &left, &right);

    left = sort_alphabet(left);
    right = sort_alphabet(right);

    return merge(left, right);
}