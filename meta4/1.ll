define i32 @main() {
%d = alloca i32
%1 = call i32 @getchar()
store i32 %1, i32* %d
%2 = load i32, i32* %d
%3 = call i32 @putchar(i32 %2)
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

