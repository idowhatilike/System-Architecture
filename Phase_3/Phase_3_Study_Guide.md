# Phase 3 — Concurrency Invariants, Communication Protocols & Contention

> **Ngân sách:** 75 giờ = 20 giờ đọc và mô hình hóa + 18 giờ lab + 25 giờ project và ADR + 12 giờ Exit Gate.
> **Lịch theo curriculum:** 23/11–10/12/2026.
> **Phạm vi:** [curriculum.md](../curriculum.md), mục 6–8.

## 1. Mục tiêu và cách sử dụng

Xây một pipeline C++ nhận byte từ TCP, ghép thành message và chuyển qua bounded queue cho worker xử lý. Với mỗi bước, xác định ai sở hữu state, invariant nào cần giữ và thread chờ điều kiện gì. Đọc cơ chế rồi làm lab tương ứng ở mục 9.

### Kiến thức kế thừa cần kiểm tra nhanh

| Từ Phase trước | Dùng ở Phase 3 |
| --- | --- |
| Process, VAS, FD và RAII | Phân biệt chia sẻ bộ nhớ với cô lập process; quản lý socket và vòng đời thread |
| Blocking và EOF trên pipe | Suy luận về chờ dữ liệu, chờ chỗ trống và kết thúc stream |
| Cache line, locality, giới hạn băng thông | Đặt giả thuyết về contention và false sharing |
| Đo thời gian, warm-up và biến động giữa các lần chạy | Tách kiểm tra correctness khỏi benchmark |

## 2. Phạm vi và nguồn đọc

### 2.1. Nguồn đọc

