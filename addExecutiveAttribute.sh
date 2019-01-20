#!/bin/bash

find . -type f -iname '*.dll' -exec git update-index --chmod=+x {} \;
find . -type f -iname '*.exe' -exec git update-index --chmod=+x {} \;
find . -type f -iname '*.cmd' -exec git update-index --chmod=+x {} \;
find . -type f -iname '*.bat' -exec git update-index --chmod=+x {} \;
find . -type f -iname '*.sh'  -exec git update-index --chmod=+x {} \;
