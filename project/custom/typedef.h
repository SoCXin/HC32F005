
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPEDEF_H_
#define __TYPEDEF_H_

#ifdef __cplusplus
 extern "C" {
#endif
#include "stdint.h"
/*!< STM32F10x Standard Peripheral Library old types (maintained for legacy purpose) */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef  int32_t  vs32;
typedef  int16_t  vs16;
typedef  int8_t   vs8;

typedef  int32_t vsc32;  /*!< Read Only */
typedef  int16_t vsc16;  /*!< Read Only */
typedef  int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef  uint32_t  vu32;
typedef  uint16_t vu16;
typedef  uint8_t  vu8;

typedef  uint32_t vuc32;  /*!< Read Only */
typedef  uint16_t vuc16;  /*!< Read Only */
typedef  uint8_t vuc8;   /*!< Read Only */

#ifdef __cplusplus
}
#endif

#endif /* __CONFIG_H_ */
