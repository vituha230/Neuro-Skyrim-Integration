

//TODO: fix local map

#include "InputActions.hpp"
#include "MapProcessor.hpp"
#include "main.hpp"


namespace MapProcessor {


	bool in_map = false;
	float in_map_timer = 0.0f; //needs time to load DO NOT RESET IN RESET 

	bool location_request_sent = false;
	bool location_choice_valid = false;
	int location_choice = -1;
	bool rolled_over = false;
	bool catch_result = false;
	float catch_result_timer = 0.0f;
	bool start_moving = false;
	bool undiscovered_travel_request_sent = false;
	bool undiscovered_travel_choice_valid = false;
	int undiscovered_travel_choice = -1;

	bool wait_and_quit_and_walk_to_marker = false;
	float wait_and_quit_and_walk_to_marker_time = 0.0f;

	bool idk_what_to_do = false;


	float cursor_move_timeout = 0.0f;
	float click_location_timeout = 0.0f;


	RE::TESObjectREFR* chosen_marker_refr = nullptr;


	bool can_fast_travel(int id)
	{
		bool result = false;

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::MapMenu>();

		if (menu)
		{
			if (id < std::size(menu->mapMarkers))
			{
				auto marker = menu->mapMarkers[id];

				RE::NiPointer<RE::TESObjectREFR> a_refrOut;

				if (RE::LookupReferenceByHandle(marker.ref, a_refrOut))
				{
					if (a_refrOut)
					{
						auto data = (RE::ExtraMapMarker*)a_refrOut->extraList.GetByType(RE::ExtraDataType::kMapMarker);
						if (data && data->mapData && data->mapData->flags)
							result = data->mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo);
					}
				}
			}
		}

