define i32 @main() {
%a = alloca i32
store i32 1, i32* %a
%1 = load i32, i32* %a
%2 = add i32 %1, 70
%3 = sub i32 %2, 12
%4 = add i32 %3, 13
store i32 %4, i32* %a
%b = alloca i32
store i32 32, i32* %b
%5 = load i32, i32* %a
%6 = sdiv i32 32, 32
%7 = sub i32 %5, %6
store i32 %7, i32* %b
%8 = load i32, i32* %a
%9 = sub i32 %8, 12
%10 = call i32 @putchar(i32 %9)
%11 = load i32, i32* %b
%12 = call i32 @putchar(i32 %11)
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

