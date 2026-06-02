#pragma once

#include "DialogueProcessor.hpp"


namespace RaceProcessor {


	void processor(float dtime);
	std::pair<bool, std::string> set_character_choice(int choice);
	std::pair<bool, std::string> set_character_name(std::string choice);

	std::pair<bool, std::string> set_generic_slider_choice(int choice);

	int get_category_shift(int page);
	int get_max_generic_slider();
	int get_generic_slider_selected_index();

	bool has_sex();

	std::pair<bool, std::string> confirm_change_character(int confirm);
	std::pair<bool, std::string> set_change_categories_choice(std::vector<int> ids);

	bool race_menu_wants_to_poke();
}


