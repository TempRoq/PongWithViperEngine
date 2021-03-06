/*
** RPI Game Architecture Engine
**
** Portions adapted from:
** Viper Engine - Copyright (C) 2016 Velan Studios - All Rights Reserved
**
** This file is distributed under the MIT License. See LICENSE.txt.
*/

#include "framework/ga_camera.h"
#include "framework/ga_compiler_defines.h"
#include "framework/ga_input.h"
#include "framework/ga_sim.h"
#include "framework/ga_output.h"
#include "jobs/ga_job.h"

#include "entity/ga_entity.h"
#include "entity/ga_lua_component.h"

#include "graphics/ga_cube_component.h"
#include "graphics/ga_ball_component.h"
#include "graphics/ga_program.h"

#include "gui/ga_font.h"

#include "physics/ga_physics_component.h"
#include "physics/ga_physics_world.h"
#include "physics/ga_rigid_body.h"
#include "physics/ga_shape.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#if defined(GA_MINGW)
#include <unistd.h>
#endif

ga_font* g_font = nullptr;
static void set_root_path(const char* exepath);

int main(int argc, const char** argv)
{
	set_root_path(argv[0]);

	ga_job::startup(0xffff, 256, 256);

	// Create objects for three phases of the frame: input, sim and output.
	ga_input* input = new ga_input();
	ga_sim* sim = new ga_sim();
	ga_physics_world* world = new ga_physics_world();
	ga_output* output = new ga_output(input->get_window());

	// Create the default font:
	g_font = new ga_font("VeraMono.ttf", 16.0f, 512, 512);

	// Create camera.
	ga_camera* camera = new ga_camera({ 0.0f, 0.0f, 50.0f });
	ga_quatf rotation;
	rotation.make_axis_angle(ga_vec3f::y_vector(), ga_degrees_to_radians(180.0f));
	camera->rotate(rotation);
	//rotation.make_axis_angle(ga_vec3f::x_vector(), ga_degrees_to_radians(15.0f));
	//camera->rotate(rotation);

	// Create an entity whose movement is driven by Lua script.
	//Right Paddle entity
	ga_entity rPaddle;
	ga_lua_component lua_move_r(&rPaddle, "data/scripts/movePaddleR.lua");
	ga_cube_component model_r(&rPaddle, "data/textures/rpi.png");
	rPaddle.translate({ 12.0f, 0.0f, 0.0f });
	ga_oobb rPaddle_oobb;
	rPaddle_oobb._half_vectors[0] = ga_vec3f::x_vector().scale_result(-1.0f);// .scale_result(0.3f);
	rPaddle_oobb._half_vectors[1] = ga_vec3f::y_vector().scale_result(4.0f);
	rPaddle_oobb._half_vectors[2] = ga_vec3f::z_vector().scale_result(0.3f);
	ga_physics_component rPaddle_collider(&rPaddle, &rPaddle_oobb, 2.0f);
	rPaddle_collider.get_rigid_body()->make_weightless();
	rPaddle_collider.get_rigid_body()->make_static();
	rPaddle.add_component(&rPaddle_collider);

	world->add_rigid_body(rPaddle_collider.get_rigid_body());
	sim->add_entity(&rPaddle);

	//Left Paddle entity
	ga_entity lPaddle;
	ga_lua_component lua_move_l(&lPaddle, "data/scripts/movePaddleL.lua");
	ga_cube_component model_l(&lPaddle, "data/textures/rpi.png");
	lPaddle.translate({ -12.0f, 0.0f, 0.0f });
	ga_oobb lPaddle_oobb;
	lPaddle_oobb._half_vectors[0] = ga_vec3f::x_vector();// .scale_result(0.3f);
	lPaddle_oobb._half_vectors[1] = ga_vec3f::y_vector().scale_result(4.0f);
	lPaddle_oobb._half_vectors[2] = ga_vec3f::z_vector().scale_result(0.3f);
	ga_physics_component lPaddle_collider(&lPaddle, &lPaddle_oobb, 2.0f);
	lPaddle_collider.get_rigid_body()->make_weightless();
	lPaddle_collider.get_rigid_body()->make_static();
	lPaddle.add_component(&lPaddle_collider);
	world->add_rigid_body(lPaddle_collider.get_rigid_body());
	sim->add_entity(&lPaddle);

	//ball entity
	ga_entity test_1_box;
	test_1_box.translate({ 0.0f, 0.0f, 0.0f });
	ga_ball_component model_b(&test_1_box, "data/textures/rpi.png");
	ga_oobb test_1_oobb;
	test_1_oobb._half_vectors[0] = ga_vec3f::x_vector().scale_result(0.3f);
	test_1_oobb._half_vectors[1] = ga_vec3f::y_vector().scale_result(0.3f);
	test_1_oobb._half_vectors[2] = ga_vec3f::z_vector().scale_result(0.3f);

	ga_physics_component test_1_collider(&test_1_box, &test_1_oobb, 1.0f);
	test_1_collider.get_rigid_body()->make_weightless();
	
	world->add_rigid_body(test_1_collider.get_rigid_body());
	sim->add_entity(&test_1_box);

	test_1_collider.get_rigid_body()->add_linear_velocity({ 10.0f, 0.0f, 0.0f });

	//floor collider
	ga_entity floor;
	ga_plane floor_plane;
	floor_plane._point = { 0.0f, 0.0f, 0.0f };
	floor_plane._normal = { 0.0f, 1.0f, 0.0f };
	floor.translate({ 0.0f, -7.0f, 0.0f });
	ga_mat4f tempscale3 = floor.get_transform();
	tempscale3.nonuniform_scale({ 1.3f, 1.0f, 0.1f });
	floor.set_transform(tempscale3);
	ga_physics_component floor_collider(&floor, &floor_plane, 0.0f);
	floor_collider.get_rigid_body()->make_static();
	//world->add_rigid_body(floor_collider.get_rigid_body());
	//sim->add_entity(&floor);

	//ceiling collider
	ga_entity ceil;
	ga_plane ceil_plane;
	ceil_plane._point = { 0.0f, 0.0f, 0.0f };
	ceil_plane._normal = { 0.0f, 1.0f, 0.0f };
	ceil.translate({ 0.0f, 7.0f, 0.0f });
	ga_mat4f tempscale4 = ceil.get_transform();
	tempscale4.nonuniform_scale({ 1.3f, 1.0f, 0.1f });
	ceil.set_transform(tempscale4);
	ga_physics_component ceil_collider(&ceil, &ceil_plane, 0.0f);
	ceil_collider.get_rigid_body()->make_static();
	//world->add_rigid_body(ceil_collider.get_rigid_body());
	//sim->add_entity(&ceil);

	// Main loop:
	while (true)
	{
		// We pass frame state through the 3 phases using a params object.
		ga_frame_params params;

		// Gather user input and current time.
		if (!input->update(&params))
		{
			break;
		}

		// Update the camera.
		camera->update(&params);

		// Run gameplay.
		sim->update(&params);

		// Step the physics world.
		world->step(&params);

		// Perform the late update.
		sim->late_update(&params);

		// Draw to screen.
		output->update(&params);
	}

	//world->remove_rigid_body(floor_collider.get_rigid_body());
	world->remove_rigid_body(rPaddle_collider.get_rigid_body());
	//world->remove_rigid_body(ceil_collider.get_rigid_body());

	delete output;
	delete sim;
	delete input;
	delete camera;

	ga_job::shutdown();

	return 0;
}

char g_root_path[256];
static void set_root_path(const char* exepath)
{
#if defined(GA_MSVC)
	strcpy_s(g_root_path, sizeof(g_root_path), exepath);

	// Strip the executable file name off the end of the path:
	char* slash = strrchr(g_root_path, '\\');
	if (!slash)
	{
		slash = strrchr(g_root_path, '/');
	}
	if (slash)
	{
		slash[1] = '\0';
	}
#elif defined(GA_MINGW)
	char* cwd;
	char buf[PATH_MAX + 1];
	cwd = getcwd(buf, PATH_MAX + 1);
	strcpy_s(g_root_path, sizeof(g_root_path), cwd);

	g_root_path[strlen(cwd)] = '/';
	g_root_path[strlen(cwd) + 1] = '\0';
#endif
}
