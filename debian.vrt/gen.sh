#!/bin/sh
cat changelog | sed -r "s/%VRT%/120/"   > 120/changelog 
cat changelog | sed -r "s/%VRT%/71/"    > 71/changelog
cat changelog | sed -r "s/%VRT%/999/"   > trunk/changelog


