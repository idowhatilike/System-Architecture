# **Phase 2 : GIỚI HẠN VẬT LÝ , BỘ NHỚ ĐỘNG VÀ ĐÁNH ĐỔI TRONG VIỆC LƯU TRỮ**

_______________________________________________________________________________

# Day 001 : **Hệ thống phân cấp bộ nhớ**

____________________________________________________________


## I / *__Kỹ thuật lưu trữ__*
(Phần lớn thành công của công nghệ máy tính bắt nguồn từ những tiến bộ vượt bật trong công nghệ lưu trữ)

### 1. Random Access Memory (RAM) 
- Chia làm 2 loại : Tĩnh (static RAM) và động (dynamic RAM) ; SRAM nhanh và đắt hơn DRAM . Vì vậy SRAM thường được dùng làm Cache (cả nằm trong hay ngoài CPU) , còn DRAM làm bộ nhớ chính

- **STATIC RAM** :
+ Lưu trữ mỗi bit trong ô nhớ 2 trạng thái ổn định (ô nhớ cấu tạo bởi 1 mạch 6 transistor)
+ Mạch có khả năng duy trì vô hạn 1 trong 2 trạng thái ổn định , nếu nằm ngoài 2 trạng thái này thì sẽ sớm chuyển về 1 trong 2 trạng thái trên do tính bất ổn định (hình dung thả đứng cây bút , 1 là ngã sang trái , hoặc sang phải về cứ thế) -> Tính chất giúp duy trì giá trị dữ liệu vô hạn miễn còn nguồn

- **DYNAMIC RAM** : 
+ Lưu trữ mỗi bit dưới dạng điện tích được tích tụ điện trên 1 tụ điện (kích thước rất nhỏ , tụ điện này chỉ có điện dung thông thường ~ 30 * 10^-15F) -> Nhờ đơn giản và nhỏ gọn nên DRAM đạt mật độ lưu trữ vô cùng cao (1 transistor truy cập + 1 tụ điện)
+ Vô cùng nhạy cảm với bất kỳ tác động gây nhiễu (kể cả tiếp xúc với ánh sáng) . Việc máy tính có chu kỳ xung nhịp (clock cycle) tính bằng nano-s . Hệ thống bộ nhớ refresh toàn bộ bit dữ liệu bằng cách đọc và ghi lại (thường là các hàng trong thời gian quy định) (một số hệ thống còn dùng mã lỗi ECC) . Ví dụ biến thể SEC , các từ máy tính được bổ sung bit , qua đó phát hiện và vá bất kỳ lỗi đơn bit nào trong 1 từ dữ liệu bởi các mạch xử lý

- So sánh :
+ SRAM dùng transistor vừa tạo mạch logic , vừa duy trì trạng thái trong khi DRAM dùng transistor như công tắc 
+ SRAM nhanh hơn nhưng đắt đỏ hơn

- **DRAMs truyền thống** : 
+ Một DRAM chip được chia thành d siêu ô bộ nhớ (supercell) (được biểu diễn dưới dạng rxc (r*c = d))
+ Mỗi supercell lại có w bit dữ liệu -> Tổng cộng d*w (= r * c * w) bit dữ liệu
+ Trong đó có 2 lại chân (Pins) : truyền địa chỉ ; truyền tín hiệu xử lý
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

