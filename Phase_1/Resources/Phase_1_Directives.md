# MODULE WORKBOOK: PHASE 1 DIRECTIVES & LEARNING OBJECTIVES
**Level**: Systems Core (Phase 1) | **Time Target**: Month 3 – Month 4 | **Core Milestone**: Strict POSIX Mini-Shell (`[GATE 1]`)

---

## 1. TÀI NGUYÊN NGUỒN BẮT BUỘC NGHIÊN CỨU (PRIMARY RESOURCES)

### 1.1. Giáo Trình Hệ Thống (Core Textbooks)
* **OSTEP (Operating Systems: Three Easy Pieces - Remzi & Andrea Arpaci-Dusseau)**:
  * **Chapter 4**: *The Abstraction: The Process* (Trạng thái tiến trình, PCB, Machine state).
  * **Chapter 5**: *Interlude: Process API* (`fork`, `wait`, `exec`, Redirection, Pipes).
  * **Chapter 6**: *Mechanism: Limited Direct Execution* (Trap, Syscall boundary, Context Switch, Timer Interrupt).
* **TLPI (The Linux Programming Interface - Michael Kerrisk)**:
  * **Chapter 6**: *Processes* (Memory layout, Environment variables).
  * **Chapter 20 & 21**: *Signals: Fundamental Concepts & Signal Handlers* (Cơ chế phân phối signal, ngắt bất đồng bộ, Async-Signal-Safe functions).
  * **Chapter 24**: *Process Creation* (Chi tiết cơ chế Copy-on-Write, tối ưu hóa của Kernel khi gọi `fork`).
  * **Chapter 25 & 26**: *Process Termination & Monitoring Child Processes* (`exit`, `_exit`, `wait`, `waitpid`, trạng thái Zombie, xử lý `SIGCHLD`).
  * **Chapter 27**: *Program Execution* (Họ hàm `exec`, nạp tệp nhị phân ELF, tính kế thừa của File Descriptors và cờ `FD_CLOEXEC`).
  * **Chapter 44**: *Pipes and FIFOs* (Nguyên lý luồng byte đơn hướng, dung lượng đệm `PIPE_BUF`, quy tắc EOF và `SIGPIPE`).

### 1.2. Linux Man Pages Bắt Buộc Tra Cứu (`RTFM`)
* **Process Lifecycle**: `man 2 fork`, `man 2 execve`, `man 3 exec`, `man 2 waitpid`, `man 2 _exit`.
* **IPC & I/O Control**: `man 2 pipe`, `man 2 pipe2`, `man 2 dup`, `man 2 dup2`, `man 2 fcntl`.
* **Signal Handling**: `man 2 sigaction`, `man 2 kill`, `man 7 signal`, `man 3 signal-safety`.
* **Terminal & Session Control**: `man 2 chdir`, `man 3 getcwd`, `man 2 setpgid`, `man 3 tcsetpgrp`.

---

## 2. BỘ CÂU HỎI ĐỊNH HƯỚNG NGHIÊN CỨU SÂU (RESEARCH DIRECTIVES)

Sir cần tự trả lời chính xác và kiểm chứng được các câu hỏi bản chất sau trước khi code đồ án:

1. **Cơ chế Copy-on-Write (COW) và Page Table**:
   Khi tiến trình cha chiếm 1GB RAM và gọi `fork()`, Kernel không copy 1GB dữ liệu mà chỉ copy Page Table và đánh dấu các trang là Read-Only. Khi tiến trình con ghi vào 1 byte dữ liệu, chuỗi sự kiện phần cứng nào (MMU, Page Fault Exception) diễn ra để Kernel cấp phát riêng trang bộ nhớ 4KB mới?
2. **Ranh giới nạp mã của `execve`**:
   Khi gọi `execve()`, những thành phần nào của tiến trình cũ bị xóa sổ (Text, Data, Heap, Stack)? Những tài nguyên nào của hệ điều hành vẫn được giữ nguyên qua lời gọi `execve()` (PID, Bảng File Descriptors, Thư mục làm việc hiện tại, Signal Masks)?
3. **Cạm bẫy kế thừa File Descriptor và `FD_CLOEXEC`**:
   Nếu một tiến trình mở file bảo mật (hoặc socket nhạy cảm) mà quên gán cờ `FD_CLOEXEC` (hoặc `O_CLOEXEC`) trước khi gọi `execve()`, chương trình mới được nạp vào có quyền truy cập vào file đó hay không? Hậu quả an ninh hệ thống là gì?
4. **Bản chất bất đồng bộ của Signal và Async-Signal-Safety**:
   Tại sao trong hàm xử lý tín hiệu (Signal Handler), việc gọi các hàm cấp phát bộ nhớ như `malloc()`/`free()` hoặc hàm I/O chuẩn như `printf()` bị cấm tuyệt đối và có thể gây Deadlock? Danh sách các hàm `Async-Signal-Safe` (`man 7 signal-safety`) được Kernel đảm bảo điều gì?
