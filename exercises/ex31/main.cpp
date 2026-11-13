#include <iostream>
#include <list>
#include <vector>

#define NUM_PAGES 0x280000
#define MAX_ORDER 10

struct page {
    int order = -1; //order of located block 
    int refcount = 0; //0: waiting for allocated, 1: allocated
};

std::vector<page> mem_map;
std::vector<std::list<page*>> free_area;

struct page* get_buddy(struct page* page, unsigned int order) {
    return &mem_map[(page - mem_map.data()) ^ (1 << order)];
}

struct page* alloc_pages(unsigned int order) {
    if (order > MAX_ORDER) return nullptr;

    unsigned int current_order = order;

    while (current_order <= MAX_ORDER && free_area[current_order].empty()) {
        current_order++;
    }

    if (current_order > MAX_ORDER) return nullptr;

    struct page* block = free_area[current_order].front();
    free_area[current_order].pop_front();

    while (current_order > order) {
        current_order--;

        struct page* buddy = get_buddy(block, current_order);
        buddy->order = current_order;
        buddy->refcount = 0;

        free_area[current_order].push_back(buddy);
    }
    block->order = order;
    block->refcount = 1;

    return block;
}

void free_pages(struct page* page) {

    struct page* curr_block = page;
    curr_block->refcount = 0;
    unsigned int curr_order = curr_block->order;

    while (curr_order < MAX_ORDER) {
        struct page* buddy = get_buddy(curr_block, curr_order);
        
        if (buddy->refcount != 0) break;
        if ((unsigned int)buddy->order != curr_order) break;

        free_area[curr_order].remove(buddy);

        unsigned int curr_block_idx = curr_block - mem_map.data();
        unsigned int buddy_idx = buddy - mem_map.data();
        
        curr_order++;

        if (curr_block_idx < buddy_idx) {
            curr_block->order = curr_order;
            curr_block->refcount = 0;
            buddy->order = -1; 
        } else {
            buddy->order = curr_order;
            buddy->refcount = 0;
            curr_block->order = -1; 

            curr_block = buddy; 
        }
    }

    free_area[curr_order].push_back(curr_block);
}

void dump() {
    for (int i = MAX_ORDER; i >= 0; i--)
        std::cout << "free_area[" << i << "] " << free_area[i].size()
                  << std::endl;
}

int main() {
    mem_map.resize(NUM_PAGES);
    free_area.resize(MAX_ORDER + 1); // 0 - 10
    for (size_t i = 0; i < NUM_PAGES; i += (1 << MAX_ORDER)) {
        mem_map[i].order = MAX_ORDER;
        free_area[MAX_ORDER].push_back(&mem_map[i]);
    }

    std::cout << "\np1:\n";
    struct page* p1 = alloc_pages(1);
    dump();

    std::cout << "\np2:\n";
    struct page* p2 = alloc_pages(1);
    dump();

    std::cout << "\np3:\n";
    struct page* p3 = alloc_pages(1);
    dump();

    free_pages(p1);
    free_pages(p2);
    free_pages(p3);

    std::cout << "\nfree:\n";
    dump();
    return 0;
}
