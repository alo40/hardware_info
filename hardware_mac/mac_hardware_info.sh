#!/bin/bash

echo "=== macOS Hardware Info ==="

# Basic system info
echo -e "\n--- Mac Info ---"
system_profiler SPHardwareDataType | grep -E "Model Identifier|Serial Number|Hardware UUID"

# CPU info
echo -e "\n--- CPU Info ---"
sysctl -n machdep.cpu.brand_string
sysctl -n hw.ncpu | awk '{print "Cores: " $1}'
sysctl -n hw.cpufrequency | awk '{printf("CPU Frequency: %.2f GHz\n", $1 / 1000000000)}'

# RAM info
echo -e "\n--- Memory Info ---"
sysctl -n hw.memsize | awk '{printf("RAM: %.2f GB\n", $1 / 1073741824)}'

# GPU info
echo -e "\n--- GPU Info ---"
system_profiler SPDisplaysDataType | grep "Chipset Model"

# Storage info
echo -e "\n--- Storage Devices ---"
system_profiler SPStorageDataType | grep -E "Media Name|Size|Mount Point"

# Network interfaces
echo -e "\n--- Network Interfaces ---"
networksetup -listallhardwareports | awk '/Hardware Port|Device|Ethernet Address/ {print}'

# USB devices
echo -e "\n--- USB Devices ---"
system_profiler SPUSBDataType | grep -E "Product ID|Vendor ID|Serial Number|Location ID|Manufacturer" | sed 's/^ *//'

# Platform UUID
echo -e "\n--- Platform UUID ---"
ioreg -rd1 -c IOPlatformExpertDevice | grep IOPlatformUUID | awk -F\" '{print "UUID: " $4}'
