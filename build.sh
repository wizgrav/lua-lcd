LUA_CFLAGS=`pkg-config lua5.1 --cflags`
LIBTOOL="libtool --tag=CC --silent"
$LIBTOOL --mode=compile cc $LUA_CFLAGS -lwiringPi -lwiringPiDev -c main.c
$LIBTOOL --mode=link cc $LUA_CFLAGS -lwiringPi -lwiringPiDev -rpath /usr/local/lib/lua/5.1 -o liblcd.la main.lo
mv .libs/liblcd.so.0.0.0 lcd.so
