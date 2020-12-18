@a = global i32 2
@b = global i32 3
@d = global double 2.0
define i32 @main() {
%1 = load double, double* @d
%2 = load i32, i32* @b
%3 = load i32, i32* @b
%4 = sitofp i32 %3 to double
%5 = fadd double %1, %4
%6 = load i32, i32* @a
%7 = load i32, i32* @a
%8 = sitofp i32 %7 to double
%9 = fadd double %5, %8
store double %9, double* @d
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

