



#include "InputActions.hpp"
#include "AlchemyProcessor.hpp"
#include "main.hpp"


namespace AlchemyProcessor {



bool in_alchemy = false;

bool alchemy_request_sent = false;
bool ingredient_choice_valid = false;
std::vector<int> ingredient_choice{};
bool rolled_over_alchemy = false;
bool ingredient_list_valid = false;
int alchemy_category_choice = 0;
bool check_results = false;
bool ask_for_more_request_sent = false;
bool ask_for_more_choice_valid = false;
int ask_for_more = 0;
bool quitting_menu = false;

void reset_alchemy()
{
	alchemy_request_sent = false;
	ingredient_choice_valid = false;
	ingredient_choice.clear();
	rolled_over_alchemy = false;
	ingredient_list_valid = false;
	alchemy_category_choice = 0;
	check_results = false;
	ask_for_more_request_sent = false;
	ask_for_more_choice_valid = false;
	ask_for_more = 0;
	quitting_menu = false;
}

//_root.Menu.CategoryList._ItemsList.iSelectedIndex = -1.000000 - category selected (-1 when not selected)
//_root.Menu.ItemList.iSelectedIndex = 1.000000 - item selected
//


std::pair<bool, std::string> choose_craft_more(int amount_more)
{
	std::pair<bool, std::string> result{};

	if (amount_more == -1)
	{
		quit_menu();
		result.first = true;
		result.second = "[Stopped alchemy]";
		return result;
	}


	if (amount_more > ask_for_more || amount_more < 0)
	{
		result.first = false;
		result.second = "Invalid amount";
	}
	else
	{
		if (amount_more == 0)
		{
			result.first = true;
			result.second = "[Processing...]";
			reset_alchemy();
		}
		else
		{
			ask_for_more = amount_more;
			ask_for_more_choice_valid = true;
			result.first = true;
			result.second = "[Processing...]";
		}
	}

	
	return result;
}


std::pair<bool, std::string> choose_ingredients(std::vector<int> ingredients)
{
	std::pair<bool, std::string> result{};


	std::sort(std::begin(ingredients), std::end(ingredients));


	int amount = std::size(ingredients);

	if (amount > 0 && ingredients.at(0) == -1)
	{
		quit_menu();
		result.first = true;
		result.second = "[Stopped alchemy]";
		return result;
	}

	if (amount > 3 || amount < 2)
	{
		result.first = false;
		result.second = "Invalid number of ingredients";
		return result;
	}

	if (ingredients[0] == ingredients[1])
	{
		result.first = false;
		result.second = "Ingredients must be different";
		return result;
	}

	if (amount == 3 && ingredients[1] == ingredients[2] || ingredients[0] == ingredients[2])
	{
		result.first = false;
		result.second = "Ingredients must be different";
		return result;
	}


	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	if (menu && menu->subMenu)
	{
		auto alchemy_menu = (RE::CraftingSubMenus::AlchemyMenu*)menu->subMenu;
		int size = std::size(alchemy_menu->ingredientEntries);

		for (auto ingredient : ingredients)
		{
			if (ingredient < size && ingredient >= 0)
				ingredient_choice.push_back(ingredient);
			else
			{
				result.first = false;
				result.second = "Invalid ingredient ID";
				return result;
			}
		}

		ingredient_choice_valid = true;
		result.first = true;
		result.second = "[Making the potion...]";
	}

	return result;
}

//_root.Menu.CategoryList._CategoriesList.iSelectedIndex = 1.000000

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

int get_selected_item_index()
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



//effectfilterID in ingredient_entries looks like index of menu category.
//so each item has 4 effects and 4 filterIDs to be displayed in a category.




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
	int selected_item = get_selected_item_index();

	if (is_inside_of_category())
	{
		if (selected_item > index)
			cursor_up();

		if (selected_item < index)
			cursor_down();
	}
}




std::vector<MenuOption> get_ingredient_options()
{
	std::vector<MenuOption> result{};

	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	if (menu && menu->subMenu)
	{
		auto alchemy_menu = (RE::CraftingSubMenus::AlchemyMenu*)menu->subMenu;
		//std::vector<RE::InventoryEntryData*> ingredients{};
		//std::vector<std::string> ingredient_string_list{};

		int i = 0;
		for (auto ingredient_entry : alchemy_menu->ingredientEntries)
		{
			auto ingredient = ingredient_entry.ingredient;

			std::string ingredient_name = ingredient->GetDisplayName();

			MenuOption option = { i, ingredient_name + " x" + std::to_string(ingredient->countDelta) };
			result.push_back(option);

			i++;
		}

		result.push_back({ -1, "[QUIT ALCHEMY]" });
	}

	return result;
}




bool is_ingredient_selected(int index)
{
	bool result = false;

	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	if (menu && menu->subMenu)
	{
		auto alchemy_menu = (RE::CraftingSubMenus::AlchemyMenu*)menu->subMenu;

		if (index < std::size(alchemy_menu->ingredientEntries))
		{
			result = alchemy_menu->ingredientEntries[index].isSelected;
		}
	}

	return result;
}



std::string get_alchemy_result_message()
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
		quitting_menu = true;

		set_universal_block(1.0f);
	}

	return result;
}




