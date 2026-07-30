## **Lab1: Hello World**

### **Feature1: Basic Initialization**
完成 Kernel Runtime Initialization，使 Bare-metal Kernel 能安全執行 C 程式，包含清除 `.bss` 區段並初始化 Stack Pointer。

- [x] T1.1: 理解 Bootloader 與 Kernel 的交接流程
- [ ] T1.2: 清除 `.bss` Section
- [ ] T1.3: 設定 Stack Pointer
- [ ] T1.4: 驗證 `start_kernel()` 可正常執行

### **Feature2: UART Driver**
實作 UART Driver，使 OrangePi RV2 能透過 Serial Communication 與 Host 進行雙向通訊。

- [ ] T2.1: 找出 UART Base Address
- [ ] T2.2: 理解 UART Register Layout
- [ ] T2.3: 實作 `uart_putc()`
- [ ] T2.4: 實作 `uart_getc()`
- [ ] T2.5: 驗證 UART 收發功能

### **Feature3: Simple Shell**
利用 UART Driver 建立最小可互動 Shell，讓使用者能透過 UART 輸入指令並取得回應。

- [ ] T3.1: 建立 Command Loop
- [ ] T3.2: 接收使用者輸入
- [ ] T3.3: 實作 `help`
- [ ] T3.4: 實作 `hello`
- [ ] T3.5: 處理 `\n` → `\r\n`
- [ ] T3.6: 處理未知指令

### **Feature4: System Information (SBI)**
實作 SBI (Supervisor Binary Interface) 呼叫，透過 OpenSBI 取得系統資訊，並整合至 Shell。

- [ ] T4.1: 理解 SBI Calling Convention
- [ ] T4.2: 實作 `sbi_ecall()`
- [ ] T4.3: 取得 OpenSBI Specification Version
- [ ] T4.4: 取得 Implementation ID
- [ ] T4.5: 取得 Implementation Version
- [ ] T4.6: 實作 `info` 指令