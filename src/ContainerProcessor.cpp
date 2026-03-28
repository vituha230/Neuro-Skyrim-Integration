

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
bool close_empty_container = false;
std::vector<int> item_choice_array{};
int old_item_list_size = -1;
bool refresh_items_list = false;
bool process_next_item_after_refresh = false;


bool slider_request_sent = false;
bool slider_choice_valid = false;
int slider_choice = -1;
bool slider_confirmed = false;
bool slider_confirming = false;
bool quantity_was_specified = false;


int amount_skips = 0;

bool missing_category_detected = false;

float move_cursor_timeout0 = 0.0f;
float move_cursor_timeout1 = 0.0f;
float move_cursor_timeout2 = 0.0f;

float bad_pos_time = 0.0f;

int last_selected_index = 0;

std::vector<std::string> item_names_to_skip{};



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



bool is_possessions_chest()
{
	RE::UI* ui = RE::UI::GetSingleton();
	RE::GFxValue var1;

	if (ui)
	{
		auto menu = ui->GetMenu<RE::ContainerMenu>();

		if (menu)
		{
			auto container_ref_handle = menu->GetTargetRefHandle();

			auto container_ref_ptr = RE::TESObjectREFR::LookupByHandle(container_ref_handle);

			if (container_ref_ptr && container_ref_ptr.get())
			{
				auto container_ref = container_ref_ptr.get();

				auto extra_list = &container_ref->extraList;

				if (extra_list)
				{
					auto extra_alias = (RE::ExtraAliasInstanceArray*)extra_list->GetByType(RE::ExtraDataType::kAliasInstanceArray);

					if (extra_alias)
					{
						for (auto alias : extra_alias->aliases)
						{
							if (alias && alias->alias)
							{
								std::string alias_name = alias->alias->aliasName.c_str();
								std::string evidence = "EvidenceChest";

								if (alias_name == evidence)
								{
									return true;
								}

							}
						}
					}
				}
			}
		}
	}

	return false;
}


std::vector<MenuOption> get_items_options()
{
	std::vector<MenuOption> result{};

	if (!is_pickpocketing())
		result.push_back({ -2, "[TAKE ALL]" });


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




	if (is_possessions_chest() && !is_pickpocketing())
	{
		result.clear();
		result.push_back({ -2, "[TAKE ALL]" });
	}


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


int get_item_selected_position()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc.ItemsListHolder.List_mc.iSelectedIndex"))
					return var1.GetNumber();

	return -1;
}

std::string get_item_selected_name()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.ItemName.text"))
					return var1.GetString();

	return "";
}



