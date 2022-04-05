#include "morse_search_tree.h"

struct morse_search_tree thole = {'\0', &thole, &thole};

struct morse_search_tree t5 = {'5', &thole, &thole},
    t4 = {'4', &thole, &thole}, t3 = {'3', &thole, &thole}, tqm = {'?', &thole, &thole},
    tund = {'_', &thole, &thole}, t2 = {'2', &thole, &thole}, tdq = {'\"', &thole, &thole},
    tdot = {'.', &thole, &thole}, tat = {'@', &thole, &thole}, tap = {'\'', &thole, &thole},
    tmin = {'-', &thole, &thole}, teq = {'=', &thole, &thole}, tsl = {'/', &thole, &thole},
    tpa = {';', &thole, &thole}, texcl = {'!', &thole, &thole}, tcp = {')', &thole, &thole},
    t7 = {'7', &thole, &thole}, tcm = {',', &thole, &thole}, tq = {'Q', &thole, &thole},
    tdd = {':', &thole, &thole}, t9 = {'9', &thole, &thole}, t0 = {'0', &thole, &thole};

struct morse_search_tree tdj = {'\0', &tqm, &tund}, tee = {'\0', &tdq, &thole},
    tpl = {'+', &thole, &tdot}, taa = {'\0', &tat, &thole}, t1 = {'1', &tap, &thole},
    t6 = {'6', &thole, &tmin}, tnull1 = {'\0', &tpa, &texcl}, top = {'(', &thole, &tcp},
    tnull2 = {'\0', &thole, &tcm}, t8 = {'8', &tdd, &thole};

struct morse_search_tree th = {'H', &t5, &t4}, tv = {'V', &thole, &t3},
    tf = {'F', &thole, &thole}, tuuml = {'\0', &tdj, &t2}, tl = {'L', &thole, &tee}, 
    tauml = {'\0', &tpl, &thole}, tp = {'P', &thole, &taa}, tj = {'J', &thole, &t1},
    tb = {'B', &t6, &teq}, tx = {'X', &tsl, &thole}, tc = {'C', &thole, &tnull1},
    ty = {'Y', &top, &thole}, tz = {'Z', &t7, &tnull2}, touml = {'\0', &t8, &thole},
    tch = {'\0', &t9, &t0};

struct morse_search_tree ts = {'S', &th, &tv}, tu = {'U', &tf, &tuuml},
    tr = {'R', &tl, &tauml}, tw = {'W', &tp, &tj}, td = {'D', &tb, &tx},
    tk = {'K', &tc, &ty}, tg = {'G', &tz, &tq}, to = {'O', &touml, &tch};

struct morse_search_tree ti = {'I', &ts, &tu}, ta = {'A', &tr, &tw},
    tn = {'N', &td, &tk}, tm = {'M', &tg, &to};

struct morse_search_tree te = {'E', &ti, &ta}, tt = {'T', &tn, &tm};

struct morse_search_tree morse_tree = {'\0', &te, &tt};

struct morse_search_tree *get_morse_tree()
{
    return &morse_tree;
}