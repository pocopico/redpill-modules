#!/bin/bash

header="%-20s| %-21s | %-22s | %-22s | %-21s | %-21s | %-21s | %-21s  %c\n"

checkmodule() {

    [ ! -f $1/$2 ] && cp ../compiled-modules/$1/$2 $1/$2 && echo "fixed $1/$2 missing" >>fixed.txt
    [ -f $1/$2 ] && printf "1" || printf "0"

}

printf "$header" Module Name apollolake-4.4.180 broadwell-4.4.180 broadwellnk-4.4.180 denverton-4.4.180 geminilake-4.4.180 r1000-4.4.180 v1000-4.4.180
echo "------------------------------------------------------------------------------------------------------------------------"

for module in $(cat modules.txt); do

    printf "$header" "$module" $(checkmodule "apollolake-4.4.180" "$module") $(checkmodule "broadwell-4.4.180" "$module") $(checkmodule "broadwellnk-4.4.180" "$module") $(checkmodule "denverton-4.4.180" "$module") $(checkmodule "geminilake-4.4.180" "$module") $(checkmodule "r1000-4.4.180" "$module") $(checkmodule "v1000-4.4.180" "$module")

done

#for file in $(ls *tgz); do
#    file=$file
#    dir="$(echo $file | sed -e 's/.tgz//g')"
#    mkdir $dir && cd $dir && tar xf ../$file && cd ../
#    ls -ltr $dir/*.ko | wc -l
#done
