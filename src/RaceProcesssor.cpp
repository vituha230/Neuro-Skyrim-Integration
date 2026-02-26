



//TODO: make lock move faster (need to send inputs more frequently - DIDNT WORK???)
//TODO: lock level info 
//TODO: it looks a bit like shit.. need to smooth it somehow

#include "InputActions.hpp"
#include "RaceProcessor.hpp"
#include "main.hpp"


namespace RaceProcessor {



	bool in_racemenu = false;
	int last_cursor_move = 0;
	bool missing_item_detected = false;
	bool up_done = false;
	bool down_done = false;
	bool fill_items_one_direction_done = false;
	int last_scroll = -1;
	bool scroll_stuck = false;
	float scroll_stuck_time = 0.0f;
	bool filling_items = false;
	bool items_list_valid = false;
	//bool item_choice_request_sent = false;
	//bool item_choice_valid = false;
	//int item_choice = -1;


	float confirm_race_time = 0.0f;
	bool confirm_race = false;


	bool race_defined = false;
	bool race_request_sent = false;
	bool race_choice_valid = false;
	int race_choice = -1;

	bool gender_defined = false;
	bool gender_request_sent = false;
	bool gender_choice_valid = false;
	int gender_choice = -1;

	bool preset_defined = false;
	bool preset_request_sent = false;
	bool preset_choice_valid = false;
	int preset_choice = -1;

	bool name_defined = false;
	bool name_request_sent = false;
	bool name_choice_valid = false;
	std::string name_choice = "";

	bool name_field_called = false;
	bool rolled_over = false;

	std::string typed_string = "";

	struct item_data {
		std::string name;
		std::string description;
	};


	std::map<int, item_data> items_list{};

	enum way_to_fill {
		down,
		up,
		downup,
		updown
	};

	way_to_fill way{};









	/*
	int get_item_scroll_position()
	{
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iScrollPosition"))
						return var1.GetNumber();
		return -1;
	}


	int get_item_max_scroll_position()
	{
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iMaxScrollPosition"))
						return var1.GetNumber();
		return -1;
	}
	*/


	int get_item_selected_index()
	{
		int result = -1;
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::RaceSexMenu>();
		RE::GFxValue var1;

		if (ui && menu && ui->IsMenuOpen(RE::RaceSexMenu::MENU_NAME))
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList1.iSelectedIndex"))
					if (!var1.IsNull() && var1.IsNumber())
						result = var1.GetNumber();

		return result;
	}


	void move_to_item(int index)
	{
		int selected_item = get_item_selected_index();

		//if (is_inside_of_category())
		if (true)
		{
			if (selected_item > index)
				cursor_up();

			if (selected_item < index)
				cursor_down();
		}
	}



	void update_items_list()
	{
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList1"))
					{
						std::string path_root = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList1.TopHalf.";
						for (int i = 0; i < 10; i++)
						{
							//std::string full_path = path_root + "Entry" + std::to_string(i);
							//if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))

							MenuOption result{};

							result.id = i;
							std::string name = "";
							if (menu->uiMovie->GetVariable(&var1, ("_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList1.EntriesA." + std::to_string(result.id) + ".text").c_str()))
								if (!var1.IsNull() && var1.IsString())
									name = var1.GetString();

							std::string description = "";

							//_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList1.EntriesA		[10].1.raceDescription
							if (menu->uiMovie->GetVariable(&var1, ("_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList1.EntriesA." + std::to_string(result.id) + ".raceDescription").c_str()))
								if (!var1.IsNull() && var1.IsString())
									description = var1.GetString();


							if (name != "")
							{
								if (result.id == get_item_selected_index())
								{
									item_data data{};
									data.name = name;
									data.description = description;
									items_list.insert({ result.id, data });

								}

							}

						}
					}
	}




	void fill_items_list_one_direction(bool direction_down, float dtime)
	{
		int max_scroll = 9;
		int scroll = get_item_selected_index();

		if (scroll == last_scroll && !scroll_stuck)
			scroll_stuck_time += dtime;
		else
			scroll_stuck_time = 0.0f;

		if (scroll_stuck_time > 1.0f)
		{
			scroll_stuck = true;
			scroll_stuck_time = 0.0f;
			last_scroll = -1;
		}
		last_scroll = scroll;

		if (direction_down)
		{
			if (scroll < max_scroll)
			{
				update_items_list(); //do this on every scroll
				if (scroll_stuck)
					perk_down();
				else
					cursor_down();
			}
			else
			{
				update_items_list(); //do this on every scroll
				if (scroll_stuck)
					filling_items = false; //restart fill
				else
					fill_items_one_direction_done = true;
			}
		}
		else
			if (scroll > 0)
			{
				update_items_list(); //do this on every scroll
				if (scroll_stuck)
					perk_up();
				else
					cursor_up();
			}
			else
			{
				update_items_list(); //do this on every scroll
				if (scroll_stuck)
					filling_items = false; //restart fill
				else
					fill_items_one_direction_done = true;
			}
	}


