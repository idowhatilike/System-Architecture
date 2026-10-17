# Các lưu ý Phase 0 :
+ I/O là nhập xuất từ thiết bị nhưng việc CPU vẫn tham gia vào việc bắt đầu (bằng system call) và kết thúc (nhận dữ liệu và chuyển đổi trạng thái)
+ Read = Fd -> buffer , Write = Buffer -> fd

# Day 002 : Introducion to OS (9 / 10 / 2026)

_______________________________________________________________________

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

_______________________________________________________________________

# Day 003 : 10 / 10 / 2026

_______________________________________________________________________

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

_______________________________________________________________________

# Day 004 + 005 : 11 / 10 / 2026 + 12 / 10 / 2026

_______________________________________________________________________

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

_______________________________________________________________________

# Day 006 : 14 / 10 / 2026 : Mechanism: Limited Direct Execution

- Việc chia sẽ CPU cho dường như là vô số chương trình chạy cùng lúc là lúc ta cần ảo hóa CPU .
- Đánh giá việc ảo hóa CPU dựa trên 2 tiêu chí : performance và retaining control

_______________________________________________________________________

## I/ Kĩ thuật cơ bản : Hạn chế thực thi trực tiếp (Limited Direct Execution - LDE Protocol) 

- Hãy tưởng tượng việc không có hạn chế nào mà cứ để tiến trình chạy trực tiếp trên CPU , quá trình ấy bao gồm tạo điểm bắt đầu tiến trình , nạp dữ liệu , xác định và thiết lập bộ nhớ , ... rồi trả về nhân kernel . Nghe đơn giản và thực sự là tiến trình rất nhanh (vì không qua khâu trung gian nào) nhưng có hai hạn chế : Làm sao đảm bảo ta sẽ không thực hiện những thao tác ta không muốn ? Và làm sao để dừng tiến trình hiện tại để đổi sang tiến trình khác -> Khi đó OS không hơn không kém một thư viện cung cấp API , ta cần một sự "limited" 

___________________________________________

### a. Vấn đề 1 : Những thao tác bị hạn chế (Restricted Operations)

#### Vấn đề đặt ra
- Làm sao để tiến trình có thể thực thi các thao tác I/O hoặc các thao tác bị hạn chế khác nhưng không cần được cấp quyền hoàn toàn trên toàn bộ hệ thống ?
- Nếu system call và procedure call giống nhau vậy thì làm sao phân biệt ?
- Vậy kernel-mode có những hạn chế nào cần thiết hay không ?
- Điều gì sẽ xảy ra nếu bạn có quyền tạo và thay đổi Trap table trong user-mode ?

#### Giải quyết câu hỏi 1
- Sử dụng cơ chế chuyển đổi quyền điều khiển bảo vệ (PROTECTED CONTROL TRANSFER) . Thông qua sự giúp đỡ của phần cứng thông qua hai chế độ : user-mode và kernel-mode với việc chuyển đổi trap và return-from-trap (OS cho phần cứng biết trap tables nằm đâu trong bộ nhớ).

- User mode : Yêu cầu thực thi lệnh không phù hợp -> Kill process . Yêu cầu thực thi lệnh bị hạn chế -> Dùng lệnh trap để nhảy tới nhân kernel và đưa quyền lên kernel mode , lúc này OS xử lý các lệnh rồi thông qua lệnh return-from-trap về user-mode . Phần cứng phải đảm bảo khi thực thi trap phải lưu trữ đủ các thanh ghi tiến trình hiện tại vào Kernel stack để lệnh return-from-trap biết trạng thái đúng cần quay về

#### Giải quyết câu hỏi 2 
- Để phân biệt system call và procedure thật ra rất đơn giản , lấy ví dụ write() hay open() trong C , khi gọi hàm thì tiến trình làm hai việc : Báo tên (chỉ số) việc cần làm ; Kích hoạt lệnh Trap . Còn procedure call thường chỉ nhảy đến đoạn code khác trong cùng user-mode (tức trong cùng một address space) , và không chứa lệnh trap

