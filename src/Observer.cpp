

#include "InputActions.hpp"
#include "WalkerProcessor.hpp"
#include "Observer.hpp"
#include "Misc.hpp"
#include "main.hpp"
//TODO: at least put it in groups. maybe tell what can be done with each group



namespace Observer {


	bool surroundings_scanned = false;

	float no_threats_timer = 0.0f;

	float detect_threats_time = 0.0f;
	bool threats_response_request_sent = false;
	int threats_response_choice_valid = false;
	int threats_response_choice = -1;
	bool action_taken = false;
	bool pause_was_made = false;

	float dont_check_threats_timer = 0.0f;
	float detect_interesting_time = 0.0f;
	float detect_interesting_spit_results_time = 0.0f;
	float detect_events_time = 0.0f;
	float state_monitor_timer = 0.0f;
	float state_monitor_subtimer = 0.0f;
	
	bool player_dead_sent = false;

	int last_health_value = 0;
	int last_stamina_value = 0;
	int last_mana_value = 0;

	int old_unbound_quest_stage = 0;
	bool first_cycle = true;
	bool first_cycle2 = true;


	bool old_mount_state = false;
	RE::ActorPtr mount = nullptr;

	bool old_can_interact = false;
	bool old_can_look = false;
	bool old_can_walk = false;
	bool old_can_fight = false;


	bool old_peace_quest_state = false;



	float detect_locations_timer = 0.0f;

	float last_saved_time = 0.0f;

	float start_game_timer = 0.0f;
	bool wait_and_send_game_start_context = false;

	bool observers_green_light = false;
	float green_light_delay = 0.0f;


	bool jail_serving_notified = false;
	bool jail_escaping_notified = false;

	bool player_monitor_finished = false;

	float inventory_monitor_timer = 0.0f;


	bool old_objects_around_valid = false;

	RE::TESObjectREFR* old_occupied_furniture = nullptr;


	bool not_first_inventory_info = false; //do not reset 

	std::string last_weather = "";

	std::string old_time_text = "";


	int runaway_in_a_row = 0;


	bool player_can_be_arrested = false;
	RE::TESObjectREFR* closest_guard = nullptr;


	bool tried_to_heal = false;
	float tried_to_heal_time = 0.0f;


	std::map<RE::TESObjectREFR*, std::string> interesting_buffer{};


	int active_puzzle = -1;
	bool puzzle_pause_was_made = false;
	bool puzzle_request_was_sent = false;
	bool puzzle_choice_valid = false;
	int puzzle_choice = -1;
	RE::TESObjectREFR* puzzle_target = nullptr;


	float object_cleanup_timer = 0.0f;


	int old_greybeard_call_stage = 0;
	bool greybeards_called = false;
	float greybeards_called_timer = 0.0f;

	bool old_had_any_quests = false;




	float jail_reminder_time = 0.0f;


	bool dont_inform_inventory = false;


	RE::SIT_SLEEP_STATE old_sit_state{};


	std::string old_furniture_name = "";



	void reset_quest_puzzles()
	{
		active_puzzle = -1;
		puzzle_pause_was_made = false;
		puzzle_request_was_sent = false;
		puzzle_choice_valid = false;
		puzzle_choice = -1;
		puzzle_target = nullptr;
	}


	std::pair<bool, std::string> set_quest_puzzle_choice(int id)
	{
		std::pair<bool, std::string> result{};

		if (!puzzle_request_was_sent)
		{
			reset_quest_puzzles();
			result.first = true;
			result.second = "[Error]";
		}
		else
		{



			int min_range = 1;
			int max_range = 3;






			switch (active_puzzle)
			{
			case 1:
			case 2:
			{
				min_range = 1;
				max_range = 5;
				break;
			}

			case 3:
			{

				min_range = 1;
				max_range = 4;
				break;
			}

			case 5:
			{

				min_range = 1;
				max_range = 4;

				auto player = RE::PlayerCharacter::GetSingleton();
				if (player)
				{
					if (player->GetPosition().x < 132600.0f)
						max_range = 3;
				}

				break;
			}

			case 4:
			{
				min_range = 1;
				max_range = 6;

				break;
			}

			default:
			{
				reset_quest_puzzles();
				result.first = true;
				result.second = "[Error]";
				break;
			}
			}


			if (id >= min_range && id <= max_range)
			{
				puzzle_choice = id;
				puzzle_choice_valid = true;
				result.first = true;
				result.second = "[Processing...]";
			}
			else
			{
				result.first = false;
				result.second = "Invalid choice ID";
			}



			

		}


		return result;
	}


	void set_quest_puzzle_type(int type)
	{
		active_puzzle = type;
	}


	float not_processing_ustengrev_time = 0.0f;
	float pause_puzzle_scan_time = 0.0f;

	void timed_quest_puzzles_processor(float dtime)
	{

		if (pause_puzzle_scan_time <= 0.0f)
		{
			auto ustengrev_finish_trigger = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x52477);


			if (active_puzzle == -1 && MiscThings::player_inside_of_karthspire_plate_puzzle())
			{
				//no time delay, its a sharp turn without threats + it will be threat if we step on the plates
				WalkerProcessor::reset_walker();
				RE::TESObjectREFR* chain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x511a4);

				WalkerProcessor::look_at_object_by_refr(chain, true, 1.0f);
				active_puzzle = 5;
			}

