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