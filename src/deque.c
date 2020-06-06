/* deque.c
 * Copyright (c) 2020 Alex Forster
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "deque.h"

#include <stdlib.h>
#include <string.h>

struct deque
{
    size_t nr_entries;
    size_t sz_entry;
    void *slots;
    size_t nr_slots;
    size_t head;
    size_t tail;
    size_t mask;
    size_t __padding[1];
};

static inline size_t next_power_of_two(size_t n)
{
    return n == 1 ? 1 : 1 << (64 - __builtin_clzll(n - 1));
}

struct deque *deque_new(size_t nr_entries, size_t sz_entry)
{
    if(nr_entries == 0 || sz_entry == 0)
    {
        return NULL;
    }

    size_t nr_slots = next_power_of_two(nr_entries);

    size_t sz_alloc = sizeof(struct deque) + (nr_slots * sz_entry);
    struct deque *deque = malloc(sz_alloc);

    if(deque == NULL)
    {
        return NULL;
    }

    deque->nr_entries = nr_entries;
    deque->sz_entry = sz_entry;

    deque->slots = (char *)deque + sizeof(struct deque);
    deque->nr_slots = nr_slots;

    deque->head = 0;
    deque->tail = 0;
    deque->mask = deque->nr_slots - 1;

    return deque;
}

bool deque_empty(struct deque *deque)
{
    return deque->head == deque->tail;
}

bool deque_full(struct deque *deque)
{
    return deque_size(deque) == deque_capacity(deque);
}

size_t deque_size(struct deque *deque)
{
    return deque->head - deque->tail;
}

size_t deque_capacity(struct deque *deque)
{
    return deque->nr_entries;
}

void *deque_front(struct deque *deque)
{
    if(deque_empty(deque))
    {
        return NULL;
    }

    return (char *)deque->slots + (((deque->head - 1) & deque->mask) * deque->sz_entry);
}

void *deque_back(struct deque *deque)
{
    if(deque_empty(deque))
    {
        return NULL;
    }

    return (char *)deque->slots + ((deque->tail & deque->mask) * deque->sz_entry);
}

void *deque_push_front(struct deque *deque, void *entry)
{
    if(deque_full(deque))
    {
        return entry;
    }

    void *slot = (char *)deque->slots + ((deque->head++ & deque->mask) * deque->sz_entry);
    memcpy((char *)slot, (char *)entry, deque->sz_entry);

    return NULL;
}

void *deque_push_back(struct deque *deque, void *entry)
{
    if(deque_full(deque))
    {
        return entry;
    }

    void *slot = (char *)deque->slots + ((--deque->tail & deque->mask) * deque->sz_entry);
    memcpy((char *)slot, (char *)entry, deque->sz_entry);

    return NULL;
}

void *deque_pop_front(struct deque *deque)
{
    if(deque_empty(deque))
    {
        return NULL;
    }

    void *slot = (char *)deque->slots + ((--deque->head & deque->mask) * deque->sz_entry);

    return slot;
}

void *deque_pop_back(struct deque *deque)
{
    if(deque_empty(deque))
    {
        return NULL;
    }

    void *slot = (char *)deque->slots + ((deque->tail++ & deque->mask) * deque->sz_entry);

    return slot;
}
