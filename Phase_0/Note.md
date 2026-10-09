# Day 002 : Introducion to OS (9 / 10 / 2026)

---

## I/ OS là gì và tác dụng

Chạy chương trình = Thực thi vô số lệnh lệnh (instructions) . Tiến trình (processor) sẽ thực hiện ba bước : 
- Tìm và nạp (fetches) lệnh từ bộ nhớ
- Giải mã (decodes) lệnh (Hiểu , tìm ra lệnh này là gì ?)
- Thực thi (executes) lệnh (Làm theo đúng những gì lệnh bảo)
Đó cơ bản là mô hình máy tính đơn giản theo Von Neumann .
Đơn giản nhưng khi chạy cần vô số thao tác để khiến hệ thống dễ dùng .
Và phần mền (software) chịu trách nhiệm khiến dễ dàng chạy chương trình như :
+ Cấp phát và chia sẽ bộ nhớ
+ Cho phép ứng dụng thao tác với thiết bị
+ ... v.v
-> Gọi là Operating System (OS) . Thứ giúp chúng ta dễ dàng thao tác máy tính
- Kỹ thuật kinh điển của OS để giúp chúng ta dễ dàng hơn là sự ảo hóa . Biến tài nguyên cứng -> ảo , nên ta gọi OS = máy ảo (virtual machine) 
- OS còn cho phép chúng ta thao tác với tính năng của máy ảo và cung cấp hoàng loạt các API (interfaces) cũng như vô số lệnh gọi hệ thống (system calls) nên ta còn gọi OS = thư viện chuẩn (standard libary) . Và vì đồng thời quản lý tài nguyên , như ảo hóa các tài nguyên và đồng thời cho các ứng dụng tiếp cận đến vùng tài nguyên cần thiết như dữ liệu (data) hay thiết bị (devices) (chia sẽ tài nguyên) nên ta gọi OS = quản lý tài nguyên (resource manager) -> OS là phần mềm có nhiệm vụ : quản lý tài nguyên , thư viện chuẩn và máy ảo 

# II/ Ảo hóa CPU (Virtualizing the CPU)

- Bằng việc dùng một số thiết bị phần cứng (hardware devices) mà OS tạo ra ảo giác biến 1 CPU (hay 1 tập hợp nhỏ) thành vô số CPUs , gọi đó là Ảo hóa CPU (đơn nhiên ta phải dùng APIs để ưu cầu OS thực hiện các thao tác với các CPU ảo này - APIs là mà OS giúp chúng ta đơn giản hóa thao tác) . Và có nhiều câu hỏi nãy sinh sẽ được trả lời bằng các luật (policy) và cơ chế (mechanisms) (quản lý các cpu ảo = quản lý tài nguyên = OS)

# III/ Ảo hóa bộ nhớ (Virtualizing Memory)

- Trước tiên bộ nhớ vật lý vô cùng đơn giản : Dãy các bytes , ta đọc dữ liệu nếu có địa chỉ và viết (hoặc cập nhật) bộ nhớ thông qua địa chỉ được cho
Ảo hóa bộ nhớ là việc đưa cho mỗi tiến trình xử lý một địa chỉ ảo (thực tế là bộ nhớ vật lý đang được chia sẽ nhưng đảm bảo không đè nhau mà hỏng)

# IV/ Tính đồng thời (Concurrency)

-Tính đồng thời là một trong những vấn đề lớn không chỉ trong OS mà hiện nay còn là đa luồng . Khi OS phải chạy quá trình đầu tiên , thứ hai , v.v sẽ xảy ra các lỗi và sự bất thống nhất . Lấy ví dụ việc lập loops và nâng biến đếm cnt , lỗi ở đâu . Lý do nằm ở việc nâng biến đếm bản chất gồm ba thao tác : Đọc thông tin từ bộ nhớ , Ghi lại giá trị và Ghi lại vào địa chỉ bộ nhớ . Nhưng ba hành động này không diễn ra đồng thời lập tức nên những điều kỳ lạ diễn ra

# V/ Tính kiên trì (Persistence)

-Dữ liệu rất dễ thất thoát , đặc biệt khi DRAM lưu dữ liệu theo phương pháp bay hơi , một sự có như sự cố hệ thống hoặc sập nguồn là mất mát mà không ai mong muốn . Vì vậy ta cần cả phần cứng và mềm của máy tính lưu thiết bị một cách kiên trì . Phần cứng như các thiết bị I/O , ổ đĩa cứng (hay gần đây là ổ đĩa rắn) chịu trách nhiệm cho các thông tin lâu dài (long lived inf) 

- Phần mềm chịu trách nhiệm đó gọi là tệp tin hệ thống (file system) . Khác với CPU hay RAM , nó đã là một tầng trựu tượng của OS (không cần qua ảo hóa của OS) vì giả sử nhu cầu thiết yếu của người dùng là chia sẽ file (ta không thể viết code tren editor , rồi biên dịch bằng gcc rồi chạy file nếu ảo hóa và tách ra thành từng file system riêng , điều đó rất khó hoặc bất khả thi) . Phổ biến là dùng các lời gọi hệ thống định tuyến đến tệp tin hệ thống  
 
- Cách tệp tin hệ thống ghi dữ liệu :
+ Xác định vị trí dữ liệu sẽ nằm đâu và giữ quan sát thông qua nhiều cấu trúc dữ liệu khác nhau do tệp tin hệ thống duy trì . Để làm được việc này thì tệp tin hệ thống phải liên tục yêu cầu các thao tác I/O tới thiết bị dưới nhằm đọc các cấu trúc đang tồn tại hoặc viết (hoặc cập nhật) . Các thao tác này được đơn giản hóa nhờ OS thông qua lời gọi hệ thống (OS = thư viện chuẩn) (nếu tự tay thực hiện thì việc này vô cùng khó đòi hỏi sự tỉ mĩ và phức tạp với hiểu biết về thiết bị low-level và ngữ nghĩa (exact semantics)) 