#pragma once

#include "main.h"

struct vector3 {
  f32 a;
  f32 b;
  f32 c;
};

static inline i32 mod(i32 dividend, i32 divisor) {
  i32 r = dividend % divisor;
  return (r < 0) ? (r + divisor) : r;
}