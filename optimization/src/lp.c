#include <chaos.h>

static f64 *tableau_pivot_column(u8 stride, u8 constraints, f64 *tableau) {
  u8 objective_row = stride * constraints;
  f64 most_negative = 0.0;
  f64 *pivot_column = 0;
  for (u8 i = 0; i != stride; ++i, ++tableau) {
    f64 coeff = tableau[objective_row];
    if (coeff < most_negative) {
      most_negative = coeff;
      pivot_column = tableau;
    }
  }
  return pivot_column;
}

static u8 tableau_pivot_row(u8 stride, u8 constraints, f64 *pivot_column,
                            f64 *constraint_column) {
  u8 pivot = constraints;
  f64 least_positive = 9999999.0;

  for (u8 i = 0; i != constraints; ++i) {
    if (*pivot_column > 0) {
      f64 ratio = *constraint_column / *pivot_column;
      if (0 < ratio && ratio < least_positive) {
        least_positive = ratio;
        pivot = i;
      }
    }
    pivot_column += stride;
    constraint_column += stride;
  }
  return pivot;
}

static void elementary_row_multiply(u8 stride, f64 *tableau, u8 row, f64 m) {
  f64 *it = tableau + row * stride;
  for (u8 i = 0; i != stride; ++i) {
    *it *= m;
    ++it;
  }
}

static void elementary_row_multiply_add(u8 stride, f64 *tableau, u8 row1, f64 m,
                                        u8 row2) {
  f64 *it1 = tableau + row1 * stride;
  f64 *it2 = tableau + row2 * stride;
  for (u8 i = 0; i != stride; ++i) {
    *it1 += m * (*it2);
    ++it1;
    ++it2;
  }
}

static void tableau_print(u8 stride, u8 constraints, f64 *tableau) {
  for (u8 i = 0; i != constraints+1; ++i) {
    for (u8 j = 0; j != stride; ++j) {
      print_f64(tableau[i*stride + j]);
      print(" ");
    }
    print("\n");
  }
}

static bool linear_programme_solve(u8 stride, u8 constraints, f64 *tableau) {
  while (1) {
    f64 *pivot_column = tableau_pivot_column(stride, constraints, tableau);
    if (pivot_column == 0) {
      print("\n");
      return 1;
    }
    u8 pivot = tableau_pivot_row(stride, constraints, pivot_column,
                                 tableau + stride - 1);

// clang-format off
    print("r");print_u64(pivot+1);print(" -> ");print_f64(1.0 / pivot_column[pivot * stride]);print("*r");print_u64(pivot+1);print("\n");
// clang-format on

    elementary_row_multiply(stride, tableau, pivot,
                            1.0 / pivot_column[pivot * stride]);

    for (u8 i = 0; i != constraints + 1; ++i) {
      if (i == pivot) continue;
// clang-format off
    print("r");print_u64(i+1);print(" -> r");print_u64(i+1);print(" + ");print_f64(-pivot_column[i * stride]);print("*r");print_u64(pivot+1);print("\n");
// clang-format on
      elementary_row_multiply_add(stride, tableau, i, -pivot_column[i * stride],
                                  pivot);
    }
    print("\n");
    tableau_print(stride, constraints, tableau);
    print("\n");
  }
}

// clang-format off
static f64 tableau[] = {
        2.0,  4.0, -3.0, 1.0, 0.0, 0.0,  80.0,
        3.0,  1.0, -2.0, 0.0, 1.0, 0.0, 165.0,
       -1.0, -1.0,  2.0, 0.0, 0.0, 1.0, 105.0,
       -6.0, -2.0,  2.0, 0.0, 0.0, 0.0,   0.0,
};
// clang-format on

u32 chaos_main() {

  tableau_print(6+1, 3, tableau);
  print("\n");
  linear_programme_solve(6+1, 3, tableau);

  return 0;
}
