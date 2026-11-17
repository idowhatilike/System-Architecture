# **Phase 2 : GIỚI HẠN VẬT LÝ , BỘ NHỚ ĐỘNG VÀ ĐÁNH ĐỔI TRONG VIỆC LƯU TRỮ**

_______________________________________________________________________________

# **Hệ thống phân cấp bộ nhớ**

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

____________________________________________________________

## II / Tính cục bộ (Locality)
- Một chương trình có thiết kế tốt có tính cục bộ cao (tức truy cập phần tử gần phần tử vừa truy cập) . Xu hướng này gọi là nguyên lý tính cục bộ (principle of locality) . Được chia thành 2 loại :
+ Cục bộ theo thời gian : Một bộ nhớ sau khi truy cập sẽ được truy cập nhiều lần trong tương lai gần
+ Cục bộ theo không gian : Truy cập đến bộ nhớ gần bộ nhớ đã truy cập ngay trước đó
- *Note* : Thay vì tăng tốc độ xung nhịp thứ vốn tiêu thụ quá nhiều điện năng (giới hạn công suất) thì bước tiến công nghệ đầu thế kỷ 21 là thay thế 1 bộ xử lý đơn lớn = nhiều lõi xử lý nhỏ (P = f * C * V^2) trong đó : f = xung nhịp , C điện dung và V điện áp (Định luật Moore tốc độ tăng tiến hàm mũ)

### 1. Tính cục bộ của các tham chiếu (Dữ liệu chương trình)
- Mẫu truy cập bước k = truy cập cách đều mỗi k phần tử
- Mẫu truy cập bước 1 còn gọi là truy cập tuần tự
Ví dụ : Nếu có mảng a(M,N) , truy cập (1->M)(1->N) (k = 1) tốt hơn (1->N)(1->M) (k = N)

### 2. Tính cục bộ theo lệnh nạp (Locality of Instruction Fetches)
- Vì các lệnh chương trình được lưu trong bộ nhớ và phải được CPU nạp để thực thi nên ta có thể đánh giá tính cục bộ của chương trình dựa trên việc nạp . Một điểm quan trọng là mã lệnh khác với dữ liệu chương trình là hiếm khi bị thay đổi 

### 3. Tổng kết 
- Giới thiệu sơ bộ và 1 số đặc tính như : 
+ Các chương trình lặp đi lặp lại 1 biến -> Tốt về thời gian cục bộ
+ Các chương trình với mẫu truy cập bước càng nhỏ -> Tốt về không gian cục bộ
+ Các vòng lặp có tính cục bộ tốt theo thời gian , không gian có lợi cho việc nạp lệnh . Phần thân vòng lặp càng nhỏ , số lần lặp lại càng nhiều thì tính cục bộ càng tốt

____________________________________________________________

## III / Hệ thống phân cấp bộ nhớ (The memory hierarchy)
- Sự bổ trợ cho nhau giữa phần cứng/mền tạo ra 1 pp tổ chức các hệ thống bộ nhớ (memory hierarchy)

### 1. Cơ chế lưu đệm trong hệ thống phân cấp bộ nhớ
- Nhìn chung là thiết bị lưu trữ nhỏ , tốc độ cao , là vùng lưu trữ trung gian cho các đối tượng dữ liệu lớn nhưng chậm hơn . Ý tưởng cốt lõi : Cấp k đóng vai trò cache cho thiết bị cấp k + 1 . Ví dụ : Ổ đĩa là bộ nhớ đệm cho các tệp từ các ổ đĩa server ; bộ nhớ chính ... ổ đỉa ; ... ; thanh ghi cpu . Vì vậy các cấp bộ nhớ càng xa CPU thường sử dụng block có kích thước lớn để giảm ảnh hưởng thời gian truy cập dài hơn (1 lần đi 5MB = 5 lần đi , mỗi lần 1 MB)
- *_Cache Hit_* : Khi cần một đối tượng dữ liệu d từ tầng k + 1 , nếu đối tượng nằm ở cấp k thì có thể đọc trực tiếp , ta gọi là cache hit
- *_Cache Miss_* : Ngược lại , cache ở cấp k sẽ lấy dữ liệu từ cấp k + 1 , nếu đã đầy phải ghi đề lên 1 block đang được lưu tại đó (block bị thay thế hoặc loại bỏ được gọi là victim block) . Block bị thay thế dựa trên chính sách 
- Các loại Cache Miss : cold miss (khi cache cấp k rỗng) -> linh hoạt nhưng tốn kém : mọi block cấp k + 1 lưu vào -> block bất kỳ cấp k hoặc : giới hạn vị trí lưu của mỗi block cấp k + 1 vào 1 tập con nhỏ cấp k . Các chương trình thường trải qua nhiều giai đoạn , mỗi giai đoạn truy cập đến 1 tập block cache tương đổi ổn định , được gọi là working set của tập đó 
- Quản lý Cache : Mỗi cấp cần cơ chế quản lý gồm : chia vùng lưu trữ thành các block ; truyền các block giữa các cấp ; xác định cache hit và cache miss (đồng thời xử lý) được triển khai bằng phần cứng , mềm hoặc cả 2