### 2. Bộ nhớ bất khả biến (ROM)
- DRAMs và SRAM là bộ nhớ khả biến (có thể mất) còn bộ nhớ bất khả biến vẫn giữ được dữ liệu ngay cả khi thiết bị tắt nguồn , có nhiều loại bộ nhớ bất khả biến và ví lý do lịch sử mà được gọi là ROM (Ready-Only Memory) mặc dù một số loại vẫn cho ghi . Được phân biệt dựa trên 2 tiêu chí : *Ghi dữ liệu mới vào* (số lần được lập trình lại) ; *Cơ chế để lập trình lại* . 
+ 1. Programmable ROM (PROM) : ROM được lập trình đúng 1 lần , mỗi ô nhớ có cầu chì để nhận biết ghi
+ 2. Erasable PROM : Có 1 ô cửa sổ thạch anh trong suốt cho phép ánh sáng (tia cực tím) chiếu vào các ô nhớ (ví dụ để xóa và đưa giá trị về 0) , lập trình EPROM được thực hiện bằng 1 thiết bị chuyên dụng để ghi giá trị 1 vào (ước tính 1 chip EPROM được tái lập trình khoảng 1000 chu kỳ)
+ 3. Electrically EPROM : Giống với EPROM nhưng không cần thiết bị chuyên dụng và dùng được ~10^5 
+ 4. Flash Memory : Được phát triển trên EEPROM và trở thành 1 công nghệ quan trọng với tốc độ và độ bền . Ổ đĩa rắn (SSD) sử dụng bộ nhớ Flash là giải pháp thay thế các ổ đĩa truyền thống với tốc độ cao , độ bền tốt hơn và mức tiêu thụ điện năng thấp hơn 
+ Các chương trình được lưu trong ROMs được gọi là firmware , khi máy tính bật nguồn sẽ thực thi firmware , một số hệ thống cung cấp 1 phần nhỏ I/O ngay trong firmware như BIOS trên PC hoặc các thiết bị phức tạp dựa vào firmwave để chuyển đổi chính xác lệnh I/O từ CPU thành những thao tác tương ứng các thiết bị có thể thực hiện

- **Truy cập bộ nhớ chính** : Dữ liệu được truyền qua lại giữa bộ xử lý và bộ nhớ chính qua các đường dẫn điện dùng chung , gọi là bus . Mỗi lần truyền dữ liệu giữa CPU và bộ nhớ đều được thông qua một chuỗi các bước giao dịch gọi là giao dịch bus (Bus transaction - gồm 2 loại : read/write transaction)
. Một bus là tập hợp các dây dẫn song song để truyền tải dữ liệu , địa chỉ hay tín hiệu điều khiển (tùy thuộc vào kiến trúc thiết kế mà dữ liệu và tín hiệu có thể chung hay là hai bus riêng biệt) ; ngoài ra 1 bus có thể được chia sẻ bởi nhiều thiết bị . Các dây điều khiển mang những tín hiệu có chức năng đồng bộ hóa và xác định giao dịch đang thực hiện . Xem ví dụ 6.7 để hiểu rõ 


### 3. Đĩa lưu trữ
- Là thiết bị lưu trữ chính với bộ nhớ lớn nhưng tốc đốc truy cập rất chậm so với RAM

- Cấu trúc hình học đĩa : Được cấu tạo từ các phiến đĩa có 2 mặt đều được phủ vật liệu ghi từ có một trục quay nằm giữa với tốc độ góc không đổi (const w , thường w : (5400 -> 15000 RPM)) (Hình ảnh minh họa đĩa đơn giản cho ta biết vì sao lưu được rất nhiều dữ liệu , 1 sector thường ghi 512 byte) . Khi nói đến đĩa quay (rotating disk) để phân biệt với SSD (flash-based) vốn không dùng bộ phận chuyển động.
Mô tả cấu trúc hình học đĩa nhiều phiến = Cylinder (trụ) : tập hợp các rảnh có cùng khoảng cách -> tâm
(1 disk = c platter ; 1 platter = 2 surface ; 1 surface = n track (2 mặt) ; 1 track = m sector)

- Dung lượng đĩa : Là số bit tối đa có thể ghi trên 1 đĩa , được quyết định bởi các yếu tố : 
+ Mật độ ghi (Recording density - bits/in) : Số bit có thể ghi được trên 1 đoạn dài 1 inch của track
+ Mật độ rảnh (Track density - tracks/in) : Số rãnh có thể bố trị trên 1 đoạn bán kính 1 inch tính từ tâm phiến đĩa
+ Mật độ diện tích (Areal density - bits/in^2) : Tích mật độ ghi và mật độ rãnh
Các đĩa đời đầu khi AD còn thấp thì mỗi rảnh được chia thành các sector (xác định dựa vào rãnh trong cùng) nên cố định nên càng ra xa giữa sector sẽ tồn tại cách khoảng cách . Càng về sau thì thiết kế này tạo ra những khoảng cách quá lớn , nên sau này dùng kỹ thuật ghi theo nhiều vùng (multiple zone recording) - chia cylinder thành các nhóm không giao nhau gọi là vùng ghi (recording zone) -> các vùng ghi thì các rãnh có cùng số sector (tóm lại là chia nhỏ)
*Note* : Ta điều biết T = G*K = M*K*K = K^3 , thế nhưng với RAM (SRAM & DRAMs) thì K = 2^10 còn với dung lượng các thiết bị I/O như disk và mạng thì là K = 10^3
- Công thức : 
Dung lượng = (Số bit / sector) * (Số sectors trung bình / track) * (Số track / mặt) * (Số mặt / phiến) * (Số phiến / đĩa)

