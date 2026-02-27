

#include "WalkerProcessor.hpp"
#include "Observer.hpp"
#include "Misc.hpp"
#include "main.hpp"
//TODO: at least put it in groups. maybe tell what can be done with each group



namespace Observer {








	float detect_threats_time = 0.0f;
	bool threats_response_request_sent = false;
	int threats_response_choice_valid = false;
	int threats_response_choice = -1;
	bool action_taken = false;
	bool pause_was_made = false;

	float dont_check_threats_timer = 0.0f;
	float detect_interesting_time = 0.0f;
	float detect_events_time = 0.0f;
	float state_monitor_timer = 0.0f;
	float state_monitor_subtimer = 0.0f;
	
	bool player_dead_sent = false;

	int last_health_value = 0;
	int last_stamina_value = 0;
	int last_mana_value = 0;

	bool old_unbound_quest = false;
	bool first_cycle = true;

	bool old_mount_state = false;
	RE::ActorPtr mount = nullptr;


	float detect_locations_timer = 0.0f;


	void set_threat_action_taken()
	{
		threats_response_request_sent = true;
		threats_response_choice_valid = true;
		action_taken = true;

		dont_check_threats_timer = 2.0f;

	}



	void reset_threats()
	{
		detect_threats_time = 0.0f;
		threats_response_request_sent = false;
		threats_response_choice_valid = false;
		threats_response_choice = -1;
		action_taken = false;
		pause_was_made = false;
	}


	void reset_observer()
	{
		dont_check_threats_timer = 0.0f;
		detect_interesting_time = 0.0f;
		detect_events_time = 0.0f;
		state_monitor_timer = 0.0f;
		state_monitor_subtimer = 0.0f;
		player_dead_sent = false;

		old_unbound_quest = false;
		first_cycle = true;

		detect_locations_timer = 0.0f;
	}



	std::vector<MenuOption> get_threat_options()
	{
		std::vector<MenuOption> threat_options;
		threat_options.push_back({ 0, "Fight back" });
		threat_options.push_back({ 1, "Run" });
		threat_options.push_back({ 2, "Ignore" });

		return threat_options;
	}


