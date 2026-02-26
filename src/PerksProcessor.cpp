

//TODO: reasons for perk unavailability
//TODO: optimal pathfinding


//perks menu is called StatsMenu
#include "InputActions.hpp"
#include "PerksProcessor.hpp"
#include "main.hpp"

#include <iostream>

namespace PerksProcessor {



	bool in_perks = false;
	bool select_tree_request_sent = false;
	int tree_choice = 0;
	bool tree_choice_valid = false;
	bool lvlup_menu_filter = false;
	float lvlup_menu_filter_time = 0.0f;
	bool do_moving_tree = false; //this just needs to be faster
	bool do_moving_perk = false; //this just needs to be faster
	bool select_perk_request_sent = false;
	bool perk_choice_valid = false;
	int perk_choice = 0;
	bool perk_nodes_list_valid = false;
	int last_perk_index = 0;
	bool rolled_over_perks = false;
	bool confirm_request_sent = false;
	bool confirm_choice = false;
	bool confirm_choice_valid = false;
	bool perk_all_nodes_list_valid = false;
	int starting_node = -1;
	int old_perk_id = -1;
	bool cur_path_valid = false;
	int cur_path_step = 0;
	std::vector<int> cur_path{};

	bool quit_until_no_menus_left = true;



	std::vector<std::map<int, RE::BGSSkillPerkTreeNode*>> perk_all_nodes_list{};



	std::map<int, RE::BGSSkillPerkTreeNode*> perk_nodes_list{};





	bool quit_perk_menu()
	{
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();
		bool result = false;
		if (menu && !ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME))
		{
			result = true;
			RE::UIMessageQueue::GetSingleton()->AddMessage(RE::StatsMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
		}

		quit_until_no_menus_left = true;

		return result;
	}









	/*
	struct navigation_map_node {
		int id;
		int dirs[4];
	};

	struct navigation_map_tree {
		int id;
		std::vector<navigation_map_node> perk_nodes;
	};
	*/
	//std::vector<navigation_map_tree> navigation_map =



	struct tree_path_node {
		int id;
		std::map<int, std::vector<int>> perk_nodes;
	};


