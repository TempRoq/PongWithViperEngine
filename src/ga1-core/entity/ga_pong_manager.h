#pragma once

#include "entity/ga_component.h"
#include "math/ga_vec3f"
#include "math/ga_mat4f"
class ga_pong_manager : public ga_component {

public:
	ga_pong_manager(class ga_entity* ent, ga_entity* left, ga_entity* right, ga_entity* _ball, int maxPoints );
	~ga_pong_manager();
	void ScorePoint(bool left);
	void EndGame();
	void MoveBallToPoint(ga_vec3f);
	virtual void update(struct ga_frame_params* params) override;



private:

	ga_entity* left_paddle;
	ga_entity* right_paddle;
	ga_entity* ball;
	int leftScore;
	int rightScore;
	int pointsToWin;
};
