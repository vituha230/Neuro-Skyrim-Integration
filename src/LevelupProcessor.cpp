

#include "InputActions.hpp"
#include "LevelupProcessor.hpp"
#include "main.hpp"
#include "Misc.hpp"



namespace LevelupProcessor {

	bool in_levelup = false;

	bool leveled_up_attribute = false;
	bool attribute_request_sent = false;
	bool paused_before_confirm = false;
	bool confirmed = false;
	bool rolled_over = false;


	float levelup_timer = 0.0f;
	float levelup_timer2 = 0.0f;


	bool reset_levelup()
	{
		leveled_up_attribute = false;
		attribute_request_sent = false;
		paused_before_confirm = false;
		confirmed = false;
		rolled_over = false;

		levelup_timer = 0.0f;
		levelup_timer2 = 0.0f;

		return true;
	}


	std::pair<bool, std::string> levelup_attritube(int attribute)
	{
		std::pair<bool, std::string> result{};

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::LevelUpMenu>();

		if (in_levelup && menu) //TODO: return not bool but informative message on what was the error if there was one
		{
			switch (attribute)
			{
			case 1:
			{
				if (menu->uiMovie->Invoke("_root.LevelUpMenu_mc.HealthButton.onPress", nullptr, nullptr, 0))
				{
					result.first = true;
					result.second = "[Upgrading health...]";
				}
				else
				{
					result.first = true;
					result.second = "[Error]";
				}
				break;
			}
			case 2:
			{
				if (menu->uiMovie->Invoke("_root.LevelUpMenu_mc.MagickaButton.onPress", nullptr, nullptr, 0))
				{
					result.first = true;
					result.second = "[Upgrading magicka...]";
				}
				else
				{
					result.first = true;
					result.second = "[Error]";
				}
			}
			case 3:
			{
				if (menu->uiMovie->Invoke("_root.LevelUpMenu_mc.StaminaButton.onPress", nullptr, nullptr, 0))
				{
					result.first = true;
					result.second = "[Upgrading stamina...]";
				}
				else
				{
					result.first = true;
					result.second = "[Error]";
				}
				break;
			}
			default:
			{
				result.first = false;
				result.second = "Invalid attribute ID";
				break;
			}

			}
		}
		else
		{
			result.first = true;
			result.second = "Not in levelup menu";
		}

		if (result.first)
			leveled_up_attribute = true; //notify processor

		return result;
	}



	float levelup_processor_timer = 0.0f;

	void processor(float dtime)
	{

		RE::UI* ui = RE::UI::GetSingleton();
		in_levelup = ui->IsMenuOpen(RE::LevelUpMenu::MENU_NAME);
		auto menu = ui->GetMenu<RE::LevelUpMenu>();


		if (levelup_processor_timer > 0.1f)
		{
			levelup_processor_timer = 0.0f;







			if (in_levelup)
			{
				//WalkerProcessor::reset_walker();

				if (!attribute_request_sent)
				{
					
					if (force_choice({ {1, "Health"},{2, "Magicka"},{3, "Stamina"} }, "You leveled up. Choose an attribute to increase. ", force_type::levelup_attribute))
						attribute_request_sent = true;
				}
				else
				{
					if (leveled_up_attribute)
					{
						if (!paused_before_confirm)
						{
							paused_before_confirm = true;
							set_universal_block(1.0f);
						}
						else
						{
							if (auto menu_confirm = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm)
							{
								RandomMessageBoxProcessor::set_messagebox_handeled();

								if (!rolled_over)
								{
									menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
									rolled_over = true;
									set_universal_block(0.5f);
								}
								else
								{
									menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
									set_universal_block(0.5f);
									reset_levelup();
									//the rest is in StatsMenu (perks)
								}
							}
						}
					}
				}
			}
			else
			{
				bool entered_levelup = ui->IsMenuOpen(RE::LevelUpMenu::MENU_NAME) || ui->IsMenuOpen(RE::StatsMenu::MENU_NAME);

				if (MiscThings::player_has_levelup() && !entered_levelup)
				{
					if (levelup_timer > 6.0f)
					{
						if (!MiscThings::is_game_paused())
							MiscThings::pause_game();
						else
							if (levelup_timer2 > 0.5f)
							{
								confirm();
								reset_levelup();
							}
							else
							{
								levelup_timer2 += dtime + 0.1f;
								cursor_up();
							}
					}
					else
						levelup_timer += dtime + 0.1;

				}
				else
					reset_levelup();
			}

		}
		else
			levelup_processor_timer += dtime;


	}

}

