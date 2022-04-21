#include "ga_pong_manager.h"

ga_pong_manager::ga_pong_manager(class ga_entity* ent, ga_entity* left, ga_entity* right, ga_entity* _ball, int maxPoints){

	ga_component(ent);
	left_paddle = left;
	right_paddle = right;
	ball = _ball;
	
	if (maxPoints <= 0) {
		pointsToWin = 1;
	}
	else {
		pointsToWin = maxPoints;
	}
	leftScore = 0;
	rightScore = 0;
	
}

ga_pong_manager::~ga_pong_manager() {
	~ga_component();
}

ga_pong_manager::ScorePoint(bool left) {
	if (left) {
		leftScore++;
		if (leftScore == pointsToWin) {
			EndGame();
		}
	}
	else {
		rightScore++;
		if (rightScore == pointsToWin) {
			EndGame();
		}
	}
}

ga_pong_manager::EndGame() {
	if (leftScore == pointsToWin) {
		//Left Wins;
	}
	else {
		//Right Wins;
	}
}

ga_pong_manager::MoveBallToPoint(ga_vec3f loc) {
	ball->translate(loc);
}