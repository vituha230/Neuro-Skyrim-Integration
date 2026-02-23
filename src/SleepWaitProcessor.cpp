



//TODO: make lock move faster (need to send inputs more frequently - DIDNT WORK???)
//TODO: lock level info 
//TODO: it looks a bit like shit.. need to smooth it somehow

#include "InputActions.hpp"
#include "SleepWaitProcessor.hpp"
#include "main.hpp"


namespace SleepWaitProcessor {



	bool in_sleepwait = false;
	
	bool sleepwait_request_sent = false;
	bool sleepwait_choice_valid = false;
	int sleepwait_choice = -1;
	bool done = false;
	bool was_sleeping = false;


	//DO NOT RESET ON NORMAL RESET
	bool try_calling_waitmenu = false;


	bool catch_call_wait_result()
	{
		bool result = false;

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
													result = true;
							}

						}
		return result;
	}



	bool quit_menu()
	{
		bool result = true;

		RE::UIMessageQueue::GetSingleton()->AddMessage(RE::SleepWaitMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);

		return result;
	}


	std::pair<bool, std::string> call_wait_menu()
	{
		std::pair<bool, std::string> result{};

		result.first = true;
		result.second = "[Calling wait menu...]";

		try_calling_waitmenu = true;

		return result;
	}



	bool is_sleeping()
	{
		bool result = true;

		//_root.SleepWaitMenu_mc.QuestionInstance.text = Wait how long?
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::SleepWaitMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.SleepWaitMenu_mc.QuestionInstance.text"))
						if (!var1.IsNull() && var1.IsString())
						{
							std::string var1_string = var1.GetString();
							if (var1_string.find("Wait how long") != std::string::npos)
								result = false;
						}
							
		return result;
	}



	std::pair<bool, std::string> set_sleepwait_choice(int choice)
	{
		std::pair<bool, std::string> result{};

		std::string sleep_or_wait = "";
		std::string action = "";

		if (is_sleeping())
		{
			sleep_or_wait = "Sleep";
			action = "Sleeping";
		}
		else
		{
			sleep_or_wait = "Wait";
			action = "Waiting";
		}
			


		if (choice == -1)
		{
			quit_menu();
			result.first = true;
			result.second = "[" + sleep_or_wait + " menu closed]";
			return result;
		}


		if (choice > 0 && choice <= 24)
		{
			sleepwait_choice = choice;
			sleepwait_choice_valid = true;
			result.first = true;
			result.second = "[" + action + "...]";
		}
		else
		{
			result.first = false;
			result.second = "Invalid choice";
		}

		return result;
	}


	std::string get_current_time()
	{
		std::string result = "";

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::SleepWaitMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.SleepWaitMenu_mc.CurrentTime.text"))
						if (!var1.IsNull() && var1.IsString())
							result = var1.GetString();


		return result;
	}



	std::string get_force_message()
	{
		std::string sleep_or_wait = "Wait";
		std::string action = "wait";
		if (is_sleeping())
		{
			sleep_or_wait = "Sleep";
			action = "sleep";
		}
		

		std::string result = "You are in " + sleep_or_wait + " menu. Choose how many hours to " + action + ". Current date and time: " + get_current_time() + ". You can send -1 to cancel " + sleep_or_wait + " menu.";

		return result;
	}


	std::string get_choice_range()
	{
		std::string result = "from 1 to 24";

		return result;
	}


	int get_slider_position()
	{
		int result = -1;
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::SleepWaitMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.SleepWaitMenu_mc.HoursSlider._value"))
						if (!var1.IsNull() && var1.IsNumber())
							result = var1.GetNumber();

		return result;
	}



	void move_slider(int desired_pos)
	{
		int selected_index = get_slider_position();

		if (selected_index > desired_pos)
			left();
		if (selected_index < desired_pos)
			right();
	}



	void reset_menu()
	{
		in_sleepwait = false;


		sleepwait_request_sent = false;
		sleepwait_choice_valid = false;
		sleepwait_choice = -1;
		done = false;
		was_sleeping = false;

	}


	void saveload_reset()
	{
		try_calling_waitmenu = false;
	}


	float sleepwait_processor_timer = 0.0f;

	void processor(float dtime)
	{

		RE::UI* ui = RE::UI::GetSingleton();

		in_sleepwait = ui->IsMenuOpen(RE::SleepWaitMenu::MENU_NAME);

		auto menu = ui->GetMenu<RE::SleepWaitMenu>();

		
		if (try_calling_waitmenu)
		{
			if (catch_call_wait_result() || in_sleepwait)
				try_calling_waitmenu = false;
			else
				press_t();
		}


		if (sleepwait_processor_timer > 0.01f )
		{
			sleepwait_processor_timer = 0.0f;

			if (in_sleepwait)
			{
				if (!sleepwait_request_sent)
				{
					sleepwait_request_sent = true;
					force_choice({}, get_force_message() + "Valid range: " + get_choice_range(), force_type::sleepwait_time);
				}
				else
				{
					if (sleepwait_choice_valid && !done)
					{
						if (get_slider_position() != sleepwait_choice)
						{
							move_slider(sleepwait_choice);
						}
						else
						{
							//confirm();
							if (menu->uiMovie)
							{
								menu->uiMovie->Invoke("_root.SleepWaitMenu_mc.ButtonRect.AcceptMouseButton.onPress", nullptr, nullptr, 0);
								menu->uiMovie->Invoke("_root.SleepWaitMenu_mc.ButtonRect.AcceptMouseButton.onRelease", nullptr, nullptr, 0);
								done = true;

								if (is_sleeping())
									was_sleeping = true;
							}
						}
					}
				}
			}
			else
			{
				if (done)
					if (was_sleeping)
						send_random_context("[You woke up]");
					else
						send_random_context("[Waiting is over]");

				reset_menu();
			}

		}
		else
			sleepwait_processor_timer += dtime;
	}


}