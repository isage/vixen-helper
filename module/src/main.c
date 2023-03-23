#include "controller.h"

#include <psp2kern/ctrl.h>
#include <psp2kern/io/fcntl.h>
#include <psp2kern/kernel/cpu.h>
#include <psp2kern/kernel/debug.h>
#include <psp2kern/kernel/modulemgr.h>
#include <psp2kern/kernel/suspend.h>
#include <psp2kern/kernel/sysclib.h>
#include <psp2kern/kernel/sysmem/data_transfers.h>
#include <psp2kern/kernel/threadmgr.h>
#include <psp2kern/kernel/threadmgr/event_flags.h>
#include <psp2kern/sblaimgr.h>
#include <psp2kern/usbd.h>
#include <psp2kern/usbserv.h>
#include <taihen.h>


static int started = 0;
static int vendor = 0;
static int product = 0;

SceUID transfer_ev = 0;

static Controller cont;

int logme(char* fmt, ...)
{
    SceUID fd = ksceIoOpen("ux0:data/vixen.txt", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_APPEND, 6);
    if (fd < 0) return -1;
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    ksceIoWrite(fd, buffer, strlen(buffer));
    ksceIoClose(fd);
    return 0;
}

int libvixen_probe(int device_id);
int libvixen_attach(int device_id);
int libvixen_detach(int device_id);

static const SceUsbdDriver libvixenDriver = {
    .name   = "libvixen",
    .probe  = libvixen_probe,
    .attach = libvixen_attach,
    .detach = libvixen_detach,
};

int libvixen_usbcharge_probe(int device_id) { return SCE_USBD_PROBE_FAILED; }
int libvixen_usbcharge_attach(int device_id) { return SCE_USBD_PROBE_FAILED; }
int libvixen_usbcharge_detach(int device_id) { return SCE_USBD_PROBE_FAILED; }

static const SceUsbdDriver libvixenFakeUsbchargeDriver = {
    .name   = "usb_charge",
    .probe  = libvixen_usbcharge_probe,
    .attach = libvixen_usbcharge_attach,
    .detach = libvixen_usbcharge_detach,
};

int libvixen_probe(int device_id)
{
  SceUsbdDeviceDescriptor *device;
  ksceDebugPrintf("probing device: %x\n", device_id);
  device = (SceUsbdDeviceDescriptor *)ksceUsbdScanStaticDescriptor(device_id, 0, SCE_USBD_DESCRIPTOR_DEVICE);
  if (device)
  {
    ksceDebugPrintf("vendor: %04x\n", device->idVendor);
    ksceDebugPrintf("product: %04x\n", device->idProduct);
    if (vendor == device->idVendor && product == device->idProduct)
    {
        return SCE_USBD_PROBE_SUCCEEDED;
    }
  }
  return SCE_USBD_PROBE_FAILED;
}

