define i32 @main() {
%a = alloca i32
store i32 98, i32* %a
%b = alloca i32
%1 = load i32, i32* %a
store i32 %1, i32* %b
%2 = load i32, i32* %b
%3 = call i32 @putchar(i32 %2)
%c = alloca i32
store i32 67, i32* %c
%4 = load i32, i32* %c
%5 = call i32 @putchar(i32 %4)
%6 = call i32 @putchar(i32 33)
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

