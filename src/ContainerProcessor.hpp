#pragma once

#include "DialogueProcessor.hpp"


namespace ContainerProcessor {


	void processor(float dtime);
	bool quit_menu();
	std::pair<bool, std::string> set_item_choice(int id);
	std::pair<bool, std::string> set_item_choice_array(std::vector<int> ids);

	void reset_pickpocketing();


};









