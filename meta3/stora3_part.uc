void f(void);
void g0(void);
char g1(void);
double g2(void);

void g0(void) {
    double a0;
    a0 = g2();
    return f();
}
char g1(void) {
    char a;
    a = g1();
    return a+1;
}
double g2(void) {
    char a;
    a = 0;
    return (0,a);
}

void f(void){
    d=2;
}