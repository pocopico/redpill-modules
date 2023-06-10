#!/bin/bash

cat <<EOF > README.md

## Module sources used for redpill extension creation for all platforms

Some sources are vanilla kernel sources and others are backported to compile on the current kernel version

Please report any issues using GH issues

To compile you need syno toolkit and edit the compile.sh accoring to your paths

EOF

echo "|Module Source |Version | Status |" >> README.md
echo "| ------------ |:------:|:------:|" >> README.md

for src in `cat all_modules`
do 
	echo "|$src|`cat $src/.version`|Compiles up to 4.4.180+|"

done >> README.md

