#!/usr/bin/env bash

echo "Creating ISO image for PlayStation Emulator"

echo "Copying files to tmp directory"

cp -r ../../bin_psx /tmp
cp -r ../cdrom /tmp
cp -r ~/.wine/drive_c/psyq/cdgen/LCNSFILE /tmp

echo "Generating unlicensed image file"

echo -e 'D:\r cd "tmp/cdrom/"\r BUILDCD.EXE -l -iMETIS.IMG METIS.CTI\r exitemu\r' | dosemu -dumb

echo "Generating licensed ISO file"

stripisowin.exe s 2352 /tmp/cdrom/METIS.IMG /tmp/cdrom/METIS.ISO

PSXLICENSE.exe /eu /i /tmp/cdrom/METIS.ISO

echo "Moving ISO to bin_psx directory"

cp /tmp/cdrom/METIS.ISO ../../bin_psx

echo "Cleaning Up files"

rm -rf /tmp/bin_psx
rm -rf /tmp/cdrom
rm -rf /tmp/LCNSFILE

