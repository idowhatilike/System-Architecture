# 📘 TỔNG HỢP TOÀN DIỆN PHASE 1: PROCESS, SYSCALLS, LDE & PROCESS API

**Chương trình**: Systems Engineering Core Curriculum  
**Giai đoạn**: Phase 1 (Tháng 3 – Tháng 4)  
**Mục tiêu tối thượng**: Đóng **`[GATE 1]: Strict POSIX Mini-Shell`**  
**Tài liệu tham khảo chuẩn**: OSTEP (Ch. 4, 5, 6), TLPI (Ch. 6, 24, 25, 26, 27), Linux Man Pages (`man 2 syscalls`, `man 2 fork`, `man 2 execve`, `man 2 waitpid`, `man 2 pipe`, `man 2 dup2`, `man 2 sigaction`).

---

## MỤC LỤC
1. [Bản Chất Cốt Lõi & Mô Hình Tư Duy (Mental Models)](#1-bản-chất-cốt-lõi--mô-hình-tư-duy-mental-models)
2. [Hạ Tầng Kernel: Không Gian Địa Chỉ & Ranh Giới LDE](#2-hạ-tầng-kernel-không-gian-địa-chỉ--ranh-giới-lde)
3. [Bảng Quản Lý File Descriptors (3 Tầng Kernel Table)](#3-bảng-quản-lý-file-descriptors-3-tầng-kernel-table)
4. [Bộ Ba API Tiến Trình (Fork - Exec - Wait) & IPC Pipe](#4-bộ-ba-api-tiến-trình-fork---exec---wait--ipc-pipe)
5. [Tổng Hợp Thực Nghiệm & Đo Kiểm Đã Hoàn Thành](#5-tổng-hợp-thực-nghiệm--đo-kiểm-đã-hoàn-thành)
6. [Nội Dung Bổ Sung Bắt Buộc Để Qua Gate 1 (Signals & Job Control)](#6-nội-dung-bổ-sung-bắt-buộc-để-qua-gate-1-signals--job-control)
7. [Đặc Tả Đồ Án Cột Mốc: Strict Mini-Shell (`[GATE 1]`)](#7-đặc-tả-đồ-án-cột-mốc-strict-mini-shell-gate-1)
8. [Khung Tự Đánh Giá Exit Gate 1 (Self-Assessment Framework)](#8-khung-tự-đánh-giá-exit-gate-1-self-assessment-framework)

---

## 1. BẢN CHẤT CỐT LÕI & MÔ HÌNH TƯ DUY (MENTAL MODELS)

### 1.1. Program vs. Process
* **Program (Chương trình)**: Là tập hợp các byte tĩnh nằm trên đĩa cứng (dưới định dạng nhị phân như ELF trên Linux), chứa mã máy (`.text`), biến toàn cục khởi tạo (`.data`), kích thước bộ nhớ chưa khởi tạo (`.bss`) và các bảng ký hiệu (symbols).
* **Process (Tiến trình)**: Là một thể hiện đang thực thi (running instance) của một chương trình được cấp phát tài nguyên động trong bộ nhớ bởi Hệ điều hành:
  * Không gian địa chỉ ảo độc lập (Virtual Address Space).
  * Trạng thái máy (Machine State): Thanh ghi CPU (Program Counter/RIP, Stack Pointer/RSP, Frame Pointer/RBP, General Registers).
  * Quyền sở hữu tài nguyên Kernel: Bảng File Descriptors, PID, PPID, UID, GID, Signal Handlers.

```
+-------------------------------------------------------------+
| DISK (Tĩnh)                RAM (Động - Virtual Address Space)|
| +-----------------+         +-----------------------------+ |
| | ELF Header      |         | User Stack (Phát triển xuống)| |
| | .text (code)    | =====>  |              |              | |
| | .data (init)    |  Nạp    |              v              | |
| | .bss (uninit)   | (execve)| Heap (Phát triển lên qua brk)| |
| +-----------------+         | .bss / .data                | |
|                             | .text (Read-only / Executable) |
+-------------------------------------------------------------+
```

### 1.2. Vòng Đời & Chuyển Dịch Trạng Thái Tiến Trình (Process State Machine)
Mỗi tiến trình trong Kernel được đại diện bởi một cấu trúc **`task_struct`** (thường gọi là Process Control Block - PCB) và chuyển dịch giữa các trạng thái cơ bản:
* **Initial (Khởi tạo)**: Được cấp phát PCB thông qua `fork()`, đang nạp mã hoặc kế thừa tài nguyên.
* **Ready (Sẵn sàng)**: Đã có đủ mọi tài nguyên, nằm trong hàng đợi của Bộ lập lịch (Runqueue), chỉ đợi được cấp CPU.
* **Running (Đang chạy)**: Được CPU nạp ngữ cảnh và đang trực tiếp thực thi lệnh máy.
* **Blocked / Sleeping (Tạm gác)**: Bị tước CPU và đưa vào hàng đợi chờ (*Wait Queue*) do chờ I/O, đọc pipe rỗng, chờ lock hoặc phát lệnh `sleep()`. Gồm 2 mức:
  * `TASK_INTERRUPTIBLE` (`S`): Ngủ nhưng bị đánh thức nếu có Signal gửi tới.
  * `TASK_UNINTERRUPTIBLE` (`D`): Ngủ sâu (thường chờ I/O đĩa/thiết bị), miễn nhiễm với mọi Signal kể cả `SIGKILL`.
* **Zombie (`Z`)**: Tiến trình đã gọi `exit()` hoặc bị hủy bởi signal. Toàn bộ bộ nhớ ảo và File Descriptors đã được Kernel thu hồi, **chỉ còn giữ lại PCB trong Process Table** chứa `exit status` để chờ cha đọc.
* **Orphan (Mồ côi)**: Tiến trình cha kết thúc trước khi con kết thúc. Tiến trình con tự động được chuyển quyền giám hộ (reparent) cho tiến trình gốc `init` / `systemd` (PID 1). PID 1 có trách nhiệm tự động gọi `wait()` để dọn dẹp các tiến trình con này khi chúng thoát.

---

## 2. HẠ TẦNG KERNEL: KHÔNG GIAN ĐỊA CHỈ & RANH GIỚI LDE

### 2.1. Limited Direct Execution (LDE Protocol)
Hệ điều hành ảo hóa CPU bằng kỹ thuật **LDE**: Cho phép mã người dùng chạy trực tiếp trên phần cứng CPU để đạt hiệu năng tối đa, nhưng đặt ra các giới hạn bảo vệ nghiêm ngặt thông qua phần cứng.

```
User Space (Ring 3)        Kernel Space (Ring 0)
Mã ứng dụng               Kernel Code / Trap Handlers
    |                                   ^
    |--- 1. Phát lệnh 'syscall' --------| (Phần cứng nâng quyền Ring 0,
    |    (Mã định danh trong RAX,        |  Lưu RIP, RSP, RFLAGS vào Kernel Stack)
    |     tham số trong RDI, RSI, RDX...)
    |                                   |--- 2. Tra cứu Syscall Table
    |                                   |--- 3. Thực thi logic trong Kernel
    |<-- 4. Lệnh 'sysret' --------------| (Phần cứng hạ quyền Ring 3,
    |    (Trả về kết quả trong RAX)         Khôi phục RIP, RSP từ Kernel Stack)
    v
Chạy tiếp
```

### 2.2. Mode Switch vs. Context Switch
Đây là hai khái niệm tuyệt đối không được nhầm lẫn:
* **Mode Switch (Chuyển mức đặc quyền: Ring 3 $\leftrightarrow$ Ring 0)**:
  * Xảy ra khi: Tiến trình phát sinh `syscall`, nhận ngắt phần cứng (`interrupt`), hoặc gặp lỗi CPU (`trap/fault` như chia cho 0, Page Fault).
  * Tiến trình thực thi **không đổi**. CPU chỉ đổi thanh ghi cờ đặc quyền (`CS`), trỏ ngăn xếp sang `Kernel Stack` của chính tiến trình đó, và lưu các thanh ghi User-mode tạm thời vào `k-stack`.
  * Chi phí: Rất nhỏ (~100 – 300 chu kỳ CPU).
* **Context Switch (Chuyển ngữ cảnh tiến trình: Process A $\rightarrow$ Process B)**:
  * Xảy ra khi: Bộ lập lịch (Scheduler) quyết định dừng Process A để nạp Process B (do hết quantum thời gian từ Timer Interrupt, hoặc Process A bị Block do chờ I/O).
  * Quy trình:
    1. Lưu toàn bộ thanh ghi phần cứng của Process A vào PCB/`task_struct` của A.
    2. Đổi con trỏ `CR3` sang Page Directory của B (gây xả TLB Cache - TLB Flush).
    3. Đổi Stack Pointer (`RSP`) sang `Kernel Stack` của B.
    4. Gọi `return-from-trap` / `sysret` để nạp thanh ghi và đưa CPU về chạy mã của B ở Ring 3.
  * Chi phí: Rất lớn (~vài $\mu s$, làm mất tính định vị Cache L1/L2/L3 và TLB miss hàng loạt).

### 2.3. Hai Cơ Chế Giành Quyền Kiểm Soát Của OS
1. **Cooperative (Cộng tác - Cổ điển)**: OS chờ tiến trình tự nguyện nhượng quyền qua syscall (như `yield()`) hoặc khi tiến trình gây lỗi sập (chia cho 0, segfault). Nhược điểm: Nếu tiến trình chạy vòng lặp vô hạn `while(1)` không gọi syscall, OS mất hoàn toàn quyền kiểm soát CPU.
2. **Non-Cooperative (Bất hợp tác - Hiện đại)**: OS cấu hình **Timer Interrupt** tại thời điểm boot. Cứ sau mỗi vài mili-giây, phần cứng phát ngắt đồng hồ, buộc CPU trao quyền cho Interrupt Handler của Kernel. Tại đây, Scheduler được kích hoạt để quyết định có thực hiện Context Switch hay không.

---

## 3. BẢNG QUẢN LÝ FILE DESCRIPTORS (3 TẦNG KERNEL TABLE)

Mỗi File Descriptor (`int fd`) thực chất chỉ là một **chỉ số (index)** trỏ vào cấu trúc dữ liệu nhiều tầng trong Kernel:

```
[Tiến trình A (PCB)]
FD Table:
0: stdin
1: stdout
2: stderr
3: [con trỏ] --------+
                      |
[Tiến trình B (PCB)]  v
FD Table:             [Open File Table (Hệ thống)]
3: [con trỏ] --------> Entry X:
                       - File Offset: 1024
                       - Status Flags: O_RDONLY
                       - Reference Count: 2
                              |
                              v
                      [Inode Table (Hệ thống)]
                      Inode Entry:
                      - File Type, Permissions
                      - File Size, Data Blocks trên Disk
```

### 3 Tầng Bảng:
1. **Per-Process File Descriptor Table**: Nằm trong PCB (`task_struct->files`). Mỗi tiến trình có một bảng riêng. Quản lý cờ riêng của FD (như `FD_CLOEXEC`).
2. **System-Wide Open File Table**: Được chia sẻ chung toàn hệ thống. Một entry được tạo ra mỗi khi gọi `open()`. Quản lý: **Con trỏ đọc/ghi hiện tại (Current File Offset)**, quyền mở file (`O_RDWR`, `O_APPEND`), và biến đếm tham chiếu (`ref_count`).
3. **System-Wide Inode Table**: Đại diện cho file vật lý trên hệ thống tệp. Quản lý metadata thực tế của file trên đĩa cứng.

### Hệ Quả Sâu Sắc:
* Khi gọi `fork()`, bảng FD của cha được sao chép nguyên vẹn sang con. **Cả cha và con cùng trỏ vào CHUNG một entry trong Open File Table**. Do đó, cha đọc 5 byte làm con trỏ offset tăng lên 5, con đọc tiếp sẽ đọc từ byte thứ 6.
* Khi gọi `dup2(oldfd, newfd)`: Gán chỉ số `newfd` trỏ vào cùng Open File entry với `oldfd`. Đây là nền tảng của kỹ thuật chuyển hướng I/O (Redirection).

---

## 4. BỘ BA API TIẾN TRÌNH (FORK - EXEC - WAIT) & IPC PIPE

### 4.1. `fork()`: Phân Nhánh Tiến Trình
```cpp
pid_t pid = fork();
if (pid < 0) { /* Lỗi */ }
else if (pid == 0) { /* Mã tiến trình Con */ }
else { /* Mã tiến trình Cha (pid giữ PID của con) */ }
```
* **Cơ chế Copy-on-Write (COW)**: Khi `fork()`, Kernel không sao chép toàn bộ RAM của cha sang con ngay (vì quá tốn kém). Kernel chỉ sao chép Page Table và đánh dấu tất cả các trang bộ nhớ là **Read-Only**. Chỉ khi nào cha hoặc con thực hiện ghi dữ liệu vào một trang, phần cứng phát sinh Page Fault, Kernel mới nhân bản riêng trang đó ra vùng nhớ mới.

### 4.2. `execve()` / Họ Hàm `exec*`: Thay Thế Không Gian Thực Thi
```cpp
char* args[] = {(char*)"ls", (char*)"-l", nullptr};
execvp(args[0], args);
// Nếu chạy đến đây là exec thất bại!
perror("execvp failed");
exit(1);
```
* `exec` **không tạo tiến trình mới**. Nó giữ nguyên PID, PPID và bảng FD (trừ các FD có cờ `FD_CLOEXEC`), nhưng xóa sạch toàn bộ ngăn xếp (Stack), Heap, Data Segment của chương trình cũ và nạp chương trình mới từ đĩa vào thực thi từ đầu.

### 4.3. `wait()` / `waitpid()`: Đồng Bộ Hóa & Thu Dọn Zombie
```cpp
int status;
pid_t child_pid = waitpid(target_pid, &status, WUNTRACED | WCONTINUED);
if (WIFEXITED(status)) {
    int exit_code = WEXITSTATUS(status);
} else if (WIFSIGNALED(status)) {
    int term_sig = WTERMSIG(status);
}
```
* Nếu cha không bao giờ gọi `wait()`, tiến trình con khi chết sẽ mãi mãi tồn tại dưới dạng **Zombie (`Z`)**, chiếm dụng bảng PID của Kernel.

### 4.4. Cơ Chế Anonymous Pipe (`pipe(int fd[2])`)
* Pipe là một vùng đệm vòng (circular buffer - thường là 64KB trên Linux) được cấp phát hoàn toàn trong bộ nhớ Kernel (`pipefs`).
* `fd[0]` là đầu đọc (`read_end`), `fd[1]` là đầu ghi (`write_end`).
* **Quy tắc vàng về EOF trên Pipe**:
  * Hàm `read()` trên pipe chỉ trả về `0` (`EOF`) khi và chỉ khi **TẤT CẢ các bản sao của đầu ghi `write_end` trên toàn bộ các tiến trình đã bị đóng hoàn toàn (`close`)**.
  * Nếu một tiến trình vừa đọc vừa giữ đầu ghi, nó sẽ không bao giờ nhận được `EOF` và rơi vào trạng thái treo vô hạn (**Deadlock**).
* **Quy tắc về Broken Pipe**: Ghi vào một pipe mà tất cả các đầu đọc đã bị đóng sẽ khiến Kernel gửi tín hiệu **`SIGPIPE`** (mặc định làm sập tiến trình) và lệnh `write()` trả về `-1` với `errno = EPIPE`.

---

## 5. TỔNG HỢP THỰC NGHIỆM & ĐO KIỂM ĐÃ HOÀN THÀNH

Trong chặng đường vừa qua, Sir đã hiện thực và kiểm chứng trên máy thật các bài toán:

| Bài Lab / Module | File Mã Nguồn | Khái Niệm Kiểm Chứng Thành Công |
| :--- | :--- | :--- |
| **OSTEP Ch.4 Homework** | `cpu-intro/process-run.py` | Mô phỏng Scheduler, trạng thái Ready/Running/Blocked và chi phí I/O. |
| **OSTEP Ch.5 API Suite** | `src/I_5_1.cpp` $\rightarrow$ `I_5_8.cpp` | Biến sau `fork()`, chia sẻ FD khi mở file trước `fork()`, dùng `dup2()` chuyển hướng `stdout`, nối 2 tiến trình con qua `pipe()`. |
| **Syscall Latency (Task A)** | `src/measure_syscall.cpp` | Đo chi phí của 1 syscall rỗng `read(STDIN_FILENO, nullptr, 0)`: ~300–400 ns. |
| **Buffering vs Direct (Task B)**| `src/stage_0.cpp`, `lab/benchmark.cpp` | So sánh `write()` 1 byte (1,000,000 syscalls) vs `fwrite()` (đệm 4KB gom thành 244 syscalls) $\rightarrow$ chênh lệch hiệu năng 30–50 lần. |
| **Context Switch Cost (Task C)**| `src/measure_ctx_switch.cpp` | Ép tiến trình chạy trên Core 0 (`sched_setaffinity`), truyền byte qua 2 pipe và đo thời gian 1 lần context switch (~2–4 $\mu s$). |
| **RAII Wrapper** | `lab/unique_Fd.hpp` | Viết class `uniqueFd` độc quyền sở hữu (`Rule of Five`), tự đóng FD bằng `close()` khi ra khỏi scope; bọc `scopedPipe`. |
| **Observability (`strace`)** | CLI `strace -c` | Đo độ nhiễu (Observer Effect) của cơ chế `ptrace`: làm chậm chương trình 300–500 lần do 2 lần context switch mỗi syscall. |
| **Observability (`valgrind`)**| `valgrind --track-fds=yes` | Xác minh 0 rò rỉ bộ nhớ, 0 rò rỉ File Descriptor (`4 open / 4 inherited at exit`). |
| **Fault Injection (Break-Check 2)**| `lab/benchmark.cpp` | Cố tình đảo thứ tự đọc pipe $\rightarrow$ bắt trạng thái Deadlock `State: S (sleeping)` và kênh đợi Kernel `wchan: anon_pipe_read`. |

---

## 6. NỘI DUNG BỔ SUNG BẮT BUỘC ĐỂ QUA GATE 1 (SIGNALS & JOB CONTROL)

Để bước từ các bài thực nghiệm rời rạc lên một sản phẩm hoàn chỉnh là **Strict Mini-Shell**, Sir cần làm chủ 3 mắt xích cuối cùng:

### 6.1. Xử Lý Tín Hiệu Hệ Điều Hành (POSIX Signals)
Tín hiệu là cơ chế ngắt phần mềm bất đồng bộ (Asynchronous Software Interrupt) do Kernel gửi tới tiến trình:
* **`SIGINT` (`Ctrl + C`)**: Tín hiệu ngắt. Shell cha **không được phép chết** khi người dùng bấm `Ctrl + C`, chỉ có tiến trình con đang chạy ở tiền cảnh (foreground) bị dừng.
* **`SIGTSTP` (`Ctrl + Z`)**: Tín hiệu tạm dừng tiến trình đưa vào background (`State: T (stopped)`).
* **`SIGCHLD`**: Được Kernel tự động gửi cho tiến trình cha mỗi khi một tiến trình con kết thúc, bị dừng hoặc tiếp tục chạy.
  * **Cạm bẫy tín hiệu**: Các signal chuẩn trong POSIX **không có hàng đợi (Non-queued)**. Nếu 5 tiến trình con cùng chết cùng một lúc, Kernel chỉ gửi đúng 1 tín hiệu `SIGCHLD`. Do đó, trong handler của `SIGCHLD` bắt buộc phải thu dọn bằng vòng lặp `while(waitpid(-1, &status, WNOHANG) > 0)` để tránh tạo ra zombie!

### 6.2. Thiết Lập Signal Bằng `sigaction` (Thay vì `signal()` cũ)
```cpp
struct sigaction sa;
sa.sa_handler = sigchld_handler;
sigemptyset(&sa.sa_mask);
sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
sigaction(SIGCHLD, &sa, nullptr);
```
* Cờ **`SA_RESTART`**: Tự động phục hồi các syscall bị gián đoạn (như `read()` trên terminal) thay vì trả về lỗi `EINTR`.

### 6.3. Chuỗi Đường Ống Đa Tầng (Multi-Command Pipeline)
Thay vì chỉ nối 1 pipe giữa 2 lệnh (`cmd1 | cmd2`), Shell chuẩn phải hỗ trợ chuỗi $N$ lệnh:
```bash
cat /etc/passwd | grep -v nobody | cut -d: -f1 | sort
```
* **Thuật toán vòng lặp Pipeline**:
  Với $N$ lệnh, cần $N - 1$ đường ống `pipe()`.
  Lệnh thứ $i$ ($0 \le i < N$):
  - Nếu $i > 0$: Đổi `stdin` thành đầu đọc của pipe $i - 1$ (`dup2(pipes[i-1][0], STDIN_FILENO)`).
  - Nếu $i < N - 1$: Đổi `stdout` thành đầu ghi của pipe $i$ (`dup2(pipes[i][1], STDOUT_FILENO)`).
  - Đóng **toàn bộ** các đầu pipe trong tiến trình con sau khi đã `dup2`.
  - Tiến trình cha đóng các đầu pipe đã dùng xong và gọi `waitpid()` cho toàn bộ $N$ tiến trình con.

---

## 7. ĐẶC TẢ ĐỒ ÁN CỘT MỐC: STRICT MINI-SHELL (`[GATE 1]`)

### 7.1. Kiến Trúc Phân Lớp Của Shell
```
+-----------------------------------------------------------+
| 1. REPL Loop: In dấu nhắc (Prompt: "minishell$ ")         |
| 2. Input Reader: Đọc chuỗi ký tự an toàn (std::string)   |
| 3. Parser / Tokenizer: Tách chuỗi thành tokens & AST      |
|    - Phân tách lệnh theo dấu pipe '|'                     |
|    - Phân tách tham số và cờ chuyển hướng '<', '>', '>>'  |
+-----------------------------------------------------------+
                             |
                             v
+-----------------------------------------------------------+
| 4. Built-in Checker:                                      |
|    - 'exit': Dọn dẹp tài nguyên và thoát shell            |
|    - 'cd <path>': Thay đổi thư mục (bằng syscall chdir)   |
|    - 'pwd': In thư mục làm việc hiện tại                  |
+-----------------------------------------------------------+
                             | (Nếu không phải built-in)
                             v
+-----------------------------------------------------------+
| 5. Process Executor:                                      |
|    - Fork child process(es)                               |
|    - Thiết lập redirection (dup2) & pipe chéo            |
|    - Gọi execvp()                                         |
| 6. Process Supervisor:                                    |
|    - Thu hồi child processes (waitpid), bắt exit code    |
+-----------------------------------------------------------+
```

### 7.2. Tiêu Chuẩn Kỹ Thuật Bắt Buộc (Strict Requirements)
1. **Quản lý tài nguyên bằng RAII**: Toàn bộ các File Descriptors phát sinh khi mở file redirect hoặc pipe phải được bọc trong class `uniqueFd` / `scopedPipe` của Sir.
2. **Không Zombie**: Đảm bảo dọn dẹp sạch 100% tiến trình con trong mọi kịch bản (chạy bình thường, lệnh bị lỗi, hoặc bị ngắt bởi `SIGINT`).
3. **Không Leak File Descriptors**: Khi chạy kiểm tra bằng `ls -l /proc/$$/fd` trên chính Shell của Sir, số lượng FD đang mở luôn giữ nguyên ở trạng thái ban đầu (`0, 1, 2`) sau khi bất kỳ lệnh con nào thực thi xong.
4. **Toolchain Compilation**: Biên dịch sạch sẽ 100% với:
   ```bash
   cmake -B build -S .
   cmake --build build
   # Flags: -std=c++20 -Wall -Wextra -Wpedantic -Werror -fsanitize=address,undefined
   ```

---

## 8. KHUNG TỰ ĐÁNH GIÁ EXIT GATE 1 (SELF-ASSESSMENT FRAMEWORK)

Sir chỉ được xem là chính thức vượt qua **`[GATE 1]`** khi tự kiểm tra và đạt đầy đủ 6 tiêu chí:

* [ ] **1. Knowledge Check**: Trình bày rành mạch, không nhìn tài liệu chuỗi sự kiện phần cứng và kernel khi gọi `fork()` $\rightarrow$ `execve()` $\rightarrow$ `waitpid()`; phân biệt được Inode Table và Open File Table.
* [ ] **2. Implementation Check**: Mini-Shell thực thi mượt mà các lệnh ngoại vi phức tạp kèm redirect và multi-pipe (ví dụ: `cat file.txt | grep error | wc -l > out.log`).
* [ ] **3. Break Check**: Thử cố tình truyền file không tồn tại vào lệnh redirect đầu vào (`< non_exist.txt`), kiểm tra shell bắt đúng lỗi, không bị crash, in `strerror(errno)` và tiếp tục vòng lặp REPL.
* [ ] **4. Debugging Check**: Sử dụng `strace -f ./minishell` để theo dõi chính xác từng syscall `clone`/`fork`, `dup2`, `execve`, `close` của cả tiến trình cha và con.
* [ ] **5. Reasoning Check**: Giải thích được tại sao lệnh `cd` bắt buộc phải được cài đặt dưới dạng **Built-in command** thực thi trực tiếp trên tiến trình cha mà không thể `fork()` rồi gọi một file nhị phân bên ngoài.
* [ ] **6. Transfer Check**: Giải thích cơ chế Daemonization trong Linux: Làm sao một tiến trình có thể tự tách mình hoàn toàn khỏi Terminal quản lý để trở thành một Background Daemon chạy ngầm vĩnh viễn (Double Fork + `setsid()` + redirect `0, 1, 2` vào `/dev/null`).