	void fill_items_list(float dtime)
	{
		switch (way) {
		case (way_to_fill::down):
		{
			if (fill_items_one_direction_done)
				items_list_valid = true;
			else
				fill_items_list_one_direction(true, dtime);

			break;
		}

		case (way_to_fill::up):
		{
			if (fill_items_one_direction_done)
				items_list_valid = true;
			else
				fill_items_list_one_direction(false, dtime);

			break;
		}

		case (way_to_fill::downup):
		{
			if (down_done)
				if (fill_items_one_direction_done)
					items_list_valid = true;
				else
					fill_items_list_one_direction(false, dtime);
			else
				if (fill_items_one_direction_done)
				{
					down_done = true;
					fill_items_one_direction_done = false;
				}
				else
					fill_items_list_one_direction(true, dtime);

			break;
		}

		case (way_to_fill::updown):
		{
			if (up_done)
				if (fill_items_one_direction_done)
					items_list_valid = true;
				else
					fill_items_list_one_direction(true, dtime);
			else
				if (fill_items_one_direction_done)
				{
					up_done = true;
					fill_items_one_direction_done = false;
				}
				else
					fill_items_list_one_direction(false, dtime);
			break;
		}
		}
	}



	void setup_fill_item_list()
	{
		int min_scroll = 0;
		int max_scroll = 9;
		int scroll = get_item_selected_index();

		last_cursor_move = 0;
		missing_item_detected = false;

		up_done = false;
		down_done = false;
		fill_items_one_direction_done = false;

		if (scroll <= min_scroll)
			way = way_to_fill::down;
		else
			if (scroll >= max_scroll)
				way = way_to_fill::up;
			else
				if (scroll <= (max_scroll - min_scroll) / 2)
					way = way_to_fill::updown;
				else
					way = way_to_fill::downup;
	}






	void reset_menu()
	{

		in_racemenu = false;
		last_cursor_move = 0;
		missing_item_detected = false;
		up_done = false;
		down_done = false;
		fill_items_one_direction_done = false;
		last_scroll = -1;
		scroll_stuck = false;
		scroll_stuck_time = 0.0f;
		filling_items = false;
		items_list_valid = false;



		confirm_race_time = 0.0f;
		confirm_race = false;


		race_defined = false;
		race_request_sent = false;
		race_choice_valid = false;
		race_choice = -1;

		gender_defined = false;
		gender_request_sent = false;
		gender_choice_valid = false;
		gender_choice = -1;

		preset_defined = false;
		preset_request_sent = false;
		preset_choice_valid = false;
		preset_choice = -1;

		name_defined = false;
		name_request_sent = false;
		name_choice_valid = false;
		name_choice = "";

		name_field_called = false;
		rolled_over = false;

		items_list.clear();


		typed_string = "";
	}



	std::pair<bool, std::string> set_race_choice(int choice)
	{
		std::pair<bool, std::string> result{};

		if (choice >= 0 && choice < 10)
		{
			race_choice = choice;
			race_choice_valid = true;

			result.first = true;
			result.second = "[Processing...]";
		}
		else
		{
			result.first = false;
			result.second = "Invalid choice";
		}


		return result;
	}


	std::pair<bool, std::string> set_gender_choice(int choice)
	{
		std::pair<bool, std::string> result{};


		if (choice >= 0 && choice < 2)
		{
			gender_choice = choice;
			gender_choice_valid = true;

			result.first = true;
			result.second = "[Processing...]";
		}
		else
		{
			result.first = false;
			result.second = "Invalid choice";
		}

		return result;
	}


	std::pair<bool, std::string> set_preset_choice(int choice)
	{
		std::pair<bool, std::string> result{};


		if (choice >= 0 && choice < 10)
		{
			preset_choice = choice;
			preset_choice_valid = true;

			result.first = true;
			result.second = "[Processing...]";
		}
		else
		{
			result.first = false;
			result.second = "Invalid choice";
		}


		return result;
	}