		return result;
	}


	//id, marker_ref, vector of nearest quests

	









	std::vector<MenuOption> get_location_options()
	{
		std::map<int, std::pair<RE::TESObjectREFR*, std::vector<int>>> markers_to_remember{};

		//markers_to_remember.clear();

		std::vector<MenuOption> result{};
		std::map<int, MenuOption> pre_result{};
		std::map<int, bool> can_travel{};

		auto player = RE::PlayerCharacter::GetSingleton();
		auto player_pos = player->GetPosition();

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::MapMenu>();

		if (menu)
		{
			int id = 0;


			int closest_id = -1;
			float closest_distance = FLT_MAX;






			for (auto &marker : menu->mapMarkers)
			{
				if (marker.fullName)
				{
					std::string name = "";
					name = marker.fullName->fullName;

					if (name != "" && name != "Current Location")
					{
						MenuOption option{};

						RE::NiPointer<RE::TESObjectREFR> a_refrOut;

						if (RE::LookupReferenceByHandle(marker.ref, a_refrOut))
						{
							if (a_refrOut && a_refrOut.get())
							{
								auto data = (RE::ExtraMapMarker*)a_refrOut->extraList.GetByType(RE::ExtraDataType::kMapMarker);
								if (data && data->mapData && data->mapData->flags)
								{
									can_travel.insert({ id, data->mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo) });

									auto distance = player->GetDistance(a_refrOut.get());

									if (distance < closest_distance)
									{
										closest_id = id;
										closest_distance = distance;
									}
										
									//if (marker.)
									option.id = id;
									option.text = marker.fullName->fullName;

									pre_result.insert({ id, option });

									markers_to_remember.insert({ id, {a_refrOut.get(), {}} });

								}
							}
						}
					}
				}

				id++;
			}


			auto temp_quest_result = MiscThings::get_current_quests(); //refresh quests

			std::vector<MiscThings::quest>* p_quests = nullptr;

			if (MiscThings::is_quest_list_valid())
				p_quests = MiscThings::get_p_quest_list();


			if (p_quests)
			{
				int quest_actual_id = 0;

				for (auto a_quest : *p_quests)
				{
					RE::ObjectRefHandle quest_ref_handle{};
					if (a_quest.target)
					{
						a_quest.target->GetTrackingRef(quest_ref_handle, a_quest.quest); //try tracked
						if (!quest_ref_handle)
							a_quest.target->GetTargetRef(quest_ref_handle, false, a_quest.quest); //no tracked - try actual target


						if (quest_ref_handle && quest_ref_handle.get() && quest_ref_handle.get().get())
						{
							auto quest_ref = quest_ref_handle.get().get();

							if (quest_ref->data.objectReference)
							{
								float min_location_dist = FLT_MAX;
								int id_closest_to_quest = -1;
								for (auto marker : markers_to_remember)
								{
									auto marker_ref = marker.second.first;

									if (marker_ref && !marker_ref->IsDisabled() && marker_ref->data.objectReference)
									{
										//float distance = marker_ref->GetDistance(quest_ref, true, true);
										auto distance = MiscThings::get_quest_target_distance(a_quest.target, a_quest.quest, marker_ref);


										if (distance < min_location_dist)
										{
											id_closest_to_quest = marker.first;
											min_location_dist = distance;
										}
									}
								}

								if (id_closest_to_quest >= 0 && min_location_dist <= 15000.0f)
								{
									markers_to_remember.at(id_closest_to_quest).second.push_back(quest_actual_id);
								}
							}

						}
					}
	
					quest_actual_id++;

				}
			}






			for (auto option_raw : pre_result)
			{
				MenuOption option{};
				int local_id = option_raw.second.id;
				option.id = local_id;
				option.text = option_raw.second.text;
				 
				std::vector<int> closest_quests{};

				auto temp_p = markers_to_remember.find(option.id);

				if (temp_p != markers_to_remember.end())
				{
					closest_quests = temp_p->second.second;
				}


				if (can_travel.find(local_id) != can_travel.end())
				{
					if (local_id != closest_id || closest_distance > 10000.0f)
					{
						std::string can_travel_text = "";

						if (can_travel.find(local_id)->second)
							can_travel_text = ". Can fast travel. ";
						else
							can_travel_text = ". Cannot fast-travel. ";

						option.text += can_travel_text;
					}
					else
						option.text += ". You are here. ";
				}


				if (std::size(closest_quests) > 0)
				{
					std::string info = "";

					bool is_first = true;
					for (auto closest_quest_id : closest_quests)
					{


						if (closest_quest_id >= 0 && closest_quest_id < std::size(*p_quests))
						{

							auto quest_struct = p_quests->at(closest_quest_id);

							if (is_first)
							{
								info = "Closest to quest";
								if (std::size(closest_quests) > 1)
									info += "s";

								info += ": ";
							}
							else
								info += "; ";


							std::string quest_name = "[id " + std::to_string(quest_struct.id) + "] " + quest_struct.name + ": " + quest_struct.displaytext;

							info += quest_name;

							is_first = false;
						}
					}


					if (info != "")
						option.text += info;
				}



				result.push_back(option);
			}




			result.push_back({ -1, "[CLOSE MAP]" } );
		}


		return result;
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
		auto menu = ui->GetMenu<RE::MapMenu>();
		bool result = false;


		if (menu && !ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME))
		{
			result = true;
			set_universal_block(1.0f);
			RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		}

		return result;
	}

	void hardquit_menu()
	{
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::MapMenu>();




		if (menu)
		{
			if (ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME))
			{
				RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MessageBoxMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
			}

			RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
			RE::UIMessageQueue::GetSingleton()->AddMessage(RE::CursorMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		}




	}


	bool map_is_allowed()
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		if (MiscThings::is_intro() || MiscThings::is_intro2())
			return false;

		if (player->actorState1.flyState != RE::FLY_STATE::kNone)
			return false;

		if (player->actorState1.swimming)
			return false;

		if (player->GetParentCell() && player->GetParentCell()->IsInteriorCell())
			return false;



		return true;
	}


	std::string cant_open_map_reason()
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		std::string result = "";

		RE::UI* ui = RE::UI::GetSingleton();

		if (ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
			result = "Cannot open map while in crafting menu!";

		if (ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME))
			result = "Cannot open map while in dialogue!";

		if (ui->IsMenuOpen(RE::StatsMenu::MENU_NAME))
			result = "Cannot open map while in skill menu!";

		if (ui->IsMenuOpen(RE::LevelUpMenu::MENU_NAME))
			result = "Cannot open map while in levelup menu!";

		if (ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME))
			result = "Cannot open map while in training menu!";

		if (ui->IsMenuOpen(RE::SleepWaitMenu::MENU_NAME))
			result = "Cannot open map while in sleep/wait menu!";

		if (ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME))
			result = "Cannot open map right now, you have to make a choice first!";

		if (ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
			result = "Cannot open map while in container menu!";

		if (ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
			result = "Cannot open map while in barter menu!";

		if (ui->IsMenuOpen(RE::BookMenu::MENU_NAME))
			result = "Cannot open map while reading!";

		if (ui->IsMenuOpen(RE::GiftMenu::MENU_NAME))
			result = "Cannot open map while in gift menu!";

		if (ui->IsMenuOpen(RE::LockpickingMenu::MENU_NAME))
			result = "Cannot open map while lockpicking!";

		if (ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
			result = "The map is already opened!";

		if (ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME))
			result = "Cannot open map while game is loading!";

		if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
			result = "Cannot open map while in main menu! Wait for the game to start";

		if (ui->IsMenuOpen(RE::RaceSexMenu::MENU_NAME))
			result = "Cannot open map while creating your character!";

		if (MiscThings::is_intro() || MiscThings::is_intro2())
			result = "Cannot open map right now! It will become available later in the game";

		if (player->actorState1.flyState != RE::FLY_STATE::kNone)
			result = "Cannot open map while in the air! Wait a little and try again if you want...";

		if (player->actorState1.swimming)
			result = "Cannot open map while swimming!";

		if (player->GetParentCell() && player->GetParentCell()->IsInteriorCell())
			result = "Cannot open map in this location!";

		return result;
	}

	std::pair<bool, std::string> open_menu()
	{
		RE::UI* ui = RE::UI::GetSingleton();
		
		std::pair<bool, std::string> result{};

		std::string cant_open_reason = cant_open_map_reason();

		if (cant_open_reason != "")
		{
			result.first = false;
			result.second = cant_open_reason;
			return result;
		}

		if (MiscThings::is_intro() || MiscThings::is_intro2())
		{
			result.first = false;
			result.second = "Cannot open the map right now. ";
			return result;
		}



		auto menu = ui->GetMenu<RE::MapMenu>();

		if (!menu)
		{
			set_universal_block(0.5f);
			RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
			result.first = true;
			result.second = "[Opening map...]";
		}
		else
		{
			result.first = false;
			result.second = "Already in map menu";
		}

		return result;
	}





	std::pair<bool, std::string> set_location_choice(int id)
	{
		std::pair<bool, std::string> result{};

		auto ui = RE::UI::GetSingleton();
		if (!ui->IsMenuOpen(RE::MapMenu::MENU_NAME))
		{
			result.first = true;
			result.second = "[Error]";
			return result;
		}

		if (!ui->IsMenuOpen(RE::MapMenu::MENU_NAME))
		{
			result.first = true;
			result.second = "Not in map menu";
			return result;
		}

		auto menu = ui->GetMenu(RE::MapMenu::MENU_NAME);

		if (id == -1)
		{
			quit_menu();
			result.first = true;
			result.second = "[Closed map]";
			return result;
		}

		std::vector<MenuOption> options = get_location_options();

		bool valid = false;

		for (auto option : options)
		{
			if (option.id == id)
			{
				valid = true;
				break;
			}
		}

		if (valid)
		{
			location_choice = id;
			location_choice_valid = true;

			auto menu = ui->GetMenu<RE::MapMenu>();
			if (id < std::size(menu->mapMarkers))
			{
				auto marker = menu->mapMarkers[id];
				RE::NiPointer<RE::TESObjectREFR> a_refrOut;

				RE::LookupReferenceByHandle(marker.ref, a_refrOut);
					
				

				if (a_refrOut)
				{
					chosen_marker_refr = a_refrOut.get();
				}
				else
				{
					result.first = false;
					result.second = "This location is inaccessible";
					return result;
				}
					
			}
			else
			{
				result.first = false;
				result.second = "This location is inaccessible";
				return result;
			}

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




	float K = 100.0f;



	bool move_cursor_to_marker(int id)
	{
		bool result = false;

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::MapMenu>();

		auto menu_cursor = RE::MenuCursor::GetSingleton();


		if (menu_cursor && menu)
		{
			float cursor_posX = menu_cursor->cursorPosX / menu_cursor->screenWidthX;
			float cursor_posY = menu_cursor->cursorPosY / menu_cursor->screenWidthY;

			//mouse_cursor_move(menu_cursor->screenWidthX / 2 - menu_cursor->cursorPosX, -(menu_cursor->screenWidthY / 2 - menu_cursor->cursorPosY));
			//mouse_cursor_move(menu_cursor->screenWidthX - menu_cursor->cursorPosX, -(menu_cursor->screenWidthY - menu_cursor->cursorPosY));




			if ((id * 4 + 3)< std::size(menu->markerData))
			{
				auto marker_posX_raw = menu->markerData[id * 4 + 1];
				auto marker_posY_raw = menu->markerData[id * 4 + 2];

				if (!marker_posX_raw.IsNull() && marker_posX_raw.IsNumber() && !marker_posY_raw.IsNull() && marker_posY_raw.IsNumber())
				{
					float marker_posX = marker_posX_raw.GetNumber();
					float marker_posY = marker_posY_raw.GetNumber();

					bool camera_posX_ok = true;
					bool camera_posY_ok = true;


					if (marker_posY > 0.53)
						camera_posY_ok = false;

					if (marker_posY < 0.47)
						camera_posY_ok = false;

					if (marker_posX > 0.52)
						camera_posX_ok = false;

					if (marker_posX < 0.48)
						camera_posX_ok = false;


					if (!camera_posX_ok)
					{
						if (marker_posX > 0.52)
							right();

						if (marker_posX < 0.48)
							left();
					}
					else
					{
						if (!camera_posY_ok)
						{
							if (marker_posY > 0.53)
								cursor_down();

							if (marker_posY < 0.47)
								cursor_up();
						}
					}



					if (camera_posX_ok && camera_posY_ok)
					{
						if (click_location_timeout > 4.0f)
						{
							marker_posX *= 0.97;
							marker_posY *= 0.97;
						}
						else
							if (click_location_timeout > 3.0f)
							{
								marker_posX *= 0.97;
								marker_posY *= 1.03;
							}
							else
								if (click_location_timeout > 2.0f)
								{
									marker_posX *= 1.03;
									marker_posY *= 0.97;
								}
								else
									if (click_location_timeout > 1.0f)
									{
										marker_posX *= 1.03;
										marker_posY *= 1.03;
									}

						float dif_posX = marker_posX - cursor_posX;
						float dif_posY = marker_posY - cursor_posY;

						float mouse_moveX = dif_posX * K;
						float mouse_moveY = dif_posY * K;

						if (abs(mouse_moveX) < 1.0f)
							if (mouse_moveX < 0)
								mouse_moveX = -1.0f;
							else
								mouse_moveX = 1.0f;

						if (abs(mouse_moveY) < 1.0f)
							if (mouse_moveY < 0)
								mouse_moveY = -1.0f;
							else
								mouse_moveY = 1.0f;


						mouse_cursor_move(mouse_moveX, mouse_moveY);

						if (abs(dif_posX) < 0.006 && abs(dif_posY) < 0.006)
							result = true;
					}

					//if (camera_posX_ok && camera_posY_ok)
					//{
							
					//		result = true;
					//}
					
				}

			}
			


		}


		return result;
	}




	void reset_menu()
	{
		in_map = false;
		location_request_sent = false;
		location_choice_valid = false;
		location_choice = -1;
		rolled_over = false;
		catch_result_timer = 0.0f;
		start_moving = false;
		catch_result = false;

		undiscovered_travel_request_sent = false;
		undiscovered_travel_choice_valid = false;
		undiscovered_travel_choice = -1;

		cursor_move_timeout = 0.0f;
		click_location_timeout = 0.0f;

		if (get_active_force() == force_type::map_location || get_active_force() == force_type::map_undiscovered)
			set_active_force(-1);


		chosen_marker_refr = nullptr;

	}

	void reset_menu2()
	{
		wait_and_quit_and_walk_to_marker = false;
		wait_and_quit_and_walk_to_marker_time = 0.0f;
		idk_what_to_do = false;
	}



	std::pair<bool, std::string> set_undiscovered_choice(int choice)
	{
		std::pair<bool, std::string> result{};

		auto ui = RE::UI::GetSingleton();
		if (!ui->IsMenuOpen(RE::MapMenu::MENU_NAME))
		{
			result.first = true;
			result.second = "[Error]";
			return result;
		}


		if (choice == -1)
		{
			quit_menu();
			result.first = true;
			result.second = "[Closed map]";
			return result;
		}


		if (choice != 0 && choice != 1)
		{
			result.first = false;
			result.second = "Invalid choice ID";
		}
		else
		{
			undiscovered_travel_choice = choice;
			undiscovered_travel_choice_valid = true;
			result.first = true;
			result.second = "[Processing...]";
		}

		return result;
	}

	bool abandon_all_messages()
	{
		return idk_what_to_do;
	}


	bool in_local_map()
	{
		bool result = false;

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::MapMenu>();
		if (menu)
			if (menu->localMapMenu.showingMap)
				result = true;

		return result;
	}


	bool toggle = false;

	bool exit_local_map()
	{
		bool result = false;
		result = true;
		set_universal_block(2.0f);
		
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::MapMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.Bottom.LocalMapButton"))
					{
						result = var1.Invoke("onPress");
						result &= var1.Invoke("onRelease"); //here it wants release to work 
					}
		return result;
	}



	std::string get_undiscovered_message()
	{
		std::string result = "[You havent discovered this location yet. Would you like to place a marker and travel there by foot?]";

		return result;
	}



	std::vector<MenuOption> get_undiscovered_options()
	{
		std::vector<MenuOption> result{};
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::MessageBoxMenu>();

		if (menu)
		{
			result.push_back({ 0, "No"});
			result.push_back({ 1, "Yes" });
		}

		//result.push_back({ -1, "[CLOSE MAP]" });


		return result;
	}


	float map_processor_timer = 0.0f;

	void processor(float dtime)
	{
		

		if (wait_and_quit_and_walk_to_marker)
		{
			
			if (wait_and_quit_and_walk_to_marker_time > 1.0f)
			{
				hardquit_menu();
				//auto test_new_input = RE::BSInputDeviceManager::GetSingleton();
				//test_new_input->ResetInputDevices();
				//clear_input_queue();

				if (wait_and_quit_and_walk_to_marker_time > 2.0f)
				{
					idk_what_to_do = true;
					if (wait_and_quit_and_walk_to_marker_time > 3.0f)
					{
						wait_and_quit_and_walk_to_marker = false;
						wait_and_quit_and_walk_to_marker_time = 0.0f;
						WalkerProcessor::walk_to_player_marker(chosen_marker_refr);
						idk_what_to_do = false;
					}
					else
						wait_and_quit_and_walk_to_marker_time += dtime;
				}
				else	
					wait_and_quit_and_walk_to_marker_time += dtime;
			}
			else
				wait_and_quit_and_walk_to_marker_time += dtime;


			return;
		}


		if (catch_result)
		{
			if (catch_result_timer > 5.0f)
			{
				set_universal_block(1.0f);
				send_random_context("[Cannot travel there]", false);
				reset_menu();
			}
			//catch message why we cannot fast travel
			std::string catch_result = get_result_message();
			if (catch_result != "")
			{
				if (catch_result == "Autosaving...")
					send_random_context("[Successfully travelled]", false);
				else
					send_random_context("[" + catch_result + "]", false);

				set_universal_block(1.0f);
				reset_menu();
			}

			catch_result_timer += dtime;
		}
		else
			catch_result_timer = 0.0f;


		in_map = false;

		RE::UI* ui = RE::UI::GetSingleton();

		auto menu = ui->GetMenu<RE::MapMenu>();

		//auto cursor_menu = ui->GetMenu<RE::CursorMenu>();







		if (ui && menu && ui->IsMenuOpen(RE::MapMenu::MENU_NAME))
		{
			//RE::UIMessageQueue::GetSingleton()->AddMessage(RE::CursorMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); 
			if (in_map_timer > 2.5f) //big pause is necessary
			{
				in_map = true;
			}
			else
				in_map_timer += dtime;

		}
		else
			in_map_timer = 0.0f;



		if (in_map && start_moving)
		{
			if (move_cursor_to_marker(location_choice))
			{
				cursor_move_timeout = 0.0f;
				//cursor arrived.
				if (can_fast_travel(location_choice))
				{
					if (auto menu_confirm_quit = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm_quit)
					{
						RandomMessageBoxProcessor::set_messagebox_handeled();
						RE::GFxValue var1;

						if (menu_confirm_quit->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
							if (!var1.IsNull() && var1.IsString())
							{
								if (!rolled_over)
								{
									menu_confirm_quit->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
									rolled_over = true;
									set_universal_block(2.0f);
								}
								else
								{
									WalkerProcessor::set_just_teleported();
									WalkerProcessor::invalidate_path();

									rolled_over = false;
									menu_confirm_quit->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
									set_universal_block(1.5f);

									//supposedly we exit the menu now
								}
							}
					}
					else
					{
						
						

						//auto current_root = menu->camera.ro;

						if (!catch_result && chosen_marker_refr)
						{
							auto normal_camera = (RE::TESCamera*)(&menu->camera);
							auto old_camera_root = menu->camera.cameraRoot;
							auto camera_position = menu->camera.cameraRoot->world.translate;

							auto target_marker_pos = chosen_marker_refr->GetPosition();
							target_marker_pos.x += 200.0f;
							target_marker_pos.y += 400.0f;
							target_marker_pos.z = camera_position.z;

							menu->camera.cameraRoot->world.translate = target_marker_pos;
							//menu->camera.SetMapCameraRoot(old_camera_root.get(), target_marker_pos);
							//menu->camera.SetCameraRoot(old_camera_root);

						}

						catch_result = true;

						leftclick();
						//perk_up();//also zoom mouse

						click_location_timeout += dtime + 0.5f;

						if (click_location_timeout > 5.0f)
						{
							send_random_context("Cannot travel there right now", false);
							reset_menu();
						}

					}
				}
				else
				{
					//can put marker and travel manually...

					if (auto menu_confirm_quit = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm_quit)
					{
						catch_result = false;
						catch_result_timer = 0.0f;

						RandomMessageBoxProcessor::set_messagebox_handeled();


						if (!undiscovered_travel_request_sent)
						{
							
							if (force_choice(get_undiscovered_options(), get_undiscovered_message(), force_type::map_undiscovered))
								undiscovered_travel_request_sent = true;
						}
						else
						{
							if (undiscovered_travel_choice_valid)
							{

								if (undiscovered_travel_choice == 1)
								{
									RE::GFxValue var1;
									if (menu_confirm_quit->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
										if (!var1.IsNull() && var1.IsString())
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

												wait_and_quit_and_walk_to_marker = true;
												set_universal_block(1.5f);
											}
										}
								}
								else
								{
									RE::GFxValue var1;
									if (menu_confirm_quit->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
										if (!var1.IsNull() && var1.IsString())
										{
											if (!rolled_over)
											{
												menu_confirm_quit->uiMovie->Invoke("_root.MessageMenu.Buttons.Button1.onRollOver", nullptr, nullptr, 0);
												rolled_over = true;
												set_universal_block(2.0f);
											}
											else
											{
												rolled_over = false;
												menu_confirm_quit->uiMovie->Invoke("_root.MessageMenu.Buttons.Button1.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all

												reset_menu();
											}
										}
								}

								
							}
							
						}
						
					}
					else
					{
						/*
						if (!catch_result && chosen_marker_refr)
						{
							auto normal_camera = (RE::TESCamera*)(&menu->camera);
							auto old_camera_root = menu->camera.cameraRoot;
							auto camera_position = menu->camera.cameraRoot->world.translate;

							auto target_marker_pos = chosen_marker_refr->GetPosition();
							target_marker_pos.x += 200.0f;
							target_marker_pos.y += 400.0f;
							target_marker_pos.z = camera_position.z;

							menu->camera.cameraRoot->world.translate = target_marker_pos;

							//menu->camera.SetMapCameraRoot(old_camera_root.get(), target_marker_pos);
							//menu->camera.SetCameraRoot(old_camera_root);
						}
						*/


						catch_result = true;

						leftclick();
						//perk_up();//also zoom mouse

						click_location_timeout += dtime + 0.5f; //because leftclick() gives universal block 0.5

						if (click_location_timeout > 5.0f)
						{
							send_random_context("Cannot travel there right now", false);
							reset_menu();
						}
					}

					//send_random_context("[Cannot fast travel there]");
					//reset_menu();
					//set_universal_block(1.0f);
				}
			}
			else
			{
				cursor_move_timeout += dtime;

				if (cursor_move_timeout > 20.0f)
				{
					send_random_context("Cannot travel there right now", false);
					reset_menu();
				}
			}
		}




		if (map_processor_timer > 0.01)
		{
			map_processor_timer = 0.0f;

			RE::GFxValue var1;

	
			if (in_map)
			{
				//WalkerProcessor::reset_walker();

				if (in_local_map())
				{
					exit_local_map();
				}
				else
				{
					if (!location_request_sent)
					{
						
						if (force_choice(get_location_options(), "You opened the map. Choose location to travel to. ", force_type::map_location))
							location_request_sent = true;
					}
					else
					{
						if (location_choice_valid)
						{
							start_moving = true;
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
			map_processor_timer += dtime;


	}



}








/* //this failed
bool roll_over_marker(int id)
{
	//
	bool result = false;

	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::MapMenu>();
	std::string path1 = "_root.MarkerClips.Marker" + std::to_string(id) + ".onRollOver";
	std::string path2 = "_root.WorldMap.MarkerContainer.Marker" + std::to_string(id) + ".onRollOver";
	std::string path3 = "_root.WorldMap.MapMovie.MarkerClips.Marker" + std::to_string(id) + ".onRollOver";
	std::string path4 = "_root.WorldMap.Markers." + std::to_string(id) + ".onRollOver";

	if (ui)
		if (const auto menu = ui->GetMenu<RE::MapMenu>(); menu)
			if (menu->uiMovie)
			{
				result = menu->uiMovie->Invoke(path1.c_str(), nullptr, nullptr, 0);
				result = menu->uiMovie->Invoke(path2.c_str(), nullptr, nullptr, 0);
				result = menu->uiMovie->Invoke(path3.c_str(), nullptr, nullptr, 0);
				result = menu->uiMovie->Invoke(path4.c_str(), nullptr, nullptr, 0);
			}


	if (result)
		set_universal_block(1.0f);



	std::vector<std::vector<std::string>> names_all;

	names_all.clear();

	if (const auto ui = RE::UI::GetSingleton(); ui)
		//if (const auto menu = ui->GetMenu<RE::HUDMenu>(); menu)
		//if (const auto menu = ui->GetMenu<RE::LevelUpMenu>(); menu)
		if (const auto menu = ui->GetMenu<RE::MapMenu>(); menu)
			if (menu->uiMovie)
			{

				path1 = "_root.MarkerClips.Marker" + std::to_string(id);


				std::vector<std::string> names{};

				if (menu->uiMovie->GetVariable(&var1, path1.c_str()))
				{
					if (var1.IsObject())
					{
						var1.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
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
					auto test = var1.GetType();
				}

				names_all.push_back(names);
			}


	if (std::size(names_all) != 0)
	{
		auto test = var1.GetType();
	}

	return result;
}


bool press_marker(int id)
{
	bool result = false;

	RE::GFxValue var1;
	RE::UI* ui = RE::UI::GetSingleton();
	auto menu = ui->GetMenu<RE::MapMenu>();

	//_root.WorldMap.SelectedMarker.onPress - this doesnt exist until we hover.
	//_root.WorldMap.Markers[18].0.onPress - try this maybe it works

	//std::string path = "_root.MarkerClips.Marker" + std::to_string(id) + ".onPress"; - OLD DOESNT WORK
	std::string path = "_root.WorldMap.Markers." + std::to_string(id) + ".onPress";

	result = menu->uiMovie->Invoke(path.c_str(), nullptr, nullptr, 0);

	std::vector<std::string> variants = {
		"onRollOut",
		"onReleaseOutside",
		"onDragOut",
		"onPress",
		"onRelease",
		"onRollOver",
		"onDragOver"
	};




	if (ui)
		if (const auto menu = ui->GetMenu<RE::MapMenu>(); menu)
			if (menu->uiMovie)
			{
				for (auto variant : variants)
				{
					path = "_root.MarkerClips.Marker" + std::to_string(id) + "." + variant;
					result = menu->uiMovie->Invoke(path.c_str(), nullptr, nullptr, 0);
				}

			}


	if (result)
		set_universal_block(1.5f);

	return result;
}
*/




/*
if (!location_pressed)
{
	location_pressed = true;
	location_press_result = roll_over_marker(location_choice);
}
else
{
	if (!location_pressed_2)
	{
		location_pressed_2 = true;
		location_press_result = press_marker(location_choice);
	}
	else
	{
		if (auto menu_confirm_quit = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm_quit)
		{
			if (menu_confirm_quit->uiMovie->GetVariable(&var1, "_root.MessageMenu.MessageText.text"))
				if (!var1.IsNull() && var1.IsString())
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

						//supposedly we exit the menu now
					}
				}
		}
	}

}
*/