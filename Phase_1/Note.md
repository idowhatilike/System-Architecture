# Day 001-2 : 24-25/10/2026

___________________________________________________________

## I / Environment Variables & Dynamic Linker - Biến môi trường và liên kết động

- **Dynamic linker:** Tìm và nạp các thư viện chia sẻ cần thiết vào không gian địa chỉ của tiến trình, đồng thời liên kết các symbol để tiến trình có thể sử dụng chúng.
- **Environment variables:** Là các biến do người dùng hoặc chương trình thiết lập, dùng để tác động đến hành vi của tiến trình.
- **Kế thừa và thay đổi:** Biến môi trường được kế thừa từ tiến trình cha. Mọi thay đổi chỉ ảnh hưởng đến tiến trình hiện tại và các tiến trình con được tạo sau đó, không ảnh hưởng ngược lên tiến trình cha hay các tiến trình khác đã tồn tại. Có thể thay đổi môi trường thông qua Shell script, Batch file,... Ở cấp độ API, có thể thay đổi giữa `fork()` và `execve()` bằng cách thao tác gián tiếp qua `extern char **environ` hoặc thiết lập trực tiếp các cặp `name=value` trong `envp` khi gọi `execve()` (hoặc `execle()`).
- **Bảo mật với `setuid`:** Khi chương trình đặc quyền được thực thi trong chế độ bảo mật, dynamic linker sẽ lọc hoặc bỏ qua một số biến môi trường nguy hiểm nhằm ngăn việc lợi dụng chúng để can thiệp vào quá trình nạp thư viện.

___________________________________________________________

## II / Signals

- **Khái niệm** : là thông báo bất đồng bộ được chuẩn hóa gửi đến tiến trình hoặc luồng cụ thể khi gặp sự kiện (ngoại lệ , ...) để kích họat xử lý sự kiện . Là 1 thông báo bất đồng bộ (là thông báo không cần nhận thông báo ngay lập tức - tức việc phát sinh gửi signal và xử lý không nhất thiết là xảy ra gần như đồng thời) với công dụng phổ biến như ngắt , dừng và chấm dứt , ... Khi gửi tín hiệu và signal đã sẵn sàng , OS ngắt luồng thực thi để chuyển giao tín hiệu cho tiến trình . Tùy vào tín hiệu đã được đăng ký bởi tiến trình , nếu có thì hàm xử lý được gọi , ngược lại xử lý mặc định . Đặc biệt hữu ích trong các chương trình nhúng để thao tác liên tiến trình 
(Có thể thấy rất giống cơ chế các lệnh ngắt như hardware timer interrupt , ... nhưng khác với việc các lệnh ngắt được xử lý bởi CPU và được xử lý bởi nhân thì signal điều phối bởi OS (có thể thông qua lời gọi hệ thống) và xử lý bởi từng tiến trình riêng lẻ)
- **Cách gửi** : Thông qua kill(2) , kill(1) , .. hoặc các tổ hợp phím (Ctrl + C , Ctrl + Z , ... thay đổi bằng lệnh stty ở OS nếu có)
- **Xử lý** : Đối với các tín hiệu chưa được cài đặt trình xử lý thì tiến trình sẽ dùng hàm thực thi mặc định (thường là quy tắc chung như terminate) , ngược lại tiến trình có thể bắt và xử lý tiến trình đó hoặc dùng thực thi mặc định hoặc bỏ qua (trừ SIGKILL , SIGSTOP là không thể bị chặn bắt , bỏ qua)
- **Rủi ro** : Vì signal dùng để giải quyết sự kiện thường là lỗi nên đương nhiên ta không mong muốn có lỗi trong quá trình xử lý tín hiệu . Vì việc xử lý tín hiệu dễ gây ra tranh chấp (race conditions) , tín hiệu được gửi khi đang xử lý tín hiệu . Việc dùng các phương pháp như sigprocmask (trên đơn luồng và pthread_signmask() tổng quát) chặn và bỏ chặn và việc xử lý tín hiệu không nên có tác dụng phụ không mong muốn và các thay đổi không an toàn thậm chí bị cấm , như thay đổi errno hay signal mask , sử dụng hàm bất tái nhập , ... (khuyến nghị đơn giản là dùng cờ sig_atomic_t để kiểm tra) . Cách thường thấy là đẩy các tín hiệu vào queue và xử lý ngay lập tức khi nhận đủ một số lượng signal tối thiểu để tránh gián đoạn tiến trình gây phức tạp hóa không mong muốn (gián đoạn ở đây là các thao tác gây chặn , dừng và trả về sớm) và được xử lý bởi luồng chính (main thread) thay vì luồng phụ (workers pool) . (Tuy nhiên mỗi sig_atomic_t là không đủ nếu có lệnh không nguyên tử)