#### Giải quyết câu hỏi 3 (và 4)
- Vì ở user-mode quyền hạn bị hạn chế và tiến trình bị nhốt ở căn phòng ảo (địa chỉ không gian) nên ta được thoải mái nhảy nhưng kernel-mode nếu không cẩn thận sẽ gây ra lỗ hổng bảo mật . Khi chuyển từ User-mode sang Kernel-mode thì sự khác biệt về quyền hạn sẽ thay đổi hành vi của OS đối với quyền truy cập và thao tác với CPU
- Khi một sự kiện ngoại lệ xảy ra (như ngắt nguồn phần cứng , system call , ...) thì phần cứng sẽ nhìn vào Bảng bẫy (Trap Table - thứ được tạo và điều chỉnh ngay lúc khởi động máy (boot up) bởi kernel vì khi đó máy sẽ ở kernel-mode) để biết địa chỉ Xử lý bẫy (Trap handler - cũng được tạo lúc bật và nằm trong RAM - thường là một đoạn code) để xử lý (làm gì , file nào , ...)
- Đương nhiên chỉ kernel-mode thao tác lên trap table (và bất khả thi ở user-mode để thao tác lên trap table dù là thông qua system call) . Còn nếu được thì đơn giản chiếm máy là khả thi 

___________________________________________

### b. Vấn đề 2 : Quá trình chuyển đổi giữa các tiến trình

- Quá trình đơn giản là dừng tiến trình đang thực thi và chạy một tiến trình khác . Nghe đơn giản nhỉ , nhưng vấn đế lúc này là theo lý thuyết OS đang không chạy trên CPU mà là tiến trình , vậy thì làm sao mà OS thực hiện việc chuyển đổi , về cơ bản OS không làm được nếu OS không chạy trên CPU 


#### Vấn đề đặt ra :
- Làm sao OS đoạt quyền kiểm soát CPU ?
- Làm sao OS đoạt quyền kiểm soát khi tiến trình không trả quyền ?
- OS sẽ làm gì tiếp khi đã có quyền kiểm soát trở lại và lưu giá trị tiến trình để thay đổi như nào ?
- Làm sao để đổi từ tiến trình A -> B đúng ?
- Mất bao lâu để switch A và B ?

#### Giải quyết vấn đề 1
- Cách giải quyết cổ điển (trong các OS cũ) là quan hệ đối tác (cooperative) tức OS sẽ tin tưởng vào tiến trình chạy hợp lý , nếu tiến trình chạy quá lâu và tốn tài nguyên thì tự nguyện cho OS sẽ đoạt lại quyền kiểm soát CPU . OS sẽ chiếm lại quyền thông qua một loạt các system call chỉ có tác dụng chuyển quyền kiểm soát từ tiến trình -> OS hoặc tiến trình có những hành vi bất hợp pháp (chia cho 0 , tiếp cận bộ nhớ không được phép , ...) rồi OS đoạt quyền và kill process (Cách giải quyết thụ động và nhiều tình huống dẫn đến OS sẽ không bao giờ có được quyền) . NHƯNG nếu tiến trình rơi vào việc thực thi quá lâu mà không có sự hỗ trợ phần cứng khiến tiến trình không gọi các sys call cần thiết thì OS bất lực và chỉ còn :3 đơn nhiên là restart
(Cách OS đối phó với hành vi bất hợp lệ : Chấm dứt chương trình - thô bạo , đơn giản nhưng đủ hiệu quả)

