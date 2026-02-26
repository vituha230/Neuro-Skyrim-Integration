



//TODO: make lock move faster (need to send inputs more frequently - DIDNT WORK???)
//TODO: lock level info 
//TODO: it looks a bit like shit.. need to smooth it somehow

#include "InputActions.hpp"
#include "TrainingProcessor.hpp"
#include "main.hpp"


namespace TrainingProcessor {



	bool in_training = false;
	
	bool training_request_sent = false;
	bool training_choice_valid = false;
	int training_choice = -1;
	//bool done = false;
	//bool was_sleeping = false;


	//DO NOT RESET ON NORMAL RESET
	//bool try_calling_waitmenu = false;


	bool quit_menu()
	{
		bool result = true;

		RE::UIMessageQueue::GetSingleton()->AddMessage(RE::TrainingMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);

		return result;
	}




	std::string get_your_skill()
	{
		std::string result;
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::TrainingMenu>();
		RE::GFxValue var1;
		if (ui && menu && ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME))
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.TrainingMenuObj.SkillName.text"))
					if (!var1.IsNull() && var1.IsString())
					{
						std::string var1_string = var1.GetString();

						auto last_space_pos = var1_string.find_last_of(" ");
						if (last_space_pos != std::string::npos)
						{
							result = var1_string.substr(last_space_pos + 1, var1_string.length() - last_space_pos - 1);
						}
						else
							result = "0";
					}


		return result;
	}


	std::string get_skill_name()
	{
		std::string result;
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::TrainingMenu>();
		RE::GFxValue var1;
		if (ui && menu && ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME))
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.TrainingMenuObj.SkillName.text"))
					if (!var1.IsNull() && var1.IsString())
					{
						std::string var1_string = var1.GetString();

						auto last_space_pos = var1_string.find_last_of(" ");
						if (last_space_pos != std::string::npos)
						{
							result = var1_string.substr(0, last_space_pos);
						}
						else
							result = var1_string;
					}


		return result;
	}

	std::string get_current_gold()
	{
		std::string result;
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::TrainingMenu>();
		RE::GFxValue var1;
		if (ui && menu && ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME))
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.TrainingMenuObj.CurrentGold.text"))
					if (!var1.IsNull() && var1.IsString())
						result = var1.GetString();

		return result;
	}


	std::string get_train_cost()
	{
		std::string result;
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::TrainingMenu>();
		RE::GFxValue var1;
		if (ui && menu && ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME))
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.TrainingMenuObj.TrainCost.text"))
					if (!var1.IsNull() && var1.IsString())
						result = var1.GetString();

		return result;
	}


	std::string get_times_trained()
	{
		std::string result;
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::TrainingMenu>();
		RE::GFxValue var1;
		if (ui && menu && ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME))
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.TrainingMenuObj.TimesTrained.text"))
					if (!var1.IsNull() && var1.IsString())
						result = var1.GetString();

		return result;
	}


	std::string get_trainer_skill()
	{
		std::string result;
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::TrainingMenu>();
		RE::GFxValue var1;
		if (ui && menu && ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME))
			if (menu->uiMovie)
				if (menu->uiMovie->GetVariable(&var1, "_root.TrainingMenuObj.TrainerSkill.text"))
					if (!var1.IsNull() && var1.IsString())
					{
						result = var1.GetString();

						if (result == "Common")
							result += " (max 50)";

						if (result == "Expert")
							result += " (max 75)";

						if (result == "Master")
							result += " (max 90)";
					}


		return result;
	}







	std::string cant_train_reason()
	{
		std::string result = "";
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::TrainingMenu>();
		if (menu)
		{
			auto player = RE::PlayerCharacter::GetSingleton();
			std::string trainerName = menu->trainer->GetDisplayFullName();
			std::string skillName = get_skill_name();
			std::string trainerSkill = get_trainer_skill();
			int yourSkill = std::stoi(get_your_skill());
			int trainCost = std::stoi(get_train_cost());
			int currentGold = std::stoi(get_current_gold());
			std::string timesTrained = get_times_trained();

			int trainer_skill_int = 0;
			if (trainerSkill == "Common (max 50)")
				trainer_skill_int += 50;

			if (trainerSkill == "Expert (max 75)")
				trainer_skill_int += 75;

			if (trainerSkill == "Master (max 90)")
				trainer_skill_int += 90;


			int times_trained_int = timesTrained.at(0) - '0';

			if (trainCost > currentGold)
				result = "You dont have enough gold";
			else
				if (yourSkill >= trainer_skill_int)
					result = trainerName + " can not teach you anymore. Your skill is too high. ";
				else
					if (times_trained_int >= 5)
						result = "You already took maximum lessons on this level";
		}

		return result;
	}


	std::pair<bool, std::string> set_training_choice(int choice)
	{
		std::pair<bool, std::string> result{};

		if (choice == -1)
		{
			quit_menu();
			result.first = true;
			result.second = "[Training menu closed]";
			return result;
		}

		if (choice == 1)
		{
			auto reason = cant_train_reason();

			if (reason == "")
			{
				training_choice = choice;
				training_choice_valid = true;
				result.first = true;
				result.second = "[Training...]";
			}
			else
			{
				result.first = false;
				result.second = reason;
			}

		}
		else
		{
			result.first = false;
			result.second = "Invalid choice";
		}

		return result;
	}




	std::string get_force_message()
	{
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::TrainingMenu>();
		
		std::string result = "[You are in training menu. Choose action]";
		if (menu)
		{

			auto player = RE::PlayerCharacter::GetSingleton();
			std::string trainerName = menu->trainer->GetDisplayFullName();
			std::string skillName = get_skill_name();			//menu->skillName.GetString();
			std::string trainerSkill = get_trainer_skill();		//menu->trainerSkill.GetString();
			std::string yourSkill = get_your_skill();						// std::to_string(player->GetActorValue(menu->skill));
			std::string trainCost = get_train_cost();			// menu->trainCost.GetString();
			std::string currentGold = get_current_gold();		// menu->currentGold.GetString();
			std::string timesTrained = get_times_trained();		// menu->timesTrained.GetString();

			result = "[" + trainerName + " teaches " + skillName + ". Trainer skill: " + trainerSkill + ". Your skill: " + yourSkill + ". Lesson cost: " + trainCost + ". Your gold: " + currentGold + ". You trained " + timesTrained + " times. Choose action]";

		}
		
		return result;
	}




	void reset_menu()
	{
		in_training = false;
		training_request_sent = false;
		training_choice_valid = false;
		training_choice = -1;
	}

	float training_processor_timer = 0.0f;

	void processor(float dtime)
	{
		//return; 
		RE::UI* ui = RE::UI::GetSingleton();

		in_training = ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME);

		auto menu = ui->GetMenu<RE::TrainingMenu>();

		
		if (training_processor_timer > 0.01f )
		{
			training_processor_timer = 0.0f;

			if (in_training)
			{
				if (!training_request_sent)
				{
					
					if (force_choice({ {1, "Take a lesson"},{-1, "[QUIT TRAINING MENU]"} }, get_force_message(), force_type::training_choice))
						training_request_sent = true;
				}
				else
				{
					if (training_choice_valid)
					{
						if (training_choice == 1)
						{
							confirm();
							reset_menu();
						}
						else
						{
							reset_menu();
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
			training_processor_timer += dtime;
	}


}