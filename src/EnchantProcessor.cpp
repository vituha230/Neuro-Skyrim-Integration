

//TODO: 2 enchantments per item

#include "InputActions.hpp"
#include "EnchantProcessor.hpp"
#include "main.hpp"



namespace EnchantProcessor {





bool in_enchant = false;
bool quitting_menu = false;
bool rolled_over = false;
bool operation_type_request_sent = false;
bool operation_type_choice_valid = false;
int operation_type_choice = -1;
//bool disenchant_item_request_sent = false;
//bool disenchant_item_choice_valid = false;
//int disenchant_item_choice = -1;
bool check_results = false;
bool up_done = false;
bool down_done = false;
bool fill_items_one_direction_done = false;
bool items_list_valid = false;
int last_cursor_move = 0;
bool missing_item_detected = false;
bool filling_items = false;
int step_enchanting = -1;
bool enchant_item_request_sent = false;
bool enchant_item_choice_valid = false;
int enchant_item_choice = -1;
float result_timeout = 0.0f;
bool filter_already_selected = false;
bool scroll_stuck = false;
float scroll_stuck_time = 0.0f;
int last_scroll = -1;

int slider_choice = 0;
bool slider_choice_valid = false;
bool slider_request_sent = false;
bool slider_confirmed = false;
bool slider_confirming = false;

struct item_data {
	std::string name;
	int price; //TODO: replace with something useful (maybe type of item)
};

std::map<int, item_data> items_list{};


enum way_to_fill {
	down,
	up,
	downup,
	updown
};

way_to_fill way{};





void reset_enchant()
{
	quitting_menu = false;
	rolled_over = false;
	operation_type_request_sent = false;
	operation_type_choice_valid = false;
	operation_type_choice = -1;
	//disenchant_item_request_sent = false;
	//disenchant_item_choice_valid = false;
	//disenchant_item_choice = -1;
	check_results = false;
	up_done = false;
	down_done = false;
	fill_items_one_direction_done = false;
	items_list_valid = false;
	last_cursor_move = 0;
	missing_item_detected = false;
	filling_items = false;
	items_list.clear();
	step_enchanting = -1;
	enchant_item_request_sent = false;
	enchant_item_choice_valid = false;
	enchant_item_choice = -1;
	result_timeout = 0.0f;
	filter_already_selected = false;

	scroll_stuck = false;
	scroll_stuck_time = 0.0f;
	last_scroll = -1;

	slider_choice = 0;
	slider_choice_valid = false;
	slider_request_sent = false;
	slider_confirmed = false;
	slider_confirming = false;
}

void reset_next_step()
{
	up_done = false;
	down_done = false;
	fill_items_one_direction_done = false;
	items_list_valid = false;
	last_cursor_move = 0;
	missing_item_detected = false;
	filling_items = false;
	items_list.clear();
	enchant_item_request_sent = false;
	enchant_item_choice_valid = false;
	enchant_item_choice = -1;
	filter_already_selected = false;

	slider_choice = 0;
	slider_choice_valid = false;
	slider_request_sent = false;
	slider_confirmed = false;
	slider_confirming = false;

}



int get_selected_category_index()
{
	int result = -1;
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	RE::GFxValue var1;

	if (ui && menu && ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
		if (menu->uiMovie && menu->subMenu)
			//if (menu->uiMovie->GetVariable(&var1, "_root.Menu.CategoryList._ItemsList.iSelectedIndex"))
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu.CategoryList._CategoriesList.iSelectedIndex"))
				if (!var1.IsNull() && var1.IsNumber())
					result = var1.GetNumber();

	return result;
}

int get_item_selected_index()
{
	int result = -1;
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	RE::GFxValue var1;

	if (ui && menu && ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
		if (menu->uiMovie && menu->subMenu)
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu.ItemList.iSelectedIndex"))
				if (!var1.IsNull() && var1.IsNumber())
					result = var1.GetNumber();

	return result;
}



int get_amount_of_items()
{
	int result = -1;
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	RE::GFxValue var1;

	if (ui && menu && ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
		if (menu->uiMovie && menu->subMenu)
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu.ItemList.iNumUnfilteredItems"))
				if (!var1.IsNull() && var1.IsNumber())
					result = var1.GetNumber();

	return result;
}

bool slider_active();

bool is_inside_of_category()
{
	if (slider_active())
		return true;

	bool result = false;
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	RE::GFxValue var1;

	if (ui && menu && ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
		if (menu->uiMovie && menu->subMenu)
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu.ItemList.focused"))
				if (!var1.IsNull() && var1.IsNumber())
					result = var1.GetNumber();

	return result;
}


void move_to_category(int index)
{
	int selected_category = get_selected_category_index();

	if (is_inside_of_category())
	{
		if (selected_category != index) //these checks are a bit weird but it will be useful in case get_selected_category_index lingers after we exit the category
			left();
	}
	else
	{
		if (selected_category > index)
			cursor_up();
		
		if (selected_category < index)
			cursor_down();

		if (selected_category == index)
			right();
	}
}



void move_to_item(int index)
{
	int selected_item = get_item_selected_index();

	if (is_inside_of_category())
	{
		if (selected_item > index)
			cursor_up();

		if (selected_item < index)
			cursor_down();
	}
}



/*
std::vector<MenuOption> get_enchantment_options()
{
	std::vector<MenuOption> result{};

	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	if (menu && menu->subMenu)
	{
		auto enchant_menu = (RE::CraftingSubMenus::EnchantConstructMenu*)menu->subMenu;


	}

	return result;
}
*/



std::string get_result_message()
{
	std::string result = "";

	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
			if (menu->uiMovie)
				//+		[11]	"_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray[1].0.TextFieldClip.tf1.text = Quicksaving..."	std::string
				if (menu->uiMovie->GetVariable(&var1, "_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray"))
					if (var1.IsArray())
					{
						int size = var1.GetArraySize();
						if (size > 0)
						{
							RE::GFxValue subvar;
							if (var1.GetElement(0, &subvar)) //size-1 or 0? idk the order..
								if (subvar.GetMember("TextFieldClip", &subvar))
									if (subvar.GetMember("tf1", &subvar))
										if (subvar.GetMember("text", &subvar))
											if (subvar.IsString())
												result = subvar.GetString();
						}

					}

	if (result != "")
		MiscThings::update_old_topleft_nofification(result);

	return result;
}


bool quit_menu()
{
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	bool result = false;


	if (menu && !ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME))
	{
		result = true;

		//result = menu->uiMovie->Invoke("_root.Menu.BottomBarInfo.Buttons.1.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
		
		quitting_menu = true;

		set_universal_block(1.0f);


		//RE::UIMessageQueue::GetSingleton()->AddMessage(RE::CraftingMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
	}

	return result;
}



std::pair<bool, std::string> set_enchant_type(int in_type)
{
	std::pair<bool, std::string> result{};

	if (in_type == -1)
	{
		quit_menu();
		result.first = true;
		result.second = "[Stopped enchanting]";
		return result;
	}

	if (in_enchant)
	{
		if (in_type == 0 || in_type == 1)
		{
			if (in_type == 0)
			{
				operation_type_choice = enchant_type::enchant;
			}
			else
				operation_type_choice = enchant_type::disenchant;
			
			operation_type_choice_valid = true;
			result.first = true;
			result.second = "[Processing...]";
		}
		else
		{
			result.first = false;
			result.second = "Invalid choice";
		}
	}
	else
	{
		result.first = true;
		result.second = "Not in enchanting menu";
	}

	return result;
}



std::vector<MenuOption> get_enchantment_options()
{
	std::vector<MenuOption> result{};
	for (std::pair<int, item_data> item : items_list)
	{
		MenuOption option = { item.first, item.second.name};
		result.push_back(option);
	}

	result.push_back({ -1, "[QUIT ENCHANTING TABLE]" });

	return result;
}



int get_item_scroll_position()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.InventoryLists._ItemsList.iScrollPosition"))
					//send_random_context(std::to_string(var1.GetNumber()).c_str());
					return var1.GetNumber();
	return -1;
}


int get_item_max_scroll_position()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.InventoryLists._ItemsList.iMaxScrollPosition"))
					//send_random_context(std::to_string(var1.GetNumber()).c_str());
					return var1.GetNumber();
	return -1;
}




void update_items_list()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.InventoryLists._ItemsList"))
				{
					std::string path_root = "_root.Menu.InventoryLists._ItemsList.";
					for (int i = 0; i < 30; i++)
					{
						std::string full_path = path_root + "Entry" + std::to_string(i);
						if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
						{
							if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
							{
								//_root.Menu.InventoryLists._ItemsList.Entry8.textField.textColor = 16777215.000000
								//_root.Menu.InventoryLists._ItemsList.Entry9.textField.textColor =  5000268.000000

								//got indexed item
								if (!var1.IsNull() && var1.IsNumber())
								{
									MenuOption result{};

									result.id = var1.GetNumber();
									std::string name = "";
									if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.text").c_str()))
										if (!var1.IsNull() && var1.IsString())
											name = var1.GetString();

									int text_color = 0;
									if (menu->uiMovie->GetVariable(&var1, (full_path + ".textField.textColor").c_str()))
										if (!var1.IsNull() && var1.IsNumber())
											text_color = var1.GetNumber();


									if (name != "" && text_color > 10000000)
									{
										if (result.id == get_item_selected_index())
										{
											item_data data{};
											data.name = name;
											data.price = 0;
											items_list.insert({ result.id, data });
										}

									}
								}
							}
						}
					}
				}
}






void fill_items_list_one_direction(bool direction_down, float dtime)
{
	int max_scroll = get_item_max_scroll_position();
	int scroll = get_item_scroll_position();

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
	int max_scroll = get_item_max_scroll_position();
	int scroll = get_item_scroll_position();

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////// SLIDER
/*

			"_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._focused" - slider active
			_focused - slider active
			_minimum
			_maximum
			_value - current position
			*/

int get_slider_min()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.ItemInfo.EnchantingSlider_mc._minimum"))
					if (!var1.IsNull() && var1.IsNumber())
						return var1.GetNumber();
	return -1;
}

int get_slider_max()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.ItemInfo.EnchantingSlider_mc._maximum"))
					if (!var1.IsNull() && var1.IsNumber())
						return var1.GetNumber();
	return -1;
}