#### Giải quyết vấn đề 2
- Cách giải quyết chủ động và hiệu quả hơn chính là : Ngắt bởi bộ hẹn giờ (timer interrupt) . Đơn giản là một bộ đếm sẽ được lập trình để ngắt chương trình hiện tại mỗi vài milis và trình xử lý ngắt (interrupt handler) trong OS sẽ chạy và xong . 
OS cần báo cho phần cứng biết mỗi lần khởi động lại cần làm gì , và đồng thời khi khởi động OS cần bật lại bộ hẹn giờ , cả hai đều là hoạt động đặc quyền (privileged operation) -> OS cảm thấy an toàn và chạy tiến trình nào cũng được (đôi khi bộ đếm có thể bị tắt) (Phần cứng chịu trách nhiệm quan trọng lưu đủ thông tin trạng thái đặc biệt là cho trap và return-from-trap)
- Khi ngắt thì thông tin tiến trình (A) sẽ được lưu vào kernel stack (A) và chuyển sang kernel-mode

#### Giải quyết vấn đề 3
- Khi có quyền kiểm soát , OS đối mặt với hai câu hỏi : Tiếp tục hay tiến trình mới . Câu trả lời thông qua một phần của OS là Bộ lịch (Scheduler - một policy) . Nếu lựa chọn là thay đổi , OS sẽ thực thi một đoạn mã nguồn mà ta hay gọi là chuyển đổi bối cảnh (context - switch) : lưu dữ liệu tiến trình hiện tại (các giá trị thanh ghi) và con trỏ kernel stack A , ... vào bộ nhớ quản lý của A và nạp của tiến trình sắp chạy lên

#### Giải quyết vấn đề 4
- Ta cần một phương pháp đủ đơn giản để đổi từ A -> B mà đồng thời chỉnh từ kernel-mode -> user-mode , vâng đó chính là return-from-trap . OS chỉ cần lưu giá trị của A lại và lấy giá trị của B (bằng cách đổi kernel stack A -> B) sau đó dùng lệnh return-from-trap , lệnh này sẽ lấy đỉnh dữ liệu tiến trình Kernel stack và chuyển qua user-mode 
Ví dụ : (Bộ nhớ quản lý là 1 proc_struct của tiến trình nào đó)
A thực thi -> Ngắt (regs(A) -> k-stack(A) ; user-mode -> kernel-mode ; trap -> Xử lý trap)
Trap handler : OS sẵn sàng và gọi switch(A,B) ; regs(A) -> proc_struct(A) ; regs(B) <- proc_struct(B) ; k-stack(A) -> k-stack(B) ;
return-from-trap (đi vào B) : regs(B) <- k-stack(B) ; kernel-mode -> user-mode ; ...
- Note quan trọng :
+ Để đổi stack tiến trình đang dùng thì ta dùng con trỏ stack - Stack Pointer
+ Sở dĩ 2 lần lưu regs -> kstack và proc là vì regs->kstack lưu user-mode và còn lại lưu cho kernel-mode 
+ Mỗi tiến trình có 1 kstack riêng và không hề có kernel stack chung nào cả
+ Có 2 kiểu lưu/khôi phục thanh ghi là : Ngắt đếm thời gian , thanh ghi người dùng được phần cứng lưu ngầm định vào kstack của tiến trình đó . Thứ hai là khi OS quyết định chuyển từ A->B thì lưu một cách minh bạch vào cấu trúc tiến trình và hoạt động như thể vừa mới trap vào kstack B chứ không phải trap kstack A . Giải thích kỹ : trap = user-mode -> kernel-mode = đi vào kstack còn return-from-trap là kernel-mode->user-mode bằng việc lấy dữ liệu từ kstack 


#### Giải quyết vấn đề 5
- Thử dùng Imbench
- Ví dụ như : (1996 , Linux 1.3.37 , 200 MHz P6 CPU) -> (Sys cal : 4 us ; context switch : 6 us)

___________________________________________

## II / Vấn đề Tính đồng thời

___________________________________________

### Đặt ra vấn đề 
- Trong lúc thực thi một lệnh gọi hệ thống thì ngắt thời gian xảy ra , hay một cú ngắt đang diễn ra trong lúc một cú ngắt khác thì chuyện gì sẽ xảy ra ? -> Vấn đề tính đồng thời

