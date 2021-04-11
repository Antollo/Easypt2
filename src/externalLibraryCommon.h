#ifndef D9964C33_5B83_41FE_B49D_2D1A0E5275CF
#define D9964C33_5B83_41FE_B49D_2D1A0E5275CF

#include <cstdint>
#include "number.h"

using FunctionTypeType = int64_t;
using registerFunctionType = void (*)(const char *, void *, FunctionTypeType);
using registerIntegerConstantType = void (*)(const char *, int64_t);
using initializeType = void (*)(registerFunctionType, registerIntegerConstantType);

constexpr const int8_t voidType = 0;
constexpr const int8_t bufferType = 1;
constexpr const int8_t stringType = 2;
constexpr const int8_t numberType = 3;
constexpr const int8_t arrayType = 4;
constexpr const int8_t pointerAsBufferType = 5;
constexpr const int8_t errorType = 6;

#endif /* D9964C33_5B83_41FE_B49D_2D1A0E5275CF */
