#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <psp2common/types.h>
#include <psp2kern/kernel/debug.h>
#include <psp2kern/usbd.h>
#include <stdint.h>

typedef struct
{
  uint8_t attached; // actual gamepad attached
  uint8_t inited;   // usb device attached and inited
  int device_id;
  uint8_t port;
  SceUID pipe_in;
  SceUID pipe_out;
  SceUID pipe_control;
  unsigned char buffer[64] __attribute__((aligned(64)));
  size_t buffer_size;
} Controller;

void usb_read(Controller *c);

#endif // __CONTROLLER_H__
