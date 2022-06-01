#include <chaos.h>

static u32 shapley_shubik_index(u64 count, u32 *weight, u32 *permutation,
                                u32 quota, u32 pivotal_voter) {
  u32 winning_listings = 0;

  do {
    u32 total_weight = 0;
    for (u64 i = 0; i != count; ++i) {
      u32 voter = permutation[i];
      total_weight += weight[voter];
      if (total_weight >= quota) {
        if (voter == pivotal_voter) ++winning_listings;
        break;
      }
    }
  } while (next_permutation(count, permutation));

  return winning_listings;
}

extern u32 chaos_main(void) {
  u32 weights[] = {1, 1, 1, 1, 1, 1, 2, 5};
  u32 permutation[] = {0, 1, 2, 3, 4, 5, 6, 7};
  u32 winners = shapley_shubik_index(8, weights, permutation, 7, 7);

  c8 buffer[20];
  u64_to_string(winners, 20, buffer, 0);

  print(buffer);
  print("\n");

  return 0;
}
