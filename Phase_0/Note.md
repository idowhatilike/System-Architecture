# Note : Phase 0 - C++ Lifetime, RAII, Smart Pointers, CMake Toolchain Pipeline 

__

## 1. Trọng Tâm & Mục Tiêu Giai Đoạn (Phase Focus & Core Objectives)

### Mục Tiêu Học Tập Cốt Lõi (Core Learning Objectives)
- **Quản lý Vòng đời & Thời gian lưu trữ (Storage Duration & Lifetime)**: Phân định ranh giới và đặc tính vận hành giữa 3 loại bộ nhớ: `static`, `automatic` (Stack), và `dynamic` (Heap).
- **Nguyên lý RAII (Resource Acquisition Is Initialization)**: Đóng gói quản lý tài nguyên (bộ nhớ, file descriptors, locks) vào Destructor để đảm bảo dọn dẹp tự động và an toàn với Exception.
- **Con trỏ thông minh & Semantics sở hữu (Smart Pointers & Ownership)**: Phân tích và làm chủ `std::unique_ptr` (sở hữu độc quyền / exclusive ownership) và `std::shared_ptr` (sở hữu chia sẻ / reference counting).

### Sản Phẩm Bàn Giao Cột Mốc (Phase Milestone Deliverables)
- **Thư viện Quản lý Tài nguyên RAII**: Mã nguồn C++20 mẫu tuân thủ nghiêm ngặt không sử dụng `raw new/delete` thủ công.
- **Hệ thống Kiểm thử Bộ nhớ**: Cấu hình pipeline biên dịch tích hợp cờ cảnh báo nghiêm ngặt (`-Wall -Wextra -Wpedantic -Werror`) cùng bộ công cụ kiểm thử rò rỉ bộ nhớ **AddressSanitizer (`-fsanitize=address`)** và **Valgrind**.

---

## 2. Tài Nguyên Học Tập Đang Sử Dụng (Active Learning Resources)

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

## 3. Theo Dõi Nhận Thức & Năng Lực (Understanding Tracker)

### Tôi Đã Hiểu Gì? (What Do I Understand?)

### Tôi Có Thể Giải Thích Gì? (What Can I Explain?)

### Tôi Có Thể Xây Dựng Gì? (What Can I Build?)
- Thiết lập môi trường phát triển C++20 cơ bản trên Linux/WSL2 với GCC/Clang và CMake.

### Tôi Còn Thắc Mắc Gì? (What Still Confuses Me?)

---

## 4. Yêu Cầu Bằng Chứng Học Tập (Learning Evidence Requirements)

| Danh Mục (Category) | Thành Phần Cần Hoàn Thành (Required Artifacts) | Trạng Thái (Status) |
| :--- | :--- | :--- |
| **Ghi Chú Kỹ Thuật (Technical Notes)** | `docs/memory-layout-amd64.md` (Bố cục bộ nhớ ELF, Không gian địa chỉ ảo, Ranh giới User/Kernel)<br>`docs/raii-and-ownership.md` (Nguyên lý RAII, Quyền sở hữu tài nguyên và Smart Pointers) | Chưa Bắt Đầu (*Not Started*) |
| **Thực Nghiệm Lập Trình (Experiments)** | `experiments/stack-vs-heap/` (Quan sát chiều phát triển Stack và Heap)<br>`experiments/linker-errors/` (Kiểm tra Bảng ký hiệu Symbol Table qua `nm`/`objdump`) | Chưa Bắt Đầu (*Not Started*) |
| **Đồ Án Cột Mốc (Milestone Projects)** | `projects/multi-module-cli/` (Dự án C++20 CLI Utility đa module quản lý bằng CMake - Phase 0 Milestone) | Chưa Bắt Đầu (*Not Started*) |
| **Bài Lab & Benchmark** | `benchmarks/smart-pointer-overhead/` (Đo đạc chi phí chênh lệch thời gian giữa raw pointer, `unique_ptr`, `shared_ptr`) | Chưa Bắt Đầu (*Not Started*) |

---

## 5. Câu Hỏi Mở & Giả Thuyết Nghiên Cứu (Open Research Questions)

1. **Bộ Nhớ (Memory):** Khi đối tượng `std::unique_ptr` bị tiêu hủy, lệnh `delete` bên dưới sẽ gọi lời gọi hệ thống `brk()` / `sbrk()` ngay lập tức hay trả bộ nhớ về cho User-Space Allocator (`glibc ptmalloc`) quản lý?
2. **Công Cụ (Toolchain):** Làm thế nào trình biên dịch (GCC/Clang) tối ưu hóa loại bỏ hoàn toàn chi phí trừu tượng (*Zero-Overhead Abstraction*) của `std::unique_ptr` để tạo ra mã Assembly tương đương con trỏ thô?
3. **Liên Kết (Linker):** Sự khác biệt về bản chất giữa Static Linking (`.a`) và Dynamic Linking (`.so`) ảnh hưởng thế nào đến kích thước tệp thực thi và địa chỉ bộ nhớ khi nạp chương trình?
4. **Chuẩn C++ (Language Standard):** Tại sao việc sử dụng `raw new/delete` lại bị coi là Anti-Pattern trong lập trình C++ hiện đại và trong các hệ thống đòi hỏi tính an toàn cao?

---