int get_amount_of_ingredient_by_index(int index)
{
	int result = 0;

	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::CraftingMenu>();
	if (menu && menu->subMenu)
	{
		auto alchemy_menu = (RE::CraftingSubMenus::AlchemyMenu*)menu->subMenu;
		if (index < std::size(alchemy_menu->ingredientEntries))
		{
			result = alchemy_menu->ingredientEntries[index].ingredient->countDelta;
		}
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
		auto alchemy_menu = (RE::CraftingSubMenus::AlchemyMenu*)menu->subMenu;
		auto selected_indexes = alchemy_menu->selectedIndexes;

		if (std::size(selected_indexes) != 0)
		{
			//still have some selected. ask if we want to make some more;
			int max_amount_more = 9999;
			for (auto selected_index : selected_indexes)
			{
				int amount = get_amount_of_ingredient_by_index(selected_index);

				if (amount < max_amount_more)
					max_amount_more = amount;
			}

			if (max_amount_more != 9999)
				result = max_amount_more;

		}
	}

	return result;
}




//"_root.Menu.MenuDescription.text = Alchemy: Combine ingredients to make potions",
//"_root.Menu.BottomBarInfo.PlayerInfoCard_mc.SkillLevelLabel.text = Alchemy",

float alchemy_processor_timer = 0.0f;

void processor(float dtime)
{

	RE::UI* ui = RE::UI::GetSingleton();

	auto menu = ui->GetMenu<RE::CraftingMenu>();

	//std::string test = "";

	RE::GFxValue var1;

	in_alchemy = false;

	if (ui && menu && ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
		if (menu->uiMovie && menu->subMenu)
			if (menu->uiMovie->GetVariable(&var1, "_root.Menu.MenuDescription.text"))
				if (!var1.IsNull() && var1.IsString())
				{
					std::string menu_name = var1.GetString();
					if (menu_name == "Alchemy: Combine ingredients to make potions")
					{
						in_alchemy = true;
						auto alchemy_menu = (RE::CraftingSubMenus::AlchemyMenu*)menu->subMenu;
					}
				}

	if (alchemy_processor_timer > 0.005f)
	{
		alchemy_processor_timer = 0.0f;

		if (in_alchemy)
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

							if (var1_string == "Quit alchemy?")
							{
								if (!rolled_over_alchemy)
								{

									menu_confirm_quit->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
									rolled_over_alchemy = true;
									set_universal_block(2.0f);
								}
								else
								{
									rolled_over_alchemy = false;
									menu_confirm_quit->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
									set_universal_block(1.5f);
									reset_alchemy();

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
				if (ask_for_more > 0)
				{
					if (!ask_for_more_request_sent)
					{
						ask_for_more_request_sent = true;
						force_choice({}, "You can craft more potions of this kind. Choose how many more to craft. Valid range: from 0 to " + std::to_string(ask_for_more), force_type::alchemy_amount_more);
					}
					else
					{
						if (ask_for_more_choice_valid)
						{
							menu->uiMovie->Invoke("_root.Menu.BottomBarInfo.Buttons.3.onPress", nullptr, nullptr, 0);
							std::string result = get_alchemy_result_message();
							if (result != "")
								send_random_context("[" + result + "]");

							ask_for_more--;

							if (ask_for_more < 1)
								reset_alchemy();

							set_universal_block(0.5f);
						}
					}
				}
				else
				{
					if (auto menu_confirm = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm)
					{
						RandomMessageBoxProcessor::set_messagebox_handeled();

						if (!rolled_over_alchemy)
						{
							if (menu_confirm->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
								if (!var1.IsNull() && var1.IsString())
								{
									std::string var_string = var1.GetString();
									send_random_context("[" + var_string + "]");
								}
									


							menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
							rolled_over_alchemy = true;
							set_universal_block(2.0f);
						}
						else
						{
							rolled_over_alchemy = false;
							menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
							set_universal_block(1.5f);

							int make_more = how_many_more_can_make();

							if (make_more > 0)
								ask_for_more = make_more;
							else
								reset_alchemy();
						}
					}
					else
					{
						if (get_selected_category_index() != alchemy_category_choice) //for npw the category is fixed as 0. will work unless we have too many items?
							move_to_category(alchemy_category_choice);
						else
						{
							if (!is_inside_of_category())
								right();
							else
							{
								if (!alchemy_request_sent)
								{
									alchemy_request_sent = true;
									force_choice(get_ingredient_options(), "You are in alchemy menu. Choose 2 or 3 ingredients to combine. Ingredients must be different.", force_type::alchemy_ingredients);
								}
								else
								{
									if (ingredient_choice_valid)
									{

										auto alchemy_menu = (RE::CraftingSubMenus::AlchemyMenu*)menu->subMenu;
										auto selected_indexes = alchemy_menu->selectedIndexes;
										bool have_wrong_indexes_selected = false;

										for (auto selected_index : selected_indexes)
										{
											if (std::find(ingredient_choice.begin(), ingredient_choice.end(), selected_index) == ingredient_choice.end())
											{
												have_wrong_indexes_selected = true;
												if (is_ingredient_selected(selected_index))
												{
													if (get_selected_item_index() != selected_index)
													{
														move_to_item(selected_index);
													}
													else
													{
														confirm();
													}
													break;
												}
											}
										}

										if (!have_wrong_indexes_selected)
										{
											bool have_unselected_choices = false;
											for (int ingredient_subchoice : ingredient_choice)
											{
												if (!is_ingredient_selected(ingredient_subchoice))
												{
													have_unselected_choices = true;
													if (get_selected_item_index() != ingredient_subchoice)
													{
														move_to_item(ingredient_subchoice);
													}
													else
													{
														confirm();
													}

													break;
												}
											}


											if (!have_unselected_choices)
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
													std::string result = get_alchemy_result_message();
													if (result != "")
														send_random_context("[" + result + "]");

													int make_more = how_many_more_can_make();

													if (make_more > 0)
														ask_for_more = make_more;
													else
														reset_alchemy();

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
			reset_alchemy();
		}	
	}
	else
		alchemy_processor_timer += dtime;
}


}