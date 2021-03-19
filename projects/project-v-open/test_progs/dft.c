///////////////////////////////////////////////
// dft.c
// discrete fourier transform
// kindly contributed by group 4 Winter 2019
//   - Jielun Tan, 12/2019
//////////////////////////////////////////////

#include <stdio.h>
#include "tj_malloc.h"

#define SHORT_TYPE_SIZE 32
typedef short byte;

const byte n = 8;
const byte t = 1;
const byte m = 17;
const byte w = 2;
const byte w_r = 9;
const byte n_r = 15;

byte modulo_m(byte x) {
  while (x < 0) {
    x += m;
  }
  while (x >= m) {
    x -= m;
  }
  return x;
}

void dft(byte *a, byte len, byte root) {
  byte i;
  if (root == 1) {
    return;
  }

  byte x[n], y[n];
  for (i = 0; i < len; i += 2) {
    x[i >> 1] = a[i];
    y[i >> 1] = a[i+1];
  }

  dft(x, len >> 1, modulo_m(root * root));
  dft(y, len >> 1, modulo_m(root * root));
  byte base = 1;

  for (i = 0; i < (len >> 1); ++ i) {
    a[i] = modulo_m(x[i] + base * y[i]);
    a[i + (len >> 1)] = modulo_m(x[i] - base * y[i]);
    base = modulo_m(base * root);
  }

  return;
}

void to_poly(byte x, byte *a) {
  byte i;
  for (i = 0; i < n; ++ i) {
    if (x & 1) {
      a[i] = 1;
    }
    else {
      a[i] = 0;
    }
    x = x >> 1;
  }
}

byte mult(byte x, byte y) {
  byte a[n], b[n];
  byte i;
  to_poly(x, a);
  to_poly(y, b);
  dft(a, n, w);
  dft(b, n, w);

  for (i = 0; i < n; ++ i) {
    a[i] = modulo_m(a[i] * b[i]);
  }

  dft(a, n, w_r);
  byte base = 1;
  byte result = 0;
  for (i = 0; i < n; ++ i) {
    a[i] = modulo_m(a[i] * n_r);
    result = modulo_m(result + base * a[i]);
    base = modulo_m(base * w);
  }

  return result;
}

int main() {
  int a = mult(3, 16);
  int b = mult(4, 23);
  int c = mult(a, b);
  return c;
}
