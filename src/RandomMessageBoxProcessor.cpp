

//TODO: all new opened menus reset_walking


#include "RandomMessageBoxProcessor.hpp"
#include "main.hpp"



namespace RandomMessageBoxProcessor {

	bool message_box_open = false;
	float handeled_timeout = 0.0f;
	bool is_handeled = false;

	bool message_box_request_sent = false;
	bool message_box_choice_valid = false;
	int message_box_choice = -1;
	bool rolled_over = false;


	struct option {
		int button_id;
		std::string text;
	};

	std::vector<option> options;



	void set_messagebox_handeled()
	{
		is_handeled = true;
	}


	std::pair<bool, std::string> set_message_box_choice(int choice)
	{
		std::pair<bool, std::string> result{};
		bool choice_valid = false;

		for (auto option : options)
		{
			if (option.button_id == choice)
			{
				choice_valid = true;
				break;
			}

		}

		if (choice_valid)
		{
			message_box_choice = choice;
			message_box_choice_valid = true;
			result.first = true;
			result.second = "[Processing...]";
		}
		else
		{
			result.first = false;
			result.second = "Invalid choice ID";
		}


		return result;
	}


	std::vector<MenuOption> get_options()
	{
		std::vector<MenuOption> result{};

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::MessageBoxMenu>();
		if (menu)
		{
			std::string root_path = "_root.MessageMenu.Buttons.Button";


			for (int i = 0; i < 10; i++)
			{
				std::string path = root_path + std::to_string(i);

				RE::GFxValue var1;

				if (menu->uiMovie->GetVariable(&var1, path.c_str()))
				{
					RE::GFxValue var_text;
					if (menu->uiMovie->GetVariable(&var_text, (path + ".ButtonText.text").c_str()))
						if (!var_text.IsNull() && var_text.IsString())
						{
							option the_option{};
							the_option.button_id = i;
							the_option.text = var_text.GetString();
							options.push_back(the_option);

							MenuOption option = { i, var_text.GetString()};
							result.push_back(option);
						}
				}
			}
		}

		return result;
	}



	void reset_menu()
	{
		message_box_open = false;
		handeled_timeout = 0.0f;
		is_handeled = false;

		message_box_request_sent = false;
		message_box_choice_valid = false;
		message_box_choice = -1;
		rolled_over = false;
		options.clear();

	}




	float walker_processor_timer = 0.0f;

	void processor(float dtime)
	{
		RE::UI* ui = RE::UI::GetSingleton();

		message_box_open = ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME);


		if (message_box_open)
		{
			if (!is_handeled)
			{
				if (handeled_timeout > 1.5f)
				{
					RE::GFxValue var1;
					auto menu = ui->GetMenu(RE::MessageBoxMenu::MENU_NAME);
					if (menu->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
					{
						std::string text = "";
						if (!var1.IsNull() && var1.IsString())
							text = var1.GetString();




						if (!message_box_request_sent)
						{
							auto option_test = get_options();

							if (std::size(option_test) == 1)
							{
								send_random_context("[" + text + "]");
								set_message_box_choice(option_test.at(0).id);
								message_box_request_sent = true;
							}
							else
							{
								if (text.find("Survival Mode") != std::string::npos)
								{
									set_message_box_choice(1);
									message_box_request_sent = true;
								}
								else
									if (force_choice(get_options(), text, force_type::messagebox_option))
										message_box_request_sent = true;
							}

							
							
						}
						else
						{
							if (message_box_choice_valid)
							{
								std::string index = std::to_string(message_box_choice);

								if (!rolled_over)
								{
									menu->uiMovie->Invoke(("_root.MessageMenu.Buttons.Button" + index + ".onRollOver").c_str(), nullptr, nullptr, 0);
									rolled_over = true;
									set_universal_block(1.0f);
								}
								else
								{
									rolled_over = false;
									menu->uiMovie->Invoke(("_root.MessageMenu.Buttons.Button" + index + ".onPress").c_str(), nullptr, nullptr, 0);
									set_universal_block(1.0f);
									reset_menu();
									//supposedly we exit the menu now
								}
							}
						}
					}
				}
				else
					handeled_timeout += dtime;
			}


		}
		else
			reset_menu();

	}

}

