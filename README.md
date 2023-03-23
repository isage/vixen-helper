# VXHelper

Helper app to provide necessary data required for adding new controllers support into [ViXEn](https://github.com/isage/vixen)

## Usage

* Copy `vixen-helper.skprx` into  `ur0:/tai` and add it to your config.txt under `*KERNEL`, reboot.  
* Install VXHelper.vpk and launch.
* Connect your usb gamepad when asked (you can also connect it prior to launching app).
* Select your device from list of VENDOR_ID:PRODUCT_ID, you can get this values by connecting gamepad to pc and using [gamepad tester website](https://gamepad-tester.com/) or lsusb (on linux/osx).  
* Follow on-screen instructions:
  * When app tells you to "Press and hold <button> then press X" this means press and hold said button on usb gamepad. While still holding it - press X on ds3/ds4.
  * When the app tells you to press and hold next button - **release** previous button, then press and hold new one.
  * Repeat for all buttons. If your gamepad doesn't have corresponding button - you can skip it by pressing O on ds3/ds4
  * You can exit app when it tells you to
* Locate `ux0:/data/vixen.txt`, open [new issue](https://github.com/isage/vixen/issues/new) in ViXEn repo, provide gamepad name and attach your file

## License

GPLv3, see LICENSE.md
