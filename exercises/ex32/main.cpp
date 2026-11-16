#include <iostream>
#include <list>
#include <vector>
#include <cmath>

#define PAGE_SIZE (1UL << 12) //page size = 4KB
#define NUM_PAGES 0x280000
#define MAX_ORDER 10

typedef unsigned long phys_addr_t;

struct page {
    int order = -1;
    int refcount = 0; //0: waiting to be allocated; 1: allocated; 2: reserved
};

std::vector<page> mem_map;
std::vector<std::list<page*>> free_area;

struct page* get_buddy(struct page* block, unsigned int order) {
    size_t idx = block - mem_map.data();
    size_t buddy_idx = idx ^ (1UL << order);

    if (buddy_idx >= mem_map.size()) return nullptr;

    return &mem_map[buddy_idx];
}

void memory_reserve(phys_addr_t base, size_t size) {
    if (size == 0) return;
    phys_addr_t  end = base + size;


    // 取得 page frame number [start_pfn, end_pfn)
    size_t start_pfn = base / PAGE_SIZE;
    size_t end_pfn = end / PAGE_SIZE + (end % PAGE_SIZE != 0);

    int current_order = MAX_ORDER;
    while (current_order >= 0) {
        auto& blocks = free_area[current_order];
        auto it = blocks.begin();

        while (it != blocks.end()) {
            struct page* current_block = *it;

            size_t block_start = current_block - mem_map.data();
            size_t block_end = block_start + (1UL << current_order);

            if (block_end <= start_pfn || block_start >= end_pfn) {
                ++it;
            } else if (block_start >= start_pfn && block_end <= end_pfn) {
                current_block->refcount = 2;
                it = blocks.erase(it);
            } else {
                it = blocks.erase(it);
                int split_order = current_order - 1;
                struct page* right = get_buddy(current_block, split_order);

                current_block->order = split_order;
                right->order = split_order;

                current_block->refcount = 0;
                right->refcount = 0;
                
                free_area[split_order].push_back(current_block);
                free_area[split_order].push_back(right);
            }
        }
        current_order--;
    }
}

void dump() {
    for (int i = MAX_ORDER; i >= 0; i--)
        std::cout << "free_area[" << i << "] " << free_area[i].size()
                  << std::endl;
}

void mm_init() {
    mem_map.resize(NUM_PAGES);
    free_area.resize(MAX_ORDER + 1);
    for (size_t i = 0; i < NUM_PAGES; i += (1 << MAX_ORDER)) {
        mem_map[i].order = MAX_ORDER;
        free_area[MAX_ORDER].push_back(&mem_map[i]);
    }
    memory_reserve(0, 0x82a69510);
}

int main() {
    mm_init();
    dump();
    return 0;
}
