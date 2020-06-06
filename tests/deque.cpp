#include "catch.hpp"
#include "deque.h"

#include <vector>

#define NR_ENTRIES 8

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

TEST_CASE("symmetric operations behavior", "[deque]")
{
    size_t i;
    struct deque *deque = deque_new(NR_ENTRIES, sizeof(size_t));

    for(i = 1; i <= NR_ENTRIES; i++)
    {
        CAPTURE(i);
        REQUIRE(deque_push_front(deque, &i) == nullptr);
        REQUIRE(*(size_t *)deque_front(deque) == i);
        REQUIRE(deque_size(deque) == i);
    }

    REQUIRE(deque_size(deque) == NR_ENTRIES);
    REQUIRE(deque_full(deque));
    REQUIRE(deque_push_front(deque, &i) == &i);

    for(i = 1; i <= NR_ENTRIES; i++)
    {
        CAPTURE(i);
        REQUIRE(*(size_t *)deque_back(deque) == i);
        REQUIRE(*(size_t *)deque_pop_back(deque) == i);
        REQUIRE(deque_size(deque) == (NR_ENTRIES - i));
    }

    REQUIRE(deque_size(deque) == 0);
    REQUIRE(deque_empty(deque));
    REQUIRE(deque_pop_back(deque) == nullptr);

    for(i = 1; i <= NR_ENTRIES; i++)
    {
        CAPTURE(i);
        REQUIRE(deque_push_back(deque, &i) == nullptr);
        REQUIRE(*(size_t *)deque_back(deque) == i);
        REQUIRE(deque_size(deque) == i);
    }

    REQUIRE(deque_size(deque) == NR_ENTRIES);
    REQUIRE(deque_full(deque));
    REQUIRE(deque_push_back(deque, &i) == &i);

    for(i = 1; i <= NR_ENTRIES; i++)
    {
        CAPTURE(i);
        REQUIRE(*(size_t *)deque_front(deque) == i);
        REQUIRE(*(size_t *)deque_pop_front(deque) == i);
        REQUIRE(deque_size(deque) == (NR_ENTRIES - i));
    }

    REQUIRE(deque_size(deque) == 0);
    REQUIRE(deque_empty(deque));
    REQUIRE(deque_pop_front(deque) == nullptr);

    free(deque);
    deque = NULL;
}

TEST_CASE("wrapping behavior", "[deque]")
{
    size_t i;
    struct deque *deque = deque_new(NR_ENTRIES, sizeof(size_t));

    for(i = 1; i <= NR_ENTRIES; i++)
    {
        REQUIRE(deque_push_front(deque, &i) == nullptr);
    }

    REQUIRE_THAT(std::vector<size_t>((size_t *)deque->slots, (size_t *)deque->slots + NR_ENTRIES), Catch::Equals(std::vector<size_t> { 1, 2, 3, 4, 5, 6, 7, 8 }));

    REQUIRE(*(size_t *)deque_pop_back(deque) == 1);
    REQUIRE(deque->tail == 1);
    REQUIRE(*(size_t *)deque_pop_front(deque) == 8);
    REQUIRE(deque->head == 7);

    i = 8;
    REQUIRE(deque_push_back(deque, &i) == nullptr);
    REQUIRE(deque->tail == 0);
    i = 1;
    REQUIRE(deque_push_back(deque, &i) == nullptr);
    REQUIRE(deque->tail == SIZE_T_MAX);

    REQUIRE_THAT(std::vector<size_t>((size_t *)deque->slots, (size_t *)deque->slots + NR_ENTRIES), Catch::Equals(std::vector<size_t> { 8, 2, 3, 4, 5, 6, 7, 1 }));

    for(i = 16; i <= (16 + NR_ENTRIES - 1); i++)
    {
        REQUIRE(deque_pop_back(deque) != nullptr);
        REQUIRE(deque_push_front(deque, &i) == nullptr);
    }

    REQUIRE_THAT(std::vector<size_t>((size_t *)deque->slots, (size_t *)deque->slots + NR_ENTRIES), Catch::Equals(std::vector<size_t> { 17, 18, 19, 20, 21, 22, 23, 16 }));
    REQUIRE(deque->head == 15);
    REQUIRE(deque->tail == 7);

    free(deque);
    deque = NULL;
}

TEST_CASE("empty queue behavior", "[deque]")
{
    size_t i;
    struct deque *deque = deque_new(NR_ENTRIES, sizeof(size_t));

    REQUIRE(deque_size(deque) == 0);
    REQUIRE(deque_empty(deque));
    REQUIRE(!deque_full(deque));
    REQUIRE(deque_front(deque) == nullptr);
    REQUIRE(deque_pop_front(deque) == nullptr);
    REQUIRE(deque_back(deque) == nullptr);
    REQUIRE(deque_pop_back(deque) == nullptr);

    free(deque);
    deque = NULL;
}

TEST_CASE("full queue behavior", "[deque]")
{
    size_t i;
    struct deque *deque = deque_new(NR_ENTRIES, sizeof(size_t));

    i = 42;
    while(!deque_full(deque))
    {
        REQUIRE(deque_push_front(deque, &i) == nullptr);
    }
    REQUIRE(deque_push_front(deque, &i) == &i);

    free(deque);
    deque = NULL;
}

TEST_CASE("single entry queue behavior", "[deque]")
{
    size_t i;
    struct deque *deque = deque_new(NR_ENTRIES, sizeof(size_t));

    i = 42;
    REQUIRE(deque_push_back(deque, &i) == nullptr);
    REQUIRE(deque_front(deque) == deque_back(deque));
    REQUIRE(*(size_t *)deque_front(deque) == *(size_t *)deque_back(deque));
    REQUIRE(*(size_t *)deque_pop_back(deque) == i);
    REQUIRE(deque_push_front(deque, &i) == nullptr);
    REQUIRE(deque_front(deque) == deque_back(deque));
    REQUIRE(*(size_t *)deque_front(deque) == *(size_t *)deque_back(deque));
    REQUIRE(*(size_t *)deque_pop_front(deque) == i);

    free(deque);
    deque = NULL;
}

TEST_CASE("pushing to full queue returns input", "[deque]")
{
    size_t i;
    struct deque *deque = deque_new(NR_ENTRIES, sizeof(size_t));

    while(!deque_full(deque))
    {
        deque_push_front(deque, &i);
    }

    REQUIRE(deque_push_front(deque, &i) == &i);

    free(deque);
    deque = NULL;
}

TEST_CASE("construction behavior", "[deque]")
{
    struct deque *deque;

    deque = deque_new(0, sizeof(size_t));
    REQUIRE(deque == nullptr);

    deque = deque_new(NR_ENTRIES, 0);
    REQUIRE(deque == nullptr);

    deque = deque_new(NR_ENTRIES - 1, sizeof(size_t));
    REQUIRE(deque_capacity(deque) == NR_ENTRIES - 1);

    free(deque);
    deque = NULL;
}
