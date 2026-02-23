#pragma once

#include "DialogueProcessor.hpp"


namespace LockpickProcessor {

	bool quit_lockpicking();
	void processor(float dtime);
	std::pair<bool, std::string> set_angle_choice(int angle);

}