___________________________________________________________

## III / Zombie , Embryo và Orphan Process

- Zombie là tiến trình mà đã kết thúc nhưng cha chưa thu nhận
- Orphan là tiến trình mà cha chết trước con
- Embryo là thời điểm sơ khai nhất của tiến trình con mới khởi tạo nhưng chưa sẵn sàng để lập lịch chạy 
- Chương trình con cần thoát ra _exit() (không phải exit())

___________________________________________________________

## IV / POSIX Threads

- **Tạo và tham gia** : Dùng pthread_create (trả về errno != 0 nếu fail) và pthread_join
- **Mutex** : Dùng để khóa xác định sỡ hữu nhằm đảm bảo tài nguyên chung với cơ chế dành/nhả khóa khỏi các thao tác đồng thời (nhưng dev vẫn cần đảm bảo quy tắc đồng bộ , ...)
- **Biến điều kiện** : thay vì lặp lại việc kiểm tra điều kiện mỗi khác 1 cách vô ích tốn tài nguyên thì ta dùng pthread_cond_wait với 2 mục đích : đưa vào trạng thái ngủ và nhả khóa , khi được đánh thức (có thể là bị đánh thức giả) thì dành lại khóa trước rồi kiểm tra điều kiện (nếu như chưa thõa mà dậy do bị đánh thức giả thì tiếp tục vòng lặp) khi tới lệnh tiếp theo
- **Dữ liệu từng Thread/TLS** : Đôi khi ta cần 1 biến toàn cục hoặc static mà mọi luồng đều có thể dùng nhưng ĐỘC LẬP giá trị (ví dụ như errno) . Mỗi luồng khi tạo sẽ có 1 nơi lưu trữ ẩn dữ liệu , để dùng 1 biến chung lưu được thông tin độc lập nhiều giá trị thì biến chung ấy không chứa dữ liệu mà chứa chỉ số nơi lưu thông tin cần lưu ở luồng đấy
- **Hủy và dọn dẹp luồng** : Khi một luồng bị dừng đột ngột mà chưa kịp giải phóng tài nguyên (như mutex , ...) dễ dẫn đến các sự cố (như deadlock , ...) . POSIX truyền thống là dùng pthread_cancel thay vì giết luồng tức thì thì đợi đến các hàm tương tác chờ đợi hệ thống (cancellation points) như sleep , read , write , pthread_cond_wait , ... và luồng tự bảo vệ thông qua cơ chế thay đổi cancellation points nhờ pthread_setcancelstate hoặc pthread_setcanceltype . Để dọn dẹp bất kể sự cố thì ta dùng pthread_cleanup để nếu có sự cố sẽ dùng hàm ta đã cài (gồm push để đăng ký hanlder và pop để gỡ hanlder , và sẽ gọi hanlder ngay khi gỡ nếu execute != 0)
- **Note** : Trên linux (glibc) thì pthread_cancel sẽ dùng cơ chế stack unwiding tương tự giống như ném expection mà không cần pthread_cleanup

___________________________________________________________

## V / Program Execution và Pipes và FIFOs

