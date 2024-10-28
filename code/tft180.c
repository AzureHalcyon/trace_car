#include "tft180.h"

uint16_t x = 0, y = 0;
uint16_t char_width = 8;  // 8x16����
uint16_t char_height = 16;

void display(const char *format, ...) {
    char buffer[256];  // ����ַ�������Ϊ256
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
                y = 0;      // ���������Ļ�߶ȣ��Ӷ������¿�ʼ
            }
        } else {
            if (x + char_width > TFT180_XMAX) {
                x = 0;
                y += char_height;
                if (y + char_height > TFT180_YMAX) {
                    y = 0;  // ���������Ļ�߶ȣ��Ӷ������¿�ʼ
                }
            }
            tft180_show_char(x, y, *str);
            x += char_width;
        }
        str++;
    }
}
