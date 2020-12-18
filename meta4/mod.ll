define i32 @main() {
%a = alloca i32
store i32 3, i32* %a
%b = alloca i32
%1 = load i32, i32* %a
%2 = srem i32 %1, 2
store i32 %2, i32* %b
%3 = load i32, i32* %b
%4 = add i32 %3, 80
%5 = sub i32 %4, 12
%6 = call i32 @putchar(i32 %5)
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

