bithack-ln
==========

Implementing log2 using only integer operations and bithacks


== Overview

==== Goal

Our goal is to implement the following function

    double lg2(double d) {
      // lg2(2) = 1
      // lg2(4) = 2
      // lg2(8) = 3
      
      // Operations Allowed:
      // * Basic CPU-supported single-instruction Integer/FP Ops (+, -, *, /)
      // * Bithacks such as casting between Integer / FP types
    }

==== The 10-bit log2 Primative

It's important to first understand the [Double-precision floating-point format](http://en.wikipedia.org/wiki/Double-precision_floating-point_format) - 1 SIGN bit, 11 EXPONENT bits, and 52 SIGNIFICAND bits.

<img src="http://upload.wikimedia.org/wikipedia/commons/a/a9/IEEE_754_Double_Floating_Point_Format.svg" />

Thus (where SIGN, SIGNIFICAND, and EXPONENT are the unsigned integer interpretations of their bits),

    Value = (-1)^SIGN * (1 + SIGNIFICAND/(2^52)) * 2^(EXPONENT - 1023)

    [0,0,0] = 1^-1023 // smallest positive number
    [1,0,0] = -1*1^-1023 // smallest negative number
    [0, 4503599627370495, 2047] = (~2)^1024


Getting the bits from a double:
    
    #define FTOL(d)     ({ double _d = d; *(unsigned long*)&_d; })
   
More verbosely, it would look like this

    unsigned long double2long(double value) {
      double *p_value = &value;
      unsigned long *p_valuebits = (unsigned long*)p_value; // this is a reinterpret_cast
      unsigned long valuebits = *p_valuebits;
      return valuebits;
    }

Getting the EXPONENT from a double

    lg2_10bits = (int)(valuebits >> 52) - 1023;
    return valuebits;

    #define LTOF(l)     ({ unsigned long _l = l; *(double*)&_l; })

