#include "display.h"

#include <stdio.h>
#include <stdint.h>

struct _C8Display {
    GObject parent;
    uint64_t disp[32];
};

G_DEFINE_TYPE(C8Display, c8_display, G_TYPE_OBJECT)

static void c8_display_init(C8Display* self) {
    for (int i = 0; i < 32; ++i) {
        self->disp[i] = 0;
    }
}

static void c8_display_class_init(C8DisplayClass* class) {}

C8Display* c8_display_new(void) {
    return g_object_new(C8_TYPE_DISPLAY, NULL);
}

void c8_display_clear(C8Display* self) {
    for (int i = 0; i < 32; ++i) {
        self->disp[i] = 0;
    }
}

bool c8_display_get_pixel(C8Display* self, int x, int y) {
    g_assert(0 <= x && x < 64);
    g_assert(0 <= y && y < 32);

    return self->disp[y] >> x & 1;
}

void c8_display_set_pixel(C8Display* self, int x, int y, bool value) {
    g_assert(0 <= x && x < 64);
    g_assert(0 <= y && y < 32);

    self->disp[y] &= ~((uint64_t)(1) << x);
    self->disp[y] |= (uint64_t)(value) << x;
}

void c8_display_screenshot(C8Display* self, const char* path) {
    FILE* fp = fopen(path, "wb");
    g_assert(fp != NULL);

    fprintf(fp, "P1\n64 32\n");

    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            fprintf(fp, "%d ", c8_display_get_pixel(self, x, y) ? 1 : 0);
        }

        fprintf(fp, "\n");
    }

    fclose(fp);
}
