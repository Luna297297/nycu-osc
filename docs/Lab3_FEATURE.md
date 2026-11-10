# exercise

本計劃先追蹤 host-side C++ exercises；完成練習不代表完成 kernel 整合。
理解項目依討論結果勾選；實作與驗證項目須有 code review 或實際輸出證據才勾選。

## exercise 31

### implemented alloc_pages()

- [x] T1.1: 理解 `alloc_pages(order)` 的輸入是 order，配置大小為 `2^order` 個連續 page frames
- [x] T1.2: 理解從 requested order 向上搜尋非空 free list 的原因
- [x] T1.3: 理解 split 一次會產生兩個 order 減一的 blocks
- [x] T1.4: 理解繼續切割其中一半，另一半放回對應 order 的 free list，直到符合 requested order
- [x] T1.5: 理解配置出去的 block 必須從 free list 移除，避免重複配置
- [ ] T1.6: 確立 block 起點與內部 entries 的 metadata invariant，以及 `order`／`refcount` 的更新規則
- [ ] T1.7: 實作從 requested order 到 `MAX_ORDER`（含）的搜尋，找不到時回傳 `nullptr`
- [ ] T1.8: Review 搜尋邏輯，執行編譯檢查；在配置與釋放尚未完成前不執行原有完整測試
- [ ] T1.9: 從找到的 free list 取出一個 block，逐層 split，將剩餘 buddy 放回對應 free list
- [ ] T1.10: 更新配置 block 的 metadata，回傳其起點 node
- [ ] T1.11: Review allocation 的邊界、metadata 與 free-list 一致性，執行編譯檢查
- [ ] T1.12: 在可安全執行的測試流程中驗證直接配置、單層與多層 split，以及不重複配置

### implemented free_pages()

- [x] T2.1: 理解相鄰且大小相同的 blocks 不一定互為 buddy，必須來自同一個 parent
- [x] T2.2: 理解 `buddy_index = index ^ (1UL << order)` 如何切換同一 parent 的左右半邊
- [x] T2.3: 理解合併前必須確認 buddy 是 free block，且 order 相同
- [x] T2.4: 理解合併後起點取較小 index，order 加一，並可繼續向上合併
- [ ] T2.5: 確立釋放與合併時各 node 的 metadata 更新規則，避免內部 entry 被誤認為 free block 起點
- [ ] T2.6: 實作釋放後尋找 buddy，檢查範圍、free state、相同 order 與最大 order 限制
- [ ] T2.7: 實作從 free list 移除可合併的 buddy，更新起點與 order，逐層合併
- [ ] T2.8: 合併停止後，將最終 block 放回對應 free list，維持 metadata 一致
- [ ] T2.9: Review 並執行編譯檢查
- [ ] T2.10: 由使用者執行 `make -C exercises/ex31 run`，對照 README 的各次 allocation 與最終 free-list 數量
- [ ] T2.11: 驗證 buddy 仍被使用時不合併，以及單層、多層合併與釋放後重新配置
- [ ] T2.12: 回顧 allocation／free 的完整流程、invariant 與一個可能的 failure mode

## exercise 32

### implemented memory_reserve()

- [ ] T3.1: 理解 reservation 的目的、輸入 `[base, base + size)` 與不可再次配置的要求
- [ ] T3.2: 理解與保留區間相交的 page 必須整頁保留，並確立 PFN 邊界與 overflow 處理
- [ ] T3.3: 理解 block 與保留範圍不相交、完全包含及部分重疊三種情況
- [ ] T3.4: 設計由高 order 向下處理的流程，以及走訪時移除／加入 free-list nodes 的方式
- [ ] T3.5: 確立 reserved block 的 metadata invariant，避免被配置或參與合併
- [ ] T3.6: 實作不相交時略過、完全落在保留範圍時移出 free list 並標記保留
- [ ] T3.7: 實作部分重疊時 split，將兩半交由較低 order 的迴圈繼續檢查
- [ ] T3.8: Review 邊界、迭代器有效性與 metadata 一致性，執行編譯檢查
- [ ] T3.9: 由使用者執行 `make -C exercises/ex32 run`，對照 README 的預期 free-list 數量
- [ ] T3.10: 驗證不對齊範圍、整個 block 保留與部分重疊，確認剩餘 free blocks 不與保留範圍相交
- [ ] T3.11: 回顧 reservation 流程，區分此模擬練習與 kernel 的 DTB／linker-symbol reservation 整合
