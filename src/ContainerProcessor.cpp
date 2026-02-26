

//TODO: 2 enchantments per item

#include "InputActions.hpp"
#include "ContainerProcessor.hpp"
#include "main.hpp"



namespace ContainerProcessor {



bool in_container = false;
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
bool item_choice_request_sent = false;
bool item_choice_valid = false;
int item_choice = -1;
bool check_results = false;
bool catch_pickpocket_result = false;




struct item_data {
	std::string name;
	std::string price;
	std::string weight;
	std::string damage;
	std::string armor;
	bool is_stealing;
	int chance_to_steal;
	//int price; //TODO: replace with something useful (maybe type of item)
};


std::map<int, item_data> items_list{};

enum way_to_fill {
	down,
	up,
	downup,
	updown
};

way_to_fill way{};

















bool is_inside_of_category()
{
	bool result = false;
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::ContainerMenu>();
	RE::GFxValue var1;

	if (ui && menu && ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
		if (menu->uiMovie)
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.focused"))
				if (!var1.IsNull() && var1.IsNumber())
					result = var1.GetNumber();

	return result;
}


bool is_pickpocketing()
{
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::ContainerMenu>();

	auto container_mode = menu->GetContainerMode();

	return container_mode == RE::ContainerMenu::ContainerMode::kPickpocket;
}



std::vector<MenuOption> get_items_options()
{
	std::vector<MenuOption> result{};

	for (std::pair<int, item_data> item : items_list)
	{
		std::string steal_text = "";
		if (item.second.is_stealing)
			steal_text = ". Is stealing";

		std::string price_text = "";
		if (item.second.price != "")
			price_text = ". Value: " + item.second.price;
		std::string weight_text = "";
		if (item.second.weight != "")
			weight_text = ". Weight: " + item.second.weight;
		std::string damage_text = "";
		if (item.second.damage != "")
			damage_text = ". Damage: " + item.second.damage;
		std::string armor_text = "";
		if (item.second.armor != "")
			armor_text = ". Armor: " + item.second.armor;

		std::string steal_chance_text = "";

		RE::NiPointer<RE::TESObjectREFR> container_refr_p{};
		if (is_pickpocketing())
			if (item.second.chance_to_steal != -1)
				steal_chance_text = ". " + std::to_string(item.second.chance_to_steal) + "% chance to steal";

		


		MenuOption option = { item.first, item.second.name + price_text + weight_text + damage_text + armor_text + steal_text + steal_chance_text };
		result.push_back(option);
	}

	if (!is_pickpocketing())
		result.push_back({ -2, "[TAKE ALL]" });

	result.push_back({ -1, "[STOP LOOTING]" });
	

	return result;
}



std::string get_force_message()
{
	std::string result = "You opened a container. Select item to take. ";

	RE::UI* ui = RE::UI::GetSingleton();
	RE::GFxValue var1;

	auto menu = ui->GetMenu<RE::ContainerMenu>();

	if (ui && menu && ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
		if (menu->uiMovie)
		{
			auto container_mode = menu->GetContainerMode();
			auto target_handle = menu->GetTargetRefHandle();

			RE::NiPointer<RE::TESObjectREFR> container_refr_p{};

			if (RE::LookupReferenceByHandle(target_handle, container_refr_p))
			{
				auto container_refr = container_refr_p.get();

				if (container_refr)
				{
					std::string name = container_refr->GetDisplayFullName();
					std::string action = "You are looting ";

					if (container_mode == RE::ContainerMenu::ContainerMode::kPickpocket)
						action = "You are pickpocketing ";
					if (container_mode == RE::ContainerMenu::ContainerMode::kLoot)
						action = "You are looting ";
					if (container_mode == RE::ContainerMenu::ContainerMode::kSteal)
						action = "You are stealing from ";

					result = action + name + ". Select item to take. ";
				}
				
			}



			
		}
			


	return result;
}




int get_item_scroll_position()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
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
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iMaxScrollPosition"))
					return var1.GetNumber();
	return -1;
}

int get_item_selected_index()
{
	int result = -1;
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::ContainerMenu>();
	RE::GFxValue var1;

	if (ui && menu && ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
		if (menu->uiMovie)
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.iSelectedIndex"))
				if (!var1.IsNull() && var1.IsNumber())
					result = var1.GetNumber();

	return result;
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




//_root.Menu_mc.InventoryLists_mc._ItemsList.Entry9.textField.text = Salt Pile (4)

//REDO THIS. IT DOESNT ADD UP AT ALL




void update_items_list()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList"))
				{
					std::string path_root = "_root.Menu_mc.InventoryLists_mc._ItemsList.";
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

									bool is_stealing = text_color < 16750000;


									RE::GFxValue subvar;

									std::string price = "";
									std::string weight = "";
									std::string damage = "";
									std::string armor = "";

									if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ItemValueText.text"))
										if (!subvar.IsNull() && subvar.IsString())
											price = subvar.GetString();
									if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ItemWeightText.text"))
										if (!subvar.IsNull() && subvar.IsString())
											weight = subvar.GetString();
									if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.WeaponDamageValue.text"))
										if (!subvar.IsNull() && subvar.IsString())
											damage = subvar.GetString();
									if (menu->uiMovie->GetVariable(&subvar, "_root.Menu_mc.ItemCard_mc.ApparelArmorValue.text"))
										if (!subvar.IsNull() && subvar.IsString())
											armor = subvar.GetString();

									int chance_to_steal = -1;
									RE::GFxValue chance_var;
									if (menu->uiMovie->GetVariable(&chance_var, "_root.Menu_mc.ItemCardFadeHolder_mc.StealTextInstance.PercentTextInstance.text"))
										if (!chance_var.IsNull() && chance_var.IsString())
										{
											std::string chance_string = chance_var.GetString();
											
											auto percent_pos = chance_string.find("%");
											if (percent_pos != std::string::npos)
											{
												chance_string = chance_string.substr(0, percent_pos);

												chance_to_steal = std::stoi(chance_string);

												bool test = false;
											}


										}
									// = 39%TO STEAL



									if (name != "")
									{
										if (result.id == get_item_selected_index())
										{
											item_data data{};
											data.name = name;
											data.price = price;
											data.weight = weight;
											data.damage = damage;
											data.armor = armor;
											data.is_stealing = is_stealing;
											data.chance_to_steal = chance_to_steal;
											items_list.insert({ result.id, data });
										}

									}
								}
							}
						}
					}
				}
}