### 2. Tổng kết 
- Dựa trên 2 yếu tố : Chương trình có tính cục bộ ; Bộ nhớ có tốc độ truy vấn nhanh có chi phí cao hơn
- Khai thác tính cục bộ thời gian : Nhờ vào tính lặp đi lặp lại và lần ghi vào cache đầu tiên
- Khai thác tính cục bộ không gian : Kỳ vọng chi phí sao chép cache miss bù đắp ở cách lần truy cập

____________________________________________________________

## IV / Bộ nhớ đệm (Cache Memories)

- Ban đầu thì chỉ có 3 cấp bộ nhớ : Thanh ghi CPU ; Bộ nhớ ; Bộ lưu trữ đĩa . Nhưng khoảng cách hiệu năng CPU và bộ nhớ chính , nsx bổ sung bộ nhớ đệm SRAM nhỏ gọi là L1 Cache

### 1. Cấu trúc tổng quan bộ nhớ đệm
- Xét 1 hệ thống máy tính mỗi địa chỉ có m bit (M = 2^m địa chỉ khác nhau) . Cache được tổ chức :
+ cache = mảng S = 2^s set ; 
+ set = E cache line ; 
+ cache line : 1 valid bit ; t (= m - b + s) bits tag được lấy 1 phần từ địa chỉ bộ nhớ của block ; b byte data (B = 2^b) (t bits tag là địa chỉ trong bộ nhớ chính và dùng trong việc đối chiếu block data)
-> Ta sẽ dùng (S,E,B,m) = cache struct và dung lượng là : C = S * E * B 
- Vậy làm thế nào để cache biết cache hit hay miss :
+ Chia địa chỉ m bit thành : t bits (Tag) + s bits (Set Id) + b bits (Block offset)
+ Đầu tiên nó truy cập vào St = set[Set Id] ; Nếu trong St có line sao cho t bits tag = Tag và valid bit = 1 -> line trong set chứa data cần tìm và vị trí qua Block offset

### 2. Cache ánh xạ trực tiếp (DR-Mapped Cache)
- Cache được phân loại dựa trên số lượng cache line (E) và set có E = 1 gọi là direct-mapped cache
- Quá trình cache xác định một yêu cầu miss hay hit có 3 bước : 
+ Set selection -> Line matching -> Word extraction (ta sẽ dùng quy trình trong DMC cho đơn giản nhất)
- Set selection : Đơn giản là lấy s bit index phần giữa địa chỉ rồi truy cập vào set đó
- Line matching : Valid bit = 1 và Tag trong cache line trùng với Tag rút ra được từ đầu địa chỉ
- Word selection : Nếu hit ở Line matching , ta dùng offset xác định vị trí (1 block = n bytes)
- Line replacement : Khi miss , truy vấn dữ liệu từ cache thấp hơn và thay thế cache line tương ứng

### 3. Conflict miss 
- Conflict miss thường xuyên xảy ra và gây ra vấn đề hiệu năng khó lý giải . Ví dụ DMC và size = 2^n thì truy cập mảng , ví dụ dot product 2 mảng n = 8 (Sum(x[i] * y[i])) , vì liên tục cache miss và thay thế block (x0 -> x4 ; y0 -> y4 = set0 cứ lặp đi lặp lại 2 block này - giả sử block chứa được 4 giá trị)
*Note* : Lý do tại sao dùng các bit giữa thay vì đầu là giúp nó trải đều chỉ số các block ra thay vì tụ lại và giữ nguyên id block giữa các byte trong block

