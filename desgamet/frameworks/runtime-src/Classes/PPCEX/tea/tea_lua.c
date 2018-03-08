#include <stdint.h>
#include <stdlib.h>
#include "tolua++.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

void qq_encrypt(uint8_t* plain,uint32_t size,uint32_t*key,uint32_t** crypt,uint32_t* c_size);
void qq_decrypt(uint32_t* crypt, uint32_t size, uint32_t* key, uint8_t** plain, uint32_t* p_size, uint32_t* fm);

int tea_encrypts(lua_State *L)
{
    size_t size;
	uint8_t* plain = (uint8_t*)lua_tolstring(L, 1, &size);
    size_t key_size;
    uint32_t* key = (uint32_t*)lua_tolstring(L, 2, &key_size);
    uint32_t* crypt;
    uint32_t c_size;
    qq_encrypt(plain,size,key,&crypt,&c_size);
    lua_pushlstring(L, (char*)crypt, c_size);
    free(crypt);
  return 1;
}
int tea_decrypts(lua_State *L)
{
    size_t size;
    uint32_t* crypt = (uint32_t*)lua_tolstring(L, 1, &size);
    size_t key_size;
    uint32_t* key = (uint32_t*)lua_tolstring(L, 2, &key_size);
    uint8_t* plain;
    uint32_t p_size;
	uint32_t* fm=0;
    qq_decrypt(crypt,size,key,&plain,&p_size,fm);
    char* tmp = (char*)plain;
    if (tmp[0] == 0) {
        tmp ="";
        p_size = 0;
    }
	lua_pushlstring(L, tmp, p_size);
    free(fm);


  return 1;
}


int luaopen_tea(lua_State *L){
    luaL_Reg reg[]= {
        {"encrypts", tea_encrypts},
        {"decrypts", tea_decrypts},
        {NULL,NULL},
    };
    //luaL_newlib(L,reg);
	luaL_register(L, "tea", reg);
    //lua_register(L, "tea_encrypt", tea_encrypt);
    //lua_register(L, "tea_decrypt", tea_decrypt);
    return 1;
}