	//dirs are: up, right, down, left.
	//node id is perk's index
	//-1 - dir leads nowhere
	std::vector<tree_path_node> navigation_map =
	{
		//Enchanting
		{0, {{1, {-1, 2, 8, -1}}, //Storm Enchanter
		{2, {12, -1, 6, 1}}, //Soul Siphon
		{3, {7, 6, -1, -1}}, //Enchanter
		{6, {2, -1, -1, 11}}, //Soul Squeezer
		{7, {8, 11, 3, -1}}, //Fire Enchanter
		{8, {1, 10, 7, -1}}, //Frost Enchanter
		{10, {12, -1, 3, 8}}, //Corpus Enchanter
		{11, {1, 6, 3, 7}}, //Insightful Enchanter
		{12, {-1, -1, 10, 1}}} //Extra Effect
		},
		//Smithing
		{1, {{3, {13, 4, -1, 14}}, //Dwarven Smithing
		{4, {-1, 10, -1, 3}}, //Orcish Smithing
		{5, {8, 3, -1, -1}}, //Elven Smithing
		{6, {14, 10, -1, 5}}, //Steel Smithing
		{7, {-1, 10, 3, 13}}, //Daedric Smithing
		{8, {-1, 14, 5, -1}}, //Advanced Armors
		{9, {-1, 13, 6, 8}}, //Glass Smithing
		{10, {-1, -1, -1, 4}}, //Ebony Smithing
		{13, {-1, 7, 3, 9}}, //Dragon Armor
		{14, {9, 3, 6, 5}}} //Arcane Blacksmith
		},
		//Heavy Armor
		{2, {{1, {2, 5, -1, 8}}, //Juggernaut
		{2, {-1, -1, 6, 4}}, //Reflect Blows
		{3, {2, -1, 5, 8}}, //Tower of Strength
		{4, {-1, 6, 7, -1}}, //Conditioning
		{5, {3, -1, -1, 8}}, //Well Fitted
		{6, {2, -1, 5, 7}}, //Matching Set
		{7, {4, 5, 8, -1}}, //Cushioned
		{8, {4, 5, -1, -1}}} //Fists of Steel
		},
		//Block
		{3, {{1, {5, -1, 7, 2}}, //Deadly Bash
		{2, {3, 7, 6, 8}}, //Quick Reflexes
		{3, {-1, 1, 2, 4}}, //Block Runner
		{4, {-1, 1, 8, -1}}, //Elemental Protection
		{5, {-1, -1, 1, 9}}, //Disarming Bash
		{6, {2, 7, -1, 8}}, //Shield Wall
		{7, {1, -1, -1, 8}}, //Power Bash
		{8, {4, 6, -1, -1}}, //Deflect Arrows
		{9, {-1, 5, 2, 3}}} //Shield Charge
		},
		//Two-handed
		{4, {{1, {2, 6, 7, 4}}, //Champion's Stance
		{2, {3, 6, 1, 8}}, //Devastating Blow
		{3, {10, -1, 2, -1}}, //Sweep
		{4, {3, 5, 7, -1}}, //Limbsplitter
		{5, {3, 6, 7, 4}}, //Deep Wounds
		{6, {10, -1, 7, 5}}, //Skullcrusher
		{7, {1, -1, -1, -1}}, //Barbarian
		{8, {3, 2, 1, -1}}, //Great Critical Charge
		{10, {-1, -1, 3, -1}}} //Warmaster
		},
		//One-handed
		{5, {{1, {11, 6, 7, 3}}, //Fighting Stance
		{2, {9, 11, 1, -1}}, //Savage Strike
		{3, {9, 5, 7, -1}}, //Hack and Slash
		{4, {10, -1, 7, 5}}, //Bladesman
		{5, {11, 4, 7, 3}}, //Bone Breaker
		{6, {10, -1, -1, 7}}, //Dual Flurry
		{7, {1, 6, -1, -1}}, //Armsman
		{9, {-1, -1, 2, -1}}, //Paralyzing Strike
		{10, {9, -1, 6, 2}}, //Dual Savagery
		{11, {9, 10, 1, 2}}} //Critical Charge
		},
		//Archery
		{6, {{1, {9, -1, -1, 4}}, //Overdraw
		{2, {7, -1, 3, 6}}, //Ranger
		{3, {2, -1, 9, 5}}, //Hunter's Discipline
		{4, {5, 9, -1, -1}}, //Eagle Eye
		{5, {6, 9, 4, -1}}, //Power Shot
		{6, {-1, 2, 5, -1}}, //Quick Shot
		{7, {-1, -1, 3, 6}}, //Bullseye
		{8, {3, 9, 1, 4}}, //Steady Hand
		{9, {3, -1, 1, 8}}} //Critical Shot
		},
		//Light Armor
		{7, {{1, {2, -1, -1, -1}}, //Agile Defender
		{2, {3, -1, 1, -1}}, //Custom Fit
		{3, {5, -1, 2, 4}}, //Matching Set
		{4, {5, 3, 6, -1}}, //Wind Walker
		{5, {-1, -1, 6, -1}}, //Deft Movement
		{6, {4, 3, 1, -1}}} //Unhindered
		},
		//Sneak
		{8, {{1, {-1, -1, 2, 5}}, //Shadow Warrior
		{2, {5, -1, 10, 6}}, //Deadly Aim
		{3, {2, -1, -1, 7}}, //Stealth
		{4, {5, 9, 7, -1}}, //Silent Roll
		{5, {-1, 1, 9, -1}}, //Silence
		{6, {5, 9, 7, -1}}, //Light Foot
		{7, {6, 10, -1, -1}}, //Muffled Movement
		{9, {5, 2, 10, 4}}, //Assassin's Blade
		{10, {2, -1, 3, 7}}} //Backstab
		},
		//Lockpicking
		{9, {{1, {2, -1, 10, 11}}, //Apprentice Locks
		{2, {8, -1, 1, 7}}, //Adept Locks
		{3, {6, 8, 4, -1}}, //Treasure Hunter
		{4, {7, 2, 10, 11}}, //Quick Hands
		{5, {-1, 8, 7, 3}}, //Locksmith
		{6, {-1, 9, 3, -1}}, //Unbreakable
		{7, {5, 2, 4, 11}}, //Golden Touch
		{8, {9, -1, 2, 3}}, //Expert Locks
		{9, {-1, -1, 8, 6}}, //Master Locks
		{10, {8, -1, -1, -1}}, //Novice Locks
		{11, {6, 7, 10, -1}}} //Wax Key
		},
		//Pickpocket
		{10, {{1, {6, -1, 8, 3}}, //Extra Pockets
		{2, {6, 1, 5, -1}}, //Keymaster
		{3, {6, 1, 4, 5}}, //Cutpurse
		{4, {5, -1, 8, -1}}, //Night Thief
		{5, {2, 3, 4, -1}}, //Poisoned
		{6, {-1, 7, 3, -1}}, //Misdirection
		{7, {-1, -1, 1, 6}}, //Perfect Touch
		{8, {4, -1, -1, -1}}} //Light Fingers
		},
		//Speech
		{11, {{1, {4, 7, -1, -1}}, //Haggling
		{2, {5, -1, 3, 11}}, //Intimidation
		{3, {2, -1, 7, 11}}, //Persuasion
		{4, {11, 7, 1, -1}}, //Allure
		{5, {-1, -1, 3, 9}}, //Master Trader
		{7, {3, -1, -1, 4}}, //Bribery
		{9, {-1, 5, 10, -1}}, //Fence
		{10, {9, 3, 11, -1}}, //Investor
		{11, {10, 3, 4, -1}}} //Merchant
		},
		//Alchemy
		{12, {{1, {11, -1, 6, 7}}, //Snakeblood
		{2, {5, 3, -1, -1}}, //Alchemist
		{3, {4, -1, -1, 2}}, //Physician
		{4, {6, -1, 3, 5}}, //Benefactor
		{5, {8, 4, 2, -1}}, //Poisoner
		{6, {1, -1, 4, 8}}, //Experimenter
		{7, {11, 1, 8, -1}}, //Green Thumb
		{8, {7, 6, 5, -1}}, //Concentrated Poison
		{11, {-1, -1, 1, -1}}} //Purity
		},
		//Illusion
		{13, {{1, {9, -1, 8, 3}}, //Quiet Casting
		{2, {4, 13, -1, -1}}, //Illusion Dual Casting
		{3, {9, 1, 10, 6}}, //Rage
		{4, {5, 7, 2, -1}}, //Apprentice Illusion
		{5, {6, 8, 4, -1}}, //Adept Illusion
		{6, {12, 11, 5, -1}}, //Expert Illusion
		{7, {1, -1, -1, 10}}, //Animage
		{8, {1, -1, 13, 10}}, //Kindred Mage
		{9, {-1, -1, 1, 12}}, //Master of the Mind
		{10, {11, 8, 13, 4}}, //Hypnotic Gaze
		{11, {12, 1, 10, 6}}, //Aspect of Terror
		{12, {-1, 9, 11, -1}}, //Master Illusion
		{13, {10, 7, -1, 2}}} //Novice Illusion
		},
		//Conjuration
		{14, {{1, {10, 4, 15, 14}}, //Mystic Binding
		{2, {-1, 12, 8, 5}}, //Twin Souls
		{3, {6, 4, -1, -1}}, //Summoner
		{4, {9, -1, 15, 1}}, //Apprentice Conjuration
		{5, {-1, 8, 6, -1}}, //Elemental Potency
		{6, {5, 7, 3, -1}}, //Atromancy
		{7, {8, 13, 15, 6}}, //Necromancy
		{8, {2, 13, 7, 5}}, //Dark Souls
		{9, {11, -1, 4, 10}}, //Adept Conjuration
		{10, {13, 9, 1, 7}}, //Soul Stealer
		{11, {12, -1, 9, 13}}, //Expert Conjuration
		{12, {-1, -1, 11, 2}}, //Master Conjuration
		{13, {12, 11, 10, 7}}, //Oblivion Binding
		{14, {7, 1, 15, 3}}, //Conjuration Dual Casting
		{15, {1, -1, -1, 3}}} //Novice Conjuration
		},
		//Destruction
		{15, {{1, {-1, 16, 7, -1}}, //Intense Flames
		{2, {11, -1, -1, 13}}, //Destruction Dual Casting
		{3, {5, 4, 2, 7}}, //Apprentice Destruction
		{4, {11, -1, 2, 3}}, //Impact
		{5, {6, -1, 3, 1}}, //Adept Destruction
		{6, {12, -1, 5, 10}}, //Expert Destruction
		{7, {1, 16, 13, -1}}, //Augmented Flames
		{8, {9, 16, 13, 7}}, //Augmented Frost
		{9, {12, 10, 8, 1}}, //Deep Freeze
		{10, {12, 6, 16, 9}}, //Disintegrate
		{11, {6, -1, 4, 16}}, //Rune Master
		{12, {-1, -1, 6, 1}}, //Master Destruction
		{13, {16, 2, -1, -1}}, //Novice Destruction
		{16, {10, 11, 13, 8}}} //Augmented Shock
		},
		//Restoration
		{16, {{1, {-1, -1, 13, 5}}, //Avoid Death
		{2, {6, -1, -1, 4}}, //Restoration Dual Casting
		{4, {5, 2, 12, 8}}, //Apprentice Restoration
		{5, {6, 1, 4, 10}}, //Adept Restoration
		{6, {-1, 1, 5, 11}}, //Expert Restoration
		{7, {11, 13, -1, -1}}, //Respite
		{8, {10, 13, -1, 7}}, //Regeneration
		{9, {-1, 1, 10, -1}}, //Master Restoration
		{10, {9, 5, 12, 11}}, //Ward Absorb
		{11, {9, 10, 7, -1}}, //Necromage
		{12, {4, 2, -1, 7}}, //Novice Restoration
		{13, {6, -1, 2, 4}}} //Recovery
		},
		//Alteration
		{17, {{1, {-1, 11, 2, -1}}, //Atronach
		{2, {7, 9, 12, -1}}, //Mage Armor
		{3, {10, -1, 14, 9}}, // Magic Resistance
		{6, {9, -1, 14, 12}}, //Apprentice Alteration
		{7, {1, 10, 2, -1}}, //Stability
		{9, {1, 3, 6, 2}}, //Adept Alteration
		{10, {11, -1, 3, 7}}, //Expert Alteration
		{11, {-1, -1, 3, 1}}, //Master Alteration
		{12, {2, 6, 14, -1}}, //Alteration Dual Casting
		{14, {6, -1, -1, -1}}} //Novice Alteration
		}
	};



