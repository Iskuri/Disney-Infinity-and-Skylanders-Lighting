#!/usr/bin/python

import usb.core
import usb.util

dev = usb.core.find(idVendor=0x0e6f, idProduct=0x0129)

if dev is None:
    raise ValueError('Device not found')


interface = 0
# if dev.is_kernel_driver_active(interface) is True:
dev.detach_kernel_driver(interface)
usb.util.claim_interface(dev, interface)
usb.util.release_interface(dev, interface)
dev.attach_kernel_driver(interface)

dev.set_configuration()

cfg = dev.get_active_configuration()
intf = cfg[(0,0)]

dev.write(1,"LALA");