5. **Hiện tượng thất thoát tín hiệu (Signal Coalescing / Non-queued Signals)**:
   Các tín hiệu chuẩn POSIX (từ 1 đến 31) không có hàng đợi (không xếp hàng). Nếu 10 tiến trình con cùng kết thúc tại cùng một thời điểm, Kernel chỉ bật 1 bit đại diện cho `SIGCHLD` trong pending mask của cha. Tại sao nếu chỉ gọi `waitpid()` một lần trong handler sẽ khiến 9 tiến trình con còn lại vĩnh viễn biến thành Zombie?
6. **Tại sao `cd` bắt buộc phải là Built-in Command?**:
   Thư mục làm việc hiện tại (`Current Working Directory - CWD`) được Kernel lưu ở đâu trong `task_struct`? Nếu cài đặt lệnh `cd` bằng cách `fork()` và gọi một binary `/bin/cd` bên ngoài, tại sao tiến trình Shell cha không hề thay đổi thư mục?

---

## 3. BÀI TOÁN ĐÁNH ĐỔI & ĐỊNH LƯỢNG (QUANTITATIVE TRADE-OFFS)

* **Chi phí COW Latency**:
  Viết microbenchmark cấp phát một mảng bộ nhớ lớn ($100\text{ MB}$, $500\text{ MB}$, $1\text{ GB}$), đo thời gian gọi `fork()` thuần túy (khi chưa ghi) so với thời gian ghi đè dữ liệu lên mảng đó ở tiến trình con. Xác định chi phí Page Fault xử lý COW của Linux Kernel.
* **Chi phí `execve` vs. Function Call**:
  Đo thời gian trung bình của một lời gọi hàm C++ thông thường so với thời gian của chuỗi thao tác `fork() + execve(/bin/true) + waitpid()`. Đánh giá sự chênh lệch (order of magnitude) giữa chuyển ngữ cảnh mức hàm trong User Space và chuyển giao quyền nạp file nhị phân qua Kernel.

---

## 4. MỤC TIÊU CODE LAB & THỬ THÁCH BREAK-CHECK (LAB TARGETS)

### Lab 1: Signal Interceptor & Robust Child Reaper (`src/lab1_signal.cpp`)
* **Mục tiêu**:
  * Thiết lập bắt tín hiệu `SIGINT` (`Ctrl + C`) sao cho Shell không bị thoát mà chỉ hủy tiến trình con đang chạy.
  * Cài đặt handler cho `SIGCHLD` dọn dẹp toàn bộ tiến trình con đã chết bằng vòng lặp không chặn:
    ```cpp
    while (waitpid(-1, &status, WNOHANG) > 0);
    ```
* **Break-Check 1 (Zombie Apocalypse)**:
  * Viết vòng lặp tạo 100 tiến trình con thoát ngay lập tức (`exit(0)`) nhưng vô hiệu hóa `waitpid()` và handler `SIGCHLD`.
  * Quan sát sự bùng nổ của tiến trình `Zombie` qua lệnh `ps aux | grep 'Z'`.
  * Đọc file `/proc/sys/kernel/pid_max` để xác định giới hạn tối đa trước khi Kernel từ chối tạo tiến trình mới với lỗi `EAGAIN`.
* **Break-Check 2 (Lost Signal Simulation)**:
  * Cho 50 tiến trình con cùng exit một lúc. Trong signal handler, cố tình chỉ gọi `waitpid()` đúng 1 lần duy nhất (không dùng vòng lặp `while`).
  * Kiểm tra số lượng Zombie còn sót lại trên hệ thống và ghi nhận kết quả.

### Lab 2: Multi-Stage Pipeline Engine (`src/lab2_pipeline.cpp`)
* **Mục tiêu**:
  * Viết hàm thực thi chuỗi lệnh đa tầng bất kỳ: `cmd_1 | cmd_2 | ... | cmd_N` với $N \ge 3$.
  * Tự động khởi tạo $N - 1$ đường ống `pipe()`, kết nối đúng đầu đọc/ghi của từng tiến trình qua `dup2()`.
  * Đóng sạch sẽ toàn bộ các đầu pipe không dùng ở cả cha và các con.
* **Break-Check 3 (Pipe Hang / Leak Write End)**:
  * Trong một pipeline gồm 2 lệnh `cat /dev/urandom | head -c 100`, cố tình bỏ quên lệnh đóng đầu ghi `pipe[1]` ở tiến trình cha.
  * Quan sát hiện tượng tiến trình đọc bị treo vô hạn (Deadlock) vì không bao giờ nhận được tín hiệu `EOF`.

---

## 5. NHIỆM VỤ OBSERVABILITY & DEBUGGING (INVESTIGATION TASKS)

Thực hiện các thao tác điều tra bằng công cụ hệ thống và ghi lại kết quả:
1. **Theo dõi dòng đời tiến trình qua `strace -f`**:
   Chạy `strace -f -e trace=clone,fork,execve,wait4,dup2,pipe,close ./minishell`.
   *Mục tiêu phân tích*: Phân biệt được sự kiện diễn ra ở PID cha và PID con; kiểm chứng thứ tự gọi `pipe` $\rightarrow$ `fork` $\rightarrow$ `dup2` $\rightarrow$ `close` $\rightarrow$ `execve`.
