



//TODO: make lock move faster (need to send inputs more frequently - DIDNT WORK???)
//TODO: lock level info 
//TODO: it looks a bit like shit.. need to smooth it somehow

#include "InputActions.hpp"
#include "BookProcessor.hpp"
#include "main.hpp"


namespace BookProcessor {



	bool in_book = false;
	bool book_request_sent = false;
	bool book_choice_valid = false;
	bool book_choice = -1;
	bool start_turning_pages = false;
	bool done = false;
	bool catch_result = false;
	float turn_page_time = 0.0f;
	float catch_result_time = 0.0f;
	bool sent_quit_force = false;
	float try_taking_again_time = 0.0f;
	float in_book_time = 0.0f;
	bool book_text_sent = false;
	std::string last_book_type = "book";

	bool quit_menu()
	{
		bool result = true;

		RE::UIMessageQueue::GetSingleton()->AddMessage(RE::BookMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);

		return result;
	}


	bool is_note()
	{
		bool result = false;

		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::BookMenu>(); menu)
			{
				result = menu->isNote;
			}
							
		return result;
	}




	std::string get_book_text()
	{
		std::string result = "";

		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::BookMenu>(); menu)
			{
				RE::BSString the_string = menu->GetDescription();
				result = the_string;

				result = MiscThings::replace_aliases_all_quests(result);
			}

		return result;
	}




	std::string get_book_name()
	{
		std::string result = "";


		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::BookMenu>(); menu)
			{
				auto name = menu->GetTargetForm();

				if (name)
				{
					result = name->GetFullName();

					result = MiscThings::replace_aliases_all_quests(result);
				}
					
			}

		return result;
	}


	std::string book_type()
	{
		if (is_note())
		{
			return "note";
		}
		else
			return "book";
	}

	std::string get_force_message()
	{
		std::string book_or_note = book_type();

		
		std::string raw_descr = get_book_text();
		MiscThings::clean_controls_from_string(&raw_descr);

		std::string result = "You are reading a " + book_or_note + ": " + get_book_name() + "...\n[" + MiscThings::fix_book_description(raw_descr) + "]";


		return result;
	}



	std::string get_take_steal_text()
	{
		std::string result = "";

		//_root.BottomBar.TakeButton.onPress

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::BookMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.BottomBar.TakeButton._label"))
						if (!var1.IsNull() && var1.IsString())
						{
							std::string var1_string = var1.GetString();
							if (var1_string.length() > 0)
								result = var1_string.substr(1, var1_string.length() - 1);
						}
		return result;
	}


	void take_steal_book()
	{
		confirm();
		/*
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::BookMenu>();
		if (menu && menu->uiMovie)
		{
			menu->uiMovie->Invoke("_root.BottomBar.TakeButton.onPress", nullptr, nullptr, 0);
			menu->uiMovie->Invoke("_root.BottomBar.TakeButton.onRelease", nullptr, nullptr, 0);
		}
		*/
	}


	std::vector<MenuOption> get_options()
	{
		std::vector<MenuOption> options{};

		if (MiscThings::has_thrown_a_book())
		{
			options.push_back({ 1, "Stop reading" }); //fake take
		}
		else
		{
			std::string bonus = get_take_steal_text();

			std::string book_or_note = "book";
			if (is_note())
			{
				book_or_note = "note";
			}


			if (bonus != "")
				options.push_back({ 1, "Stop reading and " + bonus + " the " + book_or_note });

			options.push_back({ -1, "Stop reading" });
		}


		return options;
	}



	std::pair<bool, std::string> set_book_choice(int choice)
	{
		std::pair<bool, std::string> result{};

		auto ui = RE::UI::GetSingleton();
		if (!ui->IsMenuOpen(RE::BookMenu::MENU_NAME))
		{
			result.first = true;
			result.second = "[Error]";
			return result;
		}

		if (choice == 1)
		{
			book_choice = choice;
			book_choice_valid = true;
			result.first = true;
			result.second = "[Processing...]";
		}
		else
		{
			if (choice == -1)
			{
				result.first = true;
				result.second = "[Stopped reading]";
				quit_menu();
			}
			else
			{
				result.first = false;
				result.second = "Invalid choice";
			}
		}


		return result;

	}




	void reset_menu()
	{
		in_book = false;
		book_request_sent = false;
		book_choice_valid = false;
		book_choice = -1;
		start_turning_pages = false;
		done = false;
		catch_result = false;
		turn_page_time = 0.0f;
		catch_result_time = 0.0f;
		sent_quit_force = false;
		try_taking_again_time = 0.0f;

		in_book_time = 0.0f;
		book_text_sent = false;

		last_book_type = "book";
	}


	void turn_page()
	{
		left_attack();
	}

	
	float book_processor_timer = 0.0f;

	void processor(float dtime)
	{

		RE::UI* ui = RE::UI::GetSingleton();

		in_book = ui->IsMenuOpen(RE::BookMenu::MENU_NAME);

		auto menu = ui->GetMenu<RE::BookMenu>();

		
		if (catch_result)
		{
			if (catch_result_time > 4.0f)
			{
				if (!sent_quit_force)
				{
					
					std::vector<MenuOption> quit_option = { {-1, "Stop reading" } };

					if (force_choice(quit_option, "[Failed to take the " + book_type() + "]", force_type::book))
					{
						sent_quit_force = true;
					}
				}
			}
			else
			{
				if (try_taking_again_time > 0.90f)
				{
					try_taking_again_time = 0.0f;
					take_steal_book();
				}
				else
					try_taking_again_time += dtime;
				
				catch_result_time += dtime;
			}
				
		}



		if (start_turning_pages)
		{
			if (turn_page_time > 3.0f)
			{
				turn_page_time = 0.0f;
				turn_page();
			}
			else
				turn_page_time += dtime;
		}


		if (in_book)
			in_book_time += dtime;


		if (book_processor_timer > 0.01f )
		{
			book_processor_timer = 0.0f;

			if (in_book)
			{
				if (in_book_time > 1.5f)
				{
					if (!book_text_sent)
					{
						book_text_sent = true;
						send_random_context(get_force_message(), false);
					}
					else
					{
						start_turning_pages = true;

						if (in_book_time > 7.0f)
						{
							if (!book_request_sent)
							{

								if (force_choice(get_options(), "Choose what to do next. ", force_type::book))
									book_request_sent = true;
							}
							else
							{
								if (book_choice_valid && !done)
								{
									if (book_choice == 1)
									{
										
										if (!MiscThings::has_thrown_a_book())
											send_random_context("[Trying to take the " + book_type() + "...]");
										else
											send_random_context("[Closing the " + book_type() + "...]", false);

										take_steal_book();

										set_universal_block(1.5f);

										catch_result = true;
										last_book_type = book_type();
										done = true;
										//reset_menu();
									}
								}
							}
						}
					}
					
				}
				
			}
			else
			{
				if (done)
					if (!MiscThings::has_thrown_a_book())
						send_random_context("[Took the " + last_book_type + "]", false);

				reset_menu();
			}

		}
		else
			book_processor_timer += dtime;
	}


}