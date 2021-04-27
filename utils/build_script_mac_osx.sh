set -x #echo on
curr_path=${BASH_SOURCE%/*}

# check that all the required environment vars are set
: "${ZANO_QT_PATH:?variable not set, see also macosx_build_config.command}"
: "${ZANO_BOOST_ROOT:?variable not set, see also macosx_build_config.command}"
: "${ZANO_BOOST_LIBS_PATH:?variable not set, see also macosx_build_config.command}"
: "${ZANO_BUILD_DIR:?variable not set, see also macosx_build_config.command}"
: "${CMAKE_OSX_SYSROOT:?CMAKE_OSX_SYSROOT should be set to macOS SDK path, e.g.: /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk}"

ARCHIVE_NAME_PREFIX=zano-macos-x64-

if [ -n "$build_prefix" ]; then
  ARCHIVE_NAME_PREFIX=${ARCHIVE_NAME_PREFIX}${build_prefix}-
  build_prefix_label="$build_prefix "
fi

if [ "$testnet" == true ]; then
  testnet_def="-D TESTNET=TRUE"
  testnet_label="testnet "
  ARCHIVE_NAME_PREFIX=${ARCHIVE_NAME_PREFIX}testnet-
fi


rm -rf $ZANO_BUILD_DIR; mkdir -p "$ZANO_BUILD_DIR/release"; cd "$ZANO_BUILD_DIR/release"

cmake $testnet_def -D CMAKE_OSX_SYSROOT=$CMAKE_OSX_SYSROOT -D BUILD_GUI=TRUE -D CMAKE_PREFIX_PATH="$ZANO_QT_PATH/clang_64" -D CMAKE_BUILD_TYPE=Release -D BOOST_ROOT="$ZANO_BOOST_ROOT" -D BOOST_LIBRARYDIR="$ZANO_BOOST_LIBS_PATH" ../..
if [ $? -ne 0 ]; then
    echo "Failed to cmake"
    exit 1
fi



make -j Zano
if [ $? -ne 0 ]; then
    echo "Failed to make Zano"
    exit 1
fi

make -j connectivity_tool daemon simplewallet
if [ $? -ne 0 ]; then
    echo "Failed to make binaries!"
    exit 1
fi


cd src/
if [ $? -ne 0 ]; then
    echo "Failed to cd src"
    exit 1
fi

# copy all necessary libs into the bundle in order to workaround El Capitan's SIP restrictions
mkdir -p Zano.app/Contents/Frameworks/boost_libs
cp -R "$ZANO_BOOST_LIBS_PATH/" Zano.app/Contents/Frameworks/boost_libs/
if [ $? -ne 0 ]; then
    echo "Failed to cp workaround to MacOS"
    exit 1
fi

# rename process name to big letter 
mv Zano.app/Contents/MacOS/zano Zano.app/Contents/MacOS/Zano
if [ $? -ne 0 ]; then
    echo "Failed to rename process"
    exit 1
fi

cp zanod simplewallet Zano.app/Contents/MacOS/
if [ $? -ne 0 ]; then
    echo "Failed to copy binaries to Zano.app folder"
    exit 1
fi

# fix boost libs paths in main executable and libs to workaround El Capitan's SIP restrictions
source ../../../utils/macosx_fix_boost_libs_path.sh
fix_boost_libs_in_binary @executable_path/../Frameworks/boost_libs Zano.app/Contents/MacOS/Zano
fix_boost_libs_in_binary @executable_path/../Frameworks/boost_libs Zano.app/Contents/MacOS/simplewallet
fix_boost_libs_in_binary @executable_path/../Frameworks/boost_libs Zano.app/Contents/MacOS/zanod
fix_boost_libs_in_libs @executable_path/../Frameworks/boost_libs Zano.app/Contents/Frameworks/boost_libs



"$ZANO_QT_PATH/clang_64/bin/macdeployqt" Zano.app
if [ $? -ne 0 ]; then
    echo "Failed to macdeployqt Zano.app"
    exit 1
fi



rsync -a ../../../src/gui/qt-daemon/html Zano.app/Contents/MacOS --exclude less --exclude package.json --exclude gulpfile.js
if [ $? -ne 0 ]; then
    echo "Failed to cp html to MacOS"
    exit 1
fi

cp ../../../src/gui/qt-daemon/app.icns Zano.app/Contents/Resources
if [ $? -ne 0 ]; then
    echo "Failed to cp app.icns to resources"
    exit 1
fi

codesign -s "Zano" --deep -vv -f Zano.app
if [ $? -ne 0 ]; then
    echo "Failed to sign application"
    exit 1
fi

read version_str <<< $(DYLD_LIBRARY_PATH=$ZANO_BOOST_LIBS_PATH ./connectivity_tool --version | awk '/^Zano/ { print $2 }')
version_str=${version_str}
echo $version_str


echo "############### Prepearing archive... ################"
mkdir package_folder
if [ $? -ne 0 ]; then
    echo "Failed to zip app"
    exit 1
fi

mv Zano.app package_folder 
if [ $? -ne 0 ]; then
    echo "Failed to top app package"
    exit 1
fi

package_filename=${ARCHIVE_NAME_PREFIX}${version_str}.dmg

source ../../../utils/macosx_dmg_builder.sh
build_fancy_dmg package_folder $package_filename
if [ $? -ne 0 ]; then
    echo "Failed to create fancy dmg"
    exit 1
fi

cd ../../..
echo "Build success"

echo "############### Uploading... ################"

package_filepath=$ZANO_BUILD_DIR/release/src/$package_filename

scp $package_filepath zano_build_server:/var/www/html/builds/
if [ $? -ne 0 ]; then
    echo "Failed to upload to remote server"
    exit 1
fi


read checksum <<< $( shasum -a 256 $package_filepath | awk '/^/ { print $1 }' )

mail_msg="New ${build_prefix_label}${testnet_label}build for macOS-x64:<br>
https://build.zano.org/builds/$package_filename<br>
sha256: $checksum"

echo "$mail_msg"

echo "$mail_msg" | mail -s "Zano macOS-x64 ${build_prefix_label}${testnet_label}build $version_str" ${emails}
