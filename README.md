Qompose
=======

Qompose is a simple programmer's text editor. It is designed to be extremely lightweight, and to not include a large number of features that are largely unused, but to still support a very efficient workflow.

Dependencies
------------

- CMake 3.0.2 or later.
- GCC 4.8 or later, or Visual Studio 2015 or later
- Boost 1.56.0 or later.
- Hunspell 1.3.3 or later.
- Libgit2 0.22.2 or later.
- ICU 54.1 or later.
- Qt 5.4 or later.

**Note:** linking against these dependencies statically is unsupported on all platforms. It should work without too much trouble, in theory, but it is untested.

Building on Linux
-----------------

Building on Linux, assuming that all of the dependencies have been installed via the system's package manager, is trivial. From inside the Qompose source tree, perform something like the following:

```
mkdir build && cd build
cmake ..
make
```

Building on Windows
-------------------

First, on Windows there are sometimes issues where executables generated by things like configure scripts cannot be removed because soon after being created, they are indexed by the Windows Search, or are scanned by the system's antivirus. It's worth disabling these features, at least for the development directory, to avoid these issues.

Before building Qompose, we need to build a copy of ICU. For this, we'll use the normal Windows command prompt. The build environment will only work if a) MSVC is in the PATH, b) Cygwin is in the path, but **after** MSVC, and c) no other GNU utilities are in the path before Cygwin. To achieve this, we do the following in a standard Windows command prompt:

```
set "PATH=C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\Systemm32\Wbem"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat"
set "PATH=%PATH%;C:\cygwin64\bin"
```

Then, we build and install ICU - from inside the ICU directory:

```
cd source
dos2unix *
dos2unix -f configure

bash runConfigureICU Cygwin/MSVC --prefix=/cygdrive/c/dev/prefix \
	--disable-static --enable-shared \
	--disable-debug --enable-release

make
make install
```
There are quite a few common build errors in ICU on Cygwin/MSVC. Believe it or not, all (or almost all) of these errors are caused by an incorrect `PATH` setting. If ICU fails to build, try recofiguring your environment exactly as described above. For more information on building ICU, see [this page](http://qt-project.org/wiki/Compiling-ICU-with-MSVC).

Now we'll build a copy of Qt 5 for Qompose. Qompose is most well-supported by Qt 5.4, so we'll build a copy from Git. The instructions for doing this are discussed in detail [here](http://qt-project.org/wiki/Building_Qt_5_from_Git). First, we need to setup a sane environment for Qt 5. This is done using something like the following, from inside a standard Windows command prompt:

```
set "PATH=C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\Systemm32\Wbem"
set "PATH=%PATH%;C:\Perl64\bin;C:\Python34;C:\Ruby200-x64\bin;C:\Program Files (x86)\CMake\bin"
"C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat"

set "PATH=%PATH%;C:\dev\prefix\lib"
set "INCLUDE=%INCLUDE%;C:\dev\prefix\include"
set "LIB=%LIB%;C:\dev\prefix\lib"
```

Then, we build and install Qt 5 - from inside the root of the Qt 5 source tree:

```
configure -prefix C:\dev\prefix -opensource -confirm-license \
	-release -shared -nomake examples -nomake tests

nmake
nmake install
```

Then, finally, it's time to build Qompose itself. From inside the Qompose source tree, using the same environment that we used to build Qt 5 itself:

```
mkdir build
cd build

cmake .. -G "Visual Studio 11 2012" \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_PREFIX_PATH=/c/dev/prefix

cmake --build . --config Release --target ALL_BUILD
```

The Qompose executable should be in `build/Release`. The only other step that's necessary is to copy any DLL's it needs into the same directory as the executable. The final directory layout should look something like this:

```
icudt54.dll
icuin54.dll
icuuc54.dll
libEGL.dll
libGLESv2.dll
Qompose.exe
Qt5Core.dll
Qt5Gui.dll
Qt5Network.dll
Qt5PrintSupport.dll
Qt5Widgets.dll
```
