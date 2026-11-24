# Docs Phase 3 : Concurrency
__(1 trong 3 vấn đề trụ cột trong thiết kế máy tính)__


# Locks 
- Ý tưởng cơ bản : Là 1 cơ chế đồng bộ cho phép dev quyền tối thiểu can thiệp vào lịch do os sắp xếp sẵn cho các đối tượng thực thi vào các critical section (phần quan trọng) và trở thành owner
- Vấn đề xây dựng 1 lock hiệu quả : Đảm bảo tính loại trừ lẫn nhau với chi phí thấp (và 1 số thuộc tính) . Chúng ta sẽ tập trung vào việc khai thác các phần cứng chuyên biệt
- Tiêu chí đánh giá : 
+ Tính loại trừ lẫn nhau (Mutual Exclusion) : Tối đa 1 thread tại 1 thời điểm 
+ Tính công bằng (Fairness) : Đảm bảo tính công bằng khi tranh dành lock
+ Tính hiệu năng (Performance) : Chi phí phát sinh do việc sử dụng lock (có nhiều trường hợp)

## I / Kiểm soát ngắt (Controlling Interrupts)
- Điều đầu tiên để đảm bảo Mutual Exclusion là vô hiệu hóa ngắt trong phần quan trọng . Ví dụ ở đơn nhân ta chỉ cần dis/en-able interrupts trong lock/unlock . Đơn giản nhưng nhiều nhược điểm

## II / Test - Set (Atomic Exchange)
- Một trong những thiết kế hệ thống phần cứng hỗ trợ locking đơn giản nhất là test-and-set , hay còn gọi là atomic exchange . Nó đơn giản chỉ là hỗ trợ kiểm tra giá trị cũ và thiết lập giá trị mới atomic
- Đơn giản và để hoạt động chính xác trên hệ thống đơn xử lý ta cần preemptive scheduler để cưỡng chế thu hồi CPU từ 1 thread thông qua time interrupts bởi 1 thread trên spin sẽ không nhường CPU

## III / Compare - Swap (Exchange)
- Trả về giá trị thực tế tại vị trí bộ nhớ và cập nhật giá trị nếu đoán đúng giá trị tại vị trí đó
- Ở mức độ đơn giản thì cả Test - Set và Compare - Swap khá giống nhau nhưng thật ra Compare - Swap sẽ mạnh hơn (vì có so sánh điều kiện) và được dùng trong wait-free synchronization

## IV / Load-Linked & Store-Conditional
- Load-Linked đơn giản là lấy giá trị tại vị trí (ptr) vào thanh ghi . Mấu chốt ở Store-Conditional chỉ thành công khi không có lệnh store nào ghi vào địa chỉ ptr trong thời gian giữa 2 thao tác

## V / Fetch - Add 
- Tăng giá trị địa chỉ lên 1 theo 1 cách atomic và trả về giá trị cũ dùng cặp giá trị (ticket ; turn)