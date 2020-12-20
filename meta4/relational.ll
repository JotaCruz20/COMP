define i32 @main() {
%a = alloca i32
store i32 2, i32* %a
%b = alloca i32
store i32 4, i32* %b
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

