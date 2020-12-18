int A0, A1, A2, A3, A4;
short B1, B2, B3, B4;
char C0, C1, C2, C3, C4;
double D1, D2, D3, D4;

void d(void);

int main(void) {
    int a, b, c;
    if(D1+A1) {b=1; c=2;} else {c=1;}
    if(B1) {b=1; c=2;} else {c=1;}
    if(main) {b=1; c=2;} else {c=1;}
    
    if(B1+1.0) {A1+A2;} else {c=1; b=2;}
    if(5) {b=1; c=2;} else {C1-C2;}
    if (d)
    {
    }
    else
    {
        C1 - C2;
    }
    while(D1+A1) {b=1; c=2;}
    while(B1) {b=1; c=2;}
    while(main) {b=1; c=2;}

    while(B1+1.0) {A1+A2;}
    while(5) {b=1; c=2;}

    while(d){}
}