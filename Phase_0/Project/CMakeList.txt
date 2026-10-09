cmake_minimum_required(VERSION 3.16)

# Tên dự án
project(CpuThreadExample CXX)

# Thiết lập chuẩn C ++ 11 (bắt buộc cho std::thread)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Tải gói Threads (tự động tìm -pthread trên Linux/macOS hoặc cấu hình phù hợp trên Windows)
find_package(Threads REQUIRED)

# Tạo file thực thi tên là 'cpu' từ file nguồn cpp
add_executable(cpu TestMultiThread.cpp)

# Liên kết thư viện Threads vào file thực thi
target_link_libraries(cpu PRIVATE Threads::Threads)