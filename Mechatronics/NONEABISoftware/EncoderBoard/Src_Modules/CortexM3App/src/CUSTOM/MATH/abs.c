#include "abs.h"

int32_t custom_abs(int32_t value){

  return (value + (value >> 31)) ^ (value >> 31);
}
