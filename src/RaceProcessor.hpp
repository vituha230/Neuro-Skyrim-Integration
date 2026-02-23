#pragma once

#include "DialogueProcessor.hpp"


namespace RaceProcessor {


	void processor(float dtime);
	std::pair<bool, std::string> set_character_choice(int choice);
	std::pair<bool, std::string> set_character_name(std::string choice);


}


