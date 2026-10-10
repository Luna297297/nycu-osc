# Lab2: Booting

## Feature1: UART Bootloader (Exercise 2-1)

透過 Shell 的 `load` Command，從 UART 接收 Host 傳來的 Kernel Image，將 Image 放到不會覆蓋目前 Bootloader 的記憶體區域，然後跳轉執行。

### Feature1.1: UART Bootloader Overview

理解 UART Bootloader 的用途、Protocol、Memory Layout 與完整控制流程。

- [x] T1.1: 理解 Bootloader 與 Loaded Kernel 是兩個不同執行角色
- [x] T1.2: 理解 UART 是沒有 Message Boundary 的 Byte Stream
- [x] T1.3: 理解 Magic、Size 與 Payload 的 State Machine
- [x] T1.4: 理解 Little-endian Header Encoding
- [x] T1.5: 理解 Direct-to-destination Byte Traversal 與 Half-open Range
- [x] T1.6: 理解 Link Address、Load Address 與 Entry Address 的差異
- [x] T1.7: 理解 Payload `_start`、Hart ID、DTB 與 `fence.i` 的必要性
- [x] T1.8: 理解 Shell Text Mode 與 Loader Binary Mode 的責任邊界
- [x] T1.9: 理解 QEMU PTY、Host Sender 與 Guest UART 的測試模型

---

### Feature1.2: Shell to Loader Transition

加入 `load` Command，讓 Shell 將 Loader Request 回報給上層 Boot Flow；此階段不接收 Binary Data。

- [x] T1.10: 決定 Command Parser 如何表示 Continue Shell 與 Loader Request
- [x] T1.11: 修改 `command_parser()` 的 Declaration 與 Return Type
- [x] T1.12: 讓既有 Command Paths 回傳 Continue Shell
- [x] T1.13: 加入 `load` Command，回傳 Loader Request
- [x] T1.14: 在 `start_kernel()` 接收 Command Parser Result
- [x] T1.15: 在上層 Boot Flow 處理 Loader Request，暫時只輸出測試訊息
- [x] T1.16: 更新 `help` Command，列出 `load`
- [x] T1.17: Build 並確認沒有新增 Compiler Warning
- [x] T1.18: Regression Test `hello`、`help`、`info`、`ls`、`cat` 與 Unknown Command
- [x] T1.19: 驗證 `load` 進入 Placeholder Loader Mode 後能安全返回 Shell Prompt

---

### Feature1.3: QEMU PTY and Host Raw-byte Sender

建立 Host 與 QEMU UART 之間的 Binary-safe 傳輸通道；此階段不修改 Guest 的 UART Receive 行為，也不解析 Boot Protocol。

- [x] T1.20: 決定 QEMU PTY Test Target 與 Host Sender 的檔案位置及操作介面
- [x] T1.21: 在 Makefile 加入獨立的 QEMU PTY Test Target，保留既有 Interactive Target
- [x] T1.22: 啟動 QEMU 並取得其建立的 `/dev/pts/*` Serial Device Path
- [x] T1.23: 建立最小 Host Sender，接收 Serial Device Path 作為輸入
- [x] T1.24: 以 Binary、Unbuffered 模式開啟 PTY，傳送固定 Known-byte Sequence
- [x] T1.25: 檢查 Host Sender 的 Write Result，確認完整送出預期 Byte Count
- [ ] T1.26: 驗證 Host Sender 能透過 PTY 將資料送至 QEMU UART
- [ ] T1.27: Regression Test 原本的 QEMU Interactive Shell Target

---

### Feature1.4: UART Bootloader Integration Roadmap

依照 QEMU-first 順序，逐步完成 Binary Transport、Protocol Parsing、Payload Loading 與 Kernel Jump。

- [ ] T1.28: 驗證 Bootloader Raw UART Receive 不會修改任何 Byte
- [ ] T1.29: 接收並驗證固定 4-byte Magic；失敗時回到 Shell
- [ ] T1.30: 解析 Little-endian Size，並在接收 Payload 前驗證 Destination Range
- [ ] T1.31: 接收恰好 `size` Bytes 至 Target RAM，以 Known Pattern 驗證 Range 與 Content
- [ ] T1.32: 建立高位址 Payload Build，驗證 ELF Entry、`_start` 與 Section Addresses
- [ ] T1.33: 執行 `fence.i` 並跳至 Minimal Payload，驗證 Runtime Entry、Hart ID 與 DTB
- [ ] T1.34: 在 QEMU 驗證 Bootloader → UART Upload → Full Kernel → Kernel Shell
- [ ] T1.35: 在 OrangePi RV2 驗證 SD Bootloader、`/dev/ttyUSBx` 與 Full Kernel Boot

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

- [x] T3.4: 理解 New ASCII CPIO Archive 的資料格式
- [x] T3.5: 解析 CPIO Header
- [x] T3.6: 解析 Filename 與 File Data
- [x] T3.7: 處理 4-Byte Alignment（Padding）
- [x] T3.8: 辨識 Archive 結尾（`TRAILER!!!`）

## Feature4: Kernel Integration