void move_to_position(int index)
{
	int selected_item = get_item_selected_position();

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

	items_list.clear();
	items_list_valid = false;

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




bool is_malborn()
{

	RE::UI* ui = RE::UI::GetSingleton();

	if (ui)
	{
		auto menu = ui->GetMenu<RE::ContainerMenu>();
		if (menu)
		{
			auto target_ref_handle = menu->GetTargetRefHandle();

			auto target_ref_ptr = RE::TESObjectREFR::LookupByHandle(target_ref_handle);

			if (target_ref_ptr && target_ref_ptr.get())
			{
				auto target_ref = target_ref_ptr.get();

				//33f46
				auto malborn = RE::TESObjectREFR::LookupByID(0x39f1e);


				if (target_ref == malborn)
					return (target_ref->IsActor() && !target_ref->IsDead());

			}
		}
	}

	return false;
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
									{
										RE::GFxValue subvar_text;
										if (subvar.GetMember("text", &subvar_text))
											if (subvar_text.IsString())
											{
												result = subvar_text.GetString();

												if (is_malborn())
												{
													subvar_text.SetString("");
													subvar.SetMember("text", subvar_text);
													//var1.SetArraySize(0);
													//var1.RemoveElements(0, size);
													//bool test_result = menu->uiMovie->SetVariable("_root.HUDMovieBaseInstance.MessagesInstance.ShownMessageArray", var1, RE::GFxMovie::SetVarType::kNormal);
													//bool stop_here = false;
												}
											}
									}					
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
	close_empty_container = false;

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

	item_choice_array.clear();

	old_item_list_size = -1;

	refresh_items_list = false;
	process_next_item_after_refresh = false;


	slider_request_sent = false;
	slider_choice_valid = false;
	slider_choice = -1;
	slider_confirmed = false;
	slider_confirming = false;
	quantity_was_specified = false;


	missing_category_detected = false;
	move_cursor_timeout0 = 0.0f;
	move_cursor_timeout1 = 0.0f;
	move_cursor_timeout2 = 0.0f;

	amount_skips = 0;


	last_selected_index = 0;

	item_names_to_skip.clear();

	bad_pos_time = 0.0f;
}








void process_next_item()
{
	if (std::size(item_choice_array) <= 0)
		reset_container();
	else
	{
		item_choice = item_choice_array.at(0);
		item_choice_array.erase(item_choice_array.begin());


		if (item_choice == -1)
		{
			quit_menu();
			reset_container();
			return;
		}

		if (item_choice == -2 && is_pickpocketing())
		{
			item_choice_valid = false;
			process_next_item();
			return;
		}

		if (item_choice == -2)
		{
			//take all
			set_universal_block(1.0f);
			ready_weapon();
			reset_container();
			return;
		}
		else
		{
			if (items_list_valid)
			{
				if (items_list.find(item_choice) == items_list.end())
				{
					item_choice_valid = false;
					process_next_item();
					return;
				}
				else
				{
					item_choice_valid = true;
					return;
				}
			}
			else
			{
				reset_container();
				return;
			}
		}
	}
}



std::pair<bool, std::string> set_item_choice_array(std::vector<int> ids)
{
	std::pair<bool, std::string> result{};

	auto ui = RE::UI::GetSingleton();
	if (!ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
	{
		result.first = true;
		result.second = "[Error]";
		return result;
	}


	//check take all
	for (auto id : ids)
	{
		if (id == -2 && !is_pickpocketing())
		{
			set_universal_block(1.0f);
			ready_weapon();

			result.first = true;
			result.second = "[Taking all...]";
			return result;
		}
	}


	if (std::size(ids) <= 0)
	{
		result.first = false;
		result.second = "Invalid choice";
		return result;
	}


	item_choice_array = ids;

	process_next_item();
	result.first = true;
	result.second = "[Processing...]";

	return result;
}






std::pair<bool, std::string> set_item_choice(int id)
{
	std::pair<bool, std::string> result{};

	auto ui = RE::UI::GetSingleton();
	if (!ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
	{
		result.first = true;
		result.second = "[Error]";
		return result;
	}

	if (id == -1)
	{
		quit_menu();
		result.first = true;
		result.second = "[Stopped looting]";
		return result;
	}

	if (id == -2 && is_pickpocketing())
	{
		result.first = false;
		result.second = "Invalid item ID";
		return result;
	}


	if (id == -2)
	{
		//take all
		
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
	send_random_context("[Container closed]", false);
	RE::UIMessageQueue::GetSingleton()->AddMessage(RE::ContainerMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
	return true;
}


bool is_container_empty()
{

	//auto options = get_items_options();

	//return std::size(options) <= 2;

	
	return false;

	/*
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
	*/				

	//return result;
	
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
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._minimum"))
					if (!var1.IsNull() && var1.IsNumber())
						return var1.GetNumber();
	return -1;
}

int get_slider_max()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._maximum"))
					if (!var1.IsNull() && var1.IsNumber())
						return var1.GetNumber();
	return -1;
}

int get_slider_pos()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._value"))
					if (!var1.IsNull() && var1.IsNumber())
						return var1.GetNumber();
	return -1;
}


bool quantity_slider_active()
{
	bool result = false;
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.ItemCard_mc.QuantitySlider_mc._focused"))
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



std::pair<bool, std::string> set_slider_choice(int pos)
{
	std::pair<bool, std::string> result{};

	auto ui = RE::UI::GetSingleton();
	if (!ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
	{
		result.first = true;
		result.second = "[Error]";
		return result;
	}

	if (pos == -1)
	{
		quit_menu();
		result.first = true;
		result.second = "[Container closed]";
		return result;
	}

	
	if (pos != 0 && (pos < get_slider_min() || pos > get_slider_max() || !item_choice_valid))
	{
		result.first = false;
		result.second = "Invalid slider position ID";
		return result;
	}

	slider_choice = pos;
	slider_choice_valid = true;

	result.first = true;
	result.second = "[Processing...]";
	return result;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




std::string get_item_text_by_id(int id)
{
	auto item_pos = items_list.find(id);

	if (item_pos == items_list.end())
		return "";
	else
		return item_pos->second.name;
}











int get_category_selected_index()
{
	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	if (ui)
		if (const auto menu = ui->GetMenu<RE::ContainerMenu>(); menu)
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.Menu_mc.InventoryLists_mc.CategoriesListHolder.List_mc.iSelectedIndex"))
					return var1.GetNumber();

	return -1;
}



void move_cursor_to_category(int id)
{
	int id_selected = get_category_selected_index();

	if (last_cursor_move != 1 && last_cursor_move != 2)
		bool test = true;


	if (is_inside_of_category())
		left();
	else
	{
		if (id_selected > id)
		{
			if (last_cursor_move == 2)
				missing_category_detected = true;
			last_cursor_move = 1;
			cursor_up();
		}
		if (id_selected < id)
		{
			if (last_cursor_move == 1)
				missing_category_detected = true;
			last_cursor_move = 2;
			cursor_down();
		}
	}

	
}







float container_processor_timer = 0.0f;



void processor(float dtime)
{
	//return;
	RE::UI* ui = RE::UI::GetSingleton();

	//std::string test = "";

	RE::GFxValue var1;

	in_container = ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME);

	auto menu = ui->GetMenu<RE::ContainerMenu>();
	

	if (catch_pickpocket_result)
	{
		if (in_container)
			send_random_context("[Successfully stole an item]", false);
		else
			send_random_context("[You have been caught]", false);

		catch_pickpocket_result = false;

	}




	if (container_processor_timer > 0.01f)
	{
		container_processor_timer = 0.0f;

		if (in_container)
		{
			if (close_empty_container)
			{
				cancel();
			}
				//quit_menu();

			WalkerProcessor::reset_walker();


			bool category_done = false;
			int category_choice = 0;

			if (is_malborn())
			{
				Observer::set_dont_inform_inventory();
				category_choice = 11;
			}

			if (get_category_selected_index() != category_choice)
			{
				if (!missing_category_detected)
				{
					move_cursor_to_category(category_choice);
					if (move_cursor_timeout0 > 10.0f)
					{
						reset_container();
						return;
					}
					else
						move_cursor_timeout0 += 0.02f;
				}
				else
				{
					move_cursor_timeout0 = 0.0f;
					reset_container();
				}

			}
			else
			{
				if (!is_inside_of_category() && !is_container_empty() && !check_results)
					right();// confirm();
				else
				{   //NOW ITEMS

					if (refresh_items_list)
					{
						refresh_items_list = false;
						items_list_valid = false;
					}

					if (!is_malborn() && !items_list_valid && !check_results)
						if (!filling_items)
						{
							setup_fill_item_list();
							filling_items = true;
						}
						else
						{
							fill_items_list(0.02f);

							if (move_cursor_timeout1 > 10.0f)
							{
								reset_container();
								return;
							}
							else
								move_cursor_timeout1 += 0.02f;
						}

					else
					{
						move_cursor_timeout1 = 0.0f;
						filling_items = false;
						if (!check_results)
						{
							int new_items_list_size = std::size(items_list);

							if (new_items_list_size < old_item_list_size)
							{
								for (auto& a_choice : item_choice_array)
								{
									if (a_choice > 0 && a_choice > item_choice)
										a_choice -= 1;
								}

								quantity_was_specified = false;
							}
							else
							{
								if (old_item_list_size != -1 && !quantity_was_specified)
								{
									//we took something but it didnt change the list. this means there are more items of that kind we just picked up. take all
									process_next_item_after_refresh = false;
									quantity_was_specified = false;
								}
							}



							old_item_list_size = new_items_list_size;
						}

						if (process_next_item_after_refresh)
						{
							process_next_item_after_refresh = false;
							process_next_item();
							return;
						}


						if (!is_malborn() && !item_choice_request_sent && !check_results)
						{

							auto options = get_items_options();

							if (std::size(options) <= 2 && !is_possessions_chest())
							{
								send_random_context("[Container is empty. Closing container...]");
								//if (force_choice(get_items_options(), "You opened a container. It is empty. Send -1 to exit. ", force_type::container_item))
								missing_item_detected = false;
								last_cursor_move = 0;
								item_choice_request_sent = true;
								close_empty_container = true;

								set_universal_block(2.0f);

								return;
							}


							if (is_malborn())
							{
								missing_item_detected = false;
								last_cursor_move = 0;
								item_choice_request_sent = true;

								std::vector<int> all_ids{};

								bool first = true;
								for (auto option : options)
								{
									if (option.id != -2)
										all_ids.push_back(option.id);
								}

								set_item_choice_array(all_ids);

							}
							else
							{
								auto force_type = force_type::container_item;

								if (!is_pickpocketing())
									force_type = force_type::container_item_array;

								if (force_choice(options, get_force_message(), force_type))
								{
									missing_item_detected = false;
									last_cursor_move = 0;
									item_choice_request_sent = true;
								}
							}
						}
						else
						{
							if (is_malborn() || item_choice_valid)
							{
								int selected_index = get_item_selected_index();
								if (!is_malborn() && (selected_index != item_choice) && !check_results)
								{
									if (!missing_item_detected)
									{
										move_to_item(item_choice);
										if (move_cursor_timeout2 > 10.0f)
										{
											reset_container();
											return;
										}
										else
											move_cursor_timeout2 += 0.02f;
									}
									else
									{
										move_cursor_timeout2 = 0.0f;
										reset_container();
									}
								}
								else
								{
									move_cursor_timeout2 = 0.0f;
									if (!check_results)
									{
										
										//this is [R]Craft button
										//menu->uiMovie->Invoke("_root.Menu.BottomBarInfo.Buttons.3.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all

										//RE::ItemList* old_item_list = menu->itemCard;

										old_item_list_size = std::size(items_list);


										//if (get_item_selected_position() != last_selected_index)
										//	amount_skips = get_item_selected_position();


										if (is_malborn())
										{
											bool bad_pos = false;

											int cur_pos = get_item_selected_index();

											for (auto skip_name : item_names_to_skip)
											{
												if (get_item_selected_name() == skip_name)
												{
													cursor_down();
													perk_down();
													bad_pos = true;
													bad_pos_time += 0.02f;

													if (bad_pos_time > 3.0f)
													{
														send_random_context("You gave Malborn all your items so he can smuggle them into the embassy...");
														Observer::reset_dont_inform_inventory();
														close_empty_container = true;
													}
														
													return;
												}
													
											}

											if (!bad_pos)
											{
												ready_weapon();
												bad_pos_time = 0.0f;
											}
												
										}
										else
											confirm();

										if (!is_malborn())
											set_universal_block(0.3f);

										std::string result = get_result_message();
										if (result != "")
										{
											if (!is_malborn())
												send_random_context("[" + result + "]", false);
											else
											{
												if (result.find("Quest Items cannot") != std::string::npos)
												{
													item_names_to_skip.push_back(get_item_selected_name());
												}
													
											}
										}

										check_results = true;
											
									}
									else
									{
										if (quantity_slider_active())
										{
											
											if (!is_malborn())
											{
												confirm();
												set_universal_block(0.3f);
											}
											else
											{
												confirm_fast();
											}

											return;

											if (!slider_request_sent)
											{
												if (force_choice({}, "Choose amount of " + get_item_text_by_id(item_choice) + " to take. Valid range: from " +
													std::to_string(get_slider_min()) + " to " + std::to_string(get_slider_max()), force_type::container_quantity))
													slider_request_sent = true;

												return;
											}
											else
											{
												if (slider_choice_valid)
												{
													quantity_was_specified = true;
													if (slider_choice > 0)
													{
														if (get_slider_pos() != slider_choice)
														{
															move_slider_to_pos(slider_choice);
															return;
														}
														else
														{
															if (!slider_confirmed)
															{
																confirm();
																slider_confirming = true;
																return;
															}
															else
															{
																return;
																;//put some watchdog here
															}
														}
													}

													cancel();
													set_universal_block(0.3f);
													return;
												}
												else
													return;
											}
										}

										slider_request_sent = false;
										slider_choice_valid = false;
										slider_choice = -1;
										slider_confirmed = false;
										slider_confirming = false;


										{
											//no slider or slider done

											check_results = false;

											if (is_pickpocketing())
											{
												reset_container();
												catch_pickpocket_result = true;
											}
											else
											{
												refresh_items_list = true;
												process_next_item_after_refresh = true;
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
			reset_container();
		}	
	}
	else
		container_processor_timer += dtime;
}


};
