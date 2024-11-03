#ifndef __TFT180_H__
#define __TFT180_H__

#include "defines.h"
#include "zf_common_headfile.h"
#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"

void display(const char *format, ...);
void display_at(uint16_t x, uint16_t y, const char *format, ...);

#endif // !__TFT180_H__
