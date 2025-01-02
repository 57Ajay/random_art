#define STB_IMAGE_WRITE_IMPLEMENTATION
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX

#include "nob.h"
#include "stb_image_write.h"
#include <stdint.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

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
  return 0;
}
