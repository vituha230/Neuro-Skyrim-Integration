#pragma once

//#include "DialogueProcessor.hpp"


namespace TrainingProcessor {


	void processor(float dtime);
	std::pair<bool, std::string> set_training_choice(int choice);

	bool quit_menu();
}


