# Kết quả và giải thích 

Lệnh = Chương trình xử dụng (Ghi ngắn gọn là lệnh)

## 1/
CPU chiếm 100% thời gian sử dụng bởi vì không có lệnh I/O tham gia 

## 2/
11 tick vì 4 lệnh CPU tương đương 4 tick + 1 lệnh I/O với không flag L thì thời gian xử lý là 7 tick
(Nhưng lưu ý là CPU chiếm 6 tick với 2 giai đoạn đầu và kết thúc của IO)

## 3/
7 tick vì khi xong tick 1 thì lệnh I/O được gác sang 1 bên và 4 lệnh CPU chạy xong thì tick 6 mới xong I/O và tick 7 thì CPU mới nhận và báo ổn

## 4/
Câu trả lời là không , khác với câu 3 thì CPU sẽ nhàn rỗi (idle) vì hệ thống
không đổi qua 4 tiến trình chỉ cần chạy CPU kia mà đợi CPU báo xong I/O mới chạy các tiến trình kia , chi phí giống câu 2

## 5/ 
Cơ bản là đưa về lại multiprogram thì kết quả giống câu 3

## 6/
Không tối ưu tài nguyên , việc I/O bị trì hoãn vì OS không chiếm quyền tiến trình CPU để hoàn thành việc I/O 1 là chưa tận dụng được I/O (idle) do chưa tối ưu thứ tự chạy CPU . 

## 7/ 
Tối ưu hơn hẵn vì việc ưu tiên xử lý sau khi xong I/O sẽ cho phép đưa lệnh I/O tiếp theo sớm vào IOs hơn tận dụng tối đa thời gian IOs và không cho CPU nhàn rỗi

## 8/
1 tiến trình có thể chứa nhiều lệnh trong đó có cả CPU thuần và I/O
tùy vào tỉ lệ cung cấp . Tối ưu nhất luôn là -S SWITCH_ON_IO và -I IO_RUN_IMMEDIATE để không cho cả IOs và CPU nhàn rỗi