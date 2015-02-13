
#include <lua.h>
#include <lauxlib.h>

#ifdef LUA_WIN

typedef unsigned long DWORD;
typedef long LONG;
typedef long long LONGLONG;

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;



#include <sys/utime.h> 	/* High-presision counter */

static int l_clock(lua_State *L) {
	/* printf("warning: sys.clock not implemented on Windows\n");  */
    LARGE_INTEGER freq = 0;
	LARGE_INTEGER tm0;
	
	if (freq.LowPart == 0 && freq.HighPart == 0)
	    QueryPerformanceFrequency(&fr);
	QueryPerformanceCounter(&tm0);

	double precise_time = (double)tm0/(double)freq;
	lua_pushnumber(L,precise_time);
	return 1;
}

static int l_usleep(lua_State *L) {
    printf("warning: sys.usleep not implemented on Windows\n");
    return 0;
}

#else

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <sys/time.h>
#include <unistd.h>
#include <time.h>

static int l_clock(lua_State *L) {
  struct timeval tv;
  struct timezone tz;
  struct tm *tm;
  gettimeofday(&tv, &tz);
  tm=localtime(&tv.tv_sec);
  double precise_time = tv.tv_sec + tv.tv_usec / 1e6;
  lua_pushnumber(L,precise_time);
  return 1;
}

static int l_usleep(lua_State *L) {
  int time = 1;
  if (lua_isnumber(L, 1)) time = lua_tonumber(L, 1);
  usleep(time);
  return 1;
}

#endif

static const struct luaL_reg routines [] = {
  {"clock", l_clock},
  {"usleep", l_usleep},
  {NULL, NULL}
};

int luaopen_libsys(lua_State *L)
{
  luaL_openlib(L, "libsys", routines, 0);
  return 1;
}
