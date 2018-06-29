#change these to match your MinGW installation:
# platform name, for configure scripts

# prefix for MinGW executables (e.g. if MinGW gcc is named i686-w64-mingw32-gcc, use "i686-w64-mingw32-")
MINGW_BIN_PREFIX="i686-w64-mingw32-"
# where to install the libraries
# you'll probably want to set this to the location where all the existing MinGW bin/lib/include folders are
MINGW_INSTALL_DIR="/usr/i686-w64-mingw32" 

export AR=${MINGW_BIN_PREFIX}ar
export CC=${MINGW_BIN_PREFIX}gcc
export RANLIB=${MINGW_BIN_PREFIX}ranlib
export WINDRES=${MINGW_BIN_PREFIX}windres
export PREFIX=${MINGW_INSTALL_DIR}
MAKE="make -j 8"

#cd to the lua source dir first!
$MAKE LUA_A="liblua.a" LUA_T="lua.exe" \
	CC="$CC" AR="$AR rcu" RANLIB="$RANLIB" mingw #&& \
	#$MAKE LUA_A="liblua5.2.a" LUAC_T="luac.exe" \
	#CC="$CC" AR="$AR rcu" RANLIB="$RANLIB" 
	$MAKE install RANLIB="$RANLIB" INSTALL_TOP="$MINGW_INSTALL_DIR" \
	INSTALL_INC="$MINGW_INSTALL_DIR/include/lua/" \
TO_BIN="lua.exe luac.exe" TO_LIB="liblua.a"
