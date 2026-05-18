



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

	bool generic_slider_defined = false;
	bool generic_slider_request_sent = false;
	bool generic_slider_choice_valid = false;
	int generic_slider_choice = -1;


	bool name_defined = false;
	bool name_request_sent = false;
	bool name_choice_valid = false;
	std::string name_choice = "";

	bool name_field_called = false;
	bool rolled_over = false;


	float last_pause_time = 0.0f;


	int current_page = -1;
	int current_slider = -1;


	void reset_category()
	{
		current_page = 0;
		current_slider = 0;
	}


	bool has_facial_hair()
	{
		bool result = false;
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::RaceSexMenu>();
		RE::GFxValue var1;

		if (ui && menu && ui->IsMenuOpen(RE::RaceSexMenu::MENU_NAME))
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList2.EntriesA.46.sliderMax"))
					if (!var1.IsNull() && var1.IsNumber())
						result = true;

		return result;
	}

	void increment_category()
	{
		if (current_page == 0)
		{
			current_page = 1;
			current_slider = 0;
			return;
		}

		if (current_page == 1)
		{
			if (current_slider < 3)
			{
				current_slider++;
			}
			else
			{
				current_page++;
				current_slider = 0;
			}
			return;
		}


		if (current_page == 2)
		{
			if (current_slider < 5)
			{
				current_slider++;
			}
			else
			{
				current_page++;
				current_slider = 0;
			}
			return;
		}


		if (current_page == 3)
		{
			if (current_slider < 14)
			{
				current_slider++;
			}
			else
			{
				current_page++;
				current_slider = 0;
			}
			return;
		}



		if (current_page == 4)
		{
			if (current_slider < 7)
			{
				current_slider++;
			}
			else
			{
				current_page++;
				current_slider = 0;
			}
			return;
		}



		if (current_page == 5)
		{
			if (current_slider < 3)
			{
				current_slider++;
			}
			else
			{
				current_page++;
				current_slider = 0;
			}
			return;
		}


		if (current_page == 6)
		{
			if (current_slider < 6)
			{
				current_slider++;
			}
			else
			{
				current_page++;
				current_slider = 0;
			}
			return;
		}


		if (current_page == 7)
		{
			if (current_slider < (1 + has_facial_hair()))
			{
				current_slider++;
			}
			else
			{
				current_page++;
				current_slider = 0;
			}
			return;
		}

	}


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
		last_pause_time = 0.0f;
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

		reset_category();
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




	std::pair<bool, std::string> set_generic_slider_choice(int choice)
	{
		std::pair<bool, std::string> result{};


		if (choice >= 0 && choice < get_max_generic_slider() + 1)
		{
			generic_slider_choice = choice;
			generic_slider_choice_valid = true;

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



	int get_selected_page()
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

		return result;
	}



	bool is_inside_of_category(int page, int category)
	{
		bool result = false;

		//_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.EntriesA[8].0.text = Race
		//_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text = Race
		//_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.List_mc.SelectedEntry.textField.text = Race

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();


		int shift = get_category_shift(page);

		switch (page) {
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

									result = var1_number == category + shift;
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
								is_in_body = var1_string == "Head";
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

									result = var1_number == category + shift;
								}
			}

			break;
		}

		case (3):
		{
			bool is_in_body = false;
			if (ui)
				if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
					if (menu->uiMovie)
						if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text"))
							if (!var1.IsNull() && var1.IsString())
							{
								std::string var1_string = var1.GetString();
								is_in_body = var1_string == "Face";
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

									result = var1_number == category + shift;
								}
			}

			break;
		}


		case (4):
		{
			bool is_in_body = false;
			if (ui)
				if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
					if (menu->uiMovie)
						if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text"))
							if (!var1.IsNull() && var1.IsString())
							{
								std::string var1_string = var1.GetString();
								is_in_body = var1_string == "Eyes";
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

									result = var1_number == category + shift;
								}
			}

			break;
		}


		case (5):
		{
			bool is_in_body = false;
			if (ui)
				if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
					if (menu->uiMovie)
						if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text"))
							if (!var1.IsNull() && var1.IsString())
							{
								std::string var1_string = var1.GetString();
								is_in_body = var1_string == "Brow";
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

									result = var1_number == category + shift;
								}
			}

			break;
		}

		case (6):
		{
			bool is_in_body = false;
			if (ui)
				if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
					if (menu->uiMovie)
						if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text"))
							if (!var1.IsNull() && var1.IsString())
							{
								std::string var1_string = var1.GetString();
								is_in_body = var1_string == "Mouth";
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

									result = var1_number == category + shift;
								}
			}

			break;
		}



		case (7):
		{
			bool is_in_body = false;
			if (ui)
				if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
					if (menu->uiMovie)
						if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._CategoriesList.SelectedEntry.textField.text"))
							if (!var1.IsNull() && var1.IsString())
							{
								std::string var1_string = var1.GetString();
								is_in_body = var1_string == "Hair";
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

									result = var1_number == category + shift;
								}
			}

			break;
		}


		case (8):
		{
			//name
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



	void page_right()
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
				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.RightClickInstance.onRelease", nullptr, nullptr, 0);
			}

	}


	void page_left()
	{
		bool result = false;

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
			{
				//menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.ButtonHintRightInstance.onRollOver", nullptr, nullptr, 0);
				//menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.CagetoryLockBaseInstance.CategoryInstance.ButtonHintRightInstance.onPress", nullptr, nullptr, 0);

				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.LeftClickInstance.onRollOver", nullptr, nullptr, 0);
				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.LeftClickInstance.onPress", nullptr, nullptr, 0);
				menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.LeftClickInstance.onRelease", nullptr, nullptr, 0);
			}

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


	int get_category_shift(int page)
	{
		switch (page)
		{
		case (0): return 0; //race
		case (1): return 0; //body
		case (2): return 4; //head
		case (3): return 24 + has_facial_hair(); //face
		case (4): return 12 + has_facial_hair(); //eyes
		case (5): return 20 + has_facial_hair(); //brow
		case (6): return 39 + has_facial_hair(); //mouth
		case (7): return 10; //hair
		case (8): return 0;
		}

		return -1;
	}



	int get_selected_slider()
	{
		int result = -1;

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();


		int shift = get_category_shift(get_selected_page());

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._SubList2.iSelectedIndex"))
						if (!var1.IsNull() && var1.IsNumber())
						{
							result = var1.GetNumber() - shift;
						}

		return result;
	}



	void move_to_category(int page, int category)
	{
		int selected_page = get_selected_page();
		int selected_slider = get_selected_slider();


		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::RaceSexMenu>();

		if (menu)
		{
			if (selected_page > page)
				page_left();

			if (selected_page < page)
				page_right();

			if (selected_page == page)
			{
				if (selected_slider < category)
					cursor_down();

				if (selected_slider > category)
					cursor_up();
			}

		}
		
	}

	/*
	std::vector<std::vector<std::string>> slider_names_table = 
	{
		{"Race"},
		{"Gender", "Preset", "Skin Tone", "Weight"},
		{"Complexion", "Dirt", "Dirt colour", "Scars", "War paint", "War paint colour"},
		{"Nose type", "Nose height", "Nose length", "Jaw width", "Jaw height", "Jaw forward", "Cheekbone height", "Cheekbone width", "Cheek color", "Laugh lines", "Cheek colour lower", "Nose colour", "Chin colour", "Neck colour", }
	}
	*/

	std::string get_current_slider_name()
	{
		//_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA[47].2.text = Skin Tone

		std::string result = "Parameter";

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
				{
					int selected_slider = get_selected_slider();

					if (selected_slider < 0)
						return "Parameter";

					int shift = get_category_shift(get_selected_page());

					selected_slider += shift;

					std::string selected_slider_text = std::to_string(selected_slider);

					std::string path = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA." + selected_slider_text + ".text";

					if (menu->uiMovie->GetVariable(&var1, path.c_str()))
						if (!var1.IsNull() && var1.IsString())
						{
							result = var1.GetString();
						}
				}


		return result;
	}


	std::string get_force_message(int page, int category)
	{
		std::string result = "";

		switch (page) 
		{
		case (0):
		{
			result = "You are creating your character in Skyrim. Select your character's race from provided options";
			break;
		}

		case (1):
		{
			switch (category)
			{
				case (0):
				{
					result = "You are creating your character in Skyrim. Select your character's gender from provided options";
					break;
				}

				case (1):
				{
					result = "You are creating your character in Skyrim. Select your character's look preset from provided options"; 
					break;
				}

				default:
				{
					result = "You are creating your character in Skyrim. Select your character's " + get_current_slider_name() + " from provided options";
					break;
				}
			}

			break;
		}

		case (8):
		{
			result = "You are creating your character in Skyrim. Give your character's name";
			break;
		}

		default:
		{
			result = "You are creating your character in Skyrim. Select your character's " + get_current_slider_name() + " from provided options";
			break;
		}


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



	int get_max_generic_slider()
	{
		int result = -1;

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
				{
					int selected_slider = get_selected_slider();

					if (selected_slider < 0)
						return selected_slider;

					int shift = get_category_shift(get_selected_page());

					selected_slider += shift;

					std::string selected_slider_text = std::to_string(selected_slider);

					std::string path_position = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA." + selected_slider_text + ".position";
					std::string path_interval = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA." + selected_slider_text + ".interval";
					std::string path_min = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA." + selected_slider_text + ".sliderMin";
					std::string path_max = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA." + selected_slider_text + ".sliderMax";

					RE::GFxValue var_position;
					RE::GFxValue var_interval;
					RE::GFxValue var_min;
					RE::GFxValue var_max;

					float position = 0.0f;
					float interval = 0.0f;
					float min = 0.0f;
					float max = 0.0f;



					if (menu->uiMovie->GetVariable(&var_position, path_position.c_str()))
						if (!var_position.IsNull() && var_position.IsNumber())
						{
							position = var_position.GetNumber();
						}

					if (menu->uiMovie->GetVariable(&var_interval, path_interval.c_str()))
						if (!var_interval.IsNull() && var_interval.IsNumber())
						{
							interval = var_interval.GetNumber();
						}

					if (menu->uiMovie->GetVariable(&var_min, path_min.c_str()))
						if (!var_min.IsNull() && var_min.IsNumber())
						{
							min = var_min.GetNumber();
						}

					if (menu->uiMovie->GetVariable(&var_max, path_max.c_str()))
						if (!var_max.IsNull() && var_max.IsNumber())
						{
							max = var_max.GetNumber();
						}


					float slider_length = max - min;

					if (interval == 0.0f)
						return -1;

					result = slider_length / interval;


				}


		return result;
	}




	std::vector<MenuOption> get_options(int page, int category)
	{
		std::vector<MenuOption> result{};

		switch (page) 
		{
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
				switch (category)
				{
					case (0):
					{
						MenuOption option = { 0, "Male" };
						result.push_back(option);
						option = { 1, "Female" };
						result.push_back(option);
						break;
					}

					case (1):
					{
						MenuOption option = { 0, "Preset1" };

						for (int i = 0; i < get_max_preset_slider() + 1; i++)
						{
							option = { i, "Preset" + std::to_string(i) };
							result.push_back(option);
						}

						break;
					}

					default:
					{
						MenuOption option = { 0, "Value 0" };

						for (int i = 0; i < get_max_generic_slider() + 1; i++)
						{
							option = { i, "Value " + std::to_string(i) };
							result.push_back(option);
						}

						break;
					}
				}

				break;
			}

			default:
			{
				MenuOption option = { 0, "Value 0" };

				for (int i = 0; i < get_max_generic_slider() + 1; i++)
				{
					option = { i, "Value " + std::to_string(i) };
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

		auto ui = RE::UI::GetSingleton();
		if (!ui->IsMenuOpen(RE::RaceSexMenu::MENU_NAME))
		{
			result.first = true;
			result.second = "[Error]";
			return result;
		}

		if (!race_defined)
		{
			return set_race_choice(choice);
		}

		if (!generic_slider_defined)
		{
			return set_generic_slider_choice(choice);
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


	int get_generic_slider_selected_index()
	{
		int result = -1;

		
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::RaceSexMenu>(); menu)
				if (menu->uiMovie)
				{
					int selected_slider = get_selected_slider();

					if (selected_slider < 0)
						return selected_slider;

					int shift = get_category_shift(get_selected_page());

					selected_slider += shift;

					std::string selected_slider_text = std::to_string(selected_slider);

					std::string path_position = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA." + selected_slider_text + ".position";
					std::string path_interval = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA." + selected_slider_text + ".interval";
					std::string path_min = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA." + selected_slider_text + ".sliderMin";
					std::string path_max = "_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance.PanelTwoWideInstance.List_mc.EntriesA." + selected_slider_text + ".sliderMax";

					RE::GFxValue var_position;
					RE::GFxValue var_interval;
					RE::GFxValue var_min;
					RE::GFxValue var_max;

					float position = 0.0f;
					float interval = 0.0f;
					float min = 0.0f;
					float max = 0.0f;



					if (menu->uiMovie->GetVariable(&var_position, path_position.c_str()))
						if (!var_position.IsNull() && var_position.IsNumber())
						{
							position = var_position.GetNumber();
						}

					if (menu->uiMovie->GetVariable(&var_interval, path_interval.c_str()))
						if (!var_interval.IsNull() && var_interval.IsNumber())
						{
							interval = var_interval.GetNumber();
						}

					if (menu->uiMovie->GetVariable(&var_min, path_min.c_str()))
						if (!var_min.IsNull() && var_min.IsNumber())
						{
							min = var_min.GetNumber();
						}

					if (menu->uiMovie->GetVariable(&var_max, path_max.c_str()))
						if (!var_max.IsNull() && var_max.IsNumber())
						{
							max = var_max.GetNumber();
						}


					float slider_length = max - min;

					if (interval == 0.0f)
						return -1;

					//float slider_coef = slider_length / interval; //amount of steps on slider

					float pos_corrected = position - min;

					result = pos_corrected / interval;


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



	void move_generic_slider(int generic_slider_choice)
	{
		int selected_generic_slider_index = get_generic_slider_selected_index();

		if (selected_generic_slider_index > generic_slider_choice)
			left();
		if (selected_generic_slider_index < generic_slider_choice)
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
		//int test123 = get_selected_slider();
		//bool stop_here = false;
		//return;


		RE::UI* ui = RE::UI::GetSingleton();

		in_racemenu = ui->IsMenuOpen(RE::RaceSexMenu::MENU_NAME);

		auto menu = ui->GetMenu<RE::RaceSexMenu>();

		
		
		if (in_racemenu)
		{
			pause_timer += dtime;
		}

		/*
		if (confirm_race)
		{
			if (confirm_race_and_go_to_body(dtime))
			{
				confirm_race = false;
				race_defined = true;
			}
		}
		*/


		if (racemenu_processor_timer > 0.01f)
		{
			racemenu_processor_timer = 0.0f;

			if (in_racemenu && pause_timer > 2.0f)
			{
				if (!race_defined)
				{
					reset_category();

					if (!is_inside_of_category(current_page, current_slider))
						move_to_category(current_page, current_slider);
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
								set_universal_block(0.2f);
							}
								
						}
						else
						{
							if (!race_request_sent)
							{
								
								if (force_choice(get_options(current_page, current_slider), get_force_message(current_page, current_slider), force_type::race))
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
										//confirm_race = true;

										confirm_race = false;
										race_defined = true;

										increment_category();

										set_universal_block(1.0f);
									}
								}
							}
						}
					}
					
				}
				else
				{
					if (current_page < 8)
					{
						if (!generic_slider_defined)
						{
							if (!is_inside_of_category(current_page, current_slider))
								move_to_category(current_page, current_slider);
							else
							{

								if (!generic_slider_request_sent)
								{
									if (force_choice(get_options(current_page, current_slider), get_force_message(current_page, current_slider), force_type::race))
										generic_slider_request_sent = true;
								}
								else
								{
									if (generic_slider_choice_valid)
									{
										int selected_index = get_generic_slider_selected_index();
										if ((selected_index != generic_slider_choice))
										{
											if (!missing_item_detected)
												move_generic_slider(generic_slider_choice);
											else
												reset_menu();
										}
										else
										{
											//generic_slider_defined = true;

											generic_slider_defined = false;
											generic_slider_request_sent = false;
											generic_slider_choice_valid = false;
											generic_slider_choice = -1;

											increment_category();
											set_universal_block(1.0f);
										}
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

									if (force_choice({}, get_force_message(current_page, current_slider), force_type::character_name))
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
											if (last_pause_time > 1.0f)
											{
												send_random_context("[Character creation is done!]", false);

												name_defined = true;
												//menu->ChangeName(typed_string.c_str()); //this jus closes menu (and probably sets the name) but it wasnt typed

												menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField.AcceptButton.onPress", nullptr, nullptr, 0);
												menu->uiMovie->Invoke("_root.RaceSexMenuBaseInstance.RaceSexPanelsInstance._TextEntryField.AcceptButton.onRelease", nullptr, nullptr, 0);

												set_universal_block(2.0f);
											}
											else
												last_pause_time += dtime + 0.01;

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