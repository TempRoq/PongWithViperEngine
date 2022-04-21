#pragma once

/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "entity/ga_component.h"

#include <cstdint>

/*
** Renderable basic textured cubed.
*/
class ga_ball_component : public ga_component
{
public:
	ga_ball_component(class ga_entity* ent, const char* texture_file);
	virtual ~ga_ball_component();

	virtual void update(struct ga_frame_params* params) override;

private:
	class ga_material* _material;
	uint32_t _vao;
	uint32_t _vbos[4];
	uint32_t _index_count;
};
