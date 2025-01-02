#define STB_IMAGE_WRITE_IMPLEMENTATION
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define ARENA_IMPLEMENTATION

#include "arena.h"
#include "nob.h"
#include "stb_image_write.h"
#include <stdint.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

static Arena node_arena = {0};

typedef enum { NK_X, NK_Y, NK_NUMBER, NK_ADD, NK_MULT, NK_TRIPLE } Node_Kind;

typedef struct Node Node;

typedef struct {
  Node *lhs;
  Node *rhs;
} Node_Binop;

typedef struct {
  Node *first;
  Node *second;
  Node *third;
} Node_Triple;

typedef union {
  float number;
  Node_Binop binop; // binary operaton;
  Node_Triple triple;
} Node_As;

struct Node {
  Node_Kind kind;
  const char *file;
  int line;
  Node_As as;
};

Node *node_loc(const char *file, int line, Node_Kind kind) {
  Node *node = arena_alloc(&node_arena, sizeof(Node));
  node->kind = kind;
  node->file = file;
  node->line = line;
  return node;
}

Node *node_number_loc(const char *file, int line, float number) {
  Node *node = node_loc(file, line, NK_NUMBER);
  node->as.number = number;
  return node;
}

#define node_number(number) node_number_loc(__FILE__, __LINE__, number)

#define node_x() node_loc(__FILE__, __LINE__, NK_X)

// Node *node_x_loc(const char *file, int line) {
//   return node_loc(file, line, NK_X);
// }

#define node_y() node_loc(__FILE__, __LINE__, NK_Y)

// Node *node_y(void) {
//   Node *node = arena_alloc(&node_arena, sizeof(Node));
//   node->kind = NK_Y;
//   return node;
// }

Node *node_add_loc(const char *file, int line, Node *lhs, Node *rhs) {
  Node *node = node_loc(file, line, NK_ADD);
  node->as.binop.lhs = lhs;
  node->as.binop.rhs = rhs;
  return node;
}

#define node_add(lhs, rhs) node_add_loc(__FILE__, __LINE__, lhs, rhs)

Node *node_mult_loc(const char *file, int line, Node *lhs, Node *rhs) {
  Node *node = node_loc(file, line, NK_MULT);
  node->as.binop.lhs = lhs;
  node->as.binop.rhs = rhs;
  return node;
}

#define node_mult(lhs, rhs) node_mult_loc(__FILE__, __LINE__, lhs, rhs)

Node *node_triple_loc(const char *file, int line, Node *first, Node *second,
                      Node *third) {
  Node *node = node_loc(file, line, NK_TRIPLE);
  node->as.triple.first = first;
  node->as.triple.second = second;
  node->as.triple.third = third;
  return node;
}

#define node_triple(first, second, third)                                      \
  node_triple_loc(__FILE__, __LINE__, first, second, third)

void node_print(Node *node) {
  switch (node->kind) {
  case NK_X:
    printf("x");
    break;
  case NK_Y:
    printf("y");
    break;
  case NK_NUMBER:
    printf("%f", node->as.number);
    break;
  case NK_ADD:
    printf("(");
    node_print(node->as.binop.lhs);
    printf(", ");
    node_print(node->as.binop.rhs);
    printf(")");
    break;
  case NK_MULT:
    printf("mult(");
    node_print(node->as.binop.lhs);
    printf(", ");
    node_print(node->as.binop.rhs);
    printf(")");
  case NK_TRIPLE:
    printf("(");
    node_print(node->as.triple.first);
    printf(", ");
    node_print(node->as.triple.second);
    printf(", ");
    node_print(node->as.triple.third);
    printf(")");
    break;
  }
}

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} RGBA32;

static RGBA32 pixels[WIDTH * HEIGHT];

typedef struct {
  float X, Y;
} Vector2;

typedef struct {
  float r, g, b;
} Color;

Color grey_gradient(float x, float y) {
  (void)y;
  return (Color){x, x, x};
}

Color cool(float x, float y) {
  if (x * y > 0) {
    return (Color){x, y, 1};
  } else {
    float r = fmodf(x + 1e-3, y + 1e-3); // adding 0.001 to avoid division by
                                         // zero
    return (Color){r, r, r};
  }
}

Color justTry(float x, float y) {
  if (x * y > 10) {
    return (Color){x, y, 26};
  } else {
    float r = fmodf(x + 1e-3, y + 1e-3);
    return (Color){r + 1, r, r + 7};
  }
}

bool expect_number(Node *expr) {
  if (expr->kind != NK_NUMBER) {
    printf("%s:%d: ERROR: expected number", expr->file, expr->line);
    return false;
  };
  return true;
}

