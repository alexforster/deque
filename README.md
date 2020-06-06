# deque

A simple fixed-size deque implementation in C.

**Author:** Alex Forster \<alex@alexforster.com\><br/>
**License:** Apache-2.0

## Getting Started

Copy these files into the source of your application:

 * `src/deque.c` – implementation
 * `src/deque.h` – interface

Alternatively, a library can be built using the `cmake` build system.

## Documentation

An API overview is below. See `src/deque.h` for complete documentation of each
function. The tests in `tests/deque.cpp` contain various examples.

```c
struct deque;

struct deque *deque_new(size_t nr_entries, size_t sz_entry);

bool deque_empty(struct deque *deque);
bool deque_full(struct deque *deque);

size_t deque_size(struct deque *deque);
size_t deque_capacity(struct deque *deque);

void *deque_front(struct deque *deque);
void *deque_back(struct deque *deque);

void *deque_push_front(struct deque *deque, void *entry);
void *deque_push_back(struct deque *deque, void *entry);
void *deque_pop_front(struct deque *deque);
void *deque_pop_back(struct deque *deque);
```
