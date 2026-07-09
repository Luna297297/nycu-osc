## **Lab0: Environment Setup**

### **Feature1: Cross Compiler**
安裝 cross compiler，使 Ubuntu 虛擬機上的 src code 能編譯成 OrangePi RV2 (64-bit risc-v) 架構的指令，aka 能產生 RISC-V 64 目標檔。
- [x] T1.1: 安裝 gcc-riscv64-unknown-elf
- [x] T1.2: 確認指令存在
- [x] T1.3: 編譯一個最小檔案
- [x] T1.4: 檢查編譯出來的檔案是不是 RISC-V 格式
### ***Feature2: Linker**
- [x] T2.1: 建立 `linker.ld`
- [x] T2.2: 撰寫 Linker Script (`SECTIONS`)
- [x] T2.3: 確認 `linker.ld` 已放置於 `labs/lab0/linker/`
- [x] T2.4: 完成 Linker Script 準備，待下一階段進行 Linking