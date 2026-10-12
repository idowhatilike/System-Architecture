# Các lưu ý Phase 0 :
+ I/O là nhập xuất từ thiết bị nhưng việc CPU vẫn tham gia vào việc bắt đầu (bằng system call) và kết thúc (nhận dữ liệu và chuyển đổi trạng thái)
+ Read = Fd -> buffer , Write = Buffer -> fd

# Day 002 : Introducion to OS (9 / 10 / 2026)

---

## I/ OS là gì và tác dụng

Chạy chương trình = Thực thi vô số lệnh lệnh (instructions) . Tiến trình (processor) sẻ thực hiện ba bước : 
- Tìm và nạp (fetches) lệnh từ bộ nhớ
- Giải mã (decodes) lệnh (Hiểu , tìm ra lệnh này là gì ?)
- Thực thi (executes) lệnh (Làm theo đúng những gì lệnh bảo)
Đó cơ bản là mô hình máy tính đơn giản theo Von Neumann .
Đơn giản nhưng khi chạy cần vô số thao tác để khiến hệ thống dễ dùng .
Và phần mền (software) chịu trách nhiệm khiến dễ dàng chạy chương trình như :
+ Cấp phát và chia sẻ bộ nhớ
+ Cho phép ứng dụng thao tác với thiết bị
+ ... v.v
-> Gọi là Operating System (OS) . Thứ giúp chúng ta dễ dàng thao tác máy tính
- Kỹ thuật kinh điển của OS để giúp chúng ta dễ dàng hơn là sự ảo hóa . Biến tài nguyên cứng -> ảo , nên ta gọi OS = máy ảo (virtual machine) 
- OS còn cho phép chúng ta thao tác với tính năng của máy ảo và cung cấp hoàng loạt các API (interfaces) cũng như vô số lệnh gọi hệ thống (system calls) nên ta còn gọi OS = thư viện chuẩn (standard libary) . Và vì đồng thời quản lý tài nguyên , như ảo hóa các tài nguyên và đồng thời cho các ứng dụng tiếp cận đến vùng tài nguyên cần thiết như dữ liệu (data) hay thiết bị (devices) (chia sẻ tài nguyên) nên ta gọi OS = quản lý tài nguyên (resource manager) -> OS là phần mềm có nhiệm vụ : quản lý tài nguyên , thư viện chuẩn và máy ảo 

# II/ Ảo hóa CPU (Virtualizing the CPU)

- Bằng việc dùng một số thiết bị phần cứng (hardware devices) mà OS tạo ra ảo giác biến 1 CPU (hay 1 tập hợp nhỏ) thành vô số CPUs , gọi đó là Ảo hóa CPU (đơn nhiên ta phải dùng APIs để ưu cầu OS thực hiện các thao tác với các CPU ảo này - APIs là mà OS giúp chúng ta đơn giản hóa thao tác) . Và có nhiều câu hỏi nãy sinh sẻ được trả lời bằng các luật (policy) và cơ chế (mechanisms) (quản lý các cpu ảo = quản lý tài nguyên = OS)

# III/ Ảo hóa bộ nhớ (Virtualizing Memory)

- Trước tiên bộ nhớ vật lý vô cùng đơn giản : Dãy các bytes , ta đọc dữ liệu nếu có địa chỉ và viết (hoặc cập nhật) bộ nhớ thông qua địa chỉ được cho
Ảo hóa bộ nhớ là việc đưa cho mỗi tiến trình xử lý một địa chỉ ảo (thực tế là bộ nhớ vật lý đang được chia sẻ nhưng đảm bảo không đè nhau mà hỏng)

# IV/ Tính đồng thời (Concurrency)