void update_items_list2()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.EntriesA"))
				{
							if (!var1.IsNull() && var1.IsArray())
							{
								for (int i = 0; i < var1.GetArraySize(); i++)
								{
									RE::GFxValue var_element;
									
									if (var1.GetElement(i, &var_element))
									{
										RE::GFxValue var_index;
										RE::GFxValue var_text_field;

										std::vector<std::string> names{};

										if (var_element.IsObject())
										{
											var_element.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
												{
													std::string name_str = name;
													std::string name_orig = name;

													if (!a_value.IsNull())
													{
														if (a_value.IsObject())
															name_str += "{obj}";
														else
														{
															if (a_value.IsArray())
																name_str += "[" + std::to_string(a_value.GetArraySize()) + "]";
															if (a_value.IsNumber())
																name_str += " = " + std::to_string(a_value.GetNumber());
															if (a_value.IsBool())
																name_str += " = " + std::to_string(a_value.GetBool());
															if (a_value.IsString())
															{
																std::string string_val = a_value.GetString();
																name_str += " = " + string_val;
															}
														}
													}
													else
														int isnullwtf = 1;

													names.push_back(name_str);

												});
										}

										//text
										//isStealing
										//clipIndex

										if (var_element.GetMember("clipIndex", &var_index) && 
											var_element.GetMember("text", &var_text_field))
										{
											int index = var_index.GetNumber();
											if (index == get_item_selected_index())
											{
												//_root.Menu.InventoryLists._ItemsList.Entry8.textField.textColor = 16777215.000000
																							//_root.Menu.InventoryLists._ItemsList.Entry9.textField.textColor =  5000268.000000

												RE::GFxValue var_text;
												RE::GFxValue vat_text_color;

												if (var_text_field.GetMember("text", &var_text) &&
													var_text_field.GetMember("textColor", &vat_text_color))
												{

													if (!var_index.IsNull() && var_index.IsNumber() &&
														!var_text.IsNull() && var_text.IsString() &&
														!vat_text_color.IsNull() && vat_text_color.IsNumber())
													{
														MenuOption result{};
														result.id = index;
														std::string name = var_text.GetString();
														int text_color = vat_text_color.GetNumber();


														if (name != "")
														{
															if (result.id == get_item_selected_index())
															{
																item_data data{};
																data.name = name;
																data.is_stealing = false; //TODO

																items_list.insert({ result.id, data });
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
	scroll_stuck = false; 

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





void reset_pickpocketing()
{
	catch_pickpocket_result = false;
}

void reset_container()
{
	in_container = false;
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
	item_choice_request_sent = false;
	item_choice_valid = false;
	item_choice = -1;
	check_results = false;

	items_list.clear();




}



std::pair<bool, std::string> set_item_choice(int id)
{
	std::pair<bool, std::string> result{};

	if (id == -1)
	{
		quit_menu();
		result.first = true;
		result.second = "[Stopped looting]";
		return result;
	}

	if (id == -2)
	{
		//take all
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::ContainerMenu>();
		if (menu && menu->uiMovie)
		{
			set_universal_block(1.0f);
			//menu->uiMovie->Invoke("_root.Menu_mc.BottomBar_mc.Button0.onPress", nullptr, nullptr, 0);
			//menu->uiMovie->Invoke("_root.Menu_mc.BottomBar_mc.Button0.onRelease", nullptr, nullptr, 0);

			ready_weapon();

			result.first = true;
			result.second = "[Taking all...]";
		}
		else
		{
			result.first = true;
			result.second = "[Error]";
		}

	}
	else
	{
		if (items_list_valid)
		{
			if (items_list.find(id) == items_list.end())
			{
				result.first = false;
				result.second = "Invalid item ID";
			}
			else
			{
				item_choice = id;
				item_choice_valid = true;
				result.first = true;
				result.second = "[Processing...]";
			}
		}
		else
		{
			result.first = true;
			result.second = "[Error]";
		}
	}

	return result;
}


bool quit_menu()
{
	RE::UIMessageQueue::GetSingleton()->AddMessage(RE::ContainerMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
	return true;
}


bool is_container_empty()
{
	return false;


	bool result = true;

	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::ContainerMenu>();
	RE::GFxValue var1;

	if (ui && menu && ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
		if (menu->uiMovie)
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc._ItemsList.EntriesA"))
			{
				if (!var1.IsNull() && var1.IsArray())
				{
					int size = var1.GetArraySize();
					result = size < 1;
				}
			}
					

	return result;
}



float container_processor_timer = 0.0f;


void processor(float dtime)
{

	RE::UI* ui = RE::UI::GetSingleton();

	//std::string test = "";

	RE::GFxValue var1;

	in_container = ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME);

	auto menu = ui->GetMenu<RE::ContainerMenu>();
	

	if (catch_pickpocket_result)
	{
		if (in_container)
			send_random_context("[Successfully stole an item]");
		else
			send_random_context("[You have been caught]");

		catch_pickpocket_result = false;

	}




	if (container_processor_timer > 0.01f)
	{
		container_processor_timer = 0.0f;

		if (in_container)
		{
			WalkerProcessor::reset_walker();

			if (!is_inside_of_category() && !is_container_empty() && !check_results)
				right();// confirm();
			else
			{   //NOW ITEMS

				if (is_container_empty())
				{
					if (!item_choice_request_sent)
					{

						if (force_choice(get_items_options(), "You opened a container. It is empty. Send -1 to exit. ", force_type::container_item))
						{
							missing_item_detected = false;
							last_cursor_move = 0;
							item_choice_request_sent = true;
						}
					}
				}



				if (!items_list_valid && !check_results)
					if (!filling_items)
					{
						setup_fill_item_list();
						filling_items = true;
					}
					else
						fill_items_list(0.02f);
				else
				{

					if (!item_choice_request_sent && !check_results)
					{

						if (force_choice(get_items_options(), get_force_message(), force_type::container_item))
						{
							missing_item_detected = false;
							last_cursor_move = 0;
							item_choice_request_sent = true;
						}
					}
					else
					{
						if (item_choice_valid)
						{
							int selected_index = get_item_selected_index();
							if ((selected_index != item_choice) && !check_results)
							{
								if (!missing_item_detected)
									move_to_item(item_choice);
								else
									reset_container();
							}
							else
							{
								if (!check_results)
								{
									check_results = true;
									//this is [R]Craft button
									//menu->uiMovie->Invoke("_root.Menu.BottomBarInfo.Buttons.3.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
									confirm();
									set_universal_block(1.0f);
									std::string result = get_result_message();
									if (result != "")
										send_random_context("[" + result + "]");
								}
								else
								{
									if (is_pickpocketing())
										catch_pickpocket_result = true;

									reset_container();
								}
							}
						}
					}
				}
			}

			
		}
		else
		{
			reset_container();
		}	
	}
	else
		container_processor_timer += dtime;
}


};
