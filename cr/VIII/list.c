#include "list.h"

string make_string(char* p) {
    string s;
    s.p = p;
    s.l = 0;
    while (s.p[s.l] != '\0') {
        ++s.l;
    }
    return s;
}

bool string_eq(string a, string b) {
    if (a.l != b.l) {
        return false;
    }
    size_t i = 0;
    while (i < a.l) {
        if (a.p[i] != b.p[i]) {
            return false;
        }
        ++i;
    }
    return true;
}

node* make_node(string s) {
    node* p = (node*)malloc(sizeof(node));
    p->r = NULL;
    p->s = s;
    return p;
}

void link_node(node* l, node* m, node* r) {
    if (! (l == NULL))
        l->r = m;
    m->r = r;
}

void link_nodes(node* l, node* r) {
    if (! (l == NULL)) {
        l->r = r;
    }
}

void node_free(node* p) {
    free(p->s.p);
    p->s.p = NULL;
    p->s.l = 0;
    p->r = NULL;
}

void list_create_empty(list* l) {
    l->begin = l->back = NULL;
    l->size = 0;
}

void list_add(list* l, string s, size_t index) {
    node* _p = make_node(s);
    if (index < l->size) {
        node* p = l->begin;
        for (size_t i = 0; i < index; ++i) {
            p = p->r;
        }
        link_node(p, _p, p->r);
    } else {
        if (l->back == NULL) {
            l->begin = l->back = _p;
            l->size = 0;
        } else {
            link_node(l->back, _p, NULL);
            l->back = _p;
        }
    }
    l->size++;
}

node* list_find_node(list* l, string s) {
    node* p = l->begin;
    while (! (p == NULL) && ! string_eq(p->s, s)) {
        p = p->r;
    }
    return p;
}

void list_remove(list* l, string s) {
    if (! (l->begin == NULL) && string_eq(l->begin->s, s)) {
        node* r = l->begin;
        l->begin = l->begin->r;
        if (l->begin == NULL) {
            l->back = NULL;
        } else {
            link_nodes(l->begin, l->begin->r->r);
        }
        node_free(r);
        l->size--;
    }
    else {
        node* p = l->begin;
        while (!(p->r == NULL) && ! string_eq(p->r->s, s)) {
            p = p->r;
        }
        if (! (p->r == NULL)) {
            node* r = p->r;
            link_nodes(p, p->r->r);
            node_free(r);
            l->size--;
        }
    }
}

void list_remove_subtree(list* l, node* s, node* e) {
    node* p = l->begin,* r;
    if (p != s) {
        while (! (p->r == NULL) && ! (p->r == s)) {
            p = p->r;
        }
        if (p->r == NULL) {
            return;
        }
        if (e == NULL) {
            l->back = p;
        }
        r = p->r;
        p->r = e;
        p = r;
    } else {
        l->begin = e;
    }
    while (! (p->r == NULL) && ! (p->r == e)) {
        r = p;
        p = p->r;
        node_free(r);
        --l->size;
    }
    node_free(p);
    --l->size;
}

void list_free(list* l) {
    if (l->begin == NULL) {
        l->back = NULL;
        l->size = 0;
        return;
    }
    node* p = l->begin,* r;
    while (! (p->r == NULL)) {
        r = p;
        p = p->r;
        node_free(r);
    }
    node_free(p);
    l->begin = l->back = NULL;
    l->size = 0;
}

size_t list_len(node* l, node* r) {
    if (l == NULL || r == NULL) {
        return 0;
    }
    node* p = l;
    size_t len = 1;
    while (! (p == r)) {
        p = p->r; ++len;
    }
    return len;
}

void list_print(list* l) {
    printf("{");
    if (! (l->begin == NULL)) {
        node* p = l->begin;
        while (! (p->r == NULL)) {
            printf(" %s,", p->s.p);
            p = p->r;
        }
        printf(" %s ", p->s.p);
    }
    printf("}");
}

void list_cut(list* l, char side, string s) {
    node* p = list_find_node(l, s);
    if (p == NULL) {
        return;
    }
    if (side == 'l' && ! (p == l->begin)) {
        list_remove_subtree(l, l->begin, p);
    } else if (side == 'r' && ! (p->r == NULL)) {
        list_remove_subtree(l, p->r, NULL);
    }
}