define i32 @main() {
%a = alloca i32
store i32 2, i32* %a
%d = alloca double
store double 5.4, double* %d
%b = alloca i32
%1 = load i32, i32* %a
%2 = icmp ne i32 %1, 0
%3 = xor i1 %2, true
%4 = zext i1 %3 to i32
store i32 %4, i32* %b
%5 = load i32, i32* %b
%6 = add i32 %5, 65
%7 = call i32 @putchar(i32 %6)
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

