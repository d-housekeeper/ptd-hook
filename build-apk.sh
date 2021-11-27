#!/usr/bin/env bash
set -eu
: "$KEYSTORE_FILE"
cp -R ./smali/ ./out/apktool/
cp AndroidManifest.xml ./out/apktool/
cp out/cmake/hook/libhook.so out/cmake/acd/lib__57d5__.so ./out/apktool/lib/armeabi-v7a/

pushd ./out/apktool/
apktool b -f -o ../PTD_tmp.apk
popd

rm -f ./out/PTD_modded.apk
zipalign -v 4 ./out/PTD_tmp.apk ./out/PTD_modded.apk
rm ./out/PTD_tmp.apk
apksigner sign --ks "$KEYSTORE_FILE" ./out/PTD_modded.apk
