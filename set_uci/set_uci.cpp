#include <windows.h>
#include <stdio.h>

// Example IOCTL definition
#define IOCTL_set_uci CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_clear_uci CTL_CODE(FILE_DEVICE_UNKNOWN, 0x400, METHOD_BUFFERED, FILE_ANY_ACCESS)

int main() {
    HANDLE hDevice;
    DWORD bytesReturned;
    BOOL result;

    // Open a handle to the device
    hDevice = CreateFileW(L"\\\\.\\ARMEE_Driver",  // Device name
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (hDevice == INVALID_HANDLE_VALUE) {
        printf("Failed to open device, error - %lu\n", GetLastError());
        return 1;
    }

    // Send the IOCTL_set_uci request
    result = DeviceIoControl(hDevice,                    // Device handle
        IOCTL_set_uci,              // Control code
        NULL,                       // Input buffer (not used)
        0,                          // Input buffer size
        NULL,                       // Output buffer (not used)
        0,                          // Output buffer size
        &bytesReturned,             // Bytes returned
        NULL);                      // Overlapped (not used)

    if (!result) {
        printf("DeviceIoControl failed, error - %lu\n", GetLastError());
        CloseHandle(hDevice);
        return 1;
    }

    printf("IOCTL_set_uci request sent successfully\n");

    // Close the handle to the device
    CloseHandle(hDevice);

    return 0;
}