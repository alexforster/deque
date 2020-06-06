/* deque.h
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

#pragma once

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/// Represents a double-ended queue of a certain fixed size.
///
struct deque;

/// Returns a pointer to a <tt>struct deque</tt> with a capacity of
/// `nr_entries`, where each entry is `sz_entry` bytes. If either arguments are
/// zero, or if <tt>malloc</tt> fails, then <tt>NULL</tt> is returned instead.
///
/// The user owns the returned pointer and is expected to call <tt>free</tt>
/// once it is no longer needed.
///
struct deque *deque_new(size_t nr_entries, size_t sz_entry);

/// Indicates whether `deque` is empty.
///
bool deque_empty(struct deque *deque);

/// Indicates whether `deque` is full.
///
bool deque_full(struct deque *deque);

/// Returns the current number of entries in `deque`.
///
size_t deque_size(struct deque *deque);

/// Returns the total number of entries that `deque` can hold.
///
size_t deque_capacity(struct deque *deque);

/// Returns a pointer to the entry at the front of `deque`, or <tt>NULL</tt>
/// if `deque` is empty.
///
/// The returned pointer is owned by `deque` and becomes invalid the next
/// time `deque` is mutated.
///
void *deque_front(struct deque *deque);

/// Returns a pointer to the entry at the back of `deque`, or <tt>NULL</tt>
/// if `deque` is empty.
///
/// The returned pointer is owned by `deque` and becomes invalid the next
/// time `deque` is mutated.
///
void *deque_back(struct deque *deque);

/// Appends a byte-wise copy of the contents of `entry` to the front of `deque`,
/// returning `entry` if `deque` is full, or <tt>NULL</tt> otherwise.
///
void *deque_push_front(struct deque *deque, void *entry);

/// Appends a byte-wise copy of the contents of `entry` to the back of `deque`,
/// returning `entry` if `deque` is full, or <tt>NULL</tt> otherwise.
///
void *deque_push_back(struct deque *deque, void *entry);

/// Removes an entry from the front of `deque` and returns a pointer to it,
/// or <tt>NULL</tt> if `deque` is empty.
///
/// The returned pointer is owned by `deque` and becomes invalid the next
/// time `deque` is mutated.
///
void *deque_pop_front(struct deque *deque);

/// Removes an entry from the back of `deque` and returns a pointer to it,
/// or <tt>NULL</tt> if `deque` is empty.
///
/// The returned pointer is owned by `deque` and becomes invalid the next
/// time `deque` is mutated.
///
void *deque_pop_back(struct deque *deque);

#ifdef __cplusplus
}
#endif
