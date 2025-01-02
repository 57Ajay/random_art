#define STB_IMAGE_WRITE_IMPLEMENTATION
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define ARENA_IMPLEMENTATION

#include "arena.h"
#include "nob.h"
#include "stb_image_write.h"
#include <stdint.h>
#include <stdio.h>

#define WIDTH 1440
#define HEIGHT 1080

static Arena node_arena = {0};

typedef enum {
  NK_X,
  NK_Y,
  NK_NUMBER,
  NK_ADD,
  NK_MULT,
  NK_TRIPLE,
  NK_BOOL,
  NK_GT,
  NK_IF,
  NK_MOD,
} Node_Kind;

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

typedef struct {
  Node *cond;
  Node *then;
  Node *elze;
} node_if;

typedef union {
  float number;
  Node_Binop binop; // binary operaton;
  Node_Triple triple;
  bool boolean;
  node_if iff;
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

Node *node_if_loc(const char *file, int line, Node *cond, Node *then,
                  Node *elze) {
  Node *node = node_loc(file, line, NK_IF);
  node->as.iff.cond = cond;
  node->as.iff.then = then;
  node->as.iff.elze = elze;
  return node;
}

#define node_if(cond, then, elze)                                              \
  node_if_loc(__FILE__, __LINE__, cond, then, elze)

Node *node_gt_loc(const char *file, int line, Node *lhs, Node *rhs) {
  Node *node = node_loc(file, line, NK_GT);
  node->as.binop.lhs = lhs;
  node->as.binop.rhs = rhs;
  return node;
}

#define node_gt(lhs, rhs) node_gt_loc(__FILE__, __LINE__, lhs, rhs)

Node *node_mod_loc(const char *file, int line, Node *lhs, Node *rhs) {
  Node *node = node_loc(file, line, NK_MOD);
  node->as.binop.lhs = lhs;
  node->as.binop.rhs = rhs;
  return node;
}

#define node_mod(lhs, rhs) node_mod_loc(__FILE__, __LINE__, lhs, rhs)

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
  case NK_MOD:
    printf("mod(");
    node_print(node->as.binop.lhs);
    printf(", ");
    node_print(node->as.binop.rhs);
    printf(")");
    break;

  case NK_BOOL:
    printf("%s", node->as.boolean ? "true" : "false");
    break;
  case NK_GT:
    printf("gt(");
    node_print(node->as.binop.lhs);
    printf(", ");
    node_print(node->as.binop.rhs);
    printf(")");
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
    break;
  case NK_TRIPLE:
    printf("(");
    node_print(node->as.triple.first);
    printf(", ");
    node_print(node->as.triple.second);
    printf(", ");
    node_print(node->as.triple.third);
    printf(")");
    break;
  case NK_IF:
    printf("if ");
    node_print(node->as.iff.cond);
    printf(" then ");
    node_print(node->as.iff.then);
    printf(" else ");
    node_print(node->as.iff.elze);
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
    printf("%s:%d: ERROR: expected number\n", expr->file, expr->line);
    return false;
  };
  return true;
}

bool expect_triple(Node *expr) {
  if (expr->kind != NK_TRIPLE) {
    printf("%s:%d: ERROR: expected triple\n", expr->file, expr->line);
    return false;
  }
  return true;
}

bool expect_boolean(Node *expr) {
  if (expr->kind != NK_BOOL) {
    printf("%s:%d: ERROR: expected boolean\n", expr->file, expr->line);
    return false;
  }
  return true;
}

Node *node_boolean_loc(const char *file, int line, bool boolean) {
  Node *node = node_loc(file, line, NK_BOOL);
  node->as.boolean = boolean;
  return node;
}

