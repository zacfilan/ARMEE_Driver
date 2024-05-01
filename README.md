# Building an ARM Windows Driver

> I tried this natively on the ARM64-Windows box, but ran into some compilation errors. In order to build this driver I needed to use Visual Studio Enterprise on my X86 developer laptop and cross-compile to arm64.

I need some code from a user space application to execute some code in EL1. The only way to do that is apparently to create a driver. So I started here [Building ARM64 Windows Drivers](https://learn.microsoft.com/en-us/windows-hardware/drivers/develop/building-arm64-drivers).

* Download the Windows [SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/). Ensure that you have SDK version 16299 (Windows 10, version 1709) or later. 
  >I had to uninstall a few other SDKs first, before this would allow itself to be installed.
* Download the [WDK](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk). Ensure that you have WDK version 16299 or later. Put the arm64 tools dir in the PATH.
  >This didn't work when it went to install the extension into Visual Studio, but I was able to search for and add the extension from within Visual Studio itself. 

> I will want to build a KMDF (Kernel-Mode Driver Framework) driver, to be able to run in kernel mode.

Starting with [Write a Hello World Windows Driver (KMDF)](https://learn.microsoft.com/en-us/windows-hardware/drivers/gettingstarted/writing-a-very-small-kmdf--driver). Then write assembly code  to set/clear uci. I needed to create a custom build rule for this in the solution to use armasm64.

> I am able to build this with a cross compile. So I'll need to deploy it to the ARM-Windows machine for testing.

In these steps target machine is the ARM64-Windows machine. Host machine is my laptop.
* Connect to the target machine by RDP
* Deploy the whole ARMEE_Driver folder over to the target machine (copy and paste via RDP)
* In admin terminal **NOT POWERSHELL** on target machine execute
  ```
  Microsoft Windows [Version 10.0.22621.3447]
  (c) Microsoft Corporation. All rights reserved.

  sc delete ARMEE_Driver
  devcon install c:\Users\zac\Desktop\ARMEE_Driver\ARMEE_Driver.inf "Root\ARMEE_Driver"
  ```
  > Note: Don't use these. These are just breadcrumbs of things I tried and are here for reference.
  >
  > ```c
  > // bcdedit /set testsigning on
  > // cd c:\Users\zac\Desktop\ARMEE_Driver
  > // sc create ARMEE_Driver type=kernel start=demand binPath="c:\Users\zac\Desktop\ARMEE_Driver\ARMEE_Driver.sys"
  > // sc start ARMEE_Driver
  >
  > // You can also right click the .inf file and install it.
  >
  > // queries:
  > // devcon status Root/ARMEE_Driver
  > // sc qc ARMEE_Driver
  > // sc query ARMEE_Driver
  > // sc status ARMEE_Driver

### Kernel Driver Debugging

Use WinDbg for ARM64, local live kernel debugging on the target machine.
```
bcdedit -debug on
```