-Tính đồng thời là một trong những vấn đề lớn không chỉ trong OS mà hiện nay còn là đa luồng . Khi OS phải chạy quá trình đầu tiên , thứ hai , v.v sẻ xảy ra các lỗi và sự bất thống nhất . Lấy ví dụ việc lập loops và nâng biến đếm cnt , lỗi ở đâu . Lý do nằm ở việc nâng biến đếm bản chất gồm ba thao tác : Đọc thông tin từ bộ nhớ , Ghi lại giá trị và Ghi lại vào địa chỉ bộ nhớ . Nhưng ba hành động này không diễn ra đồng thời lập tức nên những điều kỳ lạ diễn ra

# V/ Tính kiên trì (Persistence)

-Dữ liệu rất dễ thất thoát , đặc biệt khi DRAM lưu dữ liệu theo phương pháp bay hơi , một sự có như sự cố hệ thống hoặc sập nguồn là mất mát mà không ai mong muốn . Vì vậy ta cần cả phần cứng và mềm của máy tính lưu thiết bị một cách kiên trì . Phần cứng như các thiết bị I/O , ổ đĩa cứng (hay gần đây là ổ đĩa rắn) chịu trách nhiệm cho các thông tin lâu dài (long lived inf) 

- Phần mềm chịu trách nhiệm đó gọi là tệp tin hệ thống (file system) . Khác với CPU hay RAM , nó đã là một tầng trựu tượng của OS (không cần qua ảo hóa của OS) vì giả sử nhu cầu thiết yếu của người dùng là chia sẻ file (ta không thể viết code tren editor , rồi biên dịch bằng gcc rồi chạy file nếu ảo hóa và tách ra thành từng file system riêng , điều đó rất khó hoặc bất khả thi) . Phổ biến là dùng các lời gọi hệ thống định tuyến đến tệp tin hệ thống  
 
- Cách tệp tin hệ thống ghi dữ liệu :
+ Xác định vị trí dữ liệu sẻ nằm đâu và giữ quan sát thông qua nhiều cấu trúc dữ liệu khác nhau do tệp tin hệ thống duy trì . Để làm được việc này thì tệp tin hệ thống phải liên tục yêu cầu các thao tác I/O tới thiết bị dưới nhằm đọc các cấu trúc đang tồn tại hoặc viết (hoặc cập nhật) . Các thao tác này được đơn giản hóa nhờ OS thông qua lời gọi hệ thống (OS = thư viện chuẩn) (nếu tự tay thực hiện thì việc này vô cùng khó đòi hỏi sự tỉ mĩ và phức tạp với hiểu biết về thiết bị low-level và ngữ nghĩa (exact semantics)) 

# Day 003 : 10 / 10 / 2026

## I/ Xây dựng một hệ điều hành (OS) cần gì và lịch sử của hệ điều hành ?

- Xây dựng một hệ điều hành cần đặt ra những mục tiêu trong quá trình thiết kế cũng như đưa ra các đánh đổi (trade-offs) . Trong đó đảm bảo các tiêu chí : Tầng trừu tượng (abstraction) (nền tảng quan trọng) , hiệu năng (high performance) (giảm thiểu chi phí phát sinh (overheads)) . Bên cạnh đó là tính bảo vệ (protection) hay việc hệ điều hành chạy không ngừng (non-stop) . Những tiêu chí bổ sung khác là : hiệu quả năng lượng (energy effeciency) , tính bảo mật (security - sự tổng quát của protection) 

