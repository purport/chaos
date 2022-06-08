#include <chaos/check.h>
#include <chaos/vector.h>

u32 chaos_main() {
  CHECK_START();
  vec3f v1 = {1.0f, 2.0f, 3.0f};
  vec3f v2 = {5.0f, 1.0f, 1.0f};
  CHECK(vec3f_scalar_mult(v1, 2.0f).a, ==, 2.0f);
  CHECK(vec3f_scalar_mult(v1, 2.0f).b, ==, 4.0f);
  CHECK(vec3f_scalar_mult(v1, 2.0f).c, ==, 6.0f);
  CHECK(vec3f_add(v1, v2).a, ==, 6.0f);
  CHECK(vec3f_add(v1, v2).b, ==, 3.0f);
  CHECK(vec3f_add(v1, v2).c, ==, 4.0f);
  CHECK(vec3f_add(v2, v1).a, ==, 6.0f);
  CHECK(vec3f_add(v2, v1).b, ==, 3.0f);
  CHECK(vec3f_add(v2, v1).c, ==, 4.0f);


  return CHECK_END();
}
