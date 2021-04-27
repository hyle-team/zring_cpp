# ZRing C++

### Building in Ubuntu (gcc 7.5.0)

Recommended OS version: Ubuntu 18.04 LTS.

1. Prerequisites

        sudo apt-get install -y build-essential g++ autotools-dev git
    
2. Install CMake 3.13.0 or newer

        curl -OL https://cmake.org/files/v3.13/cmake-3.13.0-Linux-x86_64.sh
        chmod +x cmake-3.13.0-Linux-x86_64.sh 
        sudo sh cmake-3.13.0-Linux-x86_64.sh --prefix=/usr/local/ --exclude-subdir

3. Download and build Boost 1.68 or newer

       wget https://dl.bintray.com/boostorg/release/1.68.0/source/boost_1_68_0.tar.bz2
       tar -xjf boost_1_68_0.tar.bz2
       cd boost_1_68_0
       ./bootstrap.sh --with-libraries=system,filesystem,thread,date_time,chrono,regex,serialization,atomic,program_options,locale,timer
       ./b2

3. Set `BOOST_ROOT` envinorment variable\
  For instance, by adding these lines to `~/.bashrc`:
  
       export BOOST_ROOT=/home/user/boost_1_68_0  


4. Build sources

        cd zring_cpp
        mkdir build && cd build
        cmake ..
        make -j1

5. Look for the binaries in `build/tests` folder

6. Run crypto tests:

        tests/functional_tests --crypto-tests=*


### Building in Windows (MSVC 2017)
Recommended OS version: Windows 7 x64.
1. Install required prerequisites (Boost >=1.68, CMake >=3.13).

2. Copy `utils/configure_local_paths.cmd.example` to `utils/configure_local_paths.cmd` and edit paths in that file.

3. Run `utils/configure_win64_msvs2017.cmd`

4. Go to the build folder and open generated sln file in MSVC.

5. Build and enjoy!
