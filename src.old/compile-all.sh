#!/bin/bash
KERNEL_SOURCE="/opt/dsm-kernel/$TARGET_PLATFORM/linux-4.4.x"
KHOME="/opt/dsm-kernel/$TARGET_PLATFORM"

CROSS_COMPILE="$KHOME/usr/local/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/bin/x86_64-pc-linux-gnu-"
CFLAGS="-I$KHOME/usr/local/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/include/" 
LDFLAGS="-I$KHOME/usr/local/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/lib/" 
RANLIB="/usr/bin/x86_64-linux-gnu-ranlib" 
LD="/usr/bin/x86_64-linux-gnu-ld" 
CC="/usr/bin/x86_64-linux-gnu-gcc"
LD_LIBRARY_PATH="$KHOME/usr/local/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/lib" 
KSRC="$KHOME/usr/local/x86_64-pc-linux-gnu/x86_64-pc-linux-gnu/sys-root/usr/lib/modules/DSM-7.1/build/" \
ARCH=x86_64 



function compile() {

echo -n "Compiling module "

make KSRC=${KSRC} -C $KSRC M=`pwd` modules $@ 

 while read module; do
    module="`basename ${module}`"
    modinfo --field version ./$module > .version
    strip -g "${module}"
    echo "Copying ${module}"
    cp "${module}" "/opt/dsm-kernel/modules_make/$TARGET_PLATFORM/"
    [ -f /opt/dsm-kernel/modules_make/$TARGET_PLATFORM/${module} ] || echo "${module} Compile or copy failed" && echo "${module} Compiled and copied" 
  done < <(find . -name \*.ko)

echo "Cleaning compiled objects " && rm -rf *.o *.ko

}

for mod in `cat all_modules`
do
echo "Entering module source -> $mod "
cd $mod
compile 1> /dev/null 2> compile.log
cd ../
done