	int get_selected_category()
	{
		int result = -1;

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.iSelectedIndex"))
						if (!var1.IsNull() && var1.IsNumber())
						{
							result = var1.GetNumber();
						}

		if (result == 1)
		{
			if (ui)
				if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
					if (menu->uiMovie)
						if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList2.iSelectedIndex"))
							if (!var1.IsNull() && var1.IsNumber())
							{
								result = result + var1.GetNumber();
							}
		}


		return result;
	}



	bool is_inside_of_category(int category)
	{
		bool result = false;

		//_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.EntriesA[8].0.text = Race
		//_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text = Race
		//_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.List_mc.SelectedEntry.textField.text = Race

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		switch (category) {
		case (0):
		{

			if (ui)
				if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
					if (menu->uiMovie)
						if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text"))
							if (!var1.IsNull() && var1.IsString())
							{
								std::string var1_string = var1.GetString();
								return var1_string == "Race";
							}
			break;
		}

		case (1):
		{
			bool is_in_body = false;
			if (ui)
				if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
					if (menu->uiMovie)
						if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text"))
							if (!var1.IsNull() && var1.IsString())
							{
								std::string var1_string = var1.GetString();
								is_in_body = var1_string == "Body";
							}

			if (is_in_body)
			{
				if (ui)
					if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
						if (menu->uiMovie)
							if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList2.iSelectedIndex"))
								if (!var1.IsNull() && var1.IsNumber())
								{
									int var1_number = var1.GetNumber();

									result = var1_number == 0;
								}
			}

			break;
		}

		case (2):
		{
			bool is_in_body = false;
			if (ui)
				if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
					if (menu->uiMovie)
						if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text"))
							if (!var1.IsNull() && var1.IsString())
							{
								std::string var1_string = var1.GetString();
								is_in_body = true;
							}

			if (is_in_body)
			{
				if (ui)
					if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
						if (menu->uiMovie)
							if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList2.iSelectedIndex"))
								if (!var1.IsNull() && var1.IsNumber())
								{
									int var1_number = var1.GetNumber();

									result = var1_number == 1;
								}
			}

			break;
		}

		case (3):
		{

			break;
		}

		}



		return result;
	}



	void categories_left()
	{
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
			{
				//menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.ButtonHintLeftInstance.onRollOver", nullptr, nullptr, 0);
				//menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.ButtonHintLeftInstance.onPress", nullptr, nullptr, 0);

				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.LeftClickInstance.onRollOver", nullptr, nullptr, 0);
				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.LeftClickInstance.onPress", nullptr, nullptr, 0);
			}
				
	}




	bool wtf = false;

	bool confirm_race_and_go_to_body(float dtime)
	{
		bool result = false;

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
			{
				//menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.ButtonHintRightInstance.onRollOver", nullptr, nullptr, 0);
				//menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.ButtonHintRightInstance.onPress", nullptr, nullptr, 0);

				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.RightClickInstance.onRollOver", nullptr, nullptr, 0);
				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.RightClickInstance.onPress", nullptr, nullptr, 0);

				if (confirm_race_time > 2.0f)// && wtf)
				{
					result = true;
					confirm_noblock(); //???????????????????????????????????????????????
				}
				else
					confirm_race_time += dtime;

			}

		return result;
	}

	/*
	void categories_right()
	{
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
			{
				//menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.ButtonHintRightInstance.onRollOver", nullptr, nullptr, 0);
				//menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.ButtonHintRightInstance.onPress", nullptr, nullptr, 0);

				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.RightClickInstance.onRollOver", nullptr, nullptr, 0);
				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.RightClickInstance.onPress", nullptr, nullptr, 0);
				if (menu->unk188 == 7)
					if (wtf)
					{
						wtf = false;
						menu->unk188 = 0;
					}
						
			}
	}
	*/

	int get_selected_slider()
	{
		int result = -1;

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList2.iSelectedIndex"))
						if (!var1.IsNull() && var1.IsNumber())
						{
							result = var1.GetNumber();
						}

		return result;
	}



	void move_to_category(int category)
	{
		int selected_category = get_selected_category();

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::RaceSexMenu>();

		if (menu)
		{
			if (category == 0)
			{
				if (selected_category < 0)
					;// categories_right();
				if (selected_category > 0)
					categories_left();

				//menu->unk188 = 0;
			}
			else
			{
				if (category == 1)
				{
					if (selected_category < 1)
						;// categories_right();
					else
						if (selected_category > 2)
							categories_left();
				
						//if (selected_category < 1 || selected_category > 2)
						//	menu->unk188 = 1;
						else
						{
							int selected_subcategory = get_selected_slider();

							if (selected_subcategory > 0)
								cursor_down();
							if (selected_subcategory < 0)
								cursor_up();
						}
				}
				else
					if (category == 2)
					{
						if (selected_category < 1)
							;// categories_right();
						else
							if (selected_category > 2)
								categories_left();

						//if (selected_category < 1 || selected_category > 2)
						//	menu->unk188 = 1;
							else
							{
								int selected_subcategory = get_selected_slider();

								if (selected_subcategory > 1)
									cursor_up();
								if (selected_subcategory < 1)
									cursor_down();
							}
					}
			}
		}
		


	}