- Lịch sử phát triển của OS : Ban đầu chỉ đóng vai trò như một thư viện cung cấp API , khi ấy máy tính mainframe chỉ chạy một tiến trình (process) do người vận hành (human operator) kiểm soát (thực chất việc ngày nay do OS làm thì khi ấy là do người vận hành làm) với phương thức tính toán là xử lý hàng đợi (batch processing) (các thao tác thực hiện theo "lô" (batch)) sau đó người ta nhận ra việc mọi thao tác đều ở mức cao nhất (tương đương kernel mode ngày nay) là vô cùng nguy hiểm khi mã nguồn có thể làm bất cứ điều gì -> OS thay đổi và tạo ra hai chế độ là user mode (nơi thao tác các thiết bị phần cứng , ... bị hạn chế) và kernel mode (toàn quyền) , để xử lý hai chế độ thì OS dùng lời gọi hệ thống (system call) (chuyển giao điều khiển và nâng quyền) còn procedure call (không được nâng quyền) . System call được khởi tạo qua trap nhờ sự giúp đỡ của thiết bị phần cứng (qua bổ sung cặp lệnh phần cứng và trạng thái phần cứng) để đưa cho nhân (tức kernel mode) xử lý xong trả về một lệnh thông qua return-to-trap và trả chế độ (quyền) về lại cho user mode . Sau này với sự bùng nổ của mini computer việc đa chương trình (multiprogramming) là không tránh khỏi khi tốc độ xử lý của CPU nhanh gấp hàng triệu lần việc xử lý I/O , bắt CPU chờ I/O là lãng phí , thay vào đó khi gặp tiền trình cần xử lý I/O -> cho vào hàng waiting xử lý và chuyển sang việc chương trình khác

## II/ Sự trừu tượng hóa căn bản : Tiến Trình (The process)

- Tiến trình là một chương trình (program) đang chạy . Một chương trình khái quát (đơn giản hóa) là tập hợp các lệnh (và dữ liệu tĩnh) nằm trên ổ đĩa (disk) chờ được thực hiện . Chính OS đọc các byte về chương trình và đưa các chương trình vào một môi trường thực thi cho một instance trạng thái thực thi . 
- Ảo hóa CPU có nhiều kỹ thuật , kỹ thuật cơ bản kinh điển là phân chia thời gian (time sharing) tức cho một tiến trình này chạy trong thời gian (ngắn) , dừng và cho tiến trình khác chạy , đổi lại hiệu năng sẻ chậm hơn (vì chia sẻ CPU và chi phí thao tác) ngoài ra còn có chia sẻ bộ nhớ (space sharing - không phải kỹ thuật ảo hóa mà giống như là quản lý tài nguyên) . Và để thực hiện các kỹ thuật để ảo hóa CPU hiệu quả , nhất là phân chia thời gian , OS cần các cơ chế cấp thấp (mechanisms) lẫn các chính sách cấp cao (policies) . Cơ chế cấp thấp là phương thức (method) hoặc giao thức (protocol) để thực thi một chức năng cần thiết , ví dụ là Chuyển đổi bối cảnh (context switch) . Nằm trên cơ chế là các chính sách , phần trí tuệ của OS , được viết bởi các thuật toán để đưa ra quyết định (Policy = Quyết định làm gì , Mechanism = Cách để làm một việc gì đó)
- Tiến trình là một chương trình đang chạy , và ta có thể tóm tắt một tiến trình bằng thành phần tiến trình truy cập hoặc tác động . Trạng thái máy (machine state) là trạng thái máy tính tại một thời điểm cần được duy trì để thực thi chương trình , lúc này ta nói về thời điểm tiến trình đang chạy . Lúc này các thành phần quan trọng đó chính là bộ nhớ (memory) vì tiến trình sử dụng không gian địa chỉ (address space - không gian địa chỉ process nhìn thấy (phần lớn là process thông qua địa chỉ ảo)) , ngoài ra còn có các thanh ghi (registers) : đọc / cập nhật các thanh ghi và quan trọng trong thực thi tiến trình , một số thanh ghi đặc biệt quan trọng như con trỏ lệnh (program counter - PC) (hay instruction pointer - IP) cho biết lệnh nào của chương trình đang hoặc sắp thực thi hay con trỏ ngăn xếp (stack pointer) và con trỏ khung (frame pointer) ... Cuối cùng là các thiết bị thiết bị bền vững . Thông tin quản lý I/O (I/O information - dữ liệu thông tin hỗ trợ I/O) bao gồm danh sách các tệp tiến trình đang mở (có thể hiểu đơn giản OS quản lý tệp tin bằng vị trí đọc / ghi trên file , còn thao tác đọc / ghi trực tiếp lên file  thay vì quản lý toàn bộ nội dung file , còn thao tác được thực hiện trên file thông qua OS)

