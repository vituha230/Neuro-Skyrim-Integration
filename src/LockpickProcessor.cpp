



//TODO: make lock move faster (need to send inputs more frequently - DIDNT WORK???)
//TODO: lock level info 
//TODO: it looks a bit like shit.. need to smooth it somehow

#include "InputActions.hpp"
#include "LockpickProcessor.hpp"
#include "main.hpp"


namespace LockpickProcessor {


	bool in_lockpicking = false;


	int angle_choice = 0.0f;
	bool angle_choice_request_sent = false;
	bool angle_choice_valid = false;
	int max_lock_angle = 0;
	float lock_not_moving_time = 0.0f;
	int max_lock_angle_stuck = 0;
	bool move_onetime = false;
	bool was_stuck = false;
	bool victory = false;
	bool do_lock_moving = false;


	void reset_lockpicking()
	{
		angle_choice = 0.0f;
		angle_choice_request_sent = false;
		angle_choice_valid = false;
		max_lock_angle = 0;
		lock_not_moving_time = 0.0f;
		max_lock_angle_stuck = 0;
		move_onetime = false;
		was_stuck = false;
		do_lock_moving = false;

	}


	bool quit_menu()
	{
		RE::UIMessageQueue::GetSingleton()->AddMessage(RE::LockpickingMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		return true;
	}


	std::pair<bool, std::string> set_angle_choice(int angle)
	{
		std::pair<bool, std::string> result{};


		if (angle == -1000)
		{
			quit_menu();
			result.first = true;
			result.second = "[Stopped lockpicking]";
			return result;
		}

		if (angle <= 90 && angle >= -90)
		{
			angle_choice = angle;
			angle_choice_valid = true;
			result.first = true;
			result.second = "[Trying to pick the lock...]";
		}
		else
		{
			result.first = false;
			result.second = "Invalid angle choice";
		}
		return result;
	}


	bool quit_lockpicking()
	{
		RE::UIMessageQueue::GetSingleton()->AddMessage(RE::LockpickingMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		return true;
	}


	//TODO: figure out what these numbers mean.


	/*
	void mouse_left()
	{
		RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
		RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(-15.0, 0.0);
		//RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
		set_allowed_events(1);

		//auto bsInputEventQueue = RE::BSInputEventQueue::GetSingleton();
		//bsInputEventQueue->ClearInputQueue();
		//static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);
		//static auto kEvent2 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 0.0f, holdTime);
		//bsInputEventQueue->PushOntoInputQueue(kEvent1);
		///bsInputEventQueue->PushOntoInputQueue(kEvent2);
		//set_allowed_events(1);

	}
	*/



	/*
	+[0]	"_root.LockpickingMenu_mc.fPickMaxAngle = 90.000000"	std::string

	_root.LockpickingMenu_mc.fPickMaxAngle
	_root.LockpickingMenu_mc.fPickMixAngle

	*/





	bool detect_pick_broke()
	{
		bool result = false;
		if (const auto ui = RE::UI::GetSingleton(); ui)
			if (const auto menu = ui->GetMenu<RE::LockpickingMenu>(); menu)
				if (const auto pickBreak = menu->pickBreak; pickBreak)
				{
					if (pickBreak->state == RE::NiControllerSequence::AnimState::kAnimating)
						result = true;
				}

		return result;
	}

	float get_pick_damage()
	{
		float result = 0.0f;
		if (const auto ui = RE::UI::GetSingleton(); ui)
			if (const auto menu = ui->GetMenu<RE::LockpickingMenu>(); menu)
				if (const auto pickDamage = menu->pickDamage; pickDamage)
				{
					result = pickDamage->offset;
				}

		return result;
	}


	std::string get_lock_level_text()
	{
		std::string result = "";

		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::LockpickingMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.LockpickingMenu_mc.InfoRect_mc.LockLevelText.text"))
						if (!var1.IsNull() && var1.IsString())
							result = var1.GetString();

		return result;
	}

	int get_pick_angle() //maybe use floats?
	{
		int result = 0.0f;
		if (const auto ui = RE::UI::GetSingleton(); ui)
			if (const auto menu = ui->GetMenu<RE::LockpickingMenu>(); menu)
				result = menu->pickAngle;

		return result;
	}

	int get_lock_angle() //maybe use floats?
	{
		int result = 0.0f;
		if (const auto ui = RE::UI::GetSingleton(); ui)
			if (const auto menu = ui->GetMenu<RE::LockpickingMenu>(); menu)
				result = menu->lockAngle;

		return result;
	}



	int get_picks_amount_int()
	{
		int result = 0;

		const auto inv = RE::PlayerCharacter::GetSingleton()->GetInventory([](RE::TESBoundObject& a_object)
			{
				return a_object.IsLockpick();// a_object.IsObject();
			});


		std::string lockpick_name = "Lockpick";

		for (auto& [item, data] : inv)
		{
			if (data.first > 0 && item->GetName() != lockpick_name)
			{
				result = 999; //i think there are only normal lockpicks and that one infinite lockpick.. so if its not "Lockpick" then its infinite
				break;
			}
			else
				result = data.first;
		}

		return result;
	}


	std::string get_picks_amount()
	{
		std::string result = "0";

		const auto inv = RE::PlayerCharacter::GetSingleton()->GetInventory([](RE::TESBoundObject& a_object)
			{
				return a_object.IsLockpick();// a_object.IsObject();
			});


		std::string lockpick_name = "Lockpick";

		for (auto& [item, data] : inv)
		{
			if (data.first > 0 && item->GetName() != lockpick_name)
			{
				result = "infinite"; //i think there are only normal lockpicks and that one infinite lockpick.. so if its not "Lockpick" then its infinite
			}
			else
				result = std::to_string(data.first);
		}

		return result;
	}




	bool detect_win()
	{
		if (get_lock_angle() == 90)
		{
			victory = true;
			return true;
		}


		return false;
	}

	bool is_lock_stuck(float dtime)
	{
		int lock_angle = get_lock_angle();
		if (lock_not_moving_time > 0.2f)
		{
			lock_not_moving_time = 0.0f;
			return true;
		}
		else
		{
			if (lock_angle > max_lock_angle_stuck + 5)
			{
				lock_not_moving_time = 0.0f;
				max_lock_angle_stuck = lock_angle;
			}
			else
			{
				lock_not_moving_time += dtime;
			}
		}

		return false;

		/*
		bool result = false;
		if (const auto ui = RE::UI::GetSingleton(); ui)
			if (const auto menu = ui->GetMenu<RE::LockpickingMenu>(); menu)
				if (const auto pickDamage = menu->pickDamage; pickDamage)
				{
					//menu is ready.
					if (pickDamage->offset - starting_offset > 5.0f)
						result = true;
				}

		return result;

		*/
	}




	void move_pick_to_angle(int angle_choice)
	{
		int current_angle = get_pick_angle();

		float ang_dif = (float)(angle_choice - current_angle);
		float bonus = 0.0f;

		if (ang_dif < 0)
			bonus = -1.0f;
		else
			bonus = 1.0f;



		float angle_to_move = bonus * (sqrt((abs(ang_dif) / 5.0f) + 1.0f) * 6.0f);

		if (abs(ang_dif) < angle_to_move)
			angle_to_move = ang_dif;


		mouse_mouse_x(angle_to_move);


		/*
		if (current_angle < angle_choice)
			mouse_right();
		if (current_angle > angle_choice)
			mouse_left();

			*/
	}





	float lockpicking_processor_timer = 0.0f;

	void processor(float dtime)
	{

		RE::UI* ui = RE::UI::GetSingleton();

		in_lockpicking = ui->IsMenuOpen(RE::LockpickingMenu::MENU_NAME);

		auto menu = ui->GetMenu<RE::LockpickingMenu>();

		//mouse_right();

		if (do_lock_moving)
			forward_lockpick();



		if (lockpicking_processor_timer > 0.005f)
		{
			lockpicking_processor_timer = 0.0f;

			if (in_lockpicking)
			{
				if (angle_choice_valid)
				{
					if (detect_pick_broke()) //this check acts funny so do it in 2 places
					{
						send_random_context("[The lockpick broke]");
						reset_lockpicking();
						if (get_picks_amount_int() == 0)
							send_random_context("[Out of lockpicks. Exiting lockpicking menu]");
					}
					else
					{
						if (get_pick_angle() != angle_choice)
							move_pick_to_angle(angle_choice);
						else
						{
							if (detect_pick_broke()) //this check acts funny so do it in 2 places
							{
								send_random_context("[The lockpick broke]");
								reset_lockpicking();
							}
							else
							{
								//if (menu && menu->unk10A)
								if (!is_lock_stuck(dtime) && !was_stuck)
								{
									if (max_lock_angle < get_lock_angle())
										max_lock_angle = get_lock_angle();

									do_lock_moving = true;

									if (detect_win())
									{
										do_lock_moving = false;
										cancel_forward_lockpick();
										send_random_context("[Successfully picked the lock]");
										reset_lockpicking();
									}


								}
								else
								{
									do_lock_moving = false;
									was_stuck = true;
									cancel_forward_lockpick();
									//it stopped moving, wait for it to return to original position and reset
									if (get_lock_angle() < 45.0f)
									{
										send_random_context(("[Attempt failed. The lock only rotated by " + std::to_string(max_lock_angle) + " degrees (has to reach 90 to open)]").c_str());
										reset_lockpicking(); //MAYBE GIVE DIFFERENT PROMPT ON RETRY?
									}
								}
							}
						}
					}
				}
				else
					if (!angle_choice_request_sent && !victory)
					{
						std::string lock_level_text = get_lock_level_text();
						if (lock_level_text != "")
						{
							
							if (force_choice({}, "You are lockpicking. You have " + get_picks_amount() + " lockpicks. The lock level is: " + lock_level_text + ". Choose angle of pick to try. Valid range: from -90 to 90. You can send -1000 to quit lockpicking. ", force_type::lockpick_angle))
								angle_choice_request_sent = true;
						}

					}

			}
			else
			{
				reset_lockpicking();
				victory = false;
			}

		}
		else
			lockpicking_processor_timer += dtime;
	}



}

