# Learning Dashboard — Systems Engineering

---

## 1. Sứ Mệnh & Nguyên Tắc Học Tập (Mission & Learning Principles)

Xây dựng nền tảng vững chắc, không thỏa hiệp trong Kỹ thuật Hệ thống thông qua việc hiện thực hóa từ nguyên lý đầu tiên (First-Principles Implementation), kiểm thử đối kháng (Adversarial Testing), và sửa lỗi có hệ thống (Systematic Debugging). Chuyển dịch từ việc tiêu thụ các tầng trừu tượng cấp cao sang làm chủ các quy luật cốt lõi của phần mềm: **bố cục bộ nhớ (Memory Layouts), bộ đệm vi xử lý (CPU Caches), nhân hệ điều hành (OS Kernels), các nguyên mẫu đồng thời (Concurrency Primitives), và trạng thái mạng bền vững (Durable Network State)**.

---

## 2. Vị Trí Hiện Tại (Current Position)

| Tham Số (Parameter) | Giá Trị Hiện Tại (Current Value) |
| :--- | :--- |
| **Giai Đoạn Hiện Tại (Current Phase)** | **Phase 0** — Nền Tảng Toolchain & C++ Modern / Quản Lý Bộ Nhớ (*Toolchain, Memory, and Modern C++ Foundations*) |
| **Tháng Hiện Tại (Current Month)** | **Tháng 1 (Month 1)** |
| **Chủ Đề Hiện Tại (Current Topic)** | **Vòng Đời Đối Tượng, Thời Gian Lưu Trữ & Quyền Sở Hữu / RAII** (*C++ Object Lifetime, Storage Duration, Ownership & RAII Semantics*) |
| **Tiến Độ Tổng Thể (Overall Progress)** | **0%** (*Điểm Xuất Phát / Baseline Start*) |
| **Câu Hỏi Cốt Lõi (Current Core Question)** | Làm thế nào để gắn liền vòng đời tài nguyên với vòng đời đối tượng (`RAII`) và thiết lập cơ chế quản lý bộ nhớ an toàn, xác định (`Deterministic`) mà không bị rò rỉ bộ nhớ (`Memory Leaks`) hay truy cập bất hợp lệ (`Use-After-Free`)? |

---

## 3. Tài Nguyên Học Tập Đang Sử Dụng (Active Learning Resources)

### Giáo Trình Chính (Primary Texts)
* **Computer Systems: A Programmer's Perspective (CS:APP3e)** — *Bryant & O'Hallaron*
  *Trọng tâm nghiên cứu:* Chapter 1 (A Tour of Computer Systems), Chapter 3 (Machine-Level Representation of Programs).
* **The Linux Programming Interface (TLPI)** — *Michael Kerrisk*
  *Trọng tâm nghiên cứu:* Chapter 6 (Processes), Chapter 7 (Memory Allocation).
* **Effective Modern C++** — *Scott Meyers*
  *Trọng tâm nghiên cứu:* Items 1–4 (Deducing Types), Items 18–22 (Smart Pointers), Items 23–25 (Move Semantics & Rvalue References).

### Tài Liệu Chuẩn & Tiêu Chuẩn Kỹ Thuật (Documentation & Standards)
* **POSIX.1-2017 Specification**: Quản lý bộ nhớ và lời gọi hệ thống cấp thấp (`brk`, `sbrk`, `mmap`).
* **System V Application Binary Interface (AMD64 ABI)**: Khung Stack Frame, quy ước gọi hàm (Calling Conventions), thanh ghi và Alignment.
* **C++20 Standard Reference (cppreference.com)**: `std::unique_ptr`, `std::move`, Value Categories (`lvalue`/`rvalue`), Storage Duration.

### Hướng Dẫn Công Cụ Chẩn Đoán (Diagnostic Tooling Manuals)
* **GNU Debugger (GDB)**: Kiểm tra ô nhớ (`x/gx`), Hardware Watchpoints (`watch`), kiểm tra thanh ghi (`layout reg`).
* **AddressSanitizer (ASan) & LeakSanitizer (LSan)**: Các cờ biên dịch và Runtime Environment Variables.

---

## 4. Theo Dõi Nhận Thức & Năng Lực (Understanding Tracker)

### Tôi Đã Hiểu Gì? (What Do I Understand?)
- Định hướng lộ trình Kỹ thuật Hệ thống 18 tháng và phương pháp học tập dựa trên vòng lặp Debug có hệ thống.
- Cấu trúc tổng quan của Phase 0: Quản lý bộ nhớ C++20, RAII, Smart Pointers, và Pipeline biên dịch CMake.