- **Nạp ELF** : Chương trình chỉ là dãy bit tĩnh , vậy làm sao trả lời 3 câu hỏi : Quyền (W,R,...) ; Bộ nhớ ; Vị trí lệnh máy đầu tiên -> Tệp ELF là bản thiết kế trả lời được đính đầu trang . Quá trình nạp file nhị phân thực chất là lời gọi execve để thiết kế lại bộ nhớ ảo mới bằng việc đảm bảo đúng là file ELF và hợp lệ thì xóa bộ nhớ ảo bộ bảng trang cũ (giữ PID)
- **Kế thừa FD và cờ FD_CLOEXEC** : Kể cả khi fork hay execv thì FD nằm trong proc sẽ được giữ nguyên vẹn nhằm thiết kế giao tiếp trước khi chương trình mới chạy . Nhưng vấn đề khi chia sẽ tài nguyên , ví dụ như treo tài nguyên hay lỗ hỏng bảo mật . Ta dùng FD_CLOEXEC để không cần đóng tự động execve() mà Linux sẽ gán cờ trong bảng FD của tiến trình . Bản chất là khi fork tiến trình con sẽ tự động kế thừa FD và cả cờ FD_CLOEXEC , nếu execve() trong quá trình nạp file Kernel tự động quét và đóng FD bật cờ FD_CLOEXEC
- **Shell pipe và pipe()** : Dùng dup2() và pipe để điều chỉnh thao tác luồng
- **FIFOs** : FIFO (First-In , First-Out) giải quyết 1 vấn đề là nếu ta muốn dùng 1 công cụ tương tự như pipe() mà 2 tiến trình không có quan hệ (tiến trình sau không kế thừa FD tiến trình trước) . Cơ chế là tạo ra 1 điểm hẹn có tên trong tệp hệ thống và bất kỳ tiến trình nào có thể thao tác nếu biết tên ấy (chứ không phải thực sự là một file lưu xuống bộ nhớ ảo) mà truyền trực tiếp qua buffer
- **PIPE_BUF** : Hiểu đơn giản là 1 hằng số dùng để chỉ kích thước tối đa (tính theo byte) được đảm bảo ghi vào một pipe hoặc FIFO một cách nguyên tử (atomic)
- **Vận hành pipe** : Các pipe bao gồm pipe() (ẩn danh) hay FIFO (công khai) đều vận hành dựa trên 1 circ buffer trong RAM do kernel quản lý với 4 quy tắc phổ biến truyền thống :
+ 1. __Luồng byte đơn hướng gồm__ : Luồng byte (Pipe chỉ đơn giản là ghi nối đuôi nhau giữa trước read mà không phân biệt) ; Đọc là hủy và không thể tua (Read sẽ lấy byte từ buffer và xóa ngay lập tức - mức độ nguyên tử) ; Đơn hướng (Một chiều - Dữ liệu chỉ chảy theo 1 chiều cố định từ đầu ghi sang đầu đọc)
+ 2. __Dung lượng buffer và tính nguyên tử__ : Linux hiện đại mặc định là 65536 - 64KB (mò ra con số này ở bài nào á quên rồi :>) , nếu ghi đầy mà tiếp tục ghi thì write sẽ bị chặn cho đến khi nào có chỗ trống . Còn PIPE_BUF (trên Linux thường là 4KB) là giới hạn trên kích thước ghi đảm bảo tính nguyên tử việc ghi vào , ngược lại sẽ chia nhỏ ra và khi đó không còn đảm bảo tính chất nguyên tử ấy (con số có thể khác biệt phụ thuộc vào nhiều yếu tố)
+ 3. __Quy tắc EOF__ : Bản chất là xét còn có thể có dữ liệu nào được gửi đến không , nếu còn thì cho read() vào trạng thái đợi , ngược lại lập tức kết thức và trả về 0 
+ 4. __Quy tắc SIGPIPE__ : Bản chất là khi dữ liệu vào ống nhưng tất cả đầu đọc đã đóng gây ra đầy RAM vô ích hoặc treo tiến trình nếu không xử lý đúng cách . Kernel xử lý bằng việc gửi tín hiệu SIGPIPE để chấm dứt , nếu fail thì nhận về -1 và errno = EPIPE

_______________________________________________________________________________________

# Day 001-2 : 24-25/10/2026

___________________________________________________________

## I / Syscall

- **_exit()** : Dùng để kết thúc tiến trình ngay lập tức mà không thực hiện các thao tác dọn dẹp như chuẩn hàm exit() (nhưng kernel vẫn thu hồi tài nguyên)
- **fcntl** : Dùng để thao tác với các fd đang mở 
- **sigaction** : Dùng để kiểm tra và thiết lập bố trí (disposition) của 1 tín hiệu (hành động khi nhận tín hiệu)
- **kill** : gửi tín hiệu đến một (nhóm) tiến trình cụ thể

