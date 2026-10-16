# MODULE WORKBOOK 006: LIMITED DIRECT EXECUTION, SYSCALL BOUNDARY & RAII OS WRAPPERS
**Level** : Systems Core (Phase 0) | **Time Target** : 2 - 3 Hours | **Prerequisites** : Modern C++ (RAII, Move Semantics), CMake, OSTEP Ch.4 & Ch.6

---

#### 1. Định Hướng Nghiên Cứu Core (First-Principles Research Directives)

* **Tài liệu nguồn bắt buộc tự đọc:**
  * *OSTEP*: Chapter 4 (*The Abstraction: The Process*), Chapter 6 (*Mechanism: Limited Direct Execution*).
  * *Linux Man Pages*: `man 2 syscalls`, `man 1 strace`, `man 2 pipe`, `man 2 close`, `man 2 sched_setaffinity`, `man 3 stdio`.
  * *Modern C++ Docs (cppreference)*: RAII, Rule of Five, `std::exchange`, `std::unique_ptr` (Custom Deleters).

* **Câu hỏi định hướng nghiên cứu sâu:**
  1. **Hai tầng lưu trữ ngữ cảnh (OSTEP Ch.6):** Đối chiếu sự khác biệt về thời điểm, chủ thể thực hiện (Hardware vs. OS Kernel), và vị trí lưu trữ trong RAM giữa: (a) Lưu thanh ghi User-mode vào `Kernel Stack` khi xảy ra Trap/Interrupt, và (b) Lưu thanh ghi Kernel-mode vào `proc_struct` (PCB) khi gọi `switch()`. Tại sao việc thay đổi thanh ghi Stack Pointer (`SP`) sang `k-stack` của tiến trình đích lại khiến lệnh `return-from-trap` chuyển quyền thực thi sang tiến trình mới?
  2. **Cơ chế truyền tham số System Call (`man 2 syscalls` & `man 2 syscall`):** Khi gọi hàm bọc (wrapper) trong `glibc` như `read()` hoặc `write()`, User-space truyền mã định danh System Call (syscall number) và các đối số qua những thanh ghi CPU nào trước khi phát lệnh `syscall`/`trap`? Giá trị trả về và mã lỗi (`errno`) được Kernel trả ngược về User-space bằng cơ chế nào?
  3. **User-space Buffering vs. Unbuffered Syscalls (`man 3 stdio` & `man 2 write`):** Phân tích cơ chế bộ đệm mặc định của `stdio` (`fwrite` / `std::ofstream`) so với lệnh gọi hệ thống trực tiếp `write()`. Tại sao việc gom dữ liệu ở User-space trước khi gọi `write()` lại giảm thiểu chi phí chuyển đổi đặc quyền (Mode Switch overhead)?
  4. **RAII Ownership trên tài nguyên Kernel (`man 2 close` & C++ Lifetime):** Một File Descriptor (`int fd`) là chỉ số trỏ vào bảng File Descriptor Table nằm trong vùng nhớ của Kernel. Khi bọc `fd` vào một class C++ (RAII), tại sao bắt buộc phải vô hiệu hóa Copy Constructor/Copy Assignment (`= delete`) và sử dụng `std::exchange(other.fd_, -1)` trong Move Constructor?

* **Bài toán đánh đổi & Định lượng:**
  * Tính toán tổng chi phí chu kỳ CPU (CPU cycles) bị tiêu hao thuần túy cho việc chuyển đổi ngữ cảnh đặc quyền (User $\leftrightarrow$ Kernel) khi ghi $1.000.000 	ext{ bytes}$ (mỗi lần $1 	ext{ byte}$) bằng hai cách: (a) Gọi trực tiếp `write()` $1.000.000$ lần, và (b) Ghi qua bộ đệm $8	ext{KB}$ ($8192 	ext{ bytes}$) của `fwrite()` (chỉ phát sinh $123$ lệnh `write()`). Giả định mỗi lần Mode Switch tốn $200 	ext{ CPU cycles}$. Đối chiếu lý thuyết này với số liệu đo thực tế bằng `std::chrono::steady_clock` trên máy của bạn.

---

#### 2. Thiết Lập CMake Toolchain Pipeline (Phase 0 Standard)

Tự khởi tạo dự án đa module bằng CMake (tách biệt thư viện tĩnh quản lý tài nguyên RAII và chương trình thực thi đo kiểm):

