#include "tft180.h"

uint16_t x = 0, y = 0;
uint16_t char_width = 8;  // 8x16字体
uint16_t char_height = 16;

void display(const char *format, ...) {
    char buffer[256];  // 最大字符串长度为256
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    const char *str = buffer;
    while (*str) {
        if (*str == '\n') {
            x = 0;
            y += char_height;
            if (y + char_height > TFT180_YMAX) {
                y = 0;      // 如果超出屏幕高度，从顶部重新开始
            }
        } else {
            if (x + char_width > TFT180_XMAX) {
                x = 0;
                y += char_height;
                if (y + char_height > TFT180_YMAX) {
                    y = 0;  // 如果超出屏幕高度，从顶部重新开始
                }
            }
            tft180_show_char(x, y, *str);
            x += char_width;
        }
        str++;
    }
}
