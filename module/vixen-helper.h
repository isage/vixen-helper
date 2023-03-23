#ifndef __VIXEN_H__
#define __VIXEN_H__

#include <psp2/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

  int vixenhelperStart(uint16_t vid, uint16_t pid);
  int vixenhelperStop();
  int vixenhelperRead(int button);
#ifdef __cplusplus
}
#endif

#endif // __VIXEN_H__