			if (active_puzzle == -1 && ustengrev_finish_trigger && !ustengrev_finish_trigger->IsDisabled() && MiscThings::player_inside_of_ustengrev_gate_puzzle())
			{
				if (!WalkerProcessor::processing_ustengrev())
					not_processing_ustengrev_time += dtime;
				else
					not_processing_ustengrev_time = 0.0f;

				if (not_processing_ustengrev_time > 3.0f)
					active_puzzle = 3;
			}
			else
				not_processing_ustengrev_time = 0.0f;
		}
		else
		{
			if (!WalkerProcessor::is_walking_important_path())
				pause_puzzle_scan_time -= dtime;
		}
			





		if (active_puzzle != -1)
		{
			switch (active_puzzle)
			{
			case 1:
			{
				//greybeards ghostshouting
				
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					options.push_back({ 1, "Stare at the ghost" });
					options.push_back({ 2, "Attack the ghost with currently equipped weapons" });
					options.push_back({ 3, "Use Unrelenting Force shout at the ghost" });
					options.push_back({ 4, "Do nothing" });
					options.push_back({ 5, "Do some spins" });

					if (force_choice(options, "The greybeards want you to demonstrate that you are able to use the power of the voice. They summoned a ghost as a dummy target for you. What will you do?", force_type::timed_quest_puzzle))
					{
						if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
						{
							puzzle_request_was_sent = true;
							puzzle_pause_was_made = true;
							MiscThings::pause_game();
						}
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{
						register_allowed_actions();

						if (puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						switch (puzzle_choice)
						{
						case 1:
						{
							WalkerProcessor::walk_to_object_by_refr(puzzle_target, 0);
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							WalkerProcessor::walk_to_object_by_refr(puzzle_target, 3);
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							auto shout_form = (RE::TESShout*)RE::TESForm::LookupByID(0x13e07);
							WalkerProcessor::shout_at_target(puzzle_target, shout_form);
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							reset_quest_puzzles();
							break;
						}
						case 5:
						{
							int amount_of_spins = (float)std::rand() / RAND_MAX * 6 + 1;
							int speed = (float)std::rand() / RAND_MAX * 11 - 5.5f;
							if (speed == 0)
								speed = 5;

							WalkerProcessor::make_spins(amount_of_spins, speed);
							reset_quest_puzzles();
							break;
						}

						default:
						{
							reset_quest_puzzles();
							break;
						}

						}
					}
				}
				

				break;
			}

			case 2:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					options.push_back({ 1, "Stare at the gate" });
					options.push_back({ 2, "Use Whirlwind Sprint shout to fly through the gate" });
					options.push_back({ 3, "Do nothing" });
					options.push_back({ 4, "Jump" });
					options.push_back({ 5, "Attack the gate" });

					if (force_choice(options, "The greybeards want you to demonstrate that you are able to use the power of the voice once again. In this trial you have to use a shout to quickly fly through a gate, before it closes. What will you do?", force_type::timed_quest_puzzle))
					{
						if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
						{
							puzzle_request_was_sent = true;
							puzzle_pause_was_made = true;
							MiscThings::pause_game();
						}
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{
						if (puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						switch (puzzle_choice)
						{
						case 1:
						{
							RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3FAFB);
							WalkerProcessor::walk_to_object_by_refr(gate, 0);
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3FAFB);
							auto shout_form = (RE::TESShout*)RE::TESForm::LookupByID(0x2f7ba);
							WalkerProcessor::shout_at_target(gate, shout_form, true);
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							jump();
							reset_quest_puzzles();
							break;
						}
						case 5:
						{
							RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3FAFB);
							WalkerProcessor::walk_to_object_by_refr(gate, 3);
							reset_quest_puzzles();
							break;
						}

						default:
						{
							reset_quest_puzzles();
							break;
						}

						}
					}
				}


				break;
			}



			case 3:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					//options.push_back({ 1, "Run around a keystone" });
					options.push_back({ 1, "Use Whirlwind Sprint shout to fly near the keystones first, and then run through gates" });
					options.push_back({ 2, "Jump off the nearest cliff" });
					options.push_back({ 3, "Run through the keystones to open the gates first, then quickly use Whirlwind Sprint shout to fly through the gates" });
					options.push_back({ 4, "Run through the keystones and then continue running through the gates without using a shout" });


					if (force_choice(options, "The path is blocked by some kind of gate puzzle. There are 3 keystones and 3 gates. When you run past a keystone, it opens 1 gate. But in short period of time the gate closes again. Looks you will need to use the Whirlwind shout because gates close too fast. What will you do?", force_type::timed_quest_puzzle))
					{
						if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
						{
							puzzle_request_was_sent = true;
							puzzle_pause_was_made = true;
							MiscThings::pause_game();
						}
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{
						if (puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						pause_puzzle_scan_time = 5.0f;

						switch (puzzle_choice)
						{
						//case 1:
						//{
						//	reset_quest_puzzles();
						//	break;
						//}
						case 1:
						{
							WalkerProcessor::ustengrev_puzzle_get_ready(2);
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							WalkerProcessor::ustengrev_off_the_cliff();
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							WalkerProcessor::ustengrev_puzzle_get_ready(0);
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							WalkerProcessor::ustengrev_puzzle_get_ready(1);
							reset_quest_puzzles();
							break;
						}

						default:
						{
							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}


			case 4:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					//options.push_back({ 1, "Run around a keystone" });
					options.push_back({ 1, "Close the door" });
					options.push_back({ 2, "Do some spins" });
					options.push_back({ 3, "Do nothing" });
					options.push_back({ 4, "Run away" });
					options.push_back({ 5, "Pickpocket Delphine" });
					options.push_back({ 6, "Attack Delphine" });

					if (force_choice(options, "Looks like Delphine wants you to close the door before you can continue talking... What will you do?", force_type::timed_quest_puzzle))
					{
						puzzle_request_was_sent = true;

						//if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
						//{
						//	puzzle_request_was_sent = true;
						//	puzzle_pause_was_made = true;
						//	MiscThings::pause_game();
						//}
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{
						if (puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						pause_puzzle_scan_time = 5.0f;

						switch (puzzle_choice)
						{
							//case 1:
							//{
							//	reset_quest_puzzles();
							//	break;
							//}
						case 1:
						{
							auto door_refr = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xe3a79);
							WalkerProcessor::walk_to_object_by_refr(door_refr, 1);
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							int amount_of_spins = (float)std::rand() / RAND_MAX * 6 + 1;
							int speed = (float)std::rand() / RAND_MAX * 11 - 5.5f;
							if (speed == 0)
								speed = 5;

							WalkerProcessor::make_spins(amount_of_spins, speed);
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							WalkerProcessor::walk_to_object_by_index(-1, 0);
							reset_quest_puzzles();
							break;
						}

						case 5:
						{
							auto delphine = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x13485);
							WalkerProcessor::walk_to_object_by_refr(delphine, 2);
							reset_quest_puzzles();
							break;
						}

						case 6:
						{
							auto delphine = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x13485);
							WalkerProcessor::walk_to_object_by_refr(delphine, 3);
							reset_quest_puzzles();
							break;
						}

						default:
						{
							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}



			case 5:
			{
				if (!puzzle_request_was_sent)
				{
					std::vector<MenuOption> options{};
					//options.push_back({ 1, "Run around a keystone" });
					options.push_back({ 1, "Run towards the exit, ignoring the puzzle" });
					options.push_back({ 2, "Simply walk to the chain and pull it" });
					options.push_back({ 3, "Use Whirlwind Sprint shout to fly to the chain, then pull it" });

					auto player = RE::PlayerCharacter::GetSingleton();
					if (player)
					{
						if (player->GetPosition().x > 132600.0f)
							options.push_back({ 4, "Walk carefully to the chain, only stepping on the pressure plates with the Dragonborn symbol on them" });
					}
					

					if (force_choice(options, "You stop and see another puzzle. Suspicious pressure plates cover entire floor of this room, they have different symbols on them. There is a pulling chain across the room, and exit cave on the right. What will you do?", force_type::timed_quest_puzzle))
					{
						puzzle_request_was_sent = true;

						//if (!puzzle_pause_was_made && !MiscThings::is_game_paused())
						//{
						//	puzzle_request_was_sent = true;
						//	puzzle_pause_was_made = true;
						//	MiscThings::pause_game();
						//}
					}
				}
				else
				{
					if (puzzle_choice_valid)
					{
						if (false && puzzle_pause_was_made)
						{
							if (MiscThings::is_game_paused())
							{
								MiscThings::unpause_game();
							}
							//set_universal_block(0.5f);
							puzzle_pause_was_made = false;
							return;

						}

						pause_puzzle_scan_time = 5.0f;

						switch (puzzle_choice)
						{
							//case 1:
							//{
							//	reset_quest_puzzles();
							//	break;
							//}
						case 1:
						{
							auto our_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ203");
							WalkerProcessor::walk_to_quest_by_index(WalkerProcessor::get_quest_id_by_refr(our_quest), false);
							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						case 2:
						{
							RE::TESObjectREFR* chain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x511a4);
							WalkerProcessor::walk_to_object_by_refr(chain, 1);
							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						case 3:
						{
							RE::TESObjectREFR* chain = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x511a4);
							auto shout_form = (RE::TESShout*)RE::TESForm::LookupByID(0x2f7ba);
							WalkerProcessor::shout_at_target(chain, shout_form, false, chain);
							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}
						case 4:
						{
							WalkerProcessor::walk_karthspire_plates();
							pause_puzzle_scan_time = 10.0f;
							reset_quest_puzzles();
							break;
						}

						default:
						{
							reset_quest_puzzles();
							break;
						}
						}
					}
				}


				break;
			}



			default:
				reset_quest_puzzles();
			}
		}
		else
		{
			reset_quest_puzzles();
		}
	}




	void set_threat_action_taken()
	{
		threats_response_request_sent = true;
		threats_response_choice_valid = true;
		action_taken = true;

		dont_check_threats_timer = 2.0f;

	}


	bool can_surrender_to_guards()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto escaping_jail = player->playerFlags.escaping;
		return closest_guard && player_can_be_arrested && !escaping_jail;
	}


	void reset_threats()
	{
		if (closest_guard && player_can_be_arrested)
			unregister_surrender_to_guards();

		detect_threats_time = 0.0f;
		threats_response_request_sent = false;
		threats_response_choice_valid = false;
		threats_response_choice = -1;
		action_taken = false;
		pause_was_made = false;
		closest_guard = nullptr;

		no_threats_timer = 0.0f;
	}





	std::map<RE::TESObjectREFR*, long long> player_hit_info{};
	bool hitmap_lock = false;

	void attatch_hitmap()
	{
		EventSink::GetSingleton()->attatch_hitmap(&player_hit_info, &hitmap_lock, *send_random_context);
	}




	void reset_observer()
	{
		if (!hitmap_lock)
			player_hit_info.clear();

		dont_check_threats_timer = 0.0f;
		detect_interesting_time = 0.0f;
		detect_events_time = 0.0f;
		state_monitor_timer = 0.0f;
		state_monitor_subtimer = 0.0f;
		player_dead_sent = false;

		old_unbound_quest_stage = false;
		first_cycle = true;
		first_cycle2 = true;
		last_saved_time = 0.0f;

		detect_locations_timer = 0.0f;

		old_peace_quest_state = false;


		old_can_interact = false;
		old_unbound_quest_stage = 0;
		old_can_look = false;
		old_can_fight = false;

		jail_serving_notified = false;
		jail_escaping_notified = false;

		runaway_in_a_row = 0;

		observers_green_light = false;
		surroundings_scanned = false;

		old_occupied_furniture = nullptr;

		player_monitor_finished = false;


		inventory_monitor_timer = 0.0f;

		last_health_value = 0.0f;
		last_stamina_value = 0.0f;
		last_mana_value = 0.0f;

		last_weather = "";
		old_time_text = "";


		tried_to_heal = false;
		tried_to_heal_time = 0.0f;

		detect_interesting_spit_results_time = 0.0f;
		interesting_buffer.clear();

		object_cleanup_timer = 0.0f;

		dont_inform_inventory = false;
	}

	

	bool not_informing_inventory()
	{
		return dont_inform_inventory;
	}

	void set_dont_inform_inventory()
	{
		dont_inform_inventory = true;
	}

	void reset_dont_inform_inventory()
	{
		dont_inform_inventory = false;
	}

	std::vector<MenuOption> get_threat_options()
	{
		std::vector<MenuOption> threat_options;
		threat_options.push_back({ 1, "Fight back" });
		threat_options.push_back({ 2, "Run" });
		threat_options.push_back({ 3, "Ignore" });
		if (player_can_be_arrested && closest_guard)
			threat_options.push_back({ 4, "Surrender to guards" });

		return threat_options;
	}


	std::pair<bool, std::string> set_threat_response_choice(int id)
	{
		std::pair<bool, std::string> result{};

		if (!threats_response_request_sent)
		{
			result.first = true;
			result.second = "[Error]";
		}
		else
		{
			if ((id >= 1 && id <= 3) || (id == 4 && player_can_be_arrested))
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
		}


		return result;
	}



	void detect_threats(float dtime)
	{
		auto player = RE::PlayerCharacter::GetSingleton();


		if (threats_response_request_sent && threats_response_choice_valid)
		{
			//had one instance of it not unpausing the game for no visible reason.. unpause it here
			if (MiscThings::is_game_paused() && !MiscThings::have_force_only_menu_open() && !MiscThings::player_has_levelup())
			{
				MiscThings::unpause_game();

				set_universal_block(0.5f);
				return;
			}

		}



		if (observers_green_light)
		{
			if (dont_check_threats_timer > 0.0f)
				dont_check_threats_timer -= dtime;
			else
			{
				auto attackers = MiscThings::get_player_attackers(true);

				if (std::size(attackers) != 0)
				{
					no_threats_timer = 0.0f;
					if (!WalkerProcessor::is_fighting() && !MiscThings::have_force_only_menu_open())
					{
						//wait a little then notify

						if (detect_threats_time > 0.5f)
						{


							if (!threats_response_request_sent)
							{
								player_can_be_arrested = false;


								std::string attacked_by = "";
								for (auto attacker : attackers)
								{
									attacked_by += MiscThings::insert_object_into_list_and_get_info(attacker);
									attacked_by += "; ";
									auto crime_faction = attacker->GetCrimeFaction();
									if (crime_faction)
									{
										auto crime_values = crime_faction->crimeData.crimevalues;
										bool stop_here = false;
									}

									auto tried_to_yield = player->playerFlags.attemptedYieldInCurrentCombat;
									auto escaping_jail = player->playerFlags.escaping;

									if (!tried_to_yield && !escaping_jail && attacker->IsGuard())
									{
										//didnt try to yield yet and attacker is guard. can try to yield
										player_can_be_arrested = true;
										if (!closest_guard)
											closest_guard = attacker;
									}

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
										if (threats_response_choice == 1)
										{
											runaway_in_a_row = 0;
											if (DialogueProcessor::is_in_dialogue(nullptr))
												DialogueProcessor::quit_menu();
											WalkerProcessor::walk_to_object_by_refr(attackers.at(0), 3);
											action_taken = true;


											//if (player_can_be_arrested && closest_guard)
											//	register_surrender_to_guards();

										}

										if (threats_response_choice == 2)
										{
											std::string message = WalkerProcessor::run_away().second;

											runaway_in_a_row++;
											if (runaway_in_a_row >= 3)
												message += "[You will not have much progress in the game if you keep running away from fights every time]";

											send_random_context(message);
											action_taken = true;
										}

										if (threats_response_choice == 3)
										{
											runaway_in_a_row = 0;
											action_taken = true;
										}


										if (threats_response_choice == 4)
										{
											runaway_in_a_row = 0;
											action_taken = true;
											send_random_context("[You try to surrender to guards...]");
											WalkerProcessor::surrender_to_guards();

										}

									}
									else
									{
										if (threats_response_choice != 2 && threats_response_choice != 3 && threats_response_choice != 4 && !WalkerProcessor::walker_active())
										{
											//walker inactive, but we have threats. reset threats
											reset_threats();
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
				}
				else
				{
					if (no_threats_timer > 10.0f)
					{
						if (!WalkerProcessor::is_running_away())
							reset_threats();
					}
					else
						no_threats_timer += dtime;
					
				}
			}
		}

	}



	//struct interesting_object {
	//	std::string info;
	//	RE::TESObjectREFR* refr;
	//};

	

	bool are_surroundings_scanned()
	{
		return surroundings_scanned;
	}


	

	void cleanup_invalid_objects(float dtime)
	{
		if (object_cleanup_timer > 0.5f)
		{
			object_cleanup_timer = 0.0f;
			if (MiscThings::is_objects_around_valid())
			{
				auto object_list = MiscThings::get_p_objects_around();


				auto player_ref = RE::PlayerCharacter::GetSingleton()->AsReference();


				std::map<RE::TESObjectREFR*, int> current_objects{};

				RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 30000.0,
					[&](RE::TESObjectREFR* a_ref) {
						
						current_objects.insert({ a_ref, 0 });

						return RE::BSContainer::ForEachResult::kContinue;
					});


				for (auto object : *object_list)
				{
					if (object.second.object && current_objects.find(object.second.object) != current_objects.end())
					{
						if (!MiscThings::is_object_valid(object.second.object))
						{
							MiscThings::nullify_object_by_id(object.first);
						}
					}
					else
					{
						//it doesnt exist anymore within 30k range
						MiscThings::nullify_object_by_id(object.first);
					}
				}
			}
		}
		else
			object_cleanup_timer += dtime;

	}



	void detect_interesting_objects(float dtime, bool ignore_raycast)
	{
		if (observers_green_light && !MiscThings::have_force_only_menu_open())
		{
			if (ignore_raycast || detect_interesting_time > 0.5f || (first_cycle2 && detect_interesting_time > 2.0f))
			{
				if (first_cycle2)
					first_cycle2 = false;

				surroundings_scanned = true;

				detect_interesting_time = 0.0f;
				
				auto player = RE::PlayerCharacter::GetSingleton();
				auto player_ref = player->AsReference();
				auto player_actor = (RE::Actor*)player_ref;

				float scan_distance = 4000.0f;
				auto player_cell = player->GetParentCell();
				if (player_cell && player_cell->IsInteriorCell())
					scan_distance = 3000.0f;

				if (!player->IsDead())
				{
					RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 4000.0f,
						//player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
						[&](RE::TESObjectREFR* a_ref) {

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

							if (base_type == RE::FormType::Activator)
							{
								if (!MiscThings::is_object_in_the_list(a_ref))
								{
									auto word_of_power = MiscThings::get_word_of_power(a_ref);

									if (word_of_power && word_of_power != (RE::TESObjectREFR*)(-1))
									{
										std::string info = MiscThings::insert_object_into_list_custom_name("Word of Power, calling for you", a_ref);
										if (info != "")
											interesting_buffer.insert_or_assign(a_ref, info);
									}
								}

							}
							
							auto distance = player_ref->GetDistance(a_ref);




							if (distance <= scan_distance)
							{
								RE::BSString result_string = "";
								RE::TESNPC* player_npc = RE::TESForm::LookupByID(0x7)->As<RE::TESNPC>();
								player_npc->GetActivateText(a_ref, result_string);
								std::string result_string_actual_string = result_string.c_str();
								if (result_string_actual_string.find("Carriage") != std::string::npos)
								{
									bool stop_here = false;
								}




								bool jail_distance_met = distance < 500.0f;
								bool serving_jail = MiscThings::is_serving_jail();

								bool jail_condition_raycastable = serving_jail && jail_distance_met && MiscThings::raycastable(a_ref, 500.0f, false);
								bool jail_condition_all = serving_jail && jail_distance_met;
								



								std::string name = a_ref->GetName();
								std::string player_name = RE::PlayerCharacter::GetSingleton()->GetName();


								if (name.find("Crumbling") != std::string::npos)
								{
									bool stop_here = false;
								}

								if (!MiscThings::is_object_valid(a_ref))
									return RE::BSContainer::ForEachResult::kContinue;



								if (name[0] != '\0' && std::size(name) > 1 && name != player_name && name != "Sit")
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

									if (name.find("default") != std::string::npos)
										return RE::BSContainer::ForEachResult::kContinue;



									if (auto extra = a_ref->extraList.GetByType(RE::ExtraDataType::kItemDropper); extra)
									{
										auto extra_dropper = (RE::ExtraItemDropper*)extra;

										if (extra_dropper && extra_dropper->dropper && extra_dropper->dropper.get())
										{
											auto dropper = extra_dropper->dropper.get().get();

											if (dropper)
											{
												if (dropper->IsActor())
												{
													return RE::BSContainer::ForEachResult::kContinue; //exclude dropped items, they have weird position.
												}
											}
										}
									}


									if (a_ref->AsReference()->IsActor())
									{
										if (!MiscThings::is_object_in_the_list(a_ref) && (a_ref->GetDistance(player_ref) < 150.0f || MiscThings::raycastable(a_ref, scan_distance)))
										{
											std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
											if (info != "")
											{
												if (MiscThings::is_carriage_driver(a_ref))
													send_random_context("You see: " + info, false);
												else
													interesting_buffer.insert_or_assign(a_ref, info);
											}
												
										}

									}


									if (base_type == RE::FormType::Door)
									{
										if (!MiscThings::is_object_in_the_list(a_ref))
										{
											auto door = (RE::TESObjectDOOR*)base_obj;
											std::string model = door->GetModel();
											if (model.find("LoadMarker") != std::string::npos)
											{
												if (distance < 1000.0f)
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
														interesting_buffer.insert_or_assign(a_ref, info);
												}
											}
											else
											{
												if (jail_condition_all || ignore_raycast || MiscThings::raycastable(a_ref, scan_distance))
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
														interesting_buffer.insert_or_assign(a_ref, info);
												}
											}
										}
										
									}



									if (base_type == RE::FormType::Activator)
									{
										if (!MiscThings::is_object_in_the_list(a_ref))
										{
											float scan_distance_norm = scan_distance;
											if (a_ref == RE::TESObjectREFR::LookupByID(0xC3B29))
												scan_distance = 130.0f;

											bool local_ignore_raycast = false;

											if (name.find("Nirnroot") != std::string::npos || name.find("Ashpile") != std::string::npos)
												local_ignore_raycast = true;

											if (jail_condition_all || local_ignore_raycast || ignore_raycast || MiscThings::raycastable(a_ref, scan_distance, false))
											{
												std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);

												
												if (info != "" && MiscThings::is_object_valid(a_ref))
												{
													if (name.find("Fishing Supplies") != std::string::npos && !WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path())
													{
														WalkerProcessor::look_at_object_by_refr(a_ref, true, 1.0f);
														send_random_context("You see: " + info, false); 
													}
													else
														interesting_buffer.insert_or_assign(a_ref, info);

												}
													
											}

											scan_distance = scan_distance_norm;
										}
									}

									if (base_type == RE::FormType::Furniture) //pullchains/levers
									{
										auto furniture = (RE::TESFurniture*)base_obj;
										auto workbenchtype = furniture->workBenchData.benchType;
										if (workbenchtype == RE::TESFurniture::WorkBenchData::BenchType::kNone)
										{
											if (furniture->HasKeywordString("ActivatorLever") || furniture->HasKeywordString("isPullChain"))
											{
												if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_all || ignore_raycast || MiscThings::raycastable(a_ref, scan_distance)))
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
														interesting_buffer.insert_or_assign(a_ref, info);
												}
											}
										}
									}




									if (a_ref->GetDistance(player_ref) < 1000.0f)
									{
										//and now with smaller range

										if (base_type == RE::FormType::Container)
										{
											if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_raycastable || ignore_raycast || MiscThings::raycastable(a_ref, 1000.0f)))
											{
												std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
												if (info != "")
												{
													if (info.find("Large Treasure") != std::string::npos)
													{
														RE::TESObjectREFR* klimmek_chest = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9c614);

														if (klimmek_chest == a_ref)
														{
															auto klimmek_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("FreeformIvarstead04");
															if (klimmek_quest)
															{
																auto klimmek_stage = klimmek_quest->GetCurrentStageID();

																if (klimmek_stage != 20 || !MiscThings::is_container_empty(a_ref))
																	return RE::BSContainer::ForEachResult::kContinue;
															}
														}



														if (!WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path() && !MiscThings::is_container_empty(a_ref))
														{
															WalkerProcessor::look_at_object_by_refr(a_ref, true, 1.0f);
															send_random_context("You see: " + info, false); //large chests are not silent and immidiate 
														}
														else
															interesting_buffer.insert_or_assign(a_ref, info);
													}
													else
														interesting_buffer.insert_or_assign(a_ref, info);
												}

											}
										}

										if (base_obj && !MiscThings::is_serving_jail())
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

												auto test_flags = a_ref->AsReference()->GetFormFlags();
												bool already_harvested = false;
												if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
													already_harvested = true;

												if (tree_form->produceItem && !already_harvested)
													is_harvestable = true;
											}


											if (is_harvestable)
											{
												if (!MiscThings::is_object_in_the_list(a_ref))
												{
													std::string temp_name = a_ref->GetDisplayFullName();

													if (temp_name.find("Coin Purse") != std::string::npos)
													{
														if (ignore_raycast || MiscThings::raycastable(a_ref, 3000.0f, false))
														{
															std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
															if (info != "")
															{
																if (MiscThings::get_player_gold() < 100)
																{
																	if (!WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path())
																		WalkerProcessor::reset_walker();

																	send_random_context("You see: " + info, false); //coin purses are not silent when we are broke so they dont end up with 0 gold for too long
																}
																else
																{
																	interesting_buffer.insert_or_assign(a_ref, info);

																}
															}
														}
													}
													else
													{
														std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
														if (info != "")
														{
															interesting_buffer.insert_or_assign(a_ref, info);
														}
													}
														


												}
											}
										}

										if (base_type == RE::FormType::Furniture) //workbenches and beds
										{
											bool this_isnt_a_furniture = false;
											auto furniture = (RE::TESFurniture*)base_obj;
											auto workbenchtype = furniture->workBenchData.benchType;
											if (workbenchtype != RE::TESFurniture::WorkBenchData::BenchType::kNone)
											{
												if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_raycastable || ignore_raycast || MiscThings::raycastable(a_ref, 1000.0f)))
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
														interesting_buffer.insert_or_assign(a_ref, info);
												}
											}
											else
											{
												if (furniture->furnFlags.any(RE::TESFurniture::ActiveMarker::kCanSleep))
													if (!MiscThings::is_object_in_the_list(a_ref) && (jail_condition_raycastable || ignore_raycast || MiscThings::raycastable(a_ref, 1000.0f)))
													{
														std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
														if (info != "")
															interesting_buffer.insert_or_assign(a_ref, info);
													}
											}
										}

										if (a_ref->GetDistance(player_ref) < 500.0f)
										{
											if (base_obj->IsInventoryObject())
											{
												if (!MiscThings::is_object_in_the_list(a_ref))
												{
													std::string info = MiscThings::insert_object_into_list_and_get_info(a_ref);
													if (info != "")
														interesting_buffer.insert_or_assign(a_ref, info);
												}
											}
										}


									}

								}
								else
								{
									//nameless things that still have to be tracked.
									//FXspiderWebKitDoorSpecial - normal
									//FXspiderWebKitDoorSpecialDest - destroyed


									if (a_ref->IsActor() && a_ref != player_ref)
									{
										//may be ghost targets of greybeards

										if (!MiscThings::is_object_in_the_list(a_ref))
											if (a_ref->GetDistance(player_ref) < 200.0f || (jail_condition_raycastable || ignore_raycast || MiscThings::raycastable(a_ref, 5000.0f, false)))
											{
												auto actor = (RE::Actor*)a_ref;
												auto npc = (RE::TESNPC*)a_ref->data.objectReference;

												auto model3d = actor->GetCurrent3D();


												//no name = ghost (i guess)
												// 
												//if (actor->IsGhost())
												{
													std::string info = MiscThings::insert_object_into_list_custom_name(" Ghost", a_ref);
													if (info != "")
													{
														//give it immidiately
														send_random_context("You see: " + info, false);


														auto ghost_shouting_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ105");

														if (ghost_shouting_quest)
														{
															int quest_stage = ghost_shouting_quest->GetCurrentStageID();

															if (quest_stage == 85 || quest_stage == 80)
															{
																active_puzzle = 1;
																puzzle_target = a_ref;
															}
														}

													}
													//interesting_buffer.insert_or_assign(a_ref, info);
												}
											}

									}

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
													{
														std::string info = MiscThings::insert_object_into_list_custom_name("[Destructible] Cobweb", a_ref);
														if (info != "")
															interesting_buffer.insert_or_assign(a_ref, info);
													}
												}
											}

										}




									}

								}
							}

							



							

							return RE::BSContainer::ForEachResult::kContinue;
						});


					



				}

			}
			else
				detect_interesting_time += dtime;


			if (detect_interesting_spit_results_time > 5.0f)
			{
				detect_interesting_spit_results_time = 0.0f;
				auto player_ref = RE::PlayerCharacter::GetSingleton()->AsReference();
				auto player_pos = player_ref->GetPosition();

				std::vector<std::pair<RE::TESObjectREFR*, std::string>> sortable_copy{};

				for (std::pair<RE::TESObjectREFR*, std::string> raw : interesting_buffer)
				{
					sortable_copy.push_back(raw);
				}


				std::sort(sortable_copy.begin(), sortable_copy.end(), [&](std::pair<RE::TESObjectREFR*, std::string> left, std::pair<RE::TESObjectREFR*, std::string> right) {
					//return left->GetDistance(player) > right->GetDistance(player); //switch > to < for inversed order. this is last->closest
					if (left.first->data.objectReference && right.first->data.objectReference)
					{
						RE::NiPoint3 pos_left = left.first->GetPosition();
						RE::NiPoint3 pos_right = right.first->GetPosition();

						return pos_left.GetDistance(player_pos) < pos_right.GetDistance(player_pos); //alphabetical order. top = A
					}
					else
						return false;
					});

				std::string info_string = "";


				bool veryclose_line_made = false;
				bool nearby_line_made = false;
				bool faraway_line_made = false;

				std::string last_name = "";
				bool has_last = false;

				bool mzark_special = false;

				for (auto result_entry : sortable_copy)
				{
					if (result_entry.first && result_entry.first->data.objectReference)
					{
						auto this_object = result_entry.first->data.objectReference;
						auto distance = player_ref->GetDistance(result_entry.first);

						if (result_entry.first == RE::TESObjectREFR::LookupByID(0x1ba5f))
							mzark_special = true;

						if (this_object && result_entry.second != "")
						{
							if (distance < 450.0f)
								if (!veryclose_line_made)
								{
									std::string last_name = "";
									bool has_last = false;
									info_string += "\nVery close:\n";
									veryclose_line_made = true;
								}

							if (distance >= 450.0f && distance < 2000.0f)
								if (!nearby_line_made)
								{
									std::string last_name = "";
									bool has_last = false;
									info_string += "\nNearby:\n";
									nearby_line_made = true;
								}


							if (distance >= 2000.0f && distance < 10000.0f)
								if (!faraway_line_made)
								{
									std::string last_name = "";
									bool has_last = false;
									info_string += "\nFar away:\n";
									faraway_line_made = true;
								}



							//std::string category = get_object_category(object.second);

							std::string result_name = result_entry.second;//insert_object_into_list_and_get_info(this_object); //they are all in the list but whatever. just to get the name

							auto id_end = result_name.find_first_of("]");

							if (result_name != "" && id_end != std::string::npos)
							{
								std::string name_no_id = result_name.substr(id_end + 1, result_name.length() - id_end);
								std::string id_text_raw = result_name.substr(0, id_end + 1);
								std::string id_text = result_name.substr(4, id_end - 4);


								if (has_last && name_no_id == last_name)
								{
									auto last_start = info_string.rfind("\n", info_string.length() - 2);

									auto last_id_start = info_string.find("[id", last_start);

									if (last_id_start == std::string::npos || last_start == std::string::npos)
									{
										info_string += result_name + "\n";
									}
									else
									{
										if (info_string.substr(last_id_start + 3, 1) != "s")
										{
											info_string.insert(last_id_start + 3, "s");
										}

										auto last_substr = info_string.substr(last_id_start, info_string.length() - last_id_start);

										auto last_id_sub_end = last_substr.find_first_of("]");

										auto last_id_insert_pos = last_id_start + last_id_sub_end;

										info_string.insert(last_id_insert_pos, ", " + id_text);
									}

								}
								else
									info_string += result_name + "\n";

								last_name = name_no_id;
								has_last = true;
							}


						}
					}
					

				}

				if (info_string != "")
				{
					info_string = "[You see objects around you: \n" + info_string;
					send_random_context(info_string);


					if (mzark_special)
					{
						auto astrolabe = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xa0334);

						if (!WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path())
						{
							WalkerProcessor::look_at_object_by_refr(astrolabe, true, 0.25f);
							send_random_context("You walk into some spherical room, there is a large dwemer mechanism in the middle. A huge armillary, and some astrolabe with green crystal lenses above it...", false);
						}
					}
				}

				interesting_buffer.clear();
			}
			else
				detect_interesting_spit_results_time += dtime;

		}
	}



	struct old_object_state {
		bool dead{};
		bool fleeing{};
		RE::RefHandle target;
		int action_flags;
		int pillar_face_code;
		int trap_firing;
		int destructible_state;
		bool reanimating;
		int wakeup;
	};


	std::map<RE::TESObjectREFR*, old_object_state> objects_to_track{};


	long long last_periodic_info = 0;


	void clear_objects_to_track()
	{
		objects_to_track.clear();
	}



	std::string get_trap_activator_name(RE::TESObjectREFR* a_ref)
	{
		std::vector<RE::TESObjectREFR*> activator_candidates{};
		std::string activator_name = "";

		auto player = RE::PlayerCharacter::GetSingleton();
		auto player_ref = player->AsReference();

		RE::TES::GetSingleton()->ForEachReferenceInRange(a_ref, 300.0f,
			[&](RE::TESObjectREFR* a_ref) {

				if (MiscThings::is_new_object_valid(a_ref))
					activator_candidates.push_back(a_ref);

				return RE::BSContainer::ForEachResult::kContinue;
			});

		std::sort(activator_candidates.begin(), activator_candidates.end(), [&](RE::TESObjectREFR* left, RE::TESObjectREFR* right) {
			if (left->data.objectReference && left->data.objectReference &&
				right->data.objectReference)
				return left->GetDistance(a_ref) < right->GetDistance(a_ref); //switch > to < for inversed order. this is last->closest
			else
				return false;
			});

		RE::TESObjectREFR* the_activator = nullptr;
		for (auto candidate : activator_candidates)
		{
			if (candidate != a_ref)
			{
				the_activator = candidate;
				break;
			}
		}

		if (the_activator)
		{
			if (the_activator == player_ref)
				activator_name = "You";
			else
			{
				activator_name = the_activator->GetDisplayFullName();
			}
		}

		return activator_name;
	}




	void detect_events(float dtime)
	{
		if (observers_green_light)
		{
			if (detect_events_time > 0.1f)
			{
				detect_events_time = 0.0f;
				std::vector<std::string> result{};
				auto player = RE::PlayerCharacter::GetSingleton();
				auto player_ref = player->AsReference();
				auto player_actor = (RE::Actor*)player_ref;

				float scan_distance = 2000.0f;
				auto player_cell = player->GetParentCell();
				if (player_cell && player_cell->IsInteriorCell())
					scan_distance = 2000.0f;



				auto carriage_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("DialogueCarriageSystem");

				auto object_p = MiscThings::General::Script::GetObject(carriage_quest, "CarriageSystemScript");

				RE::BSFixedString prop_name = "currentDestination";
				int current_destination = MiscThings::General::Script::GetProperty<int>(object_p, prop_name);

				prop_name = "currentDriver";
				RE::Actor* driver = MiscThings::General::Script::GetProperty<RE::Actor*>(object_p, prop_name);

				if (current_destination != 0 && current_destination != -1 && driver)
				{

					auto extra = driver->extraList.GetByType(RE::ExtraDataType::kLinkedRef);

					auto carriage_seat = RE::TESForm::LookupByEditorID("LinkCarriageSeat");

					if (extra)
					{
						auto extra_linked = (RE::ExtraLinkedRef*)extra;
						for (auto linked_ref : extra_linked->linkedRefs)
						{
							if (linked_ref.keyword == carriage_seat && !WalkerProcessor::is_getting_into_carriage() && !MiscThings::is_intro())
							{
								unregister_all_actions();
								WalkerProcessor::get_into_carriage(linked_ref.refr);
								return;
							}
						}
						
					}
				}

				

				//if (std::size(objects_to_track) > 5000)
				//	objects_to_track.clear();

				bool silent = false;

				RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, scan_distance,
					//player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
					[&](RE::TESObjectREFR* a_ref) {

						auto base_obj = a_ref->GetBaseObject();
						auto base_type = base_obj->GetFormType();





						//if (base_type == RE::FormType::Static || base_type == RE::FormType::Hazard)
						if (base_type == RE::FormType::Hazard)
							return RE::BSContainer::ForEachResult::kContinue;


						if (a_ref->IsActor())
						{
							//std::string name1 = a_ref->GetDisplayFullName();
							//if (name1 == "Headsman")
							//{
							//	bool headsman_found = true;
							//}


							auto actor_ref = (RE::Actor*)a_ref;

							//actor_ref->currentProcess->high.

							

							if (objects_to_track.find(a_ref) == objects_to_track.end())
							{
								auto new_target = 0;
								if (actor_ref->currentProcess)
									new_target = actor_ref->currentProcess->target;

								bool is_fleeing = false;
								if (actor_ref->combatController)
									is_fleeing = actor_ref->combatController->IsFleeing();//actor_ref->currentProcess->middleHigh->unk326;

								old_object_state state = { a_ref->IsDead(), is_fleeing,  new_target, (int)actor_ref->actorState1.flyState, 0, -1, -1, (int)actor_ref->actorState2.reanimating, (int)actor_ref->actorState1.sitSleepState };
								objects_to_track.insert({ a_ref, state });
							}
							else
							{
								auto old_entry = objects_to_track.find(a_ref);
								old_object_state old_state = old_entry->second;

								bool is_fleeing = false;
								if (actor_ref->combatController)
									is_fleeing = actor_ref->combatController->IsFleeing();//actor_ref->currentProcess->middleHigh->unk326;

								auto new_target = 0;
								if (actor_ref->currentProcess)
									new_target = actor_ref->currentProcess->target;

								old_object_state new_state = { a_ref->IsDead(), is_fleeing, new_target, old_state.action_flags, old_state.pillar_face_code, old_state.trap_firing , old_state.destructible_state, (int)actor_ref->actorState2.reanimating, (int)actor_ref->actorState1.sitSleepState };


								if (old_state.dead != new_state.dead)
								{
									if (new_state.dead)
									{
										bool dont_add = false;

										objects_to_track.insert_or_assign(a_ref, new_state);

										std::string victim_name = MiscThings::insert_object_into_list_and_get_info(a_ref);
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
													std::string killer_name = MiscThings::insert_object_into_list_and_get_info(killer_actor);
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
										{
											silent = true;
											result.push_back(message_text);
										}
											
									}
									
								}

								if (old_state.wakeup != new_state.wakeup)
								{
									std::string message_text = "";

									std::string waker_name = MiscThings::insert_object_into_list_and_get_info(a_ref);

									if (new_state.wakeup == (int)RE::SIT_SLEEP_STATE::kWantToWake)
									{
										message_text = "[" + waker_name + " wakes up]";
									}

									if (new_state.wakeup == (int)RE::SIT_SLEEP_STATE::kWantToSleep)
									{
										message_text = "[" + waker_name + " goes to sleep]";
									}

									if (message_text != "")
									{
										silent = true;
										result.push_back(message_text);
									}
										

									objects_to_track.insert_or_assign(a_ref, new_state);
								}


								if (old_state.reanimating != new_state.reanimating)
								{
									if (new_state.reanimating)
									{
										std::string zombie_name = MiscThings::insert_object_into_list_and_get_info(a_ref);

										std::string message_text = "";

										message_text = "[" + zombie_name + " is reanimating!]";

										if (zombie_name != "")
										{
											silent = true;
											result.push_back(message_text);
										}
									}

									objects_to_track.insert_or_assign(a_ref, new_state);
								}


								if (old_state.fleeing != new_state.fleeing)
								{
									if (!a_ref->IsDead())
									{
										std::string victim_name = MiscThings::insert_object_into_list_and_get_info(a_ref);

										std::string message_text = "";

										if (new_state.fleeing)
											message_text = "[" + victim_name + " is fleeing]";
										else
											message_text = "[" + victim_name + " stopped fleeing]";

										if (victim_name != "")
										{
											silent = true;
											result.push_back(message_text);
										}
											
									}

									objects_to_track.insert_or_assign(a_ref, new_state);
								}

								if (old_state.target != new_state.target)
								{
									if (old_state.target != 0 && new_state.target == 0)
									{
										//interacted with something?
										objects_to_track.insert_or_assign(a_ref, new_state);

										bool new_target = false;

										auto p_target = RE::TESObjectREFR::LookupByHandle(new_state.target);
										if (!p_target)
										{
											p_target = RE::TESObjectREFR::LookupByHandle(old_state.target);
										}
										else
											new_target = true;

										if (p_target)
										{
											auto target_refr = p_target.get();
											if (target_refr && target_refr != player_ref && a_ref && a_ref != player_ref)
											{
												std::string target_short_name = target_refr->GetDisplayFullName();
												std::string actor_short_name = a_ref->GetDisplayFullName();
												if (target_short_name != "" && actor_short_name != "")
												{
													std::string interaction_name = " interacted with ";

													auto target_base_obj = target_refr->GetBaseObject();
													auto target_base_type = target_base_obj->GetFormType();

													if (target_base_type == RE::FormType::Furniture)
													{
														return RE::BSContainer::ForEachResult::kContinue;

													if (target_refr == a_ref)
														return RE::BSContainer::ForEachResult::kContinue; //alduin kills himself during helgen attack for some reason. skip it

													if (actor_ref->race->fullName == "Dragon Race" && target_refr->IsActor())
														interaction_name = " killed "; //when dragon interacts with actor it means he eats them or something like that

													auto target_name = MiscThings::insert_object_into_list_and_get_info(target_refr);
													std::string actor_name = MiscThings::insert_object_into_list_and_get_info(a_ref);

													if (target_name != "" && actor_name != "")
													{
															auto furniture_obj = (RE::TESFurniture*)target_base_obj;

															if (furniture_obj->furnFlags.any(RE::TESFurniture::ActiveMarker::kCanSit))
															{
																//chair
																if (new_target) //THIS IS WRONG WAY OF DOING IT. only stood up makes sense
																	return RE::BSContainer::ForEachResult::kContinue; //interaction_name = " sat on ";
																else
																	interaction_name = " stood up from ";
															}
															else
															{
																if (new_target)
																	return RE::BSContainer::ForEachResult::kContinue;
															}
														}
														else
														{
															if (new_target)
																return RE::BSContainer::ForEachResult::kContinue;
														}

														std::string message = "[" + actor_name + interaction_name + target_name;
														if (!a_ref->IsDead())
														{
															silent = true;
															result.push_back(message);
														}
															
													}

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

							
							auto test_refr = RE::TESObjectREFR::LookupByID(0x945b8);
							
							if (a_ref == test_refr)
								bool break_here = false;

							if (true || base_type == RE::FormType::Activator || base_type == RE::FormType::Door)
							{
								if (objects_to_track.find(a_ref) == objects_to_track.end())
								{
									old_object_state state = { 0, 0, 0, 0, 0, -1, -1, 0, 0 };
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

									state = { 0, 0, 0, activation, pillar_face, trap_firing, destructible_state, 0, 0 };

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

									old_object_state new_state = { 0, 0, 0, activation, pillar_face, trap_firing, destructible_state, 0, 0 };


									if (base_type == RE::FormType::Door)// && a_ref->GetDisplayFullName() == "")
									{
										if (old_state.action_flags != new_state.action_flags)
										{
											auto door = (RE::TESObjectDOOR*)base_object;
											std::string model = door->GetModel();

											if (model.find("CaveGSecretDoor") != std::string::npos)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Secret stone wall door", a_ref);

												if (old_state.action_flags == 2)
													if (activation == 0)
														result.push_back("[ " + name + " opened]");

												if (old_state.action_flags == 3)
													if (activation == 1)
														result.push_back("[ " + name + " closed]");

												if (old_state.action_flags == 1)
													if (activation == 2)
														result.push_back("[ " + name + " is opening...]");

												if (old_state.action_flags == 0)
													if (activation == 3)
														result.push_back("[ " + name + " is closing...]");
											}

											if (model.find("DwePtnDoor01") != std::string::npos)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer metal gate", a_ref);

												if (std::size(result) == 0 || result.at(std::size(result) - 1).find("Dwemer metal gate") == std::string::npos)
												{
													if (old_state.action_flags == 3)
														if (activation == 1)
															result.push_back("[ " + name + " closed]");

													if (old_state.action_flags == 2)
														if (activation == 0)
															result.push_back("[ " + name + " opened]");
												}
											}


											if (model.find("TrapDoorALT01") != std::string::npos)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Nordic metal floor gate", a_ref);

												if (activation == 1)
													result.push_back("[ " + name + " closed]");

												if (activation == 0)
													result.push_back("[ " + name + " opened]");
											}


											if (model.find("WRPrisonCellFloorGrate01Door") != std::string::npos)
											{
												std::string name = MiscThings::insert_object_into_list_custom_name("Secret stone wall door", a_ref);

												if (activation == 2 && MiscThings::player_escaping_jail() && player_cell && player_cell->GetFormID() == 0x4A376)
													WalkerProcessor::walk_whiterun_prison_grate();
												
											}

											//Architecture\WhiteRun\WRInteriors\WRDungeon\WRPrisonCellFloorGrate01Door.nif

										}
									}

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
															std::string name = MiscThings::insert_object_into_list_custom_name("Large wooden bridge", a_ref);

															if (activation == 0)
																result.push_back("[ " + name + " closed]");

															if (activation == 1)
																result.push_back("[ " + name + " opened]");
														}

														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweLexiconStandBlank01")
														{
															std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

															if (activation == 1)
																result.push_back("[You insert Blank Lexicon Cube into " + name + " stand...]");

															if (activation == -1)
																result.push_back("[You take Blank Lexicon Cube out of " + name + " stand...]");

														}


														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweLexiconStandRunes01")
														{
															std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

															//if (activation == 1)
															//	result.push_back("[You insert Blank Lexicon Cube into " + name + " stand...]");

															if (activation == -1)
																result.push_back("[You take Enscribed Lexicon Cube out of " + name + " stand...]");

														}


														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweAstrolabeArmillary01")
														{
															std::string name = "Dwemer armillary mechanism";

															if (activation == 10)
															{
																result.push_back("[" + name + " started rotating...]");
																WalkerProcessor::look_at_object_by_refr(a_ref, true, 0.25f);
															}
															else
																result.push_back("[" + name + " stopped rotating, now it is in a new position]");

														}

														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweAstrolabeHub01")
														{
															std::string name = "Dwemer astrolabe mechanism";

															if (activation == 10)
															{
																result.push_back("[" + name + " started rotating...]");
																WalkerProcessor::look_at_object_by_refr(a_ref, true);
															}
															else
																if (activation == 20)
																{
																	result.push_back("[" + name + " stopped moving... the mechanism lowered some big crystal capsule from the ceiling. The capsule opens... and you see some shining scroll inside]");
																	auto elder_scroll_pickup = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x88268);
																	WalkerProcessor::look_at_object_by_refr(elder_scroll_pickup, true);
																}
																else
																	result.push_back("[" + name + " stopped rotating, now it is in a new position]");

														}

														//


														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "DweButton01")
														{
															std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

															if (old_state.action_flags == 1)
																result.push_back("[" + name + " opened, now it can be pressed]");

															if (activation == 1)
																result.push_back("[" + name + " closed, now it cannot be pressed]");

														}



														if (extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName == "SkyHavenRetractableBridge01")
														{
															std::string name = MiscThings::insert_object_into_list_custom_name("Stone bridge", a_ref);




															if (activation == 0)
																result.push_back("[ " + name + " closed]");

															if (activation == 1)
															{
																RE::TESObjectREFR* karthspire_bridge1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4efba);
																RE::TESObjectREFR* karthspire_bridge2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4e862);

																if (a_ref == karthspire_bridge1 || a_ref == karthspire_bridge2)
																	quicksave();


																result.push_back("[ " + name + " opened]");
															}
																
														}

														std::string anim_name = extra_anim_graph->animGraphMgr->variableCache.animationGraph->projectName.c_str();

														if (anim_name == "NorSecRmSmDoorSm01")
														{
															std::string name = MiscThings::insert_object_into_list_custom_name("Secret cave wall door", a_ref);

															if (activation == 1)
																result.push_back("[ " + name + " closed]");

															if (activation == 0)
																result.push_back("[ " + name + " opened]");
														}

														if (anim_name == "ImpPortcullisSmall01")
														{
															std::string name = MiscThings::insert_object_into_list_custom_name("Heavy wooden gate", a_ref);

															if (activation == 0)
																result.push_back("[ " + name + " opened]");

															if (activation == 1)
																result.push_back("[ " + name + " closed]");
														}

														//

														if (anim_name == "NorPortcullisGate01")
														{
															std::string name = MiscThings::insert_object_into_list_custom_name("Small metal gate", a_ref);

															if (activation == 0)
																result.push_back("[ " + name + " opened]");

															if (activation == 1)
																result.push_back("[ " + name + " closed]");
														}


														if (anim_name == "PortcullisLarge01")
														{
															std::string name = MiscThings::insert_object_into_list_custom_name("Metal gate", a_ref);

															if (activation == 0)
																result.push_back("[ " + name + " opened]");

															if (activation == 1)
																result.push_back("[ " + name + " closed]");
														}

														if (anim_name == "SarcophagusTopOpen")
														{
															std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
															if (activation == 1)
																result.push_back("[ " + name + " crumbles and reveals the passage behind it]");

														}


														if (anim_name == "PortGatePole06")
														{
															std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer metal pole gate", a_ref);

															if (activation == 0)
																result.push_back("[ " + name + " opened]");

															if (activation == 1)
																result.push_back("[ " + name + " closed]");
														}


														if (anim_name == "PortGatePoleDwemer01")
														{
															std::string name = MiscThings::insert_object_into_list_custom_name("Dwemer metal pole gate", a_ref);

															if (activation == 0)
																result.push_back("[ " + name + " opened]");

															if (activation == 1)
																result.push_back("[ " + name + " closed]");
														}


														if (anim_name.find("PuzzleDoorKeyHole") != std::string::npos && anim_name.find("PuzzleDoorKeyHoleIvory") == std::string::npos)
														{
															std::string name = MiscThings::insert_object_into_list_custom_name("[Puzzle door] Ancient Nordic Door", a_ref);

															//if (activation == 0)
															//	result.push_back("[" + name + " 0]");

															if (activation == 1)
																result.push_back("[" + name + " is opening...]");

															//if (activation == 2)
															//	result.push_back("[" + name + " 2]");


															//if (activation == 3)
															//	result.push_back("[" + name + " 3]");

															if (activation == 4)
																result.push_back("[" + name + " didn't move... Seems like the puzzle rings on it must be rotated in correct positions for the key to work]");
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

											//if (pillar_face_name != "")
											//	solved_text = MiscThings::get_pillar_solved_text(a_ref);

											result.push_back(pillar_name + " turned to" + pillar_face_name + solved_text);
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
												std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging blade", a_ref);
												result.push_back("[ " + name + " swinged!]");
												silent = true;
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
											std::string name = MiscThings::insert_object_into_list_custom_name("Pressure plate", a_ref);

											std::string activator_name = get_trap_activator_name(a_ref);

											silent = true;

											if (activator_name == "")
											{
												result.push_back("[ " + name + " was triggered!]");
											}
											else
											{
												result.push_back("[" + activator_name + " triggered " + name + "!]");
											}

										}


										if (new_state.trap_firing == 3)
										{
											std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging wall", a_ref);
											result.push_back("[ " + name + " launched!]");
										}


										if (new_state.trap_firing == 4)
										{
											std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging wall", a_ref);
											result.push_back("[ " + name + " went back]");
										}

										if (new_state.trap_firing == 5)
										{
											std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging blade", a_ref);
											result.push_back("[ " + name + " deactivated]");
										}

										if (new_state.trap_firing == 6)
										{
											std::string name = MiscThings::insert_object_into_list_custom_name("Trap swinging blade", a_ref);
											result.push_back("[ " + name + " started swinging!]");
										}

										if (new_state.trap_firing == 7)
										{
											std::string name = MiscThings::insert_object_into_list_custom_name("Trap oil lamp", a_ref);
											result.push_back("[ " + name + " fell down and exploded!]");
										}

										if (new_state.trap_firing == 8)
										{
											std::string name = MiscThings::insert_object_into_list_custom_name("Oil on the floor", a_ref);
											result.push_back("[ " + name + " started burning]");
										}

										if (new_state.trap_firing == 9)
										{
											if (std::size(result) == 0 || result.at(std::size(result) - 1) != "[Boulders fall from the ceiling!]")
												result.push_back("[Boulders fall from the ceiling!]");
										}

										if (new_state.trap_firing == 10)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

											std::string activator_name = get_trap_activator_name(a_ref);

											if (activator_name != "")
												result.push_back("[" + activator_name + " triggered " + name + "!]");
											else
												result.push_back("[ " + name + " was triggered!]");
										}


										if (new_state.trap_firing == 11)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);

											std::string activator_name = get_trap_activator_name(a_ref);

											if (activator_name != "")
												result.push_back("[" + activator_name + " triggered " + name + "!]");
											else
												result.push_back("[ " + name + " was triggered!]");
										}


										if (new_state.trap_firing == 12)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
											result.push_back("[ " + name + " was rearmed]");
										}


										if (new_state.trap_firing == 13)
										{
											if (base_type == RE::FormType::Activator)
											{
												auto acti = (RE::TESObjectACTI*)base_obj;
												std::string model = acti->GetModel();

												if (model.find("TrapSkullRam0") != std::string::npos)
												{
													result.push_back("[Mammoth skull on a rope swings!]");
												}

												if (model.find("TrapMace01") != std::string::npos)
												{
													result.push_back("[Mace on a rope swings!]");
												}
											}
										}


										if (new_state.trap_firing == 14)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
											result.push_back("[ " + name + " was triggered!]");
										}


										if (new_state.trap_firing == 15)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
											result.push_back("[ " + name + " was disarmed]");
										}

										if (new_state.trap_firing == 16)
										{
											result.push_back("[Battering ram log swings from the ceiling!]");
										}
										//only when close.



									}

									if (old_state.destructible_state != new_state.destructible_state)
									{
										if (new_state.destructible_state == 1)
										{
											std::string name = MiscThings::insert_object_into_list_custom_name("[Destructible] Cobweb", a_ref);

											result.push_back(name + " was destroyed");
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

									old_object_state state = { a_ref->IsDead(), 0, new_target, 0 };
									objects_to_track.insert({ a_ref, state });
								}
								else
								{
									auto old_entry = objects_to_track.find(a_ref);
									old_object_state old_state = old_entry->second;


									old_object_state new_state = { old_state.dead, 0, old_state.target, (int)actor_ref->actorState1.flyState };

									objects_to_track.insert_or_assign(a_ref, new_state);

									if (old_state.action_flags != new_state.action_flags)
									{
										silent = true;

										std::string in_the_distance = "";

										if (player_ref->GetDistance(a_ref) > 15000.0f)
											in_the_distance = " in the distance";

										if (new_state.action_flags & (int)RE::FLY_STATE::kCruising)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
											std::string action = " is flying";
											if (result.empty() && name != "")
												result.push_back("[" + name + action + in_the_distance + "]");
										}

										if (new_state.action_flags & (int)RE::FLY_STATE::kLanding)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
											std::string action = " is landing";
											if (result.empty() && name != "")
												result.push_back("[" + name + action + in_the_distance + "]");
										}

										if (new_state.action_flags & (int)RE::FLY_STATE::kHovering)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
											if (result.empty() && name != "")
												std::string action = " is hovering";

										}

										if (new_state.action_flags & (int)RE::FLY_STATE::kTakeOff)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
											std::string action = " takes off";
											if (result.empty() && name != "")
												result.push_back("[" + name + action + in_the_distance + "]");
										}

										if (new_state.action_flags & (int)RE::FLY_STATE::kPerching)
										{
											std::string name = MiscThings::insert_object_into_list_and_get_info(a_ref);
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

					send_random_context(message, silent);

				}


			}
			else
				detect_events_time += dtime;
		}

	}




	

	void detect_locations(float dtime)
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto player_worldspace = player->GetWorldspace();
		auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);


		if (observers_green_light && !MiscThings::have_force_only_menu_open() && player_worldspace == tamriel_worldspace)
		{
			if (detect_locations_timer > 30.0f && !(MiscThings::is_intro() || MiscThings::is_intro2()))
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

						if (real_marker && !real_marker->IsDisabled() && data && data->mapData)// && marker_name.find("Military Camp") == std::string::npos) //just filter military camps for now
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
	}


	


	void inventory_monitor(float dtime)
	{

		RE::UI* ui = RE::UI::GetSingleton();

		if (player_monitor_finished && observers_green_light && !ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) && !ui->IsMenuOpen(RE::MainMenu::MENU_NAME) && !MiscThings::is_intro() && !MiscThings::is_intro2())
		{
			if (inventory_monitor_timer > 0.5f)
			{
				/////// ITEMS ADDED
				std::string new_info = "";

				RE::TESObjectREFR::InventoryItemMap inventory = MiscThings::get_filtered_inventory();

				for (auto& [item, data] : inventory)
				{
					if (!MiscThings::is_inventory_item_in_the_list(item))
					{
						std::string info = MiscThings::insert_item_into_inventory_list_and_get_info(item);

						if (info == "")
							continue;

						new_info += info + "\n";
					}
				}

				if (new_info != "")
				{
					std::string message = "";
					if (!not_first_inventory_info)
					{
						not_first_inventory_info = true;
						message = "[Your inventory contents: ";
					}
					else
						message = "[New items in the inventory: ";

					message += new_info;
					send_random_context(message);
				}

				///// ITEMS REMOVED

				auto p_inventory = MiscThings::get_p_inventory_items_list();

				std::string removed_info = "";

				for (auto it = p_inventory->cbegin(); it != p_inventory->cend();)
				{
					auto item = it->second.object;

					if (inventory.find(item) == inventory.end() || inventory.find(item)->second.first <= 0)
					{
						std::string info = MiscThings::insert_item_into_inventory_list_and_get_info(item);

						if (info != "")
						{
							removed_info += info + "\n";
						}

						it = p_inventory->erase(it);    // or "it = m.erase(it)" since C++11
					}
					else
					{
						++it;
					}
				}


				if (removed_info != "")
				{
					std::string message = "[No longer in the inventory: ";
					message += removed_info;
					if (!dont_inform_inventory)
						send_random_context(message);
				}


			}
			else
				inventory_monitor_timer += dtime;

		}
	}









	void player_state_monitor(float dtime)
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		if (wait_and_send_game_start_context)
		{
			if (start_game_timer > 11.777f)
			{
				//game start context
				send_random_context("You awake on a carriage going down a mountain to Helgen. You were unfortunate enough to stumble across an imperial raid on rebels while crossing the border and were arrested with other Stormcloaks, a horse thief, and Ulfric Stormcloak himself. Everyone assumes correctly that they are being led to their execution.", false);
				wait_and_send_game_start_context = false;
				register_allowed_actions();

			}
			else
				start_game_timer += dtime;
		}


		auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");

		if (threshold_quest && !observers_green_light)
		{
			int unbound_quest_stage = threshold_quest->GetCurrentStageID();
			if (unbound_quest_stage >= 15)
			{
				if (unbound_quest_stage < 50)
				{
					if (green_light_delay > 15.0f && !wait_and_send_game_start_context)
					{
						observers_green_light = true;
					}
					else
						green_light_delay += dtime;
				}
				else
					observers_green_light = true;
			}
		}


		
		//700c4bc - huge cutter
		//700ca1f - huge cutter2


		if (last_saved_time > 180.0f && !WalkerProcessor::is_fighting() && !WalkerProcessor::is_walking_important_path() && !MiscThings::have_force_only_menu_open() && get_active_force() == -1 && MiscThings::player_hp_more_than(0.3f))
		{
			quicksave();
			last_saved_time = 0.0f;
		}
		else
			last_saved_time += dtime;

		RE::UI* ui = RE::UI::GetSingleton();
		if (!ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) && !ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
		{
			if (state_monitor_timer > 0.5f)
			{
				if (first_cycle)
				{
					first_cycle = false;
					//auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
					
					if (threshold_quest)
					{
						int unbound_quest_stage = threshold_quest->GetCurrentStageID();
						if (unbound_quest_stage < 160)
						{
							old_unbound_quest_stage = unbound_quest_stage;
						}
					}
						

					auto control_map = RE::ControlMap::GetSingleton();
					bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
					bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);
					bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
					bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);

					old_can_interact = can_interact;
					old_can_look = can_look;
					old_can_walk = can_walk;
					old_can_fight = can_fight;

					auto greybeard_call = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQGreybeardCall");
					if (greybeard_call)
						old_greybeard_call_stage = greybeard_call->GetCurrentStageID();



					old_had_any_quests = MiscThings::have_any_quests();
					old_sit_state = player->GetSitSleepState();
					old_furniture_name = "";


				}
				else
				{
					//auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
					if (threshold_quest)
					{
						int unbound_quest_stage = threshold_quest->GetCurrentStageID();
						if (unbound_quest_stage != old_unbound_quest_stage)
						{

							if (unbound_quest_stage == 15)
								wait_and_send_game_start_context = true;

							if (unbound_quest_stage == 54)
							{
								//unregister_all_actions();
								send_random_context("[You are getting out of the carriage with the others...]", false);
							}
								

							if (unbound_quest_stage == 70)
							{
								send_random_context("[You got out of the carriage]", false);
							}
								

							if (unbound_quest_stage == 80)
								send_random_context("[You walk after captain towards the block, other prisoners are already there...]", false);

							if (unbound_quest_stage == 82)
								send_random_context("[A loud roar is heard in the distance]", false);

							if (unbound_quest_stage == 85)
								send_random_context("[Stormcloak soldier is forced to put his head on the block...]", false);

							if (unbound_quest_stage == 90)
								send_random_context("[Stormcloak soldier has been beheaded by Headsman]", false);

							if (unbound_quest_stage == 95)
							{
								//unregister_all_actions();
								send_random_context("[You walk towards the block...]", false);
							}
								

							if (unbound_quest_stage == 97)
								send_random_context("[You are pushed down on the beheading block]", false);

							if (unbound_quest_stage == 100)
								send_random_context("[You lie down on the block. Ready to be executed...]", false);

							if (unbound_quest_stage == 120)
								send_random_context("[Headsman puts his axe in the air above you...]", false);

							if (unbound_quest_stage == 130)
								send_random_context("[The ground shakes, Headsman loses balance and falls on the ground, you are unharmed]", false);

							if (unbound_quest_stage == 140)
								send_random_context("[The dragon shouts. Firestorm begins. Meteorites fall from the sky, buildings are getting destroyed. You try to get up...]", false);

							if (unbound_quest_stage == 150)
								send_random_context("[You stood up]", false);

							if (unbound_quest_stage == 160)
							{
								unregister_look_action();
								register_allowed_actions();
								send_random_context("[YOU CAN WALK NOW]", false);
							}
								

							old_unbound_quest_stage = unbound_quest_stage;
						}
					}
				}


				state_monitor_timer = 0.0f;
				auto player_ref = player->AsReference();
				auto player_actor = (RE::Actor*)player_ref;

				int health = player->GetActorValue(RE::ActorValue::kHealth);
				int stamina = player->GetActorValue(RE::ActorValue::kStamina);
				int mana = player->GetActorValue(RE::ActorValue::kMagicka);

				int max_health = player->GetActorValueMax(RE::ActorValue::kHealth);
				int max_stamina = player->GetActorValueMax(RE::ActorValue::kStamina);
				int max_mana = player->GetActorValueMax(RE::ActorValue::kMagicka);

				bool player_dead = player->IsDead();

				auto control_map = RE::ControlMap::GetSingleton();
				bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
				bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
				bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
				bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);


				auto escaping_jail = player->playerFlags.escaping;

				//auto serving_jail = player->playerFlags.servingJailTime;
				auto jail_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("JailQuest");
				//bool serving_jail = jail_quest->IsRunning() && (jail_quest->GetCurrentStageID() == 10) && (!jail_quest->data.flags.all(RE::QuestFlag::kDisplayedInHUD)) && !escaping_jail && !MiscThings::is_intro() && !MiscThings::is_intro2() && MiscThings::escaped_helgen();
				bool serving_jail = (bool)player->currentPrisonFaction && !escaping_jail && !MiscThings::is_intro() && !MiscThings::is_intro2() && MiscThings::escaped_helgen();
				
				//serving_jail |= (bool)player->currentPrisonFaction;


				auto greybeard_call = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQGreybeardCall");

				if (greybeard_call)
				{
					int greybeard_call_stage = greybeard_call->GetCurrentStageID();

					if (greybeard_call_stage == 10 && old_greybeard_call_stage == 0)
					{
						greybeards_called = true;
					}

					old_greybeard_call_stage = greybeard_call_stage;
				}
				


				if (greybeards_called)
				{
					if (greybeards_called_timer > 1.0f)
					{
						greybeards_called = false;
						greybeards_called_timer = 0.0f;
						send_random_context("The ground shakes, and loud as thunder shout is heard from the sky, it said: Do-va-kiin!", false);
					}
					else
						greybeards_called_timer += 0.5f;
				}


				//hit events




				//auto test_wall_of_power = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70b4a);

				//auto base_obj_test = test_wall_of_power->GetBaseObject();
				//auto base_type = base_obj_test->GetFormType();

				//WordWallTriggerScript

				bool objects_around_valid = MiscThings::is_objects_around_valid();

				if (objects_around_valid && !old_objects_around_valid)
				{
					if (!MiscThings::is_intro())
					{
						if (!MiscThings::is_intro2())
							register_walk_to_object();
						else
							register_walk_to_object_do_nothing();
						register_get_objects_around();
					}
					else
					{
						if (can_look)
						{
							register_look_at_object();
							//register_get_objects_around();
						}
					}
						

					
				}

				old_objects_around_valid = MiscThings::is_objects_around_valid();


				bool new_had_any_quests = MiscThings::have_any_quests();
				

				if (new_had_any_quests && !old_had_any_quests)
					register_allowed_actions();


				old_had_any_quests = new_had_any_quests;

			
				if (serving_jail && !jail_serving_notified)
				{
					jail_serving_notified = true;
					send_random_context("[You are in jail. You can try to find a bed to sleep through your jail time, or try to escape (investigate surroundings)]", false);
				}


				if (serving_jail)
				{
					if (jail_reminder_time > 40.0f)
					{
						auto player_cell = player->GetParentCell();

						if (player_cell && player_cell->GetFormID() == 0x16203)
						{
							auto cidhna_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MS02");

							if (cidhna_quest && (cidhna_quest->GetCurrentStageID() >= 5 || cidhna_quest->GetCurrentStageID() < 100))
								;
							else
							{
								send_random_context("[If you dont know what to do, you can find a pickaxe and mine some ore to serve your jail time and get out of jail]", false);
								reset_poke();
							}
								
						}
						else
						{
							send_random_context("[If you dont know what to do, you can use nearest bed to \"serve your jail time\" and get out of jail (instantly)]", false);
							reset_poke();
						}
							

						jail_reminder_time = 0.0f;
					}
					else
						jail_reminder_time += 0.5f;
				}
				else
					jail_reminder_time = 0.0f;

				if (escaping_jail && !jail_escaping_notified)
				{
					if (objects_around_valid)
						register_escape_jail();

					jail_escaping_notified = true;
					send_random_context("[You are escaping from jail! Try to find exit and avoid guards... you also might want to try to get back your belongings (or might return to get them later)]", false);
				}


				if (!old_can_interact && can_interact)
				{
					register_allowed_actions();
					send_random_context("[Your hands are free]", false);

					
				}



				if (!old_can_look && can_look)
				{
					//auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
					if (threshold_quest)
					{
						int unbound_quest_stage = threshold_quest->GetCurrentStageID();
						if (unbound_quest_stage == 80)
						{
							send_random_context("[You stop near the execution site with the others. ]", false);
						}
					}
					register_allowed_actions();
				}

				if (old_can_look && !can_look)
				{
					//auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
					//if (threshold_quest)
					//{
						//int unbound_quest_stage = threshold_quest->GetCurrentStageID();
						//if (unbound_quest_stage < 160)
						//{
							unregister_all_actions();
					//	}
					//}
				}

				if (!old_can_walk && can_walk)
				{
					unregister_look_action();
					register_allowed_actions();
				}

				if (old_can_walk && !can_walk)
				{
					unregister_all_actions();
					register_look_at_object();
				}



				if (!old_can_fight && can_fight)
				{
					register_attack_action();
				}

				if (old_can_fight && !can_fight)
				{
					unregister_attack_action();
				}


				if (!old_can_interact && can_interact)
				{
					register_walk_and_interact();
				}

				if (old_can_interact && !can_interact)
				{
					unregister_walk_and_interact();
				}


				old_can_look = can_look;
				old_can_interact = can_interact;
				old_can_walk = can_walk;
				old_can_fight = can_fight;

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
							mount_name = MiscThings::insert_object_into_list_and_get_info(mount_refr);
						}

						send_random_context("[You got onto " + mount_name + "]", false);
					}
					else
					{
						std::string mount_name = "Horse";
						if (mount)
						{
							RE::TESObjectREFR* mount_refr = (RE::TESObjectREFR*)mount.get();
							mount_name = MiscThings::insert_object_into_list_and_get_info(mount_refr);
						}
						mount = nullptr;

						send_random_context("[You got off " + mount_name + "]", false);
					}
						
				}

				
				RE::TESObjectREFR* occupied_furniture = nullptr;

				if (player->currentProcess && player->currentProcess->middleHigh && player->currentProcess->middleHigh->occupiedFurniture && player->currentProcess->middleHigh->occupiedFurniture.get())
				{
					occupied_furniture = player->currentProcess->middleHigh->occupiedFurniture.get().get();
				}

				if (occupied_furniture != old_occupied_furniture)
				{
					if (occupied_furniture)
					{
						unregister_all_actions2();
					}
					else
					{
						register_allowed_actions();
					}
				}
					
				old_occupied_furniture = occupied_furniture;





				auto sit_state = player->GetSitSleepState();

				
				if (old_sit_state != sit_state)
				{
					auto furniture_handle = player_actor->GetOccupiedFurniture();

					if (furniture_handle && furniture_handle.get() && furniture_handle.get().get())
					{
						auto furniture_refr = furniture_handle.get().get();
						auto base_obj = furniture_refr->GetBaseObject();
						if (base_obj && base_obj->GetFormType() == RE::FormType::Furniture)
						{
							auto furniture = (RE::TESFurniture*)base_obj;

							std::string furniture_name = furniture->GetFullName();

							if (furniture_name != "")
							{
								if (furniture_name.find("Carriage") != std::string::npos)
								{
									if (sit_state == RE::SIT_SLEEP_STATE::kWaitingForSitAnim)
										send_random_context("[You get into carriage...]");

								}
									
								old_furniture_name = furniture_name;
							}
							//auto model = furniture->GetModel();

							//bool stop_here = false;
						}
					}

					if (sit_state == RE::SIT_SLEEP_STATE::kNormal)
					{
						if (old_furniture_name.find("Carriage") != std::string::npos)
							send_random_context("[You get out of carriage...]");


						old_furniture_name = "";
					}
						

				}


				old_sit_state = sit_state;




				if (player_dead)
				{
					if (!player_dead_sent)
					{
						unregister_all_actions();
						set_active_force(-1);

						player_dead_sent = true;
						send_random_context("[YOU DIED. The game will resume from last save soon]", false);
						MiscThings::set_time_of_death(std::chrono::steady_clock::now().time_since_epoch().count());
					}

				}
				else
				{
					int health_dif = (last_health_value - health);
					int stamina_dif = (last_stamina_value - stamina);
					int mana_dif = (last_mana_value - mana);

					bool send_info = false;

					if (abs(health_dif) > 0.1f*max_health)
					{
						last_health_value = health;
						send_info = true;
					}

					if (abs(stamina_dif) > 0.5f*max_stamina)
					{
						last_stamina_value = stamina;
						//send_info = true;
					}

					if (abs(mana_dif) > 0.5f*max_mana)
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

						float mana_percent = ((float)mana) / ((float)max_mana) * 100.0f;

						bool silent = true;
						if (MiscThings::player_hp_less_than(40.0f) || mana_percent < 10.0f)
							silent = false;

						send_random_context(message, silent);

						player_monitor_finished = true;

					}

					

					if (!tried_to_heal && MiscThings::player_hp_less_than(60.0f))
					{
						bool right_healing = MiscThings::is_self_healing_spell(true);
						bool left_healing = MiscThings::is_self_healing_spell(false);

						if (right_healing || left_healing)
						{
							if (right_healing)
								try_casting_hand(true);
							else
								try_casting_hand(false);
							
							tried_to_heal = true;
						}
					}

					if (tried_to_heal)
					{
						if (tried_to_heal_time > 15.0f)
							tried_to_heal = false;
						else
							tried_to_heal_time += dtime;
					}


					//potions/food
					bool want_health = MiscThings::player_hp_less_than(30) && WalkerProcessor::is_fighting();
					bool want_mana = (float)mana / (float)max_mana < 0.4f && WalkerProcessor::is_fighting();

					if (want_health || want_mana)
					{
						auto inventory = MiscThings::get_filtered_inventory();

						for (auto& [item, data] : inventory)
						{
							if (want_health && MiscThings::get_restore_value(item, RE::ActorValue::kHealth) > 0)
							{
								MiscThings::activate_inventory_object_by_refr(item);
							}

							if (want_mana && MiscThings::get_restore_value(item, RE::ActorValue::kMagicka) > 0)
							{
								MiscThings::activate_inventory_object_by_refr(item);
							}

						}
					}








					auto sky = RE::Sky::GetSingleton();


					std::string time_text = "";
					auto hour = sky->currentGameHour;
					if (hour < 5 || hour >= 21)
						time_text = "[Night]";
					else
						if (hour >= 5 && hour < 11)
							time_text = "[Morning]";
						else
							if (hour >= 11 && hour < 13)
								time_text = "[Noon]";
							else
								if (hour >= 13 && hour < 17)
									time_text = "[Afternoon]";
								else
									time_text = "[Evening]";

					//5-11 morning
					//11-13 noon
					//13-17 afternoon
					//17-20 evening
					//21-5 night


					if (!MiscThings::is_interior_cell() && time_text != old_time_text)
					{
						send_random_context("Time of day: " + time_text);
					}

					old_time_text = time_text;
					
					auto blackreach_worldspace = RE::TESForm::LookupByID(0x1ee62);
					auto player_worldspace = player->GetWorldspace();

					if (!MiscThings::is_interior_cell() && player_worldspace != blackreach_worldspace)
					{
						auto weather = sky->currentWeather;

						if (weather)
						{
							bool silent = true;
							std::vector<std::string> weather_vector{};

							if (weather->data.flags.any(RE::TESWeather::WeatherDataFlag::kSnow))
								weather_vector.push_back("[Snowing]");

							if (weather->data.flags.any(RE::TESWeather::WeatherDataFlag::kCloudy))
								weather_vector.push_back("[Cloudy]");

							if (weather->data.flags.any(RE::TESWeather::WeatherDataFlag::kRainy))
								weather_vector.push_back("[Raining]");

							if (weather->data.flags.any(RE::TESWeather::WeatherDataFlag::kPleasant))
								weather_vector.push_back("[Clear]");

							std::string aurora_model = "";
							aurora_model = weather->aurora.model;
							if (aurora_model != "" && (hour < 3 || hour > 19.6))
							{
								weather_vector.push_back("[Aurora Borealis]");
								silent = false;
							}
								

							std::string result_weather = "";

							for (auto weather_entry : weather_vector)
							{
								result_weather += weather_entry;
							}

							if (result_weather != "")
							{
								result_weather = "Weather: " + result_weather;
								
							}

							if (result_weather != last_weather && result_weather != "")
							{
								send_random_context(result_weather, silent);
							}


//RE::TESWeather::WeatherDataFlag
//			kNone = 0,
//			kPleasant = 1 << 0,
//			kCloudy = 1 << 1,
//			kRainy = 1 << 2,
//			kSnow = 1 << 3,
//			kPermAurora = 1 << 4,
//			kAuroraFollowsSun = 1 << 5

							last_weather = result_weather;
						}

						

					}



				}



			}
			else
				state_monitor_timer += dtime;
		}
	}



}