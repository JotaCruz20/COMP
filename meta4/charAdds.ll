define i32 @main() {
%a = alloca i32
store i32 0, i32* %a
%1 = mul i32 027, 01365
%2 = add i32 %1, 65
%3 = call i32 @putchar(i32 %2)
ret i32 0
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