## III / API của tiến trình 

- 5 API phải có : 
+ Create : Tạo tiến trình mới
+ Destroy : Cưỡng chế dừng tiến trình (!= exit là khi tự tiến trình thoát bình thường , dùng khi gặp sự cố như treo máy)
+ Wait : Cho một tiến trình chờ một tiến trình khác thực hiện
+ Status : Tra cứu thông tin và trạng thái
+ Miscellaneous Control : Các thao tác bổ sung (ví dụ : suspend - đình chỉ tiến trình , resume : tiếp tục tiến trình)

# Day 004 + 005 : 11 / 10 / 2026 + 12 / 10 / 2026

## I / Bước khởi tạo tiến trình 

- Để chạy chương trình thì OS cần nạp (load) các đoạn mã (lúc này dạng mã thực thi chứ không phải ở dạng mã nguồn) (code) và dữ liệu tĩnh (static data : biến toàn cục (initialized variables , ...)) từ ổ đĩa (disk) sang bộ nhớ (memory) cụ thể là không gian địa chỉ (address space) (chương trình nằm ở ổ đĩa cứng (hoặc flash SSD) dưới dạng file thực thi)
- Có hai kiểu nạp : Eager loading (Nạp trước theo một chiến lược đã chọn - ví dụ như nạp tất cả) và Lazi loading (Nạp những đoạn mã và dữ liệu cần thiết) (cần hiểu rõ cơ chế paging và swapping)
- Sau khi đã nạp dữ liệu , OS cần phải làm một số việc . Tiêu biểu là cấp phát bộ nhớ cho ngăn xếp thực thi (run-time stack hay stack) (stack chứa dữ liệu các biến cục bộ , tham số hàm , địa chỉ trả về ...) và heap (ví dụ heap trong C có tác dụng để cấp phát / xóa dữ liệu động một cách tường minh) . Hay liên quan đến I/O thì Unix/Linux khi mở một file sẽ có các file mô tả (file descriptor ví dụ như : standard inp , standard out , standard error , ..) .Sau khi hoàn thành các công đoạn , bước chính cuối cùng của OS chính là thông qua cơ chế chuyên biệt mà nhảy đến điểm bắt đầu chương trình (entry point)

## II / Các trạng thái của quy trình (Process States)

- Để đơn giản hóa ta chỉ xét ba trạng thái cơ bản của một quy trình bao gồm : Sẵn sàng (Ready - Sẵn sàng để chạy nhưng chưa chạy) , Đang thực thi (Running - Đang chạy) và Tạm gác (Blocked - Đang chờ các sự kiện hoặc thực hiện các lệnh khiến tiến trình chưa thể chạy tiếp cho đến khi hoàn thành các lệnh để về Ready)
- Mối quan hệ : Ready --(được xếp lịch - scheduled)> running --(chuẩn bị I/O - I/O initate) --(hoàn tất I/O)> ready và running --(bị hủy bỏ - Descheduled)> ready

## III / Các cấu trúc dữ liệu (Data structes)

- OS cần các cấu trúc dữ liệu quan trọng để theo dõi các trạng thái của quy trình ví dụ như danh sách hoặc thông tin cần thiết của các quy trình sẵn sàng , đang chạy hay đang bị Tạm gác . Khi bị gác các thanh ghi chứa nội dung của tiến trình sẽ lưu vào các vùng lưu ngữ cảnh (register context - trên bộ nhớ) và khi hoàn tất và OS muốn tiếp tục tiến trình chỉ cần đặt đúng giá trị các thanh ghi về giá trị vật lý thực tế (liên quan đến context switch) (Và ngoài ra còn có các trạng thái khác của tiến trình : Khởi tạo (inital) hay Cuối cùng (Zombie - tiến trình đã xong nhưng chưa xóa nhằm cung cấp thông tin cho trạng thái cha - parent process) , ...) .
- Thông tin thêm : Cấu trúc đơn lẻ để lưu tiến trình thường được gọi là Khối điều khiển tiến trình (Process Controll Block - PCB - chỉ là một struct)