- Hoạt động đĩa : 
+ Thông qua đầu đọc/ghi được gắn trên cần truyền động , đọc/ghi theo từng khối kích thước = sector . Thời gian truy cập 1 sector chia làm 3 phần chính : thời gian tìm kiếm (seek time) ; độ trễ quay (rotational latency) và thời gian truyền dữ liệu (transfer time) :
+ Seek time : Di chuyển đến rãnh (di chuyển cần chuyển động và định vị đầu đọc) , ký hiệu Tseek , phụ thuộc vào vị trí trước đó của đầu đọc và tốc độ di chuyển , có giá trị trung bình thường 3-9 ms và có thể lên tới 20ms
+ Rotational latency : thời gian để bit đầu tiên của sector đích quay đến vị trí đầu đọc , phụ thuộc vào :  vị trí của đầu đọc khi vừa tới rãnh và bit đó ; tốc độ quay . Nếu đen đủi , chúng ta cần đợi quay cả 1 vòng , và độ trễ tối đa = (1 / RPM) * 60 (s) (giá trị trung bình = 1/2 * Tmax_rotation)
+ Transfer time : thời gian đọc/ghi tính từ lúc đọc/ghi bit đầu tiên ; phụ thuộc vào tốc độ quay và số sector trên track . Công thức thời gian trung bình : = (1 / RPM) * (1 / Số sector trung bình trên 1 track) * 60 (s) (dữ liệu đi qua đầu đọc ghi = đã thao tác)

- Khối logic trên đĩa :
Để đơn giản hóa cấu trúc khỏi OS , các ổ đĩa cung cấp B khốc logic (mỗi khối có kích thước = 1 sector) (đánh số từ 0->B-1) . Bộ điều khiển (disk controller) bao gồm phần cứng và firmware chịu trách nhiệm duy trì ánh xạ khối logic và sector vật lý , khi có thao tác thì 1 lệnh được gửi đến và bộ điều khiển tra cứu ánh xạ khối logic thành bộ ba tương ứng (surface ; track ; sector) duy nhất.
*Note* : Trước khi lưu dữ liệu , đĩa phải được bộ điều kiển định dạng (định danh sector , ...)

- Kết nối với các thiết bị I/O : Các thiết bị I/O kết nối với CPU và bộ nhớ qua bus I/O , được thiết kế để hoạt động độc lập với CPU (khác với system/memory bus) , tuy có tốc độ thấp nhưng hỗ trợ nhiều loại thiết bị I/O của bên thứ ba

- Truy cập ổ đĩa :
+ 1. Memory-mapped I/O : CPU gửi lệnh đến các thiết bị I/O bằng kỹ thuật memory-mapped I/O , trong đó hệ thống sử dụng 1 vùng không gian địa chỉ dành riêng để giao tiếp thiết bị I/O qua 1 cổng (được gọi là I/O port) . Khi một thiết bị I/O được nối , sẽ được gắn với 1 hoặc nhiều cổng I/O (lưu ý nhỏ : 1 GHz với chu kỳ xung nhịp 1ns có thể thực hiện 16m lệnh trong 16ms để đọc dữ liệu từ đĩa)
+ 2. Direct Memory Access (DMA) : Disk Controller khi nhận lệnh CPU sẽ : Chuyển đổi block id -> (surface ; track ; sector) duy nhất ; Đọc nội dung sector ; truyền trực tiếp nội dung đó vào bộ nhớ chính mà không cần CPU can thiệp => Quá trình đọc/ghi bộ nhớ không cần CPU tham gia gọi là DMA
+ 3. Interrupt : Sau DMA transfer (nội dung sector -> bộ nhớ chính) thì bộ điều khiển đĩa gửi tới CPU 1 tín hiệu ngắt
*__Note__* : Các tiến bộ trong thiết kế I/O : Thay vì nhiều thiết bị dùng chung 1 bus và tại 1 thời điểm chỉ có 1 thiết bị -> dùng các kết nối serial tốc độ cao , point-to-point , liên kết thông qua switch (nhanh hơn khoảng 30 lần) : 
+ Serial tốc độ cao : Bit đi lần lượt trên đường thay vì truyền nhiều bit song song trên nhiều dây 
+ P2P : Một kết nối là một đường truyền trực tiếp , không phải tranh nhau đường truyền chung

