#!/bin/bash


echo "========================= DYLIB Clean = =============================="
set -e
echo "Cleaning patch to insert dylib linking. OutDir is: $1. APPNAME is $2"
APPNAME="$2";
OUTDIR=$(dirname $(realpath -s $1/dummy.txt))
echo "Canonical outdir: $OUTDIR"

MACOS_DIR="$OUTDIR/$APPNAME.app/Contents/MacOS"

APP_PATH="$MACOS_DIR/$APPNAME"
ORIG="_original";

APP_PATH_ORIG="$MACOS_DIR/$APPNAME$ORIG"

if test -f "$APP_PATH_ORIG"; then
  echo "Moving original back to $APP_PATH"
  mv "$APP_PATH_ORIG" "$APP_PATH"
else
  echo "No original app found at '$APP_PATH_ORIG' No action needed for clean"
fi