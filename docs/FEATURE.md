## **Lab0: Environment Setup**

### **Feature1: Cross Compiler**
安裝 cross compiler，使 Ubuntu 虛擬機上的 src code 能編譯成 OrangePi RV2 (64-bit risc-v) 架構的指令，aka 能產生 RISC-V 64 目標檔。
- [x] T1.1: 安裝 gcc-riscv64-unknown-elf
- [x] T1.2: 確認指令存在
- [x] T1.3: 編譯一個最小檔案
- [x] T1.4: 檢查編譯出來的檔案是不是 RISC-V 格式
### **Feature2: Linker**
- [x] T2.1: 完成 Linker Script (`SECTIONS`)
### **Feature3: QEMU**
- [x] T3.1: 安裝 `qemu-system-riscv64`
- [x] T3.2: 確認 QEMU 指令存在
- [x] T3.3: 暫不執行 kernel，待產生 `kernel.bin` 後再進行 QEMU 驗證
### **Feature4: Source Code → Object File**
使用 Cross Compiler 將 Assembly 編譯成 Relocatable ELF（Object File）。
- [x] T4.1: 建立 `a.S`
- [x] T4.2: 使用 `riscv64-unknown-elf-gcc` 編譯為 `a.o`
- [x] T4.3: 確認 `a.o` 為 RISC-V Relocatable ELF
- [x] T4.4: 使用 `objdump` 檢視 `.text` 與組合語言內容
### **Feature 5：Object File → Executable ELF**
使用 Linker 將 Object File 配置 Memory Layout，產生 Executable ELF。
- [ ] T5.1: 使用 `linker.ld` 連結 `a.o`
- [ ] T5.2: 產生 `kernel.elf`
- [ ] T5.3: 確認 `kernel.elf` 為 Executable ELF
- [ ] T5.4: 檢查 `_start` 已配置至正確記憶體位址
### **Feature 6：Executable ELF → Raw Binary**
將 Executable ELF 轉換為可供 Bootloader 使用的 Raw Binary。
- [ ] T6.1: 使用 `objcopy` 將 `kernel.elf` 轉換為 `kernel.bin`
- [ ] T6.2: 確認 `kernel.bin` 已成功產生
- [ ] T6.3: 比較 `kernel.elf` 與 `kernel.bin` 的差異
- [ ] T6.4: 理解 Raw Binary 在 Boot Flow 中的角色
### **Feature 7：Verify on QEMU**
在部署到 Orange Pi RV2 前，先使用 QEMU 驗證 Kernel Image。
- [ ] T7.1: 使用 `kernel.bin` 啟動 QEMU
- [ ] T7.2: 觀察 QEMU 輸出的組合語言 (`-d in_asm`)
- [ ] T7.3: 確認 Kernel Image 可正常載入與執行
- [ ] T7.4: 理解 QEMU 驗證的限制