Node *eval(Node *expr, float x, float y) {
  switch (expr->kind) {
  case NK_X: {
    return node_number_loc(expr->file, expr->line, x);
    break;
  }
  case NK_Y: {
    return node_number_loc(expr->file, expr->line, y);
    break;
  }
  case NK_NUMBER: {
    return expr;
    break;
  }
  case NK_BOOL: {
    return expr;
    break;
  }
  case NK_GT: {
    Node *lhs = eval(expr->as.binop.lhs, x, y);
    if (!lhs)
      return NULL;
    if (!expect_number(lhs)) {
      return NULL;
    }
    Node *rhs = eval(expr->as.binop.rhs, x, y);
    if (!rhs)
      return NULL;
    if (!expect_number(rhs))
      return NULL;
    return node_boolean_loc(expr->file, expr->line,
                            lhs->as.number > rhs->as.number);
    break;
  }

  case NK_ADD: {
    Node *lhs = eval(expr->as.binop.lhs, x, y);
    if (!lhs)
      return NULL;
    if (!expect_number(lhs))
      return NULL;
    Node *rhs = eval(expr->as.binop.rhs, x, y);
    if (!rhs)
      return NULL;
    if (!expect_number(rhs))
      return NULL;
    return node_number_loc(expr->file, expr->line,
                           lhs->as.number + rhs->as.number);
    break;
  }
  case NK_MULT: {
    Node *lhs = eval(expr->as.binop.lhs, x, y);
    if (!lhs)
      return NULL;
    if (!expect_number(lhs))
      return NULL;
    Node *rhs = eval(expr->as.binop.rhs, x, y);
    if (!rhs)
      return NULL;
    if (!expect_number(rhs))
      return NULL;
    return node_number_loc(expr->file, expr->line,
                           lhs->as.number * rhs->as.number);
    break;
  }
  case NK_TRIPLE: {
    Node *first = eval(expr->as.triple.first, x, y);
    Node *second = eval(expr->as.triple.second, x, y);
    Node *third = eval(expr->as.triple.third, x, y);
    return node_triple_loc(expr->file, expr->line, first, second, third);
    break;
  }
  case NK_MOD: {
    Node *lhs = eval(expr->as.binop.lhs, x, y);
    if (!lhs)
      return NULL;
    if (!expect_number(lhs))
      return NULL;
    Node *rhs = eval(expr->as.binop.rhs, x, y);
    if (!rhs)
      return NULL;
    if (!expect_number(rhs))
      return NULL;
    return node_number_loc(expr->file, expr->line,
                           fmodf(lhs->as.number, rhs->as.number));

    break;
  }
  case NK_IF: {
    Node *cond = eval(expr->as.iff.cond, x, y);
    if (!cond)
      return NULL;
    if (!expect_boolean(cond))
      return NULL;
    Node *then = eval(expr->as.iff.then, x, y);
    if (!then)
      return NULL;
    Node *elze = eval(expr->as.iff.elze, x, y);
    if (!elze)
      return NULL;
    return cond->as.boolean ? then : elze;
    break;
  }
  default: {
    NOB_UNREACHABLE("eval");
  }
  }
}

bool *eval_func(Node *body, float x, float y, Color *c) {
  Node *result = eval(body, x, y);
  if (result == NULL) {
    return (void *)false;
  }
  if (!expect_triple(result)) {
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

bool render_pixels(Node *f) {
  // inside thew for loop we have to normalize the HEIGHT and WIDTH between -1
  // to 1 but we have current range 0 to Height and 0 to Width;
  for (int y = 0; y < HEIGHT; y++) {
    // 0..<HEIGHT -> 0..<1 -> 0..<2 -> -1..<1
    float ny = (float)y / HEIGHT * 2.0f - 1.0f;
    for (int x = 0; x < WIDTH; x++) {
      // 0..<WIDTH -> 0..<1 -> 0..<2 -> -1..<1
      float nx = (float)x / WIDTH * 2.0f - 1.0f;
      // Color c = f(nx, ny);
      Color c;
      if (!eval_func(f, nx, ny, &c))
        return false;
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
  return true;
}

#define node_print_ln(node) (node_print(node), printf("\n"))

int main(void) {
  printf("\033[1;32m\n------------code Execution starts "
         "here------------\n\033[0m");
  // bool ok = render_pixels(node_if(
  //     node_gt(node_mult(node_x(), node_y()), node_number(0)),
  //     node_triple(node_x(), node_y(), node_number(1)),
  //     node_triple(node_mod(node_x(), node_y()), node_mod(node_x(), node_y()),
  //                 node_mod(node_x(), node_y()))));
  // bool ok = render_pixels(node_triple(node_y(), node_x(), node_x()));
  bool ok = render_pixels(node_if(
      node_gt(node_mult(node_x(), node_y()), node_number(0)),
      node_triple(node_x(), node_y(), node_number(1)),
      node_triple(node_mod(node_x(), node_y()), node_mod(node_x(), node_y()),
                  node_mod(node_x(), node_y())))

  );
  if (!ok)
    return 1;
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
