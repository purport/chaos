#include <chaos/check.h>
#include <chaos/hash.h>
#include <scanner.h>

static u64 buffer_chunk = 10;
static u64 buffer_index;
static u64 buffer_size;
static c8 buffer[1024];

static scanner fill_buffer(c8 *b) {
  buffer_index = 0;
  buffer_size = string_length(b);
  memcpy(buffer, b, buffer_size);
  return (scanner){.it = buffer, .end = buffer + buffer_size};
}


#undef FIND_HASHES
static u32 find_hash_seed(void);

u32 chaos_main() {
  scanner s;
  CHECK_START();
  // TODO: put this into a utility exe
#ifdef FIND_HASHES
  u32 seed = find_hash_seed();
  CHECK(seed, !=, (u32)-1);
#endif

  s = fill_buffer("");
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("           ");
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("+");
  CHECK(scanner_next(&s), ==, TOKEN_PLUS);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("0 1 2 3 4 5 6 7 8 9");
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 0);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 1);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 2);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 3);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 4);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 5);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 6);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 7);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 8);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 9);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("123 543");
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 123);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 543);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("18446744073709551615");
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(s.n, ==, 18446744073709551615ull);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("18446744073709551616");
  CHECK(scanner_next(&s), ==, TOKEN_ERROR);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("a+ hello A0 0a");
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_PLUS);
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("and");
  CHECK(scanner_next(&s), ==, TOKEN_KEYWORD);
  CHECK(s.keyword, ==, KEYWORD_AND);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("return");
  CHECK(scanner_next(&s), ==, TOKEN_KEYWORD);
  CHECK(s.keyword, ==, KEYWORD_RETURN);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("returns");
  CHECK(scanner_next(&s), ==, TOKEN_IDENTIFIER);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  s = fill_buffer("*=::=.=<<=>>=0-1(){},;!!=+-*/+=-=*=/=");
  CHECK(scanner_next(&s), ==, TOKEN_MULT_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_COLON);
  CHECK(scanner_next(&s), ==, TOKEN_ASSIGN);
  CHECK(scanner_next(&s), ==, TOKEN_DOT);
  CHECK(scanner_next(&s), ==, TOKEN_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_LT);
  CHECK(scanner_next(&s), ==, TOKEN_LEQ);
  CHECK(scanner_next(&s), ==, TOKEN_GT);
  CHECK(scanner_next(&s), ==, TOKEN_GEQ);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(scanner_next(&s), ==, TOKEN_MINUS);
  CHECK(scanner_next(&s), ==, TOKEN_NUMBER);
  CHECK(scanner_next(&s), ==, TOKEN_PAREN_OPEN);
  CHECK(scanner_next(&s), ==, TOKEN_PAREN_CLOSE);
  CHECK(scanner_next(&s), ==, TOKEN_BRACE_OPEN);
  CHECK(scanner_next(&s), ==, TOKEN_BRACE_CLOSE);
  CHECK(scanner_next(&s), ==, TOKEN_COMMA);
  CHECK(scanner_next(&s), ==, TOKEN_END_OF_LINE);
  CHECK(scanner_next(&s), ==, TOKEN_NOT);
  CHECK(scanner_next(&s), ==, TOKEN_NOT_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_PLUS);
  CHECK(scanner_next(&s), ==, TOKEN_MINUS);
  CHECK(scanner_next(&s), ==, TOKEN_MULT);
  CHECK(scanner_next(&s), ==, TOKEN_DIVIDE);
  CHECK(scanner_next(&s), ==, TOKEN_PLUS_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_MINUS_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_MULT_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_DIVIDE_EQ);
  CHECK(scanner_next(&s), ==, TOKEN_EOF);
  CHECK(s.it, ==, buffer + buffer_size);
  CHECK(s.end, ==, buffer + buffer_size);

  return CHECK_END();
}


// TODO: clean up this stuff and put into utiltiy.
static u32 hash_string(u32 seed, u8 *s) {
  murmur3 hash = {.value = seed};
  for (u8 ch = *s++; ch != 0; ch = *s++) {
    // c8 buf[] = {(c8)ch, '\n', 0};
    // print(buf);
    hash_murmur3(&hash, ch);
  }
  return hash_murmur3_fin(&hash);
}

static bool is_keyword(u8 length, c8 **keywords, c8 *tst) {
  for (u8 i = 0; i != length; ++i) {
    c8 *k = keywords[i];
    bool found = 1;
    for (u8 j = 0; j != 6; ++j) {
      c8 ch1 = k[j];
      c8 ch2 = tst[j];
      if (ch1 != ch2) {
        found = 0;
        break;
      }
      if (ch1 == '\0' || ch2 == '\0') {
        break;
      }
    }
    if (found) {
      return 1;
    }
  }
  return 0;
}