	std::pair<bool, std::string> set_threat_response_choice(int id)
	{
		std::pair<bool, std::string> result{};

		if (threats_response_request_sent && id >= 0 && id < 3)
		{
			threats_response_choice = id;
			threats_response_choice_valid = true;
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



	void detect_threats(float dtime)
	{
		if (dont_check_threats_timer > 0.0f)
			dont_check_threats_timer -= dtime;
		else
		{
			auto attackers = MiscThings::get_player_attackers();

			if (std::size(attackers) != 0 && !WalkerProcessor::is_fighting())
			{
				//wait a little then notify

				if (detect_threats_time > 0.5f)
				{
					if (!threats_response_request_sent)
					{
						

						std::string attacked_by = "";
						for (auto attacker : attackers)
						{
							attacked_by += MiscThings::insert_into_list_and_get_info(attacker);
							attacked_by += "; ";
						}



						if (force_choice(get_threat_options(), "There are enemies around you. Choose what to do. Enemies: " + attacked_by, force_type::threat_response))
						{
							if (!pause_was_made && !MiscThings::is_game_paused())
							{
								threats_response_request_sent = true;
								pause_was_made = true; //tween menu mouse kills walker so it has disabled mouse in main.cpp
								MiscThings::pause_game();
							}
						}




					}
					else
					{
						if (threats_response_choice_valid)
						{
							if (pause_was_made)
							{
								if (MiscThings::is_game_paused())
								{
									MiscThings::unpause_game();
								}
								//set_universal_block(0.5f);
								pause_was_made = false;
								return;

							}

							if (!action_taken)
							{
								if (threats_response_choice == 0)
								{
									if (DialogueProcessor::is_in_dialogue(nullptr))
										DialogueProcessor::quit_menu();
									WalkerProcessor::walk_to_object_by_refr(attackers.at(0), 3);
									action_taken = true;
								}

								if (threats_response_choice == 1)
								{
									send_random_context(WalkerProcessor::run_away().second);
									action_taken = true;
								}

								if (threats_response_choice == 2)
								{
									action_taken = true;
								}
							}
						}
					}
				}
				else
				{
					detect_threats_time += dtime;
				}
			}
			else
			{
				reset_threats();
			}
		}
		
	}



	struct interesting_object {
		std::string info;
		RE::TESObjectREFR* refr;
	};


	void detect_interesting_objects(float dtime)
	{
		if (detect_interesting_time > 5.0f)
		{
			detect_interesting_time = 0.0f;
			std::vector<interesting_object> result{};
			auto player = RE::PlayerCharacter::GetSingleton();
			auto player_ref = player->AsReference();
			auto player_actor = (RE::Actor*)player_ref;

			float scan_distance = 4000.0f;
			auto player_cell = player->GetParentCell();
			if (player_cell && player_cell->IsInteriorCell())
				scan_distance = 2000.0f;

			if (!player->IsDead())
			{
				RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, scan_distance,
					//player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
					[&](RE::TESObjectREFR* a_ref) {

						std::string name = a_ref->GetName();
						std::string player_name = RE::PlayerCharacter::GetSingleton()->GetName();

						auto base_obj = a_ref->GetBaseObject();
						RE::FormType base_type{};

						if (base_obj)
						{
							base_type = base_obj->GetFormType();
							bool debug_type = true;
						}
						else
						{
							bool no_base_object = true;
						}

						if (name[0] != '\0' && std::size(name) > 1 && name != player_name)
						{

							if (MiscThings::has_digits(name))
								return RE::BSContainer::ForEachResult::kContinue; 

							if (a_ref->AsReference()->modelState == 0)
								return RE::BSContainer::ForEachResult::kContinue; //skip objects without world model


							if (base_type == RE::FormType::Activator)
							{
								auto test = (RE::TESObjectACTI*)base_obj;
								std::string model = test->GetModel();
								if (model.find("Marker_LinkMarker") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
									return RE::BSContainer::ForEachResult::kContinue;

								//little flags
								if (model.find("MapFlag") != std::string::npos) //exclude markers. for some reason their model state is not 0 even though the model doesnt exist
									return RE::BSContainer::ForEachResult::kContinue;



							}



							if (name.find("not be visible") != std::string::npos) //"This should not be visible [Furniture]"
								return RE::BSContainer::ForEachResult::kContinue;

							if (name.find("Do Not Delete") != std::string::npos)
								return RE::BSContainer::ForEachResult::kContinue;

							if (name.find("nvisible") != std::string::npos && name.find("arker") != std::string::npos)
								return RE::BSContainer::ForEachResult::kContinue;




							if (a_ref->AsReference()->IsActor())
							{
								if (!MiscThings::is_object_in_the_list(a_ref))
									result.push_back({ MiscThings::insert_into_list_and_get_info(a_ref), a_ref });
							}


							if (base_type == RE::FormType::Door)
							{
								if (!MiscThings::is_object_in_the_list(a_ref))
									result.push_back({ MiscThings::insert_into_list_and_get_info(a_ref), a_ref });
							}



							if (base_type == RE::FormType::Activator)
							{
								if (!MiscThings::is_object_in_the_list(a_ref))
									result.push_back({ MiscThings::insert_into_list_and_get_info(a_ref), a_ref });
							}

							if (base_type == RE::FormType::Furniture) //pullchains/levers
							{
								auto furniture = (RE::TESFurniture*)base_obj;
								auto workbenchtype = furniture->workBenchData.benchType;
								if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
								{
									if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
									{
										if (!MiscThings::is_object_in_the_list(a_ref))
											result.push_back({ MiscThings::insert_into_list_and_get_info(a_ref), a_ref });
									}
								}
							}




							if (a_ref->GetDistance(player_ref) < 1000.0f)
							{
								//and now with smaller range




								if (base_type == RE::FormType::Container)
								{
									if (!MiscThings::is_object_in_the_list(a_ref))
										result.push_back({ MiscThings::insert_into_list_and_get_info(a_ref), a_ref });
								}


								if (base_obj)
								{
									bool is_harvestable = false;

									if (base_type == RE::FormType::Tree)
									{
										auto tree_form = (RE::TESObjectTREE*)base_obj;

										auto test_flags = a_ref->AsReference()->GetFormFlags();

										bool already_harvested = false;

										if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
											already_harvested = true;

										if (test_flags & 2048) //this is potentially only one we need here
											already_harvested = true;




										if (tree_form->produceItem && !already_harvested)
											is_harvestable = true;
									}

									if (base_type == RE::FormType::Flora)
									{
										auto tree_form = (RE::TESFlora*)base_obj;

										if (tree_form->produceItem)
											is_harvestable = true;
									}


									if (is_harvestable)
									{
										if (!MiscThings::is_object_in_the_list(a_ref))
											result.push_back({ MiscThings::insert_into_list_and_get_info(a_ref), a_ref });
									}
								}

								if (base_type == RE::FormType::Furniture) //workbenches and beds
								{
									bool this_isnt_a_furniture = false;
									auto furniture = (RE::TESFurniture*)base_obj;
									auto workbenchtype = furniture->workBenchData.benchType;
									if (workbenchtype != RE::TESFurniture::WorkBenchData::BenchType::kNone)
									{
										if (!MiscThings::is_object_in_the_list(a_ref))
											result.push_back({ MiscThings::insert_into_list_and_get_info(a_ref), a_ref });
									}
									else
									{
										if (furniture->furnFlags.any(RE::TESFurniture::ActiveMarker::kCanSleep))
											if (!MiscThings::is_object_in_the_list(a_ref))
												result.push_back({ MiscThings::insert_into_list_and_get_info(a_ref), a_ref }); //bed
									}
								}

								if (a_ref->GetDistance(player_ref) < 500.0f)
								{
									if (base_obj->IsInventoryObject())
									{
										if (!MiscThings::is_object_in_the_list(a_ref))
											result.push_back({ MiscThings::insert_into_list_and_get_info(a_ref), a_ref });
									}
								}


							}

						}
						else
						{
							//nameless things that still have to be tracked.
							//FXspiderWebKitDoorSpecial - normal
							//FXspiderWebKitDoorSpecialDest - destroyed

							if (base_obj && (base_obj->formFlags & RE::TESForm::RecordFlags::kDestructible))
							{
								base_type = base_obj->GetFormType();

								if (base_type == RE::FormType::Activator)
								{
									auto static_obj = (RE::TESObjectACTI*)base_obj;

									std::string model = static_obj->GetModel();

									if (model.find("FXspiderWebKitDoorSpecial") != std::string::npos)
									{
										if (a_ref->GetDistance(player_ref) < 300.0f)
										{
											if (!MiscThings::is_object_in_the_list(a_ref))
												result.push_back({ MiscThings::insert_into_list_custom_name("[Destructible] Cobweb", a_ref), a_ref });
										}
									}

								}

								/*
								if (base_type == RE::FormType::Furniture)
								{
									auto static_obj = (RE::TESFurniture*)base_obj;

									std::string model = static_obj->GetModel();

									if (model.find("FXspiderWebKitDoorSpecial") != std::string::npos)
									{
										if (a_ref->GetDistance(player_ref) < 500.0f)
										{
											if (!MiscThings::is_object_in_the_list(a_ref))
												result.push_back({ MiscThings::insert_into_list_custom_name("[Destructible] Cobweb", a_ref), a_ref });
										}
									}
								}
								*/
								auto test_base = 0xec3de;

								auto web_refr = RE::TESObjectREFR::LookupByID(0x45f3c);
								//auto web_refr2 = RE::TESObjectREFR::LookupByID(0xec3de);

								if (web_refr)
								{
									
									RE::TESObjectREFR* refr1 = (RE::TESObjectREFR*)web_refr;
									/*
									auto object_p = MiscThings::General::Script::GetObject(refr1, "PressurePlate");

									if (object_p)
									{

										RE::BSFixedString prop_name = "::isOpen_var";

										//if (General::Script::GetVariable<bool>(object_p, prop_name))
										//	result = 0;
										//else
										//	result = 1;

									}
									*/


									auto base_obj1 = refr1->GetBaseObject();
									auto base_type1 = base_obj1->GetFormType();

									//auto activator = (RE::TESObjectACTI*)base_obj1;
									//auto model = activator->GetModel();


									bool here1 = false;
								}

								
							}

						}

						return RE::BSContainer::ForEachResult::kContinue;
					});


				auto player_pos = player_ref->GetPosition();


				std::sort(result.begin(), result.end(), [&](interesting_object left, interesting_object right) {
					//return left->GetDistance(player) > right->GetDistance(player); //switch > to < for inversed order. this is last->closest
					RE::NiPoint3 pos_left = left.refr->GetPosition();
					RE::NiPoint3 pos_right = right.refr->GetPosition();

					return pos_left.GetDistance(player_pos) < pos_right.GetDistance(player_pos); //alphabetical order. top = A

					});

				std::string info_string = "[You see: \n";

				for (auto result_entry : result)
				{
					info_string += result_entry.info + "\n";
				}

				if (std::size(result) != 0)
					send_random_context(info_string);
			}

		}
		else
			detect_interesting_time += dtime;

	}



	struct old_object_state {
		bool dead{};
		RE::RefHandle target;
		int action_flags;
		int pillar_face_code;
		int trap_firing;
		int destructible_state;
	};


	std::map<RE::TESObjectREFR*, old_object_state> objects_to_track{};


	long long last_periodic_info = 0;


	void clear_objects_to_track()
	{
		objects_to_track.clear();
	}


	void detect_events(float dtime)
	{

		if (detect_events_time > 0.1f)
		{
			detect_events_time = 0.0f;
			std::vector<std::string> result{};
			auto player = RE::PlayerCharacter::GetSingleton();
			auto player_ref = player->AsReference();
			auto player_actor = (RE::Actor*)player_ref;

			float scan_distance = 4000.0f;
			auto player_cell = player->GetParentCell();
			if (player_cell && player_cell->IsInteriorCell())
				scan_distance = 2000.0f;

			RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, scan_distance,
				//player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
				[&](RE::TESObjectREFR* a_ref) {

					if (a_ref->IsActor())
					{
						std::string name1 = a_ref->GetDisplayFullName();
						if (name1 == "Headsman")
						{
							bool headsman_found = true;
						}


						auto actor_ref = (RE::Actor*)a_ref;

						if (objects_to_track.find(a_ref) == objects_to_track.end())
						{
							auto new_target = 0;
							if (actor_ref->currentProcess)
								new_target = actor_ref->currentProcess->target;

							old_object_state state = { a_ref->IsDead(), new_target, (int)actor_ref->actorState1.flyState, 0, -1, -1};
							objects_to_track.insert({ a_ref, state });
						}
						else
						{
							auto old_entry = objects_to_track.find(a_ref);
							old_object_state old_state = old_entry->second;

							auto new_target = 0;
							if (actor_ref->currentProcess)
								new_target = actor_ref->currentProcess->target;

							old_object_state new_state = { a_ref->IsDead(), new_target, old_state.action_flags, old_state.pillar_face_code, old_state.trap_firing , old_state.destructible_state};

							if (old_state.dead != new_state.dead)
							{
								bool dont_add = false;

								objects_to_track.insert_or_assign(a_ref, new_state);

								std::string victim_name = MiscThings::insert_into_list_and_get_info(a_ref);
								std::string message_text = "[" + victim_name + " died]";

								auto target_actor = (RE::Actor*)a_ref;
								auto killer = target_actor->myKiller;
								if (killer)
								{
									if (a_ref == player_ref) //have dedicated message for that
										dont_add = true;

									auto killer_ptr = killer.get();
									if (killer_ptr)
									{
										auto killer_actor = killer_ptr.get();


										if (MiscThings::is_intro2() && killer_actor == a_ref && actor_ref->race->fullName == "Dragon Race")
											return RE::BSContainer::ForEachResult::kContinue; //alduin kills himself during helgen attack for some reason. skip it

										if (killer_actor)
										{
											std::string killer_name = MiscThings::insert_into_list_and_get_info(killer_actor);
											if (killer_actor == player_actor)
												killer_name = "You";

											message_text = "[" + killer_name + " killed " + victim_name + "]";

										}
									}
								}
								else
								{
									if (a_ref == player_ref) //have dedicated message for that
										dont_add = true;
								}

								if (!dont_add)
									result.push_back(message_text);
							}


							if (old_state.target != new_state.target)
							{
								if (old_state.target != 0 && new_state.target == 0)
								{
									//interacted with something?
									objects_to_track.insert_or_assign(a_ref, new_state);

									auto p_target = RE::TESObjectREFR::LookupByHandle(old_state.target);

									if (p_target)
									{
										auto target_refr = p_target.get();
										if (target_refr && target_refr != player_ref)
										{
											std::string target_short_name = target_refr->GetDisplayFullName();
											std::string actor_short_name = a_ref->GetDisplayFullName();
											if (target_short_name != "" && actor_short_name != "")
											{
												std::string interaction_name = " interacted with ";

												if (target_refr == a_ref)
													return RE::BSContainer::ForEachResult::kContinue; //alduin kills himself during helgen attack for some reason. skip it

												if (actor_ref->race->fullName == "Dragon Race" && target_refr->IsActor())
														interaction_name = " killed ";

												auto target_name = MiscThings::insert_into_list_and_get_info(target_refr);
												std::string actor_name = MiscThings::insert_into_list_and_get_info(a_ref);

												std::string message = "[" + actor_name + interaction_name + target_name;

												result.push_back(message);
											}
										}
									}

								}
								else
								{
									objects_to_track.insert_or_assign(a_ref, new_state);
								}
							}
						}
					}
					else
					{
						auto base_object = a_ref->GetBaseObject();
						auto base_type = base_object->GetFormType();

						if (true || base_type == RE::FormType::Activator || base_type == RE::FormType::Door)
						{
							if (objects_to_track.find(a_ref) == objects_to_track.end())
							{
								old_object_state state = { 0, 0, 0, 0, -1 };
								RE::ExtraDataList* extralist = &a_ref->extraList;
								auto extra = extralist->GetByType(RE::ExtraDataType::kAction);
								int pillar_face = MiscThings::get_pillar_face_name(a_ref);
								//int action_data = 0;
								int trap_firing = MiscThings::trap_firing(a_ref);
								int activation = MiscThings::two_state_activator_state(a_ref);
								int destructible_state = MiscThings::get_destructible_state(a_ref);

								//if (extra)
								//{
								//	auto extra_action = (RE::ExtraAction*)extra;
								//	action_data = static_cast<int>(*extra_action->action);
								//}

								state = { 0, 0, activation, pillar_face, trap_firing, destructible_state };

								objects_to_track.insert({ a_ref, state });
							}
							else
							{
								RE::ExtraDataList* extralist = &a_ref->extraList;
								//auto extra = extralist->GetByType(RE::ExtraDataType::kAction);
								auto old_entry = objects_to_track.find(a_ref);
								old_object_state old_state = old_entry->second;
								int pillar_face = MiscThings::get_pillar_face_name(a_ref);
								int trap_firing = MiscThings::trap_firing(a_ref);
								int activation = MiscThings::two_state_activator_state(a_ref);
								int destructible_state = MiscThings::get_destructible_state(a_ref);

								old_object_state new_state = { 0, 0, activation, pillar_face, trap_firing, destructible_state };


								if (base_type == RE::FormType::Door)// && a_ref->GetDisplayFullName() == "")
								{
									if (old_state.action_flags != new_state.action_flags)
									{
										auto door = (RE::TESObjectDOOR*)base_object;
										std::string model = door->GetModel();

										if (model.find("CaveGSecretDoor") != std::string::npos)
										{
											std::string name = MiscThings::insert_into_list_custom_name("Secret stone wall door", a_ref);

											if (activation == 0)
												result.push_back("[ " + name + " opened]");

											if (activation == 1)
												result.push_back("[ " + name + " closed]");

											if (activation == 2)
												result.push_back("[ " + name + " is opening...]");

											if (activation == 3)
												result.push_back("[ " + name + " is closing...]");
										}
									}
								}
								else
								{
									if (true)//(extra)
									{
										//auto extra_action = (RE::ExtraAction*)extra;
										//int action_data = static_cast<int>(*extra_action->action);

										//new_state = { 0, 0, action_data, pillar_face, trap_firing };


										if (old_state.action_flags != new_state.action_flags)
										{
											if (true)//(action_data & (int)RE::OBJECT_ACTION::kOpen)
											{
												auto extra_anim = extralist->GetByType(RE::ExtraDataType::kAnimGraphManager);
												if (extra_anim)
												{
													auto extra_anim_graph = (RE::ExtraAnimGraphManager*)extra_anim;
													if (extra_anim_graph->animGraphMgr)
													{
														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "NorRetractableBridge01")
														{
															std::string name = MiscThings::insert_into_list_custom_name("Large wooden bridge", a_ref);

															if (activation == 0)
																result.push_back("[ " + name + " opened]");

															if (activation == 1)
																result.push_back("[ " + name + " closed]");
														}

														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "ImpPortcullisSmall01")
														{
															std::string name = MiscThings::insert_into_list_custom_name("Heavy wooden gate", a_ref);

															if (activation == 0)
																result.push_back("[ " + name + " opened]");

															if (activation == 1)
																result.push_back("[ " + name + " closed]");
														}

														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PortcullisLarge01")
														{
															std::string name = MiscThings::insert_into_list_custom_name("Metal gate", a_ref);

															if (activation == 0)
																result.push_back("[ " + name + " opened]");

															if (activation == 1)
																result.push_back("[ " + name + " closed]");
														}





														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "PuzzleDoorKeyHole01")
														{
															std::string name = "Ancient Nordic Door";

															//if (activation == 0)
															//	result.push_back("[" + name + " 0]");

															if (activation == 1)
																result.push_back("[" + name + " is opening...]");

															//if (activation == 2)
															//	result.push_back("[" + name + " 2]");


															//if (activation == 3)
															//	result.push_back("[" + name + " 3]");

															if (activation == 4)
																result.push_back("[" + name + " didn't move]");
														}
														//

													}
												}
											}
										}
									}

									if (old_state.pillar_face_code != new_state.pillar_face_code)
									{
										if (new_state.pillar_face_code > 0 && new_state.pillar_face_code < 4)
										{
											std::string pillar_name = MiscThings::get_stateless_info(a_ref);
											std::string pillar_face_name = MiscThings::get_pillar_face_name(a_ref, new_state.pillar_face_code);

											std::string solved_text = "";

											if (pillar_face_name != "")
												solved_text = MiscThings::get_pillar_solved_text(a_ref);

											result.push_back(pillar_name + " turned to " + pillar_face_name + solved_text);
										}
									}

									if (old_state.trap_firing == 6)
									{
										if (player->GetDistance(a_ref) < 400.0f)
										{
											long long now = std::chrono::steady_clock::now().time_since_epoch().count();
											float dtime = (double)(now - last_periodic_info) / 1000000000.0;
											if (dtime > 2.0f)
											{
												last_periodic_info = now;
												std::string name = MiscThings::insert_into_list_custom_name("Trap swinging blade", a_ref);
												result.push_back("[ " + name + " swinged!]");
											}
										}
									}


									if (old_state.trap_firing != new_state.trap_firing)
									{
										if (new_state.trap_firing == 1)
										{
											if (std::size(result) == 0 || result.at(std::size(result) - 1) != "[Dart trap triggered!]")
												result.push_back("[Dart trap triggered!]");
										}

										if (new_state.trap_firing == 2)
										{
											std::string name = MiscThings::insert_into_list_custom_name("Pressure plate", a_ref);
											result.push_back("[ " + name + " was triggered!]");
										}


										if (new_state.trap_firing == 3)
										{
											std::string name = MiscThings::insert_into_list_custom_name("Trap swinging wall", a_ref);
											result.push_back("[ " + name + " launched!]");
										}


										if (new_state.trap_firing == 4)
										{
											std::string name = MiscThings::insert_into_list_custom_name("Trap swinging wall", a_ref);
											result.push_back("[ " + name + " went back]");
										}

										if (new_state.trap_firing == 5)
										{
											std::string name = MiscThings::insert_into_list_custom_name("Trap swinging blade", a_ref);
											result.push_back("[ " + name + " deactivated]");
										}

										if (new_state.trap_firing == 6)
										{
											std::string name = MiscThings::insert_into_list_custom_name("Trap swinging blade", a_ref);
											result.push_back("[ " + name + " started swinging!]");
										}

										if (new_state.trap_firing == 7)
										{
											std::string name = MiscThings::insert_into_list_custom_name("Trap oil lamp", a_ref);
											result.push_back("[ " + name + " fell down and exploded!]");
										}

										if (new_state.trap_firing == 8)
										{
											std::string name = MiscThings::insert_into_list_custom_name("Oil on the floor", a_ref);
											result.push_back("[ " + name + " started burning]");
										}


										//only when close.



									}

									if (old_state.destructible_state != new_state.destructible_state)
									{
										if (new_state.destructible_state == 1)
										{
											std::string name = MiscThings::insert_into_list_custom_name("[Destructible] Cobweb", a_ref);

											result.push_back(name + " was destroyed");
										}
									}
								}

								









								objects_to_track.insert_or_assign(a_ref, new_state);
							}
						}





					}

					return RE::BSContainer::ForEachResult::kContinue;
				});



				RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 20000.0f, //999999.0 is too much
					//player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
					[&](RE::TESObjectREFR* a_ref) {

						if (a_ref->IsActor())
						{
							auto actor_ref = (RE::Actor*)a_ref;
							if (actor_ref->race->fullName == "Dragon Race")
							{

								if (objects_to_track.find(a_ref) == objects_to_track.end())
								{
									int new_target = 0;
									if (actor_ref->currentProcess)
										new_target = actor_ref->currentProcess->target;

									old_object_state state = { a_ref->IsDead(), new_target, 0 };
									objects_to_track.insert({ a_ref, state });
								}
								else
								{
									auto old_entry = objects_to_track.find(a_ref);
									old_object_state old_state = old_entry->second;


									old_object_state new_state = { old_state.dead , old_state.target, (int)actor_ref->actorState1.flyState };

									objects_to_track.insert_or_assign(a_ref, new_state);

									if (old_state.action_flags != new_state.action_flags)
									{
										std::string in_the_distance = "";

										if (player_ref->GetDistance(a_ref) > 15000.0f)
											in_the_distance = " in the distance";

										if (new_state.action_flags & (int)RE::FLY_STATE::kCruising)
										{
											std::string name = MiscThings::insert_into_list_and_get_info(a_ref);
											std::string action = " is flying";
											if (result.empty())
											result.push_back("[" + name + action + in_the_distance + "]");
										}

										if (new_state.action_flags & (int)RE::FLY_STATE::kLanding)
										{
											std::string name = MiscThings::insert_into_list_and_get_info(a_ref);
											std::string action = " is landing";
											if (result.empty())
											result.push_back("[" + name + action + in_the_distance + "]");
										}

										if (new_state.action_flags & (int)RE::FLY_STATE::kHovering)
										{
											std::string name = MiscThings::insert_into_list_and_get_info(a_ref);
											if (result.empty())
											std::string action = " is hovering";
											
										}

										if (new_state.action_flags & (int)RE::FLY_STATE::kTakeOff)
										{
											std::string name = MiscThings::insert_into_list_and_get_info(a_ref);
											std::string action = " takes off";
											if (result.empty())
											result.push_back("[" + name + action + in_the_distance + "]");
										}

										if (new_state.action_flags & (int)RE::FLY_STATE::kPerching)
										{
											std::string name = MiscThings::insert_into_list_and_get_info(a_ref);
											std::string action = " is perching";
											//result.push_back("[" + name + action + "]"); //looks like perching randomly comes with other types making no sense
										}

									}
								}

							}
						}

						return RE::BSContainer::ForEachResult::kContinue;
					});



			if (std::size(result) > 0)
			{
				std::string message = "";
				for (auto result_entry : result)
				{
					message += result_entry + "\n";
				}

				send_random_context(message);

			}


		}
		else
			detect_events_time += dtime;

	}




	

	void detect_locations(float dtime)
	{
		if (detect_locations_timer > 30.0f)
		{
			auto player = RE::PlayerCharacter::GetSingleton();

			RE::BSTArray<RE::ObjectRefHandle> map_markers = player->currentMapMarkers;

			std::vector<std::string> location_list{};


			for (auto marker : map_markers)
			{
				if (marker.get())
				{
					auto real_marker = marker.get().get();
					auto data = (RE::ExtraMapMarker*)real_marker->extraList.GetByType(RE::ExtraDataType::kMapMarker);
					auto data_radius = (RE::ExtraRadius*)real_marker->extraList.GetByType(RE::ExtraDataType::kRadius);

					std::string marker_name = data->mapData->locationName.GetFullName();

					//if (marker_name.find("eirmund") != std::string::npos)
					//	bool stop_here = false;

					float radius = 2000.0f;

					//if (!data_radius)
					//{
					//	bool test_breakpoint = false;
					//}
					//else
					//{
					//	;// radius = data_radius->radius;// *10.0f; //this radius is useless, markers just appear on 20k distance
					//}
						
					if (data && data->mapData && marker_name.find("Military Camp") == std::string::npos) //just filter military camps for now
					{
						if (marker_name != "")
						{

							auto distance = real_marker->GetDistance(player);

							
							if (distance < radius * 10.0f) //looks like devs put exactly this value for compass
							{
								if (!MiscThings::is_location_in_the_list(real_marker))
								{
									std::string location_info = MiscThings::insert_location_into_list_and_get_info(real_marker);
									if (location_info != "")
										location_list.push_back(location_info);
								}
							}
						}
					}
				}
			}


			if (std::size(location_list) > 0)
			{
				std::string message = "[New locations on compass:\n";

				for (auto location : location_list)
				{
					message += location + "\n";
				}
				message = message.substr(0, message.length() - 1);
				message += "]";

				send_random_context(message);
			}
			////
		}
		else
			detect_locations_timer += dtime;
		
	}







	void player_state_monitor(float dtime)
	{
		RE::UI* ui = RE::UI::GetSingleton();
		if (!ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) && !ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
		{
			if (state_monitor_timer > 0.5f)
			{
				if (first_cycle)
				{
					first_cycle = false;
					old_unbound_quest = false;
					auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
					if (threshold_quest)
						if (threshold_quest->GetCurrentStageID() < 160)
						{
							old_unbound_quest = true;
							//game start context
							send_random_context("You awake on a carriage going down a mountain to Helgen. You were unfortunate enough to stumble across an imperial raid on rebels while crossing the border and were arrested with other Stormcloaks, a horse thief, and Ulfric Stormcloak himself. Everyone assumes correctly that they are being led to their execution.");
						}
							
				}
				else
				{
					auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
					if (threshold_quest)
						if (threshold_quest->GetCurrentStageID() >= 160 && old_unbound_quest)
						{
							old_unbound_quest = false;
							send_random_context("[YOU CAN WALK NOW]");
						}
				}



				state_monitor_timer = 0.0f;
				auto player = RE::PlayerCharacter::GetSingleton();
				auto player_ref = player->AsReference();
				auto player_actor = (RE::Actor*)player_ref;

				int health = player->GetActorValue(RE::ActorValue::kHealth);
				int stamina = player->GetActorValue(RE::ActorValue::kStamina);
				int mana = player->GetActorValue(RE::ActorValue::kMagicka);

				int max_health = player->GetActorValueMax(RE::ActorValue::kHealth);
				int max_stamina = player->GetActorValueMax(RE::ActorValue::kStamina);
				int max_mana = player->GetActorValueMax(RE::ActorValue::kMagicka);

				bool player_dead = player->IsDead();



				bool cur_mount = MiscThings::is_on_horse();

				if (old_mount_state != cur_mount)
				{
					old_mount_state = cur_mount;

					if (cur_mount)
					{
						RE::ActorPtr mount_ptr;
						player_actor->GetMount(mount_ptr);
						std::string mount_name = "Horse";
						mount = mount_ptr;
						if (mount_ptr)
						{
							RE::TESObjectREFR* mount_refr = (RE::TESObjectREFR*)mount_ptr.get();
							mount_name = MiscThings::insert_into_list_and_get_info(mount_refr);
						}

						send_random_context("[You got onto " + mount_name + "]");
					}
					else
					{
						std::string mount_name = "Horse";
						if (mount)
						{
							RE::TESObjectREFR* mount_refr = (RE::TESObjectREFR*)mount.get();
							mount_name = MiscThings::insert_into_list_and_get_info(mount_refr);
						}
						mount = nullptr;

						send_random_context("[You got off " + mount_name + "]");
					}
						
				}



				if (player_dead)
				{
					if (!player_dead_sent)
					{
						player_dead_sent = true;
						send_random_context("[YOU DIED]");
						MiscThings::set_time_of_death(std::chrono::steady_clock::now().time_since_epoch().count());
					}

				}
				else
				{
					int health_dif = (last_health_value - health);
					int stamina_dif = (last_stamina_value - stamina);
					int mana_dif = (last_mana_value - mana);

					bool send_info = false;

					if (abs(health_dif) > 10.0f)
					{
						last_health_value = health;
						send_info = true;
					}

					if (abs(stamina_dif) > 10.0f)
					{
						last_stamina_value = stamina;
						//send_info = true;
					}

					if (abs(mana_dif) > 10.0f)
					{
						last_mana_value = mana;
						send_info = true;
					}


					if (send_info)
					{
						std::string health_text = std::to_string(health) + "/" + std::to_string(max_health);
						std::string stamina_text = std::to_string(stamina) + "/" + std::to_string(max_stamina);
						std::string mana_text = std::to_string(mana) + "/" + std::to_string(max_mana);


						std::string message = "[Your state: Health " + health_text + ", Stamina: " + stamina_text + ", Magicka: " + mana_text + "]";

						send_random_context(message);

					}



				}



			}
			else
				state_monitor_timer += dtime;
		}
	}



}