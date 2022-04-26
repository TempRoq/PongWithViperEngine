#pragma once
#include "ga_pong_manager.h"

ga_pong_manager::ga_pong_manager(class ga_entity* ent, ga_entity* left, ga_entity* right, ga_entity* _ball, int maxPoints){

	ga_component(ent);
	left_paddle = left;
	right_paddle = right;
	ball = _ball;
	
	if (maxPoints <= 0) {
		points_to_win = 1;
	}
	else {
		points_to_win = maxPoints;
	}
	left_score = 0;
	right_score = 0;
	
}

ga_pong_manager::~ga_pong_manager() {
	~ga_component();
}

void ga_pong_manager::ScorePoint(bool left) {
	if (left) {
		left_score++;
		if (left_score == points_to_win) {
			end_game();
		}
	}
	else {
		right_score++;
		if (right_score == points_to_win) {
			end_game();
		}
	}
}

void ga_pong_manager::end_game()
{if (left_score == points_to_win) {
		//Left Wins;
	}
	else {
		//Right Wins;
	}
}



void ga_pong_manager::reset_ball() {
	ga_vec3f position;
	position.x = 0;
	position.y = 0;
	position.z = 0;
	ball->translate(position);
}