### 4. Ổ đĩa thể rắn (Solid State Disk - SSD)
- SSD là công nghệ lưu trữ trên bộ nhớ flash có thể thay thế ổ đĩa quay truyền thống vài trường hợp . Một thiết bị SSD bao gồm : N chip bộ nhớ flash + Flash Translation Layer (FTL) (vai trò ~ disk control)
- Cấu trúc và cơ chế ghi của bộ nhớ flash : Bao gồm B block , mỗi block gồm P page (thông thường : page có kích thước 512 byte -> 4KB ; block có kích thước 32-128 page với tổng dung lưỡng 16-512 KB) . Dữ liệu được đọc/ghi theo đơn vị page . Một page chỉ được ghi khi toàn bộ block chứa page đó bị xóa (thường tất cả bit gán thành 1) (block bị xóa thì mỗi page trong đó được ghi 1 lần mà không cần bị xóa thêm ; 1 block sẽ bị hao mòn sau 10^5 lần tái lập trình và không thể tiếp tục sử dụng) 
- Vì sao ghi ngẫu nhiên trên SSD chậm ? Vì 2 lý do sau :
+ Thao tác xóa block mất nhiều thời gian , việc xóa block ~ 1ms lâu hơn 1 bậc so với truy cập page
+ Phải sao chép dữ liệu trước khi ghi : khi thay đổi 1 page có chứa dữ liệu thì trước khi ghi phải copy mọi page đang chứa dữ liệu hữu ích khác sang block mới
- Các nxs đã phát triển những cơ chế xử lý phức tạp trong FTL nhằm giảm thời gian (chủ yếu 2 vấn đề đó)
- Ưu điểm : 
+ Cấu tạo từ bộ nhớ bán dẫn không có bộ phẩn chuyển động cơ học
+ Có thời gian truy cập nhanh hơn ổ đĩa quay
+ Tiêu thụ ít điện năng và độ bền vật lý cao
- Nhược điểm : 
+ Giới hạn tuổi thọ : Block hao mòn theo sử dụng , SSD có thể hỏng theo thời gian 
(Cơ chế wear-leveling kéo dài tuổi thọ bằng phân bổ các thao tác xóa đồng đều trên block)
+ Chi phí cao : giá thành trên mỗi byte cao hơn ssd 30 lần (nhưng đang giảm nhanh)

### 5. Xu hướng phát triển của công nghệ lưu trữ
- Sự đánh đổi giữa các công nghệ khác nhau : Chi phí và hiệu năng 
- Ví dụ : SRAM nhanh hơn DRAM chút ; DRAM nhanh hơn ổ đĩa rất nhiều ; nhưng chi phí thì ngược lại
Bạn có thể xem hình 6.15 trong CS:APP để theo dõi 1 bảng số liệu (không chính thức)
- Những xu hướng thay đổi dài hạn làm nổi bật 1 điều : Tăng mật độ lưu trữ giảm chi phí dễ thực hiện hơn nhiều so với giảm thời gian truy cập và hiệu năng ổ đĩa và DRAM đang tụt hậu so với CPU . Trước khi multi-core (đa nhân) thì khoảng cách do latency-DRAM và disk truy cập dữ liệu chậm hơn CPU , sau khi có đa nhân khoảng cách còn nới rộng do throughput (đa nhân cùng gửi truy vấn) . Giải pháp là dùng SRAM-based cache để thu hẹp 