### Giải quyết câu hỏi 
- Nó thuộc về tính đồng thời , sau này ta sẽ học rõ . Nhưng nói đơn giản là OS dùng một cơ chế đơn giản là vô hiệu hóa ngắt khi có một tiến trình ngắt đang hoạt động (nhưng nếu bỏ lâu thì sẽ mất ngắt - điều thực sự rất tồi tệ) . Hiện nay các OS đã phát triển một sơ đồ cơ chế khóa phức tạp bảo về truy cập đồng thời nội bộ , thứ rất hữu ích cho phép nhiều hoạt động diễn ra trên kernel cùng lúc

___________________________________________

## III / Tổng kết chương 

___________________________________________

- Tập hợp các kỹ thuật để ảo hóa CPU chính là Limited Direct Execution - LDE . Ý tưởng cơ bản là cứ cho tiến trình chạy trực tiếp trên CPU nhưng có những thiết lập phần cứng đảm bảo giới hạn những gì tiến trình có thể làm và tài nguyên sử dụng . OS thiết lập trap (trap table , trap hander , ...) và các thư viện đính kém vào procedure (và chúng ta gọi procedure của các thư viện , có thể là OS) và bật bộ đếm giờ và hướng dẫn cho phần cứng "việc cần làm" khi gặp trường hợp "cần làm" . Bằng một cơ chế đoạt quyền đã cài , OS sẽ chiếm lại quyền đã cho một tiến trình thực thi trực tiếp trên CPU , rồi dựa vào Policy mà chọn hành động tiếp theo . Nếu đổi tiến trình sẽ lưu trạng thái user-mode và kernel-mode xong dùng đổi địa chỉ kernel stack và return-from-trap để đổi tiến trình . OS cũng cần đảm bảo một cơ chế xử lý khi các sys call xảy ra đồng thời 

___________________________________________

# Day 007 : 15 / 10 / 2026

___________________________________________

## I / Kết quả bài tập Mechanism LDE 

- gettimeoffday() = thời gian tính từ mốc 1970 , gồm 2 giá trị : Thời gian theo s và thời gian theo us
(có thể bị tác động bởi chênh lệch thời gian thì OS hiệu chỉnh)
- steady_clock() của chrono : đếm trực tiếp trên CPU nên chính xác hơn gettimeoffday

___________________________________________

## II / RAII 

