define i32 @main() {
%a = alloca i32
store i32 2, i32* %a
%1 = load i32, i32* %a
%2 = sdiv i32 68, %1
store i32 %2, i32* %a
%3 = load i32, i32* %a
%4 = call i32 @putchar(i32 %3)
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

