#!/bin/bash
for file in `cat all_modules `
do 
ls $file/Makefile > /dev/null || echo "$file Error" && echo "$file -> OK !"
done