	std::string get_force_message(int category)
	{
		std::string result = "";

		switch (category) {
		case (0):result = "You are creating your character. Select your character's race from provided options"; break;
		case (1):result = "You are creating your character. Select your character's gender from provided options"; break;
		case (2):result = "You are creating your character. Select your character's look preset from provided options"; break;
		case (3):result = "You are creating your character. Give your character's name"; break;
		}

		return result;
	}




	int get_max_preset_slider()
	{
		int result = -1;

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
				{
					std::string path = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA.1.sliderMax";
					if (menu->uiMovie->GetVariable(&var1, path.c_str()))
						if (!var1.IsNull() && var1.IsNumber())
						{
							result = var1.GetNumber();
						}
				}


		return result;
	}


	std::vector<MenuOption> get_options(int category)
	{
		std::vector<MenuOption> result{};

		switch (category) {
		case (0):
		{

			for (std::pair<int, item_data> item : items_list)
			{
				std::string name_text = "";
				if (item.second.name != "")
					name_text = item.second.name;
				std::string description_text = "";
				if (item.second.description != "")
					description_text = ". Description: " + item.second.description;


				MenuOption option = { item.first, name_text + description_text };
				result.push_back(option);
			}
			break;
		}

		case (1):
		{
			MenuOption option = { 0, "Male" };
			result.push_back(option);
			option = { 1, "Female" };
			result.push_back(option);
			break;
		}

		case (2):
		{
			MenuOption option = { 0, "Preset1" }; 

			for (int i = 0; i < get_max_preset_slider() + 1; i++)
			{
				option = { i, "Preset" + std::to_string(i)};
				result.push_back(option);
			}

			break;
		}



		}

		return result;
	}



	std::pair<bool, std::string> set_character_choice(int choice)
	{
		std::pair<bool, std::string> result{};

		if (!race_defined)
		{
			return set_race_choice(choice);
		}

		if (!gender_defined)
		{
			return set_gender_choice(choice);
		}

		if (!preset_defined)
		{
			return set_preset_choice(choice);
		}


		result.first = false;
		result.second = "[Error]";
		return result;
	}


	std::pair<bool, std::string> set_character_name(std::string choice)
	{
		std::pair<bool, std::string> result{};


		name_choice = choice;
		name_choice_valid = true;
		result.first = true;
		result.second = "[Processing...]";

		return result;
	}



	int get_gender_selected_index()
	{
		int result = -1;

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA.0.position"))
						if (!var1.IsNull() && var1.IsNumber())
						{
							result = var1.GetNumber();
						}

		return result;
	}


	int get_preset_selected_index()
	{
		int result = -1;

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA.1.position"))
						if (!var1.IsNull() && var1.IsNumber())
						{
							result = var1.GetNumber();
						}

		return result;
	}


	void move_gender_slider(int gender_choice)
	{
		int selected_gender_index = get_gender_selected_index();

		if (selected_gender_index > gender_choice)
			left();
		if (selected_gender_index < gender_choice)
			right();
	}


	void move_preset_slider(int preset_choice)
	{
		int selected_preset_index = get_preset_selected_index();

		if (selected_preset_index > preset_choice)
			left();
		if (selected_preset_index < preset_choice)
			right();
	}



	void send_char_to_text_input_window(char in_char)
	{
		RE::GFxKeyEvent* my_event = new RE::GFxKeyEvent; //im not sure if its destroyed after use but i dont really care since this is supposed to be unique event in playthrough

		my_event->type = RE::GFxKeyEvent::EventType::kCharEvent;
		my_event->keyCode = (RE::GFxKey::Code)in_char;//RE::GFxKey::Code::kKP_Multiply;
		my_event->wCharCode = 13;

		const auto uiMessageQueue = RE::UIMessageQueue::GetSingleton();
		if (uiMessageQueue) {
			const auto msgData = RE::UIMessageDataFactory::Create<RE::BSUIScaleformData>();
			msgData->scaleformEvent = my_event;
			uiMessageQueue->AddMessage(RE::RaceSexMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kScaleformEvent, msgData);
		}
	}