| Nguồn | Phần cần đọc |
| --- | --- |
| [OSTEP Ch. 28 — Locks](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf) | 28.1, 28.2 và 28.4. Nắm critical section, lý do cần mutual exclusion và tiêu chí đánh giá lock. |
| [OSTEP Ch. 30 — Condition Variables](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf) | 30.1–30.2. Lần theo các bản producer–consumer sai và bản sửa. |
| [Beej Ch. 5 — System Calls or Bust](https://beej.us/guide/bgnet/html/split/system-calls-or-bust.html) | 5.1–5.9 để nắm vòng đời socket và các syscall chính. |
| [Beej Ch. 6 — Client-Server Background](https://beej.us/guide/bgnet/html/split/client-server-background.html) | 6.1–6.2 về stream server/client. |
| [Beej 7.4 và 7.6](https://beej.us/guide/bgnet/html/split/slightly-advanced-techniques.html) | Đọc thêm hai mục về partial send và framing cho Project 3. |
| [OSTEP 32.3 — Deadlock Bugs](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-bugs.pdf) | Tra bốn điều kiện deadlock và lock ordering khi làm Lab 3.3. |

### 2.2. Tra cứu khi triển khai

- C++ working draft: [data races](https://eel.is/c++draft/intro.races), [mutex](https://eel.is/c++draft/thread.mutex.requirements), [condition_variable](https://eel.is/c++draft/thread.condition.condvar), [thread::join](https://eel.is/c++draft/thread.thread.member). Dùng tên clause; số chương có thể khác giữa các bản chuẩn.
- Linux man-pages: [recv(2)](https://man7.org/linux/man-pages/man2/recv.2.html) và [send(2)](https://man7.org/linux/man-pages/man2/send.2.html).
- [LLVM — ThreadSanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html): cách build và đọc báo cáo data race.

**Ngoài phạm vi:** lock-free structures, `memory_order_*` nâng cao, semaphore internals, tự cài futex, work-stealing, raw sockets, sửa IP/TCP headers và xây server dùng epoll. Event loop chỉ cần hiểu để so sánh mô hình.

## 3. Phân bổ 75 giờ

| Khối | Giờ | Công việc | Đầu ra |
| --- | ---: | --- | --- |
| Đọc và mô hình hóa | 20 | 3h mô hình thực thi; 6h shared state và locks; 5h CV và queue; 6h sockets và framing | Sơ đồ ownership, bảng invariant, contract I/O |
| Lab | 18 | Sáu lab, mỗi lab 3h, tại mục 9 | Chương trình nhỏ và ghi nhận Build → Break → Observe → Fix |
| Project và ADR | 25 | Các mốc tích hợp tại mục 10 | Pipeline hoàn chỉnh và bằng chứng kiểm chứng |
| Exit Gate độc lập | 12 | Giải thích 2h; sửa code 3h; dự đoán/đo 3h; phân tích lỗi và quyết định 4h | Tự bảo vệ được thiết kế |
| **Tổng** | **75** | | |

Trình tự đề xuất: mục 4–5 → Lab 3.1 → mục 6 → Lab 3.2–3.3 → mục 7 → Lab 3.4 → mục 8 → Lab 3.5–3.6 → project → Exit Gate. Thời gian lab không tính lại vào 25 giờ project.

## 4. Chọn mô hình thực thi từ bài toán

| Mô hình | Ownership và failure boundary | Chi phí/rủi ro cần cân nhắc | Khi nào đáng thử |
| --- | --- | --- | --- |
| Process | Address space riêng; giao tiếp qua IPC | Truyền dữ liệu, giám sát và thu dọn process | Cần cô lập lỗi hoặc đặc quyền |
| Thread | Heap/data dùng chung; stack thực thi riêng | Đồng bộ, shared-state corruption, lifecycle | Cần chia việc trong cùng process |
| Event loop | Handler xử lý sự kiện; có thể tập trung state vào một thread | Một handler chặn lâu sẽ giữ các việc khác; cần quản lý state qua nhiều sự kiện | Nhiều hoạt động chờ I/O, handler ngắn |

Khi chọn mô hình, cân nhắc số kết nối, lượng tính toán và mức cô lập cần thiết.

## 5. Shared state, invariant và mutex

### 5.1. Phân biệt hai loại vấn đề

Trong C++, các truy cập xung đột giữa thread, ít nhất một truy cập không atomic và không có quan hệ happens-before thích hợp, có thể tạo **data race**, dẫn tới undefined behavior. **Race condition** rộng hơn: kết quả logic sai do thứ tự thực thi, kể cả khi từng truy cập riêng đã đồng bộ. `volatile` không thay mutex để đồng bộ thread. Nguồn semantics: [C++ — intro.races](https://eel.is/c++draft/intro.races).

Giả sử `size()` và `enqueue()` khóa riêng, còn `enqueue()` không kiểm tra capacity. Hai producer có thể cùng thấy còn một chỗ rồi lần lượt enqueue, làm queue vượt `K`. Kiểm tra và cập nhật phải nằm trong cùng critical section.

### 5.2. Bắt đầu bằng bảng invariant

| State | Invariant/ownership |
| --- | --- |
| Container và capacity | `0 <= size <= K`; capacity cố định, `K > 0` |
| `closed` | Ban đầu false, chỉ chuyển sang true; cùng mutex với container |
| Message trong queue | Đã nhận đủ frame và đã chuyển ownership cho queue |
| Message đã pop | Worker giữ riêng; xử lý ngoài queue mutex |
| Kết quả worker | Mỗi worker ghi vào vùng riêng; coordinator đọc sau join |

Mutex giúp đồng bộ các truy cập tuân theo cùng giao thức khóa; nó không tự biết invariant nghiệp vụ. Khi một update đi qua trạng thái trung gian, các reader liên quan cũng phải tuân thủ giao thức đó. Tham khảo [C++ — mutex requirements](https://eel.is/c++draft/thread.mutex.requirements).

Dùng RAII cho lock. Trước mỗi critical section, tự trả lời: **đang bảo vệ quan hệ nào giữa các dữ liệu và thời điểm nào operation có hiệu lực?** Không giữ queue mutex trong lúc xử lý task hoặc chờ socket.

### 5.3. Contention và false sharing

Đặt hai giả thuyết riêng:

- **Contention:** nhiều worker cần cùng một lock hoặc cùng tài nguyên; tăng worker có thể chỉ tăng thời gian chờ.
- **False sharing:** các state độc lập về logic có thể nằm trên cùng cache line; các core ghi chúng vẫn có thể gây lưu lượng coherence.

Nối lại với mô hình cache đã học ở Phase 2; chưa cần học giao thức MESI chi tiết. Phân biệt bằng thiết kế thí nghiệm: trước hết kiểm tra phạm vi lock; sau đó mới xét bố trí dữ liệu. Không kết luận false sharing chỉ từ một lần chạy chậm.

## 6. Condition variable: chờ một điều kiện của state

`wait(lock)` nhả mutex và bắt đầu chờ như một thao tác nguyên tử; khi trả về đã lấy lại mutex. Nó có thể tỉnh do notification hoặc spuriously. Overload có predicate tương đương `while (!pred()) wait(lock)`. Notification không lưu một “vé” cho thread đến sau. Nguồn: [C++ — condition_variable](https://eel.is/c++draft/thread.condition.condvar).

Áp dụng vào contract đề xuất:

| Operation | Predicate để hết chờ | Việc phải quyết định dưới lock |
| --- | --- | --- |
| `push` | `closed || size < K` | Nếu closed thì từ chối; nếu không thì enqueue |
| `pop` | `closed || !empty` | Nếu còn item thì lấy; nếu closed và empty thì kết thúc |

Sau push thành công, đánh thức consumer qua `not_empty`; sau pop thành công, đánh thức producer qua `not_full`. Khi close, đánh thức tất cả trên cả hai CV. State thay đổi dưới mutex; có thể notify sau khi nhả mutex.

Nếu hai consumer cùng tỉnh và một consumer lấy item trước, consumer còn lại sẽ thấy queue rỗng khi lấy lại mutex. Hãy lần theo lịch này trong Lab 3.2.

## 7. Bounded queue và worker lifecycle

### 7.1. Contract của queue

- `push(std::string&& message) -> bool` chờ khi đầy. Nếu queue đã đóng, trả `false`. Caller giữ nguyên message và chịu trách nhiệm giải phóng. Khi enqueue thành công, queue nhận ownership và trả `true`. Nhận `std::string&&` để `push(std::move(msg))` chỉ thực sự move khỏi `msg` ở bước enqueue, sau khi kiểm tra `closed` và có chỗ dưới mutex.
- `pop() -> optional<message>` chờ khi rỗng. Trả item nếu còn, hoặc `nullopt` khi queue đã đóng và hết item.
- `close()` chuyển sang trạng thái đóng, từ chối push mới và để worker drain item đã nhận. Gọi lại vẫn an toàn.

### 7.2. Queue đầy và backpressure

Project chặn producer khi queue đầy. Nếu input đến nhanh hơn khả năng xử lý, producer sẽ tạm ngừng đọc thêm từ socket. Áp lực có thể lan tới sender sau khi các buffer đầy. Trong ADR, so sánh chi phí chờ của blocking với việc trả lỗi ngay khi dùng reject.

Giới hạn payload của queue là `K * MAX_PAYLOAD`. Khi ước lượng RAM, tính thêm overhead, frame producer đang giữ, task đang chạy và socket buffers.

### 7.3. Deadlock

Với ví dụ hai mutex: T1 giữ A chờ B; T2 giữ B chờ A. Bốn điều kiện cần xét là mutual exclusion, hold-and-wait, no preemption và circular wait. Quy tắc mọi nơi lấy A trước B loại vòng chờ của ví dụ này. Nguồn: [OSTEP 32.3](https://pages.cs.wisc.edu/~remzi/OSTEP/threads-bugs.pdf).

Một lỗi lifecycle khác cần tự tìm: coordinator giữ queue mutex rồi gọi join, còn worker cần mutex đó để hoàn tất. Hãy vẽ quan hệ chờ trước khi sửa; chỉ đếm số mutex là chưa đủ để nhận diện mọi deadlock.

## 8. TCP stream và protocol framing

### 8.1. Socket lifecycle

Server tạo socket → bind → listen → accept; accept trả connected FD riêng. Client tạo socket rồi connect. Listening FD phục vụ nhận kết nối, còn connected FD phục vụ truyền byte. Một lần send không tạo ranh giới message tương ứng ở recv. Nguồn: [Beej Ch. 5](https://beej.us/guide/bgnet/html/split/system-calls-or-bust.html) và [Ch. 6](https://beej.us/guide/bgnet/html/split/client-server-background.html).

Trên TCP, với yêu cầu đọc số byte dương: `recv > 0` là số byte đã đọc, `recv == 0` là EOF, `recv == -1` là lỗi cần xét errno. Retry khi `EINTR`; EOF giữa frame phải được phân biệt với EOF đúng ranh giới. Nguồn: [recv(2)](https://man7.org/linux/man-pages/man2/recv.2.html).

`send` có thể trả số byte nhỏ hơn yêu cầu; cần cập nhật offset và gửi phần còn lại. Thành công không chứng minh ứng dụng peer đã xử lý message. Trên Linux, có thể dùng `MSG_NOSIGNAL` và vẫn kiểm tra lỗi `EPIPE`. Nguồn: [send(2)](https://man7.org/linux/man-pages/man2/send.2.html).

### 8.2. Format đề xuất cho bài tập

| Thành phần | Contract |
| --- | --- |
| Header | 4 byte unsigned length theo network byte order (big-endian) |
| Length | Chỉ số byte của payload, không tính header; không phải số ký tự Unicode |
| Payload | Từ 0 đến 65.536 byte; cho phép chứa byte NUL |
| Length vượt trần | Báo protocol error và đóng connection trước khi cấp phát theo length |
| EOF ngay ranh giới | Kết thúc input bình thường |
| EOF giữa header/payload | Truncated frame; không enqueue phần dang dở |

Đọc đủ 4 byte vào buffer rồi ghép từng byte đã chuyển sang `std::uint32_t` trước khi dịch bit. Có thể dùng `std::memcpy` vào một biến `std::uint32_t` rồi gọi `ntohl`. Tránh ép buffer sang `uint32_t*` rồi dereference vì alignment và quy tắc aliasing. So length với trần ở kiểu unsigned trước khi cấp phát. [Alignment](https://eel.is/c++draft/basic.align) · [Type accessibility](https://eel.is/c++draft/basic.lval)

Dùng `read_exact` để đọc header rồi payload. Khi length bằng 0, phát message rỗng ngay. Cùng một stream có thể được chia khác nhau giữa các lần `recv`, nên dùng nguồn đọc có điều khiển cho các test về framing. [Beej 7.4 và 7.6](https://beej.us/guide/bgnet/html/split/slightly-advanced-techniques.html)

Length prefix cho biết cần đọc bao nhiêu byte và cần có trần. Delimiter tiện cho text, nhưng cũng cần giới hạn frame và quy tắc xử lý delimiter xuất hiện trong payload.

## 9. Sáu lab thực nghiệm — 18 giờ

Mỗi lab ghi giả thuyết, cách tái hiện, bằng chứng và nguyên nhân.

### Lab 3.1 — Data race và atomicity logic (3h)

Cho hai thread tăng chung counter. Chạy TSan ở bản thiếu đồng bộ rồi kiểm tra bản dùng mutex đạt `threads * iterations`. Với ví dụ check-then-enqueue ở mục 5, cho hai producer cùng kiểm tra trước khi enqueue và kiểm tra capacity sau khi sửa.

### Lab 3.2 — Predicate và notification (3h)

Dùng buffer một slot, một producer và hai consumer. So sánh bản dùng `if` với predicate loop khi một consumer khác lấy item trước, hoặc khi có notification mà state chưa đổi. Bản đúng giữ invariant đầy/rỗng. Bỏ notify để quan sát lỗi progress.

### Lab 3.3 — Deadlock và lock ordering (3h)

- **GDB** — Cho hai thread khóa mutex đầu theo thứ tự ngược nhau, đồng bộ tại barrier rồi lấy mutex thứ hai. Khi treo, dùng `info threads` và `thread apply all bt` để đối chiếu vòng chờ.
- **TSan** — Trong bản build có `-fsanitize=thread`, cho hai đường khóa A→B và B→A chạy tuần tự trên cùng hai mutex, bỏ barrier. Chạy với `TSAN_OPTIONS=detect_deadlocks=1` để kiểm tra báo cáo `lock-order-inversion` khi runtime hỗ trợ. TSan quan sát thứ tự khóa đã thực thi, còn GDB cho thấy hai thread đang chờ nhau. [TSan Deadlock Detector](https://github.com/google/sanitizers/wiki/ThreadSanitizerDeadlockDetector)
- **Sửa** — Cho cả hai đường dùng cùng thứ tự khóa và bỏ barrier giữa hai lần lấy khóa. Xác nhận bản sửa hoàn tất. Dùng timeout nếu chạy bản treo ngoài GDB.

### Lab 3.4 — Phạm vi lock và contention (3h)

So sánh giữ queue lock suốt task với chỉ giữ lock lúc pop. Thử 1, 2 và 4 worker với cùng workload và cùng bản build tối ưu. Kiểm tra output khớp, rồi dùng throughput và thời gian chờ để xác định phần việc bị tuần tự hóa.

### Lab 3.5 — Socket lifecycle và partial I/O (3h)

Gửi chuỗi byte cố định qua TCP loopback rồi đóng chiều gửi. Giới hạn mỗi lần đọc ở 1–3 byte để buộc dữ liệu đi qua nhiều lần đọc. Log giá trị syscall trả về và offset. Kiểm tra dữ liệu khớp với input và vòng đọc kết thúc khi EOF. Nếu TCP chưa trả short send, dùng hàm gửi giả lập chỉ nhận một phần để kiểm tra offset.

### Lab 3.6 — Framing theo nhiều cách chia byte (3h)

Cho framer đọc một stream có hai frame, lần lượt theo từng byte, theo mọi vị trí tách thành hai phần và qua TCP thật. Kiểm tra hai message đúng thứ tự trong framer. Thử thêm EOF giữa header/payload, payload rỗng, byte NUL và length vượt trần.

## 10. Project 3 — Framed Protocol & Bounded Worker Queue (25 giờ)

### 10.1. Phạm vi

Prototype dùng **một kết nối loopback**, queue `K = 5` và **2 worker**. I/O producer sở hữu socket và frame đang đọc. Mỗi worker xử lý message ngoài queue mutex và ghi kết quả riêng để coordinator tổng hợp sau join. Gắn ID vào payload thử nghiệm và dùng checksum để đối chiếu kết quả. Hai worker có thể hoàn thành khác thứ tự pop.

### 10.2. Các mốc triển khai

| Mốc | Giờ | Công việc | Điều kiện chuyển bước |
| --- | ---: | --- | --- |
| 1 | 3 | Viết protocol contract, ownership và invariant | Quy định rõ length, giới hạn, EOF, close và full queue |
| 2 | 6 | Hoàn thiện bounded queue và drain shutdown | Đầy/rỗng/close có hành vi xác định; waiter đều thoát |
| 3 | 5 | Hoàn thiện read_exact, send loop và framer | Qua các trường hợp chia byte, input lỗi |
| 4 | 5 | Tích hợp loopback TCP → queue → 2 workers | Đúng tập ID/kết quả; xử lý task ngoài queue lock |
| 5 | 4 | Break-check, TSan và đo overload | Có bằng chứng queue bị giới hạn và producer phải chờ |
| 6 | 2 | Viết ADR và giải thích kết quả | Quyết định gắn với workload và bằng chứng |
| **Tổng** | **25** | | |

### 10.3. Kiểm chứng tích hợp

| Tình huống | Kết quả phải xác nhận |
| --- | --- |
| Frame hợp lệ, dù chia hoặc gộp giữa các lần đọc | Mỗi ID xuất hiện đúng một lần, checksum khớp |
| Queue đầy | Producer chờ, size không vượt 5 |
| Close khi còn waiter hoặc item | Consumer rỗng thoát, producer bị từ chối vẫn giữ nguyên message, item đã nhận được drain |
| EOF hoặc frame lỗi | Phân biệt EOF đúng ranh giới với frame dở, từ chối length quá trần trước khi cấp phát và dọn worker |
| Tạo được worker thứ nhất rồi startup thất bại | Scope guard đóng queue và join worker đã tạo |
| Tải đồng thời dưới TSan | Không có báo cáo data race trong những execution đã chạy |

Để kiểm tra queue đầy, giữ worker bằng một gate riêng, để producer vào nhánh chờ ở lần push thứ sáu, rồi mở gate. Dùng tín hiệu từ test hoặc log ở nhánh chờ để xác nhận producer đã thực sự tới đó.

### 10.4. Shutdown và lỗi

Tạo **scope guard RAII trước khi khởi chạy worker đầu tiên**, sau khi đã tạo queue, vùng kết quả và container thread. Khi scope kết thúc, guard gọi `queue.close()` trước, rồi join từng worker còn `joinable()`. Coordinator cần nhả queue mutex trước khi join để worker có thể drain và thoát. Cách này xử lý cả trường hợp mới tạo được một phần worker. Destructor của `std::thread` gọi `std::terminate` nếu thread vẫn joinable. [C++ — destructor của thread](https://eel.is/c++draft/thread.thread.destr)

Giữ task hữu hạn. Nếu task có thể ném exception, bắt và ghi lỗi ở biên worker. Việc đóng queue chỉ đánh thức các thread chờ queue. Producer đang chờ `recv` vẫn cần EOF, lỗi I/O hoặc deadline để thoát.

### 10.5. Đo overload và ADR

Giữ workload và build flags cố định, tách warm-up và đo nhiều lần bằng bản tối ưu không có TSan. Ghi số message nhận/xử lý/lỗi, max queue size, thời gian producer chờ và throughput.

ADR nêu lý do chọn mô hình worker, blocking khi đầy và length prefix. Dùng số đo để giải thích trade-off và nêu điều kiện cần đổi thiết kế.

## 11. Công cụ trên Linux/WSL

Build source của lab hoặc project với `-g -O1 -fsanitize=thread -pthread` ở bước compile và link. Lưu stack trace của báo cáo. Một lần chạy sạch chỉ cho biết TSan không báo lỗi trong lần chạy đó. [LLVM — ThreadSanitizer](https://clang.llvm.org/docs/ThreadSanitizer.html)

Nếu TSan trên WSL2/Linux dừng ngay với `unexpected memory mapping`, ghi **chưa kiểm tra được** và thử runtime mới hơn trước. Với runtime cũ bị xung đột ASLR, một workaround là tạm hạ `vm.mmap_rnd_bits` trong môi trường lab riêng, ví dụ từ 32 xuống 28 trên x86-64. Lưu giá trị ban đầu và khôi phục sau khi chạy vì đây là thiết lập ASLR cấp kernel. [Sanitizers #1716](https://github.com/google/sanitizers/issues/1716?timeline_page=1) · [Linux — mmap_rnd_bits](https://docs.kernel.org/admin-guide/sysctl/vm.html#mmap-rnd-bits)

Dùng GDB cho Lab 3.3 và `strace -f -e trace=network,read,write,close,futex` khi cần đối chiếu syscall.

## 12. Exit Gate 3 — 12 giờ thẩm định độc lập

- [ ] **Giải thích (2h)** — Lần theo owner, invariant và predicate của một message từ socket tới worker.
- [ ] **Sửa code (3h)** — Sửa một lỗi queue hoặc shutdown và một lỗi framer giả định `recv` đọc đủ.
- [ ] **Dự đoán và đo (3h)** — Dự đoán khi nào producer chờ, so với kết quả đo và trình bày báo cáo TSan.
- [ ] **Phân tích và bảo vệ quyết định (4h)** — Giải thích vòng chờ trong Lab 3.3 và bảo vệ chính sách overload dựa trên contract và số đo.