### 4. Liên kết theo tập (Set associative cache)
- Conflict miss trong DMC bắt nguồn từ việc E = 1 . SAS nới lỏng cho set chứa nhiều cache line hơn (E > 1 ; và E < C / B thường gọi là liên kết hợp E đường - E way SAS)
- Line matching & Word selection : Nah nothing , 1 lý thuyết mà dùng
- Line replacement : Khi miss , trích dữ liệu và thế vào : line chưa set valid bit (nếu có) ; ngược lại thì may rủi :> mong là thế cái line vô dụng , và policy rất rất phức tạp

### 5. 1 set duy nhất (E = C/B)
### 6. Các vấn đề khi ghi dữ liệu 
- Thao tác write phức tạp hơn read 1 tý . Nếu miss cơ bản ta sẽ cập nhật ở tầng dưới và cập nhật line replacement như read nhưng khi hit , sau khi cập nhật bản sao của w trong cache hiện tại ta cần xử lý ở tầng dưới . Đơn giản nhất là cập nhật trực tiếp xuống , gọi là write-through , nhưng như thế tạo ra số lượng truyền trên bus rất lớn , ta có thể dùng "write-back" (chỉ ghi khi block đó bị loại ở cache) nhưng phải thêm dirty bit để biết bị sửa chưa . Còn với xử lý write-miss : 
+ write-allocate : nạp block tương ứng lên rồi cập nhật trong bản sao đó nhầm tận dụng spatial locality (cbkg) nhưng cần truyền block lên mỗi write-miss
+ no-write-allocate : ghi thông thường xuống dưới 
- Ta có 2 trường phái : 
+ write-through + no-write-allocate hoặc + write-back + write-allocate (ưu tiên cho pro hehe)

### 7. Cấu trúc của 1 hệ thống phân cấp cache thực tế
### 8. Ảnh hưởng các thông số của cache đến hiệu năng 
- Được đánh giá qua vài chỉ số
- Miss rate = (Số lần miss) / (Số lần truy cập) và Hit rate = 1 - Miss_rate
- Hit time : Thời gian đưa 1 word có trong cache vào CPU 
- Miss penalty : Thời gian phát sinh thêm khi miss 1 lần
- Ảnh hưởng dung lượng cache : càng lớn hit rate càng lớn , hit time càng nhỏ
- Ảnh hưởng kích thước block : Tăng hit rate , nhưng khi ta cố định dung lượng cache thì ít cache line -> làm giảm hit rate ? và tăng miss penalty (trade-off của quỷ)
- Ảnh hưởng liên kết (tạm xét E) : giảm thrashing do conflict miss nhưng khi E cao thì đpt tăng thì hit time và miss penalty tăng
- Ảnh hưởng chiến lược ghi : Càng đi xuống thấp càng chuộng write-back


_____________________________________________________________________________________________


# **PAGING : PHÂN TRANG**

______________________________________________________________________________________________

## I / Introduction 
- MMU — bộ quản lý bộ nhớ , PTE là Page Table Entry - 1 phần tử của PT (không nhất thiết phải là PT[0])
- Bảng Trang (Page Table) được lưu ở đâu ? Vì bản thân bảng trang lớn và nếu nhiều tiến trình thì sẽ là
một lượng tài nguyên vô cùng , giả sử mỗi trang được lưu trong bộ nhớ chính (RAM)
- Bảng trang là ánh xạ VPN -> PFN . Dạng đơn giản nhất là linear page table - bảng tuyến tính : 
+ valid bit - cho biết ánh xạ có hợp lệ không , không thì trap (và os thường kết liễu tiến trình)
+ protection bits - các bit cho biết quyền được đọc , ghi , thực thi (nếu truy cập sai thì cũng sẽ gọi trap)
+ ngoài ra còn : present bit ; dirty bit ; reference bit (theo dõi trang được cập nhật chưa) ; ...
- Phân trang vẫn quá chậm ? Vì lưu trong RAM (DRAM) làm chậm tốc độ . Để từ VPN -> PFN . Cơ bản là muốn đọc từ bộ nhớ vật lý cần địa chỉ gồm : PFN + offset (lấy từ vpn_mask) , mà PFN thì cần id (liên quan đến ptbr , vpn và size(pte))

