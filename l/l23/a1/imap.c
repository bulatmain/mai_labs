#include "imap.h"

imap_item* new_imap_item(int64_t id, int64_t value, imap_item* const prev, imap_item* const next) {
    imap_item* i = (imap_item*)malloc(sizeof(imap_item));
    i->id = id; i->value = value;
    i->prev = prev;
    i->next = next;
}

imap_item* copy_imap_item(imap_item* const i) {
    return new_imap_item(i->id, i->value, i->prev, i->next);
}

imap create_empty_imap() {
    imap m;
    m.begin = NULL;
    m.back = NULL;
    m.size = 0;
    return m;
}

int64_t imap_at(imap* const m, int64_t id) {
    imap_item* iterator = m->begin;
    if (iterator == NULL) {
        exception_exit("imap at address \'%p\' begin is NULL\n", m);
    }
    while (!(iterator == NULL) && iterator->id != id) {
        iterator = iterator->next;
    }
    if (!(iterator == NULL) && iterator->id == id) {
        return iterator->value;
    }
    exception_exit("No element with id \'%ld\' in imap at address \'%p\'\n", id, m);
}

void imap_add(imap* m, int64_t id, int64_t value) {
    imap_item* i = new_imap_item(id, value, NULL, NULL);
    if (is_empty_imap(m)) {
        m->begin = m->back = i;
        m->size = 1;
        return;
    }
    if (m->begin == NULL) {
        exception_exit("Bad imap: imap at address \'%p\' begin is NULL\n", m);
    }
    if (m->back == NULL) {
        exception_exit("Bad imap: imap at address \'%p\' back is NULL\n", m);
    }
    imap_item* iterator = m->begin;
    while(!(iterator->next == NULL) && (i->id > iterator->id)) {
        iterator = iterator->next;
    }
    if (iterator->next == NULL) {
        i->prev = iterator;
        iterator->next = i;
        m->back = i;
    }
    else {
        iterator = iterator->next;
        if (iterator == m->begin) {
            i->next = iterator;
            iterator->prev = i;
            m->begin = i;
        } else {
            i->next = iterator;
            i->prev = iterator->prev;
            iterator->prev->next = i;
            iterator->prev = i;
        }
    }
    ++m->size;
}

void imap_remove_by_id(imap* m, int64_t id) {
    imap_item* iterator = m->begin;
    while (!(iterator == NULL) && iterator->id != id) {
        iterator = iterator->next;
    }
    if (!(iterator == NULL) && iterator->id == id) {
        if (iterator->prev == NULL) {
            m->begin = iterator->next;
        } else {
            iterator->prev->next = iterator->next;
        }
        if (iterator->next == NULL) {
            m->back = iterator->prev;
        } else {
            iterator->next->prev = iterator->prev;
        }
        free(iterator);
    }
}

void imap_remove_by_value(imap* m, int64_t value) {
    imap_item* iterator = m->begin;
    while (!(iterator == NULL) && iterator->value != value) {
        iterator = iterator->next;
    }
    if (!(iterator == NULL) && iterator->value == value) {
        if (iterator->prev == NULL) {
            m->begin = iterator->next;
        } else {
            iterator->prev->next = iterator->next;
        }
        if (iterator->next == NULL) {
            m->back = iterator->prev;
        } else {
            iterator->next->prev = iterator->prev;
        }
        free(iterator);
    }
}

void imap_copy(imap* m, imap* const m_) {
    if (!is_valid_edges(m)) {
        exception_exit("imap \'%p\' is not valid to copy\n", m);
    }
    clear_imap(m);
    imap_item* iterator_ = m_->begin;
    m->begin = copy_imap_item(iterator_);
    iterator_ = iterator_->next;
    imap_item* iterator  = m->begin;
    while (!(iterator_ == NULL)) {
        iterator->next = copy_imap_item(iterator_);
        iterator = iterator->next;
    }
    m->back = iterator;
}

int8_t is_valid_edges(imap* const m) {
    return !((m->begin == NULL || m->back == NULL) && !(m->begin == m->back));
}

int8_t is_valid(imap* const m) {
    if (!is_valid_edges(m)) {
        return 0;
    }
    int8_t res = 1;
    imap_item* iterator = m->begin;
    while (!(iterator == NULL) && iterator != m->back) {
        iterator = iterator->next;
    }
    return iterator == m->back;
}

void make_valid(imap* m) {
    imap_item* l = m->begin,* r = m->back;
    uint64_t count = 0;
    while (!(l == NULL) && !(l->next == NULL)) {
        l = l->next;
        ++count;
    }
    while (!(r == NULL) && !(r->prev == NULL)) {
        r = r->prev;
        ++count;
    }
    if (l == NULL || r == NULL) {
        if (l == NULL) {
            m->begin = r;
        }
        if (r == NULL) {
            m->back = l;
        }
    } else {
        l->next = r;
        r->prev = l;
    }
    m->size = count;
}

void clear_imap(imap* m) {
    imap_item* iterator = m->begin;
    if (iterator == NULL) {
        iterator = m->back;
        if (iterator == NULL) {
            m->size = 0;
            return;
        } else {
            while (!(iterator->prev == NULL)) {
                iterator = iterator->prev;
                free(iterator->next);
            }
        }
    } else {
        while (!(iterator->next == NULL)) {
            iterator = iterator->next;
            free(iterator->prev);
        }
    }
    free(iterator);
    m->begin = m->back = NULL;
    m->size = 0;
}

int8_t is_empty_imap(imap* const m) {
    return m->begin == m->back && m->back == NULL;
}