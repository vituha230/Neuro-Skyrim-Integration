#pragma once

#include "DialogueProcessor.hpp"


namespace SleepWaitProcessor {


	void processor(float dtime);
	std::pair<bool, std::string> set_sleepwait_choice(int choice);

	std::pair<bool, std::string> call_wait_menu();

	void saveload_reset();


	bool is_sleeping();



	bool quit_menu();


}