	//std::vector<tree_struct> perk_local_coords{};
	/*
	{
		//Enchanting
		//up down left right
		{1, {{1, {-1, 8, 2, -1}},  //Storm Enchanter
			 {2, {}}},

		{1, {{0, 1, 2},
			 {1, 1, 2}}},
	};
	*/

	/*

	up right down left

	-999 - unknown,
	-1 - deadend

	*/





	void perk_cancel_inputs()
	{
		perk_left_cancel();
		perk_right_cancel();
		perk_up_cancel();
		perk_down_cancel();

	}


	void visit_perk_tree(std::vector<RE::BGSSkillPerkTreeNode*>* results, RE::BGSSkillPerkTreeNode* node);

	void debug_get_coordinates()
	{
		return;

		std::vector<std::pair<std::string, std::string>> info{};

		for (auto node : perk_nodes_list)
		{
			//auto test_description = node.second->perk->GetDescription();

			info.push_back({ node.second->perk->GetFullName() ,	std::to_string(node.second->perkGridX) + " " + std::to_string(node.second->perkGridY) + "    " + std::to_string(node.second->horizontalPosition) + " " + std::to_string(node.second->verticalPosition) });
		}

		//perk_nodes_list[0]->horizontalPosition += 10.0f; //cant do this


		bool breakpoint_here = 0;



		std::vector<MenuOption> result{};
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();


		std::ofstream fs("dumpPerks.txt");


		std::vector<std::pair<std::string, std::vector<std::string>>> trees{};

		int i = 0;

		for (auto tree_id2 : ui->GetMenu<RE::StatsMenu>()->skillTrees)
		{

			int tree_id = i;

			std::pair<std::string, std::vector<std::string>> tree_result{};

			auto tree = RE::ActorValueList::GetSingleton()->GetActorValueInfo(menu->skillTrees[tree_id]);

			fs << std::to_string(tree_id) + " - " + tree->GetFullName() + "\n";

			tree_result.first = tree->GetFullName();

			std::vector<std::string> perks_result{};

			std::vector<RE::BGSSkillPerkTreeNode*> perk_nodes{};

			std::map<int, RE::BGSSkillPerkTreeNode*> perks_map{};


			for (auto perk_root : tree->perkTree->children)
				visit_perk_tree(&perk_nodes, perk_root); //fill node vector

			for (auto perk_node : perk_nodes)
			{
				if (perk_node)
				{
					perks_map.insert({ perk_node->index, perk_node }); //filter vector into map 
				}
			}

			for (auto perk_node : perks_map)
			{
				perks_result.push_back(std::to_string(perk_node.first) + " - " + perk_node.second->perk->GetFullName()); //add all results to resulting vector
				fs << "\t" + std::to_string(perk_node.first) + " - " + perk_node.second->perk->GetFullName() + "\n";
			}



			trees.push_back({ std::to_string(tree_id) + " - " + tree->GetFullName(), perks_result });

			i++;
		}
		fs.close();

		breakpoint_here = 0;

	}




	std::pair<bool, std::string> choose_perk_confirm(int choice)
	{
		std::pair<bool, std::string> result{};


		if (choice == -1)
		{
			quit_until_no_menus_left = true;
			result.first = true;
			result.second = "[Stopped skill menu]";
			return result;
		}

		if (choice != 0 && choice != 1)
		{
			result.first = false;
			result.second = "Invalid choice ID";
		}
		else
		{
			confirm_choice = choice;
			confirm_choice_valid = true;
			result.first = true;
			result.second = "[Processing...]";
		}


		return result;




	}

	bool back_to_perks();

	std::pair<bool, std::string> choose_skill_tree(int tree_id)
	{
		std::pair<bool, std::string> result{};


		if (tree_id == -1)
		{
			quit_until_no_menus_left = true;
			result.first = true;
			result.second = "[Stopped skill menu]";
			return result;
		}


		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (in_perks && menu) //TODO: return not bool but informative message on what was the error if there was one
		{
			if (tree_id >= 0 && tree_id < menu->kTotalTrees)
			{
				tree_choice = tree_id;
				tree_choice_valid = true;


				if (select_perk_request_sent)
					back_to_perks();

				result.first = true;
				result.second = "[Processing...]";
			}
			else
			{
				result.first = false;
				result.second = "Invalid tree ID";
			}
		}
		else
		{
			result.first = true;
			result.second = "Not in skill menu";
		}

		return result;
	}




	//+		[0]	"_root.StatsMenuBaseInstance.PerksLeft = 1.000000"	std::string


	std::string get_available_perk_points_text()
	{
		std::string result = "You have 0 perk points to spend";
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::StatsMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.StatsMenuBaseInstance.PerksLeft"))
						if (!var1.IsNull() && var1.IsNumber())
						{
							int perks_left = var1.GetNumber();
							if (perks_left != 0)
								result = "You have " + std::to_string(perks_left) + " perks to spend";
						}

		//"_root.StatsMenuBaseInstance.AddPerkTextInstance.AddPerkTextField.text = Perks to increase: 1"

		return result;
	}

