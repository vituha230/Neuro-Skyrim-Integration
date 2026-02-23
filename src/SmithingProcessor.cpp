


//TODO: maybe there are more types of smithing menus?



#include "InputActions.hpp"
#include "SmithingProcessor.hpp"
#include "main.hpp"



namespace SmithingProcessor {



bool in_smithing = false;
bool quitting_menu = false;
bool rolled_over = false;
bool check_results = false;
bool up_done = false;
bool down_done = false;

bool upgrade_mode = false;
bool no_category = false;

int last_cursor_move = 0;

bool fill_items_one_direction_done = false;
bool items_list_valid = false;
bool missing_item_detected = false;
bool filling_items = false;

bool fill_categories_one_direction_done = false;
bool categories_list_valid = false;
bool missing_category_detected = false;
bool filling_categories = false;


bool item_choice_request_sent = false;
bool item_choice_valid = false;
int item_choice = -1;

bool category_choice_request_sent = false;
bool category_choice_valid = false;
int category_choice = -1;


float result_timeout = 0.0f;
bool filter_already_selected = false;
bool scroll_stuck = false;
float scroll_stuck_time = 0.0f;
int last_scroll = -1;



int ask_for_more = -1;



struct item_data {
	std::string name;
	std::string ingredients_required;
	bool can_craft;
	//int price; //TODO: replace with something useful (maybe type of item)
};

std::map<int, item_data> items_list{};
std::map<int, std::string> categories_list{};

enum way_to_fill {
	down,
	up,
	downup,
	updown
};

way_to_fill way{};







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


bool is_inside_of_category()
{
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


std::string get_result_message()
{
	std::string result = "";

	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
			if (menu->uiMovie)
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
		quitting_menu = true;
		set_universal_block(1.0f);
	}

	return result;
}



std::vector<MenuOption> get_items_options()
{
	std::vector<MenuOption> result{};
	for (std::pair<int, item_data> item : items_list)
	{
		std::string can_craft_text = "";
		if (item.second.can_craft)
			if (upgrade_mode)
				can_craft_text = ". Can be upgraded.";
			else
				can_craft_text = ". Can be crafted.";
		else
		{
			if (upgrade_mode)
				can_craft_text = ". Cannot be upgraded.";
			else
				can_craft_text = ". Cannot be crafted.";
		}
			

		MenuOption option = { item.first, item.second.name + ", " + item.second.ingredients_required + can_craft_text};
		result.push_back(option);
	}
	return result;
}


std::vector<MenuOption> get_categories_options()
{
	std::vector<MenuOption> result{};
	for (std::pair<int, std::string> category : categories_list)
	{
		MenuOption option = { category.first, category.second };
		result.push_back(option);
	}
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
					return var1.GetNumber();
	return -1;
}



int get_category_scroll_position()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.InventoryLists._CategoriesList.iScrollPosition"))
					//send_random_context(std::to_string(var1.GetNumber()).c_str());
					return var1.GetNumber();
	return -1;
}



int get_category_max_scroll_position()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)						 //_root.Menu.InventoryLists._CategoriesList.iMaxScrollPosition = 6.000000
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.InventoryLists._CategoriesList.iMaxScrollPosition"))
					//send_random_context(std::to_string(var1.GetNumber()).c_str());
					return var1.GetNumber();
	return -1;
}





void update_categories_list()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.InventoryLists._CategoriesList")) //this gives only categories that appear on screen right now. if you scroll and some become invisible - this will not show them
				{
					std::string path_root = "_root.Menu.InventoryLists._CategoriesList.";
					for (int i = 0; i < 30; i++)
					{
						std::string full_path = path_root + "Entry" + std::to_string(i);
						if (menu->uiMovie->GetVariable(&var1, full_path.c_str()))
						{
							if (menu->uiMovie->GetVariable(&var1, (full_path + ".itemIndex").c_str()))
							{
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
										result.text = name;
										categories_list.insert({ result.id, result.text });
								}
							}
						}
					}
				}
}


std::string get_requires_text_selected_item()
{
	//_root.Menu.AdditionalDescription.text = Requires: Bear Pelt(1)
	std::string result = "";
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::CraftingMenu>(); menu)
			if (menu->uiMovie)				
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu.AdditionalDescription.text"))
					if (!var1.IsNull() && var1.IsString())
					{
						std::string raw_text = var1.GetString();
						auto parenthesis_open = raw_text.find("(");

						if (parenthesis_open != std::string::npos && parenthesis_open != 0)
						{
							result = raw_text.substr(0, parenthesis_open - 1);
						}
						else
							result = raw_text;
					}
	return result;
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


									if (name != "")
									{
										if (result.id == get_item_selected_index())
										{
											item_data data{};
											data.name = name;
											data.ingredients_required = get_requires_text_selected_item();
											data.can_craft = text_color > 10000000;

											items_list.insert({ result.id, data });
										}

									}
								}
							}
						}
					}
				}
}