- Thu thập tài nguyên khi khởi tạo (Resource Acquisition Is Initialization - RAII) là một cơ chế thiết kế nhằm quản lý (quyền sở hữu) tài nguyên theo vòng đời (scope , ...) . Đóng gói các tài nguyên vào một class RAI (Constructor xin tài nguyên , Destructor nhả) và ép buộc phải dùng thông qua biến tự động (object) của class RAII 
- Ý tưởng cốt lõi : Khởi tạo đối tượng (dùng construction - Xin và tiếp nhận quyền quản lý tài nguyên) ->
sử dụng tài nguyên -> Hủy đối tượng (dùng destruction - Giải phóng các tài nguyên đã xin và tiếp nhận)
+ Construction = Hàm khởi động ngay khi đối tượng (obj) được khởi tạo nhằm kiểm tra các tài nguyên cần thiết , nếu thất bại và ném về ngoại lệ thì đối tượng bị hủy bỏ , các tài nguyên đã xin được cũng trả lại (nhưng nếu tài nguyên trong thân construction - tức tài nguyên thủ công do ta tự đặt vào vẫn có thể bị rò rỉ) (không nên ném ngoại lệ trong construction vì không biết làm gì)
+ Destruction = Hàm được trình biên dịch tự động chèn vào điểm kết thúc vòng đời (scope , return , break , throw , ... bất kỳ điều gì kết thúc vòng đời đối tượng đó) dùng để giải phóng tài nguyên dù cho có thoát scope sớm (nếu gặp sự cố destruction không xử lý được như ném ngoại lệ lúc stack unwinding -> std::terminate) .
+ Khi rời khỏi chương trình hoặc tháo gỡ stack (stack unwinding) các biến cục bộ sẽ bị giải phóng ngược quá trình khởi tạo (ví dụ là khởi tạo sẽ chèn lên đỉnh stack thì sẽ bốc ngược lại , có thể không phải stack nhưng đảm bảo tính ngược lại với quá trình khởi tạo)
+ RAII là một tư duy thiết kế hệ thống , ví dụ như Rule of Five gồm 5 điều cần quan tâm khi thiết kế bất cứ điều gì : Destruction , Copy/Move construction , Copy/Move assignment operato . RAII rất mạnh nhưng không đảm bảo 100% và nó chỉ quản lý tài nguyên chứ không đảm bảo tài nguyên đó thực hiện đúng quá trình khi gặp sự cố
+ unique_ptr và shared_ptr là 2 con trỏ áp dụng RAII rất rõ : unique_ptr - chỉ duy nhất 1 biến sở hữu , giải phóng biến địa chỉ đó thì bộ nhớ giá trị đó cũng đi còn shared_ptr chỉ giải phóng khi toàn bộ biến con trỏ cùng shared con trỏ đó đã được giải phóng 
+ Ta hạn chế copy vì tài nguyên sử dụng thường là lớn và cùng một địa chỉ gây phức tạp quá trình , nên dùng move để đổi chuyển giao quyền sở hữu và trách nhiệm dọn dẹp từ a -> b
- Không phải mọi tài nguyên đều có thể tuân theo RAII ví dụ như : thời gian xử lý CPU , lượng điện tiêu thụ , ... , tức RAII không thể quản lý quyền sở hữu , tức không đảm bảo rằng thao tác trả tài nguyên là tùy ý , ví dụ nếu các tài nguyên trên đối tượng không còn hoặc chỉ mượn tài nguyên thì việc giải phóng các tài nguyên không tồn tại là một lỗ hõng lớn . Nên RAII phải đảm bảo : Tài nguyên phải tuân thủ theo constructor nếu thành công hoặc giải phóng an toàn nếu thất bại ; Destructor phải đảm bảo xử lý được những việc trong trách nhiệm 

___________________________________________

# Day 008 : 16 / 10 / 2026

___________________________________________

## I / std::exchange
(Đọc wiki đê hehe)
- Tổng quát : chuyển giao quyền sở hữu đối tượng và trả về giá trị cũ
- Giải thích cụ thể từng thành phần lệnh : 
- noexpect(list) : kiểm tra các điều kiện để hạn chế nhất có thể ngoại lệ (lỗi) gồm 2 ý chính :
+ is_nothrow_move_constructible kiểm tra có thể move construct mà không ném ngoại lệ không
+ is_nothrow_assignable kiểm tra có thể gán mà không ném ngoại lệ không
-> Nếu pass thì thực thi lệnh của hàm
- Khái niệm : lvalue và rvalue , tạm hiểu lvalue là biểu thức xác định có thể định danh và dùng truy cập đối tượng còn rvalue có thể hiểu là loại biểu thức thường biểu diễn giá trị tạm thời thường dùng để tạo ra hoặc khởi tạo thứ khác (ví dụ : int x = 20 -> x là lvalue , 20 là rvalue )
- Dùng U&& (U = T) nhằm khi nhận dữ liệu truyền vào U sẽ giữ được loại của biến truyền vào là l/rvalue và quy tắc để ép loại (chỉ có /&& + && -> && , ngược lại -> & (lvalue = T& , rvalue = T))
(lưu ý ta cần U giữ thể loại vì truyền vào exchange U&& value : value luôn sẽ là lvalue)
+ move(T& obj) : biến obj -> rvalue rồi move constructor
+ forward<U> với forward là ép kiểu , U là kiểu chính xác giá trị value truyền vào 

