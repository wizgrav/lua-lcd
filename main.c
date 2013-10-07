#include "lua.h"
#include "lauxlib.h"
#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define ARGCHECK(n)\
	if(lua_gettop(L) != n){\
		 luaL_error(L,"Invalid number of arguments.");\
	}\
	lcd_handle *l = luaL_checkudata(L,1,"wiringPi.lcd");\
	if(!l) luaL_error(L,"First argument must be an lcd handle.")

typedef struct lcd_handle{
	int handle;
}lcd_handle;

static int lcd_init (lua_State *L) {
  if(lua_gettop(L) != 13){
	  luaL_error(L,"Incorrect number of arguments.");
  }
  int rows ,  cols,  bits, rs,  strb, d0,  d1,  d2,  d3,  d4, d5,  d6,  d7;
  
  rows = (int) lua_tointeger(L,2);
  cols = (int) lua_tointeger(L,3);
  bits = (int) lua_tointeger(L,4);
  rs = (int) lua_tointeger(L,5);
  strb = (int) lua_tointeger(L,6);
  d0 = (int) lua_tointeger(L,7);
  d1 = (int) lua_tointeger(L,8);
  d2 = (int) lua_tointeger(L,9);
  d3 = (int) lua_tointeger(L,10);
  d4 = (int) lua_tointeger(L,11);
  d5 = (int) lua_tointeger(L,12);
  d6 = (int) lua_tointeger(L,13);
  d7 = (int) lua_tointeger(L,14);
  
  int handle = lcdInit ( rows,  cols,  bits, rs,  strb, d0,  d1,  d2,  d3,  d4, d5,  d6,  d7) ;
  
  if(handle < 0){
	luaL_error(L,"Error initializing lcd.");
  }
  
  lua_settop(L,0);
  lcd_handle *l = lua_newuserdata(L,sizeof(lcd_handle));
  luaL_getmetatable(L,"wiringPi.lcd");
  lua_setmetatable(L,-2);
  return 1;
}


static int lcd_home (lua_State *L) {
	ARGCHECK(1);
	lcdHome(l->handle);
	return 0;
}

static int lcd_clear (lua_State *L) {
	ARGCHECK(1);
	lcdClear(l->handle);
	return 0;
}

static int lcd_cursor (lua_State *L) {
	ARGCHECK(2);
	lcdCursor(l->handle,lua_toboolean(L,2));
	return 0;
}

static int lcd_display (lua_State *L) {
	ARGCHECK(2);
	lcdDisplay(l->handle,lua_toboolean(L,2));
	return 0;
}

static int lcd_blink (lua_State *L) {
	ARGCHECK(2);
	lcdCursorBlink(l->handle,lua_toboolean(L,2));
	return 0;
}

static int lcd_position (lua_State *L) {
	ARGCHECK(3);
	lcdPosition(l->handle,(int)lua_tointeger(L,2),(int)lua_tointeger(L,3));
	return 0;
}


static int lcd_print (lua_State *L) {
	ARGCHECK(2);
	const char *s = (const char *) lua_tolstring(L,2,NULL);
	if(!s) luaL_error(L,"Invalid type, expected string.");
	lcdPuts(l->handle,s);
	return 0;
}

static int lcd_def (lua_State *L) {
	ARGCHECK(3);
	int i = lua_tointeger(L,2);
	size_t len;
	unsigned char *s = (unsigned char *)lua_tolstring(L,3,&len);
	if(!s) luaL_error(L,"Invalid type, expected string.");
	if(len != 8) luaL_error(L,"Definition string has to be exactly 8 characters.");
	lcdCharDef(l->handle,i,s);
	return 0;
}



static const luaL_reg lcdlib[] = {
{"init",   lcd_init},
{NULL, NULL}
};

static const luaL_reg ulcdlib[] = {
{"home",   lcd_home},
{"clear",   lcd_clear},
{"display",   lcd_display},
{"def",   lcd_def},
{"cursor",   lcd_cursor},
{"blink",   lcd_blink},
{"position",   lcd_position},
{"print",   lcd_print},
{NULL, NULL}
};

LUALIB_API int luaopen_lcd (lua_State *L) {
  if (wiringPiSetup () == -1) luaL_error(L,"piBoardRev: Unable to determine board revision from /proc/cpuinfo");
  luaL_newmetatable(L, "wiringPi.lcd");
  luaL_register(L, NULL, ulcdlib);
  lua_settop(L,0);
  luaL_register(L, "lcd", lcdlib);
  return 1;
}
