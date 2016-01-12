#include <stdlib.h>
#include <stdint.h>
#include "CStringUtils.h"



string MD4(string message);														/*MD4.c*/
uint32_t *MD4Digest(uint32_t *w, int len);										/*MD4.c*/
void setMD4Registers(uint32_t AA, uint32_t BB, uint32_t CC, uint32_t DD);		/*MD4.c*/
uint32_t changeEndianness(uint32_t x);											/*MD4.c*/