int libvixen_attach(int device_id)
{
  SceUsbdDeviceDescriptor *device;
  ksceDebugPrintf("attaching device: %x\n", device_id);
  device = (SceUsbdDeviceDescriptor *)ksceUsbdScanStaticDescriptor(device_id, 0, SCE_USBD_DESCRIPTOR_DEVICE);
  if (device)
  {
    ksceDebugPrintf("vendor: %04x\n", device->idVendor);
    ksceDebugPrintf("product: %04x\n", device->idProduct);
    if (vendor == device->idVendor && product == device->idProduct)
    {
      if (!cont.attached)
      {
        // probe and reset

        cont.buffer_size   = 8;
        cont.device_id     = device_id;

        // init endpoints and stuff
        SceUsbdEndpointDescriptor *endpoint;
        ksceDebugPrintf("scanning endpoints for device %d\n", device_id);
        endpoint = (SceUsbdEndpointDescriptor *)ksceUsbdScanStaticDescriptor(device_id, 0, SCE_USBD_DESCRIPTOR_ENDPOINT);
        while (endpoint)
        {
          ksceDebugPrintf("got EP: %02x\n", endpoint->bEndpointAddress);
          if ((endpoint->bEndpointAddress & SCE_USBD_ENDPOINT_DIRECTION_BITS) == SCE_USBD_ENDPOINT_DIRECTION_IN)
          {
            ksceDebugPrintf("opening in pipe\n");
            cont.pipe_in = ksceUsbdOpenPipe(device_id, endpoint);
            cont.buffer_size = endpoint->wMaxPacketSize;
            if (cont.buffer_size > 64)
            {
                ksceDebugPrintf("Packet size too big = %d\n", endpoint->wMaxPacketSize);
                return SCE_USBD_ATTACH_FAILED;
            }
          }
          if (cont.pipe_in > 0)
            break;
          endpoint = (SceUsbdEndpointDescriptor *)ksceUsbdScanStaticDescriptor(device_id, endpoint, SCE_USBD_DESCRIPTOR_ENDPOINT);
        }

        if (cont.pipe_in > 0)
        {
          SceUsbdConfigurationDescriptor *cdesc;
          if ((cdesc = (SceUsbdConfigurationDescriptor *)ksceUsbdScanStaticDescriptor(device_id, NULL, SCE_USBD_DESCRIPTOR_CONFIGURATION)) == NULL)
              return SCE_USBD_ATTACH_FAILED;

          SceUID control_pipe_id = ksceUsbdOpenPipe(device_id, NULL);
          // set default config
          int r = ksceUsbdSetConfiguration(control_pipe_id, cdesc->bConfigurationValue, NULL, NULL);
          ksceDebugPrintf("ksceUsbdSetConfiguration = 0x%08x\n", r);
          if (r < 0)
            return SCE_USBD_ATTACH_FAILED;
            cont.attached = 1;
            cont.inited   = 1;
        }

        // something gone wrong during usb init
        if (!cont.inited)
        {
          ksceDebugPrintf("Can't init gamepad (wired)\n");
          return SCE_USBD_ATTACH_FAILED;
        }
        ksceDebugPrintf("Attached!\n");
        return SCE_USBD_ATTACH_SUCCEEDED;
      }
    }
  }
  return SCE_USBD_ATTACH_FAILED;
}

int libvixen_detach(int device_id)
{
    cont.attached     = 0;
    cont.inited       = 0;
    cont.pipe_in      = 0;
    cont.pipe_out     = 0;
    cont.pipe_control = 0;
    return SCE_USBD_DETACH_SUCCEEDED;
}

int vixenhelperStart(uint16_t vid, uint16_t pid)
{
    uint32_t state;
    ENTER_SYSCALL(state);

    vendor = vid;
    product = pid;
    cont.attached = 0;
    cont.inited = 0;
    int ret_drv = ksceUsbdRegisterDriver(&libvixenDriver);
    ksceDebugPrintf("ksceUsbdRegisterDriver = 0x%08x\n", ret_drv);
    logme("Vixen helper start: VID: 0x%04x, PID: 0x%04x\n", vid, pid);
    usb_read(&cont);

    EXIT_SYSCALL(state);
    return 0;
}

int vixenhelperStop()
{
    uint32_t state;
    ENTER_SYSCALL(state);

    vendor = 0;
    product = 0;
    cont.attached = 0;
    cont.inited = 0;
    int ret_drv = ksceUsbdUnregisterDriver(&libvixenDriver);
    ksceDebugPrintf("ksceUsbdUnregisterDriver = 0x%08x\n", ret_drv);
    logme("Vixen helper stop\n");

    EXIT_SYSCALL(state);
    return 0;
}

int vixenhelperRead(int button)
{
    uint32_t state;
    ENTER_SYSCALL(state);

    if (cont.inited)
    {
        logme("button: %x: \n", button);

        for (int i = 0; i < cont.buffer_size; i++)
            logme("0x%02x ", cont.buffer[i]);
        logme("\n");

        EXIT_SYSCALL(state);
        return 0;
    }

    EXIT_SYSCALL(state);
    return -1;
}

int module_start(SceSize args, void *argp)
{
  tai_module_info_t modInfo;
  modInfo.size = sizeof(tai_module_info_t);

  started = 1;

  if (ksceSblAimgrIsGenuineVITA())
  {
    ksceUsbServMacSelect(2, 0);
  }

  // remove sony usb_charge driver that intercepts HID devices
  int ret_drv = ksceUsbdUnregisterDriver(&libvixenFakeUsbchargeDriver);
  ksceDebugPrintf("ksceUsbdUnregisterDriver = 0x%08x\n", ret_drv);

  transfer_ev = ksceKernelCreateEventFlag("vixen_transfer", 0, 0, NULL);

  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize args, void *argp)
{
  return SCE_KERNEL_STOP_SUCCESS;
}

void _start()
{
  module_start(0, NULL);
}