	int get_available_perk_points_number()
	{
		int result = 0;
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::StatsMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.StatsMenuBaseInstance.PerksLeft"))
						if (!var1.IsNull() && var1.IsNumber())
							result = var1.GetNumber();

		//"_root.StatsMenuBaseInstance.AddPerkTextInstance.AddPerkTextField.text = Perks to increase: 1"

		return result;
	}


	void get_conditions(std::vector<RE::TESConditionItem*>& results, RE::TESConditionItem* item)
	{
		if (item)
		{
			results.push_back(item);
			if (item->next)
				get_conditions(results, item->next);
		}


	}

	//		[0]	0x0000025e83bcfa00	void * - functionData.params[0] of condition


	void get_reasons_unavailable(std::vector<std::string>& results, RE::BGSPerk* perk)
	{
		if (perk)
		{
			if (auto a = RE::PlayerCharacter::GetSingleton(); a)
				if (!a->HasPerk(perk))
				{
					if (!perk->perkConditions.IsTrue(a, a))
					{
						std::vector<RE::TESConditionItem*> condition_datas{};

						get_conditions(condition_datas, perk->perkConditions.head);

						for (auto condition : condition_datas)
						{
							if (condition->data.functionData.function == RE::FUNCTION_DATA::FunctionID::kHasPerk)
							{
								RE::BGSPerk* required_perk = (RE::BGSPerk*)condition->data.functionData.params[0];
								if (!a->HasPerk(required_perk))
								{
									std::string required_name = required_perk->GetFullName();
									results.push_back("Requires perk: " + required_name + ". ");
								}
							}

							if (condition->data.functionData.function == RE::FUNCTION_DATA::FunctionID::kGetBaseActorValue)
							{
								int required_value_type = (int)condition->data.functionData.params[0];
								int required_value = condition->data.comparisonValue.f;
								std::string required_value_name = RE::ActorValueList::GetSingleton()->GetActorValueName((RE::ActorValue)required_value_type);
								int current_value = a->GetBaseActorValue((RE::ActorValue)required_value_type);
								if (current_value < required_value)
									results.push_back(" Not enough skill level: " + required_value_name + " (" + std::to_string(current_value) + "/" + std::to_string(required_value) + "). ");

							}

						}
					}
				}
				else //we have this perk. check subperks
					if (perk->nextPerk)
						return get_reasons_unavailable(results, perk->nextPerk);
		}

	}


	int check_subperks_available(RE::BGSPerk* perk)
	{
		if (perk)
		{
			if (auto a = RE::PlayerCharacter::GetSingleton(); a)
				if (!a->HasPerk(perk))
					if (perk->perkConditions.IsTrue(a, a))
						return 1; //found available subperk
					else
					{
						return -1; //found subperk but its unavailable.
					}
				else
					if (perk->nextPerk)
						return check_subperks_available(perk->nextPerk); //check deeper
					else
						return -2; //we have all subperks
		}

		return 0; //this perk does not exist. shouldnt get here
	}



	int is_perk_available(int perk_id)
	{
		int result = 0;

		if (perk_nodes_list_valid)
		{
			auto node = perk_nodes_list.find(perk_id);

			if (node != perk_nodes_list.end())
			{
				if (node->second && node->second->perk)
				{
					result = check_subperks_available(node->second->perk);

				}
			}
		}

		return result;
	}



	//+		[1]	"_root.StatsMenuBaseInstance.DescriptionCardInstance.CardDescriptionTextInstance.text = Bows do 20% more damage."	std::string

	std::string get_perk_description()
	{
		std::string result = "";
		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();
		if (ui)
			if (const auto menu = ui->GetMenu<RE::StatsMenu>(); menu)
				if (menu->uiMovie)
					if (menu->uiMovie->GetVariable(&var1, "_root.StatsMenuBaseInstance.DescriptionCardInstance.CardDescriptionTextInstance.text"))
						if (!var1.IsNull() && var1.IsString())
						{
							result = var1.GetString();
						}
		return result;
	}


	std::string get_unavailable_perk_description() //a bit extended version with reasons why it is unavailable
	{
		std::string result = get_perk_description();



		RE::GFxValue var1;
		RE::UI* ui = RE::UI::GetSingleton();

		if (ui && result != "")
			if (const auto menu = ui->GetMenu<RE::StatsMenu>(); menu)
			{
				int current_perk = 0;

				if (perk_choice_valid && perk_nodes_list_valid)
				{
					if (menu && menu->unk1C8)
					{
						int cur_id = menu->unk1C8->unk48 & 255;

						auto perk_node = perk_nodes_list.find(cur_id);

						if (perk_node != perk_nodes_list.end() && perk_node->second && perk_node->second->perk)
						{
							std::string perk_name = perk_node->second->perk->GetFullName();
							result = "Perk: " + perk_name + ". ";

							int availability = is_perk_available(cur_id);

							if (availability == 1)
								result += " Perk is available. This message wasnt supposed to be sent.";
							else
							{
								bool got_reason = false;

								result += "Unavailable. Reasons: ";

								if (availability == -2)
								{
									got_reason = true;
									result += "Already learned this perk. ";
								}
								else
								{
									if (availability == -1)
									{
										std::vector<std::string> other_reasons{};
										get_reasons_unavailable(other_reasons, perk_node->second->perk);

										for (std::string reason : other_reasons)
											result += reason;

										if (std::size(other_reasons) != 0)
											got_reason = true;

									}
								}


								if (get_available_perk_points_number() < 1)
									result += "You have no perk points to spend. ";


								if (!got_reason)
									result += "Unknown reason. ";

							}
						}

					}
				}
			}


		return result;
	}


	std::string get_perk_name2(int tree_id, int perk_id)
	{
		std::string result = "";

		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::StatsMenu>(); menu)
			{
				if (perk_all_nodes_list_valid)
				{
					auto perk_node = perk_all_nodes_list[tree_id].find(perk_id);

					if (perk_node != perk_all_nodes_list[tree_id].end() && perk_node->second && perk_node->second->perk)
					{
						result = perk_node->second->perk->GetFullName();
					}
				}
			}

