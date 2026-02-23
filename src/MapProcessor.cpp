

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


	std::vector<MenuOption> get_location_options()
	{
		std::vector<MenuOption> result{};

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::MapMenu>();

		if (menu)
		{
			int id = 0;

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
							if (a_refrOut)
							{
								auto data = (RE::ExtraMapMarker*)a_refrOut->extraList.GetByType(RE::ExtraDataType::kMapMarker);
								if (data && data->mapData && data->mapData->flags)
								{
									bool can_travel = data->mapData->flags.any(RE::MapMarkerData::Flag::kCanTravelTo);

									std::string can_travel_text = "";

									if (can_travel)
										can_travel_text = ". Can fast travel there. ";
									else
										can_travel_text = ". Cannot fast-travel there. ";


									//if (marker.)
									option.id = id;
									option.text = marker.fullName->fullName;
									option.text += can_travel_text;

									result.push_back(option);
								}
							}
						}
					}
				}

				id++;
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




	std::pair<bool, std::string> open_menu()
	{
		std::pair<bool, std::string> result{};

		RE::UI* ui = RE::UI::GetSingleton();
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




	float K = 10.0f;







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

					if (marker_posX > 0.52)
					{
						camera_posX_ok = false;
						right();
					}

					if (marker_posX < 0.48)
					{
						camera_posX_ok = false;
						left();
					}


					if (marker_posY > 0.53)
					{
						camera_posY_ok = false;
						cursor_down();
					}

					if (marker_posY < 0.47)
					{
						camera_posY_ok = false;
						cursor_up();
					}



					if (camera_posX_ok && camera_posY_ok)
					{
		
							result = true;
					}
					
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

		result.push_back({ -1, "[CLOSE MAP]" });


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
						WalkerProcessor::walk_to_player_marker();
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
				send_random_context("[Cannot travel there]");
				reset_menu();
			}
			//catch message why we cannot fast travel
			std::string catch_result = get_result_message();
			if (catch_result != "")
			{
				if (catch_result == "Autosaving...")
					send_random_context("[Successfully travelled]");
				else
					send_random_context("[" + catch_result + "]");

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
									rolled_over = false;
									menu_confirm_quit->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
									set_universal_block(1.5f);

									//supposedly we exit the menu now
								}
							}
					}
					else
					{
						catch_result = true;
						leftclick();
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
							undiscovered_travel_request_sent = true;
							force_choice(get_undiscovered_options(), get_undiscovered_message(), force_type::map_undiscovered);
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
						//catch_result = true;
						leftclick();
					}

					//send_random_context("[Cannot fast travel there]");
					//reset_menu();
					//set_universal_block(1.0f);
				}
			}
		}




		if (map_processor_timer > 0.01)
		{
			map_processor_timer = 0.0f;

			RE::GFxValue var1;

	
			if (in_map)
			{
				WalkerProcessor::reset_walker();

				if (in_local_map())
				{
					exit_local_map();
				}
				else
				{
					if (!location_request_sent)
					{
						location_request_sent = true;
						force_choice(get_location_options(), "You opened the map. Choose location to travel to. ", force_type::map_location);
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