## IV / Giao diện lập trình chuyển tiếp (Interlude : Process API)

- Có thể hiểu đơn giản API là lớp giao diện trừu tượng hóa cách sử dụng chức năng nhưng không nhất thiết do OS cung cấp

### a. API lời gọi hệ thống tạo tiến trình mới - fork()

- Fork là một lệnh gọi hệ thống tạo ra một tiến trình mới  gần như là nhân bản của tiến trình hiện tại (gọi là cha) và tiến trình mới (gọi là con) không bắt đầu tại main mà entry point của nó là sau vị trí hàm fork trong cha (tức thực thi tại điểm mà lời gọi fork) ? Và trạng thái của 2 tiến trình này là khác nhau , nhất là giá trị fork trả về khi con chỉ trả về 0 còn cha thì là pid của con để xử lý hai luồng thực thi riêng biệt (xử lý từ fork đến tiếp). Ở đây có một vấn đề thú vị là sự đồng thời - liệu tiến trình nào sẽ thực thi trước ? ":3" (Có thể hiểu là khi sao chép ta sao chép mọi thứ không chỉ mã nguồn mà 
dữ liệu stack , heap , ... và cả thanh ghi program counter - pc chỉ báo cho CPU cần thực hiện gì nên tiến trình con sẽ chạy từ sau fork do pc con sao chép cha tức là tính sau fork) (trên máy mình thì cha thường sẽ nhanh hơn con tý - khá hợp lý , cha gọi fork xong đi trước rồi tiến trình con mới tạo xong và đuổi theo)

### b. API lời gọi hệ thống ép tiến trình chờ đợi - wait()

- Ta dùng wait để tiến trình cha thực thi khi và chỉ khi tiến trình con đã xong , sau đó tiến trình cha mức thực thi các lệnh sau và dưới wait
(Đôi khi wait được trả về trước khi tiến trình con thực sự hoàn thành)
(Không bao giờ khẳng định tính đúng đắn tuyệt đối thiếu chứng minh :3)
- Hàm chuẩn là pid_t wait(int *wstatus) trong đó wstatus trả về con trỏ giá trị thoát của tiến trình con , nếu chúng ta chỉ muốn đình chỉ tiến trình cha thì dùng nullptr , khai báo wstatus và ta có waitpid(pid_t pid,int *wstatus,0 (options - thay đổi hành vi)) trong đó pid có 4 giá trị (< -1 , -1 , 0 , > 0 , RTFM :3) xác định tập các tiến trình con thực thi cần chờ đợi , có :
            waitpid(-1 , &wstatus , 0) = wait(&wstatus)

### c. API lời gọi hệ thống thực thi tiến trình khác - exec()

- Ta dùng fork để tạo và chạy 1 tiến trình gần như là bản sao , còn exec dùng để chạy 1 tiến trình khác (thứ mà thực sự chúng ta hay dùng)

- Chi tiết về exec . Khi thực thi chương trình qua exec , có thể hiểu như ta đang gõ 1 prompt bằng code . Khi đó cần truyền các arg (args) , có 2 kiểu là liệt kê hoặc qua vector (thông qua 2 chữ l hoặc v) , nhưng không quan trọng kiểu thì cuối cùng phải có nullptr , chữ cuối là p , e hoặc rỗng tương ứng là rỗng thì chuyền chuẩn xác link file nhị phân , còn p thì tìm trong PATH còn e thì chỉ đơn giản là cho truyền biến môi trường

### d. Tại sao lại như vậy ?

- Cơ chế của fork và exec hay wait cực kỳ đơn giản nhưng rất mạnh để tạo nên những tiến trình hữu ích . Và hãy đọc thêm man page (RTFM - F bổ sung ^-^)