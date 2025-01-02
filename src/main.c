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
  Node_As as;
};

Node *node_number(float number) {
  Node *node = arena_alloc(&node_arena, sizeof(Node));
  node->kind = NK_NUMBER;
  node->as.number = number;
  return node;
}
Node *node_x(void) {
  Node *node = arena_alloc(&node_arena, sizeof(Node));
  node->kind = NK_X;
  return node;
}

Node *node_y(void) {
  Node *node = arena_alloc(&node_arena, sizeof(Node));
  node->kind = NK_Y;
  return node;
}

Node *node_add(Node *lhs, Node *rhs) {
  Node *node = arena_alloc(&node_arena, sizeof(Node));
  node->kind = NK_ADD;
  node->as.binop.lhs = lhs;
  node->as.binop.rhs = rhs;
  return node;
}

Node *node_mult(Node *lhs, Node *rhs) {
  Node *node = arena_alloc(&node_arena, sizeof(Node));
  node->kind = NK_MULT;
  node->as.binop.lhs = lhs;
  node->as.binop.rhs = rhs;
  return node;
}

Node *node_triple(Node *first, Node *second, Node *third) {
  Node *node = arena_alloc(&node_arena, sizeof(Node));
  node->kind = NK_TRIPLE;
  node->as.triple.first = first;
  node->as.triple.second = second;
  node->as.triple.third = third;
  return node;
}

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
    printf("triple(");
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
  Node *node = node_triple(node_x(), node_y(), node_number(0.5));
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
  printf(
      "\033[1;34m\n------------code Execution ends here------------\n\033[0m");
  return 0;
}
