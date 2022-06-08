#pragma once
#include <chaos.h>

typedef struct vec3f {
  float a, b, c;
} vec3f;

static vec3f vec3f_scalar_mult(vec3f v1, f32 scalar) {
  return (vec3f){.a = v1.a * scalar, .b = v1.b * scalar, .c = v1.c * scalar};
}

static vec3f vec3f_add(vec3f v1, vec3f v2) {
  return (vec3f){.a = v1.a + v2.a, .b = v1.b + v2.b, .c = v1.c + v2.c};
}