static bool table_lookup(u8 length, u32 *table, u32 hash) {
  for (u8 i = 0; i != length; ++i) {
    if (table[i] == hash) return 1;
  }
  return 0;
}

static u32 find_hash_seed() {
#define SIZE 15
  c8 AND[] = "and";
  c8 CONST[] = "const";
  c8 ELSE[] = "else";
  c8 FN[] = "fn";
  c8 FOR[] = "for";
  c8 IF[] = "if";
  c8 LET[] = "let";
  c8 NAND[] = "nand";
  c8 NOR[] = "nor";
  c8 NOT[] = "not";
  c8 OR[] = "or";
  c8 RETURN[] = "return";
  c8 TYPE[] = "type";
  c8 WHILE[] = "while";
  c8 XOR[] = "xor";
  c8 *keywords[SIZE] = {AND,  NAND,  OR,  NOR, XOR,  NOT,   FN,    IF,
                        ELSE, CONST, FOR, LET, TYPE, WHILE, RETURN};
  u32 keywords_size[SIZE] = {0};
  for (u8 i = 0; i != SIZE; ++i) {
    u8 *k = (u8 *)keywords[i];
    keywords_size[i] = (u32)string_length(keywords[i]);
  }

  for (u32 seed = 1065166; seed != 0xfffffff; seed += 1) {
    u32 table[SIZE] = {0};
    u32 indices[SIZE] = {0};
    bool minimal = 1;
    for (u8 i = 0; minimal && i != SIZE; ++i) {
      u32 val = hash_string(seed, (u8 *)keywords[i]);
      table[i] = val;
      minimal = indices[val % SIZE] == 0;
      indices[val % SIZE] = 1;
    }
    if (!minimal) continue;
    print("seed "); print_u64(seed); print(" minimal\n");

    bool collision = 0;
    u32 val = 0;
    c8 tst[7] = {0};
    for (c8 c1 = 'a'; !collision && c1 != '{'; ++c1) {
      tst[0] = c1;
      for (c8 c2 = 'a'; !collision && c2 != '{'; ++c2) {
        tst[1] = c2;
        tst[2] = 0;
        if (is_keyword(SIZE, keywords, tst)) continue;
        val = hash_string(seed, (u8 *)tst);
        if (table_lookup(SIZE, table, val)) {
          collision = 1;
        }
        for (c8 c3 = 'a'; !collision && c3 != '{'; ++c3) {
          tst[2] = c3;
          tst[3] = 0;
          if (is_keyword(SIZE, keywords, tst)) continue;
          val = hash_string(seed, (u8 *)tst);
          if (table_lookup(SIZE, table, val)) {
            collision = 1;
          }
          for (c8 c4 = 'a'; !collision && c4 != '{'; ++c4) {
            tst[3] = c4;
            tst[4] = 0;
            if (is_keyword(SIZE, keywords, tst)) continue;
            val = hash_string(seed, (u8 *)tst);
            if (table_lookup(SIZE, table, val)) {
              collision = 1;
            }
            for (c8 c5 = 'a'; !collision && c5 != '{'; ++c5) {
              tst[4] = c5;
              tst[5] = 0;
              if (is_keyword(SIZE, keywords, tst)) continue;
              val = hash_string(seed, (u8 *)tst);
              if (table_lookup(SIZE, table, val)) {
                collision = 1;
              }
              for (c8 c6 = 'a'; !collision && c6 != '{'; ++c6) {
                tst[5] = c6;
                tst[6] = 0;
                if (is_keyword(SIZE, keywords, tst)) continue;
                val = hash_string(seed, (u8 *)tst);
                if (table_lookup(SIZE, table, val)) {
                  collision = 1;
                }
              }
            }
          }
        }
      }
    }
    if (!collision) {
      print("no collisions found\n");
    }
    if (!collision) {
      print("perfect hashes with seed ");
      print_u64(seed);
      print("\n");

      u64 max_size = 0;
      for (u8 i = 0; i != SIZE; ++i) {
        print("#define ");
        print("KEYWORD_");
        for (c8 *ch = keywords[i]; *ch != '\0'; ++ch) {
          c8 b[] = {*ch - 32, 0};
          print(b);
        }
        print("_HASH ");
        print_u64(table[i]);
        print("\n");

        max_size = max_u64(max_size, keywords_size[i]);
      }
      print("#define ");
      print("KEYWORD_MAX_LENGTH");
      print(" ");
      print_u64(max_size);
      print("\n");

      print("typedef enum {\n");
      for (u8 i = 0; i != SIZE; ++i) {
        print("KEYWORD_");
        for (c8 *ch = keywords[i]; *ch != '\0'; ++ch) {
          c8 b[] = {*ch - 32, 0};
          print(b);
        }
        print(" = ");
        print_u64(table[i] % SIZE);
        print(",");
        print("\n");
      }
      print("} keyword;\n");

      return seed;
    }
  }
  return (u32)-1;
}
