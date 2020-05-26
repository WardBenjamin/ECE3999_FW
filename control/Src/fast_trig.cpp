#include <stm32f3xx_hal.h>  // Sets up the correct chip specifc defines required by arm_math
#define ARM_MATH_CM4 // TODO: might change in future board versions

#include "fast_trig.hpp"

f32 fast_sin(
    f32 x) {
  f32 sinVal, fract, in;                           /* Temporary variables for input, output */
  uint16_t index;                                        /* Index variable */
  f32 a, b;                                        /* Two nearest output values */
  int32_t n;
  f32 findex;

  /* input x is in radians */
  /* Scale the input to [0 1] range from [0 2*PI] , divide input by 2*pi */
  in = x * 0.159154943092f;

  /* Calculation of floor value of input */
  n = (int32_t) in;

  /* Make negative values towards -infinity */
  if (x < 0.0f) {
    n--;
  }

  /* Map input value to [0 1] */
  in = in - (f32) n;

  /* Calculation of index of the table */
  findex = (f32) FAST_MATH_TABLE_SIZE * in;
  index = (uint16_t) findex;

  /* when "in" is exactly 1, we need to rotate the index down to 0 */
  if (index >= FAST_MATH_TABLE_SIZE) {
    index = 0;
    findex -= (f32) FAST_MATH_TABLE_SIZE;
  }

  /* fractional value calculation */
  fract = findex - (f32) index;

  /* Read two nearest values of input value from the sin table */
  a = sinTable_f32[index];
  b = sinTable_f32[index + 1];

  /* Linear interpolation process */
  sinVal = (1.0f - fract) * a + fract * b;

  /* Return the output value */
  return (sinVal);
}

f32 fast_cos(
    f32 x) {
  f32 cosVal, fract, in;                   /* Temporary variables for input, output */
  uint16_t index;                                /* Index variable */
  f32 a, b;                                /* Two nearest output values */
  int32_t n;
  f32 findex;

  /* input x is in radians */
  /* Scale the input to [0 1] range from [0 2*PI] , divide input by 2*pi, add 0.25 (pi/2) to read sine table */
  in = x * 0.159154943092f + 0.25f;

  /* Calculation of floor value of input */
  n = (int32_t) in;

  /* Make negative values towards -infinity */
  if (in < 0.0f) {
    n--;
  }

  /* Map input value to [0 1] */
  in = in - (f32) n;

  /* Calculation of index of the table */
  findex = (f32) FAST_MATH_TABLE_SIZE * in;
  index = (uint16_t) findex;

  /* when "in" is exactly 1, we need to rotate the index down to 0 */
  if (index >= FAST_MATH_TABLE_SIZE) {
    index = 0;
    findex -= (f32) FAST_MATH_TABLE_SIZE;
  }

  /* fractional value calculation */
  fract = findex - (f32) index;

  /* Read two nearest values of input value from the cos table */
  a = sinTable_f32[index];
  b = sinTable_f32[index + 1];

  /* Linear interpolation process */
  cosVal = (1.0f - fract) * a + fract * b;

  /* Return the output value */
  return (cosVal);
}

/**
 * @} end of cos group
 */