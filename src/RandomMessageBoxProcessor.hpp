#pragma once


namespace RandomMessageBoxProcessor {

	void processor(float dtime);
	void reset_menu();


	std::pair<bool, std::string> set_message_box_choice(int choice);
	void set_messagebox_handeled();
	
}