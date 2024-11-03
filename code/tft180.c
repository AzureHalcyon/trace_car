#include "tft180.h"

uint16_t current_x = 0, current_y = 0;
uint16_t char_width = 8, char_height = 16;  // 8x16 ����


/**
 * @brief ��ָ��λ����ʾ�ַ�
 * 
 * @param x        x ����
 * @param y        y ����
 * @param format   ��ʽ���ַ���
 * @param ...      �ɱ�����б�
 * 
 * @example display_at(0, 0, "Hello, world!\n"); �� (0, 0) λ����ʾ�ı�
 */
void display_at(uint16_t x, uint16_t y, const char *format, ...) {
    va_list args;
    va_start(args, format);

    // ��̬���仺����
    size_t buffer_size = 256;
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        va_end(args);
        return;
    }

    int len = vsnprintf(buffer, buffer_size, format, args);
    if (len >= (int)buffer_size) {
        buffer_size = len + 1;
        char *new_buffer = (char *)realloc(buffer, buffer_size);
        if (!new_buffer) {
            free(buffer);
            va_end(args);
            return;
        }
        buffer = new_buffer;
        vsnprintf(buffer, buffer_size, format, args);
    }
    va_end(args);

    // ��ʾ�ı�
    const char *str = buffer;
    while (*str) {
        if (*str == '\n') {
            x = 0;
            y += char_height;
            if (y + char_height > TFT180_YMAX) {
                y = 0;
            }
        } else {
            if (x + char_width > TFT180_XMAX) {
                x = 0;
                y += char_height;
                if (y + char_height > TFT180_YMAX) {
                    y = 0;
                }
            }
            tft180_show_char(x, y, *str);
            x += char_width;
        }
        str++;
    }

    free(buffer);
}

/**
 * @brief ��ʾ�ı�
 * 
 * @param format    ��ʽ���ַ���
 * @param ...       �ɱ�����б�
 * 
 * @example display("Hello, world!\n"); �ڵ�ǰλ����ʾ�ı�
 */
void display(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // ��̬���仺����
    size_t buffer_size = 256;
    char *buffer = (char *)malloc(buffer_size);
    if (!buffer) {
        va_end(args);
        return;
    }

    int len = vsnprintf(buffer, buffer_size, format, args);
    if (len >= (int)buffer_size) {
        buffer_size = len + 1;
        char *new_buffer = (char *)realloc(buffer, buffer_size);
        if (!new_buffer) {
            free(buffer);
            va_end(args);
            return;
        }
        buffer = new_buffer;
        vsnprintf(buffer, buffer_size, format, args);
    }
    va_end(args);

    // ʹ�õ�ǰ������ʾ�ı�
    const char *str = buffer;
    uint16_t x = current_x;
    uint16_t y = current_y;
    while (*str) {
        if (*str == '\n') {
            x = 0;
            y += char_height;
            if (y + char_height > TFT180_YMAX) {
                y = 0;
            }
        } else {
            if (x + char_width > TFT180_XMAX) {
                x = 0;
                y += char_height;
                if (y + char_height > TFT180_YMAX) {
                    y = 0;
                }
            }
            tft180_show_char(x, y, *str);
            x += char_width;
        }
        str++;
    }

    // ���µ�ǰλ��
    current_x = x;
    current_y = y;

    free(buffer);
}