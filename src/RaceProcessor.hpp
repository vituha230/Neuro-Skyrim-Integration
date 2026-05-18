#pragma once

#include "DialogueProcessor.hpp"


namespace RaceProcessor {


	void processor(float dtime);
	std::pair<bool, std::string> set_character_choice(int choice);
	std::pair<bool, std::string> set_character_name(std::string choice);

	std::pair<bool, std::string> set_generic_slider_choice(int choice);

	int get_category_shift(int page);
	int get_max_generic_slider();
}