		return result;
	}

	std::string get_perk_name(int perk_id)
	{
		std::string result = "";

		RE::UI* ui = RE::UI::GetSingleton();

		if (ui)
			if (const auto menu = ui->GetMenu<RE::StatsMenu>(); menu)
			{
				if (perk_nodes_list_valid)
				{
					auto perk_node = perk_nodes_list.find(perk_id);

					if (perk_node != perk_nodes_list.end() && perk_node->second && perk_node->second->perk)
					{
						result = perk_node->second->perk->GetFullName();
					}
				}
			}

		return result;
	}



	int get_selected_tree()
	{
		int result = -1;


		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu)
			result = menu->selectedTree;

		return result;
	}


	void move_cursor_to_tree(int choice_id)
	{
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu)
		{
			int selected_tree = get_selected_tree();
			int total_trees = menu->kTotalTrees;

			int right_distance = (total_trees - selected_tree + choice_id) % total_trees;
			int left_distance = (total_trees - choice_id + selected_tree) % total_trees;


			if (right_distance > left_distance)
			{
				perk_left();
			}
			else
			{
				perk_right();
			}

		}
	}


	int get_current_perk_id()
	{
		int result = -1;

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu && menu->unk1C8)
		{
			result = menu->unk1C8->unk48 & 255;
		}

		return result;
	}






	bool is_inside_of_tree()
	{
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu->menuState == RE::StatsMenu::MenuState::kZoomed)
			return true;


		return false;
	}





	void visit_perk_tree(std::vector<RE::BGSSkillPerkTreeNode*>* results, RE::BGSSkillPerkTreeNode* node)
	{
		if (node && node->perk)
		{
			results->push_back(node);
			for (auto child : node->children)
				visit_perk_tree(results, child);
		}
	}


	void fill_all_perk_list()
	{
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu)
		{
			for (int i = 0; i < 18; i++)
			{
				perk_all_nodes_list.push_back({});

				if (menu && i >= 0 && i < menu->kTotalTrees)
				{
					auto tree = RE::ActorValueList::GetSingleton()->GetActorValueInfo(menu->skillTrees[i]);

					std::vector<RE::BGSSkillPerkTreeNode*> perk_nodes{};

					for (auto perk_root : tree->perkTree->children)
					{
						visit_perk_tree(&perk_nodes, perk_root);
					}

					for (auto perk_node : perk_nodes)
					{
						if (perk_node)
						{
							perk_all_nodes_list[i].insert({ perk_node->index, perk_node });
						}

					}

				}
			}

			perk_all_nodes_list_valid = true;
		}


	}



	void fill_perk_list(int tree_id)
	{
		std::vector<MenuOption> result{};
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();


		if (menu && tree_id >= 0 && tree_id < menu->kTotalTrees)
		{
			auto tree = RE::ActorValueList::GetSingleton()->GetActorValueInfo(menu->skillTrees[tree_id]);

			std::vector<RE::BGSSkillPerkTreeNode*> perk_nodes{};

			for (auto perk_root : tree->perkTree->children)
			{
				visit_perk_tree(&perk_nodes, perk_root);
			}

			for (auto perk_node : perk_nodes)
			{
				if (perk_node)
				{
					perk_nodes_list.insert({ perk_node->index, perk_node });
				}

			}

			perk_nodes_list_valid = true;
		}


	}



	std::vector<MenuOption> get_all_perk_options()
	{
		std::vector<MenuOption> result{};
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu && perk_nodes_list_valid)
		{
			int i = 0;
			for (auto node : perk_nodes_list)
			{
				if (node.second && node.second->perk)
				{
					MenuOption option{};
					option.id = node.first;
					option.text = node.second->perk->GetFullName();
					result.push_back(option);
				}
			}
		}

		result.push_back({ -1, "[QUIT PERK MENU]" });


		return result;
	}


	std::string get_all_perk_options_string()
	{
		std::string result = "";

		std::vector<MenuOption> options{};
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu && perk_nodes_list_valid)
		{
			int i = 0;
			for (auto node : perk_nodes_list)
			{
				if (node.second && node.second->perk)
				{
					MenuOption option{};
					option.id = node.first;
					option.text = node.second->perk->GetFullName();

					options.push_back(option);
				}
			}

			result = get_all_perk_options_string();

		}

		return result;
	}



	std::vector<MenuOption> get_available_perk_options()
	{
		//if (get_available_perk_points_number() < 1)
		//	return get_all_perk_options();


		std::vector<MenuOption> result{};
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu && perk_nodes_list_valid)
		{
			int i = 0;
			for (auto node : perk_nodes_list)
			{
				if (node.second && node.second->perk)
				{

					int availability = is_perk_available(node.first);

					if (availability == 1)
					{
						MenuOption option{};
						option.id = node.first;
						option.text = node.second->perk->GetFullName();
						result.push_back(option);
					}


					/*
					if (auto a = RE::PlayerCharacter::GetSingleton(); a)
						if (node.second->perk->perkConditions.IsTrue(a, a))
						{
							if (!a->HasPerk(node.second->perk)) //this check is not correct for perks that have several tiers..
							{
								MenuOption option{};
								option.id = node.first;
								option.text = node.second->perk->GetFullName();
								result.push_back(option);
							}
							else
							{
								int count_entries = 0;
								for (auto perk_entry : node.second->perk->perkEntries)
								{
									auto test = perk_entry;

									count_entries++;
								}

								bool test_breakpoint1 = 0;
							}
						}
					*/

				}
			}
		}
		return result;
	}


	std::vector<MenuOption> get_tree_options()
	{
		std::vector<MenuOption> result{};

		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu)
			for (int i = 0; i < menu->kTotalTrees; i++) //this isnt right but will work for now
			{
				MenuOption tree_result;

				tree_result.id = i;

				bool skip_this_one = false;

				auto tree = RE::ActorValueList::GetSingleton()->GetActorValueInfo(menu->skillTrees[i]);
				if (tree)
				{
					if (auto a = RE::PlayerCharacter::GetSingleton(); a)
					{
						auto actor_val_type = RE::ActorValueList::GetSingleton()->LookupActorValueByName(tree->enumName);
						std::string tree_name = tree->GetFullName();

						tree_result.text = tree_name + " (skill level: " + std::to_string((int)a->GetBaseActorValue(actor_val_type)) + ")"; //not sure if this is the best way but whatever
						result.push_back(tree_result);
					}
				}

			}

		return result;
	}


	std::string get_tree_name(int tree_id)
	{
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();

		if (menu && tree_id >= 0 && tree_id < menu->kTotalTrees)
		{
			auto tree = RE::ActorValueList::GetSingleton()->GetActorValueInfo(menu->skillTrees[tree_id]);
			if (tree)
				return tree->GetFullName();
		}

		return "";
	}


	/*

	void build_perk_tree()
	{
		trees.push_back(
			{
				0,
				"tree0",
					{
						{0, "perk0", {1,2,3}, {3,2,1}},
						{1, "perk1", {1,2,3}, {3,2,1}},
						{1, "perk2", {1,2,3}, {3,2,1}}
					}
			}
		);

		trees.push_back(
			{
				1,
				"tree1",
					{
							{0, "perk0", {1,2,3}, {3,2,1}},
							{1, "perk1", {1,2,3}, {3,2,1}},
							{2, "perk2", {1,2,3}, {3,2,1}},
					}
			}
		);

	}

	*/

	std::pair<bool, std::string> choose_perk(int perk_id)
	{
		RE::UI* ui = RE::UI::GetSingleton();
		auto menu = ui->GetMenu<RE::StatsMenu>();
		std::pair<bool, std::string> result{};


		if (perk_id == -1)
		{
			quit_until_no_menus_left = true;
			result.first = true;
			result.second = "[Stopped skill menu]";
			return result;
		}



		if (perk_id == -2)
		{
			if (force_choice(get_all_perk_options(), "You are in perk menu, " + get_tree_name(tree_choice) + " perk tree. You have " + get_available_perk_points_text() + " perk points. Choose perk to investigate. ", force_type::perk_perk))
			{
				result.first = true;
				result.second = "[Processing...]";
			}
			else
			{
				result.first = false;
				result.second = "Can not do that now";
			}

		}
		else
		{
			if (in_perks && menu) //TODO: return not bool but informative message on what was the error if there was one
			{
				if (tree_choice_valid)
					for (auto node : perk_nodes_list)
					{
						if (node.second && node.second->index == perk_id)
						{
							perk_choice = perk_id;
							perk_choice_valid = true;
							result.first = true;
							result.second = "[Processing...]";
							break;
						}
					}
				else
				{
					result.first = true;
					result.second = "Skill tree undefined";
				}
			}
			else
			{
				result.first = true;
				result.second = "Not in skill menu";
			}
		}


		return result;
	}




	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////// DIRECTIONS BOT //////////////////////////////////////////////////////////////////////////////////



	void dump_one_file(std::ofstream& of, int tree_id)
	{

		auto test_path_tree = &navigation_map[tree_id];

		of << "//" + get_tree_name(tree_id) + "\n";

		of << "{" + std::to_string(test_path_tree->id) + ", {";

		int last_perk_id = -1;
		int n = 0;
		for (auto perk_node : test_path_tree->perk_nodes)
		{
			n++;
			last_perk_id = perk_node.first;
			of << "{" + std::to_string(perk_node.first) + ", {";

			for (int i = 0; i < 4; i++)
			{
				of << std::to_string(perk_node.second[i]) + ", ";
			}

			long pos = of.tellp();
			of.seekp(pos - 2);

			std::string add_name = "";

			if (n < std::size(test_path_tree->perk_nodes))
				add_name = " //" + get_perk_name2(tree_id, perk_node.first);

			of << "}}," + add_name + "\n";
		}

		long pos = of.tellp();
		of.seekp(pos - 3);

		of << "} //" + get_perk_name2(tree_id, last_perk_id) + "\n},\n";

	}



	bool scanning = false;
	bool tree_was_set_up = false;




	void visit_perk_tree_scan_setup(int tree_id, RE::BGSSkillPerkTreeNode* node)
	{

		auto test_path_tree = &navigation_map[tree_id];

		if (node && node->perk)
		{
			std::vector<int> empty_dirs = { -999, -999, -999, -999 };
			test_path_tree->perk_nodes.insert({ node->index , empty_dirs });

			for (auto child : node->children)
				visit_perk_tree_scan_setup(tree_id, child);
		}
	}


	void setup_tree()
	{
		for (int i = 0; i < 18; i++)
		{
			navigation_map.push_back({});

			auto test_path_tree = &navigation_map[i];

			test_path_tree->id = i;

			RE::UI* ui = RE::UI::GetSingleton();
			auto menu = ui->GetMenu<RE::StatsMenu>();

			auto tree = RE::ActorValueList::GetSingleton()->GetActorValueInfo(menu->skillTrees[test_path_tree->id]);

			for (auto perk_node : tree->perkTree->children)
			{
				visit_perk_tree_scan_setup(i, perk_node);
				//std::vector<int> empty_dirs = { -999, -999, -999, -999 };
				//test_path_tree.perk_nodes.insert({ perk_node->index , empty_dirs });

			}
		}

		tree_was_set_up = true;
	}




	//up right down left

	std::vector<int> get_path(int tree_id, int skip_dir, int from, int to, std::vector<int> last_nodes, std::vector<int>path)
	{
		std::vector<int> empty{};

		if (from == to)
			return path; //got to the target

		for (int last_node : last_nodes)
			if (last_node == from)
			{

				return empty; //circled
			}

		for (int dir = 0; dir < 4; dir++)
		{
			if (dir != skip_dir)
			{
				auto test_path_tree = &navigation_map[tree_id];

				auto dirs_from = test_path_tree->perk_nodes.find(from);

				if (dirs_from != test_path_tree->perk_nodes.end())
				{
					int new_from = dirs_from->second[dir];

					if (new_from != -999 && new_from != -1)
					{
						std::vector<int> new_last_nodes = last_nodes;
						new_last_nodes.push_back(from);
						std::vector<int> new_path = path;
						new_path.push_back(dir);
						//(dir + 2) % 4
						std::vector<int> result = get_path(tree_id, -1, new_from, to, new_last_nodes, new_path);
						if (std::size(result) != 0)
							return result;
					}
				}

			}
		}
		return empty;
	}




	bool move_to_node(int tree_id, int from, int to)
	{
		if (!cur_path_valid)
		{
			std::vector<int> empty{};
			cur_path = get_path(tree_id, -1, from, to, empty, empty);
			if (std::size(cur_path) == 0)
				return true; //signal that we finished (because couldnt find the path). supposed to restart the algorithm

			cur_path_valid = true;

			cur_path_step = 0;
			old_perk_id = -1;
			return false;
		}
		else
		{
			if (old_perk_id == -1)
				old_perk_id = get_current_perk_id();

			if (cur_path_step < std::size(cur_path))
			{
				if (get_current_perk_id() != old_perk_id)
				{
					old_perk_id = get_current_perk_id();
					perk_cancel_inputs();
					set_universal_block(1.0f);
					cur_path_step++;
				}
				else
				{
					if (cur_path[cur_path_step] == 0)
						perk_up();
					if (cur_path[cur_path_step] == 1)
						perk_right();
					if (cur_path[cur_path_step] == 2)
						perk_down();
					if (cur_path[cur_path_step] == 3)
						perk_left();
				}
				return false;
			}
			else
			{
				return true;
			}
		}
	}



	bool have_node_to_scan = false;
	int node_to_scan = -1;

	int get_node_to_scan(int tree_id)
	{

		auto test_path_tree = &navigation_map[tree_id];

		auto current_node = test_path_tree->perk_nodes.find(get_current_perk_id());

		for (int i = 0; i < 4; i++)
			if (current_node->second[i] == -999)
				return current_node->first; //current node is unfinished

		for (auto perk_node : test_path_tree->perk_nodes)
		{
			if (perk_node.first != current_node->first) //its not current node
			{
				std::vector<int> empty{};
				std::vector<int> try_path = get_path(tree_id, -1, current_node->first, perk_node.first, empty, empty);
				if (std::size(try_path) != 0) //we have path
				{
					for (int i = 0; i < 4; i++)
						if (perk_node.second[i] == -999)
							return perk_node.first; //we can reach this node and it has unknown directions
				}
			}
		}

		return -1;
	}


	int last_perk_tree = -1;
	int last_perk_id = -1;
	float probe_timer = 0.0f;


	int get_dir_probe_result(float dtime)
	{
		if (last_perk_tree != -1 && last_perk_id != -1)
		{
			RE::UI* ui = RE::UI::GetSingleton();
			auto menu = ui->GetMenu<RE::StatsMenu>();

			int current_perk_id = get_current_perk_id();
			int current_perk_tree = get_selected_tree();

			if (current_perk_tree != last_perk_tree)
			{
				//if (current_perk_id == 6)
				//	bool dbg_breakpoint1 = 0;
				probe_timer = 0.0f;
				return -1;
			}


			if (current_perk_id != last_perk_id)
			{
				if (last_perk_id == 6)
					bool dbg_breakpoint1 = 0;
				probe_timer = 0.0f;
				return current_perk_id;
			}


			if (!is_inside_of_tree())
			{
				//if (current_perk_id == 6)
				//	bool dbg_breakpoint1 = 0;
				scanning = false; //so it returns back to the tree
				probe_timer = 0.0f;
				return -1;
			}


			if (probe_timer > 3.0f)
			{
				if (current_perk_id == 6)
					bool dbg_breakpoint1 = 0;
				probe_timer = 0.0f;
				return -1;
			}


			probe_timer += dtime;
		}
		else
			probe_timer = 0.0f;



		return -999;
	}


	int dir_to_scan = -1;

	int get_dir_to_scan(int tree_id, int node_id)
	{
		auto test_path_tree = &navigation_map[tree_id];

		auto current_node = test_path_tree->perk_nodes.find(node_id);

		for (int i = 0; i < 4; i++)
		{
			if (current_node->second[i] == -999)
			{
				return i;
			}
		}

		return -1;
	}


	void move_to_dir(int dir)
	{
		if (dir == 0)
			perk_up();
		if (dir == 1)
			perk_right();
		if (dir == 2)
			perk_down();
		if (dir == 3)
			perk_left();
	}


	int from_node = -1;


	void reset_scan()
	{
		scanning = false;
		from_node = -1;
		dir_to_scan = -1;
		last_perk_tree = -1;
		last_perk_id = -1;
		probe_timer = 0.0f;
		have_node_to_scan = false;
		node_to_scan = -1;
		old_perk_id = -1;
		cur_path_valid = false;
		cur_path_step = 0;
		//perk_cancel_inputs();
	}

	bool scan_tree(int tree_id, float dtime)
	{
		if (!have_node_to_scan)
		{
			node_to_scan = get_node_to_scan(tree_id);
			from_node = get_current_perk_id();

			if (node_to_scan == -1)
				return true; //all done
			else
				have_node_to_scan = true;
		}
		else
		{
			if (int result = get_dir_probe_result(dtime); result != -999)
			{
				auto test_path_tree = &navigation_map[tree_id];

				auto scanned_node = test_path_tree->perk_nodes.find(node_to_scan);
				scanned_node->second[dir_to_scan] = result;
				perk_cancel_inputs();
				reset_scan();
				set_universal_block(0.5f);
			}
			else
			{
				if (get_current_perk_id() != node_to_scan)
				{
					bool result1 = move_to_node(tree_id, from_node, node_to_scan);
				}
				else
				{
					old_perk_id = -1;
					cur_path_valid = false;

					if (dir_to_scan == -1)
						dir_to_scan = get_dir_to_scan(tree_id, get_current_perk_id());
					else
					{

						if (last_perk_id == -1 || last_perk_tree == -1)
						{
							if (get_current_perk_id() == 6)
								bool dbg_breakpoint_1 = false;
							perk_cancel_inputs();

							last_perk_id = get_current_perk_id();
							last_perk_tree = get_selected_tree();
						}
						else
						{
							move_to_dir(dir_to_scan);
						}
					}
				}
			}
		}


		return false;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




	bool reset_perks()
	{
		in_perks = false;
		select_tree_request_sent = false;
		tree_choice = 0;
		tree_choice_valid = false;
		lvlup_menu_filter = false;
		lvlup_menu_filter_time = 0.0f;
		do_moving_tree = false;

		do_moving_perk = false;
		select_perk_request_sent = false;
		perk_choice_valid = false;
		perk_choice = 0;
		perk_nodes_list_valid = false;
		perk_nodes_list.clear();
		last_perk_index = 0;
		rolled_over_perks = false;
		confirm_request_sent = false;
		confirm_choice = false;
		confirm_choice_valid = false;
		perk_all_nodes_list_valid = false;
		perk_all_nodes_list.clear();
		reset_scan();
		starting_node = -1;
		old_perk_id = -1;
		cur_path_valid = false;
		cur_path_step = 0;
		cur_path.clear();
		quit_until_no_menus_left = false;


		return true;
	}


	bool back_to_perks()
	{
		do_moving_perk = false;
		select_perk_request_sent = false;
		perk_choice_valid = false;
		perk_choice = 0;
		perk_nodes_list_valid = false;
		perk_nodes_list.clear();
		last_perk_index = 0;
		rolled_over_perks = false;
		confirm_request_sent = false;
		confirm_choice = false;
		confirm_choice_valid = false;
		starting_node = -1;
		old_perk_id = -1;
		cur_path_valid = false;
		cur_path_step = 0;
		cur_path.clear();

		return true;
	}




	bool scan_mode = false;
	bool release_mode = true;
	float perks_processor_timer = 0.0f;

	int tree_to_scan = -1;


	void processor(float dtime)
	{

		RE::UI* ui = RE::UI::GetSingleton();
		in_perks = ui->IsMenuOpen(RE::StatsMenu::MENU_NAME) && !ui->IsMenuOpen(RE::LevelUpMenu::MENU_NAME); //separate statsmenu and lvlupmenu
		auto menu = ui->GetMenu<RE::StatsMenu>();



		if (!perk_all_nodes_list_valid)
		{
			fill_all_perk_list();
		}


		if (quit_until_no_menus_left)
		{
			if (!ui->IsMenuOpen(RE::StatsMenu::MENU_NAME) && !ui->IsMenuOpen(RE::TweenMenu::MENU_NAME) && !ui->IsMenuOpen(RE::LevelUpMenu::MENU_NAME))
				reset_perks();
			else
			{
				cancel();
				set_universal_block(2.5f);
			}

			return;
		}


		if (in_perks && menu && (menu->menuState == RE::StatsMenu::MenuState::kNormal || menu->menuState == RE::StatsMenu::MenuState::kZoomed))
		{
			if (lvlup_menu_filter_time > 1.0f)
			{
				lvlup_menu_filter_time = 0.0f;
				lvlup_menu_filter = true;
			}
			else
				lvlup_menu_filter_time += dtime;
		}
		else
			lvlup_menu_filter_time = 0.0f;



		if (do_moving_tree)
			move_cursor_to_tree(tree_choice);

		if (do_moving_perk)
		{
			if (move_to_node(tree_choice, starting_node, perk_choice))
			{
				starting_node = -1; //this will restart pathfinding if mover is finished but we are not in target node (in case something went wrong)
				do_moving_perk = false;
			}

		}

		//move_cursor_to_perk(perk_choice);

		if (scanning)
		{
			if (!tree_was_set_up)
			{
				setup_tree();
			}
			else
			{
				if (scan_tree(tree_to_scan, dtime))
				{
					std::ofstream of("TREE_MAP_ " + std::to_string(tree_to_scan) + ".txt");
					dump_one_file(of, tree_to_scan);
					of.close();

					//get_node_to_scan(tree_to_scan);//test

					scanning = false;
					perk_nodes_list_valid = false;
					perk_nodes_list.clear();

					tree_to_scan++;
					tree_choice++;

					if (tree_to_scan >= 18)
					{
						std::ofstream of("TREE_MAP_ALL.txt");
						for (int i = 0; i < 18; i++)
							dump_one_file(of, i);
						of.close();

						scan_mode = false; //full finish

						reset_perks();
					}

				}

			}
		}

		if (scan_mode || perks_processor_timer > 0.05f)
		{
			perks_processor_timer = 0.0f;


			if (in_perks) //TODO: filter initial fade in with potential lvlup menu blocking this menu. probably need timers too
			{
				auto skill_trees = menu->skillTrees;

				if (lvlup_menu_filter)
				{
					if (!select_tree_request_sent)
					{
						
						if (force_choice(get_tree_options(), "You are in perk learning menu. You have " + get_available_perk_points_text() + " perk points. Choose skill tree. ", force_type::perk_tree))
							select_tree_request_sent = true;
					}
					else
					{
						if (tree_choice_valid)
						{
							if (get_selected_tree() != tree_choice)
							{
								if (is_inside_of_tree())
								{
									set_universal_block(3.0f);
									perk_cancel_inputs();
									cursor_down(); //why does this stop categories from being drawn? it works though
									menu->horizontalVelocity = 0.0f;
								}
								else
									do_moving_tree = true;
							}
							else
							{
								if (do_moving_tree)
								{
									set_universal_block(2.0f);
									do_moving_tree = false;
									menu->horizontalVelocity = 0.0f;
								}

								if (!is_inside_of_tree())
								{
									set_universal_block(1.0f);
									cursor_up();//leftclick();
								}
								else
								{
									if (scan_mode)
									{
										if (!perk_nodes_list_valid)
											fill_perk_list(tree_choice);


										if (tree_to_scan == -1)
											tree_to_scan = tree_choice;

										scanning = true;
									}


									if (release_mode)
									{
										if (!perk_nodes_list_valid)
										{
											fill_perk_list(tree_choice);
										}
										else
										{
											debug_get_coordinates();

											if (!select_perk_request_sent)
											{
												
												//force_perks_select_perk(get_available_perk_options(), get_tree_name(tree_choice));
												if (force_choice(get_available_perk_options(), "You are in perk menu, " + get_tree_name(tree_choice) + " perk tree. You have " + get_available_perk_points_text() + " perk points. Choose perk to learn. ", force_type::perk_perk))
													select_perk_request_sent = true;
											}
											else
											{
												if (perk_choice_valid)
												{
													if (menu->unk1C8)
													{
														if ((menu->unk1C8->unk48 & 255) != perk_choice)
														{
															{
																if (starting_node == -1)
																	starting_node = menu->unk1C8->unk48 & 255;

																do_moving_perk = true;
															}

														}
														else
														{
															if (do_moving_perk)
															{
																starting_node = -1;
																do_moving_perk = false;
															}


															if (is_perk_available(perk_choice) == 1)
															{
																if (auto menu_confirm = ui->GetMenu<RE::MessageBoxMenu>(); menu_confirm)
																{
																	RandomMessageBoxProcessor::set_messagebox_handeled();

																	if (!confirm_request_sent)
																	{
																		
																		if (force_choice({ {0, "No"},{1, "Yes"},{-1, "[QUIT PERK MENU]"} }, "You are in perk menu. You are about to learn " + get_perk_name(get_current_perk_id()) + ": " + get_perk_description() + ". Confirm?", force_type::perk_confirm))
																			confirm_request_sent = true;
																	}
																	else
																	{
																		if (confirm_choice_valid)
																		{
																			if (confirm_choice) //agree
																			{
																				if (!rolled_over_perks)
																				{
																					menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onRollOver", nullptr, nullptr, 0);
																					rolled_over_perks = true;
																					set_universal_block(0.5f);
																				}
																				else
																				{
																					menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button0.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
																					set_universal_block(0.5f);
																					reset_perks();
																				}
																			}
																			else //deny
																			{
																				if (!rolled_over_perks)
																				{
																					menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button1.onRollOver", nullptr, nullptr, 0);
																					rolled_over_perks = true;
																					set_universal_block(0.5f);
																				}
																				else
																				{
																					menu_confirm->uiMovie->Invoke("_root.MessageMenu.Buttons.Button1.onPress", nullptr, nullptr, 0); //this seems to have immidiate 100% result so do everything here, next cycle we are not getting in this menu at all
																					set_universal_block(0.5f);
																					reset_perks();
																				}
																			}
																		}
																	}


																}
																else
																	confirm();
															}
															else
															{
																send_random_context(get_unavailable_perk_description());
																set_universal_block(3.0f);
																reset_perks();
															}

														}
													}
												}
											}
										}
									}


								}
							}
						}
					}
				}
			}
			else
				reset_perks();
		}
		else
			perks_processor_timer += dtime;

	}



}
