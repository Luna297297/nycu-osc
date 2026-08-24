# Lab2: Booting

## Feature1: UART Bootloader (Exercise 2-1)

> TODO

---

## Feature2: Device Tree Parsing (Exercise 2-2)

實作 Flattened Device Tree (FDT) Parser，讓 Kernel 能透過 Device Tree 查詢硬體資訊，而不再依賴 Hardcoded Hardware Address。

### Feature2.1: FDT Header Parsing

理解並解析 Flattened Device Tree Header，取得後續各區塊的位置資訊。

- [x] T2.1: 理解 Device Tree 的用途與整體架構
- [x] T2.2: 理解 FDT 四個區塊（Header、Memory Reservation、Structure Block、Strings Block）
- [x] T2.3: 理解 Header 各欄位（magic、off_dt_struct、off_dt_strings）
- [x] T2.4: 驗證 Magic Number
- [x] T2.5: 取得 Structure Block 起始位置
- [x] T2.6: 取得 Strings Block 起始位置

---

### Feature2.2: Node Traversal (`fdt_path_offset`)

實作 `fdt_path_offset()`，遍歷 Structure Block，根據指定 Path 找到對應的 Node。

- [x] T2.7: 理解 Structure Block 為 DFS Traversal Log
- [x] T2.8: 理解 Parser 採用 Linear Scan + Stack，而非真正 DFS
- [x] T2.9: 設計 Current Path 維護方式
- [x] T2.10: 建立 Parser Traversal Loop
- [x] T2.11: 處理 `FDT_BEGIN_NODE`
- [x] T2.12: 處理 `FDT_END_NODE`
- [x] T2.13: 處理 `FDT_PROP`
- [x] T2.14: 比對 Current Path 與 Target Path
- [x] T2.15: 回傳 Node Offset
- [x] T2.16: 找不到 Node 時回傳 Error

---

### Feature2.3: Property Lookup (`fdt_getprop`)

實作 `fdt_getprop()`，根據 Node Offset 找到指定 Property，並回傳 Property Value。

- [x] T2.17: 理解 Property、nameoff 與 Strings Block 的關係
- [x] T2.18: 根據 Node Offset 定位 Node
- [x] T2.19: Traverse Node 底下所有 Property
- [x] T2.20: 解析 `nameoff`
- [x] T2.21: 從 Strings Block 取得 Property Name
- [x] T2.22: 比對 Target Property Name
- [x] T2.23: 回傳 Property Value
- [x] T2.24: 找不到 Property 時回傳 Error

---

## Feature3: Initial Ramdisk Parsing (Exercise 2-3)

實作 New ASCII CPIO Parser，解析載入至記憶體中的 Initial Ramdisk（initrd），讓 Kernel 能夠存取 CPIO Archive 中的檔案。

### Feature3.1: Initial Ramdisk Overview

理解 Initial Ramdisk 的用途，以及其在 Boot Flow 中扮演的角色。

- [x] T3.1: 理解 Initial Ramdisk（initrd）的用途
- [x] T3.2: 理解為什麼在實作 Filesystem 前需要 Initrd
- [x] T3.3: 理解 Initial Ramdisk 的整體運作流程

---

### Feature3.2: CPIO Archive Parsing

理解並解析 New ASCII CPIO Archive，實作 CPIO Parser。

- [ ] T3.4: 理解 New ASCII CPIO Archive 的資料格式
- [ ] T3.5: 解析 CPIO Header
- [ ] T3.6: 解析 Filename 與 File Data
- [ ] T3.7: 處理 4-Byte Alignment（Padding）
- [ ] T3.8: 辨識 Archive 結尾（`TRAILER!!!`）