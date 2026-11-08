# **Phase 2 : GIỚI HẠN VẬT LÝ , BỘ NHỚ ĐỘNG VÀ ĐÁNH ĐỔI TRONG VIỆC LƯU TRỮ**

_______________________________________________________________________________

# Day 001 : **Hệ thống phân cấp bộ nhớ**

____________________________________________________________


## I / Kỹ thuật lưu trữ
(Phần lớn thành công của công nghệ máy tính bắt nguồn từ những tiến bộ vượt bật trong công nghệ lưu trữ)

### 1. Random Access Memory (RAM) 
- Chia làm 2 loại : Tĩnh (static RAM) và động (dynamic RAM) ; SRAM nhanh và đắt hơn DRAM . Vì vậy SRAM thường được dùng làm Cache (cả nằm trong hay ngoài CPU) , còn DRAM làm bộ nhớ chính

- **STATIC RAM** :
+ Lưu trữ mỗi bit trong ô nhớ 2 trạng thái ổn định (ô nhớ cấu tạo bởi 1 mạch 6 transistor)
+ Mạch có khả năng duy trì vô hạn 1 trong 2 trạng thái ổn định , nếu nằm ngoài 2 trạng thái này thì sẽ sớm chuyển về 1 trong 2 trạng thái trên do tính bất ổn định (hình dung thả đứng cây bút , 1 là ngã sang trái , hoặc sang phải về cứ thế) -> Tính chất giúp duy trì giá trị dữ liệu vô hạn miễn còn nguồn

- **DYNAMIC RAM** : 
+ Lưu trữ mỗi bit dưới dạng điện tích được tích tụ điện trên 1 tụ điện (kích thước rất nhỏ , tụ điện này chỉ có điện dung thông thường ~ 30 * 10^-15F) -> Nhờ đơn giản và nhỏ gọn nên DRAM đạt mật độ lưu trữ vô cùng cao (1 transistor truy cập + 1 tụ điện)
+ Vô cùng nhạy cảm với bất kỳ tác động gây nhiễu (kể cả tiếp xúc với ánh sáng) . Việc máy tính có chu kỳ xung nhịp (clock cycle) tính bằng nano-s , hệ thống bộ nhớ Refresh toàn bộ bit dữ liệu bằng cách đọc và ghi lại (một số hệ thống còn dùng mã lỗi ECC) . Ví dụ biến thể SEC , các từ máy tính được bổ sung bit , qua đó phát hiện và vá bất kỳ lỗi đơn bit nào trong 1 từ dữ liệu bởi các mạch xử lý

- So sánh :
+ SRAM dùng transistor vừa tạo mạch logic , vừa duy trì trạng thái trong khi DRAM dùng transistor như công tắc 
+ SRAM nhanh hơn , bền hơn nhưng đắt đỏ hơn

- **DRAMs truyền thống** : 
+ Một DRAM chip được chia thành d siêu ô bộ nhớ (supercell) (được biểu diễn dưới dạng rxc (r*c = d))
+ Mỗi supercell lại có w bit dữ liệu -> Tổng cộng d*w (= r * c * w) bit dữ liệu
+ 2 chân địa chỉ (Address Pins) truyền địa chỉ (i,j) supercell , biễu diễn bằng 2 bit . Các chân khác (Pins) dùng để truyền tín hiệu
+ Mỗi chip DRAM được kết nối với Memory Controller , thứ có khả năng truyền w bit dữ liệu đến/đi -> DRAM chip trong 1 lần . Để đọc dữ liệu thì cần 2 bước cơ bản : Gửi địa chỉ hàng i -> DRAM (thao tác này gọi là RAS - Row Access Stroble) ; Gửi địa chỉ cột j -> DRAM (CAS) . DRAM chip sẽ phản hồi nội dung
+ Cả CAS và RAS đều dùng chung chân địa chỉ , gửi hàng tạo buffer nội bộ rồi bóc cột j khi nhận được
+ Được thiết kế hcn để giảm số lượng chân địa chỉ nhưng tăng thời gian truy cập bộ nhớ
(Mỗi chân địa chỉ - Pin chỉ biễu diễn 1 bit)

- **Mô đun (Modules) bộ nhớ** :
+ Các chip DRAM được đóng gói thành module bộ nhớ , sau đó cắm vào khe mở rộng trên bo mạch chủ (motherboard) , vd : Corei7 dùng loại DIMM 240 chân , có khả năng truyền dữ liệu giữa module và Memory controller theo các block 64 - bit (làm ơn ai đó làm ra 67 bit đê %v-v%)

