# 1 / 
Giữ nguyên vì thông tin trong không gian bộ nhớ con giống trong không gian bộ nhớ cha . Khi thay đổi thì cả 2 cùng 1 giá trị x nhưng lúc này là 2 x phân biệt

# 2 / 
Vì cơ chế tạo bản sao , bao gồm cả file description (cùng 1 FD) nên cả 2 tiến trình sẽ tranh chấp tài nguyên (cùng 1 đầu ra terminal) . Như đã test nhiều lần thì máy tính mình đang dùng thường cho cha in trước con in sau , chính vì vậy nên đoạn đầu ghi vào đã bị tiến trình con ghi đề nên mới có cái kết quả :
"
Child Id : 8986
s 8985
"

# 3/ 
Xong , dùng pipe %-% , không nghĩ ra cách không dùng system call

# 4/
Xong , ghi chi tiết trong Note

# 5/
Tiến trình còn wait return -1 vì sau lệnh fork mình không thêm 1 lệnh fork nên tiến trình con không hề tạo ra được tiến trình cháu

# 6/
Waitpid cho phép ta thực hiện thao tác wait một cách phức tạp hơn thông qua pid để chỉ tập tiến trình con nào thực thi mà tiến trình cha cần chờ đợi và options để lựa chọn hành vi 

# 7/ 
Vì khi đóng standard output mà không mở 1 source output khác nên khi ghi thì đọc vào fd output (đang giữ của standard outpu) và tìm không thấy (do đã đóng) nên không thể ghi ra (Đóng standard output = xóa lối vào số 1 fd , mà ghi ra tức là dùng write(1,...) mà lối vào bị đóng trước đó và trả về giá trị 1)

# 8/
:v? Giải vấn đề 3 rồi