## II / Faster Translations (TLBs)
- Tăng tốc dịch địa chỉ bằng phần cứng tên là Translation-lookaside buffer - TLB (thuộc MMU)
- Về bản chất là cache phần cứng lưu ánh xạ địa chỉ ảo sang bộ nhớ vật lý thường được sử dụng 
- Thuật toán cơ bản : Tạo 1 cache với mong muốn TLB hit nhiều bằng sự tối ưu tính cục bộ
- Xử lý TLB Miss : Có hai khả năng - Phần cứng (hardware) hoặc Phần mền (OS)
+ Phần cứng : nhóm CISC (Complex Instruction Set Computer - tập các lệnh phức tạp của máy tính) . Khi đó phần cứng cần biết : Page Table nằm ở đâu trong bộ nhớ (PTBR) ; Cấu trúc của PT . Khi gặp TLB miss , phần cứng chỉ cần dò theo cấu trúc và tìm đúng entry lấy thông tin - gọi là page-table walk
+ Phần mềm : Các hệ thống ra đời muộn (ví dụ : MIPS R10k thuộc RISC (Reduced Instruction Set Computer — máy tính có tập lệnh rút gọn)) . Khi TLB miss , phần cứng sinh expection gọi trap và xử lý . Có 2 điểm cần lưu ý : Ta thực hiện tiếp lệnh gây TLB miss chứ không bỏ qua như systemcall thông thường ; TrapHandler trong trường hợp này không được gây ra chuỗi TLB miss vô hạn (có thể giải quyết bằng nhiều cách như 1 số mã truy cập trực tiếp ; dùng wired translations - ánh xạ cố định)
- Những gì được chứa trong TLB : 1 TLB điển hình có thể có 32 , 64 hoặc 128 entry (theo kiểu fully associative) : VPN + PFN + Các bit bổ sung (valid bit ; protections bíts ; dirty bit ; ....)
- Context Switch : VPN <-> PFN của mỗi tiến trình là khác biệt . Nội dung TLB xử lý như nào khi context switch ? Cách đơn giản nhất là : flush TLB (TLB thành rỗng) (nếu do OS đảm nhiệm thì dùng lệnh đặc quyền ; hoặc đổi thanh ghi địa chỉ cơ sở PTBR - thứ vốn đổi khi ctx switch làm đổi sang không gian địa chỉ khác) - sau đó set valid bits = 0 . Để giảm chi phí , 1 số hệ thống bổ sung phần cứng giữ lại ánh xạ của nhiều tiến trình trong TLB qua các lần ctx switch , thường là address-space identifier — ASID (mã định danh vào không gian địa chỉ mở)
- Thay thế : Có nhiều Policy về việc nên thay thế entry nào (giống như mọi cache thông thường) . Phổ biến như LRU - loại bỏ entry lâu chưa sử dụng hay random 
- Culler's Law : RAM không phải lúc nào cũng là RAM
- Và ta sẽ nguyên cứu cách truy cập cache = địa chỉ ảo 
- https://fanout.sh/labs/latency-numbers

- P18 bài tập :
1. Tăng lên ; Giảm đi ; Gây lãng phí tài nguyên (cấp phát Page ; Chiếm dụng ; Lãng phí PM ; Load lâu)
2. PTEs = Size(Adr) / Size(PT) ; ... 
- P19 bài tập :
Lab : 
- Chênh lệch không lớn như trong lab
- Bench_mark : thuần (chỉ cách biệt rõ rệt khi N lớn) và khi cho extra_data -> 128 thời gian thì SoA tăng nhiều hơn là AoS (dù vẫn nhỏ hơn 2 lần) -> AoS rất bất định trong số đo

Phần 7 lab :
Lab 1 : Số lượng cache miss gần như là như nhau , còn về loaded miss tức là phần không dùng thì đúng , gấp gần 16 lần do phí 15 phần tử mà 
Lab 2 : Không tới 2-4x , N càng lớn thì càng hiệu quả nhưng đo N=10^5-10^6 thì 1.1-1.4x 
Lab 3 : Không có thao tác đọc từ đĩa mà chỉ có ghi vào trang vật lý đã ghi
Phần 8 thì : AoS đo giữa các lần lệch khá lớn và khi dùng cách upd nguyên 1 obj hay upd lần lượt từng thuộc tính của obj sẽ có lợi AoS hay SoA chỉ thực sự có chênh lệch lớn khi N lớn . Còn việc thêm data thì có thể hiểu như việc SoA nó tách cái struct ra thành các phần riêng , và có vài cái không dùng thì không quan tâm , còn SoA tức là tạo ra 1 obj thực sự hoàn chỉnh mà compile không tối ưu được , nếu như cả 2 đều dùng thì sẽ khác