void fill_categories_list_one_direction(bool direction_down, float dtime)
{
	int max_scroll = get_category_max_scroll_position();
	int scroll = get_category_scroll_position();

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
			update_categories_list(); //do this on every scroll
			if (scroll_stuck)
				perk_down();
			else
				cursor_down();
		}
		else
		{
			update_categories_list(); //do this on every scroll
			if (scroll_stuck)
				filling_items = false; //restart fill
			else
				fill_categories_one_direction_done = true;
		}
	}
	else
		if (scroll > 0)
		{
			update_categories_list(); //do this on every scroll
			if (scroll_stuck)
				perk_up();
			else
				cursor_up();
		}
		else
		{
			update_categories_list(); //do this on every scroll
			if (scroll_stuck)
				filling_items = false; //restart fill
			else
				fill_categories_one_direction_done = true;
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



void fill_categories_list(float dtime)
{
	switch (way) {
	case (way_to_fill::down):
	{
		if (fill_categories_one_direction_done)
			categories_list_valid = true;
		else
			fill_categories_list_one_direction(true, dtime);

		break;
	}

	case (way_to_fill::up):
	{
		if (fill_categories_one_direction_done)
			categories_list_valid = true;
		else
			fill_categories_list_one_direction(false, dtime);

		break;
	}

	case (way_to_fill::downup):
	{
		if (down_done)
			if (fill_categories_one_direction_done)
				categories_list_valid = true;
			else
				fill_categories_list_one_direction(false, dtime);
		else
			if (fill_categories_one_direction_done)
			{
				down_done = true;
				fill_categories_one_direction_done = false;
			}
			else
				fill_categories_list_one_direction(true, dtime);

		break;
	}

	case (way_to_fill::updown):
	{
		if (up_done)
			if (fill_categories_one_direction_done)
				categories_list_valid = true;
			else
				fill_categories_list_one_direction(true, dtime);
		else
			if (fill_categories_one_direction_done)
			{
				up_done = true;
				fill_categories_one_direction_done = false;
			}
			else
				fill_categories_list_one_direction(false, dtime);

		break;
	}
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



void setup_fill_category_list()
{
	int min_scroll = 0;
	int max_scroll = get_category_max_scroll_position();
	int scroll = get_category_scroll_position();

	last_cursor_move = 0;
	missing_category_detected = false;

	up_done = false;
	down_done = false;
	fill_categories_one_direction_done = false;

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







std::pair<bool, std::string> set_category_choice(int id)
{
	std::pair<bool, std::string> result{};

	if (id == -1)
	{
		quit_menu();
		result.first = true;
		result.second = "[Crafting cancelled]";
		return result;
	}


	if (categories_list.find(id) == categories_list.end())
	{
		//barter_category_request_sent = false; //FAILED FORCED ACTIONS ARE REFORCED AUTOMATICALLY
		result.first = false;
		result.second = "Invalid category ID";

	}
	else
	{
		if (category_choice_valid)
		{
			left(); //go back to reselect category
		}

		category_choice = id;
		category_choice_valid = true;
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
		result.second = "[Crafting cancelled]";
		return result;
	}

	if (items_list_valid && category_choice_valid)
	{
		if (id == -2)
		{
			//asked to go back
			reset_smithing();

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
				if (items_list.find(id)->second.can_craft)
				{
					item_choice = id;
					item_choice_valid = true;
					result.first = true;
					result.second = "[Processing...]";
				}
				else
				{
					result.first = false;
					result.second = "[Cannot craft this item]";
				}
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




int how_many_more_can_make()
{
	int result = 0;

	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	if (menu->subMenu)
	{
		auto alchemy_menu = (RE::CraftingSubMenus::SmithingMenu*)menu->subMenu;
		//TODO: calculate how many more
	}

	return result;
}



bool reset_smithing()
{

	if (is_inside_of_category() && !no_category && in_smithing)
		left();

	in_smithing = false;
	quitting_menu = false;
	rolled_over = false;
	category_choice_request_sent = false;
	category_choice_valid = false;
	category_choice = -1;

	fill_categories_one_direction_done = false;
	categories_list_valid = false;
	missing_category_detected = false;
	filling_categories = false;

	check_results = false;
	up_done = false;
	down_done = false;
	fill_items_one_direction_done = false;
	items_list_valid = false;
	last_cursor_move = 0;
	missing_item_detected = false;
	filling_items = false;

	item_choice_request_sent = false;
	item_choice_valid = false;
	item_choice = -1;
	result_timeout = 0.0f;
	filter_already_selected = false;
	scroll_stuck = false;
	scroll_stuck_time = 0.0f;
	last_scroll = -1;

	ask_for_more = -1;

	items_list.clear();
	categories_list.clear();

	upgrade_mode = false;
	no_category = false;



	return true;
}


void reset_items()
{
	rolled_over = false;

	check_results = false;
	up_done = false;
	down_done = false;
	fill_items_one_direction_done = false;
	items_list_valid = false;
	last_cursor_move = 0;
	missing_item_detected = false;
	filling_items = false;

	item_choice_request_sent = false;
	item_choice_valid = false;
	item_choice = -1;
	result_timeout = 0.0f;
	filter_already_selected = false;
	scroll_stuck = false;
	scroll_stuck_time = 0.0f;
	last_scroll = -1;

	items_list.clear();


	ask_for_more = -1;
}

//"_root.Menu.MenuDescription.text = Alchemy: Combine ingredients to make potions",
//"_root.Menu.BottomBarInfo.PlayerInfoCard_mc.SkillLevelLabel.text = Alchemy",


bool is_item_selected(int id)
{
	bool result = false;

	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	if (menu && menu->subMenu)
	{
		auto smithing_menu = (RE::CraftingSubMenus::SmithingMenu*)menu->subMenu;

		if (smithing_menu->currentIndex == smithing_menu->selectedIndex)
			return true;
	}


	return result;
}



std::string get_force_message(bool category)
{
	std::string result = "";

	RE::UI* ui = RE::UI::GetSingleton();
	RE::GFxValue var1;

	auto menu = ui->GetMenu<RE::CraftingMenu>();

	if (ui && menu && ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
		if (menu->uiMovie && menu->subMenu)
			//if (menu->uiMovie->GetVariable(&var1, "_root.Menu.BottomBarInfo.PlayerInfoCard_mc.SkillLevelLabel.text")) 
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu.MenuDescription.text"))
				if (!var1.IsNull() && var1.IsString())
				{
					std::string menu_name = var1.GetString();

					if (menu_name == "Smelter: Choose an item to create")
						result = "You are at smelter. Select item to craft. Send - 1 to quit smelting. ";

					if (menu_name == "Tanning Rack: Choose an item to create")
						result = "You are at tanning rack. Select item to craft. Send - 1 to quit menu. ";

					if (menu_name == "Weapon Smithing: Choose an item to improve")
						result = "You are at grindstone. Select item to improve. Send - 1 to quit menu. ";

					if (menu_name == "Armor Smithing: Choose an item to improve")
						result = "You are at workbench. Select item to improve. Send - 1 to quit menu. ";

					if (result == "") //blacksmith forge
						if (category)
							result = "You are at blacksmith forge. Select item category. Send -1 to quit smithing. ";
						else
							result = "You are at blacksmith forge. Select item to craft. Send -1 to quit smithing. Send -2 to go back to category selection.";
				}


	return result;
}







float smithing_processor_timer = 0.0f;


void processor(float dtime)
{

	RE::UI* ui = RE::UI::GetSingleton();

	auto menu = ui->GetMenu<RE::CraftingMenu>();

	RE::GFxValue var1;

	in_smithing = false;

	if (ui && menu && ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
		if (menu->uiMovie && menu->subMenu)
		{
			//these checks below are useless because smelter doesnt have "Smithing" bar
			//if (menu->uiMovie->GetVariable(&var1, "_root.Menu.BottomBarInfo.PlayerInfoCard_mc.SkillLevelLabel.text")) 
			//	if (menu->uiMovie->GetVariable(&var1, "_root.Menu.BottomBarInfo.PlayerInfoCard_mc.SkillLevelLabel.text")) 
			//		if (!var1.IsNull() && var1.IsString())
			//		{
			//			std::string test = var1.GetString();
			//			if (test == "Smithing")
			//			{
							if (menu->uiMovie->GetVariable(&var1, "_root.Menu.MenuDescription.text"))
								if (!var1.IsNull() && var1.IsString())
								{
									bool smithing_identified = false;

									std::string menu_name = var1.GetString();

									if (menu_name == "Smelter: Choose an item to create")
									{
										in_smithing = true;
										no_category = true;
										categories_list_valid = true;
										category_choice_request_sent = true;
										category_choice_valid = true;
										category_choice = 30;
										smithing_identified = true;
									}

									if (menu_name == "Weapon Smithing: Choose an item to improve")
									{
										in_smithing = true;
										upgrade_mode = true;
										no_category = true;
										categories_list_valid = true;
										category_choice_request_sent = true;
										category_choice_valid = true;
										category_choice = 0;
										smithing_identified = true;
									}

									if (menu_name == "Armor Smithing: Choose an item to improve")
									{
										in_smithing = true;
										upgrade_mode = true;
										no_category = true;
										categories_list_valid = true;
										category_choice_request_sent = true;
										category_choice_valid = true;
										category_choice = 0;
										smithing_identified = true;
									}

									if (menu_name == "Tanning Rack: Choose an item to create")
									{
										in_smithing = true;
										no_category = true;
										categories_list_valid = true;
										category_choice_request_sent = true;
										category_choice_valid = true;
										category_choice = 30;
										smithing_identified = true;
									}

									bool maybe_smithing = false;

									if (menu->subMenu && menu->subMenu->furniture && menu->subMenu->furniture->workBenchData.benchType)
									{
										auto type = menu->subMenu->furniture->workBenchData.benchType;
										if (type != RE::TESFurniture::WorkBenchData::BenchType::kAlchemy && type != RE::TESFurniture::WorkBenchData::BenchType::kAlchemyExperiment &&
											type != RE::TESFurniture::WorkBenchData::BenchType::kEnchanting && type != RE::TESFurniture::WorkBenchData::BenchType::kEnchantingExperiment)
											maybe_smithing = true;
									}


									if (!smithing_identified && maybe_smithing)
									{
										in_smithing = true; //common blacksmith forge or some weird one
									}

								}
						//}
				}

	if (smithing_processor_timer > 0.01f || check_results)
	{
		smithing_processor_timer = 0.0f;

		if (in_smithing)
		{
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
									if (no_category)
										reset_smithing();
									else
										reset_items();
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
										;//send_random_context(var1.GetString());
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

							if (no_category)
								reset_smithing();
							else
								reset_items();
						}
					}
					else
					{
						std::string result = get_result_message();
						if (result != "")
						{
							send_random_context("[" + result + "]");
							if (no_category)
								reset_smithing();
							else
								reset_items();
						}
						if (result_timeout > 2.0f)
							if (no_category)
								reset_smithing();
							else
								reset_items();
						else
							result_timeout += dtime;

					}

				}
				else
				{
					if (!categories_list_valid)
						if (!filling_categories)
						{
							setup_fill_category_list();
							filling_categories = true;
						}
						else
							fill_categories_list(0.02f);
					else
					{
						if (!category_choice_request_sent)
						{
							missing_category_detected = false;
							last_cursor_move = 0;
							category_choice_request_sent = true;
							force_choice(get_categories_options(), get_force_message(true), force_type::smithing_category);
						}
						else
						{
							if (category_choice_valid)
							{
								if (get_selected_category_index() != category_choice)
								{
									if (!missing_category_detected)
										move_to_category(category_choice);
									else
										reset_smithing();
								}
								else
									//if (get_selected_category() != category_choice && !item_confirming && !item_confirmed) 
									if (!is_inside_of_category())
										right();// confirm();
									else
									{   //NOW ITEMS
										if (!items_list_valid)
											if (!filling_items)
											{
												setup_fill_item_list();
												filling_items = true;
											}
											else
												fill_items_list(0.02f);
										else
										{
											if (!item_choice_request_sent)
											{
												missing_item_detected = false;
												last_cursor_move = 0;
												item_choice_request_sent = true;
												force_choice(get_items_options(), get_force_message(false), force_type::smithing_item);
											}
											else
											{
												if (item_choice_valid)
												{
													int selected_index = get_item_selected_index();
													if (selected_index != item_choice)
													{
														if (!missing_item_detected)
															move_to_item(item_choice);
														else
															if (no_category)
																reset_smithing();
															else
																reset_items();
													}
													else
													{
														if (upgrade_mode)
														{
															if (!is_item_selected(item_choice))
															{
																if (get_item_selected_index() != item_choice)
																{
																	move_to_item(item_choice);
																}
																else
																{
																	confirm();
																}
															}
															else
															{
																if (!check_results)
																{
																	check_results = true;
																	//this is [R]Craft button
																	menu->uiMovie->Invoke("_root.Menu.BottomBarInfo.Buttons.3.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
																	set_universal_block(1.0f);
																}
																else
																{
																	//no message box appeared, craft successful
																	std::string result = get_result_message();
																	if (result != "")
																		send_random_context("[" + result + "]");

																	if (no_category)
																		reset_smithing();
																	else
																		reset_items();
																}
															}
															
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
																				if (message != "Do you want to create this item?") //this is done automatically
																					;// send_random_context(var1.GetString());
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
																			if (no_category)
																				reset_smithing();
																			else
																				reset_items();
																		}
																		else
																			check_results = true;
																	}

																}
																else
																{
																	confirm();
																	//this is [R]Craft button
																	//menu->uiMovie->Invoke("_root.Menu.BottomBarInfo.Buttons.3.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
																	set_universal_block(0.5f);
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
			reset_smithing();
		}	
	}
	else
		smithing_processor_timer += dtime;
}


};