- **DRAMs cải tiến** :  Liên tục các loại DRAM mới xuất hiện cố gắng theo kịp tốc độ xử lý của bộ xử lý . Đều được phát triển trên DRAM truyền thống nhưng được cải tiến và bổ sung nhằm tăng tốc độ : 
+ 1. Fast Page Mode DRAM (FPM DRAM) : Thay vì chỉ chép 1 lần dữ liệu hàng i để đọc ô (i,j) thì cải tiến cho các lần gọi liên tiếp nhằm tận dụng tối đa hàng i (chỉ gọi đúng 1 lần)
+ 2. Extended Data Out DRAM : Cải tiến FPM DRAM cho phép gửi tín hiệu CAS liên tiếp được rút ngắn
+ 3. Synchronous DRAM : DRAM thông thương , EDO hay FPM đều sử dụng cơ chế bất đồng bộ , thay vào đó SDRAM thay thế nhiều tín hiệu bằng cách cạnh lên (rising edges) cho phép xuất nội dung supercell nhanh
+ 4. Double Data-Rate Synchronous DRAM : Bản cải tiến SDRAM cho phép tăng gấp đôi tốc độ truyền dữ liệu bằng cách sử dụng cả hai cạnh của tín hiệu xung nhịp làm tín hiệu điều khiển
+ 5. Video RAM : Dùng trong các bộ đệm khung hình , có nhiều điểm tương đồng FPM DRAM nhưng có 2 khác biệt chính : *Dữ liệu đầu ra được dịch chuyển tuần tự toàn bộ nội dung bộ đệm* ; *Cho phép đồng thời thao tác đọc ghi*
(Fact vv : 1995 : FPM , 1996 - 1999 : EDO , 2002 : SDRAM , 2010 : DDR3 SDRAM)

- **Bộ nhớ bất khả biến** : DRAMs và SRAM là bộ nhớ khả biến (có thể mất) còn bộ nhớ bất khả biến vẫn giữ được dữ liệu ngay cả khi thiết bị tắt nguồn , có nhiều loại bộ nhớ bất khả biến và ví lý do lịch sử mà được gọi là ROM (Ready-Only Memory) mặc dù một số loại vẫn cho ghi . Được phân biệt dựa trên 2 tiêu chí : *Ghi dữ liệu mới vào* (số lần được lập trình lại) ; *Cơ chế để lập trình lại* . 
+ 1. Programmable ROM (PROM) : ROM được lập trình đúng 1 lần , mỗi ô nhớ có cầu chì để nhận biết ghi
+ 2. Erasable PROM : Có 1 ô cửa sổ thạch anh trong suốt cho phép ánh sáng (tia cực tím) chiếu vào các ô nhớ (ví dụ để xóa và đưa giá trị về 0) , lập trình EPROM được thực hiện bằng 1 thiết bị chuyên dụng để ghi giá trị 1 vào (ước tính 1 chip EPROM được tái lập trình khoảng 1000 chu kỳ)
+ 3. Electrically EPROM : Giống với EPROM nhưng không cần thiết bị chuyên dụng và dùng được ~10^5 
+ 4. Flash Memory : Được phát triển trên EEPROM và trở thành 1 công nghệ quan trọng với tốc độ và độ bền . Ổ đĩa rắn (SSD) sử dụng bộ nhớ Flash là giải pháp thay thế các ổ đĩa truyền thống với tốc độ cao , độ bền tốt hơn và mức tiêu thụ điện năng thấp hơn 
+ Các chương trình được lưu trong ROMs được gọi là firmware , khi máy tính bật nguồn sẽ thực thi firmware , một số hệ thống cung cấp 1 phần nhỏ I/O ngay trong firmware như BIOS trên PC hoặc các thiết bị phức tạp dựa vào firmwave để chuyển đổi chính xác lệnh I/O từ CPU thành những thao tác tương ứng các thiết bị có thể thực hiện

- **Truy cập bộ nhớ chính** : Dữ liệu được truyền qua lại giữa bộ xử lý và bộ nhớ chính qua các đường dẫn điện dùng chung , gọi là bus . Mỗi lần truyền dữ liệu giữa CPU và bộ nhớ đều được thông qua một chuỗi các bước giao dịch gọi là giao dịch bus (Bus transaction - gồm 2 loại : read/write transaction)
. Một bus là tập hợp các dây dẫn song song để truyền tải dữ liệu , địa chỉ hay tín hiệu điều khiển (tùy thuộc vào kiến trúc thiết kế mà dữ liệu và tín hiệu có thể chung hay là hai bus riêng biệt) ; ngoài ra 1 bus có thể được chia sẻ bởi nhiều thiết bị . Các dây điều khiển mang những tín hiệu có chức năng đồng bộ hóa và xác định giao dịch đang thực hiện . Xem ví dụ 6.7 để hiểu rõ  