2. **Xác minh không rò rỉ File Descriptor**:
   Chạy lệnh trên Shell của Sir, sau đó kiểm tra thư mục `/proc/<PID_SHELL>/fd`:
   *Mục tiêu*: Số lượng FD đang mở của Shell bắt buộc phải luôn là **3** (`0 -> /dev/pts/*`, `1 -> /dev/pts/*`, `2 -> /dev/pts/*`). Bất kỳ FD số 3, 4, 5... còn sót lại sau khi lệnh kết thúc đều bị tính là vi phạm rò rỉ tài nguyên Kernel.
3. **Kiểm tra trạng thái tiến trình qua `/proc`**:
   Sử dụng `cat /proc/<PID>/status | grep -E "State|PPid|TracerPid"` và `cat /proc/<PID>/wchan` để chẩn đoán trạng thái thực thi khi tiến trình bị treo.

---

## 6. ĐẶC TẢ ĐỒ ÁN CỘT MỐC: STRICT POSIX MINI-SHELL (`[GATE 1]`)

### 6.1. Mục Tiêu Hành Vi Của Shell
Xây dựng một Shell thực thi dòng lệnh hoàn chỉnh từ con số 0 bằng C++20, quản lý qua CMake:
* **REPL Interface**: In dấu nhắc lệnh (`minishell$ `), đọc và parse dòng lệnh.
* **Built-in Commands**:
  * `cd <path>`: Thay đổi thư mục (bằng syscall `chdir`), cập nhật `PWD`.
  * `pwd`: In thư mục làm việc hiện tại (bằng syscall `getcwd`).
  * `exit [code]`: Dọn dẹp tài nguyên và thoát tiến trình shell với mã tương ứng.
* **External Command Execution**:
  * Tìm kiếm và thực thi file nhị phân qua đường dẫn tuyệt đối, tương đối hoặc qua biến môi trường `PATH` (sử dụng `execvp`).
* **I/O Redirection**:
  * Chuyển hướng đầu vào: `< input.txt`
  * Chuyển hướng đầu ra ghi đè: `> output.txt` (cờ `O_WRONLY | O_CREAT | O_TRUNC`)
  * Chuyển hướng đầu ra ghi tiếp: `>> append.txt` (cờ `O_WRONLY | O_CREAT | O_APPEND`)
* **Pipeline Execution**:
  * Hỗ trợ không giới hạn số lượng pipe: `cmd1 | cmd2 | cmd3 | ... | cmdN`.
* **Signal Resilience**:
  * Nhấn `Ctrl + C` (`SIGINT`) chỉ hủy tiến trình foreground đang chạy, Shell chính không bị tắt.

### 6.2. Tiêu Chuẩn Kỹ Thuật (Engineering Constraints)
* Quản lý toàn bộ File Descriptors bằng RAII wrapper (`UniqueFd`). Tuyệt đối không để rò rỉ FD (`FD Leak`).
* Biên dịch sạch 100% không warning với bộ cờ nghiêm ngặt:
  ```cmake
  target_compile_options(minishell PRIVATE
      -std=c++20 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror
      -O0 -g -fsanitize=address,undefined
  )
  ```

---

## 7. TIÊU CHUẨN NGHIỆM THU ĐÓNG `[GATE 1]` (PASS/FAIL CHECKLIST)

Sir chỉ hoàn thành Phase 1 khi vượt qua toàn bộ 6 tiêu chí:

* [ ] **1. Knowledge Check**: Trình bày chính xác không nhìn tài liệu cơ chế COW của `fork()`, ranh giới nạp mã của `execve()`, bảng 3 tầng FD Table / Open File Table / Inode Table, và cơ chế phát hiện EOF trên Pipe.
* [ ] **2. Implementation Check**: Mini-Shell chạy thành công chuỗi lệnh phức tạp kết hợp cả redirection và đa pipe:
  `cat /etc/passwd | grep -v nobody | cut -d: -f1 | sort > users.txt`
* [ ] **3. Break Check**: Bẫy thành công lỗi file không tồn tại (`cat < khong_co.txt`), lệnh không tồn tại (`lenh_la`), và đảm bảo Shell in lỗi chính xác qua `perror()` / `strerror(errno)` mà không bị sập.
* [ ] **4. Debugging Check**: Chứng minh bằng `valgrind --track-fds=yes` và `strace -f` rằng Shell không có bất kỳ bộ nhớ rò rỉ nào và không còn File Descriptor nào mở sót lại sau khi thực thi lệnh.
* [ ] **5. Reasoning Check**: Giải thích được tại sao khi tiến trình con ghi vào một pipe đã đóng hết đầu đọc, Kernel lại gửi tín hiệu `SIGPIPE` thay vì chỉ đơn giản trả về lỗi qua giá trị của `write()`.
* [ ] **6. Transfer Check**: Vận dụng kiến thức Process Lifecycle để giải thích cơ chế khởi tạo tiến trình Daemon trong Linux (Double Forking để thoát Process Group và chuyển Session ID bằng `setsid()`).
