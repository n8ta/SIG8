#!/bin/bash


echo "======================== DYLIB INSERTER =============================="
set -e
echo "Patching to insert dylib linking. OutDir is: $1. APPNAME is $2"
APPNAME="$2";
OUTDIR=$(dirname $(realpath -s $1/dummy.txt))
echo "Canonical outdir: $OUTDIR"
SCRIPT_PATH=$(dirname $(realpath -s $0))
echo "Patch script dir is: $SCRIPT_PATH"

ORIG="_original";
MACOS_DIR="$OUTDIR/$APPNAME.app/Contents/MacOS"
APP_PATH="$MACOS_DIR/$APPNAME"
APP_PATH_ORIG="$MACOS_DIR/$APPNAME$ORIG"

if test -f "$APP_PATH"; then
    echo "$APP_PATH exists."
else
  echo "Application does not exit @ $APP_PATH"
  exit 0
fi
  
echo "Move original application to application_original"
mv "$APP_PATH" "$APP_PATH_ORIG"
echo "Replace application with wrapper that loads libraries"
echo "cp $SCRIPT_PATH/app_wrapper $APP_PATH/$APPNAME"
cp "$SCRIPT_PATH/app_wrapper" "$MACOS_DIR/$APPNAME"
echo "========================================================================"