___________________________________________

## II / Construtors , Destructors , Stactic (OOP - Nguyễn Minh Huy)

- Khi khởi tạo đối tượng để đảm bảo khỏi giá trị rác khởi tạo do trình biên dịch cung cấp ta cần constructors để điều thông tin cho dữ liệu . Có thể xem như "giấy khai sinh"
- Quy tắc : Constructors should never throw expection 
- Note : 
+ Đối tượng đã được tạo , Constructor hoàn tất việc khởi tạo đấy bằng một cam kết . Nếu constructor thất bại sẽ khó xử lý -> nên constructor trước khi đối tượng được khởi tạo xong
+ Khi dùng constructor tạo copy , tức ví dụ : a(b) (copy b -> a) thì b phải buộc dùng dạng const T& vì nó không copy mà dùng chính b , nếu copy sẽ gọi đệ quy vô hạn và lỗi
- Destructor là hàm dùng để xóa bỏ dữ liệu trong giấy khai sinh .
- Static là chỉ số ta thêm vào để tất cả đối tượng cùng class dùng chung (có thể xem như biến toàn cục của cấu trúc) . Khai báo trong hàm nhưng gán giá trị ở ngoài 
- (Constructor : <class>(args) ; Destructor : ~<class>())

___________________________________________

## III / std::unique_ptr 
(Tiếp tục đọc wiki đi %-%) (Giáo sư *^*)

- Là smart pointer có độc quyền sở hữu một hay một mảng đối tượng . Tức một đối tượng thì có thể có nhiều con trỏ chứa địa chỉ nó nhưng chỉ một con trỏ là có quyền sở hữu địa chỉ đó . Vì là quyền sở hữu độc nhất nên chúng ta không thể dùng pt __copy mà phải dùng __move (các kiến thức như khi nào tài nguyên giải phóng , deleter là và có tác dụng gì , ... thì đọc wiki) . Đi sau vào 5 đặc tính cần chú ý của unique_ptr :
- Deleter phải thõa mãn điều kiện gì ? Phải nhận vào con trỏ 
- Thao tác move bắt buộc điều gì ? Không được dùng const để kế thừa hay khởi tạo . Lý do : Khi dời quyền sở hữu tức đưa đối tượng sở hữu ban đầu thành rỗng , const thì không đổi được
- Có đảm bảo khi gặp expection ? Có vì nó áp dụng tư tưởng thiết kế RAII
- Để đi sau hơn 2 tính chất sau , Reze sẽ bổ sung kiến thức  :

___________________________________________

### a. Kiểu hoàn chỉnh và chưa hoàn chỉnh

- Điểm khác nhau : Kiểu chưa hoàn chỉnh là kiểu mà ta chưa hề biết trước dữ liệu , kích thước , ... gì về nó (ví dụ : class A; )
- Ta có thể tạo con trỏ chứa 1 biến chưa hoàn chỉnh (nhưng không thể thao tác hay tạo biến đó được) nên ta cũng có thể dùng unique_ptr (unique_ptr <A> p chứ unique_ptr p(new A) là sai vì destructor phải xóa 1 đối tượng hoàn chỉnh , dùng new là cấp bộ nhớ phải xóa với con trỏ)
- Kỹ thuật phổ biến nhất áp dụng kiểu dữ liệu chưa hoàn chỉnh là : PImpl
- PIml (Pointer to Implementation) : Viết chương trình chỉ cần giao diện công khai còn các chi tiết kỹ thuật bị ẩn đi (Gọi là BlackBox cho trường hợp này nhỉ ?)

### b. Base , Derived và "virtual" (thuộc về Kế thừa và đa hình)