將已完成的 Device Tree Parser 與 Kernel Boot Flow 整合，使 Kernel 能從 Firmware 傳入的 DTB 動態取得硬體資訊，逐步移除 Lab1 的 Hardcoded Hardware Address。

### Feature4.1: Boot Context Integration

理解並建立 Firmware → `_start` → `start_kernel()` 的 Boot ABI 資料傳遞流程。

- [x] T4.1: 理解 OpenSBI / U-Boot 進入 Kernel 時的 Boot ABI
- [x] T4.2: 理解 `a0` 儲存 Hart ID、`a1` 儲存 DTB Address
- [x] T4.3: 確認 `_start` 在初始化 `.bss` 與 Stack 時不覆寫 `a0`、`a1`
- [x] T4.4: 修改 `start_kernel()`，接收 `hartid` 與 `fdt`
- [x] T4.5: 確認 Kernel 能正確取得 Firmware 傳入的 DTB Pointer

---

### Feature4.2: FDT-based UART Initialization

利用 Device Tree Parser 查詢 UART Node 的 `reg` Property，取得 Runtime UART Base Address，取代 Lab1 的 Hardcoded UART Address。

目前採用 **QEMU-first** 的整合順序：先在 QEMU 建立可靠且可觀察的 UART Console，再逐步接上 FDT 查詢結果。每個步驟都必須能獨立編譯與驗證；在 Runtime 初始化確認正常前，不直接移除 Hardcoded Base。

#### Design Decisions

- 不使用 Board-specific `#ifdef` hardcode UART Base；Base 必須來自 Firmware 傳入的 DTB。
- 測試與目前整合階段允許由 Build Configuration 選擇 UART Register Layout 與 Access Width。
- 先完成並驗證 QEMU，再處理 OrangePi RV2。
- Hardcoded UART Address 只作為遷移期間的 Baseline / Fallback，最終必須移除。
- UART 裝置的差異不只有 Base Address：QEMU `ns16550a` 與 OrangePi UART 的 Register Offset 和 Access Width 也可能不同。

```text
QEMU ns16550a
├── base:         0x10000000
├── THR/RBR:      base + 0x00
├── LSR:          base + 0x05
└── access width: 8-bit

OrangePi RV2 UART
├── base:         0xd4017000
├── THR/RBR:      base + 0x00
├── LSR:          base + 0x14
└── access width: 32-bit
```

因此，`fdt_get_uart_base()` 足以完成 Base Address 查詢，但尚不足以單獨建立完整的跨平台 UART Driver。UART Base 已由 DTB 決定；Register Layout 目前由 Build Configuration 選擇，OrangePi Runtime 驗證延後處理。

- [x] T4.6: 理解 UART 初始化與 FDT Parser 的依賴關係
- [x] T4.7: 建立最小的 UART Property Lookup 流程
- [x] T4.8: 從 FDT 找到 QEMU(/soc/uart@10000000) / OrangePi(/soc/serial@d4017000) 對應的 UART Node
- [x] T4.9: 取得 UART Node 的 `reg` Property
- [x] T4.10: 解析 `reg` Property，取得 UART Base Address

#### QEMU-first Integration

- [x] T4.11: 確認 `BOARD=qemu` 與 `BOARD=orangepi` 均能成功 Build
- [x] T4.12: 刻意保留未使用的 `depth` 作為 Exercise Code 的辨識標記
- [x] T4.13: 使用 Hardcoded QEMU Base 與 NS16550A Register Layout，驗證 QEMU Banner、輸入、Echo 與 Shell Command
- [x] T4.14: 保持 UART Driver 不變，印出並驗證 `fdt_get_uart_base()` 回傳 `0x10000000`
- [x] T4.15: 將 UART Base 從直接使用的 Constant 改為具有 Hardcoded 初始值的 Runtime State，確認行為不變
- [x] T4.16: 加入 `uart_init()`，先使用 Hardcoded QEMU Base 呼叫，確認初始化時機位於第一次 UART Access 之前
- [x] T4.17: 將 `fdt_get_uart_base()` 結果傳給 `uart_init()`，暫時保留 Hardcoded QEMU Base 作為失敗時的 Fallback
- [x] T4.18: 確認 Detected Base 非零，驗證 QEMU 未使用 Hardcoded Fallback
- [x] T4.19: 移除 Hardcoded `UART_BASE` 與 Fallback；查詢失敗時停止 Kernel
- [x] T4.20: 驗證 QEMU 從 Boot 到互動式 Shell 均只依賴 Runtime DTB UART Base

#### OrangePi Follow-up

此段延後至 QEMU 的 CPIO `ls` / `cat` 完成，再與下方 OrangePi Initrd Checkpoint 一起驗證。

- [ ] T4.21: 查詢並驗證 QEMU 與 OrangePi UART Node 的 `compatible` 及 Register Layout 資訊
- [ ] T4.22: 評估並確定 UART Register Layout 採用 Runtime `compatible` Selection 或既有 Build Configuration
- [ ] T4.23: 將 OrangePi Register Offset 與 Access Width 接入 UART Driver
- [ ] T4.24: 驗證 OrangePi 從 Boot 到互動式 Shell 均使用 Runtime DTB UART Configuration

