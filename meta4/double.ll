@a = global i32 1
define i32 @main() {
%d = alloca double
%1 = load i32, i32* @a
%2 = sitofp i32 %1 to double
store double %2, double* %d
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()

