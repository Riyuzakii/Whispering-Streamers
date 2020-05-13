#!/bin/bash
# Execute before you start working on the attack

sudo modprobe msr
sudo wrmsr -p0 0x1a4 0xe
sudo rdmsr -p0 0x1a4 | grep 'e' &> /dev/null
if [ $? == 0 ]; then
   echo "All prefetchers disabled except Stream-Prefetcher on L2!!"
else
    echo "Prefetcher Disabling FAILED!"
fi
# echo 0 | sudo tee /proc/sys/kernel/randomize_va_space | grep '0' &> /dev/null
# if [ $? == 0 ]; then
#     echo "ASLR Disabled!!"
# else
#     echo "ASLR Disabling FAILED!"
# fi
