# Lab2: Booting

## Feature1: Devicetree Parsing (Exercise 2-1)
實作 Flattened Device Tree (FDT) Parser，讓 Kernel 能透過 Device Tree 查詢硬體資訊，而不再依賴 Hardcoded Hardware Address。

### Feature1.1: FDT Header Parsing
理解並解析 Flattened Device Tree Header，取得後續各區塊的位置資訊。

- [x] T1.1: 理解 Device Tree 的用途與整體架構
- [x] T1.2: 理解 FDT 四個區塊（Header、Memory Reservation、Structure Block、Strings Block）
- [x] T1.3: 理解 Header 各欄位（magic、off_dt_struct、off_dt_strings）
- [x] T1.4: 驗證 Magic Number
- [x] T1.5: 取得 Structure Block 起始位置
- [x] T1.6: 取得 Strings Block 起始位置

---

### Feature1.2: Node Traversal (`fdt_path_offset`)
實作 `fdt_path_offset()`，遍歷 Structure Block，根據指定 Path 找到對應的 Node。

- [x] T2.1: 理解 Structure Block 為 DFS Traversal Log
- [x] T2.2: 理解 Parser 採用 Linear Scan + Stack，而非真正 DFS
- [x] T2.3: 設計 Current Path 維護方式（Stack）
- [ ] T2.4: 建立 Parser Traversal Loop
- [ ] T2.5: 處理 `FDT_BEGIN_NODE`
- [ ] T2.6: 處理 `FDT_END_NODE`
- [ ] T2.7: 處理 `FDT_PROP`
- [ ] T2.8: 比對 Current Path 與 Target Path
- [ ] T2.9: 回傳 Node Offset
- [ ] T2.10: 找不到 Node 時回傳 Error

---

### Feature1.3: Property Lookup (`fdt_getprop`)
實作 `fdt_getprop()`，根據 Node Offset 找到指定 Property，並回傳 Property Value。

- [x] T3.1: 理解 Property、nameoff 與 Strings Block 的關係
- [ ] T3.2: 根據 Node Offset 定位 Node
- [ ] T3.3: Traverse Node 底下所有 Property
- [ ] T3.4: 解析 `nameoff`
- [ ] T3.5: 從 Strings Block 取得 Property Name
- [ ] T3.6: 比對 Target Property Name
- [ ] T3.7: 回傳 Property Value
- [ ] T3.8: 找不到 Property 時回傳 Error
