#pragma once

#include "DialogueProcessor.hpp"



namespace MapProcessor {



	void processor(float dtime);

	bool quit_menu();

	std::pair<bool, std::string> set_location_choice(int id);

	std::pair<bool, std::string> open_menu();

	std::pair<bool, std::string> set_undiscovered_choice(int choice);

	void reset_menu2();

	bool abandon_all_messages();

}