	float pause_timer = 0.0f;
	float racemenu_processor_timer = 0.0f;



	bool key_up = false;


	void processor(float dtime)
	{

		RE::UI* ui = RE::UI::GetSingleton();

		in_racemenu = ui->IsMenuOpen(RE::RaceSexMenu::MENU_NAME);

		auto menu = ui->GetMenu<RE::RaceSexMenu>();

		
		
		if (in_racemenu)
		{
			pause_timer += dtime;
		}


		if (confirm_race)
		{
			if (confirm_race_and_go_to_body(dtime))
			{
				confirm_race = false;
				race_defined = true;
			}
		}



		if (racemenu_processor_timer > 0.01f)
		{
			racemenu_processor_timer = 0.0f;

			if (in_racemenu && pause_timer > 2.0f)
			{
				if (!race_defined)
				{
					if (!is_inside_of_category(0))
						move_to_category(0);
					else
					{
						if (!items_list_valid)
						{
							if (!filling_items)
							{
								items_list.clear();
								setup_fill_item_list();
								filling_items = true;
							}
							else
							{
								fill_items_list(0.02f);
								set_universal_block(1.5f);
							}
								
						}
						else
						{
							if (!race_request_sent)
							{
								
								if (force_choice(get_options(0), get_force_message(0), force_type::race))
									race_request_sent = true;
							}
							else
							{
								if (race_choice_valid)
								{
									int selected_index = get_item_selected_index();
									if ((selected_index != race_choice))
									{
										if (!missing_item_detected)
											move_to_item(race_choice);
										else
											reset_menu();
									}
									else
									{
										confirm_race = true;
									}
								}
							}
						}
					}
					
				}
				else
				{
					if (!gender_defined)
					{
						if (!is_inside_of_category(1))
							move_to_category(1);
						else
						{

							if (!gender_request_sent)
							{
								
								if (force_choice(get_options(1), get_force_message(1), force_type::race))
									gender_request_sent = true;
							}
							else
							{
								if (gender_choice_valid)
								{
									int selected_index = get_gender_selected_index();
									if ((selected_index != gender_choice))
									{
										if (!missing_item_detected)
											move_gender_slider(gender_choice);
										else
											reset_menu();
									}
									else
									{

										gender_defined = true;
										set_universal_block(1.0f);

									}
								}
							}
						}
					}
					else
					{
						if (!preset_defined)
						{
							if (!is_inside_of_category(2))
								move_to_category(2);
							else
							{

								if (!preset_request_sent)
								{
									
									if (force_choice(get_options(2), get_force_message(2), force_type::race))
										preset_request_sent = true;
								}
								else
								{
									if (preset_choice_valid)
									{
										int selected_index = get_preset_selected_index();
										if ((selected_index != preset_choice))
										{
											if (!missing_item_detected)
												move_preset_slider(preset_choice);
											else
												reset_menu();
										}
										else
										{

											preset_defined = true;
											set_universal_block(1.0f);

										}
									}
								}
							}
						}
						else
						{
							if (!name_defined)
							{
								if (!name_field_called)
								{
									if (auto menu_confirm = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm) 
									{
										RandomMessageBoxProcessor::set_messagebox_handeled();
										if (!rolled_over)
										{
											menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
											rolled_over = true;
											set_universal_block(1.0f);
										}
										else
										{
											rolled_over = false;
											menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0);
											set_universal_block(0.5f);
											name_field_called = true;
										}
									}
									else
									{
										confirm_craft();
										set_universal_block(0.3f);
									}

								}
								else
								{
									if (!name_request_sent)
									{
										
										if (force_choice({}, get_force_message(3), force_type::character_name))
										{
											name_request_sent = true;
										}
										//set_character_name("test_name_123");
									}
									else
									{
										

										if (name_choice_valid)
										{
											if (name_choice.length() > 0)
											{

												set_universal_block(0.3f);
												send_char_to_text_input_window(name_choice.at(0));
												name_choice = name_choice.substr(1, name_choice.length() - 1);
												
											}
											else
											{

												name_defined = true;
												//menu->ChangeName(typed_string.c_str()); //this jus closes menu (and probably sets the name) but it wasnt typed

												menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField.AcceptButton.onPress", nullptr, nullptr, 0);
												menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField.AcceptButton.onRelease", nullptr, nullptr, 0);

												set_universal_block(1.0f);

											}
										}
									}
								}

							}
						}
					}
				}

			}
			else
			{
				reset_menu();
			}

		}
		else
			racemenu_processor_timer += dtime;
	}


}