## II / Signal (bổ sung)
- Là cơ chế thông thông báo bất đồng bộ 
- Đặc điểm cốt lõi kỹ thuật :
+ Bất khả xâm phạm : SIGKILL và SIGSTOP (không thể bị chặn , bắt hay bỏ qua)
+ Non-real-time : Không hỗ trợ hàng đợi , 1 signal đang pending có thể gây mất mát các signal cùng loại -> Thiết kế (ban đầu) dành cho standard signal (có hoặc không)
+ Real-time Signals : Hỗ trợ xếp hàng và kèm payload dữ liệu -> Được POSIX thiết kế biến signal thành 1 cơ chế giao tiếp giữa các tiến trình
+ Signal mask : mask riêng của từng tiến trình/luồng giúp xác định tín hiệu nào bị chặn
- **signal-safety (async-signal-safety)** : cơ chế an toàn xử lý dữ liệu :
+ 1. Chỉ gọi hàm có tính Async-Signal-Safe : Sử dụng trực tiếp các hàm atomic như write , ...
+ 2. Bảo vệ biến errno : cần giữ 1 bản sao lưu trước khi thay đổi errno
+ 3. Biến chia sẽ (cục bộ || static) : Có gắng đưa vệ dạng thao tác nguyên tử như dùng volatile sig_atomic_t (tạo biến cờ và mã chính cho handler) (hoặc các atomic types chuẩn POSIX) (không 100% an toàn và volatile không biến thao tác thành nguyên tử)
+ 4. Pattern khuyến nghị : Việc xử lý signal bằng signal handler trực tiếp tiềm ẩn rủi ro do cướp luồng thực thi -> (được kernel đưa vào context của tiến trình) chuyển tín hiệu về luống chính (event loop) dưới dạng I/O . Trong đó có 2 cách phổ biến là : signalfd(2) ; dùng static pipe (vô danh nhưng không block) -> gọi đúng 1 write vào đầu ghi -> luồng chính đọc byte và dọn dẹp (tự mô phỏng signalfd) (nếu write thất bại thì fail)

## III / Terminal & Phiên

- **chdir** : Thay đổi con trỏ thư mục làm việc hiện tại (CWD) của chính tiến trình gọi (có phiên bản dành cho fd là fdchdir) . CWD có tính kế thừa (fork) và độc lập , giữ qua exec  ; Bản chất là thay đổi con tro pwd nằm trong struct fs_struct trong task struct
- **getcwd** : Truy vết thư mục làm việc hiện tại , trả về đường dẫn tuyệt đối (dựng link)
- **setpgid** : quản lý process group (support job control) bằng cách gán pid -> id process group (Mối quan hệ phân cấp : Session -> process groups -> process) (chọn 1 process làm leader group) . Vì an toàn thì kernel quy định tiến trình cha không đổi pgid của tiến trình con sau khi con gọi exec thành công , một tiến trình không thể gia nhập một Process Group thuộc về một Session khác . Race condition kinh điển : tiến trình cha , con (fork) cùng gọi setpgid 
- **tcsetpgrp** : Quy tắc là 1 thiết bị terminal (đầu cuối) chỉ có 1 nhóm tiến trình là foreground , còn lại là background . Tcsetpgrp chỉ định 1 group trở thành tiền tố của 1 terminal liên kết với fd (thường là STDIN_FILENO) (pgid phải cùng session với terminal)

## IV / Copy-on-write

