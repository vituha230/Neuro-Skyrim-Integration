#pragma once

#include "DialogueProcessor.hpp"


namespace PerksProcessor {


	void processor(float dtime);
	std::pair<bool, std::string> choose_skill_tree(int tree_id);
	std::pair<bool, std::string> choose_perk(int perk_id);
	bool quit_perk_menu();
	//std::string get_all_perk_options();
	std::pair<bool, std::string> choose_perk_confirm(int choice);


}