```text
day006-lde-raii/
├── CMakeLists.txt
├── include/
│   └── unique_fd.hpp        # RAII Wrapper cho POSIX File Descriptor
├── src/
│   └── unique_fd.cpp
└── apps/
    └── lde_benchmark.cpp    # Đo Syscall latency, Buffering & Context Switch
```

* **Yêu cầu `CMakeLists.txt`:**
  * Tạo target thư viện tĩnh `os_raii` từ `src/unique_fd.cpp`.
  * Tạo target thực thi `lde_benchmark` liên kết (`target_link_libraries`) với `os_raii`.
  * Bắt buộc áp dụng cờ biên dịch cho cả 2 target:
    ```cmake
    target_compile_options(<target> PRIVATE
        -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror
        -O0 -g -fsanitize=address,undefined
    )
    target_link_options(<target> PRIVATE -fsanitize=address,undefined)
    ```

---

#### 3. Đề Bài Code Lab & Thử Thách Break-Check (Lab Specifications)

* **Đặc tả Đồ án (Tự viết từ con số 0 - Chuẩn C++20):**
  1. **Module `UniqueFd` & `ScopedPipe` (`unique_fd.hpp` / `unique_fd.cpp`):**
     * Cài đặt class `UniqueFd` quản lý vòng đời của một `int fd`. Tự động gọi `close(fd)` trong Destructor nếu `fd >= 0`.
     * Tuân thủ nghiêm ngặt Rule of Five: Cấm Copy (`= delete`), cài đặt Move Constructor và Move Assignment đảm bảo quyền sở hữu độc quyền (exclusive ownership). Cung cấp hàm `.get()`, `.release()`, và `.reset()`.
     * Cài đặt struct/class `ScopedPipe` chứa 2 đối tượng `UniqueFd` (`read_end` và `write_end`), khởi tạo thông qua syscall `pipe()`.
  2. **Module Benchmark (`lde_benchmark.cpp`):**
     * Sử dụng `std::chrono::steady_clock` và `std::chrono::duration<double, std::nano>`.
     * **Task A (`--syscall`):** Đo độ trễ trung bình (tính bằng `ns`) của $1.000.000$ lần gọi `read(STDIN_FILENO, nullptr, 0)`.
     * **Task B (`--buffer`):** So sánh tổng thời gian ghi $100.000$ lần (mỗi lần 1 ký tự `'x'`) vào `/dev/null` giữa `write()` trực tiếp (dùng `UniqueFd`) và `fwrite()` có bộ đệm.
     * **Task C (`--ctxswitch`):** Tạo 2 `ScopedPipe`, gọi `fork()`, dùng `sched_setaffinity()` ép cả tiến trình Cha và Con chạy trên **Core 0**. Đóng ngay các đầu pipe không dùng ở mỗi phía bằng hàm `.reset()` của `UniqueFd`. Thực hiện truyền 1 byte qua lại $100.000$ vòng và tính chi phí trung bình của 1 lần Context Switch.

* **Thử thách Break-Check (Cố tình làm hỏng hệ thống):**
  * **Break-Check 1 (FD Exhaustion):** Vô hiệu hóa lệnh `close()` trong `~UniqueFd()`. Viết vòng lặp khởi tạo `ScopedPipe` liên tục cho đến khi chương trình văng lỗi hệ thống. In mã `errno` và chuỗi `strerror(errno)` ra màn hình.
  * **Break-Check 2 (Pipe Deadlock):** Trong Task C, cố tình đổi thứ tự để cả tiến trình Cha và Con đều gọi `read()` trước khi `write()`, hoặc thử xóa lệnh đóng đầu `write_end` khi chờ EOF. Quan sát trạng thái treo của tiến trình.

---

#### 4. Nhiệm Vụ Observability & Debugging (Investigation Tasks)

Thực thi các lệnh CLI sau trên Terminal và ghi lại kết quả phân tích:

1. **Thống kê tần suất và chi phí System Call (`strace`):**
   ```bash
   strace -c ./build/lde_benchmark --buffer
   ```
   *Câu hỏi phân tích:* So sánh số lượng lời gọi `write` xuất hiện trong bảng thống kê của `strace -c` giữa hàm dùng `write()` trực tiếp và hàm dùng `fwrite()`.