bool experct_triple(Node *expr) {
  if (expr->kind != NK_TRIPLE) {
    printf("%s:%d: ERROR: expected triple", expr->file, expr->line);
    return false;
  }
  return true;
}

Node *eval(Node *expr, float x, float y) {
  switch (expr->kind) {
  case NK_X:
    return node_number_loc(expr->file, expr->line, x);
    break;
  case NK_Y:
    return node_number_loc(expr->file, expr->line, y);
    break;
  case NK_NUMBER:
    return expr;
    break;
  case NK_ADD:
    Node *lhs = eval(expr->as.binop.lhs, x, y);
    if (!expect_number(lhs)) {
      return NULL;
    }
    Node *rhs = eval(expr->as.binop.rhs, x, y);
    if (!expect_number(rhs)) {
      return NULL;
    }
    return node_number_loc(expr->file, expr->line,
                           lhs->as.number + rhs->as.number);
    break;
  case NK_MULT: {
    Node *lhs = eval(expr->as.binop.lhs, x, y);

    if (!expect_number(lhs)) {
      return NULL;
    }
    Node *rhs = eval(expr->as.binop.rhs, x, y);
    if (!expect_number(rhs)) {
      return NULL;
    }
    return node_number_loc(expr->file, expr->line,
                           lhs->as.number * rhs->as.number);
    break;
  }
  case NK_TRIPLE:
    Node *first = eval(expr->as.triple.first, x, y);
    Node *second = eval(expr->as.triple.second, x, y);
    Node *third = eval(expr->as.triple.third, x, y);
    return node_triple_loc(expr->file, expr->line, first, second, third);
    break;
  default:
    return NULL;
  }
}

bool *eval_func(Node *body, float x, float y, Color *c) {
  Node *result = eval(body, x, y);
  if (result == NULL) {
    return (void *)false;
  }
  if (!experct_triple(result)) {
    return (void *)false;
  }
  if (!expect_number(result->as.triple.first)) {
    return (void *)false;
  }
  if (!expect_number(result->as.triple.second)) {
    return (void *)false;
  }
  if (!expect_number(result->as.triple.third)) {
    return (void *)false;
  }
  c->r = result->as.triple.first->as.number;
  c->g = result->as.triple.second->as.number;
  c->b = result->as.triple.third->as.number;
  return (void *)true;
}

void render_pixels(Color (*f)(float x, float y)) {
  // inside thew for loop we have to normalize the HEIGHT and WIDTH between -1
  // to 1 but we have current range 0 to Height and 0 to Width;
  for (int y = 0; y < HEIGHT; y++) {
    // 0..<HEIGHT -> 0..<1 -> 0..<2 -> -1..<1
    float ny = (float)y / HEIGHT * 2.0f - 1.0f;
    for (int x = 0; x < WIDTH; x++) {
      // 0..<WIDTH -> 0..<1 -> 0..<2 -> -1..<1
      float nx = (float)x / WIDTH * 2.0f - 1.0f;
      Color c = f(nx, ny);
      // Color c = eval_func(f, nx, ny);
      // -1 to 1 -> +1
      // 0 to 2 -> /2
      // 0 to 1 -> *255
      // 0 to 255; done;
      size_t index = y * WIDTH + x;
      pixels[index].r = (c.r + 1.0f) / 2.0f * 255;
      pixels[index].g = (c.g + 1.0f) / 2.0f * 255;
      pixels[index].b = (c.b + 1.0f) / 2.0f * 255;
      pixels[index].a = 255;
    }
  }
}

int main(void) {
  printf("\033[1;32m\n------------code Execution starts "
         "here------------\n\033[0m");
  // Node *node = node_triple(node_x(), node_y(), node_number(0.5));
  Node *node =
      node_triple(node_add(node_x(), node_y()), node_y(), node_number(0.5));
  node_print(node);
  printf("\n");
  exit(69);
  // Node *node = arena_alloc(&node_arena, sizeof(Node));
  // render_pixels(grey_gradient);
  render_pixels(cool);
  // render_pixels(justTry);
  const char *output_path = "output.png";
  if (!stbi_write_png(output_path, WIDTH, HEIGHT, 4, pixels,
                      WIDTH * sizeof(RGBA32))) {
    printf("Could not save Image: %s", output_path);
    nob_log(ERROR, "Could not save Image: %s", output_path);
    return 1;
  };
  nob_log(INFO, "Image saved to: %s", output_path);
  printf("Success\n");
  printf("\033[1;34m\n------------code Execution ends "
         "here------------\n\033[0m");
  return 0;
}
