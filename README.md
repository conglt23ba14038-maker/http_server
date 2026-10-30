
# HTTP Server bằng C++ và Winsock

HTTP server đơn giản viết bằng C++17, sử dụng Winsock2 để nhận kết nối TCP và xử lý các yêu cầu HTTP. Dự án có một server quản lý danh sách `items` trong bộ nhớ và một chương trình socket mẫu để minh họa cách gửi phản hồi HTTP.

## Tính năng

- Lắng nghe tại `http://127.0.0.1:8080` trên Windows.
- Xử lý các thao tác tạo, xem, cập nhật và xóa `items` qua HTTP.
- Đọc nội dung yêu cầu theo header `Content-Length` và trả về mã trạng thái HTTP phù hợp.
- Có chương trình `socket.exe` riêng để minh họa việc nhận một yêu cầu và trả về `Hello from Winsock!`.

## Công nghệ sử dụng

- C++17
- Winsock2 (`ws2_32`)
- MinGW-w64 `g++` và `mingw32-make`

> Dự án hiện được viết cho Windows vì mã nguồn dùng Winsock2.

## Cấu trúc dự án

```text
http sever/
├── Networking/
│   ├── socket.cpp / socket.hpp                 # Khởi tạo và quản lý socket
│   ├── bindingsocket.cpp / bindingsocket.hpp   # Gắn socket với địa chỉ và cổng
│   ├── listeningsocket.cpp / listeningsocket.hpp # Lắng nghe và chấp nhận kết nối
│   ├── connectingsocket.cpp / connectingsocket.hpp # Socket kết nối tới máy khác
│   └── main.cpp                                # Chương trình socket mẫu
├── Servers/
│   ├── server.cpp / server.hpp                 # Lớp cơ sở cho server
│   ├── testserver.cpp / testserver.hpp         # Xử lý HTTP và các route /items
│   └── test.cpp                                # Điểm bắt đầu của HTTP server
├── Makefile
└── README.md
```

## API

Nội dung `item` là **văn bản thuần**, không phải JSON. Dữ liệu chỉ nằm trong bộ nhớ và sẽ mất khi dừng server.

| Phương thức | Đường dẫn | Chức năng | Phản hồi thành công |
| --- | --- | --- | --- |
| `GET` | `/items` | Liệt kê các item dưới dạng `id: nội dung` | `200 OK` |
| `POST` | `/items` | Tạo item từ phần body; trả về ID mới | `201 Created` |
| `GET` | `/items/{id}` | Xem nội dung của một item | `200 OK` |
| `PUT` | `/items/{id}` | Thay nội dung của một item | `200 OK` |
| `DELETE` | `/items/{id}` | Xóa một item | `204 No Content` |




   