- Khái niệm : Là cơ chế (kỹ thuật) quản lý tài nguyên nhằm tiết kiệm dữ liệu . Thay vì tạo ra 1 bản sao dữ liệu khi đa tiến trình/luồng sử dụng mà không thay đổi như đọc , tìm kiếm , ... thì chỉ tạo ra bản sao riêng khi có thay đổi dữ liệu .
- Trong quản lý bộ nhớ ảo : Kernel chia sẽ bộ nhớ vật lý chung giữa cha con và đánh dấu các trang này trong PageTable là Read-Only (RO) , khi cố điều chỉnh thì phần cứng MMU kích hoạt expection Page fault để kernel bắt lấy và cung cấp khung trang vật lý mới (kernel sẽ check page fault có phải do mmu phát và cần ) (sao chép bộ nhớ , gán cờ R-W , ...) (cũng có thể dùng trong quản lý bộ nhớ ảo)
- Trong SE : std::string c++ ; php (trừ tham chiếu) ; Nhiều kiểu dữ liệu Qt Framework ; Rust ; Hệ thống tệp tầng của Docker ; ... điều áp dụng COW
- Trong Computer Storage : Cơ chế nền tảng cốt lõi trong các hệ thống tệp tin hiện đại như ZFS , Btrfs , ReFS , ... cũng như trong quản lý các phân vùng logic ; máy chủ mc ; ... Trên hệ thống tệp tin truyền thống khi sửa đổi sẽ ghi đề trực tiếp lên các khối dữ liệu ; với COW các hệ thống hiện đại sẽ dùng kỹ thuật snapshots (lưu ngắn) : chỉ ghi các vị trí bị ảnh hưởng sang vị trí mới trên đĩa và cập nhật con trỏ dữ mới liệu lại cho hệ thống (phù hợp với cơ chế Ổ cứng thể rắn như SSD/Flash) -> Ứng dụng Container và ảo hóa hệ điều hành : Block-level COW ; Fire-level COW

## V / Câu hỏi định hướng :

### Chuỗi sự kiện phần cứng diễn ra khi Kernel tạo trang mới **COW** 

- Bổ sung kiến thức inode và dentry :
+ **inode (Index node)** : bản thể thật của file (chứa kích thước , quyền , timestamps , ...)
+ **dentry (Directory Entry)** : Ta cần ánh xạ tên file (string) <-> inode . Và cấu trúc dentry ra đời với lưu giữ 3 thông tin : tên (d_name) ; con trỏ tới dentry cha ; con trỏ d_inode trỏ tới struct inode của file trên RAM (con trỏ có thể NULL)

- Đầu tiên ta sẽ làm rõ cơ chế chặn của kernel :
+ 1. Mở 1 đối tượng I/O (fd) và gắn cờ O_RDONLY (không thể bị thay đổi bằng các biện pháp đơn giản như fcntl - cờ chế độ truy cập mang tính bất biến trong phiên làm việc đó)
+ 2. Mọi syscall thao tác ghi kích hoạt hàm kiểm tra quyền của VFS (Hệ thống tệp ảo) hoặc đơn giản hơn là (file->f_mode & FMODE_WRITE) -> return -1 và errno = EBADF (bad fd)
+ 3. Nếu chuyển hướng qua bộ nhớ ảo nhằm đánh lừa kiểm tra (mmap) . Nếu dùng mmap - MAP_SHARED thì sẽ thất bại và errno = EACCES vì khi tạo mapping sẽ kiểm tra quyền , trong đó PROT_WRITE trái ngược với chế độ truy cập O_RDONLY . Còn nếu dùng mmap MAP_PRIVATE thì có thể thành công vì MAP_PRIVATE không cam kết ghi ngược dữ liệu (nếu có thì fail ngay lúc chạy mmap rồi :>) về tuy nhiên khi thao tác ghi được thực hiện thì MMU nhận ra vi phạm vì con trỏ trên Page Table vẫn đang chỉ vào file với mà có O_RDONLY và thực hiện việc gửi expection cho kernel xử lý phần còn lại còn file gốc giữ nguyên
- Thế làm thế nào khi **chỉ có 1 fd gán cờ R-O ?** : Có 2 cách tiêu biểu
+ 1. __Re_open__ : Dùng magic link - symlink (/proc/self/fd/%d) tạo 1 phiên làm việc độc lập hoàn toàn nếu như tiến trình của bạn đủ quyền
+ 2. __Atomic File Upd__ : (Khuyên dùng) Đọc toàn bộ nội dung file read ; tạo và ghi vào 1 file tạm (thông qua open(<file name - tmp>, O_WRONLY | O_CREAT | O_EXCL, 0644)) ; dùng fsync(tmp_fd) đẩy dữ liệu xuống disk và dùng syscall atomic : rename(tmp , cur) để thay thế file