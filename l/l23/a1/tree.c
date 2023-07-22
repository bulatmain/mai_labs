#include "tree.h"

b_tree create_empty_b_tree() {
    b_tree t;
    t.n = 0;
    t.last_id = -1;
    t.d = create_empty_imap();
    t.c = create_empty_imap();
    t.root = NULL;
    return t;
}

int8_t is_empty_b_tree(b_tree* const t) {
    return t->root == NULL;
}

void copy_b_tree(b_tree* t, b_tree* const t_) {
    clear_tree(t);
    __copy_b_subtree(&(t->root), t_->root, NULL);
    t->n = t_->n;
    t->last_id = t_->last_id;
}

void copy_b_tree_n_update(b_tree* t, b_tree* const t_) {
    copy_b_tree(t, t_);
    imap_copy(&(t->d), &(t_->d));
    imap_copy(&(t->c), &(t_->c));
}

void __copy_b_subtree(node** root, node* const to_copy, node* const acc) {
    if (to_copy == NULL)
        return;
    *root = new_node(to_copy->id, to_copy->data, acc);
    __copy_b_subtree(&(*root)->left, to_copy->left, *root);
    __copy_b_subtree(&(*root)->right, to_copy->right, *root);
}

void init_b_tree(b_tree* t, int64_t data) {
    if (!is_empty_b_tree(t)) {
        exception_exit("Initializing not empty b_tree");
    }
    t->root = new_node(0, data, NULL);
    t->last_id = 0;
    t->n = 1;
    t->d = create_empty_imap();
    imap_add(&(t->d), 0, 0);
    imap_copy(&(t->c), &(t->d));
}

void add_node_by_id (b_tree* t, int64_t id, int64_t data, SIDE s) {
    // Create new node
    if (is_empty_b_tree(t)) {
        init_b_tree(t, data);
    }
    node* v = node_by_id(t->root, id);
    if (v == NULL) {
        exception_exit("There is no node in tree %p with id %ld\n", t, id);
    }
    add_node_by_node(t, v, data, s);
}

void add_node_by_node(b_tree* t, node* p, int64_t data, SIDE s) {
    if (p == NULL) {
        exception_exit("Trying to access NULL node pointer\n");
    }
    node* v = p;
    // Add new node to ancestor with given id
    if (s == N_LEFT) {
        if (v->left == NULL) {
            v->left = new_node(t->last_id + 1, data, v);
        } else {
            exception_exit("%ld node already has left child - %ld node", v->id, v->left->id);
        }
    } else if (s == N_RIGHT) {
        if (v->right == NULL) {
            v->right = new_node(t->last_id + 1, data, v);
        }
        else {
            exception_exit("%ld node already has right child - %ld node", v->id, v->right->id);
        }
    } else {
        exception_exit("Root is already exist: add_node_by_node(%p, %p, %ld, N_ROOT), p id: %ld\n", t, p, data, p->id); 
    }
    t->last_id++;
    t->n++;
}

node* node_by_id(node* const root, int64_t id) {
    node* const i = root;
    if (i == NULL || i->id == id) {
        return i;
    }
    node* l = i->left,* r = i->right;
    l = node_by_id(l, id);
    r = node_by_id(r, id);
    if (l != NULL) {
        return l;
    }
    if (r != NULL) {
        return r;
    }
    return NULL;
}

node* new_node(const int64_t id, int64_t data, node* const p) {
    node* v = (node*)malloc(sizeof(node));
    v->id= id;
    v->data = data;
    v->left = NULL;
    v->right = NULL;
    v->p = p;
    return v;
}

void copy_node(node* v, node* const v_) {
    v->data = v_->data;
    v->id = v_->id;
    v->left = NULL;
    v->right = NULL;
}

void delete_node_by_id(b_tree* t, const int64_t id) {
    node* v = node_by_id(t->root, id);
    delete_node_by_node(t, v);
}

void delete_node_by_node(b_tree* t, node* v) {
    if (v == NULL) {
        return;
    }
    node* i = v;

    node* l = v->left,* r = v->right;

    if (!(l == NULL)) {
        delete_node_by_node(t, l);
    }
    if (!(r == NULL)) {
        delete_node_by_node(t, r);
    }
    if (!(i->p == NULL)) {
        if (i->p->left == i) {
            i->p->left = NULL;
        } else {
            i->p->right = NULL;
        }
    }
    free(i);
    --t->last_id;
    --t->n;
    if (t->n == 0) {
        t->root = NULL;
    }
}

int64_t data_by_id(b_tree* const t, int64_t id) {
    node* v = node_by_id(t->root, id);
    if (v == NULL) {
        exception_exit("There is no node with given id in the b_tree\n")
    }
    return v->data;
}

int8_t node_is_in_b_tree(node* const root, node* const v) {
    if (v == NULL || root == NULL) {
        return 0;
    }
    if (root == v) {
        return 1;
    }
    return node_is_in_b_tree(root->left, v) \
        || node_is_in_b_tree(root->right, v);
}

void clear_tree(b_tree* t) {
    if (!is_empty_b_tree(t)) {
        delete_node_by_node(t, t->root);
        clear_imap(&(t->d));
        clear_imap(&(t->c));
    } else {
        t->n = 0;
        t->last_id = -1;
        t->c = create_empty_imap();
        t->d = create_empty_imap();
        t->root = NULL;
    }
}

void update_depth(b_tree* t) {
    node* i = t->root;
    if (i == NULL) {
        clear_tree(t);
        return;
    }
    clear_imap(&(t->d));
    __update_depth(&(t->d), t->root, 0);
}

void __update_depth(imap* d, node* const root, int64_t depth) {
    if (root == NULL) {
        return;
    }
    imap_add(d, root->id, depth);
    __update_depth(d, root->left, depth + 1);
    __update_depth(d, root->right, depth + 1);
}

void update_degree(b_tree* t) {
    node* i = t->root;
    if (i == NULL) {
        clear_tree(t);
        return;
    }
    clear_imap(&(t->c));
    __update_degree(&(t->c), t->root);
}

void __update_degree(imap* c, node* const root) {
    if (root == NULL) {
        return;
    }
    int8_t c_ = 0;
    if (!(root->left == NULL)) {
        ++c_;
        __update_degree(c, root->left);
    }
    if (!(root->right == NULL)) {
        ++c_;
        __update_degree(c, root->right);
    }    
    imap_add(c, root->id, c_);
}

void print_b_tree(node* const root, uint8_t tab, SIDE side) {
    if (root == NULL) {
        return;
    }
    printf("%ld", root->id);
    for (uint8_t i = 0; i <= tab; ++i) {
        printf("\t");
        printf("|");
    }
    if (side == N_LEFT) {
        printf("L: ");
    } else if (side == N_RIGHT) {
        printf("R: ");
    } else {
        printf("Root: ");
    }
    printf("%ld\n", root->data);
    print_b_tree(root->left, tab + 1, N_LEFT);
    print_b_tree(root->right, tab + 1, N_RIGHT);
    //printf("\n");
}

int8_t is_B_tree(b_tree* const t) {
    if (is_empty_b_tree(t)) {
        return 1;
    }
    b_tree t_ = create_empty_b_tree();
    copy_b_tree(&t_, t);
    update_degree(&t_);
    imap_item* iterator = t_.c.begin;
    while (! (iterator == t_.c.back)) {
        if (iterator->value % 2) {
            return 0;
        }
        iterator = iterator->next;
    }
    if (iterator->value % 2) {
        return 0;
    }
    return 1;
}