2. **Đo độ nhiễu (Observer Effect) của `strace`:**
   ```bash
   ./build/lde_benchmark --syscall
   strace -c ./build/lde_benchmark --syscall
   ```
   *Câu hỏi phân tích:* Thời gian trung bình của 1 syscall khi chạy dưới `strace` tăng gấp bao nhiêu lần so với chạy trực tiếp? Đọc `man 1 strace` (tìm từ khóa `ptrace`) để giải thích tại sao `strace` lại gây ra thêm các lần Context Switch giữa tiến trình theo dõi và tiến trình bị theo dõi.

3. **Kiểm tra rò rỉ File Descriptor và Bộ nhớ (`valgrind` & `/proc`):**
   ```bash
   valgrind --track-fds=yes --leak-check=full ./build/lde_benchmark --ctxswitch
   ```
   *Câu hỏi phân tích:* Khi kích hoạt **Break-Check 1**, `valgrind --track-fds=yes` báo cáo những File Descriptor nào chưa đóng? Khi chương trình đang bị treo ở **Break-Check 2**, mở một Terminal thứ hai và chạy `ls -l /proc/<PID>/fd` cùng với `cat /proc/<PID>/status | grep State` để xác định trạng thái của tiến trình trong Kernel.

---

#### 5. Phân Tích Cạm Bẫy & Kịch Bản Sự Cố (Hazard Case Studies)

* **Sự cố 1 - Silent Data Corruption do Double-Close (Vi phạm RAII Ownership):**
  Giả sử `UniqueFd` không xóa Copy Constructor. Đối tượng `fd1` (giữ `fd = 4`) được copy sang `fd2`. Khi `fd1` hủy trước, `close(4)` được gọi và Kernel thu hồi số `4`. Ngay sau đó, một luồng khác (hoặc hàm khác) mở một file cấu hình quan trọng và được Kernel cấp phát lại đúng số `fd = 4` (do quy tắc cấp phát FD nhỏ nhất còn trống của POSIX). Khi `fd2` đi ra khỏi scope và gọi `close(4)` lần thứ hai, chuyện gì sẽ xảy ra với file cấu hình kia? Đối chiếu với mục *NOTES* trong `man 2 close`.
* **Sự cố 2 - Rò rỉ File Descriptor sang tiến trình con sau `fork()`:**
  Sau khi gọi `fork()`, tiến trình con nhận được bản sao của toàn bộ bảng File Descriptor từ tiến trình cha. Nếu tiến trình cha muốn đọc dữ liệu từ tiến trình con cho đến khi gặp `EOF` (`read()` trả về `0`), nhưng chính tiến trình cha lại quên đóng đầu ghi (`write_end`) của ống dẫn đó ở phía mình, tại sao hàm `read()` của cha sẽ bị treo vĩnh viễn dù tiến trình con đã đóng đầu ghi và kết thúc?

---

#### 6. Khung Nghiệm Thu Exit Gate (Self-Assessment Framework)

* **Knowledge Check (Tự trả lời không nhìn tài liệu):**
  1. Trình bày chính xác chuỗi sự kiện phần cứng và phần mềm diễn ra từ lúc một tiến trình ở User-mode gọi `read()` trên một đường ống rỗng $
ightarrow$ bị đưa vào trạng thái `Blocked` $
ightarrow$ CPU chuyển sang chạy tiến trình khác.
  2. Giải thích tại sao `UniqueFd` viết bằng C++ RAII có kích thước bộ nhớ (`sizeof(UniqueFd)`) đúng bằng `sizeof(int)` và không phát sinh thêm bất kỳ chi phí runtime nào so với viết code C thuần túy (Zero-overhead abstraction).
* **Break Check Target:**
  * Thu được mã lỗi `EMFILE` (errno 24: *Too many open files*) ở Break-Check 1 và xác định được giá trị giới hạn tối đa của tiến trình thông qua lệnh `ulimit -n`.
  * Chụp được trạng thái `S (sleeping)` trên kênh đợi `pipe_read` thông qua `/proc/[PID]/wchan` khi tái hiện Deadlock ở Break-Check 2.
* **Verification Metric:**
  * Dự án biên dịch sạch 100% qua CMake với đầy đủ cờ `-Wall -Wextra -Werror -fsanitize=address,undefined`.
  * Chạy `valgrind --track-fds=yes` trên bản code hoàn chỉnh trả về đúng `3 open file descriptors (0, 1, 2)` và `0 errors from 0 contexts`.