- Base (Cơ sở) và Derived (Kế thừa) là hai khái niệm về loại class . Base tức là một class tổng quát cho một đối tượng . Còn Derived chính là việc đi vào một đối tượng cụ thể phát triển từ Bass (ví dụ class Vechile - class Motobike ; class Anime - class Dog ; có thể khai báo dạng Base p = new Derived)
- Chính từ việc Base p = new Derived có một vấn đề ; nếu ta chỉ dùng Dervied p = new Derived hủy thì nó sẽ gọi destructor : ~Derived , ~Base (vì Derived = Based cụ thể , chi tiết hơn) , còn đặt kiểu p mà dùng tạo derived sẽ khiến compiler khi chạy sẽ đối xử với nó như Base , bỏ qua việc gọi derived trong khi tạo bằng derived phải dùng destructor derived giải phóng tài nguyên . Dùng virtual để nói cho C++ biết là đi sâu vào và kiểm tra xem nó thực sự là gì 
- Vậy con trỏ virtual xử lý như nào ? Gồm 2 bước : 
+ Tạo bảng tra cứu (vtable) : Địa chỉ destructor cho riêng base hay derived
+ Nhét con trỏ ẩn trong bảng tra cứu ứng với địa chỉ destructor thực sự (~Dervied() -> ~Base())

___________________________________________

# Day 009 : 17 / 10 / 2026

___________________________________________

## I / Các câu hỏi định hướng sâu

___________________________________________

### a. Lưu thanh ghi User-mode vào Kernel Stack khi xảy ra Trap/Interrupt

- Lưu thanh ghi User-mode là do đang thực hiện trên CPU vật lý tức HardWare . Khi dùng Trap để báo lệnh thì sẽ ghi thông tin dựa trên cấu trúc CPU (thường trên kstack) rồi chuyển quyền và đồng thời cũng nâng lên mức kernel-mode và OS sẽ ghi dữ liệu vào bộ nhớ quản lý của đối tượng nếu chọn context-switch (hardware chưa chắc lưu hết user-mode mà os kernel cũng có thể lưu phụ một phần)
- Khi xảy ra Interrupt (bất đồng bộ) sẽ gọi được xử lý bằng cơ chế interrupt handler do hardware và OS đảm nhiệm và ghi dữ liệu vào kstack của tiến trình hiện tại vào kstack rồi chuyển giao cho OS toàn quyền kiểm soát và OS lúc này muốn context-swtich sẽ gọi switch trước tiên là lưu dữ liệu (đang ở kernel-mode) của tiến trình vào bộ nhớ quản lý rồi tráo địa chỉ con trỏ stack
- Xét sự tương đồng hai cơ chế này khá thú vị : Trap và interrupt đều chuyển quyền điều khiển từ user-mode sang kernel-mode thông qua cơ chế phần cứng và mã xử lý của OS

### b. Lưu thanh ghi Kernel-mode vào proc_struct (PCB) khi gọi switch(). Tại sao việc thay đổi thanh ghi Stack Pointer (SP) sang k-stack của tiến trình đích lại khiến lệnh return-from-trap chuyển quyền thực thi sang tiến trình mới ?

- Sở dĩ cần lưu vào proc_struct là vì khi chuyển từ user-mode sang kernel-mode thì trạng thái tiến trình sẽ khác với trạng thái tiến trình ở lúc kết hay sẽ khác với lúc gọi switch để context_switch khi gọi trap nên lưu vào proc_struct để đảm bảo khôi phục
- Thay đổi SP khiến lệnh return-from-trap chuyển quyền thực thi ta cần phải hiểu cơ chế thực thi và return-from-trap . Sau khi khôi phục trạng thái user-mode từ return-from-trap để chạy chương trình , OS cần nạp dữ liệu từ kstack thông qua địa chỉ sp để thực thi . Chính vì sp đã trỏ vào tiến trình đích thay vì tiến trình nguồn nên chuyển quyền thực thi thành công . Vốn dĩ CPU không biết tiến trình nào , chỉ quan tâm là trạng thái tiến trình thực thi

_________________________________________________________________

## II / Syscall (man syscall(2) và man syscalls(2)) và Register

_________________________________________________________________

