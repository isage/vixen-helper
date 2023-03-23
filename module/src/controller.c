#include "controller.h"

#include <psp2kern/kernel/suspend.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/usbd.h>

extern SceUID transfer_ev;

void on_read_data(int32_t result, int32_t count, void *arg)
{
  // process buffer

  Controller *c = (Controller *)arg;
  if (result == 0 && count > 0 && arg)
  {
    if (c->inited)
    {
//        for (int i = 0; i < c->buffer_size; i++)
//            logme("0x%02x ", c->buffer[i]);
//        logme("\n");
    }
  }
//  ksceKernelSetEventFlag(transfer_ev, 2);
    usb_read(c);
}

void usb_read(Controller *c)
{
  int ret;

  if (!c->inited)
    return;

  ret = ksceUsbdInterruptTransfer(c->pipe_in, c->buffer, c->buffer_size, on_read_data, c);

  if (ret < 0)
  {
    ksceDebugPrintf("ksceUsbdInterruptTransfer(in) error: 0x%08x\n", ret);
    logme("ksceUsbdInterruptTransfer(in) error: 0x%08x\n", ret);
    // error out
  }
//  ksceKernelWaitEventFlag(transfer_ev, 2, SCE_EVENT_WAITCLEAR_PAT | SCE_EVENT_WAITAND, NULL, 0);
}
