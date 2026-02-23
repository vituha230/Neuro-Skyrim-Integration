#pragma once

#include "DialogueProcessor.hpp"


namespace BookProcessor {


	void processor(float dtime);

	bool quit_menu();

	std::pair<bool, std::string> set_book_choice(int choice);

}


