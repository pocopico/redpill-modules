#!/bin/bash

for platform in `cat platforms`; do echo -n "Working on platform : $platform, found kernel versions   : "; modinfo --field vermagic ${platform}/*.ko |uniq ; done

