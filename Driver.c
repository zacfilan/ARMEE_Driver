#include <ntddk.h>
#include <wdf.h>

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD ARMEEEvtDeviceAdd;


// Declare the assembly function
extern void set_uci(void);
extern void clear_uci(void);

// Example IOCTL definition
#define IOCTL_set_uci CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_clear_uci CTL_CODE(FILE_DEVICE_UNKNOWN, 0x400, METHOD_BUFFERED, FILE_ANY_ACCESS)

VOID ARMEEEvtDriverUnload(
    _In_ WDFDRIVER Driver
)
{
    // Perform cleanup tasks here
    UNREFERENCED_PARAMETER(Driver);
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "ARMEE: DriverUnload\n"));
}

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT     DriverObject,
    _In_ PUNICODE_STRING    RegistryPath
)
{
    // NTSTATUS variable to record success or failure
    NTSTATUS status = STATUS_SUCCESS;

    // Allocate the driver configuration object
    WDF_DRIVER_CONFIG config;

    // Print "Hello World" for DriverEntry
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "ARMEE: DriverEntry\n"));

    // Initialize the driver configuration object to register the
    // entry point for the EvtDeviceAdd callback, ARMEEEvtDeviceAdd
    WDF_DRIVER_CONFIG_INIT(&config,
        ARMEEEvtDeviceAdd
    );

    // Register the EvtDriverUnload callback
    config.EvtDriverUnload = ARMEEEvtDriverUnload;

    // Finally, create the driver object
    status = WdfDriverCreate(DriverObject,
        RegistryPath,
        WDF_NO_OBJECT_ATTRIBUTES,
        &config,
        WDF_NO_HANDLE
    );
    return status;
}

VOID MyDeviceIoControl(
    WDFQUEUE Queue,
    WDFREQUEST Request,
    size_t OutputBufferLength,
    size_t InputBufferLength,
    ULONG IoControlCode
)
{

    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(IoControlCode);

    NTSTATUS status = STATUS_SUCCESS;
    set_uci();


    switch (IoControlCode)
    {
    case IOCTL_set_uci:
        set_uci();

        // Complete the request
        status = STATUS_SUCCESS;
        break;
    case IOCTL_clear_uci:
        clear_uci();

        // Complete the request
        status = STATUS_SUCCESS;
        break;
    default:
        // The IOCTL is not recognized. Complete the request with an error status.
        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    // Complete the request. 
    WdfRequestComplete(Request, status);
}

NTSTATUS
ARMEEEvtDeviceAdd(
    _In_    WDFDRIVER       Driver,
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    // We're not using the driver object,
    // so we need to mark it as unreferenced
    UNREFERENCED_PARAMETER(Driver);

    NTSTATUS status;

    // Allocate the device object
    WDFDEVICE hDevice;

    WDF_IO_QUEUE_CONFIG ioQueueConfig;

    // Print "Hello World"
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "ARMEE: ARMEEEvtDeviceAdd\n"));

    // Create the device object
    status = WdfDeviceCreate(&DeviceInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &hDevice
    );
    if (!NT_SUCCESS(status)) {
        return status;
    }

    DECLARE_CONST_UNICODE_STRING(deviceName, L"\\??\\ARMEE_Driver");
    status = WdfDeviceCreateSymbolicLink(hDevice, &deviceName);
    if (!NT_SUCCESS(status)) {
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "ARMEE: WdfDeviceCreateSymbolicLink failed with status 0x%X\n", status));
        return status;
    }
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "ARMEE: WdfDeviceCreateSymbolicLink succeeds\n"));

    // Initialize the I/O queue configuration
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig, WdfIoQueueDispatchParallel);
    ioQueueConfig.EvtIoDeviceControl = MyDeviceIoControl;

    // Create the I/O queue
    status = WdfIoQueueCreate(hDevice, &ioQueueConfig, WDF_NO_OBJECT_ATTRIBUTES, NULL);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    return status;
}

