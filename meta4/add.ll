define i32 @main() {
%e = alloca i32
store i32 1, i32* %e
%a = alloca i32
%1 = load i32, i32* %e
store i32 %1, i32* %a
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

