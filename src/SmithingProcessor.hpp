#pragma once

#include "DialogueProcessor.hpp"


namespace SmithingProcessor {


	bool quit_menu();
	void processor(float dtime);
	bool reset_smithing();


	std::pair<bool, std::string> set_item_choice(int id);
	std::pair<bool, std::string> set_category_choice(int id);


};