- Đa phần các syscall dùng thư viện cung cấp là các hàm procedure được đính kèm lệnh trap (hay còn gọi là các hàm có bọc - wrapper) . Nhưng không phải hàm nào cũng có wrapper , khi đó ta cần dùng lệnh syscall như sau :
- syscall(number , ...) trong đó number là id lệnh thực thi , args là các tham số . Tuy nhiên vấn đề phức tạp là ở khi dùng syscall ta cần tuân thủ ABI của cấu trúc và hệ điều hành đang sử dụng
- Sự thay đổi và nhóm các lệnh syscall rất phức tạp , đi tra inf khi cần thao tác (Chisikudo Advise)
- Bus địa chỉ (Address Bus) là các đường tín hiệu (như dây dẫn) dùng để truyền địa chỉ của vị trí hoặc thiết bị cần truy cập . Tác dụng duy nhất : Truyền địa chỉ vùng nhớ mà CPU đang muốn đọc hoặc ghi
- Các thanh ghi : 
+ Tích lũy (phục vụ tính toán - lưu các phép tính và chứa kết quả) ; 
+ Đếm chương trình (PC) chứa địa chỉ ô nhớ lệnh kế tiếp (ở ô nhớ tiếp theo thường là sau khi CPU thực hiện xong 1 lệnh) ; 
+ Thanh ghi lệnh (IR) lưu lệnh đang thực hiện ; 
+ Thanh ghi MAR (Memory Address R) và MBR (Memory Buffer R) là giao diễn giữa CPU và và Bus địa chỉ , nhận địa chỉ ô nhớ từ PC -> bus địa chỉ (MRA) , nhận địa chỉ từ bus địa chỉ -> IR (MRB)
+ Con trỏ ngăn xếp (SP) chứa địa chỉ đỉnh ngăn xếp với 2 trò đẩy và lấy dữ liệu từ đỉnh ; 
+ Thanh ghi trạng thái (FR) : mỗi bit lưu trạng thái kết quả phép tính ALU được thực hiện (hoặc cụ thể hơn là các trạng thái như zero , carry , sign hoặc overflow);
+ Thanh ghi đa năng : Có nhiều chức năng như chứa toán hạng đầu vào hoặc đầu ra ;
+ Các thanh ghi tạm thời : chứa toán hạng đầu vào , hỗ trợ song song , ...
(GRP để truyền đối số ; lưu dữ liệu thường dùng GRP + PC + FR)

Note : Quá trình thực thi lệnh từ PC đến IR (cơ bản) :
+ B1 : PC giữ địa chỉ ảo lệnh cần thực hiện tiếp theo của CPU nằm trên address space
+ B2 : MAR sẽ nhận và giữ địa chỉ lệnh từ PC , sau đó thông qua Bus address (thường là CPU và RAM , có thể thông qua cơ chế truyền tải địa chỉ , trong các máy hiện đại thì như MMU , cache , ...) để truyền địa chỉ đó vào Memory (thường là RAM) xác định nội dung cần đọc và kiểm tra theo cơ chế hệ thống
+ B3 : Nội dung lệnh được truyền về vào đệm là MBR (đó là lý tên gọi Memory Buffer Register) thường thông qua Data Bus và đưa cho IR , IR giải mã nội dung và thực thi lệnh

### a. Khi gọi hàm bọc (wrapper) trong glibc như read() hoặc write(), User-space truyền mã định danh System Call (syscall number) và các đối số qua những thanh ghi CPU nào trước khi phát lệnh syscall/trap

- Bằng thanh ghi đa năng GRP (RAX , RDI , RSI , RDX , R10 , R8 , R9)

### b. Giá trị trả về và mã lỗi (errno) được Kernel trả ngược về User-space bằng cơ chế nào ?

- Khi gọi thất bại thì trả về một giá trị âm x , khi đó ta sẽ đặt errno bằng giá trị âm x đó và trả về -1 