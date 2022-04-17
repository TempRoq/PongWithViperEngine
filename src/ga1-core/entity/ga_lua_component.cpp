/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "ga_lua_component.h"

#include "entity/ga_entity.h"
#include "framework/ga_frame_params.h"

#include <lua.hpp>

#include <cassert>
#include <iostream>
#include <string>

ga_lua_component::ga_lua_component(ga_entity* ent, const char* path) : ga_component(ent)
{
	_lua = luaL_newstate();
	luaL_openlibs(_lua);

	extern char g_root_path[256];
	std::string fullpath = g_root_path;
	fullpath += path;

	int status = luaL_loadfile(_lua, fullpath.c_str());
	if (status)
	{
		std::cerr << "Failed to load script " << path << ": " << lua_tostring(_lua, -1);
		lua_close(_lua);
		_lua = nullptr;
		return;
	}

	lua_pcall(_lua, 0, 0, 0);

}

ga_lua_component::~ga_lua_component()
{
	if (_lua)
	{
		lua_close(_lua);
	}
}

void ga_lua_component::update(ga_frame_params* params)
{
	if (_lua)
	{
	}
}

int ga_lua_component::lua_frame_params_get_input_left(lua_State* state)
{
	int arg_count = lua_gettop(state);
	return 1;
}

int ga_lua_component::lua_frame_params_get_input_right(lua_State* state)
{
	int arg_count = lua_gettop(state);

	return 1;
}

int ga_lua_component::lua_component_get_entity(lua_State* state)
{
	int arg_count = lua_gettop(state);

	return 1;
}

int ga_lua_component::lua_entity_translate(lua_State* state)
{

	return 0;
}
