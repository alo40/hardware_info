#include <stdio.h>
#include <stdlib.h>
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>

void printCFString(CFStringRef str) {
    if (!str) return;
    char buffer[1024];
    if (CFStringGetCString(str, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
        printf("%s\n", buffer);
    }
}

void getIOProperty(const char *service, const char *property) {
    io_registry_entry_t entry = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching(service));
    if (entry == MACH_PORT_NULL) {
        printf("%s not found.\n", service);
        return;
    }

    CFStringRef key = CFStringCreateWithCString(kCFAllocatorDefault, property, kCFStringEncodingUTF8);
    CFTypeRef value = IORegistryEntryCreateCFProperty(entry, key, kCFAllocatorDefault, 0);
    if (value) {
        printf("%s - %s: ", service, property);
        if (CFGetTypeID(value) == CFStringGetTypeID()) {
            printCFString((CFStringRef)value);
        } else if (CFGetTypeID(value) == CFDataGetTypeID()) {
            printf("[binary data]\n");
        } else if (CFGetTypeID(value) == CFNumberGetTypeID()) {
            int num = 0;
            CFNumberGetValue((CFNumberRef)value, kCFNumberIntType, &num);
            printf("%d\n", num);
        } else {
            printf("[unknown type]\n");
        }
        CFRelease(value);
    } else {
        printf("%s - %s not available.\n", service, property);
    }

    CFRelease(key);
    IOObjectRelease(entry);
}

int main() {
    printf("=== macOS Hardware Info ===\n");

    getIOProperty("IOPlatformExpertDevice", "model");           // Mac model
    getIOProperty("IOPlatformExpertDevice", "serial-number");   // Serial
    getIOProperty("IOPlatformExpertDevice", "board-id");        // Board ID

    getIOProperty("IOPlatformExpertDevice", "IOPlatformUUID");  // Platform UUID

    getIOProperty("AppleACPIPlatformExpert", "system-type");    // System type
    getIOProperty("IOCPU", "clock-frequency");                  // CPU frequency
    getIOProperty("IOCPU", "processor-name");                   // CPU name

    return 0;
}