---

### Feature4.3: Initial Ramdisk Integration

在 QEMU 載入 New ASCII CPIO Archive，透過 DTB `/chosen` Node 動態取得 Initrd Memory Range，再將該範圍交給既有 CPIO Parser。整合順序維持 **DTB Address Discovery → Parser Library → Shell Commands**，不 hardcode Initrd Address。

#### Data Flow

```text
QEMU -initrd initramfs.cpio
        ↓
DTB /chosen
├── linux,initrd-start
└── linux,initrd-end
        ↓
Kernel Initrd Range [start, end)
        ↓
Bounded CPIO Parser
        ↓
Shell ls / cat
```

#### Design Decisions

- 先在 QEMU 完成並驗證 Initrd Integration，再處理 OrangePi FIT Image。
- Initrd Address 必須來自 DTB，不能使用 Hardcoded Address。
- 使用 half-open range `[start, end)` 表示有效記憶體範圍。
- 重用 Exercise 中已完成的 CPIO Traversal Logic，不重寫 parser。
- Kernel parser 不依賴 libc、host file I/O、dynamic allocation 或 `printf()`。
- CPIO parser 必須接收 archive end boundary，避免 malformed archive 造成越界 traversal。
- 先驗證 Initrd Range，再呼叫 CPIO parser；最後才加入 Shell Commands。

#### QEMU Initrd Loading and Address Discovery

- [x] T4.25: 準備最小的 New ASCII CPIO Archive 作為 QEMU 測試資料
- [x] T4.26: 讓 QEMU Run Command 使用 `-initrd` 載入 Archive
- [x] T4.27: 檢查 QEMU DTB `/chosen` 中的 `linux,initrd-start` 與 `linux,initrd-end`
- [x] T4.28: 設計取得 Initrd `[start, end)` 的 FDT Integration API
- [x] T4.29: 查詢並解析兩個 Initrd Properties，驗證 Property Length 與 Address Encoding
- [x] T4.30: 在 Kernel 印出並驗證 Initrd Start、End 與 Size，再進入 Shell

#### Bounded CPIO Parser Integration

- [x] T4.31: 從既有 Exercise 重用 CPIO Parser Core，分離 Host Test Harness
- [x] T4.32: 建立接受 `[start, end)` 的 Bounded `initrd_list()` Traversal Interface
- [x] T4.33: 驗證 Header、Filename、File Data 與兩次 4-byte Alignment 均未超出 End Boundary
- [x] T4.34: 在 Kernel 中驗證 Archive 能遍歷至 `TRAILER!!!`

#### Shell Integration

**Current Milestone:** 下一個小步驟是完成 bounded `initrd_cat()`；先直接重用已驗證的 `initrd_list()` record traversal，確認能精確輸出 `file_len` bytes，再接入 Shell。Parser traversal 的共用抽象延後到兩者都能運作後再評估。

- [ ] T4.35: 將 `ls` Command 接到 CPIO Listing Interface
- [ ] T4.36: 將 `cat <filename>` Command 接到 CPIO Lookup Interface，輸出實際 File Data
- [ ] T4.37: 驗證檔案存在、不存在、空檔案與非 NUL-terminated File Data
- [ ] T4.38: 驗證 QEMU 從 DTB Discovery 到 `ls` / `cat` 全程不使用 Hardcoded Initrd Address

#### OrangePi Integration Checkpoint

QEMU 的 `ls` / `cat` 完成後，先將 Basic Exercise 2 與 3 帶到 OrangePi 驗證，再開始 UART Bootloader。這是平台整合 checkpoint，不在此階段重新設計已通過驗證的 Parser。

- [ ] T4.39: 在 `kernel.its` 加入 Ramdisk Image 與 Configuration Reference
- [ ] T4.40: 驗證 OrangePi DTB `/chosen` 提供正確的 Initrd `[start, end)`
- [ ] T4.41: 驗證 OrangePi Runtime UART、`ls` 與 `cat`
- [ ] T4.42: 移除僅供整合驗證使用的 Boot-time Archive Listing

---

## Feature5: Bootloader Self-Relocation (Advanced Exercise)

讓 Bootloader 搬移自己，釋放標準 Kernel Entry Address，之後 UART 收到的 Kernel 即可載入標準位置。

> **最後實作。** 本功能依賴已可運作的 UART Bootloader，也必須避開 DTB 與 Initrd，因此不與 Basic Exercise 1 同時開發。

- [ ] T5.1: 畫出 Bootloader、Kernel、DTB、Initrd 與可用 RAM 的 Memory Map
- [ ] T5.2: 根據可用記憶體選擇 Relocation Destination
- [ ] T5.3: 調整 Linker / Relocation Design，確認搬移後仍能正確執行
- [ ] T5.4: 搬移 Bootloader 並跳轉至 Relocated Entry
- [ ] T5.5: 將新 Kernel 載入標準 Entry Address
- [ ] T5.6: 驗證 Relocation Range 不與 DTB 或 Initrd 重疊
- [ ] T5.7: 在 QEMU 完整驗證，再進行 OrangePi 驗證
