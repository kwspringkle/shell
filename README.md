# Tinyshell

### Compile:
```sh
gcc -o tinyshell main.c path.c process.c shell.c
```

### Run:
```sh
.\tinyshell.exe
```

Tiny shell có 25 lệnh cơ bản.

## I. Các lệnh cơ bản
1. **date**: In ra ngày tháng năm hiện tại
2. **time**: In ra giờ hiện tại

## II. Các lệnh liên quan đến folder
3. **pwd**: Folder đang ở hiện tại
4. **mkdir**: Tạo folder mới
5. **rmdir**: Xóa folder
6. **ls**: Liệt kê folder
7. **cd**: Chuyển sang folder khác

## III. Các lệnh liên quan đến file
8. **create**: Tạo file mới
9. **read**: Đọc nội dung file
10. **write**: Ghi nội dung file
11. **append**: Thêm nội dung vào file
12. **remove**: Xóa file

## IV. Các lệnh tiến trình
13. **bg**: Chạy background mode
14. **fg**: Chạy foreground mode
15. **list**: Liệt kê các tiến trình
16. **stop**: Suspend tiến trình
17. **resume**: Tiếp tục tiến trình
18. **kill**: Hủy bỏ tiến trình

## V. Các lệnh liên quan đến biến môi trường (Không dùng biến môi trường của HDH)
19. **showpath**: In ra các biến môi trường, mặc định ban đầu là D:\Shell
20. **addpath**: Thêm biến môi trường, khi thêm biến môi trường thì có thể chạy trực tiếp mà không cần cd tới folder
21. **removepath**: Xóa biến môi trường
22. **updatepath**: Sửa biến môi trường

## VI. Các lệnh khác
23. **bat**: Chạy bat file
24. **help**: In ra tất cả các lệnh
25. **exit**: Thoát khỏi shell
