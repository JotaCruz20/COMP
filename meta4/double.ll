define i32 @main() {
%a = alloca i32
store i32 1, i32* %a
%d = alloca double
%1 = sext i32 %0 to double
%3 = load double, double* %a
store double %3, double* %d
ret i32 0
}

declare i32 @putchar(i32)
declare i32 @getchar()


define i32 @main() {
%a = alloca i32
store i32 1, i32* %a
%d = alloca double
%1 = load i32, i32* %a
%2 = sitofp i32 %1 to double
store double %2, double* %d
ret i32 0
}