#ifndef MORSE_SEARCH_TREE_H
#define MORSE_SEARCH_TREE_H

struct morse_search_tree 
{
    char found_char;
    struct morse_search_tree *dot;
    struct morse_search_tree *dash;
};

struct morse_search_tree *get_morse_tree();

#endif