call configure_local_paths.cmd

cd ..
@mkdir build_msvc2017_64_tn
cd build_msvc2017_64_tn

cmake -D STATIC=FALSE -DBOOST_ROOT="%BOOST_ROOT%" -DBOOST_LIBRARYDIR="%BOOST_ROOT%\lib64-msvc-14.1" -G "Visual Studio 15 2017 Win64" -T host=x64 ".."