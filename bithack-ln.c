#include "stdio.h";
#include "stdlib.h";
#include "math.h";

#define FTOL(d)     ({ double _d = d; *(unsigned long*)&_d; })
#define LTOF(l)     ({ unsigned long _l = l; *(double*)&_l; })
#define INSPECT(f)  printf("%10s: 0x%016lx = %g\n", #f, *(unsigned long*)&f, f)

double exp2048(double x) { //return x ^ 1024 / (2 ^ 511);
  INSPECT(x);
  int i;
  x = x * x;
  for(i = 0; i < 9; i++) {
  INSPECT(x);
    x = x * x / 2; 
  }
  return x;
}

double lg2(double d) {
  INSPECT(d);
  // k = lg2(d) ...
  // 2^(k=i+r) = d
  // i = lg2(d) to the nearest int (doable with float hack)
  // r = lg2(d / 2^i)   ... d/2^i is between 1 and 2
  // lg2(1.01^2048) = 2048*lg2(1.01), used to lg2 stuff between 1 and 2
  
  // first batch of 11 bits
  double i = (double)(long)((FTOL(d) >> 52) - 1023);
  INSPECT(i);

  // 10 bits of decimal
  double r_up_2048 = exp2048(LTOF(FTOL(d) & 0x000FFFFFFFFFFFFFL | 0x3FF0000000000000L)); // (d / 2^k_i) ^ 2048 / (2 ^ 1023)
  INSPECT(r_up_2048);
  double r = (double)((FTOL(r_up_2048) >> 52) - 512) / 1024;
  INSPECT(r);
  
  // 10 more bits of decimal
  double r2_up_2048 = exp2048(LTOF(FTOL(r_up_2048) & 0x000FFFFFFFFFFFFFL | 0x3FF0000000000000L));
  INSPECT(r2_up_2048);
  double r2 = (double)((FTOL(r2_up_2048) >> 52) - 512) / 1024 / 1024;
  INSPECT(r2);
  return i + r + r2;  // precision is 11.20 bits for a total of 33 possible bits of precision
}

double pow2(double d) {
  // inverse of the above.  left as an exercise to the reader :)
  return pow(2, d); 
}

int main(int argc, char** argv) {
  if(argc <= 1) {
    printf("Usage: %s <d> <n> <m>  -- prints d^(n/m) using only bithacks", argv[0]);
    exit(1);
  }
  double d = atof(argv[1]);
  double n = argc > 2 ? atof(argv[2]) : 1;
  double m = argc > 3 ? atof(argv[3]) : 1;
 
  // x = d^(n/m)
  // lg(x) = lg(d) * (n/m);
  // x = 2 ^ (lg2(d) * n/m)

  double k = lg2(d) * n / m;
  double result = pow2(k);
  printf("Hello World: %f^(%f/%f) = 2^%f = %f\n", d, n, m, k, result);
}
