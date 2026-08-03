## **Lab1: Hello World**

### **Feature1: Basic Initialization**
完成 Kernel Runtime Initialization，使 Bare-metal Kernel 能安全執行 C 程式，包含清除 `.bss` 區段並初始化 Stack Pointer。

- [x] T1.1: 理解 Bootloader 與 Kernel 的交接流程
- [x] T1.2: 清除 `.bss` Section
- [x] T1.3: 設定 Stack Pointer
- [x] T1.4: 驗證 `start_kernel()` 可正常執行

### **Feature2: UART Driver**
實作 UART Driver，使 OrangePi RV2 能透過 Serial Communication 與 Host 進行雙向通訊。

### Feature2: UART Driver
實作 UART Driver，使 OrangePi RV2 能透過 Serial Communication 與 Host 進行雙向通訊。

- [x] T2.1: 找出 UART Base Address
- [x] T2.2: 理解 UART Register Layout
- [x] T2.3: 實作 `uart_getc()`
- [x] T2.4: 實作 `uart_putc()`
- [x] T2.5: 實作 `uart_puts()`
- [x] T2.6: 驗證 UART 收發功能

### **Feature3: Simple Shell**
利用 UART Driver 建立最小可互動 Shell，讓使用者能透過 UART 輸入指令並取得回應。

- [ ] T3.1: 建立 Command Loop
- [ ] T3.2: 接收使用者輸入
- [ ] T3.3: 實作 `help`
- [ ] T3.4: 實作 `hello`
- [ ] T3.5: 處理 `\n` → `\r\n`
- [ ] T3.6: 處理未知指令

### **Feature4: SBI Base Extension**
實作 SBI (Supervisor Binary Interface) Base Extension Wrapper，透過 OpenSBI 查詢系統資訊，並理解 Kernel 與 Firmware 的互動方式。

- [x] T4.1: 理解 SBI Calling Convention
- [x] T4.2: 理解 `ecall` 與 Register Passing (`a0~a7`)
- [x] T4.3: 理解 `sbi_ecall()` Wrapper 的實作
- [x] T4.4: 實作 `sbi_get_spec_version()`
- [x] T4.5: 實作 `sbi_probe_extension()`
- [x] T4.6: 驗證 SBI Version 與 Extension Probe 結果

### Future Improvement
- [ ] Verify handling when `.bss` size is not a multiple of 8 bytes.
- [ ] Compare the implementation with Linux/OpenSBI.