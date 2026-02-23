#pragma once

#include "DialogueProcessor.hpp"


namespace EnchantProcessor {

	enum enchant_type
	{
		enchant,
		disenchant
	};


	bool quit_menu();
	void processor(float dtime);

	std::pair<bool, std::string> set_item_choice(int id);
	std::pair<bool, std::string> set_enchant_type(int in_type);
	std::pair<bool, std::string> set_slider_choice(int pos);


};