### Tôi Có Thể Giải Thích Gì? (What Can I Explain?)
- Mục tiêu của nguyên lý RAII trong việc tự động hóa quản lý vòng đời tài nguyên hệ thống.
- Sự cần thiết của việc sử dụng các công cụ Memory Sanitizers (ASan/Valgrind) để phát hiện lỗi bộ nhớ sớm trong quá trình phát triển.

### Tôi Có Thể Xây Dựng Gì? (What Can I Build?)
- Thiết lập môi trường phát triển C++20 cơ bản trên Linux/WSL2 với GCC/Clang và CMake.
- *Các dự án chuyên sâu đang ở điểm xuất phát (Baseline).*

### Tôi Còn Thắc Mắc Gì? (What Still Confuses Me?)
- Cơ chế **Move Semantics** (`rvalue references &&`) biến đổi các thanh ghi và lệnh Assembly như thế nào ở tầng thấp dưới góc độ trình biên dịch?
- Sự vi phạm **ODR** (*One Definition Rule*) diễn ra chính xác ra sao ở tầng Linker khi đưa cùng một hàm Inline vào nhiều Translation Units?

---

## 5. Tiến Độ Đạt Exit Gate (Phase Exit Gate Progress)

### **Phase 0 Exit Gate: Toolchain, Memory, and Modern C++ Foundations**

**Trạng Thái Hoàn Thành:** 0% (Điểm Xuất Phát / Baseline Start)

| Dạng Đánh Giá (Evaluation Dimension) | Tiêu Chuẩn Bắt Buộc Để Qua Gate (Standard Required for Phase Exit) | Trạng Thái Hiện Tại (Current Status) | Bằng Chứng / Xác Minh (Evidence / Verification) |
| :--- | :--- | :--- | :--- |
| **1. Kiến Thức (Knowledge)** | Giải thích không cần tài liệu: Vòng đời đối tượng, Storage Duration, RAII, các giai đoạn Pipeline biên dịch (Preprocessor ➔ Linker), sự khác biệt giữa `unique_ptr` và `shared_ptr`. | Chưa Bắt Đầu (*Not Started*) | Chuẩn bị nghiên cứu tài liệu C++20 và cppreference. |
| **2. Thực Thi (Implementation)** | Xây dựng dự án CLI đa module bằng C++20 chuẩn mực, quản lý bằng CMake, tuân thủ nghiêm ngặt quyền sở hữu RAII, không dùng `raw new/delete`. | Chưa Bắt Đầu (*Not Started*) | Dự án CLI chưa khởi tạo tại `projects/multi-module-cli/`. |
| **3. Thử Thách Phá Đồ (Break)** | Chủ động tạo ra các lỗi vi phạm bộ nhớ: Memory Leaks, Use-After-Free, ODR Violations, Undefined Symbol Linker errors. | Chưa Bắt Đầu (*Not Started*) | Chưa thực hiện bài test vi phạm bộ nhớ. |
| **4. Sửa Lỗi (Debugging)** | Chẩn đoán và định vị nguyên nhân rò rỉ bộ nhớ, truy cập bất hợp lệ bằng GDB, Valgrind và AddressSanitizer (`-fsanitize=address`) trong vòng 15 phút. | Chưa Bắt Đầu (*Not Started*) | Chưa cấu hình thử nghiệm cờ ASan trong CMakeLists.txt. |
| **5. Tư Duy Lập Luận (Reasoning)** | Đưa ra giả thuyết có thể kiểm chứng bằng thực nghiệm trước khi chạy benchmark; giải thích được lý do chênh lệch hiệu năng bằng các quy luật bộ nhớ. | Chưa Bắt Đầu (*Not Started*) | Các bài microbenchmark dự kiến triển khai ở Tuần 3. |
| **6. Chuyển Giao (Transfer)** | Giải thích được cách mô hình tư duy RAII áp dụng cho việc quản lý các tài nguyên hệ thống khác (ví dụ: Lock trên Database, Socket mạng, File Descriptor). | Chưa Bắt Đầu (*Not Started*) | Dự kiến thực hiện vào bài test Capstone cuối Month 2. |

---
*Bảng điều khiển được biên soạn và cập nhật liên tục theo tiến độ lộ trình Kỹ thuật Hệ thống 18 tháng (Core Systems Engineering Curriculum).*