int get_slider_pos()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.ItemInfo.EnchantingSlider_mc._value"))
					if (!var1.IsNull() && var1.IsNumber())
						return var1.GetNumber();
	return -1;
}


bool slider_active()
{
	bool result = false;
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.ItemInfo.EnchantingSlider_mc._focused"))
					if (!var1.IsNull() && var1.IsNumber() && (int)var1.GetNumber() == 1)
						result = true;

	return result;
}


void move_slider_to_pos(int desired_pos)
{
	int slider_pos = get_slider_pos();

	if (slider_pos > desired_pos)
	{
		left();
	}
	if (slider_pos < desired_pos)
	{
		right();
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




std::pair<bool, std::string> set_slider_choice(int pos)
{
	std::pair<bool, std::string> result{};


	if (pos == -1)
	{
		quit_menu();
		result.first = true;
		result.second = "[Stopped enchanting]";
		return result;
	}

	if (pos < get_slider_min() || pos > get_slider_max() || !slider_active())
	{
		//TODO: give int result, separate invalid id, category undefined and barter_type undefined.
		//barter_item_request_sent = false; //FAILED FORCED ACTIONS ARE REFORCED AUTOMATICALLY
		result.first = false;
		result.second = "Invalid choice";
	}
	else
	{
		slider_choice = pos;
		slider_choice_valid = true;
		result.first = true;
		result.second = "[Processing...]";
	}

	return result;
}


std::pair<bool, std::string> set_item_choice(int id)
{
	std::pair<bool, std::string> result{};


	if (id == -1)
	{
		quit_menu();
		result.first = true;
		result.second = "[Stopped enchanting]";
		return result;
	}

	if (items_list_valid && operation_type_choice_valid)
	{
		if (id == -2)
		{
			if (operation_type_choice_valid && operation_type_choice == enchant_type::enchant)
			{
				//asked to go back
				if (step_enchanting == 1)
				{
					reset_enchant();
				}
				else
				{
					step_enchanting--;
					reset_next_step();
				}
			}
			else
			{
				reset_enchant();
			}

			result.first = true;
			result.second = "[Going back...]";
		}
		else
		{
			if (items_list.find(id) == items_list.end())
			{
				result.first = false;
				result.second = "Invalid choice ID";
			}
			else
			{
				enchant_item_choice = id;
				enchant_item_choice_valid = true;
				result.first = true;
				result.second = "[Processing...]";
			}
		}
	}
	else
	{
		result.first = true;
		result.second = "[Error]";
	}


	return result;
}




std::string get_slider_text()
{
	std::string result = "";

	//TODO: proper info
	result = "You need to choose strength of the enchantment. Weaker enchantments consume less charge. Stronger enchantments consume more charge. ";

	return result;
}


//"_root.Menu.MenuDescription.text = Alchemy: Combine ingredients to make potions",
//"_root.Menu.BottomBarInfo.PlayerInfoCard_mc.SkillLevelLabel.text = Alchemy",



float enchant_processor_timer = 0.0f;


void processor(float dtime)
{

	RE::UI* ui = RE::UI::GetSingleton();

	auto menu = ui->GetMenu<RE::CraftingMenu>();

	//std::string test = "";

	RE::GFxValue var1;

	in_enchant = false;

	if (ui && menu && ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
		if (menu->uiMovie && menu->subMenu)
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu.MenuDescription.text"))
				if (!var1.IsNull() && var1.IsString())
				{
					std::string menu_name = var1.GetString();
					if (menu_name == "Enchanting: Choose an item to destroy and learn its Enchantment, or combine an Item, Enchantment, and Soul Gem to create magic items")
					{
						in_enchant = true;
						auto enchant_menu = (RE::CraftingSubMenus::EnchantConstructMenu*)menu->subMenu;
					}
				}

	if (enchant_processor_timer > 0.01f || check_results)
	{
		enchant_processor_timer = 0.0f;

		if (in_enchant)
		{
			WalkerProcessor::reset_walker();

			if (quitting_menu)
			{
				if (auto menu_confirm_quit = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm_quit)
				{
					RandomMessageBoxProcessor::set_messagebox_handeled();
					if (menu_confirm_quit->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
						if (!var1.IsNull() && var1.IsString())
						{
							std::string var1_string = var1.GetString();

							if (var1_string == "Quit enchanting?")
							{
								if (!rolled_over)
								{

									menu_confirm_quit->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
									rolled_over = true;
									set_universal_block(2.0f);
								}
								else
								{
									rolled_over = false;
									menu_confirm_quit->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
									set_universal_block(1.5f);
									reset_enchant();

									//supposedly we exit the menu now
								}
							}
						}
					
				}
				else
					cancel();
			}
			else
			{
				if (check_results) //this is supposed to be finall result messsage box
				{
					if (auto menu_confirm = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm)
					{
						RandomMessageBoxProcessor::set_messagebox_handeled();
						if (!rolled_over)
						{
							if (menu_confirm->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
								if (!var1.IsNull() && var1.IsString())
								{
									std::string message = var1.GetString();
									if (message != "Are you sure you want to destroy this item and learn its effects?" && message != "Enchant item?") //this is done automatically
									{
										std::string var_string = var1.GetString();
										send_random_context("[" + var_string + "]");
									}
								}



							menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
							rolled_over = true;
							set_universal_block(2.0f);
						}
						else
						{
							rolled_over = false;
							menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
							set_universal_block(1.5f);

							reset_enchant();
						}
					}
					else
					{
						std::string result = get_result_message();
						if (result != "")
						{
							send_random_context("[" + result + "]");
							reset_enchant();
						}
						if (result_timeout > 2.0f)
							reset_enchant();
						else
							result_timeout += dtime;
							
					}
					
				}
				else
				{
					if (!operation_type_request_sent)
					{
						
						if (force_choice({ {0, "Enchanting"}, {1, "Disenchanting"}, {-1, "[QUIT ENCHANTING TABLE]"}}, "You are at enchanting table. Choose operation type. ", force_type::enchant_type))
							operation_type_request_sent = true;
					}
					else
					{
						if (operation_type_choice_valid)
						{
							if (operation_type_choice == enchant_type::disenchant)
							{
								if (get_selected_category_index() != 0) //for npw the category is fixed as 0. will work unless we have too many items?
									move_to_category(0);
								else
								{
									if (!is_inside_of_category())
										right();
									else
									{
										if (!items_list_valid)
											if (!filling_items)
											{
												setup_fill_item_list();
												filling_items = true;
											}
											else
												fill_items_list(0.01f);
										else
										{
											if (!enchant_item_request_sent)
											{
												
												if (force_choice(get_enchantment_options(), "You are at enchanting table. Choose an item to disenchant. ", force_type::enchant_item))
												{
													enchant_item_request_sent = true;
												}
											}
											else
											{
												if (enchant_item_choice_valid)
												{
													if (get_item_selected_index() != enchant_item_choice)
													{
														move_to_item(enchant_item_choice);
													}
													else
													{
														if (!check_results)
														{
															if (auto menu_confirm = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm) // this is confirmation menu. after it there is "you leaned this ok" message box, handeled up there
															{
																RandomMessageBoxProcessor::set_messagebox_handeled();
																if (!rolled_over)
																{
																	if (menu_confirm->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
																		if (!var1.IsNull() && var1.IsString())
																		{
																			std::string message = var1.GetString();
																			if (message != "Are you sure you want to destroy this item and learn its effects?" && message != "Enchant item?") //this is done automatically
																			{
																				std::string var_string = var1.GetString();
																				send_random_context("[" + var_string + "]");
																			}
																		}


																	menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
																	rolled_over = true;
																	set_universal_block(2.0f);
																}
																else
																{
																	rolled_over = false;
																	menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
																	set_universal_block(1.5f);

																	check_results = true;
																}
															}
															else
																confirm();
														}
													}
												}
											}
										}
									}
								}
							}

							if (operation_type_choice == enchant_type::enchant)
							{
								if (step_enchanting < 1 || step_enchanting > 3)
									step_enchanting = 1;

								if (get_selected_category_index() != step_enchanting + 1) //turned out it starts with 2
									move_to_category(step_enchanting + 1);
								else
								{
									if (!is_inside_of_category())
										right();
									else
									{
										if (!items_list_valid)
											if (!filling_items)
											{
												setup_fill_item_list();
												filling_items = true;
											}
											else
												fill_items_list(0.01f);
										else
										{
											if (!enchant_item_request_sent)
											{
												
												if (force_choice(get_enchantment_options(), "You are at enchantment table. Choose an enchantment to apply on item. ", force_type::enchant_item))
												{
													enchant_item_request_sent = true;
													filter_already_selected = true;
												}
												
											}
											else
											{
												if (enchant_item_choice_valid)
												{
													if (get_item_selected_index() != enchant_item_choice)
													{
														move_to_item(enchant_item_choice);
													}
													else
													{
														if (filter_already_selected)
														{
															//if already selected, disellect and try again. might be wrong item.
															auto enchant_menu = (RE::CraftingSubMenus::EnchantConstructMenu*)menu->subMenu;
															bool selection_done = false;
															switch (step_enchanting)
															{
															case (1): selection_done = enchant_menu->selected.item != nullptr; break;
															case (2): selection_done = !enchant_menu->selected.effects.empty(); break; //TODO: 2 effects
															case (3): selection_done = enchant_menu->selected.soulGem != nullptr; break;
															}

															if (selection_done)
																confirm(); //it was selected. disellect.
															else
																filter_already_selected = false; //nothing selected, safe.
														}
														else
														{
															if (slider_active())
															{
																if (!slider_request_sent)
																{
																	
																	if (force_choice({}, "You are enchanting." + get_slider_text() + "Valid range: from " + std::to_string(get_slider_min()) + " to " + std::to_string(get_slider_max()), force_type::enchant_strength))
																		slider_request_sent = true;
																}
																else
																{
																	if (slider_choice_valid)
																	{
																		if (get_slider_pos() != slider_choice)
																			move_slider_to_pos(slider_choice);
																		else
																		{
																			if (!slider_confirmed)
																			{
																				if (false)//while slider is active, confirm. once its confirmed, it disappears and algorithm will move to next step
																				{
																					slider_confirming = false;
																					slider_confirmed = true;
																				}
																				else
																				{
																					confirm();
																					slider_confirming = true;
																				}
																			}
																		}
																	}
																}
															}
															else
															{
																auto enchant_menu = (RE::CraftingSubMenus::EnchantConstructMenu*)menu->subMenu;
																bool selection_done = false;
																switch (step_enchanting)
																{
																case (1): selection_done = enchant_menu->selected.item != nullptr; break;
																case (2): selection_done = !enchant_menu->selected.effects.empty(); break; //TODO: 2 effects
																case (3): selection_done = enchant_menu->selected.soulGem != nullptr; break;
																}

																if (selection_done)
																{
																	if (step_enchanting == 3)
																	{
																		if (!check_results)
																		{
																			if (auto menu_confirm = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm) // this is confirmation menu. after it there is "you leaned this ok" message box, handeled up there
																			{
																				RandomMessageBoxProcessor::set_messagebox_handeled();
																				if (!rolled_over)
																				{
																					if (menu_confirm->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
																						if (!var1.IsNull() && var1.IsString())
																						{
																							std::string message = var1.GetString();
																							if (message != "Are you sure you want to destroy this item and learn its effects?" && message != "Enchant item?") //this is done automatically
																							{
																								std::string var_string = var1.GetString();
																								send_random_context("[" + var_string + "]");
																							}
																						}


																					menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
																					rolled_over = true;
																					set_universal_block(2.0f);
																				}
																				else
																				{
																					rolled_over = false;
																					menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
																					set_universal_block(1.5f);

																					std::string result = get_result_message();
																					if (result != "")
																					{
																						send_random_context("[" + result + "]");
																						reset_enchant();
																					}
																					else
																						check_results = true;

																				}
																			}
																			else
																			{
																				//this is [R]Craft button
																				menu->uiMovie->Invoke("_root.Menu.BottomBarInfo.Buttons.3.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
																				set_universal_block(1.0f);
																			}
																		}
																	}
																	else
																	{
																		step_enchanting++;
																		reset_next_step();
																	}
																}
																else
																	confirm();
															}
														}
													}
												}
											}
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
			reset_enchant();
		}	
	}
	else
		enchant_processor_timer += dtime;
}


};
