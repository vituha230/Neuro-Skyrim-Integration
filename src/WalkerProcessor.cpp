



#include "InputActions.hpp"
#include "WalkerProcessor.hpp"
#include "main.hpp"
#include "Misc.hpp"
#include <numbers>
#include "CustomWalkerPaths.hpp"
#include "Observer.hpp"



namespace WalkerProcessor {

    int alftand_counter = 0;
    bool dont_check_quest_target_change = false;

    RE::TESObjectREFR* activate_refr_after_walker_is_done = nullptr;
    bool always_shift = false;
    bool karthspire_plates = false;

    float crouch_timeout = 0.0f;

    int stable_target = 0;

    bool navmesh_probe_mode = false;
    bool navmesh_probe_result_valid = false;
    bool navmesh_probe_result = false;

    bool silly_walk_mode = false;


    float just_teleported_timeout = 0.0f;


    bool multiple_paths_quest_choice_confirming = false;
    bool multiple_paths_quest_choice_confirmed = false;
    bool multiple_paths_quest_request_sent = false;
    bool multiple_paths_quest_choice_valid = false;
    int multiple_paths_quest_choice = -1;
    std::vector<int> allowed_multiple_paths_quest_choices{};


    bool stop_sneaking = false;

    float path_point_reached_timeout = 0.0f;


    float pickpocket_timeout = 0.0f;

    bool no_weapons_notified = false;


    int last_walker_operation = -1; //for location switch. DO NOT RESET IN NORMAL RESET


    RE::TESQuestTarget* current_quest_target_followed = nullptr;
    RE::TESQuest* current_quest_followed = nullptr;


    bool just_teleported = false;




    RE::TESObjectREFR* special_ref_for_distance_calculation = nullptr;


    bool confirm_stealing = false;
    bool stealing_request_sent = false;
    bool stealing_choice_valid = false;
    bool stealing_choice = false;
    bool stealing_confirmed = false;
    
    bool pause_pre_stealing = false;
    float pause_pre_stealing_time = 0.0f;

    int opening_door_attempts = 0;

    float stealing_timer = 0.0f;

    bool correct_word_of_power = false;

    int advice_counter = 0;

    float active_attacking_time = 0.0f;
    float backup_interaction_time = 0.0f;

    RE::NiPoint3 last_paths_best_guess = RE::NiPoint3::Zero();


    float dead_point_time = 0.0f;

    bool dont_use_bounds_for_close_enough = false;
    bool getting_into_carriage = false;
    float getting_into_carriage_time = 0.0f;

    float walker_inactive_time = 0.0f;


    //bool reset_by_explorer = false;

    bool runaway_mode = false;

    bool crime_mode = false;

    bool gave_attacking_info = false;

    int current_path_point = -1;
    bool path_valid = false;
    std::vector<RE::NiPoint3> path{};

    std::vector<RE::NiPoint3> custom_path{};

    bool have_target_to_walk = false;
    RE::TESObjectREFR* target_ref = nullptr;

    std::string reminder_target_name = "";
    float reminder_walk_time = 0.0f;
    float reminder_distance = 0.0f;
    RE::NiPoint3 reminder_start_pos{};


    int interaction_after_walk = -1;
   
    bool walk_finished_context_sent = false;
    bool interaction_context_sent = false;

    bool try_to_remove_obstacle_mode = false;

    float flick_time = 0.0f;
    bool flicking = false;

    float have_door_targeted_time = 0.0f;
    RE::TESObjectREFR* last_targeted_ref = nullptr;

    RE::TESObjectREFR* last_blocking_targeted_ref = nullptr;
    float have_blocking_targeted_time = 0.0f;


    bool turning_around = false;

    bool walk_forward_a_little = false;
    bool walk_backward_a_little = false;
    float walked_for = 0.0f;


    bool door_is_closed_request_sent = false;
    bool door_is_closed_choice_valid = false;
    int door_is_closed_choice = -1;


    float last_point_posZ = 0.0f;

    float wiggle_camera_time = 0.0f;

    bool confirming_closed_door_interaction = false;

    bool quest_mode = false;

    //RE::GuideEffect* saved_guide_effect;


    RE::ExtraDataList* data_to_copy{};

    bool had_successful_walk = false;

    //bool move_avoid_box = false;

    bool was_slowwalking = false;
    bool locking_failed = false;

    bool catch_door_result = false;
    float catch_door_result_time = 0.0f;

    int walk_retries = 0;

    float ignore_closed_doors_time = 0.0f;


    bool wiggle_body_then_walk_again = false;

    int attempts_to_move_obstacle = 0;
    bool move_obstacle_failed = false;

    bool try_unstuck = false;
    float time_stuck = 0.0f;
    float walk_unstuck_time = 0.0f;
    int unstuck_attempts = 0;

    float attack_spell_cast_timeout = 0.0f;

    bool paused_before_interaction = false;

    bool make_clairvoyance_cast = false;

    bool tried_to_come_closer = false;

    bool wait_and_start_pickpocket = false;

    float waiting_pickpocket_time = 0.0f;

    RE::TESQuest* last_quest;
    RE::BGSQuestObjective* last_quest_objective;

    bool using_custom_path = false;

    bool start_attacking = false;
    float attacking_inanimate_object_time = 0.0f;
    bool attacking_done = false;

    bool ignore_fights_mode = false;

    bool was_sprinting = false;


    bool low_mana_detected = false;

    bool tried_to_draw_weapon1 = false;
    float draw_weapon_check_time1 = 0.0f;
    bool tried_to_draw_weapon2 = false;
    float draw_weapon_check_time2 = 0.0f;


    bool chill_with_context = false;


    bool location_mode = false;


    float time_blind_walk = 0.0f;

    bool had_any_path_found_this_run = false;

    bool backup_interaction_made = false;


    int attack_action = -1;
    float attack_action_time0 = 0.0f;
    float attack_action_time1 = 0.0f;

    RE::NiPoint3 last_target_pos{};
    RE::NiPoint3 last_u{};
    bool last_target_pos_valid = false;
    bool last_u_valid = false;


    bool was_charging_ranged = false;


    bool lock_camera_wants_to_crouch = false;

    long long lasttime = 0;
    long long lasttime_close_enough = 0;

    bool low_mana_notified = false;
    float low_mana_notify_time = 0.0f;

    bool attack_paused = false;
    float attack_pause_time = 0.0f;

    bool got_close_for_pickpocket = false;


    bool custom_path_appended = false;



    bool search_next_fight_target = false;
    float search_next_target_timer = 0.0f;


    float intro_look_timeout = 0.0f;

    std::string last_start_attacking_info = "";
    std::string last_attacking_target = "";
    std::string last_attacking_weapon = "";
    std::string last_attacking_health = "";


    bool do_spins = false;
    int amount_of_spins = 0;
    int spin_speed = 0;
    RE::NiMatrix3 spins_start_camera_dir{};
    bool spin_step_one = false;
    int amount_of_spins_done = 0;
    float spin_timeout = 0.0f;


    bool too_high_notified = false;

    bool looking_mode = false;


    bool was_already_dead = false;

    bool explore_mode = false;
    float min_dist = 3000.0f;

    bool explore_mode_notified = false;


    bool surrender_mode = false;
    float surrender_time = 0.0f;


    bool walk_again_when_finished = false;

    bool reset_after_walk = false;


    bool backup_pickup = false;
    int backup_pickup_attempts = 0;
    RE::TESObjectREFR* backup_pickup_object = nullptr;
    float backup_pickup_time = 0.0f;


    bool shout_mode = false;
    RE::TESShout* shout_to_use = nullptr;
    bool gate_shout = false;
    bool ustengrev_get_ready_mode = false;
    bool ustengrev_shout_mode = false;
    float ustengrev_gates_closed_time = 0.0f;
    bool ustengrev_finish_line_mode = false;
    bool ustengrev_run_only_mode = false;
    bool ustengrev_wrong_order_mode = false;
    bool ustengrev_cliff_mode = false;

    float anti_slowwalk_timer = 0.0f;

    bool dont_tell_result = false;




    bool trying_to_fuckup_correct_pillar = false;
    bool fuckup_pillar_confirm_request_sent = false;
    bool fuckup_pillar_confirm_choice_valid = false;
    int fuckup_pillar_confirm_choice = 0;


    bool trying_to_change_quest_course = false;
    bool change_quest_course_request_sent = false;
    bool change_quest_course_choice_valid = false;
    int change_quest_course_choice = 0;


    bool trying_to_attack_friend = false;
    bool attack_friend_request_sent = false;
    bool attack_friend_choice_valid = false;
    int attack_friend_choice = 0;
    int attack_friend_interaction = -1;
    int attack_friend_id = -1;
    bool attack_friend_confirmed = false;
    std::string attack_friend_name = "";


    bool use_last_point_of_last_path = false;
    RE::NiPoint3 last_point_of_last_path{};



    void invalidate_path()
    {
        path_valid = false;
        current_path_point = -1;
        path.clear();
        use_last_point_of_last_path = false;
    }

    void set_just_teleported() //for map
    {
        just_teleported = true;
    }

    bool get_just_teleported()
    {
        return just_teleported;
    }

    void clear_just_teleported()
    {
        just_teleported = false;
    }


    void reset_explore_mode_start_range()
    {
        if (MiscThings::is_interior_cell())
            min_dist = 3000.0f;
        else
            min_dist = 20000.0f;
    }

    bool processing_ustengrev()
    {
        return ustengrev_get_ready_mode || ustengrev_shout_mode || ustengrev_finish_line_mode || ustengrev_run_only_mode || ustengrev_wrong_order_mode || ustengrev_cliff_mode;
    }

    float get_walker_inactive_time()
    {
        return walker_inactive_time;
    }

    void reset_inactive_timer()
    {
        walker_inactive_time = 0.0f;
    }


    void set_crime_mode(bool state)
    {
        crime_mode = state;
    }


    bool is_walking_important_path()
    {
        return using_custom_path;
    }

    bool is_surrendering()
    {
        return surrender_mode;
    }

    bool is_exploring()
    {
        return explore_mode;
    }

    bool is_running_away()
    {
        return runaway_mode;
    }

    bool is_fighting()
    {
        return ((interaction_after_walk == 3) && target_ref);
    }


    bool walker_active()
    {
        return (bool)target_ref;
    }


    bool is_pickpocketing()
    {
        return interaction_after_walk == 2;
    }


    bool gate_shout_condition()
    {
        RE::TESObjectREFR* gate = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3FAFB);
        RE::TESObjectREFR* trigger = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3FB02);

        if (gate && trigger)
        {

            auto base_type = gate->GetBaseObject()->GetFormType();

            if (base_type == RE::FormType::Door)
            {
                auto gate_door = (RE::TESObjectDOOR*)gate;
                return !trigger->IsDisabled() && gate_door->GetOpenState(gate) == RE::BGSOpenCloseForm::OPEN_STATE::kOpen;
            }
        }

        return true;//true because dont want it to block forever if it goes wrong
    }


    


    bool ustengrev_run_condition(float dtime)
    {
        RE::TESObjectREFR* gate1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x2656b);
        RE::TESObjectREFR* gate2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3b2e5);
        RE::TESObjectREFR* gate3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3b2e7);

        if (gate1 && gate2 && gate3)
        {
            //auto base_type1 = gate1->GetBaseObject()->GetFormType();
            //auto base_type2 = gate2->GetBaseObject()->GetFormType();
            //auto base_type3 = gate3->GetBaseObject()->GetFormType();



            //if (base_type1 == RE::FormType::Door && base_type2 == RE::FormType::Door && base_type3 == RE::FormType::Door)
            //{

            auto extra1 = (RE::ExtraLastFinishedSequence*)gate1->extraList.GetByType(RE::ExtraDataType::kLastFinishedSequence);
            auto extra2 = (RE::ExtraLastFinishedSequence*)gate2->extraList.GetByType(RE::ExtraDataType::kLastFinishedSequence);
            auto extra3 = (RE::ExtraLastFinishedSequence*)gate3->extraList.GetByType(RE::ExtraDataType::kLastFinishedSequence);

            auto extra12 = (RE::ExtraAction*)gate1->extraList.GetByType(RE::ExtraDataType::kAction);
            auto extra22 = (RE::ExtraAction*)gate2->extraList.GetByType(RE::ExtraDataType::kAction);
            auto extra32 = (RE::ExtraAction*)gate3->extraList.GetByType(RE::ExtraDataType::kAction);

            if (extra1 && extra2 && extra3)
            {
                //auto gate1_door = (RE::TESObjectDOOR*)gate1;
                //auto gate2_door = (RE::TESObjectDOOR*)gate2;
                //auto gate3_door = (RE::TESObjectDOOR*)gate3;

                //std::string sequence1 = extra1->lastSequenceName.c_str();
                //std::string sequence2 = extra2->lastSequenceName.c_str();
                //std::string sequence3 = extra3->lastSequenceName.c_str();

                //bool gate1_closed = sequence1 == "Open";
                //bool gate2_closed = sequence2 == "Open";
                //bool gate3_closed = sequence3 == "Open";

                //gate1_closed &= extra12->action.any(RE::OBJECT_ACTION::kOpen);
                //gate2_closed &= extra22->action.any(RE::OBJECT_ACTION::kOpen);
                //gate3_closed &= extra32->action.any(RE::OBJECT_ACTION::kOpen);

                bool gate1_closed = MiscThings::two_state_activator_state(gate1) == 1;
                bool gate2_closed = MiscThings::two_state_activator_state(gate1) == 1;
                bool gate3_closed = MiscThings::two_state_activator_state(gate1) == 1;

                if (gate1_closed && gate2_closed && gate3_closed)
                    ustengrev_gates_closed_time += dtime;
                else
                    ustengrev_gates_closed_time = 0.0f;


                if (ustengrev_gates_closed_time > 4.0f)
                    return true;
                else
                    return false;


            }
            else
                return false;


            //}
            /*
            if (base_type1 == RE::FormType::Door && base_type2 == RE::FormType::Door && base_type3 == RE::FormType::Door)
            {
                auto gate1_door = (RE::TESObjectDOOR*)gate1;
                auto gate2_door = (RE::TESObjectDOOR*)gate2;
                auto gate3_door = (RE::TESObjectDOOR*)gate3;

                bool gate1_closed = gate1_door->GetOpenState(gate1) == RE::BGSOpenCloseForm::OPEN_STATE::kOpen;
                bool gate2_closed = gate1_door->GetOpenState(gate2) == RE::BGSOpenCloseForm::OPEN_STATE::kOpen;
                bool gate3_closed = gate1_door->GetOpenState(gate3) == RE::BGSOpenCloseForm::OPEN_STATE::kOpen;

                return gate1_closed && gate2_closed && gate3_closed;

            }
            */
        }


        return true;//true because dont want it to block forever if it goes wrong
    }




    bool just_escaped_solitude_prison()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        RE::NiPoint3 solitude_prison_exit_point = { -64012.2695, 106968.758, -8161.62207 };

        return (player_pos - solitude_prison_exit_point).Length() < 250.0f;
    }


    bool prefer_solitude_prison_escape_route()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto parent_cell = player->GetParentCell();
        bool is_escaping = MiscThings::player_escaping_jail();

        if (is_escaping && parent_cell && parent_cell->GetFormID() == 0x56e88)
        {
            auto crumbling_wall = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x5ABC1);
            if (crumbling_wall)
            {
                auto extra_action = crumbling_wall->extraList.GetByType(RE::ExtraDataType::kAction);

                if (extra_action)
                    return true;
            }
        }

        return false;
    }




    void cancel_charge_weapon()
    {
        was_charging_ranged = false;

        //right_attack_cancel();
        //left_attack_cancel();

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        //GetAttackState()
        if (player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowDrawn || player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowDraw || player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowAttached)// || //player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowReleasing || player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowFollowThrough || player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowReleased || 
            //is_casting_walker(true) || is_casting_walker(false))
            ready_weapon();
    }



    bool pause_attacking(float dtime)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (!attack_paused)
        {
            if (start_attacking)
            {
                attack_paused = true;
                gave_attacking_info = false;
                if (was_charging_ranged)
                {
                    cancel_charge_weapon();
                }

                right_attack_cancel();
                left_attack_cancel();
                attack_action_time0 = 0.0f;
                attack_action_time1 = 0.0f;
            }
            else
            {
                attack_paused = true;
            }
        }
        else
        {
            if (attack_pause_time < 0.5f)
            {
                attack_pause_time += dtime;
            }
            else
            {
                //attack_pause_time = 0.0f;
                return true;
            }
        }

        return false;
    }


    void unpause_attacking()
    {
        attack_pause_time = 0.0f;
        attack_paused = false;
    }






    void correct_marker_pos()
    {

        return;

        auto marker = RE::TESObjectREFR::LookupByID(0x7001834);


        if (target_ref && marker)
        {

            auto marker_ref = marker->AsReference();
            if (marker_ref)
            {
                auto shift = MiscThings::get_looking_point_shift(target_ref, false);


                auto pickpocket_shift = RE::NiPoint3::Zero();

                if (interaction_after_walk == 2 && target_ref && target_ref->IsHumanoid())
                {
                    pickpocket_shift = MiscThings::get_looking_point_shift(target_ref, true);
                }
                
                if (using_custom_path)
                    MiscThings::SetPosition_moveto(marker_ref, custom_path.at(0));
                else
                    MiscThings::SetPosition_moveto(marker_ref, marker_ref->GetPosition() + shift + pickpocket_shift);

                //if (using_custom_path)
                //    marker_ref->SetPosition(custom_path.at(0));
                //else
                //    marker_ref->SetPosition(marker_ref->GetPosition() + shift + pickpocket_shift);
            }
        }

    }



    /*
    bool has_current_activity()
    {
        std::string result = "";


        if (target_ref)
            result = "walking";

        if (interaction_after_walk == 1)
            result += " to interact with target";

        if (interaction_after_walk == 2)
            result += " to pickpocket the target";

        if (interaction_after_walk == 3)
            result += " to start a fight with the target";


        auto target_runaway = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x7003887);
        if (target_ref == target_runaway)
            result = "running away";

        if (start_attacking)
            result = "fighting";
    }
    */



    bool probe_failed()
    {
        if (navmesh_probe_result_valid)
            return !navmesh_probe_result;

        return false;
    }


    namespace Hooks {

        struct Pathing {

            //VTABLE_BSPathingRequest

            //119CC80 - need to hook this.

            



            static void myStart(RE::GuideEffect* a_guideEffect)
            {
                auto my_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("myscPath");

                if (my_quest)
                {


                    RE::TESObjectREFR* pre_probe_target = target_ref;

                    if (navmesh_probe_mode)
                    {
                        RE::TESObjectREFR* probe_target = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x7003887); //runaway marker

                        
                        pre_probe_target = target_ref;
                        target_ref = probe_target;
                    }



                    if (!target_ref)
                    {
                        stop_casting();
                    }


                    if (my_quest->GetCurrentStageID() != 10);
                    {
                        ;
                        //TODO: somehow set the stage of quest
                        /*
                        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::Console::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
                        RE::UIMessageQueue::GetSingleton()->ProcessCommands();

                        RE::UI* ui = RE::UI::GetSingleton();
                        auto console = ui->GetMenu<RE::Console>();
                        if (console)
                        {
                            console->ExecuteCommand("setstage myscPath 10");
                        }
                        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::Console::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
                        RE::UIMessageQueue::GetSingleton()->ProcessCommands();
                        */
                    }


                    //teleport pathfinder key object to target object

                    //1001834 (7001834)


                    auto marker = RE::TESObjectREFR::LookupByID(0x7001834);


                    if (target_ref && marker)
                    {


                        if (correct_word_of_power)
                        {
                            auto word_of_power = MiscThings::get_word_of_power(target_ref, true);

                            if (word_of_power && word_of_power != (RE::TESObjectREFR*)(-1))
                            {
                                target_ref = word_of_power;
                                correct_word_of_power = false;
                            }

                        }





                        //auto target_cell = target_ref->GetParentCell();
                        //marker->AsReference()->SetParentCell(target_cell);

                        //auto pos_target = target_ref->GetPosition();
                        //marker->AsReference()->SetPosition(pos_target);

                        auto marker_ref = marker->AsReference();
                        if (marker_ref)
                        {
                            marker_ref->MoveTo(target_ref);


                            auto shift = MiscThings::get_looking_point_shift(target_ref, false);
                            auto pickpocket_shift = RE::NiPoint3::Zero();

                            if (interaction_after_walk == 2 && target_ref && target_ref->IsHumanoid())
                            {
                                pickpocket_shift = MiscThings::get_looking_point_shift(target_ref, true);
                            }

                            if (using_custom_path)
                                MiscThings::SetPosition_moveto(marker_ref, custom_path.at(0));
                            else
                                MiscThings::SetPosition_moveto(marker_ref, marker_ref->GetPosition() + shift + pickpocket_shift);




                            //correct_marker_pos();
                        }
                        else
                            ;//idk do nothing? its probably already on target anyway
                    }

                    //RE::TESObjectREFR::LookupByHandle(0x7001834);
                    //10529
                    //8481

                    auto player = RE::PlayerCharacter::GetSingleton();
                    auto targets = player->questTargets;

                    std::vector<std::pair<RE::TESQuest*, bool>> quests_to_restore{};


                    player->currentProcess->cachedValues->cachedWidth = 25.0; //this is it. this is used to calculate path for clairvoyance.


                    for (auto target : targets)
                    {
                        if (target.first == my_quest)
                        {
                            RE::TESQuest* the_quest = target.first;
                            the_quest->data.flags.set(RE::QuestFlag::kActive);
                        }
                        else
                        {
                            RE::TESQuest* the_quest = target.first;

                            bool old_flag = the_quest->data.flags.all(RE::QuestFlag::kActive);

                            the_quest->data.flags.reset(RE::QuestFlag::kActive);

                            quests_to_restore.push_back({ the_quest, old_flag });
                        }
                    }

                    if (my_quest)
                    {
                        if (my_quest->Is(RE::FormType::Quest))
                        {
                            RE::TESQuest* the_quest = (RE::TESQuest*)my_quest;

                            bool test_bool = false;
                        }
                    }


                    RE::TESObjectREFR* player_marker = nullptr;
                    auto p_marker = player->playerMapMarker;

                    if (p_marker)
                    {
                        player->playerMapMarker.reset();
                    }

                    auto marker_path = player->playerMarkerPath;

                    player->playerMarkerPath = nullptr;

                    //saved_guide_effect = a_guideEffect;
                    originalStart(a_guideEffect); //call original function, let it read fake target, then restore original quest flags


                    if (p_marker)
                    {
                        player->playerMapMarker = p_marker; //restore
                    }

                    player->playerMarkerPath = marker_path;

                    for (auto quest_to_restore : quests_to_restore)
                        if (quest_to_restore.second)
                            quest_to_restore.first->data.flags.set(RE::QuestFlag::kActive);

                    my_quest->data.flags.reset(RE::QuestFlag::kActive);
                    my_quest->data.flags.reset(RE::QuestFlag::kDisplayedInHUD);


                    //fill path
                    path_valid = false;
                    //auto path_to_save = path;


                    path.clear();

                    auto hazards = a_guideEffect->hazards;
                    auto quest = a_guideEffect->questTarget;

                    if (navmesh_probe_mode)
                    {
                        navmesh_probe_result = std::size(hazards) > 2;
                        navmesh_probe_result_valid = true;
                        navmesh_probe_mode = false;
                        target_ref = pre_probe_target;
                        //and here it ends
                    }
                    else
                    {
                        for (auto hazard : hazards)
                        {
                            path.push_back(hazard.get()->data.location);
                        }


                        if (std::size(path) > 2)
                        {
                            had_any_path_found_this_run = true;
                            if (explore_mode)
                            {
                                reset_explore_mode_start_range();
                                if (!explore_mode_notified)
                                {
                                    send_random_context("[You found a direction to explore, and started walking", false);
                                    explore_mode_notified = true;
                                }

                            }
                        }
                        else
                        {
                            if (explore_mode)
                            {
                                //try to find different object
                                if (min_dist > 100.0f)
                                {
                                    min_dist /= 2.0f;

                                    auto result_probe = explore_world(true);

                                }
                                else
                                {
                                    //min_dist = 20000.0f;
                                    explore_mode = false;
                                    reset_walker();

                                    if (MiscThings::is_interior_cell())
                                    {
                                        register_exit_dungeon();
                                        unregister_explore_action();
                                    }

                                    send_random_context("You cannot find any interesting direction to explore", false);
                                }

                                return;
                            }
                            else
                            {
                                if (target_ref)
                                {
                                    if (target_is_too_high())
                                    {
                                        if (!too_high_notified)
                                        {
                                            too_high_notified = true;
                                            send_random_context(MiscThings::insert_object_into_list_and_get_info(target_ref) + " is too high! Looking at it instead. ", false);
                                        }
                                    }
                                    else
                                    {
                                        if (!navmesh_probe_result_valid)
                                        {
                                            navmesh_probe_mode = true;
                                            navmesh_probe_result = false;
                                            navmesh_probe_result_valid = false;
                                            return;
                                            //and we hope whoever called us - calls us again and we get probe
                                        }
                                    }
                                }

                                

                            }

                        }


                        if (marker)
                            if (auto marker_ref = marker->AsReference(); marker_ref && (int)std::size(path) > 0)
                            {
                                auto marker_pos = marker_ref->GetPosition();
                                auto last_hazard_pos = path.at((int)std::size(path) - 1);

                                if ((marker_pos - last_hazard_pos).Length() < 150.0f)
                                    path.push_back(marker_ref->GetPosition());
                            }


                        if (using_custom_path)
                        {
                            if (!path.empty())
                            {
                                auto last_point = path.at(std::size(path) - 1);

                                if (last_point.GetDistance(custom_path.at(0)) < 200.0f)
                                {
                                    custom_path_appended = true;
                                    path.insert(path.end(), custom_path.cbegin(), custom_path.cend()); //append custom path
                                }

                                /*
                                int pos = 0;

                                for (auto point : custom_path)
                                {
                                    if (last_point.GetDistance(point) < 200.0f)
                                    {
                                        std::vector<RE::NiPoint3> new_custom_path{};
                                        for (int i = pos; i < std::size(custom_path); i++)
                                            new_custom_path.push_back(custom_path.at(i));

                                        path.insert(path.end(), new_custom_path.cbegin(), new_custom_path.cend()); //append custom path
                                    }
                                    pos++;
                                }
                                */
                            }
                        }

                        path_valid = true;
                    }
                    

                }
                else
                {
                    RE::ConsoleLog::GetSingleton()->Print("PATHFINDER QUEST NOT FOUND, INSTALL mysc.esp!");
                    RE::UIMessageQueue::GetSingleton()->AddMessage(RE::Console::MENU_NAME, RE::UI_MESSAGE_TYPE::kShow, nullptr);
                    originalStart(a_guideEffect);
                }


                return;
            }



            //73 min 74 max


            static void myFinish(RE::GuideEffect* a_guideEffect)
            {
                //RE::GuideEffect* test = (RE::GuideEffect * )(REL::Relocation<std::uintptr_t>(RE::VTABLE_GuideEffect[0]).address());

                path_valid = false;
                path.clear();

                auto hazards = a_guideEffect->hazards;

                auto quest = a_guideEffect->questTarget;

                for (auto hazard : hazards)
                {
                    path.push_back(hazard.get()->data.location);
                }

                path_valid = true;

                return originalFinish(a_guideEffect);
            }



            //22151
            /*
            static bool setQuestStage(RE::TESQuest* quest, uint16_t stage)
            {
                if (!quest) return 0;                               //24482 //22151
                //REL::Relocation<decltype(setQuestStage)> func(REL::ID(22151));
                //return func(quest, stage);
                return originalSetStage(quest, stage);

            }
            */


            static bool myActivate(RE::TESBoundObject* source, RE::TESObjectREFR* a_targetRef, RE::TESObjectREFR* a_activatorRef, std::uint8_t a_arg3, RE::TESBoundObject* a_object, std::int32_t a_targetCount)
            {
                //auto workbench_refr = RE::TESObjectREFR::LookupByID(0x7001834);
                //auto workbench = (RE::TESFurniture*)workbench_refr;

                return originalActivate(source, a_targetRef, a_activatorRef, a_arg3, a_object, a_targetCount);

            }

            static inline REL::Relocation<decltype(myActivate)> originalActivate;



            static inline REL::Relocation<decltype(myStart)> originalStart;
            static inline REL::Relocation<decltype(myFinish)> originalFinish;


            //REL::ID(21910)


            static inline void Install() { 
                originalStart = REL::Relocation<std::uintptr_t>(RE::VTABLE_GuideEffect[0]).write_vfunc(0x14, myStart);
                //originalFinish = REL::Relocation<std::uintptr_t>(RE::VTABLE_GuideEffect[0]).write_vfunc(0x15, myFinish); 

                //originalActivate = REL::Relocation<std::uintptr_t>(RE::VTABLE_TESBoundObject[0]).write_vfunc(0x37, myActivate);
            }
        };
    }





    void install_hook()
    {
        Hooks::Pathing::Install();
    }


    RE::NiPoint3 along_next_path_points_vector()
    {

        RE::NiPoint3 result{};


        if (karthspire_plates)
        {
            auto camera = RE::PlayerCamera::GetSingleton();
            auto camera_dir = camera->cameraRoot.get()->world.rotate;
            auto camera_pos = camera->pos;

            RE::NiPoint3 direction_vector{};

            direction_vector = camera_dir.GetVectorY();

            direction_vector.z = -0.6f;

            return direction_vector;

        }


        try {
            if (use_last_point_of_last_path)
            {
                return result; //this is not right

                float max_diff = 0.0f;

                auto player = RE::PlayerCharacter::GetSingleton();

                RE::NiPoint3 direction_vector{};

                if (player)
                {
                    RE::NiPoint3 current_path_point_pos = player->GetPosition();
                    RE::NiPoint3 next_point_pos = last_point_of_last_path;

                    direction_vector = next_point_pos - current_path_point_pos;

                    direction_vector.Unitize();

                    if (MiscThings::is_on_horse())
                        direction_vector.z -= 0.2f;
                    else
                        direction_vector.z -= 0.08f;


                    /*
                    if (direction_vector.z < -0.4)
                        direction_vector.z = -0.4;

                    if (direction_vector.z > 0.4)
                        direction_vector.z = 0.4;
                    */


                    //direction_vector = next_point_pos - current_path_point_pos;
                }


                result = direction_vector;
            }
            else
            {
                if (path_valid)
                {
                    float max_diff = 0.0f;

                    auto player = RE::PlayerCharacter::GetSingleton();

                    if (player && target_ref)
                    {
                        if (player->GetDistance(target_ref, true) < 300.0f)
                            return result; //close-to-target alongs are fucked because of z shift
                    }


                    if (current_path_point <= (int)std::size(path) - 4)
                    {
                        RE::NiPoint3 current_path_point_pos = path.at(current_path_point);
                        RE::NiPoint3 next_point_pos = path.at(std::min(((int)std::size(path) - 3), current_path_point + 2));

                        RE::NiPoint3 direction_vector = next_point_pos - current_path_point_pos;


                        if (MiscThings::is_player_swimming())
                        {
                            auto player_pos = player->GetPosition();

                            next_point_pos = path.at(std::min(((int)std::size(path) - 1), current_path_point + 1));
                            direction_vector = next_point_pos - player_pos;
                        }



                        direction_vector.Unitize();

                        if (MiscThings::is_on_horse())
                            direction_vector.z -= 0.2f;
                        else
                            direction_vector.z -= 0.08f;

                        /*
                        if (direction_vector.z < -0.4)
                            direction_vector.z = -0.4;

                        if (direction_vector.z > 0.4)
                            direction_vector.z = 0.4;
                        */


                        result = direction_vector;
                    }

                }
            }

        }
        catch (const std::out_of_range& e) {
            result = { 0,0,0 }; //path is broken
        }


        return result;
    }



    bool is_about_to_fall()
    {
        bool result = false;
        try {

            auto player_pos = RE::PlayerCharacter::GetSingleton()->GetPosition();


            if (MiscThings::is_on_horse())
                return false;

            if (path_valid && !use_last_point_of_last_path)
            {
                float max_diff = 0.0f;

                int max_i = -1;

                for (int i = current_path_point; i < (std::min((int)std::size(path), current_path_point + 2)); i++)
                {
                    float diff = last_point_posZ - path.at(i).z;
                    if (diff > max_diff)
                    {
                        max_i = i;
                        max_diff = diff;
                    }
                        
                }

                if (max_diff > 100.0f)
                {
                    if (max_i >= 0)
                        if (player_pos.z > path.at(max_i).z) //path_point_reached zeroes .z sometimes and it can lead to weird situations where current path point if 10m higher than player
                            result = true;
                }
                    

            }
        }
        catch (const std::out_of_range& e) {
            return false; //path is broken
        }


        return result;
    }

    bool needs_jump()
    {
        bool result = false;
        try {

            if (path_valid && !use_last_point_of_last_path && current_path_point < (int)std::size(path) - 3)
            {
                float max_diff = 0.0f;

                for (int i = current_path_point; i < (std::min((int)std::size(path) - 3, current_path_point + 2)); i++)
                {
                    float diff = path.at(i).z - last_point_posZ;
                    if (diff > max_diff)
                        max_diff = diff;
                }

                if (max_diff > 70.0f)
                    result = true;

            }
        }
        catch (const std::out_of_range& e) {
            return false; //path is broken
        }

        return result;
    }



    bool may_sprint()
    {
        //return true;
        bool result = false;
        try {

            if (path_valid && !use_last_point_of_last_path && (int)std::size(path) > 10)
            {
                float has_sharp_turns = false;

                auto dif_big = path.at((int)std::size(path) - 2) - path.at(current_path_point);


                for (int i = current_path_point + 1; i < ((int)std::size(path) - 2); i++)
                {
                    auto dif_smal = path.at(i) - path.at(i - 1);

                    if (abs(dif_big * dif_smal - 1.0) > 0.3)
                    {
                        has_sharp_turns = true;
                        break;
                    }

                }


                result = !has_sharp_turns;


            }
        }
        catch (const std::out_of_range& e) {
            return false; //path is broken
        }

        return result;
    }



    void refresh_reminder_start_pos()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        
        if (player)
            reminder_start_pos = player->GetPosition();
    }



    bool player_is_sprinting()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
        {
            return player->playerFlags.isSprinting;
        }

        return false;
    }



    float last_walk_reminded_time = 0.0f;

    void walk_to_point(float dtime_maybe_bad)
    {



        if (path_valid || use_last_point_of_last_path)
        {
            auto player = RE::PlayerCharacter::GetSingleton();

            if (player && player->IsSneaking())
            {
                crouch(); //uncrouch
            }

            try {

            wait_and_start_pickpocket = false;
            waiting_pickpocket_time = 0.0f;


            auto camera = RE::PlayerCamera::GetSingleton();
            auto camera_dir = camera->cameraRoot.get()->world.rotate;
            auto camera_pos = camera->pos;
            RE::NiPoint3 path_point_pos;// =
              
            if (use_last_point_of_last_path)
                path_point_pos = last_point_of_last_path;
            else
                path_point_pos = path.at(current_path_point);
            
            
            //auto path_point_pos = target_ref->GetPosition();
            auto pos_dif = path_point_pos - camera_pos;
            auto pos_dif_norm = pos_dif / pos_dif.Length();

            auto camera_dirX = camera_dir.GetVectorX();
            auto camera_dirY = camera_dir.GetVectorY();
            auto camera_dirZ = camera_dir.GetVectorZ();

            RE::NiPoint3 camera_dirY_fixed = camera_dirY;// { -camera_dirY.z, camera_dirY.y, camera_dirY.x };

            //auto dir_dif_X = pos_dif_norm - camera_dir_2;

            float mouse_x = 0.0f;
            float mouse_y = 0.0f;

            
            //auto test_crossX = camera_dirY.Cross(pos_dif_norm);

            //auto camera_dirX_noZ = camera_dirY;
            //camera_dirX_noZ.z = 0.0f;

            //auto test_s

            //send_random_context()
            /*
            if (test_crossX.Length() > 1.0f)// && mulX < 0.0001)
            {
                if (mulX < 0.0f)
                    mulX = -100.0f;
                else
                    mulX = 100.0f;
            }
               */ 
            //auto mulX = 0.0f;
            //auto mulY = 0.0f;
            //auto mulZ = 0.0f;

            auto desired_direction = pos_dif_norm;
            auto desired_direction_norm = desired_direction / desired_direction.Length();

            auto mulX = camera_dirX * desired_direction_norm;
            auto mulZ = camera_dirZ * desired_direction_norm;
            desired_direction_norm.z = 0.0f;
            auto mulY = camera_dirY * desired_direction_norm;



            bool use_y = false;
            //if (target_ref && !use_last_point_of_last_path && (current_path_point < ((int)std::size(path) - 2)))

            
            if (target_ref && !use_last_point_of_last_path && (current_path_point < ((int)std::size(path) - 2)))
            {
                //auto camera_dirZ_noZ = camera_dirZ;
                //camera_dirZ_noZ.z = 0.0f;

                //auto pos_difY = target_ref->GetPosition() - camera_pos;
                //auto pos_difY_norm = pos_difY / pos_difY.Length();

                

                


                desired_direction = along_next_path_points_vector();



                if (desired_direction != RE::NiPoint3::Zero())
                {
                    desired_direction_norm = desired_direction / desired_direction.Length();

                    //if ((int)std::size(path) - current_path_point > 7 && pos_difY.Length() < 300.0f)
                    auto pos_difY_norm = pos_dif_norm;
                    pos_difY_norm.z = 0;

                    RE::NiPoint3 uuhhh = { 1.0f, 0.0f, 0.0f };



                    //mulY = camera_dirY * desired_direction_norm;
                    //mulX = camera_dirX * desired_direction_norm;
                    mulZ = camera_dirZ * desired_direction_norm;
                    desired_direction_norm.z = 0.0f;
                    //mulY = camera_dirY * desired_direction_norm;

                    //camera_dirZ_noZ = camera_dirZ_noZ / camera_dirZ_noZ.Length();
                    //mulY = camera_dirY * pos_difY_norm;
                    //mulZ = camera_dirZ * desired_direction_norm;// pos_difY_norm;
                    use_y = true;
                }

            }
            

            if (mulY < 0)
            {
                mulZ = 0.0f;// -mulZ;
                if (mulX < 0.0f)
                    mulX = -0.3f + mulX;
                else
                    mulX = 0.3f + mulX;
            }


            mouse_x = mulX * 125.0f;//200 //150
            mouse_y = mulZ * 60.0f;//200

            if (!use_y)
                mouse_y = 0.0f;

            if (abs(mouse_x) > 100.0f)
            {
                if (mouse_x < 0.0f)
                    mouse_x = -100.0f;
                else
                    mouse_x = 100.0f;
            }

            if (abs(mouse_y) > 50.0f)
            {
                if (mouse_y < 0.0f)
                    mouse_y = -50.0f;
                else
                    mouse_y = 50.0f;
            }

            /*
            if (pos_dif.Length() < 200.0f)
            {
                if (abs(mouse_x) > 10.0f)
                {
                    if (mouse_x < 0.0f)
                        mouse_x = -10.0f;
                    else
                        mouse_x = 10.0f;
                }
            }
            */

            //mouse_mouse_x(mouse_x);

            auto test_new_input = RE::BSInputEventQueue::GetSingleton();
            auto test_new_input2 = RE::BSInputDeviceManager::GetSingleton();


            



            auto mouse = test_new_input2->GetMouse();


            auto test_bool = test_new_input2->IsMouseBackground();


            //bool test12345 = test_new_input3->IsLookingControlsEnabled();
            //bool test12346 = test_new_input3->IsConsoleControlsEnabled();
            //bool test12347 = test_new_input3->IsMenuControlsEnabled();


            auto test_new_input3 = RE::ControlMap::GetSingleton();
              
            auto priority_stack = test_new_input3->contextPriorityStack;
            auto linked_mappings = test_new_input3->linkedMappings;

            for (int i = 0; i < 20; i++)
            {
                auto ii = static_cast<RE::UserEvents::INPUT_CONTEXT_ID>(i);

                test_new_input3->PopInputContext(ii);
            }
            test_new_input3->PushInputContext(RE::UserEvents::INPUT_CONTEXT_ID::kGameplay);



            if (was_charging_ranged)
            {
                cancel_charge_weapon();
            }



            if (abs(mulX) < 0.4 && mulY >= 0)
            {
                turning_around = false;
                //float height_dif = last_point_posZ - path[current_path_point].z;

                if (always_shift || is_about_to_fall())
                {
                    if (player->IsRunning() && !player->IsSneaking() && !was_slowwalking)
                    {
                        was_slowwalking = true;
                        slow_walk(); //if next point is much lower - we probably need to fall. so start walking slow to not get clinged to potential wall next to us
                    }
                        
                }
                else
                {

                    if (!always_shift && (!(player->IsRunning()) && !(player->IsSneaking()) && was_slowwalking))
                    {
                        was_slowwalking = false;
                        unslow_walk();
                    }
                    else
                    {
                        if (!player->IsRunning() && !player->IsSneaking() && !was_slowwalking && !turning_around && !always_shift)
                        {
                            //test if we are slowwalking for some reason
                            anti_slowwalk_timer += dtime_maybe_bad;

                            if (anti_slowwalk_timer > 3.0f)
                            {
                                unslow_walk();
                                anti_slowwalk_timer = 0.0f;
                            }
                        }
                        else
                            anti_slowwalk_timer = 0.0f;
                    }
                }

                        

                if (needs_jump() && !player->IsSneaking())
                    jump(); //if next point is much higher - we probably need to jump.


                float stamina_state = MiscThings::get_player_stamina() / MiscThings::get_player_max_stamina();

                if (!silly_walk_mode && !MiscThings::is_interior_cell() && may_sprint())
                {
                    //sprint();
                    if (stamina_state > 0.6f)
                    if (!player_is_sprinting())// && !was_sprinting)
                    {
                        if (!launching_sprint())
                        {
                            make_launch_sprint();
                            was_sprinting = true;
                        }
                            
                    }



                       

                    //if (!was_sprinting || !player->IsSprinting())
                    {
                        //if (!player->IsSprinting() && !was_sprinting)
                        //if (!player->IsSprinting())
                        //{
                      //  sprint();
                        //was_sprinting = true;
                        //}
                    }
                } 

                if (player_is_sprinting() && stamina_state < 0.1f)
                {
                    if (!launching_sprint())
                    {
                        make_launch_sprint();
                        was_sprinting = false;
                    }

                }

                /*
                else
                {
                    if (was_sprinting && player->IsSprinting())
                    //if (player->IsSprinting())
                    {
                        was_sprinting = false;
                        unsprint();
                    }
                    else
                    {
                        was_sprinting = false;
                    }
                        
                }
                */
                //cursor_up(); //TODO: proper walk mechanics. somethimes just holding W is not good




                //right_attack_cancel();
                //left_attack_cancel();

                bool brought_no_weapons_to_a_fight = MiscThings::has_spell_equipped(true) && MiscThings::has_spell_equipped(false) && !MiscThings::is_offensive_spell(true) && !MiscThings::is_offensive_spell(false);

                if (last_walk_reminded_time > 40.0f || ((explore_mode || runaway_mode) && last_walk_reminded_time > 15.0f) || (interaction_after_walk == 3 && brought_no_weapons_to_a_fight && last_walk_reminded_time > 15.0f))
                {
                    last_walk_reminded_time = 0.0f;

                    //reminder_target_name = "";
                    //reminder_walk_time = 0;
                    //reminder_distance = 0;

                    reminder_distance += (player->GetPosition() - reminder_start_pos).Length();

                    reminder_start_pos = player->GetPosition();

                    std::string reminder_message = "";
                    if (quest_mode)
                        reminder_message = "[You keep following the quest: ";
                    else
                        reminder_message = "[You keep walking to ";

                    if (runaway_mode)
                        reminder_message = "[You keep " + reminder_target_name;
                    else
                        reminder_message += reminder_target_name + ". ";

                    if (brought_no_weapons_to_a_fight)
                        reminder_message += " You dont have any weapons equipped for this fight! Equip something that can deal damage (weapon or spell). ";

                    std::string big_distance = "";


                    
                    float distance = player->GetDistance(target_ref, true, true);
                   
                    if (current_quest_target_followed && current_quest_followed)
                    {
                        distance = MiscThings::get_quest_target_distance(current_quest_target_followed, current_quest_followed);
                    }


                    if (special_ref_for_distance_calculation)
                    {
                        distance = player->GetDistance(special_ref_for_distance_calculation, true, true);
                    }

                    if (current_quest_target_followed && current_quest_followed)
                    {
                        distance = MiscThings::get_quest_target_distance(current_quest_target_followed, current_quest_followed);
                    }

                    //if (distance > 40000.0f)
                    {
                        int distance_int = (int)(distance/100.0f);
                        big_distance = "Distance to target: " + std::to_string(distance_int) + " m. ";
                    }


                    if (distance > 20000.0f)
                    {
                        if (current_quest_target_followed && current_quest_followed)
                        {
                            std::string good_fasttravel_location = MiscThings::get_good_fasttravel_marker_for_quest_target(current_quest_target_followed, current_quest_followed);

                            if (good_fasttravel_location != "" && advice_counter < 2)
                            {
                                advice_counter++;

                                //advice
                                big_distance += " Closest fast-travel location: " + good_fasttravel_location + ". (You can use map to fast travel)";
                            }
                        }
                        
                    }

                        

                    if (!runaway_mode)
                    {
                        reminder_message += "Distance walked: " + std::to_string((int)(reminder_distance / 100.0f)) + " m. ";
                        reminder_message += "Walk time: " + std::to_string((int)reminder_walk_time) + " s. ";
                        reminder_message += big_distance;
                    }




                    bool silent = !runaway_mode;

                    silent = false; //experimental

                    send_random_context(reminder_message, silent);
                }
                else
                    last_walk_reminded_time += dtime_maybe_bad;


                reset_inactive_timer();

                walk_forward();

            }
            else
            {
                turning_around = true;
                mouse_y = 0.0f; //maybe this can be fixed some other way


            }
                
            if (MiscThings::is_on_horse())
            {
                if (mulX < -0.1f)
                {
                    left();
                }

                if (mulX > 0.1f)
                {
                    right();
                }
            }


            //mouse_mouse_x_y(mouse_x, -mouse_y);
            mouse_look(mouse_x, -mouse_y);


            /*
            if (abs(mouse_x) < 1.0f && abs(mouse_y) >= 1.0f)
                mouse_mouse_y(-mouse_y);

            if (abs(mouse_x) >= 1.0f && abs(mouse_y) < 1.0f)
                mouse_mouse_x(mouse_x);

            if (abs(mouse_x) >= 1.0f && abs(mouse_y) >= 1.0f)
                mouse_mouse_x_y(mouse_x, -mouse_y);
            */


            //mouse_mouse_y(-mouse_y);


            //mouse_mouse_x_y(dir_dif.x * 100.0f, dir_dif.y * 100.0f);

            /*
            if (dir_dif.x < 0)
                mouse_mouse_x(dir_dif.x*1000.0f);
            else
                mouse_mouse_x(-5.0f);

            if (dir_dif.y < 0)
                mouse_mouse_y(5.0f);
            else
                mouse_mouse_y(-5.0f);
     */
            }
            catch (const std::out_of_range& e) {
                return; //path is broken
            }

            

            bool test_debug = false;
        }
       
    }



    bool have_doors_nearby()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();

        bool result = false;

        if (player_ref)
        {
            RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 500.0f,
                //player->GetParentCell()->ForEachReferenceInRange(player->GetPosition(), 3000.0,
                [&](RE::TESObjectREFR* a_ref) {

                    std::string name = a_ref->GetName();
                    std::string player_name = RE::PlayerCharacter::GetSingleton()->GetName();


                    if (!MiscThings::is_object_valid(a_ref))
                        return RE::BSContainer::ForEachResult::kContinue;

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

                    if (base_type == RE::FormType::Door)// && a_ref->GetDisplayFullName() == "")
                    {
                        if (MiscThings::raycastable(a_ref, 500.0f))
                        {
                            result = true;
                            return RE::BSContainer::ForEachResult::kStop;
                        }

                    }


                    return RE::BSContainer::ForEachResult::kContinue;
                });
        }

        return result;
    }
    


    RE::TESObjectREFR* get_targeted_ref()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();

        auto crosshair_data = RE::CrosshairPickData::GetSingleton();

        if (crosshair_data && crosshair_data->target)
        {
            return crosshair_data->target.get().get(); //wtf is this
        }

        if (target_ref)
        {
            auto base_obj = target_ref->GetBaseObject();
            if (player_ref && base_obj && (base_obj->formFlags & RE::TESForm::RecordFlags::kDestructible))
            {
                auto base_type = base_obj->GetFormType();

                if (base_type == RE::FormType::Activator)
                {
                    auto static_obj = (RE::TESObjectACTI*)base_obj;

                    std::string model = static_obj->GetModel();

                    if (model.find("FXspiderWebKitDoorSpecial") != std::string::npos)
                    {
                        if (target_ref->GetDistance(player_ref, true) < 200.0f)
                        {
                            return target_ref;
                        }
                    }

                }
            }
        
        }



        return nullptr;

    }


    bool test_about_to_be_blocked_by_door(float dtime)
    {
        bool result = false;

        if (ignore_closed_doors_time > 0)
        {
            ignore_closed_doors_time -= dtime;
            return result;
        }


        auto targeted_ref = get_targeted_ref();
        if (targeted_ref)// && (targeted_ref != target_ref || quest_mode)) //not nice
        {
            auto base_obj = targeted_ref->GetBaseObject();
            if (base_obj && base_obj->GetFormType() == RE::FormType::Door && !turning_around)
            {
                auto door_object = (RE::TESObjectDOOR*)base_obj;

                std::string model = door_object->GetModel();

                if (model.find("TrapHingeTrigger01") != std::string::npos)
                {
                    return false;
                }



                if (last_targeted_ref == targeted_ref)
                {
                    if (have_door_targeted_time > 0.3f)//0.15f)
                    {
                        //have_door_targeted_time = 0.0f;
                        //last_targeted_ref = nullptr;
                        result = true;
                    }
                    else
                        have_door_targeted_time += dtime;
                }
                else
                {
                    last_targeted_ref = targeted_ref;
                    have_door_targeted_time = 0.0f;
                }
            }
            else
            {
                last_targeted_ref = nullptr;
                have_door_targeted_time = 0.0f;
            }
        }


        return result;

    }





    std::string test_about_to_be_blocked_by_blocking(float dtime, RE::TESObjectREFR* ignore_ref = nullptr)
    {
        std::string result = "";

       // if (ignore_closed_doors_time > 0)
       // {
       //     ignore_closed_doors_time -= dtime;
       //     return result;
       // }


        auto camera = RE::PlayerCamera::GetSingleton();
        auto camera_dir = camera->cameraRoot.get()->world.rotate;
        auto camera_pos = camera->pos;

        auto targeted_ref = MiscThings::GetRaycastRef(camera_pos, camera_dir.GetVectorY(), 300.0f, nullptr);
        if (targeted_ref && targeted_ref != ignore_ref)// && (targeted_ref != target_ref || quest_mode)) //not nice
        {
            std::string blocking_name = MiscThings::get_blocking_object_name2(targeted_ref);

            if (blocking_name != "" && !turning_around)
            {
                
                if (last_blocking_targeted_ref == targeted_ref)
                {
                    if (have_blocking_targeted_time > 0.5f)//0.15f)
                    {
                        //have_door_targeted_time = 0.0f;
                        //last_targeted_ref = nullptr;
                        result = blocking_name;
                    }
                    else
                        have_blocking_targeted_time += dtime;
                }
                else
                {
                    last_blocking_targeted_ref = targeted_ref;
                    have_blocking_targeted_time = 0.0f;
                }
            }
            else
            {
                last_blocking_targeted_ref = nullptr;
                have_blocking_targeted_time = 0.0f;
            }
        }


        return result;

    }




    std::pair<bool, std::string> set_closed_door_choice(int choice)
    {
        std::pair<bool, std::string> result{};

        if (!door_is_closed_request_sent && !confirming_closed_door_interaction)
        {
            result.first = true;
            result.second = "[Error]";
            return result;
        }


        if (choice == 1 || choice == 0)
        {
            door_is_closed_choice = choice;
            door_is_closed_choice_valid = true;
            result.first = true;
            result.second = "[Processing...]";
        }
        else
        {
            result.first = false;
            result.second = "Invalid choice";
        }

        return result;
    }



    struct myPrimitive
    {
        uint32_t unk1;
        uint32_t unk2;
        uint32_t unk3;
        RE::NiPoint3 bounds;
    };


    void cut_navmesh_on_target(RE::TESObjectREFR* target)
    {
        std::vector<RE::TESObjectREFR*> dont_cut_list =
        {
            (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x361ee)
        };

        for (auto dont_cut : dont_cut_list)
        {
            if (target == dont_cut)
                return;
        }


        auto avoidsphere_form = RE::TESObjectREFR::LookupByID(0x7002dbf);
        if (avoidsphere_form)
        {
            auto avoidsphere_refr = avoidsphere_form->AsReference();
            if (avoidsphere_refr)
            {
                
                //avoidsphere_refr->SetScale(1.0f);

                auto target_bounds = target->GetBoundMax() - target->GetBoundMin();
                auto avoidsphere_bounds = avoidsphere_refr->GetBoundMax() - avoidsphere_refr->GetBoundMin();
                


                target_bounds.z = 0.0f;
                avoidsphere_bounds.z = 0.0f;

                float max_target_bound = std::max(target_bounds.x, target_bounds.y);

                float new_scale = max_target_bound / avoidsphere_bounds.x;

                
                auto extralist = &avoidsphere_refr->extraList;

                auto extra_primitive = (RE::ExtraPrimitive*)extralist->GetByType(RE::ExtraDataType::kPrimitive);

                if (extra_primitive)
                {
                    myPrimitive* primitive = (myPrimitive*)(extra_primitive->primitive);

                    primitive->bounds.x = max_target_bound / 2.0f;
                    primitive->bounds.y = max_target_bound / 2.0f;
                    primitive->bounds.z = max_target_bound / 2.0f;
                }

                avoidsphere_refr->MoveTo(target);
                
            }
                
        }
    }


    void remove_navmesh_cutter()
    {
        //'' (04026b75)
        auto avoidsphere_form = RE::TESObjectREFR::LookupByID(0x7002dbf);
        auto qasmoke_chest_form = RE::TESObjectREFR::LookupByID(0x4026b75);

        if (!avoidsphere_form || !qasmoke_chest_form)
            return;

        auto avoidsphere_refr = avoidsphere_form->AsReference();
        auto qasmoke_chest_refr = qasmoke_chest_form->AsReference();

        if (!avoidsphere_refr || !qasmoke_chest_refr)
            return;

        avoidsphere_refr->MoveTo(qasmoke_chest_refr);
    }

    bool is_targeted_door_closed()
    {
        bool result = false;

        auto target_refr = get_targeted_ref();

        if (target_refr)
        {


            auto door_refr = (RE::TESObjectDOOR*)target_refr;

            if (door_refr->GetOpenState(target_refr) == RE::BGSOpenCloseForm::OPEN_STATE::kClosed)
                result = true;
        }

        return result;

    }
    bool is_targeted_door_locked()
    {
        bool result = false;

        auto target_refr = get_targeted_ref();

        if (target_refr)
        {
            result = MiscThings::is_door_locked(target_refr);
        }
        
        return result;
    }

   
    bool wiggle_camera(float dtime)
    {
        bool result = false;

        if (wiggle_camera_time > 2.0f)
            result = true;
        else
        {
            wiggle_camera_time += dtime;

            float radius = wiggle_camera_time*12;
            float x = 8*radius*cos(wiggle_camera_time*2.0f * std::numbers::pi);
            float y = 2*radius*sin(wiggle_camera_time * std::numbers::pi); //was 1* before, revert if bad

            mouse_look(x, y);
            //mouse_mouse_x_y(x, y);
        }

        return result;
    }




    bool try_to_move_obstacle(float dtime)
    {
        bool result = false;

        auto target_ref = get_targeted_ref();

        if (target_ref || flicking)
        {
            if (flick_time < 0.7f)
            {
                flicking = true;
                flick_time += dtime;
                carry();
                //mouse_mouse_x_y(40.0f, -10.0f);
                mouse_look(40.0f, -10.0f);
            }
            else
            {
                if (flick_time < 0.85f)
                {
                    flick_time += dtime;
                    uncarry(); //give a bit of time to release the object
                }
                else
                {
                    //done
                    flicking = false;
                    result = true;
                    flick_time += dtime;
                }
            }
                
                

        }
        else
        {
            flicking = false;
            flick_time = 0.0f;
            uncarry();
            result = true; //nothing to move
        }
            




        return result;

    }



    //float arc_coef1 = 0.0001f;
    //float arc_coef2 = 0.25f;
    //float arc_coef3 = 0.900f;

    float arc_coef1 = 0.000015453617065005; //0.00002; //0.000025
    float arc_coef2 = -0.038823324136838;    //-0.06; //-0.1
    float arc_coef3 = 2759.74896969697; //arrow speed



    /*
    
    arrow trajectory: v*t
    target trajectory: u*t

    arrow source: p0
    target source: p1


    p1 + ut = p0 + vt

    t = (p1 - p0)/(v - u)

    px = p1 + u(p1 - p0)/(v - u)

    p1 + X = px

    X = px - p1
    X = p1 + u(p1 - p0)/(v - u) - p1 = u(p1 - p0)/(v - u)

    v = 2759.74896969697
    u = (pos - last_pos)/dtime
    p1 - target_current_pos
    p0 - actor_camera_current_pos
    
    

    p1 - p0 + ut = vt




    */




    //-0.000015453617065005*x*x + 0.038823324136838*x



    RE::NiPoint3 get_estimate_aim_pos(RE::TESObjectREFR* target, bool sit_correction)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (!target || !player)
            return RE::NiPoint3::Zero();


        //auto height = target->GetHeight();

        auto bounds_max = target->GetBoundMax(); //looks like this is better than height
        auto bounds_min = target->GetBoundMin();

        RE::NiPoint3 bound_dif = bounds_max - bounds_min;

        auto height = bound_dif.z;
        if (height > 270.0f)
            height = 270.0f;



        auto target_center = target->GetPosition();
        auto specific_shift = MiscThings::get_looking_point_shift(target, false);

        auto camera = RE::PlayerCamera::GetSingleton();
        auto camera_dir = camera->cameraRoot.get()->world.rotate;
        auto camera_pos = camera->pos;

        bool lookat_used = false;


        if (target->IsHumanoid())// && !target->IsDead())
        {
            auto target_actor = (RE::Actor*)target;

            if (target_actor->currentProcess)
                if (target_actor->currentProcess->middleHigh)
                    if (target_actor->currentProcess->middleHigh->headNode)
                    {
                        auto head_pos = target_actor->currentProcess->middleHigh->headNode->world.translate;

                        auto lookat_location = head_pos;

                        auto player_temp_pos = player->GetPosition();

                        //if (sit_correction)
                        if (target_actor->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSitting && !MiscThings::is_intro())
                            lookat_location.z -= 30.0f;

                        target_center = lookat_location;


                        height = 0.0f;
                        specific_shift = RE::NiPoint3::Zero();

                        lookat_used = true;
                    }

        }
        else
        {
            if (target->IsActor())// && !target->IsDead())
            {
                auto target_actor = (RE::Actor*)target;

                if (MiscThings::is_enemy_to_actor(target))
                {
                    if (target_actor->currentProcess)
                        if (target_actor->currentProcess->middleHigh)
                            if (target_actor->currentProcess->middleHigh->headNode)
                            {
                                auto torso_pos = target_actor->currentProcess->middleHigh->torsoNode->world.translate;

                                auto lookat_location = torso_pos;

                                if (MiscThings::is_dragon(target))
                                    torso_pos.z += 100.0f;


                                auto player_temp_pos = player->GetPosition();

                                target_center = lookat_location;
                                height = 0.0f;
                                specific_shift = RE::NiPoint3::Zero();

                                lookat_used = true;
                            }
                }
                else
                {
                    //if they are friendly - try head.. surely they have head..
                    if (target_actor->currentProcess)
                        if (target_actor->currentProcess->middleHigh)
                            if (target_actor->currentProcess->middleHigh->headNode)
                            {
                                auto head_pos = target_actor->currentProcess->middleHigh->headNode->world.translate;

                                auto lookat_location = head_pos;

                                auto player_temp_pos = player->GetPosition();

                                //if (sit_correction)
                                if (target_actor->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSitting && !MiscThings::is_intro())
                                    lookat_location.z -= 30.0f;

                                target_center = lookat_location;


                                height = 0.0f;
                                specific_shift = RE::NiPoint3::Zero();

                                lookat_used = true;
                            }
                }
            }
        }

        



        if (specific_shift != RE::NiPoint3::Zero())
        {
            dont_use_bounds_for_close_enough = true;
            target_center += specific_shift;
        }


        auto path_point_pos = target_center;




        if (target)
        {
            if (!lookat_used)
            {
                if (specific_shift != RE::NiPoint3::Zero())
                {
                    ;// target_center += specific_shift; //dont manipulate height in this case
                }
                else
                {
                    if (target->IsActor())
                    {
                        if (!target->IsDead())
                            target_center.z += height * 0.8f;
                        //for dead bodies add nothing. there is something wrong with height
                    }
                    else
                        target_center.z += height * 0.5f;


                    if (player)
                    {

                        if (target_center.z > player->GetHeight() * 1.5 + player->GetPosition().z)
                        {
                            //if (get_targeted_ref() == target_ref)
                            target_center.z -= player->GetHeight() * 0.5;// return true; //in case its some very tall but thin object and moving camera too high will ruin the focus
                        }

                    }
                }
            }
        }

        return target_center;
    }


    RE::NiPoint3 myGetVectorX(RE::NiMatrix3 matrix)
    {
        return RE::NiPoint3{ matrix.entry[0][0], matrix.entry[0][1], matrix.entry[0][2] };
    }

    RE::NiPoint3 myGetVectorY(RE::NiMatrix3 matrix)
    {
        return RE::NiPoint3{ matrix.entry[1][0], matrix.entry[1][1], matrix.entry[1][2] };
    }

    RE::NiPoint3 myGetVectorZ(RE::NiMatrix3 matrix)
    {
        return RE::NiPoint3{ matrix.entry[2][0], matrix.entry[2][1], matrix.entry[2][2] };
    }
    





    int debug_mode_cam = 0;


    bool lock_camera_onto_target(RE::TESObjectREFR* target, float dtime, bool slow)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        if (!target || !player)
            return false;

        
        //auto height = target->GetHeight();

        auto bounds_max = target->GetBoundMax(); //looks like this is better than height
        auto bounds_min = target->GetBoundMin();

        RE::NiPoint3 bound_dif = bounds_max - bounds_min;

        auto height = bound_dif.z;
        if (height > 270.0f)
            height = 270.0f;



        auto target_center = target->GetPosition();
        auto specific_shift = MiscThings::get_looking_point_shift(target, false);

        auto camera = RE::PlayerCamera::GetSingleton();
        auto camera_dir = camera->cameraRoot.get()->world.rotate;
        auto camera_pos = camera->pos;

        bool lookat_used = false;

        
        if (target->IsHumanoid())// && !target->IsDead())
        {
            auto target_actor = (RE::Actor*)target;
            //auto lookat_location = target_actor->GetLookingAtLocation(); //THIS IS ACTUALLY WHERE THEY LOOK AT AND NOT FROM WHERE. its from where until they have something to look at
           
            auto target_3d = target_actor->Get3D();

            auto biped = target_actor->GetBiped();
            auto test2 = target_actor->Get3D();
            auto test3 = target_actor->GetFaceNode();

            

            if (target_actor->currentProcess)
                if (target_actor->currentProcess->middleHigh)
                {
                    if (is_pickpocketing())
                    {
                        if (target_actor->currentProcess->middleHigh->torsoNode)
                        {
                            auto head_pos = target_actor->currentProcess->middleHigh->torsoNode->world.translate;

                            auto lookat_location = head_pos;

                            auto player_temp_pos = player->GetPosition();

                            if (target_actor->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSitting && !MiscThings::is_intro())
                                lookat_location.z -= 30.0f;

                            target_center = lookat_location;
                            height = 0.0f;
                            specific_shift = RE::NiPoint3::Zero();

                            lookat_used = true;
                        }
                    }
                    else
                        if (target_actor->currentProcess->middleHigh->headNode)
                        {
                            auto head_pos = target_actor->currentProcess->middleHigh->headNode->world.translate;

                            auto lookat_location = head_pos;

                            auto player_temp_pos = player->GetPosition();

                            if (target_actor->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSitting && !MiscThings::is_intro())
                                lookat_location.z -= 30.0f;

                            target_center = lookat_location;
                            height = 0.0f;
                            specific_shift = RE::NiPoint3::Zero();

                            lookat_used = true;
                        }
                }


            /*
            if (biped)
            {
                if (biped->root && std::size(biped->root->children) > 0 && biped->root->children[0])
                {

                    float root_shift = bound_dif.z * 0.4;
                    auto root_pos = biped->root->children[0]->world.translate;
                    root_pos.z += root_shift;

                    auto lookat_location = root_pos;

                    auto player_temp_pos = player->GetPosition();

                    target_center = lookat_location;
                    height = 0.0f;
                    specific_shift = RE::NiPoint3::Zero();

                    lookat_used = true;
                }
                else
                    bool breakpoint123 = false;
            }
            else
                bool breakpoint123 = false;
               */ 
                    
        }
        else
        {

            if (target->IsActor())// && !target->IsDead())
            {
                auto target_actor = (RE::Actor*)target;

                if (MiscThings::is_enemy_to_actor(target))
                {
                    if (target_actor->currentProcess)
                        if (target_actor->currentProcess->middleHigh)
                            if (target_actor->currentProcess->middleHigh->headNode)
                            {
                                auto torso_pos = target_actor->currentProcess->middleHigh->torsoNode->world.translate;

                                auto lookat_location = torso_pos;

                                if (MiscThings::is_dragon(target))
                                    torso_pos.z += 100.0f;


                                auto player_temp_pos = player->GetPosition();

                                target_center = lookat_location;
                                height = 0.0f;
                                specific_shift = RE::NiPoint3::Zero();

                                lookat_used = true;
                            }
                }
                else
                {
                    if (target_actor->currentProcess->middleHigh->headNode)
                    {
                        auto head_pos = target_actor->currentProcess->middleHigh->headNode->world.translate;

                        auto lookat_location = head_pos;

                        auto player_temp_pos = player->GetPosition();

                        if (target_actor->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSitting && !MiscThings::is_intro())
                            lookat_location.z -= 30.0f;

                        target_center = lookat_location;
                        height = 0.0f;
                        specific_shift = RE::NiPoint3::Zero();

                        lookat_used = true;
                    }
                }
            }
        }
        




        
        if (specific_shift != RE::NiPoint3::Zero())
        {
            dont_use_bounds_for_close_enough = true;
            target_center += specific_shift;
        }
        

        bool high_precision = false;

        auto path_point_pos = target_center;
        auto pos_dif = path_point_pos - camera_pos;

        if (interaction_after_walk == 3 && start_attacking && has_ranged_weapon_equipped(true))
        {
            //assuming its always pulled to the max

            long long now = std::chrono::steady_clock::now().time_since_epoch().count();

            float dtime_better = 0.016f;

            if (lasttime > 0)
            {
                dtime_better = (double)(now - lasttime) / 1000000000.0;
            }
            

            lasttime = now;



            if (last_target_pos_valid)
            {
                /*
                    v = 2759.74896969697
                    u = (pos - last_pos)/dtime
                    p1 - target_current_pos
                    p0 - actor_camera_current_pos

                    X = u(p1 - p0)/(v - u)

                    X = u*t

                    t???

                    t ~= distance/arc_coef3
                */

                auto delta_target_pos = path_point_pos - last_target_pos;
                delta_target_pos.z = 0.0f;

                auto pos_dif_copy = pos_dif;
                pos_dif_copy.z = 0.0f;

                RE::NiPoint3 u;

                //RE::NiPoint3 v = pos_dif / pos_dif.Length() *arc_coef3;
                if (last_u_valid)
                {
                    auto u_new = delta_target_pos / dtime_better;
                    auto delta_new = u_new - last_u;
                    delta_new *= 0.3;
                    u = last_u + delta_new;
                    last_u = u;
                }
                else
                {
                    u = (delta_target_pos / dtime_better);
                    last_u = u;
                    last_u_valid = true;
                }


                RE::NiPoint3 speed_shift = u * (pos_dif_copy.Length() / get_weapon_projectile_speed(get_current_active_hand())); //something is wrong. gives too little on low speeds and too much on high speeds (maybe illusion)

                    //arc_coef3 * delta_target_pos * distance;
                target_center += speed_shift;
            }


            //auto distance = pos_dif.Length();
            auto distance = (target_center - camera_pos).Length();
            //float arc_shift = arc_coef1*(arc_coef2 * distance * distance - arc_coef3 * distance);
            float arc_shift = arc_coef1 * distance * distance + arc_coef2 * distance;


            target_center.z += arc_shift;


            //target_center.z -= 7.77f;

            high_precision = true;

        }

        last_target_pos_valid = true;
        last_target_pos = path_point_pos;
        


        //auto path_point_pos = path[current_path_point];
        path_point_pos = target_center;
        pos_dif = path_point_pos - camera_pos;
        auto pos_dif_norm = pos_dif / pos_dif.Length();

        auto camera_dirX = camera_dir.GetVectorX();
        auto camera_dirY = camera_dir.GetVectorY();
        auto camera_dirZ = camera_dir.GetVectorZ();

        RE::NiPoint3 camera_dirY_fixed = camera_dirY;// { -camera_dirY.z, camera_dirY.y, camera_dirY.x };

        //auto dir_dif_X = pos_dif_norm - camera_dir_2;

        float mouse_x = 0.0f;
        float mouse_y = 0.0f;




        auto mulX = camera_dirX * pos_dif_norm;
        auto mulY = 0.0f;
        auto mulZ = 0.0f;


        if (target)
        {

            //RE::hkVector4 a_pos{};
            //RE::NiTransform a_transform{};
            //auto old_pos = target_ref->GetPosition();

            if (!lookat_used)
            {
                if (specific_shift != RE::NiPoint3::Zero())
                {
                    ;// target_center += specific_shift; //dont manipulate height in this case
                }
                else
                {
                    if (target->IsActor())
                    {
                        if (!target->IsDead())
                            target_center.z += height * 0.8f;
                        //for dead bodies add nothing. there is something wrong with height
                    }
                    else
                        target_center.z += height * 0.5f;


                    if (player)
                    {
                        if (target_center.z < (player->GetHeight() * 0.25 + player->GetPosition().z) && !(target->IsActor() && target->IsDead()))
                            if (!stop_sneaking && !player->IsSneaking() && !using_custom_path && !location_mode && !(quest_mode && target->IsActor() && !target->IsDead()))
                            {
                                lock_camera_wants_to_crouch = true;
                                crouch(); //if target is very low - sneak on it
                            }
                            else
                                lock_camera_wants_to_crouch = false;


                        if (target_center.z > player->GetHeight() * 1.5 + player->GetPosition().z)
                        {
                            //if (get_targeted_ref() == target_ref)
                            target_center.z -= player->GetHeight() * 0.5;// return true; //in case its some very tall but thin object and moving camera too high will ruin the focus
                        }

                    }
                }
            }


            


            
            

            //auto shift = target_ref->GetScale()

            pos_dif = target_center - camera_pos;
            pos_dif_norm = pos_dif / pos_dif.Length();


            //////////////////////////////////////
            //////////////////////////////////////
            //////////////////////////////////////

            //auto testX = myGetVectorX(camera_dir);
            //auto testY = myGetVectorY(camera_dir);
            //auto testZ = myGetVectorZ(camera_dir);

            auto crossX = camera_dirX.Cross(pos_dif_norm);
            auto crossY = camera_dirY.Cross(pos_dif_norm);
            auto crossZ = camera_dirZ.Cross(pos_dif_norm);

            auto dotX = camera_dirX.Dot(pos_dif_norm);
            auto dotY = camera_dirY.Dot(pos_dif_norm);
            auto dotZ = camera_dirZ.Dot(pos_dif_norm);


           // send_random_context(std::to_string(dotX) + ", " + std::to_string(dotY) + ", " + std::to_string(dotZ));
            //send_random_context(std::to_string(crossX.Length()) + ", " + std::to_string(crossY.Length()) + ", " + std::to_string(crossZ.Length()));
            //send_random_context(std::to_string(crossX.x) + ", " + std::to_string(crossY.y) + ", " + std::to_string(crossZ.z));

           // return false;

            //////////////////////////////////////
            //////////////////////////////////////
            //////////////////////////////////////



            mulX = camera_dirX * pos_dif_norm;
            mulZ = camera_dirZ * pos_dif_norm;
            pos_dif_norm.z = 0.0f;
            mulY = camera_dirY * pos_dif_norm;









            if (mulY < 0)
            {
                mulZ = 0.0f;// -mulZ;
                if (mulX < 0.0f)
                    mulX = -0.3f + mulX;
                else
                    mulX = 0.3f + mulX;
            }
                


            //return false;
        }


        float coef = 400.0f;

        if (slow)
            coef = 100.0f;

        if (MiscThings::is_intro())
        {
            mouse_x = mulX * 50.0f;
            mouse_y = mulZ * 50.0f;
        }
        else
        {
            mouse_x = mulX * coef;
            mouse_y = mulZ * coef;
        }



        if (abs(mouse_x) > 100.0f)
        {
            if (mouse_x < 0.0f)
                mouse_x = -100.0f;
            else
                mouse_x = 100.0f;
        }

        if (abs(mouse_y) > 50.0f)
        {
            if (mouse_y < 0.0f)
                mouse_y = -50.0f;
            else
                mouse_y = 50.0f;
        }


        if (!high_precision)
        {
            if (abs(mouse_y) < 3.0f && abs(mouse_y) >= 1.0f)
            {
                if (mouse_y < 0.0f)
                    mouse_y = -3.0f;
                else
                    mouse_y = 3.0f;
            }

            if (abs(mouse_x) < 3.0f && abs(mouse_x) >= 1.0f)
            {
                if (mouse_x < 0.0f)
                    mouse_x = -3.0f;
                else
                    mouse_x = 3.0f;
            }
        }



        /*
        if (pos_dif.Length() < 200.0f)
        {
            if (abs(mouse_x) > 10.0f)
            {
                if (mouse_x < 0.0f)
                    mouse_x = -10.0f;
                else
                    mouse_x = 10.0f;
            }
        }
        */

        //mouse_mouse_x(mouse_x);


        if (MiscThings::is_insect(target_ref))
        {
            auto distance_insect = target_ref->GetPosition() - player->GetPosition();
            distance_insect.z = 0.0f;

            if (distance_insect.Length() < 60.0f)
            {
                if (abs(mouse_x) < 5.0f && abs(mouse_y) < 5.0f)
                    return true;
            }
            else
            {
                if (abs(mulX) < 0.4 && mulY >= 0)
                    walk_forward();
            }
                

        }
        else
        {
            if (high_precision)
            {
                if (abs(mouse_x) < 1.0f && abs(mouse_y) < 1.0f)
                    return true;
            }
            else
                if (abs(mouse_x) < 5.0f && abs(mouse_y) < 5.0f)
                    return true;
        }


        mouse_look(mouse_x, -mouse_y);
        //mouse_mouse_x_y(mouse_x, -mouse_y);

        /*
        if (abs(mouse_x) < 1.0f && abs(mouse_y) >= 1.0f)
            mouse_mouse_y(-mouse_y);

        if (abs(mouse_x) >= 1.0f && abs(mouse_y) < 1.0f)
            mouse_mouse_x(mouse_x);

        if (abs(mouse_x) >= 1.0f && abs(mouse_y) >= 1.0f)
            mouse_mouse_x_y(mouse_x, -mouse_y);
*/


        if (interaction_after_walk == 2)
        {
            auto actor_obj = (RE::Actor*)target;
            bool is_walking = actor_obj->IsWalking() || actor_obj->actorState1.movingBack;

            if (wait_and_start_pickpocket)
            {
                waiting_pickpocket_time += dtime;
                if (is_walking)
                    ;// cursor_up(); //this is shit. hard to tell if they are actually walking (turning also counts as walking. this needs better filtering)
            }

        }


        return false;

    }


    




    bool path_point_reached(float dtime)
    {
        


        bool result = false;

        auto player_pos = RE::PlayerCharacter::GetSingleton()->GetPosition();

        try {

            if (use_last_point_of_last_path)
            {
                auto pos_dif = last_point_of_last_path - player_pos;
                

                if (abs(last_point_of_last_path.z - player_pos.z) < 50.0f)
                    pos_dif.z = 0.0f;

                auto distance = pos_dif.Length();

                if ((int)std::size(path) < 3)
                    result = distance < 60.0f * (1 + MiscThings::is_on_horse() * 4.0f) + 70.0f * MiscThings::is_player_swimming(); //100
                else
                    result = distance < 60.0f * (1 + MiscThings::is_on_horse() * 4.0f) + 70.0f * MiscThings::is_player_swimming(); //100

                if (last_point_of_last_path.z - player_pos.z > 200.0f)
                    result = true; //we either fell or pathfinding glitched

                //if (distance > 200.0f * (1 + MiscThings::is_on_horse() * 2.0f))
                    

                if (!using_custom_path)
                    path_point_reached_timeout += dtime;

                if (path_point_reached_timeout > 2.0f)
                {
                    return true;
                }

            }
            else
            {
                if (path_valid)
                {
                    if (std::size(path) <= 0)
                        return true;


                    if (!using_custom_path)
                        path_point_reached_timeout += dtime;

                    if (path_point_reached_timeout > 4.0f)
                    {
                        return true;
                    }




                    if (current_path_point < (int)std::size(path))
                    {
                        auto current_path_point_pos = path.at(current_path_point);

                        if (current_path_point_pos.z - player_pos.z > 200.0f)
                            result = true; //we either fell or pathfinding glitched. point is too high

                        /////////////////// EXPERIMENTAL /////////////////
                        if (!is_about_to_fall())
                        {
                            current_path_point_pos.z = 0.0f;
                            player_pos.z = 0.0f;
                        }
                        ///////////////////////////////////////////////////

                        auto distance = current_path_point_pos.GetDistance(player_pos);


                        if (using_custom_path)
                        {
                            if (ustengrev_cliff_mode && current_path_point == ((int)std::size(path) - 1))
                            {
                                ustengrev_cliff_mode = false;
                                send_random_context("You walk off the cliff's edge...");
                            }

                            current_path_point_pos.z = 0.0f;
                            player_pos.z = 0.0f;
                            distance = current_path_point_pos.GetDistance(player_pos);

                            float base_threshold = 80.0f;
                            if (karthspire_plates)
                                base_threshold = 30.0f;

                            result = distance < base_threshold + 70.0f * MiscThings::is_player_swimming(); //100
                        }
                        else
                            result = distance < (60.0f * (1 + MiscThings::is_on_horse() * 4.0f) + 70.0f*MiscThings::is_player_swimming()); //100
                    }
                    else
                        result = true;
                }
            }
        }
        catch (const std::out_of_range& e) {
            return true; //path is broken
        }


        return result;
    }




    bool is_casting_clairvoyance = false;


    void stop_casting_hand(bool right)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto left_caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
        auto right_caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);

        if (right)
        {
            right_caster->FinishCast();
        }
        else
        {
            left_caster->FinishCast();
        }
    }


    bool was_casting_clairvoyance()
    {
        return is_casting_clairvoyance;
    }



    bool is_casting_walker2(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        auto left_caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
        auto right_caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);

        if (right)
        {
            auto state = right_caster->state;
            if (state != RE::MagicCaster::State::kNone && !(state == RE::MagicCaster::State::kCasting && is_concentration_spell(right)))
                result = true;
        }
        else
        {
            auto state = left_caster->state;
            if (state != RE::MagicCaster::State::kNone && !(state == RE::MagicCaster::State::kCasting && is_concentration_spell(right)) && !MiscThings::is_intro2())
                result = true;
        }


        return result;
    }


    bool is_casting_walker(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        auto left_caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
        auto right_caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kRightHand);

        if (right)
        {
            auto state = right_caster->state;
            if (state != RE::MagicCaster::State::kNone)
                result = true;
        }
        else
        {
            auto state = left_caster->state;
            if (state != RE::MagicCaster::State::kNone && !MiscThings::is_intro2() )
                result = true;
        }


        return result;
    }


    void cast_immidiately()
    {
        //auto spell_form = RE::TESForm::LookupByID(135491);
        auto spell_form = RE::TESForm::LookupByID(0x700639b);

        if (spell_form)
        {
            if (spell_form->IsMagicItem())
            {
                auto spell = (RE::MagicItem*)spell_form;

                auto player = RE::PlayerCharacter::GetSingleton();
                if (player && player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant))
                {
                    auto caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);

                    caster->SetCurrentSpell(spell);
                    caster->state = RE::MagicCaster::State::kCasting;
                    caster->currentSpellCost = 0.0f;
                    caster->SpellCast(true, 1, spell);
                }

            }
        }
    }


    void start_casting()
    {
        //auto spell_form = RE::TESForm::LookupByID(135491);
        auto spell_form = RE::TESForm::LookupByID(0x700639b);

        if (spell_form)
        {
            if (spell_form->IsMagicItem())
            {
                auto spell = (RE::MagicItem*)spell_form;

                auto player = RE::PlayerCharacter::GetSingleton();
                if (player && player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant))
                {
                    auto caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
                    
                    caster->SetCurrentSpell(spell);
                    caster->state = RE::MagicCaster::State::kCasting;
                    caster->currentSpellCost = 0.0f;
                    caster->SpellCast(true, 1, spell);
                }

            }
        }
    }

    void stop_casting()
    {
        //000C9BA0
        auto spell_form = RE::TESForm::LookupByID(135491);
        if (spell_form)
        {
            if (spell_form->IsMagicItem())
            {
                auto spell = (RE::MagicItem*)spell_form;

                auto player = RE::PlayerCharacter::GetSingleton();
                if (player && player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant))
                {
                    auto caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);

                    //caster->SpellCast(false, 1, spell);
                    caster->FinishCast();


                    /*
                    caster->SetCurrentSpell(spell);
                    caster->state = RE::MagicCaster::State::kNone;
                    caster->currentSpellCost = 0.0f;
                    caster->SpellCast(false, 1, spell);
                    */
                }

            }
        }
    }


    float start_casting_time = 0.0f;

    float walk_timeout = 0.0f;

    bool cast_pathfinding(float dtime)
    {

        //cast_immidiately();
        //return true;

        bool result = false;

        //if (is_casting_walker(false))
        //    left_attack_cancel();

        //if ((!is_casting_walker(false) && !is_casting_input(false)) || is_casting_clairvoyance)
        //if ((!is_casting_walker(false) && !is_casting_input(false)) || is_casting_clairvoyance)
        {
            if (!is_casting_clairvoyance)
            {
                is_casting_clairvoyance = true;
                start_casting();
                start_casting_time = 0.0f;
            }
            else
            {
                //if (start_casting_time > 0.5f) //hazards spawn immidiately after start, dont need to wait.
                //{
                stop_casting();
                is_casting_clairvoyance = false;
                start_casting_time = 0.0f;
                result = true;
                //}
                //else
                //    start_casting_time += dtime;
            }
        }
        

        return result;
    }


    bool init_delay = false;


    bool backup_input_cancel = false;


    //void reset_explore_mode()
    //{
    //    explore_mode = false;
    //    min_dist = 20000.0f;
    //}



    void reset_walker()
    {
        


        if (activate_refr_after_walker_is_done)
        {
            auto refr_to_activate = activate_refr_after_walker_is_done;
            activate_refr_after_walker_is_done = nullptr;
            walk_to_object_by_refr(refr_to_activate, 1);
            return;
        }
        

        alftand_counter = 0;
        dont_check_quest_target_change = false;


        karthspire_plates = false;

        always_shift = false;

        crouch_timeout = 0.0f;

        stable_target = 0;

        silly_walk_mode = false;

        navmesh_probe_mode = false;
        navmesh_probe_result_valid = false;
        navmesh_probe_result = false;

        path_point_reached_timeout = 0.0f;

        multiple_paths_quest_choice_confirming = false;
        multiple_paths_quest_choice_confirmed = false;
        multiple_paths_quest_request_sent = false;
        multiple_paths_quest_choice_valid = false;
        multiple_paths_quest_choice = -1;
        allowed_multiple_paths_quest_choices.clear();

        pickpocket_timeout = 0.0f;

        current_quest_target_followed = nullptr;
        current_quest_followed = nullptr;

        just_teleported = false;

        no_weapons_notified = false; 

        stop_sneaking = false; 

        opening_door_attempts = 0;

        confirm_stealing = false;
        stealing_request_sent = false;
        stealing_choice_valid = false;
        stealing_choice = false;
        stealing_confirmed = false;

        stealing_timer = 0.0f;

        pause_pre_stealing = false;
        pause_pre_stealing_time = 0.0f;


        advice_counter = 0;

        special_ref_for_distance_calculation = nullptr;

        correct_word_of_power = false;


        last_paths_best_guess = RE::NiPoint3::Zero();


        backup_interaction_time = 0.0f;
        active_attacking_time = 0.0f;

        reset_after_walk = false;

        walk_again_when_finished = false;

        dont_use_bounds_for_close_enough = false;
        getting_into_carriage = false;
        getting_into_carriage_time = 0.0f;

        surrender_mode = false;
        surrender_time = 0.0f;


        do_spins = false;
        amount_of_spins = 0;
        spin_speed = 0;
        spin_step_one = false;
        amount_of_spins_done = 0;
        spin_timeout = 0.0f;


        attack_spell_cast_timeout = 0.0f;
        //if (!reset_by_explorer) //it renews explore_mode anyway
        //{
        if (!explore_mode)
            reset_explore_mode_start_range();

        explore_mode = false;
            
        last_walk_reminded_time = 0.0f;

        //}

        //reset_by_explorer = false;

        runaway_mode = false;

        was_already_dead = false;

        looking_mode = false;

        too_high_notified = false; 

        last_start_attacking_info = "";
        last_attacking_target = "";
        last_attacking_weapon = "";
        last_attacking_health = "";


        reminder_target_name = "";
        reminder_walk_time = 0;
        reminder_distance = 0;

        intro_look_timeout = 0.0f;

        custom_path_appended = false; 


        just_teleported_timeout = 0.0f;

        if (using_custom_path)// && !ustengrev_get_ready_mode && !ustengrev_shout_mode)
        {
            register_allowed_actions();
            quicksave();
        }
            

        if (!backup_input_cancel)
        {
            backup_input_cancel = true;
            right_attack_cancel();
            left_attack_cancel();
            //mouse_mouse_x_y(0.0f, 0.0f); //experimental
            mouse_look(0.0f, 0.0f);

            //perk_up_cancel();
        }

        lock_camera_wants_to_crouch = false;
        path_valid = false;
        current_path_point = -1;
        is_casting_clairvoyance = false;
        start_casting_time = 0.0f;
        init_delay = false;
        walk_timeout = 0.0f;
        have_target_to_walk = false;
        target_ref = nullptr;
        interaction_after_walk = -1;
        walk_finished_context_sent = false;
        interaction_context_sent = false;
        try_to_remove_obstacle_mode = false;
        flick_time = 0.0f;
        flicking = false;
        have_door_targeted_time = 0.0f;
        last_targeted_ref = nullptr;
        turning_around = false;
        walk_forward_a_little = false;
        walk_backward_a_little = false;
        walked_for = 0.0f;
        door_is_closed_request_sent = false;
        door_is_closed_choice_valid = false;
        door_is_closed_choice = -1;


        last_blocking_targeted_ref = nullptr;
        have_blocking_targeted_time = 0.0f;


        if (was_slowwalking)
        {
            was_slowwalking = false;
            unslow_walk();
        }
        

        wiggle_camera_time = 0.0f;
        locking_failed = false;

        confirming_closed_door_interaction = false;
        catch_door_result = false;
        catch_door_result_time = 0.0f;


        use_last_point_of_last_path = false;

        walk_retries = 0;

        ignore_closed_doors_time = 0.0f;


        wiggle_body_then_walk_again = false;

        attempts_to_move_obstacle = 0;
        move_obstacle_failed = false;

        try_unstuck = false;
        time_stuck = 0.0f;
        walk_unstuck_time = 0.0f;
        unstuck_attempts = 0;

        paused_before_interaction = false;


        make_clairvoyance_cast = false;

        tried_to_come_closer = false;

        wait_and_start_pickpocket = false;

        quest_mode = false;
        had_successful_walk = false;

        last_quest = nullptr;
        last_quest_objective = nullptr;

        using_custom_path = false;

        attacking_inanimate_object_time = 0.0f;
        start_attacking = false;
        attacking_done = false;

        ignore_fights_mode = false;

        was_sprinting = false;

        low_mana_detected = false;


        tried_to_draw_weapon1 = false;
        draw_weapon_check_time1 = 0.0f;
        tried_to_draw_weapon2 = false;
        draw_weapon_check_time2 = 0.0f;


        chill_with_context = false;

        location_mode = false;

        time_blind_walk = 0.0f;

        had_any_path_found_this_run = false;


        backup_interaction_made = false; 


        gave_attacking_info = false;

        attack_action = -1;
        attack_action_time0 = 0.0f;
        attack_action_time1 = 0.0f;

        last_target_pos = RE::NiPoint3::Zero();
        last_u = RE::NiPoint3::Zero();
        last_target_pos_valid = false;
        last_u_valid = false;
        was_charging_ranged = false;

        lasttime = 0;
        lasttime_close_enough = 0;

        low_mana_notified = false;
        low_mana_notify_time = 0.0f;

        attack_paused = false;
        attack_pause_time = 0.0f;
        got_close_for_pickpocket = false;

        search_next_fight_target = false;
        search_next_target_timer = 0.0f;

        explore_mode_notified = false;



        shout_mode = false;
        shout_to_use = nullptr;

        if (gate_shout)
            register_allowed_actions();

        gate_shout = false;

        ustengrev_get_ready_mode = false;
        ustengrev_shout_mode = false;
        ustengrev_gates_closed_time = 0.0f;
        ustengrev_finish_line_mode = false;
        ustengrev_cliff_mode = false;
        ustengrev_run_only_mode = false;
        ustengrev_wrong_order_mode = false;


        anti_slowwalk_timer = 0.0f;
        dont_tell_result = false;



        trying_to_fuckup_correct_pillar = false;
        fuckup_pillar_confirm_request_sent = false;
        fuckup_pillar_confirm_choice_valid = false;
        fuckup_pillar_confirm_choice = 0;

        trying_to_change_quest_course = false;
        change_quest_course_request_sent = false;
        change_quest_course_choice_valid = false;
        change_quest_course_choice = 0;


        dead_point_time = 0.0f;


        trying_to_attack_friend = false;
        attack_friend_request_sent = false;
        attack_friend_choice_valid = false;
        attack_friend_choice = -1;
        attack_friend_interaction = -1;
        attack_friend_id = -1;
        attack_friend_confirmed = false;
        attack_friend_name = "";

    }

    void walk_again()
    {
        //if (!using_custom_path)
        {

            stable_target = 0;

            path_point_reached_timeout = 0.0f;


            backup_interaction_time = 0.0f;

            walk_again_when_finished = false;

            too_high_notified = false;

            lock_camera_wants_to_crouch = false;
            path_valid = false;
            current_path_point = -1;
            is_casting_clairvoyance = false;
            start_casting_time = 0.0f;
            walk_timeout = 0.0f;

            if (start_attacking)
            {
                attacking_done = true;
                start_attacking = false;
            }

            anti_slowwalk_timer = 0.0f;

            time_blind_walk = 0.0f;

            last_target_pos = RE::NiPoint3::Zero();
            last_u = RE::NiPoint3::Zero();
            last_target_pos_valid = false;
            last_u_valid = false;
            //was_charging_ranged = false;


            lasttime = 0;
            lasttime_close_enough = 0;

            attack_action_time0 = 0.0f;
            attack_action_time1 = 0.0f;

            got_close_for_pickpocket = false;

            //search_next_fight_target = false;
            search_next_target_timer = 0.0f;

            if (was_slowwalking)
            {
                was_slowwalking = false;
                unslow_walk();
            }

            if (was_charging_ranged)
                cancel_charge_weapon();



        }
        //else
        //{

        //}
    }

    
    int unstuck_direction = 0;

    bool walk_unstuck(float dtime)
    {
        bool result = false;
        auto player = RE::PlayerCharacter::GetSingleton();

        if (walk_unstuck_time < 0.6f && player)
        {
            /*
            RE::NiPoint3 current_path_point_pos = path.at(current_path_point);
            RE::NiPoint3 next_point_pos = path.at(std::min(((int)std::size(path) - 1), current_path_point + 2));
            RE::NiPoint3 direction_vector = next_point_pos - current_path_point_pos;

            auto camera = RE::PlayerCamera::GetSingleton();
            auto camera_dir = camera->cameraRoot.get()->world.rotate;
            auto camera_dirX = camera_dir.GetVectorZ();

            auto direction_vector_norm = direction_vector / direction_vector.Length();

            float mul = camera_dirX * direction_vector_norm;
            
            if (mul < 0)
                left();
            else
                right();

            */

            if (unstuck_direction < 0 || unstuck_direction > 3)
                unstuck_direction = 0;

            if (unstuck_direction == 0)
            {
                left();
                cursor_down();
            }
                
            if (unstuck_direction == 1)
            {
                right();
                cursor_down();
            }

            if (unstuck_direction == 2)
            {
                left();
                cursor_up();
            }

            if (unstuck_direction == 3)
            {
                right();
                cursor_up();
            }

            walk_unstuck_time += dtime;
        }
        else
        {
            unstuck_direction++;
            result = true;
        }

        return result;
    }


    bool walk_fixed_time(bool forward, float how_long, float dtime)
    {
        bool result = false;

        if (walked_for > how_long)
        {
            walked_for = 0.0f;
            result = true;
        }
        else
        {
            walked_for += dtime;
            if (forward)
                cursor_up();
            else
                cursor_down();
        }

        return result;
    }




    bool is_container(RE::TESObjectREFR* refr)
    {
        bool result = false;

        if (refr)
            if (auto base_obj = refr->GetBaseObject(); base_obj)
                if (base_obj->GetFormType() == RE::FormType::Container)
                    result = true;


        return result;
    }

    bool is_door(RE::TESObjectREFR* refr)
    {
        bool result = false;

        if (refr)
            if (auto base_obj = refr->GetBaseObject(); base_obj)
                if (base_obj->GetFormType() == RE::FormType::Door)
                {
                    auto door_object = (RE::TESObjectDOOR*)base_obj;

                    std::string model = door_object->GetModel();

                    if (model.find("TrapHingeTrigger01") != std::string::npos)
                    {
                        return false;
                    }

                    result = true;
                }
                    


        return result;
    }


    bool is_workbench(RE::TESObjectREFR* refr)
    {
        bool result = false;

        if (refr)
            if (auto base_obj = refr->GetBaseObject(); base_obj)
                if (base_obj->GetFormType() == RE::FormType::Furniture)
                {
                    auto bench = (RE::TESFurniture*)base_obj;
                    if (bench->workBenchData.benchType != RE::TESFurniture::WorkBenchData::BenchType::kNone)
                        result = true;
                }
        return result;
    }


    




    bool target_is_too_high()
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        auto player_pos = player->GetPosition();

        if (target_ref)
        {
            auto target_pos = target_ref->GetPosition();


            auto pos_dif = target_pos - player_pos;

            bool its_high = pos_dif.z > 300.0f;

            pos_dif.z = 0.0f;

            float horizontal_max_distance = 200.0f;

            if (target_ref->IsActor())
            {
                auto actor_refr = (RE::Actor*)target_ref;
                if (actor_refr->race->fullName == "Dragon Race")
                {
                    horizontal_max_distance = 4000.0f;
                }
            }
            

            bool its_right_above = pos_dif.Length() < horizontal_max_distance;

            return its_high && its_right_above;//
        }


        return false;
    }


    float successful_raycast_time = 0.0f;

    bool close_enough()
    {
        auto player = RE::PlayerCharacter::GetSingleton();


        if (location_mode)
        {
            if (target_ref && player)
            {
                auto player_pos = player->GetPosition();
                auto target_pos = target_ref->GetPosition();

                auto distance = target_pos - player_pos;

                distance.z = 0.0f;


                //float distance = player->GetDistance(target_ref, true, true);

                if (distance.Length() < 3000.0f)
                    return true;
            }
        }


        if (!target_ref || !player)
            return true; //let it fully restart, something is very wrong


        auto player_pos = player->GetPosition();
        auto target_pos = target_ref->GetPosition();

        if (MiscThings::is_dragon(target_ref) && player_pos.GetDistance(target_pos) > 30000.0f)
        {
            reset_walker();
            return false;
        }

        if (using_custom_path)
        {
            if (custom_path_appended)
                return true;
            else
            {
                if (!custom_path.empty())
                    return player_pos.GetDistance(custom_path.at(std::size(custom_path) - 1)) < 200.0f;
                else
                    return true; //super error
            }
                
        }

        if (MiscThings::is_intro2())
        {
            auto distance = target_pos + MiscThings::get_looking_point_shift(target_ref, false) - player_pos;

            distance.z = 0.0f;
            if (distance.Length() < 80.0f)
                return true;
        }
        else
        {
            if (MiscThings::is_insect(target_ref))
            {
                auto distance_insect = target_pos - player_pos;
                distance_insect.z = 0.0f;

                return distance_insect.Length() < 200.0f;
            }



            if (interaction_after_walk == 2)
            {

                auto distance = target_pos + MiscThings::get_looking_point_shift(target_ref, true) - player_pos;
                

                distance.z = 0.0f; //dont account for height

                if (!wait_and_start_pickpocket && !got_close_for_pickpocket)
                {
                    if (distance.Length() < 70.0f)
                    {
                        interact_with_target(0.016); //just to crouch.
                        got_close_for_pickpocket = true;
                        return true;
                    }
                }
                else
                    if (distance.Length() < 160.0f)
                        return true;


            }
            else
            {
                //auto distance = target_pos + MiscThings::get_looking_point_shift(target_ref, false) - player_pos; //experimenting
                auto distance = target_pos - player_pos;


                   

                if (interaction_after_walk == 3)
                {

                    if (target_ref->IsActor() && target_ref->IsDead() && !was_already_dead)
                    {
                        if (was_charging_ranged)
                        {
                            cancel_charge_weapon();
                        }
                        return true;
                    }
                        

                    //if (has_ranged_weapon_equipped(get_current_active_hand()) || shout_mode)
                    {
                        if (gate_shout)
                            return true;

                        long long now = std::chrono::steady_clock::now().time_since_epoch().count();

                        float dtime_better = 0.016f;

                        if (lasttime_close_enough > 0)
                        {
                            dtime_better = (double)(now - lasttime_close_enough) / 1000000000.0;
                        }
                        else
                        {
                            successful_raycast_time = 1.0f;
                        }

                        lasttime_close_enough = now;

                        auto camera_pos = RE::PlayerCamera::GetSingleton()->pos;

                        if (lock_camera_wants_to_crouch)
                            camera_pos.z -= 34.0669f;


                        auto aim_pos = get_estimate_aim_pos(target_ref);

                        auto delta_pos = aim_pos - camera_pos;

                        float range = get_weapon_range(get_current_active_hand());

                        if (shout_mode)
                            range = 1200.0f;

                        if (start_attacking)
                            range = range * 1.25;


                        //this is for melee
                        if (MiscThings::is_dragon(target_ref) && MiscThings::is_flying(target_ref))// && (!(has_ranged_weapon_equipped(get_current_active_hand()) || shout_mode)))
                            range = 10000.0f;



                        auto raycast_ref = MiscThings::GetRaycastRef(camera_pos, delta_pos, range, target_ref);

                        //if (target_ref->IsActor() && target_ref->IsDead())
                        //    return true;

                        auto raycast_test = raycast_ref == target_ref;
                        bool target_visible = false;
                        if (raycast_test)
                        {
                            if (successful_raycast_time > 0.4f)
                                target_visible = true;
                            else
                                successful_raycast_time += dtime_better;
                        }
                        else
                        {
                            if (start_attacking)
                                if (successful_raycast_time < 0.2f)
                                {
                                    target_visible = false;
                                    successful_raycast_time = 0.0f;
                                }
                                else
                                {
                                    successful_raycast_time -= dtime_better;
                                    target_visible = true;
                                }
                                    
                        }
                            //successful_raycast_time = 0.0f;

                        distance = camera_pos - aim_pos;


                        if ((target_visible && distance.Length() < range) || distance.Length() < 200.0f)
                            return true;

                        bool stop_here = false;
                    }
                    //else

                    if (!(has_ranged_weapon_equipped(get_current_active_hand()) || shout_mode)) //melee only
                    {

                        auto bound_max = target_ref->GetBoundMax() * target_ref->GetScale();
                        auto bound_min = target_ref->GetBoundMin() * target_ref->GetScale();
                        auto bound_dif = bound_max - bound_min;

                        if (dont_use_bounds_for_close_enough)
                        {
                            bound_dif = RE::NiPoint3::Zero();
                        }

                        //TODO: this is very bad
                        //test if wide

                        if (bound_dif.x > 100.0f || bound_dif.y > 100.0f)
                        {
                            if (distance.Length() < (std::max(bound_dif.x, bound_dif.y) + 150.0f * (1 + MiscThings::is_on_horse() * 3.0f)))
                                return true;
                        }
                        else
                            if (distance.Length() < 150.0f * (1 + MiscThings::is_on_horse() * 3.0f))
                                return true;

                    }

                }
                else
                {

                    if (MiscThings::is_dragon(target_ref) && MiscThings::is_flying(target_ref) && (!(has_ranged_weapon_equipped(get_current_active_hand()) || shout_mode)))
                        if (MiscThings::raycastable(target_ref, 10000.0f, false))
                            return true;





                    auto distance = target_pos + MiscThings::get_looking_point_shift(target_ref, false) - player_pos; //experimenting


                    if (abs(distance.z) < 200.0f)
                        distance.z = 0.0f;


                    if (getting_into_carriage)
                    {
                        distance.z = 0.0f;
                        return distance.Length() < 50.0f;
                    }

                    auto bound_max = target_ref->GetBoundMax() * target_ref->GetScale();
                    auto bound_min = target_ref->GetBoundMin() * target_ref->GetScale();
                    auto bound_dif = bound_max - bound_min;

                    if (dont_use_bounds_for_close_enough)
                    {
                        bound_dif = RE::NiPoint3::Zero();
                    }

                    if (quest_mode)
                    {

                        if (target_ref && target_ref->IsActor() && !target_ref->IsDead())
                            if (distance.Length() < (std::max(bound_dif.x, bound_dif.y) + 50.0f * (1 + MiscThings::is_on_horse() * 3.0f)))
                                return true;


                        if (distance.Length() < 110.0f * (1 + MiscThings::is_on_horse() * 3.0f))
                            return true;
                    }
                    else
                    {
                        if (!is_door(target_ref) && (bound_dif.x > 100.0f || bound_dif.y > 100.0f * (1 + MiscThings::is_on_horse() * 3.0f)))
                        {
                            if (distance.Length() < (std::max(bound_dif.x, bound_dif.y) + 50.0f * (1 + MiscThings::is_on_horse() * 3.0f)))
                                return true;
                        }
                        else
                            if (distance.Length() < 150.0f * (1 + MiscThings::is_on_horse() * 3.0f))
                                return true;
                    }
                }
                

            }
        }

        return false;
    }



    std::string get_cant_walk_reason()
    {
        std::string result = "";

        RE::UI* ui = RE::UI::GetSingleton();

        if (ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
            result = "Cannot walk while in crafting menu!";

        if (ui->IsMenuOpen(RE::DialogueMenu::MENU_NAME))
            result = "Cannot walk while in dialogue!";

        if (ui->IsMenuOpen(RE::StatsMenu::MENU_NAME))
            result = "Cannot walk while in skill menu!";

        if (ui->IsMenuOpen(RE::LevelUpMenu::MENU_NAME))
            result = "Cannot walk while in levelup menu!";

        if (ui->IsMenuOpen(RE::TrainingMenu::MENU_NAME))
            result = "Cannot walk while in training menu!";

        if (ui->IsMenuOpen(RE::SleepWaitMenu::MENU_NAME))
            result = "Cannot walk while in sleep/wait menu!";

        if (ui->IsMenuOpen(RE::MessageBoxMenu::MENU_NAME))
            result = "Cannot walk right now, you have to make a choice first!";

        if (ui->IsMenuOpen(RE::ContainerMenu::MENU_NAME))
            result = "Cannot walk while in container menu!";

        if (ui->IsMenuOpen(RE::BarterMenu::MENU_NAME))
            result = "Cannot walk while in barter menu!";

        if (ui->IsMenuOpen(RE::BookMenu::MENU_NAME))
            result = "Cannot walk while reading!";

        if (ui->IsMenuOpen(RE::GiftMenu::MENU_NAME))
            result = "Cannot walk while in gift menu!";

        if (ui->IsMenuOpen(RE::LockpickingMenu::MENU_NAME))
            result = "Cannot walk while lockpicking!";

        if (ui->IsMenuOpen(RE::CraftingMenu::MENU_NAME))
            result = "Cannot walk while in map menu!";

        if (ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME))
            result = "Cannot walk while game is loading!";

        if (ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
            result = "Cannot walk while in main menu! Wait for the game to start";

        if (ui->IsMenuOpen(RE::RaceSexMenu::MENU_NAME))
            result = "Cannot walk while creating your character!";

        if (ui->IsMenuOpen(RE::MapMenu::MENU_NAME))
            result = "Cannot walk while in map!";


        return result;
    }



    std::pair<bool, std::string> explore_world(bool internal_call)
    {
        std::pair<bool, std::string> result{};


        if (MiscThings::is_objects_around_valid() && MiscThings::is_serving_jail())
        {
            auto p_normal_objects_around = MiscThings::get_p_objects_around();

            if (std::size(*p_normal_objects_around) > 0)
            {
                auto response = MiscThings::GetObjectsAround(-1);

                response.second = "You are in jail... you look around... " + response.second;
                return response;
            }
        }
        




        std::string interesting = MiscThings::check_very_interesting_objects();

        if (interesting != "")
        {
            result.first = true;
            result.second = interesting;
            return result;
        }

        float max_dist = 50000.0f;


        

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            reset_explore_mode_start_range();
            result.first = false;
            result.second = cant_walk_reason;
            return result;
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            reset_explore_mode_start_range();
            result.first = false;
            result.second = "You cannot walk yet";
            return result;
        }


        if (explore_mode && !internal_call)
        {
            result.first = true;
            result.second = "You keep exploring...";
            return result;
        }

        RE::TESObjectREFR* random_target = nullptr;

        while (min_dist > 100.0f && !random_target)
        {
            random_target = MiscThings::find_distant_unseen_reference(min_dist, max_dist);
            min_dist -= 200.0f;
        }
            
        if (!random_target)
        {
            reset_explore_mode_start_range();
            result.first = false;
            result.second = "Cannot find anything interesting to explore here...";
            if (MiscThings::is_interior_cell())
            {
                register_exit_dungeon();
                unregister_explore_action();
            }
                
            return result;
        }


        if (have_target_to_walk)
        {
            if (!internal_call)
                Observer::reset_threats();

            if (target_ref != random_target)
            {
                if (internal_call && explore_mode_notified)
                {
                    reset_walker();
                    result.first = false;
                    result.second = "[Finished exploring]";
                    return result;
                }
                else
                    reset_walker();
            }
            else
            {
                //min_dist = 20000.0f;
                result.first = true;
                result.second = "[You look around, searching for directions to explore...]";
                return result;
            }
        }



        right_attack_cancel();
        left_attack_cancel();


        target_ref = random_target;
        have_target_to_walk = true;
        reminder_start_pos = player->GetPosition();
        reminder_start_pos = player->GetPosition();
        reminder_target_name = "somewhere";
        explore_mode = true;

        interaction_after_walk = -1;

        result.first = true;
        if (!MiscThings::is_intro())
            result.second = "[You look around, searching for directions to explore...]";
        else
            result.second = "Cannot explore right now";

        return result;
    }




    int attacked_already_dead_in_a_row = 0;


    std::pair<bool, std::string> walk_to_object_by_index(int index, int interaction)
    {

        std::pair<bool, std::string> result{};

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            result.first = false;
            result.second = cant_walk_reason;
            return result;
        }







        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            result.first = false;
            result.second = "You cannot walk yet";
            return result;
        }

        if (index == -1)
        {
            return run_away();
        }



        if (MiscThings::is_objects_around_valid())
        {
            auto objects_around = MiscThings::get_p_objects_around();

            auto object = objects_around->find(index);

            if (object != objects_around->end())
            {

                if (!MiscThings::is_object_still_valid(object->second.object))
                {
                    result.first = false;
                    result.second = "This object doesnt exist anymore";
                    return result;
                }

                if ((!object->second.object->IsHumanoid() || (object->second.object->IsHumanoid() && object->second.object->IsDead())) && interaction == 2)
                {
                    if (object->second.object->IsInventoryObject() || MiscThings::is_stealing(object->second.object) != "")
                    {
                        interaction = 1; //they probably want to steal it. allow this
                    }
                    else
                    {
                        result.first = false;
                        result.second = "Cannot pickpocket this target";
                        return result;
                    }
                }


                if (interaction == 2 && MiscThings::is_intro2())
                {
                    result.first = false;
                    if (MiscThings::is_intro())
                        result.second = "Your hands are bound. Wait for the game to progress";
                    else
                    {
                            result.second = "Your hands are bound. You cannot interact now. ";

                            std::string bonus = ". You probably need to follow some quest right now. ";
                            auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
                            if (threshold_quest)
                                if (threshold_quest->GetCurrentStageID() > 200)
                                    bonus = ". You need someone to help you untie your hands. Try walking to someone. ";

                            result.second += bonus;
                    }
                    return result;
                }

                if (interaction == 3 && MiscThings::is_intro2())
                {
                    result.first = false;
                    if (MiscThings::is_intro())
                        result.second = "Your hands are bound. Wait for the game to progress";
                    else
                    {
                        result.second = "Your hands are bound. You cannot attack anything now. ";

                        std::string bonus = ". You probably need to follow some quest right now. ";
                        auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
                        if (threshold_quest)
                            if (threshold_quest->GetCurrentStageID() > 200)
                                bonus = ". You need someone to help you untie your hands. Try walking to someone. ";

                        result.second += bonus;
                    }
                    return result;
                }

                
                if (interaction == 1 && MiscThings::is_intro2())
                {
                    /*
                    result.first = false;
                    if (MiscThings::is_intro())
                        result.second = "Your hands are bound. Wait for the game to progress before interacting. Looking at the target instead";
                    else
                        result.second = "Your hands are bound. You cannot interact now. Probably its better to follow some quest right now. Looking at the target instead ";
                    return result;
                    */

                    result.first = false;
                    if (MiscThings::is_intro())
                        send_random_context("Your hands are bound. Wait for the game to progress before interacting. Looking at the target instead", false);
                    else
                    {
                        std::string message = "Your hands are bound. You cannot interact now. ";

                        std::string bonus = ". You probably need to follow some quest right now. ";
                        auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
                        if (threshold_quest)
                            if (threshold_quest->GetCurrentStageID() > 200)
                                bonus = ". You need someone to help you untie your hands. Try walking to someone. ";

                        message += bonus;
                        message += ". Looking at the target instead";

                        send_random_context(message, false);
                    }
                        
                    //return result;
                    interaction = 0;

                }
                
                if (interaction == 3 && is_fighting() && target_ref->IsActor() && !target_ref->IsDead() && (!object->second.object->IsActor() || object->second.object->IsDead() || !MiscThings::is_enemy_to_actor(object->second.object)))
                {
                    //already fighting and current target isnt dead, but new target is dead or isnt an actor

                    std::string reason = "";
                    if (object->second.object->IsActor() && object->second.object->IsDead())
                    {
                        reason = ". New target is already dead!";
                    }

                    if (!object->second.object->IsActor())
                    {
                        reason = ". This object isnt a threat";
                    }


                    if (object->second.object->IsActor() && !MiscThings::is_enemy_to_actor(object->second.object))
                    {
                        reason = ". This object is your ally";
                    }

                    result.first = false;
                    result.second = "You are in the middle of a fight" + reason;
                    return result;
                }


                if (!attack_friend_confirmed && (interaction == 3 && MiscThings::is_friend(object->second.object) && (object->second.object->IsDragon() || object->second.object->IsHumanoid())))
                {
                    trying_to_attack_friend = true;
                    attack_friend_interaction = interaction;
                    attack_friend_id = index;
                    attack_friend_name = MiscThings::insert_object_into_list_and_get_info(object->second.object);

                    //dont accept right away
                    result.first = true;
                    result.second = "[Processing...]";
                    return result;
                }
                else
                    attack_friend_confirmed = false;




                if (have_target_to_walk)
                {
                    Observer::reset_threats();

                    if (target_ref != object->second.object || interaction != interaction_after_walk)
                        reset_walker();
                    else
                    {
                        result.first = true;
                        result.second = "You keep walking...";
                        return result;
                    }
                }
                    


                right_attack_cancel();
                left_attack_cancel();


                if (MiscThings::is_on_horse())
                {
                    RE::ActorPtr mount_ptr;
                    player_actor->GetMount(mount_ptr);
                    std::string mount_name = "Horse";
                    if (mount_ptr)
                    {
                        RE::TESObjectREFR* mount_refr = (RE::TESObjectREFR*)mount_ptr.get();

                        if (mount_refr == object->second.object)
                        {
                            if (interaction == 1)
                            {
                                mount_name = MiscThings::insert_object_into_list_and_get_info(mount_refr);
                                result.first = true;
                                result.second = "[Getting off " + mount_name + "...]";
                                confirm();
                            }
                            else
                            {
                                result.first = false;
                                result.second = "[Can not do this to your mount: " + mount_name + "...]";
                            }

                            return result;
                        }
                    }
                }



                auto word_of_power = MiscThings::get_word_of_power(object->second.object);

                if (word_of_power && word_of_power != (RE::TESObjectREFR*)(-1))
                {
                    dont_tell_result = true;

                    auto word_of_power_strict = MiscThings::get_word_of_power(object->second.object, true);

                    if (word_of_power_strict && word_of_power_strict != (RE::TESObjectREFR*)(-1))
                    {
                        target_ref = word_of_power_strict;
                    }
                    else
                    {
                        correct_word_of_power = true;
                        target_ref = object->second.object;
                        dont_use_bounds_for_close_enough = true; //its a massive sphere
                    }

                }
                else
                    target_ref = object->second.object;



                float silly_walk_chance = (float)std::rand() / RAND_MAX;

                if (silly_walk_chance < 0.1)
                    silly_walk_mode = true;



                

                have_target_to_walk = true;

                if (just_escaped_solitude_prison())
                {
                    //solitude_prison_exit
                    unregister_all_actions();
                    using_custom_path = true;
                    walk_again_when_finished = true;
                    custom_path = CustomWalkerPaths::solitude_prison_exit;
                }


                reminder_start_pos = player->GetPosition();
                reminder_start_pos = player->GetPosition();
                reminder_target_name = MiscThings::insert_object_into_list_and_get_info(object->second.object);


                if (interaction > 0 && interaction < 4)
                {
                    interaction_after_walk = interaction;

                    if (interaction_after_walk == 3 && target_ref->IsActor() && target_ref->IsDead())
                        was_already_dead = true;
                }
                else
                    interaction_after_walk = -1;
                

                if (was_already_dead)
                    attacked_already_dead_in_a_row++;
                else
                    if (interaction_after_walk == 3 && target_ref->IsActor())
                        attacked_already_dead_in_a_row = 0;

                if (attacked_already_dead_in_a_row > 3 && target_ref->IsActor())
                {
                    send_random_context("It might be pointless attacking those who are already dead. Dead bodies are marked with [DEAD] tag");
                }
                    


                if (interaction_after_walk == 1 && MiscThings::is_pillar_solved(target_ref))
                    trying_to_fuckup_correct_pillar = true;

                result.first = true;
                if (!MiscThings::is_intro())
                    result.second = "[Started walking to " + reminder_target_name + "...]";
                else
                    result.second = "Cannot walk right now. Looking at target instead";

                return result;
            }
            else
                reset_walker();

        }

        result.first = false;
        result.second = "Invalid object ID"; //TODO more info

        return result;
    }



    std::pair<bool, std::string> look_at_object_by_index(int index)
    {

        std::pair<bool, std::string> result{};

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            result.first = false;
            result.second = cant_walk_reason;
            return result;
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            result.first = false;
            result.second = "You cannot look yet";
            return result;
        }


        if (MiscThings::is_objects_around_valid())
        {
            auto objects_around = MiscThings::get_p_objects_around();

            auto object = objects_around->find(index);

            if (object != objects_around->end())
            {
                

                if (have_target_to_walk)
                {
                    Observer::reset_threats();
                    if (target_ref != object->second.object)
                        reset_walker();
                    else
                    {
                        result.first = true;
                        result.second = "You keep looking...";
                        return result;
                    }
                }



                right_attack_cancel();
                left_attack_cancel();



                target_ref = object->second.object;

                have_target_to_walk = true;

                reminder_start_pos = player->GetPosition();
                reminder_start_pos = player->GetPosition();
                reminder_target_name = MiscThings::insert_object_into_list_and_get_info(object->second.object);

                interaction_after_walk = -1;

                looking_mode = true;

                result.first = true;
                if (!MiscThings::is_intro())
                    result.second = "[Looking at " + reminder_target_name + "...]";
                else
                    result.second = "[Looking at " + reminder_target_name + "...]";

                return result;
            }
            else
                reset_walker();

        }

        result.first = false;
        result.second = "Invalid object ID"; //TODO more info

        return result;
    }


    std::pair<bool, std::string> look_at_object_by_refr(RE::TESObjectREFR* object)
    {

        std::pair<bool, std::string> result{};

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            result.first = false;
            result.second = cant_walk_reason;
            return result;
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            result.first = false;
            result.second = "You cannot look yet";
            return result;
        }


            if (object)
            {

                if (have_target_to_walk)
                {
                    Observer::reset_threats();
                    if (target_ref != object)
                        reset_walker();
                    else
                    {
                        result.first = true;
                        result.second = "You keep looking...";
                        return result;
                    }
                }



                right_attack_cancel();
                left_attack_cancel();



                target_ref = object;

                have_target_to_walk = true;

                reminder_start_pos = player->GetPosition();
                reminder_start_pos = player->GetPosition();
                reminder_target_name = MiscThings::insert_object_into_list_and_get_info(object);

                interaction_after_walk = -1;

                looking_mode = true;

                result.first = true;
                if (!MiscThings::is_intro())
                    result.second = "[Looking at " + reminder_target_name + "...]";
                else
                    result.second = "[Looking at " + reminder_target_name + "...]";

                return result;
            }


        result.first = false;
        result.second = "Invalid object ID"; //TODO more info

        return result;
    }






    std::pair<bool, std::string> walk_to_location_by_index(int index)
    {
        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();

        
        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            result.first = false;
            result.second = "You cannot walk yet";
            return result;
        }

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            result.first = false;
            result.second = cant_walk_reason;
            return result;
        }


        if (!MiscThings::is_locations_around_valid())
        {
            auto get_locations_result = MiscThings::get_locations_around();
            send_random_context("Locations around you: " + get_locations_result.second);
        }


        if (MiscThings::is_locations_around_valid())
        {
            auto locations_list = MiscThings::get_p_locations_around();

            if (index < std::size(*locations_list) && index >= 0)
            {
                auto location = locations_list->at(index);

                
                auto player_pos = player->GetPosition();
                auto test_z_dif = player_pos.z - location->GetPosition().z;

                if (location)
                {
                    if (have_target_to_walk)
                    {
                        if (target_ref == location)
                        {
                            result.first = true;
                            result.second = "You keep walking...";
                            return result;
                        }

                        Observer::reset_threats();

                        reset_walker();
                    }
                        

                    if (just_escaped_solitude_prison())
                    {
                        //solitude_prison_exit
                        unregister_all_actions();
                        using_custom_path = true;
                        walk_again_when_finished = true;
                        custom_path = CustomWalkerPaths::solitude_prison_exit;
                    }


                    location_mode = true;
                    target_ref = location;
                    have_target_to_walk = true;
                    interaction_after_walk = -1;

                    reminder_start_pos = player->GetPosition();
                    reminder_target_name = MiscThings::insert_location_into_list_and_get_info(location);

                    right_attack_cancel();
                    left_attack_cancel();

                    std::string big_distance = "";
                    float distance = player->GetDistance(target_ref, true, true);


                    //if (distance > 40000.0f)
                        big_distance = " Distance to target: " + std::to_string((int)distance / 100) + " m. ";

                    result.first = true;
                    result.second = "[Started walking to location: " + reminder_target_name + "..." + big_distance + "]";
                    return result;
                }

            }
            else
                reset_walker();

        }

        result.first = false;
        result.second = "Invalid location ID. You get valid location ID's from context. ";// Use get_locations_around to get valid IDs. "; //TODO more info

        return result;
    }



    bool walk_to_location_by_refr(RE::TESObjectREFR* refr)
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            return false;
        }


        if (refr)
        {
            auto location = refr;

            if (location)
            {
                Observer::reset_threats();

                if (have_target_to_walk)
                    reset_walker();


                if (just_escaped_solitude_prison())
                {
                    //solitude_prison_exit
                    unregister_all_actions();
                    using_custom_path = true;
                    walk_again_when_finished = true;
                    custom_path = CustomWalkerPaths::solitude_prison_exit;
                }
                
                reminder_target_name = MiscThings::insert_location_into_list_and_get_info(location);
                reminder_start_pos = player->GetPosition();

                location_mode = true;
                target_ref = location;
                have_target_to_walk = true;
                interaction_after_walk = -1;

                right_attack_cancel();
                left_attack_cancel();

                return true;
            }

        }

        return false;

    }


    bool walk_to_player_marker(RE::TESObjectREFR* target_location)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            return false;
        }


        auto test_new_input = RE::BSInputEventQueue::GetSingleton();

        
        RE::BSTArray<RE::ObjectRefHandle> map_markers = player->currentMapMarkers;

        std::vector<std::string> location_list{};


        RE::TESObjectREFR* player_marker = nullptr;

        auto p_marker = player->playerMapMarker;

        if (p_marker.get())
            player_marker = p_marker.get().get();
        


        //auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();
        auto test_z_dif = player_pos.z - player_marker->GetPosition().z;


        if (player_marker)
        {
            auto location = player_marker;

            if (location)
            {
                if (have_target_to_walk)
                    reset_walker();

                Observer::reset_threats();


                if (just_escaped_solitude_prison())
                {
                    //solitude_prison_exit
                    unregister_all_actions();
                    using_custom_path = true;
                    walk_again_when_finished = true;
                    custom_path = CustomWalkerPaths::solitude_prison_exit;
                }

                location_mode = true;
                target_ref = location;
                have_target_to_walk = true;
                interaction_after_walk = -1;

                if (target_location)
                    reminder_target_name = MiscThings::insert_location_into_list_and_get_info(target_location);

                reminder_start_pos = player->GetPosition();


                //clear_input_queue();

               // set_universal_block(1.0f);

                right_attack_cancel();
                left_attack_cancel();
                //mouse_mouse_x_y(0, 0);
                //perk_up_cancel();
                //perk_down_cancel();

                return true;
            }

        }

        return false;

    }
    





    RE::TESQuest* last_quest_chosen = nullptr;
    RE::TESQuest* new_quest_chosen = nullptr;
    std::string last_quest_chosen_name = "";
    std::string new_quest_chosen_name = "";



    int get_quest_id_by_refr(RE::TESQuest* quest, RE::BGSQuestObjective* specific_objective)
    {
        int result = -1;

        if (!MiscThings::is_quest_list_valid())
        {
            auto get_quest_result = MiscThings::get_current_quests();

            if (!MiscThings::is_quest_list_valid())
            {
                return -1;
            }

        }
        else
        {
            //refresh quests in case the list changed 
            auto quest_list = MiscThings::get_p_quest_list();

            for (auto quest_entry : *quest_list)
            {
                if (!MiscThings::quest_is_hidden(quest_entry.quest))
                {
                    if (quest_entry.quest == quest)
                    {
                        auto the_quest = quest;

                        for (auto objective : the_quest->objectives)
                        {
                            if (quest_entry.objective == objective)
                            {
                                if (objective->state.all(RE::QUEST_OBJECTIVE_STATE::kDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kCompletedDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kFailedDisplayed))
                                {
                                    if (!specific_objective || (objective == specific_objective))
                                    {
                                        if (objective->numTargets > 0)
                                        {
                                            result = quest_entry.id;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        return result;
    }




    //this is supposed to be called from misc's new quest detector
    void test_new_very_close_quest()
    {
        if (last_quest_chosen)
        {
            int very_close_quest = MiscThings::get_very_close_quest();


            if (very_close_quest != -1)
            {
                if (get_quest_id_by_refr(last_quest_chosen) != very_close_quest)
                {
                    last_quest_chosen = nullptr; //so it autorefreshes when walk initiates
                }
            }
        }
    }


    std::pair<bool, std::string> walk_to_current_quest()
    {
        std::pair<bool, std::string> result{};


        if (last_quest_chosen)
        {
            if (!MiscThings::quest_is_hidden(last_quest_chosen))
            {
                if (MiscThings::is_quest_list_valid())
                {
                    auto quest_list = MiscThings::get_p_quest_list();

                    bool quest_is_still_there = false;
                    for (auto quest_entry : *quest_list)
                    {
                        if (quest_entry.quest == last_quest_chosen)
                        {
                            if (!(MiscThings::is_bad_jailquest(quest_entry.quest, quest_entry.target)))
                            {
                                quest_is_still_there = true;
                                break;
                            }
                        }
                    }

                    if (quest_is_still_there)
                    {
                        auto temp_result = walk_to_quest_by_index(get_quest_id_by_refr(last_quest_chosen, last_quest_objective), false, false);
                        if (!temp_result.first)
                            temp_result = walk_to_quest_by_index(get_quest_id_by_refr(last_quest_chosen), false, false); //now try without objective if objective failed

                        if (temp_result.first)
                        {
                            return temp_result;
                        }

                        //else - find another quest
                        
                    }
                        
                }
            }

        }

        //either quest disappeared, or became hidden, or quest list is invalid. find new best quest to follow

        if (!MiscThings::is_quest_list_valid() || !MiscThings::get_p_quest_list() || std::size(*MiscThings::get_p_quest_list()) <= 0)
        {
            result.first = false;
            result.second = "Cannot find any quests to follow";
            return result;
        }
        else
        {
            auto quest_list = MiscThings::get_p_quest_list();

            float min_distance = FLT_MAX;
            int best_id = -1;

            for (auto quest_entry : *quest_list)
            {
                if (quest_entry.target) //dont lock on quests without targets
                {
                    float corrected_distance = quest_entry.estimate_distance;
                    if (corrected_distance == 0.0f)
                        corrected_distance = 9999999.0f;

                    if (corrected_distance < min_distance)
                    {
                        if (!(MiscThings::is_bad_jailquest(quest_entry.quest, quest_entry.target)))
                        {
                            best_id = quest_entry.id;
                            min_distance = quest_entry.estimate_distance;
                        }

                    }
                }

            }

            if (best_id != -1)
                return walk_to_quest_by_index(best_id, false, false);
            else
            {
                result.first = false;
                result.second = "Cannot find any quests to follow";
                return result;
            }
        }
        

    }





    std::pair<bool, std::string> walk_to_quest_by_index(int index, bool ignore_specified_target, bool skip_confirm)
    {

        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            result.first = false;
            result.second = "You cannot walk yet";
            return result;
        }

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            result.first = false;
            result.second = cant_walk_reason;
            return result;
        }


        if (!MiscThings::is_quest_list_valid())
        {
            auto get_quest_result = MiscThings::get_current_quests();
            
            if (!MiscThings::is_quest_list_valid())
            {
                result.first = false;
                result.second = "You do not have any quests right now. ";
            }
            else
                send_random_context("Active quests: " + get_quest_result.second);


        }
        else
        {
            //refresh quests in case the list changed 
            auto quest_list = MiscThings::get_p_quest_list();

            bool quest_found = false;
            int actual_id = 0;

            for (auto quest_entry : *quest_list)
            {
                if (!MiscThings::quest_is_hidden(quest_entry.quest))
                {
                    if (quest_entry.id == index)
                    {
                        auto the_quest = quest_entry.quest;

                        for (auto objective : the_quest->objectives)
                        {
                            if (quest_entry.objective == objective)
                            {
                                if (objective->state.all(RE::QUEST_OBJECTIVE_STATE::kDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kCompletedDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kFailedDisplayed))
                                {
                                    for (auto* target : std::span(objective->targets, objective->numTargets))
                                    {
                                        if (target && target == quest_entry.target)
                                        {
                                            if (!(MiscThings::is_bad_jailquest(quest_entry.quest, quest_entry.target)))
                                            {
                                                quest_found = true;
                                                break;
                                            }
                                        }
                                    }

                                }
                            }

                        }
                    }
                }

                actual_id++;
            }

            if (!quest_found)
            {
                auto get_quest_result = MiscThings::get_current_quests();
                send_random_context("Active quests: " + get_quest_result.second);
            }
        }



        if (MiscThings::is_quest_list_valid())
        {
            
            auto player_ref = player->AsReference();

            auto quest_list = MiscThings::get_p_quest_list();

            bool quest_found = false;
            int actual_id = 0;

            for (auto quest_entry : *quest_list)
            {
                if (!MiscThings::quest_is_hidden(quest_entry.quest))
                {
                    if (quest_entry.id == index)
                    {
                        if (!(MiscThings::is_bad_jailquest(quest_entry.quest, quest_entry.target)))
                        {
                            quest_found = true;
                            break;
                        }

                    }
                }

                actual_id++;
            }

            if (!quest_found)
            {
                if (MiscThings::is_quest_list_valid())
                {
                    result.first = false;
                    result.second = "Invalid quest ID. Use get_current_quests to get valid ID list";
                }
                else
                {
                    result.first = false;
                    result.second = "No active quests. ";
                }
                return result;
            }

            if (actual_id < std::size(*quest_list) && actual_id >= 0 && player_ref)
            {
                auto quest_entry = quest_list->at(actual_id);

                auto objective = quest_entry.objective;

                if (objective)
                {
                    //if (have_target_to_walk)
                    //    reset_walker();


                    if (objective->numTargets != 0)
                    {
                        auto quest_targets = objective->targets;
 
                        for (auto* target : std::span(quest_targets, objective->numTargets)) {
                            if (target)
                            {
                                if (quest_entry.quest)
                                {
                                    bool conditions_met = false;
                                    //this needs to be figured out
                                    try {
                                        if (target->conditions.head)
                                        {
                                            auto the_condition = target->conditions.head;

                                            conditions_met = MiscThings::recursive_quest_condition_check(the_condition, quest_entry.quest, target);

                                            bool test_stop = false;

                                        }
                                        else
                                            conditions_met = true;

                                    }
                                    catch (...) {
                                        conditions_met = true;//met ?
                                    }


                                    if (conditions_met)
                                    {
                                        RE::ObjectRefHandle quest_ref_handle{};
                                        target->GetTrackingRef(quest_ref_handle, quest_entry.quest); //try tracked
                                        if (!quest_ref_handle)
                                        {
                                            target->GetTargetRef(quest_ref_handle, false, quest_entry.quest); //no tracked - try actual target
                                        }
                                        else
                                        {
                                            if (quest_ref_handle.get())
                                            {
                                                auto test_quests_target_ref = quest_ref_handle.get().get();
                                                if (test_quests_target_ref == player_ref)
                                                {

                                                    //it leads to player. try to give proper hint on what to do 

                                                    std::string actual_target_name = "maybe read some note or book?";

                                                    RE::ObjectRefHandle test_quest_ref_handle{};

                                                    target->GetTargetRef(test_quest_ref_handle, false, quest_entry.quest); //it tracks to player. check target

                                                    if (test_quest_ref_handle)
                                                    {
                                                        if (test_quest_ref_handle.get())
                                                        {
                                                            auto test_quest_target_ref2 = test_quest_ref_handle.get().get();

                                                            if (test_quest_target_ref2)
                                                            {
                                                                std::string test_actual_target_name = test_quest_target_ref2->GetDisplayFullName();

                                                                if (test_actual_target_name != "")
                                                                {
                                                                    auto base_obj = test_quest_target_ref2->GetBaseObject();
                                                                    std::string category = MiscThings::get_object_category((RE::TESForm*)base_obj);
                                                                    actual_target_name = category + " " + test_actual_target_name;

                                                                }
                                                            }
                                                        }
                                                    }
                                                    
                                                    reset_walker();
                                                    result.first = false;
                                                    result.second = "[This quest has no target to walk to... You need to investigate items in your inventory (" + actual_target_name + ")]";
                                                    return result;

                                                }
                                            }
                                        }


                                        if (!ignore_specified_target && target != quest_entry.target)
                                            continue;

                                        if (quest_ref_handle)
                                            if (quest_ref_handle.get())
                                            {
                                                auto quests_target_ref = quest_ref_handle.get().get();
                                               
                                                auto helgen_tower_marker = RE::TESObjectREFR::LookupByID(0xe24c3);



                                                bool quest_is_questionable = false;

                                                if (last_quest_chosen && quest_entry.quest != last_quest_chosen)
                                                {
                                                    //check if last quest is still there and ask for confirmation to digress from it
                                                    if (!MiscThings::quest_is_hidden(last_quest_chosen))
                                                    {
                                                        bool quest_is_still_there = false;
                                                        for (auto quest_entry : *quest_list)
                                                        {
                                                            if (quest_entry.quest == last_quest_chosen)
                                                            {
                                                                quest_is_still_there = true;
                                                                break;
                                                            }
                                                        }

                                                        if (quest_is_still_there)
                                                        {
                                                            quest_is_questionable = true;
                                                            new_quest_chosen = quest_entry.quest;
                                                            new_quest_chosen_name = quest_entry.name + ": " + quest_entry.displaytext;

                                                            trying_to_change_quest_course = true;
                                                        }
                                                    }

                                                }

                                                if (!quest_is_questionable)
                                                {
                                                    last_quest_chosen = quest_entry.quest;
                                                    last_quest_chosen_name = quest_entry.name + ": " + quest_entry.displaytext;
                                                }
                                                else
                                                {
                                                    //dont accept right away
                                                    result.first = true;
                                                    result.second = "[Processing...]";
                                                    return result;
                                                }





                                                if (!skip_confirm && quest_entry.name == "Unbound" && (target->alias == 124 || target->alias == 125))
                                                {
                                                    if (!multiple_paths_quest_choice_confirmed)
                                                    {
                                                        //dont accept right away
                                                        multiple_paths_quest_choice_confirming = true;
                                                        result.first = true;
                                                        result.second = "[Processing...]";
                                                        return result;
                                                    }
                                                    else
                                                    {
                                                        reset_multiple_paths_quest(); //clear choice and proceed
                                                    }

                                                }





                                                if (have_target_to_walk)
                                                {
                                                    Observer::reset_threats();

                                                    if (target_ref != quests_target_ref || backup_interaction_made)
                                                        reset_walker();
                                                    else
                                                    {
                                                        result.first = true;
                                                        result.second = "You keep following quest...";
                                                        return result;
                                                    }
                                                }

                                                if (quests_target_ref == helgen_tower_marker)
                                                {
                                                    unregister_all_actions();
                                                    using_custom_path = true;
                                                    custom_path = CustomWalkerPaths::helgen_tower_path;
                                                    //path_valid = true;
                                                }

                                                quest_mode = true;
                                                target_ref = MiscThings::redirect_quest_target(quest_entry.quest, quests_target_ref);


                                                current_quest_target_followed = quest_entry.target;
                                                current_quest_followed = quest_entry.quest;

                                                reminder_target_name = "[id " + std::to_string(quest_entry.id) + "] " + quest_entry.name + ": " + quest_entry.displaytext;
                                                reminder_start_pos = player->GetPosition();

                                               //MiscThings::insert_quest_into_list_and_get_info(quest);


                                                

                                                if (just_escaped_solitude_prison())
                                                {
                                                    //solitude_prison_exit
                                                    unregister_all_actions();
                                                    using_custom_path = true;
                                                    walk_again_when_finished = true;
                                                    custom_path = CustomWalkerPaths::solitude_prison_exit;
                                                }


                                                have_target_to_walk = true;
                                                interaction_after_walk = -1;
                                                
                                                auto base_obj = quests_target_ref->GetBaseObject();

                                                if (base_obj && base_obj->GetFormType() == RE::FormType::Container)
                                                    interaction_after_walk = 1;

                                                if (base_obj && base_obj->IsInventoryObject())
                                                    interaction_after_walk = 1;


                                                last_quest = quest_entry.quest;
                                                last_quest_objective = objective;


                                                

                                                right_attack_cancel();
                                                left_attack_cancel();


                                                std::string big_distance = "";
                                                float distance = player->GetDistance(target_ref, true, true);

                                                if (current_quest_target_followed && current_quest_followed)
                                                {
                                                    distance = MiscThings::get_quest_target_distance(current_quest_target_followed, current_quest_followed);
                                                }

                                                /*
                                                if (std::size(target->teleportPath.teleportRefs) > 0)
                                                {
                                                    //in different location
                                                    auto last_teleport_ref = target->teleportPath.teleportRefs.back().ref;
                                                    distance = player->GetDistance(last_teleport_ref, true, true);

                                                    special_ref_for_distance_calculation = last_teleport_ref;
                                                }
                                                */


                                                //if (distance > 40000.0f)
                                                    big_distance = " Distance to target: " + std::to_string((int)distance / 100) + " m. ";



                                                result.first = true;
                                                result.second = "[Started following quest: " + reminder_target_name + "..." + big_distance + "]";


                                                if (distance > 20000.0f)
                                                {
                                                    if (current_quest_target_followed && current_quest_followed)
                                                    {
                                                        std::string good_fasttravel_location = MiscThings::get_good_fasttravel_marker_for_quest_target(current_quest_target_followed, current_quest_followed);
                                                        if (good_fasttravel_location != "")
                                                        {
                                                            //advice
                                                            std::string advice = big_distance + " Closest fast-travel location: " + good_fasttravel_location + ". (You can use map to fast travel)";
                                                            add_delayed_message(advice);
                                                        }
                                                    }
                                                }

                                                return result;
                                            }

                                        break;
                                    }
                                }
                            }
                        }

                        if (!ignore_specified_target)
                        {
                            //try without specified target
                            return walk_to_quest_by_index(index, true);
                        }
                    }
                    else
                    {
                        //no target
                        result.first = false;
                        result.second = "This quest has no target to walk to. Perhaps you need to do something else to complete it...";
                        //return result;
                        //maybe it will find another objective that will actually work
                        return result;
                    }
                }
                else
                {
                    //no objective
                    result.first = false;
                    result.second = "This quest has no target to walk to. Perhaps you need to do something else to complete it...";
                    return result;
                }
            }
            else
                reset_walker();

        }

        result.first = false;
        if (result.second == "")
            result.second = "Invalid quest ID. Use get_current_quests to get valid ID list";

        reset_walker();

        return result;

    }


    bool detect_quest_target_changed_and_walk()
    {
        if (dont_check_quest_target_change)
            return false;

        if (using_custom_path && std::size(path) > 0 && current_path_point < (std::size(path) - 1))
            return false;


        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            return false;
        }


        if (quest_mode && last_quest && last_quest_objective && target_ref)
        {
            auto quest = last_quest;
            auto objective = last_quest_objective;

            if (objective)
            {
                if (objective->numTargets != 0)
                {
                    auto quest_targets = objective->targets;


                    //do this ONLY if old target's conditions are not met anymore
                    for (auto* target : std::span(quest_targets, objective->numTargets)) 
                    {
                        if (target)
                        {
                            if (quest)
                            {
                                bool conditions_met = false;
                                try {
                                    if (target->conditions.head)
                                    {
                                        auto the_condition = target->conditions.head;

                                        conditions_met = MiscThings::recursive_quest_condition_check(the_condition, quest, target);

                                        RE::ObjectRefHandle quest_ref_handle{};
                                        target->GetTrackingRef(quest_ref_handle, quest); //try tracked
                                        if (!quest_ref_handle)
                                            target->GetTargetRef(quest_ref_handle, false, quest); //no tracked - try actual target

                                        if (quest_ref_handle)
                                            if (quest_ref_handle.get())
                                            {
                                                auto quests_target_ref = quest_ref_handle.get().get();
                                                if (target_ref == quests_target_ref && conditions_met)
                                                    return false; //dont check if old objective is still valid
                                            }

                                    }
                                    else
                                        conditions_met = true;
                                }
                                catch (...) {
                                    conditions_met = true;//met ?
                                }

                            }
                        }
                    }


                    for (auto* target : std::span(quest_targets, objective->numTargets)) {
                        if (target)
                        {
                            if (quest)
                            {
                                bool conditions_met = false;
                                try {
                                    if (target->conditions.head)
                                    {
                                        auto the_condition = target->conditions.head;

                                        conditions_met = MiscThings::recursive_quest_condition_check(the_condition, quest, target);

                                        bool test_stop = false;
                                    }
                                    else
                                        conditions_met = true;
                                }
                                catch (...) {
                                    conditions_met = true;//met ?
                                }




                                if (conditions_met)
                                {
                                    RE::ObjectRefHandle quest_ref_handle{};
                                    target->GetTrackingRef(quest_ref_handle, quest); //try tracked
                                    if (!quest_ref_handle)
                                        target->GetTargetRef(quest_ref_handle, false, quest); //no tracked - try actual target

                                    if (quest_ref_handle)
                                        if (quest_ref_handle.get())
                                        {
                                            auto quests_target_ref = quest_ref_handle.get().get();

                                            quests_target_ref = MiscThings::redirect_quest_target(quest, quests_target_ref);

                                            if (target_ref != quests_target_ref)
                                            {
                                                path_valid = false;

                                                if (quests_target_ref == RE::TESForm::LookupByEditorID("dunCGObjectiveInn01REF") || quests_target_ref == RE::TESObjectREFR::LookupByID(0x000E24C3))
                                                {
                                                    unregister_all_actions();
                                                    using_custom_path = true;
                                                    custom_path = CustomWalkerPaths::helgen_tower_path;
                                                    path_valid = true;
                                                }
                                                else
                                                {
                                                    if (using_custom_path)
                                                    {
                                                        register_allowed_actions();
                                                        quicksave();
                                                    }
                                                        

                                                    using_custom_path = false;
                                                }
                                                    

                                                if (just_escaped_solitude_prison())
                                                {
                                                    //solitude_prison_exit
                                                    unregister_all_actions();
                                                    using_custom_path = true;
                                                    walk_again_when_finished = true;
                                                    custom_path = CustomWalkerPaths::solitude_prison_exit;
                                                }


                                                if (quests_target_ref == RE::TESObjectREFR::LookupByID(0x3FAFB)) //greybeard gate
                                                {
                                                    Observer::set_quest_puzzle_type(2);
                                                    reset_walker();
                                                }

                                                right_attack_cancel();
                                                left_attack_cancel();

                                                
                                                target_ref = MiscThings::redirect_quest_target(quest, quests_target_ref); //i think something is excessive here

                                                backup_interaction_made = false;

                                                current_quest_target_followed = target;
                                                current_quest_followed = quest;

                                                result = true;
                                            }
                                        }
                                            
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }

        return result;
    }




    std::pair<bool, std::string> walk_to_object_by_refr(RE::TESObjectREFR* target, int interaction, bool surrender_to_guards_mode)
    {    
        std::pair<bool, std::string> result{};

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            result.first = false;
            result.second = cant_walk_reason;
            return result;
        }


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            result.first = false;
            result.second = "You cannot walk yet";
            return result;
        }


        if (target)
        {
            if (have_target_to_walk)
            {
                bool shout_mode_remember = shout_mode;
                bool gate_shout_remember = gate_shout;
                RE::TESShout* shout_to_remember = shout_to_use;

                reset_walker();
                shout_mode = shout_mode_remember;
                gate_shout = gate_shout_remember;
                shout_to_use = shout_to_remember;

                if (gate_shout)
                    unregister_all_actions();
            }
                

            if (surrender_to_guards_mode)
                surrender_mode = true;


            if (just_escaped_solitude_prison())
            {
                //solitude_prison_exit
                unregister_all_actions();
                using_custom_path = true;
                walk_again_when_finished = true;
                custom_path = CustomWalkerPaths::solitude_prison_exit;
            }



            right_attack_cancel();
            left_attack_cancel();

            target_ref = target;
            have_target_to_walk = true;
            interaction_after_walk = interaction;

            auto player = RE::PlayerCharacter::GetSingleton();
            reminder_target_name = MiscThings::insert_object_into_list_and_get_info(target);
            reminder_start_pos = player->GetPosition();


            result.first = true;
            result.second = "[Started walking to " + reminder_target_name + "]";
            return result;
        }

        result.first = false;
        result.second = "[Error]";
        return result;

    }


    std::pair<bool, std::string> exit_dungeon()
    {
        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        RE::TESObjectREFR* target = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x7003887);


        auto player_pos = player->GetPosition();

        auto test_z_dif = player_pos.z - target->GetPosition().z;


        if (MiscThings::is_intro())
        {
            result.first = false;
            result.second = "Cannot exit dungeon right now";
            return result;
        }

        if (target)
        {
            //Observer::reset_threats();

            if (have_target_to_walk)
                reset_walker();

            location_mode = true;

            target_ref = target;
            have_target_to_walk = true;
            interaction_after_walk = 0;
            runaway_mode = true;
            reminder_target_name = "walking towards the exit. ";
            reminder_start_pos = player->GetPosition();
            result.first = true;
            result.second = "[You are waalking towards the exit...]";
        }
        else
        {
            Observer::reset_threats();
            result.first = false;
            result.second = "Cant find the exit! Maybe you need to find it yourself...";
        }

        return result;
    }




    std::pair<bool, std::string> run_away()
    {
        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        RE::TESObjectREFR* target = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x7003887);

        
        auto player_pos = player->GetPosition();
        
        auto test_z_dif = player_pos.z - target->GetPosition().z;


        if (MiscThings::is_intro())
        {
            result.first = false;
            result.second = "Cannot run away right now";
            return result;
        }

        if (prefer_solitude_prison_escape_route())
        {
            return escape_prison();
        }

        if (target)
        {
            //Observer::reset_threats();

            if (have_target_to_walk)
                reset_walker();

            location_mode = true;


            if (just_escaped_solitude_prison())
            {
                //solitude_prison_exit
                unregister_all_actions();
                using_custom_path = true;
                walk_again_when_finished = true;
                custom_path = CustomWalkerPaths::solitude_prison_exit;
            }


            target_ref = target;
            have_target_to_walk = true;
            interaction_after_walk = 0;
            runaway_mode = true;
            reminder_target_name = "running away]";
            reminder_start_pos = player->GetPosition();
            result.first = true;
            result.second = "[Running away...]";
        }
        else
        {
            Observer::reset_threats();
            result.first = false;
            result.second = "Nowhere to run!";
        }

        return result;
    }


    std::string get_lock_level_string(RE::TESObjectREFR* object)
    {
        std::string result = "";

        if (object)
        {
            auto door_refr = (RE::TESObjectDOOR*)object;

            if (auto extra = object->extraList.GetByType(RE::ExtraDataType::kLock); extra)
            {
                auto extra_lock = (RE::ExtraLock*)extra;

                if (auto extra_lock_data = extra_lock->lock; extra_lock_data)
                    if (extra_lock_data->IsLocked())//im not sure if this works. probably wants some other object as GetLockLevel container reference.
                    {
                        if (extra_lock_data->GetLockLevel(object) == RE::LOCK_LEVEL::kVeryEasy)
                            result = "Very Easy";
                        if (extra_lock_data->GetLockLevel(object) == RE::LOCK_LEVEL::kEasy)
                            result = "Easy";
                        if (extra_lock_data->GetLockLevel(object) == RE::LOCK_LEVEL::kAverage)
                            result = "Average";
                        if (extra_lock_data->GetLockLevel(object) == RE::LOCK_LEVEL::kHard)
                            result = "Hard";
                        if (extra_lock_data->GetLockLevel(object) == RE::LOCK_LEVEL::kVeryHard)
                            result = "Very Hard";
                        if (extra_lock_data->GetLockLevel(object) == RE::LOCK_LEVEL::kRequiresKey)
                            result = "Requires key";
                    }
            }
            else
            {
                if (object->IsLocked())
                {
                    if (object->GetLockLevel() == RE::LOCK_LEVEL::kVeryEasy)
                        result = "Very Easy";
                    if (object->GetLockLevel() == RE::LOCK_LEVEL::kEasy)
                        result = "Easy";
                    if (object->GetLockLevel() == RE::LOCK_LEVEL::kAverage)
                        result = "Average";
                    if (object->GetLockLevel() == RE::LOCK_LEVEL::kHard)
                        result = "Hard";
                    if (object->GetLockLevel() == RE::LOCK_LEVEL::kVeryHard)
                        result = "Very Hard";
                    if (object->GetLockLevel() == RE::LOCK_LEVEL::kRequiresKey)
                        result = "Requires key";
                }
            }
            

        }

        return result;
    }


    std::string get_locked_door_force_message(RE::TESObjectREFR* object)
    {
        std::string result = "The path is blocked by a closed door. Lockpick it? ";

        if (object)
        {
            std::string object_name = object->GetDisplayFullName();
            std::string lock_level_string = get_lock_level_string(object);

            result = "The path is blocked by " + object_name + " [Lock level: " + lock_level_string + "]. Lockpick it? ";
        }
        return result;
    }

    std::string get_locked_container_force_message(RE::TESObjectREFR* object)
    {
        std::string result = "This container is locked. Lockpick it? ";

        if (object)
        {
            std::string object_name = object->GetDisplayFullName();
            std::string lock_level_string = get_lock_level_string(object);

            result = object_name + " is locked [Lock level: " + lock_level_string + "]. Lockpick it? ";
        }
        return result;
    }




    bool target_is_interactive()
    {
        if (target_ref)
        {
            auto base_obj = target_ref->GetBaseObject();

            auto base_type = base_obj->GetFormType();

            if (target_ref->modelState != 0 && base_obj && base_type != RE::FormType::Static)
            {
                auto player = RE::PlayerCharacter::GetSingleton();
                auto player_ref = player->AsReference();
                RE::BSString result_string = "";

                RE::TESNPC* player_npc = (RE::TESNPC*)RE::TESForm::LookupByID(0x7); //left hand
                player_npc->GetActivateText(target_ref, result_string);

                //base_obj->GetActivateText(player_ref, result_string);

                if (result_string != "" && result_string != "Search\n")
                    return true;
            }
        }

        return false;
    }







    float get_spell_cost(bool right)
    {
        float result = 10000.0f;

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();
        if (player && player_actor)
        {
            RE::MagicItem* spell = (RE::MagicItem*)MiscThings::get_hand_contents(right);

            if (spell && spell->GetFormType() == RE::FormType::Spell)
            {
                result = spell->CalculateMagickaCost(player_actor);

                auto cast_type = spell->avEffectSetting->data.castingType;
                if (cast_type == RE::MagicSystem::CastingType::kConcentration)
                    result = 1.0f;
            }

        }

        if (result < 1.0f)
            result = 1.0f;

        return result;
    }


    std::string get_equipped_spell_name(bool right)
    {
        std::string result = "";

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            RE::MagicItem* spell = (RE::MagicItem*)MiscThings::get_hand_contents(right);


            if (spell && spell->GetFormType() == RE::FormType::Spell)
                if (spell->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
                    result = spell->GetFullName();



        }

        return result;
    }





    /*
    bool is_healing_spell(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            RE::MagicItem* spell;

            if (right)
                spell = player->selectedSpells[0];
            else
                spell = player->selectedSpells[1];


            if (spell)
                if (spell->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
                {
                    if (spell->GetDelivery() != RE::MagicSystem::Delivery::kSelf)
                    {

                    }
                }



        }
        return result;
    }
    */





    std::string get_equipped_weapon_name(bool right)
    {
        std::string result = "";

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            auto left_hand = player->currentProcess->equippedObjects[0];
            auto right_hand = player->currentProcess->equippedObjects[1];

            if (right)
            {
                if (right_hand)
                    result = right_hand->GetName();
            }
            else
            {
                if (left_hand)
                    result = left_hand->GetName();
            }

        }
        return result;
    }





    


    bool left_is_block()
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            auto left_hand = player->currentProcess->equippedObjects[0];

            if (left_hand)
            {
                auto weapon = (RE::TESObjectWEAP*)left_hand;
                if (!weapon->IsWeapon())
                    result = true;
                else
                {
                    if (weapon->IsTwoHandedAxe() || weapon->IsTwoHandedSword())
                        result = true;
                }
            }
            else
            {
                auto right_hand = player->currentProcess->equippedObjects[1];

                if (right_hand)
                {
                    auto weapon = (RE::TESObjectWEAP*)right_hand;
                    if (weapon->IsMelee())
                        result = true;
                }

            }
        }
        return result;
    }


    bool is_melee_weapon(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            auto left_hand = player->currentProcess->equippedObjects[0];
            auto right_hand = player->currentProcess->equippedObjects[1];

            if (right)
            {
                if (right_hand)
                {
                    auto weapon = (RE::TESObjectWEAP*)right_hand;
                    if (weapon->IsMelee())
                        return true;
                }
                    
            }
            else
            {
                if (left_hand)
                {
                    auto weapon = (RE::TESObjectWEAP*)left_hand;
                    if (weapon->IsMelee())
                        return true;
                }
            }

        }
        return result;
    }


    bool is_concentration_spell(bool right)
    {
        auto player = RE::PlayerCharacter::GetSingleton();

        RE::MagicItem* spell = (RE::MagicItem*)MiscThings::get_hand_contents(right);


        if (spell && spell->GetFormType() == RE::FormType::Spell)
        {
            auto cast_type = spell->avEffectSetting->data.castingType;
            if (cast_type == RE::MagicSystem::CastingType::kConcentration)
                return true;
        }

        return false;
    }


    float get_attack_time(bool right)
    {
        float result = 1.0;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            //auto left_spell = player->selectedSpells[0];
            //auto right_spell = player->selectedSpells[1];
            RE::MagicItem* spell = (RE::MagicItem*)MiscThings::get_hand_contents(right);

            if (spell && spell->GetFormType() == RE::FormType::Spell)
            {
                if (spell->avEffectSetting)
                {
                    auto cast_type = spell->avEffectSetting->data.castingType;
                    if (cast_type == RE::MagicSystem::CastingType::kConcentration)
                        result = 10.0f;
                    else
                        result = 1.3f;
                }
            }
            else
            {
                //not a spell..
                if (has_ranged_weapon_equipped(true) && !no_ammo())
                    result = 2.4f;//result = 2.7f;
                else
                    result = 0.8f;
            }

        }
        return result;

    }


    bool no_ammo()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();
        auto worn_ammo = player_actor->GetCurrentAmmo();

        RE::TESForm* hand_contents = MiscThings::get_hand_contents(true); //check if we have bow in right hand

        bool have_bow = false;

        if (hand_contents)
        {
            if (hand_contents->formType != RE::FormType::Spell)
            {
                auto weapon = (RE::TESObjectWEAP*)hand_contents;
                if (!weapon->IsMelee())
                    have_bow = true;
            }
        }

        return !worn_ammo && have_bow;

    }


    bool has_ranged_weapon_equipped(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        if (player)
        {
            RE::TESForm* hand_contents = MiscThings::get_hand_contents(right);

            if (hand_contents)
            {
                if (hand_contents->formType == RE::FormType::Spell)
                    return true;

                auto weapon = (RE::TESObjectWEAP*)hand_contents;
                if (!weapon->IsMelee())
                    return true;

            }

        }
        return result;
    }
  

    //this should never return 0
    float get_weapon_projectile_speed(bool right)
    {
        float result = 99999.0f;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            RE::TESForm* hand_contents = MiscThings::get_hand_contents(right);


            if (hand_contents)
            {
                if (hand_contents->formType == RE::FormType::Spell)
                {
                    //spell
                    auto spell = (RE::SpellItem*)hand_contents;

                    for (auto effect : spell->effects)
                    {
                        auto effectSetting = effect->baseEffect;
                        if (effectSetting && effectSetting->data.projectileBase)
                        {
                            auto speed = effectSetting->data.projectileBase->data.speed;

                            if (speed > 0.0f)
                                return speed;
                        }

                    }

                    return 99999.0f;

                }
                else
                {
                    auto weapon = (RE::TESObjectWEAP*)hand_contents;
                    if (!weapon->IsMelee())
                    {
                        //bow
                        return 2759.74896969697;
                    }
                }


            }
        }


        return result;
    }


    float get_weapon_range(bool right)
    {
        float result = 100.0f;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            RE::TESForm* hand_contents = MiscThings::get_hand_contents(right);

            if (hand_contents)
            {
                if (hand_contents->formType == RE::FormType::Spell)
                {
                    //spell
                    auto spell = (RE::SpellItem*)hand_contents;

                    auto range1 = spell->GetRange();
                    if (range1 > 0)
                        return range1;

                    if (spell->GetDelivery() != RE::MagicSystem::Delivery::kSelf)
                    {
                        for (auto effect : spell->effects)
                        {
                            auto effectSetting = effect->baseEffect;

                            if (effectSetting && effectSetting->data.projectileBase)
                            {
                                auto range2 = effectSetting->data.projectileBase->data.range;

                                if (range2 > 0.0f)
                                    return range2*0.9f;
                            }

                        }
                    }
                    else
                        return 1500.0f;

                }
                else
                {
                    auto weapon = (RE::TESObjectWEAP*)hand_contents;
                    if (!weapon->IsMelee())
                    {
                        //bow
                        if (!no_ammo())
                            return 5000.0f;
                    }
                    else
                    {
                        return 120.0f;
                    }
                }
            }
        }


        return result;
    }


    bool get_current_active_hand()
    {
        if (attack_action == 1)
            return false;
        else
            return true;
    }




   



    bool attack_target(float dtime)
    {

        if (MiscThings::is_on_horse())
        {
            confirm();
            return false; //get off the horse
        }



        if (shout_mode)
        {
            if (shout_to_use)
            {
                if (lock_camera_onto_target(target_ref, 0.016))
                {
                    if (!gate_shout || (gate_shout && gate_shout_condition()))
                    {
                        send_random_context("You are using the shout...");
                        MiscThings::cast_spell_by_refr((RE::SpellItem*)shout_to_use);
                        return true;
                    }
                }
            }
            else
            {
                return true;
            }
                

            return false;
        }

        lock_camera_onto_target(target_ref, dtime);

        bool result = false;
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();
        auto player_actor = (RE::Actor*)player_ref;

        if (MiscThings::is_dragon(target_ref) && MiscThings::is_flying(target_ref) && !target_ref->IsDead())// && (!(has_ranged_weapon_equipped(get_current_active_hand()) || shout_mode)) && !target_ref->IsDead())
        {
            float weapon_range = get_weapon_range(get_current_active_hand());
            auto player_pos = player->GetPosition();
            auto target_pos = target_ref->GetPosition();

            if (player_pos.GetDistance(target_pos) > get_weapon_range(get_current_active_hand()))
            {
                if (was_charging_ranged)
                    cancel_charge_weapon();

                return false;
            }
                
        }
            





        if (!(player_actor->IsWeaponDrawn()) && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing))
        {
            right_attack(); //this is "readyWeapon"
            //set_universal_block(0.5f);
            return false;
        }
            


        bool actually_attacked = false;

        bool no_weapons_equipped = false;

        if (target_ref && player_ref)
        {
            if (attack_action < 0 || attack_action > 1)
            {

                bool left_is_useless = MiscThings::has_spell_equipped(false) && !MiscThings::is_offensive_spell(false) && MiscThings::player_hp_more_than(90.0f);

                bool dont_use_right = false;

                dont_use_right |= (has_ranged_weapon_equipped(true) && no_ammo()) || (MiscThings::has_something_equipped(false) && !MiscThings::has_something_equipped(true) && !left_is_useless);


                attack_action = dont_use_right;
            }
                
            

            if (MiscThings::has_spell_equipped(true) && MiscThings::has_spell_equipped(false) && !MiscThings::is_offensive_spell(true) && !MiscThings::is_offensive_spell(false) && MiscThings::player_hp_more_than(90.0f))
            {
                //cannpt attack. notify
                if (!no_weapons_notified)
                {
                    no_weapons_notified = true;
                    send_random_context("You dont have anything that can deal damage in your hands! Equip weapons or offensive spells to deal damage", false);
                }

                no_weapons_equipped = true;

                goto finalize_attack;
            }
            else
                no_weapons_notified = false;


            //if (close_enough())
            //{
                if (attack_action == 0)
                {

                    if (low_mana_detected && (MiscThings::get_player_mana() > MiscThings::get_player_max_mana() * 0.3f))
                        low_mana_detected = false;


                    bool dont_check_mana = false;

                    dont_check_mana = !is_concentration_spell(true) && is_casting_walker2(true);

                    bool low_mana_check = (!dont_check_mana && MiscThings::has_spell_equipped(true) && (low_mana_detected || (MiscThings::get_player_mana() < get_spell_cost(true))));

                    if (low_mana_check)
                        low_mana_detected = true;

                    if (low_mana_check || (MiscThings::has_spell_equipped(true) && !MiscThings::is_offensive_spell(true) && MiscThings::player_hp_more_than(100.0f)))
                    {

                        //set_universal_block(1.0f);
                        right_attack_cancel();
                        //was_charging_ranged = false;
                        attack_action = 1;
                        
                        goto finalize_attack;
                        //goto attack_action_1;

                        //return false;
                    }

                    if (attack_action_time0 < get_attack_time(true) && !(MiscThings::has_spell_equipped(true) && !MiscThings::is_offensive_spell(true) && (MiscThings::player_hp_more_than(90.0f) && !is_casting_walker(true))))
                    {
                        std::string target_name = MiscThings::insert_object_into_list_and_get_info(target_ref);
                        
                        std::string attacking_info = "";

                        std::string start_attacking_info = "[You are attacking ";

                        std::string attacking_target = "";
                        std::string attacking_weapon = "";
                        std::string attacking_health = "";

                        bool casting = false;

                        bool skip_cast = false;

                        actually_attacked = true;

                        bool no_weapon = false;
                        bool silent = true;

                        if (MiscThings::has_spell_equipped(true))
                        {
                            if (attack_action_time0 > 0.7f);
                            ;// was_charging_ranged = true;

                            if (!is_casting_walker(true))
                            {
                                if (attack_spell_cast_timeout > 1.0f)
                                {
                                    skip_cast = true;
                                    attack_spell_cast_timeout = 0.0f;
                                    right_attack_cancel();
                                    attack_action_time0 = 0.0f;
                                }
                                else
                                    attack_spell_cast_timeout += dtime;
                            }
                            else
                                attack_spell_cast_timeout = 0.0f;


                            if (!skip_cast)
                            {
                                right_attack_spell();
                                if (!MiscThings::is_offensive_spell(true))
                                {
                                    casting = true;
                                    start_attacking_info = "[You are casting ";
                                }

                                attacking_weapon = get_equipped_spell_name(true) + ". ";
                            }
                            
                        }
                        else
                        {
                            attack_spell_cast_timeout = 0.0f;

                            if (has_ranged_weapon_equipped(true))
                            {
                                if (attack_action_time0 > 0.9f);
                                    was_charging_ranged = true;
                                right_attack_bow();
                            }
                            else
                                right_attack();

                            if (!MiscThings::has_something_equipped(true))
                            {
                                no_weapon = true;
                                attacking_weapon = "bare fist. You might want to equip some weapon or magic (use get_inventory and use_inventory_item to equip gear). ";
                                if (player->GetDistance(target_ref, true) > 80.0f * target_ref->GetScale())
                                    cursor_up();
                            }
                            else
                            {
                                if (has_ranged_weapon_equipped(true) && no_ammo())
                                {
                                    no_weapon = true;
                                    attacking_weapon = " left fist (you have no ammo to use with your " + get_equipped_weapon_name(true) + "). ";
                                }
                                else
                                    attacking_weapon = get_equipped_weapon_name(true) + ". ";

                                if (is_melee_weapon(true) && player->GetDistance(target_ref, true) > 100.0f * target_ref->GetScale())
                                    cursor_up();
                            }
                        }

                        if (!gave_attacking_info)
                        {
                            gave_attacking_info = true;

                            if (target_ref->IsActor() && !was_already_dead)
                            {
                                bool immortal = false;
                                auto target_actor = (RE::Actor*)target_ref;
                                int max_health = target_actor->GetActorValueMax(RE::ActorValue::kHealth);
                                int cur_health = target_actor->GetActorValue(RE::ActorValue::kHealth);
                                if (cur_health < 0)
                                {
                                    cur_health = 1;
                                    immortal = true;
                                }
                                attacking_health += "Enemy health : " + std::to_string(cur_health) + "/" + std::to_string(max_health);
                                if (immortal)
                                    ;// attacking_info += ". The target is not dying for some reason...";
                            }


                            if (attacking_weapon == "")
                                attacking_weapon = "fists. ";

                            if (attacking_info == "")
                            {
                                if (casting)
                                {
                                    attacking_info = start_attacking_info + attacking_weapon;
                                }
                                else
                                {
                                    if (last_attacking_target != target_name)
                                    {
                                        if (no_weapon)
                                            silent = false;
                                        attacking_info = start_attacking_info + target_name + " with your " + attacking_weapon + attacking_health;
                                    }
                                    else
                                        if (last_attacking_weapon != attacking_weapon)
                                        {
                                            if (no_weapon)
                                                silent = false;
                                            attacking_info = start_attacking_info + target_name + " with your " + attacking_weapon + attacking_health;
                                        }
                                        else
                                            if (last_attacking_health != attacking_health)
                                                attacking_info = attacking_health;
                                }

                            }

                            last_start_attacking_info = start_attacking_info;
                            last_attacking_target = target_name;
                            last_attacking_weapon = attacking_weapon;
                            last_attacking_health = attacking_health;


                            if (attacking_info != "")
                                send_random_context(attacking_info + "]", silent);
                        }


                        attack_action_time0 += dtime;
                    }
                    else
                    {
                        //end of attack
                        attack_spell_cast_timeout = 0.0f;

                        gave_attacking_info = false;
                        was_charging_ranged = false;
                        right_attack_cancel();

                        attack_action_time0 = 0.0f;

                        float choose_next_action = (float)std::rand() / RAND_MAX;


                        bool dont_use_left = false;

                        if (target_ref->IsActor() && target_ref->IsDead())
                        {
                            auto target_actor = (RE::Actor*)target_ref;
                            auto target_combat_controller = target_actor->combatController;

                            if (target_combat_controller)
                                dont_use_left = (target_combat_controller->IsFleeing() || target_combat_controller->ignoringCombat || !target_combat_controller->startedCombat || target_combat_controller->stoppedCombat) && left_is_block();
                        }
                        else
                        {
                            ;// dont_use_left |= left_is_block();
                        }


                        bool left_is_useless = MiscThings::has_spell_equipped(false) && !MiscThings::is_offensive_spell(false) && MiscThings::player_hp_more_than(90.0f);
                        bool right_is_useless = MiscThings::has_spell_equipped(true) && !MiscThings::is_offensive_spell(true) && MiscThings::player_hp_more_than(90.0f);

                        bool dont_use_right = false;


                        dont_use_left |= has_ranged_weapon_equipped(true) && !no_ammo();
                        //dont_use_left |= MiscThings::has_spell_equipped(true) && (!has_something_equipped(false) || (low_mana_detected && (MiscThings::get_player_mana() < get_spell_cost(false)) && MiscThings::has_spell_equipped(false)));
                        //bool dont_use_right = has_something_equipped(false) && (!has_something_equipped(true) || (low_mana_detected && (MiscThings::get_player_mana() < get_spell_cost(true)) && MiscThings::has_spell_equipped(true)));

                        dont_use_right |= (has_ranged_weapon_equipped(true) && no_ammo()) || (MiscThings::has_something_equipped(false) && !MiscThings::has_something_equipped(true) && !left_is_useless);


                        if ((choose_next_action < 0.2f && !dont_use_left) || dont_use_right)
                            attack_action = 1;
                        else
                            attack_action = 0;


                        //set_universal_block(0.2f);
                    }
                }
                else
                {

                attack_action_1:

                    if (attack_action == 1)
                    {
                        if (low_mana_detected && (MiscThings::get_player_mana() > MiscThings::get_player_max_mana() * 0.3f))
                            low_mana_detected = false;

                        bool dont_check_mana = false;

                        dont_check_mana = !is_concentration_spell(false) && is_casting_walker2(false);

                        bool low_mana_check = (!dont_check_mana && MiscThings::has_spell_equipped(false) && (low_mana_detected || (MiscThings::get_player_mana() < get_spell_cost(false))));

                        if (low_mana_check)
                            low_mana_detected = true;

                        if (low_mana_check || (MiscThings::has_spell_equipped(false) && !MiscThings::is_offensive_spell(false) && MiscThings::player_hp_more_than(100.0f)))// && MiscThings::player_is_full_hp()))
                        {
                            //set_universal_block(1.0f);
                            left_attack_cancel();
                            //was_charging_ranged = false;
                            attack_action = 0;
                            //return false;

                            goto finalize_attack;

                        }

                        if (attack_action_time1 < get_attack_time(false) && !(MiscThings::has_spell_equipped(false) && !MiscThings::is_offensive_spell(false) && (MiscThings::player_hp_more_than(90.0f) && !is_casting_walker(false))))
                        {
                            std::string target_name = MiscThings::insert_object_into_list_and_get_info(target_ref);
                            
                            std::string attacking_info = "";

                            std::string start_attacking_info = "[You are attacking ";

                            std::string attacking_target = "";
                            std::string attacking_weapon = "";
                            std::string attacking_health = "";

                            bool casting = false;

                            actually_attacked = true;

                            bool no_weapon = false;
                            bool silent = true;

                            if (MiscThings::has_spell_equipped(false))
                            {

                                bool skip_cast = false;

                                if (!is_casting_walker(false))
                                {
                                    if (attack_spell_cast_timeout > 1.0f)
                                    {
                                        skip_cast = true;
                                        attack_spell_cast_timeout = 0.0f;
                                        left_attack_cancel();
                                        attack_action_time1 = 0.0f;
                                    }
                                        
                                    else
                                        attack_spell_cast_timeout += dtime;
                                }
                                else
                                {
                                    if (attack_action_time1 > 0.7f);
                                    ;//was_charging_ranged = true;

                                    attack_spell_cast_timeout = 0.0f;
                                }
                                    

                                if (!skip_cast)
                                {
                                    left_attack_spell();
                                    if (!MiscThings::is_offensive_spell(left))
                                    {
                                        casting = true;
                                        start_attacking_info = "[You are casting ";
                                    }

                                    attacking_weapon += get_equipped_spell_name(false) + ". ";
                                }

                            }
                            else
                            {
                                attack_spell_cast_timeout = 0.0f;

                                left_attack();
                                if (!MiscThings::has_something_equipped(false) && MiscThings::has_something_equipped(true) && is_melee_weapon(true))
                                {
                                    attacking_info = "[You are blocking";
                                    if (player->GetDistance(target_ref, true) > 100.0f)
                                        cursor_up();
                                }
                                else
                                {
                                    if (!MiscThings::has_something_equipped(false))
                                    {
                                        no_weapon = true;
                                        attacking_weapon = "bare fist. You might want to equip some weapon or magic (use get_inventory and use_inventory_item to equip gear). ";
                                        if (player->GetDistance(target_ref, true) > 80.0f * target_ref->GetScale())
                                            cursor_up();
                                    }
                                    else
                                    {
                                        if (has_ranged_weapon_equipped(true) && no_ammo())
                                        {
                                            no_weapon = true;
                                            attacking_weapon = " left fist (you have no ammo to use with your " + get_equipped_weapon_name(true) + "). ";
                                        }
                                        else
                                            attacking_weapon = get_equipped_weapon_name(false) + ". ";

                                        if (is_melee_weapon(false) && player->GetDistance(target_ref, true) > 100.0f * target_ref->GetScale())
                                            cursor_up();
                                    }

                                }
                            }

                            if (!gave_attacking_info)
                            {
                                gave_attacking_info = true;

                                if (target_ref->IsActor() && !was_already_dead)
                                {
                                    bool immortal = false;
                                    auto target_actor = (RE::Actor*)target_ref;
                                    int max_health = target_actor->GetActorValueMax(RE::ActorValue::kHealth);
                                    int cur_health = target_actor->GetActorValue(RE::ActorValue::kHealth);
                                    if (cur_health < 0)
                                    {
                                        cur_health = 1;
                                        immortal = true;
                                    }
                                    attacking_health += "Enemy health : " + std::to_string(cur_health) + "/" + std::to_string(max_health);
                                    if (immortal)
                                        ;// attacking_health += ". The target is not dying for some reason...";
                                }

                                if (attacking_info == "")
                                {

                                    if (attacking_weapon == "")
                                        attacking_weapon = "fists. ";

                                    if (casting)
                                    {
                                        attacking_info = start_attacking_info + attacking_weapon;
                                    }
                                    else
                                    {
                                        if (last_attacking_target != target_name)
                                        {
                                            if (no_weapon)
                                                silent = false;
                                            attacking_info = start_attacking_info + target_name + " with your " + attacking_weapon + attacking_health;
                                        }
                                        else
                                            if (last_attacking_weapon != attacking_weapon)
                                            {
                                                if (no_weapon)
                                                    silent = false;
                                                attacking_info = start_attacking_info + target_name + " with your " + attacking_weapon + attacking_health;
                                            }
                                            else
                                                if (last_attacking_health != attacking_health)
                                                    attacking_info = attacking_health;
                                    }
                                }


                                last_start_attacking_info = start_attacking_info;
                                last_attacking_target = target_name;
                                last_attacking_weapon = attacking_weapon;
                                last_attacking_health = attacking_health;

                                if (attacking_info != "")
                                    send_random_context(attacking_info + "]", silent);
                            }

                            attack_action_time1 += dtime;
                        }
                        else
                        {
                            gave_attacking_info = false;
                            attack_spell_cast_timeout = 0.0f;
                            was_charging_ranged = false;

                            left_attack_cancel();
                            attack_action_time1 = 0.0f;
                            float choose_next_action = (float)std::rand() / RAND_MAX;

                            bool dont_use_left = false;

                            if (target_ref->IsActor() && !target_ref->IsDead())
                            {
                                auto target_actor = (RE::Actor*)target_ref;
                                auto target_combat_controller = target_actor->combatController;

                                if (target_combat_controller)
                                    dont_use_left = (target_combat_controller->IsFleeing() || target_combat_controller->ignoringCombat || !target_combat_controller->startedCombat || target_combat_controller->stoppedCombat) && left_is_block();
                            }
                            else
                            {
                                ;// dont_use_left |= left_is_block();
                            }

                            bool left_is_useless = MiscThings::has_spell_equipped(false) && !MiscThings::is_offensive_spell(false) && MiscThings::player_hp_more_than(90.0f);
                            bool right_is_useless = MiscThings::has_spell_equipped(true) && !MiscThings::is_offensive_spell(true) && MiscThings::player_hp_more_than(90.0f);

                            bool dont_use_right = false;


                            dont_use_left |= has_ranged_weapon_equipped(true) && !no_ammo();
                            //dont_use_left |= MiscThings::has_spell_equipped(true) && (!has_something_equipped(false) || (low_mana_detected && (MiscThings::get_player_mana() < get_spell_cost(false)) && MiscThings::has_spell_equipped(false)));
                            //bool dont_use_right = has_something_equipped(false) && (!has_something_equipped(true) || (low_mana_detected && (MiscThings::get_player_mana() < get_spell_cost(true)) && MiscThings::has_spell_equipped(true)));

                            dont_use_right |= (has_ranged_weapon_equipped(true) && no_ammo()) || (MiscThings::has_something_equipped(false) && !MiscThings::has_something_equipped(true) && !left_is_useless);


                            if ((choose_next_action < 0.2f && !dont_use_left) || dont_use_right)
                                attack_action = 1;
                            else
                                attack_action = 0;

                            //set_universal_block(0.2f);
                        }
                    }
                }


           // }
            

            finalize_attack:


            if (target_ref->IsActor() && !was_already_dead)
            {
                if (target_ref->IsDead())
                {
                    /* //now sent from observer
                    std::string victim_name = MiscThings::insert_object_into_list_and_get_info(target_ref);
                    std::string message_text = "[" + victim_name + " died]";

                    auto target_actor = (RE::Actor*)target_ref;
                    auto killer = target_actor->myKiller;
                    if (killer)
                    {
                        auto killer_ptr = killer.get();
                        if (killer_ptr)
                        {
                            auto killer_actor = killer_ptr.get();

                            if (killer_actor)
                            {
                                std::string killer_name = MiscThings::insert_object_into_list_and_get_info(killer_actor);
                                if (killer_actor == player_actor)
                                    killer_name = "You";

                                message_text = "[" + killer_name + " killed " + victim_name + "]";

                            }
                        }
                    }
                    send_random_context(message_text);
                    */

                    clear_input_queue();
                    right_attack_cancel();
                    left_attack_cancel();
                    


                    result = true;
                }
                else
                {
                    active_attacking_time += dtime;

                    auto target_actor = (RE::Actor*)target_ref;

                    if (active_attacking_time > 20.0f)
                    {
                        active_attacking_time = 0.0f;

                        

                        if (MiscThings::is_immortal(target_actor))
                        {
                                send_random_context("Attacking doesnt work... They are not dying. You can try to run away or ignore the fight instead.", false);
                                Observer::reset_threats(); //so it can actually offer choice to run or ignore
                                reset_walker();
                                return true;
                        }

                        std::string message = "You keep attacking...";

                        if (no_weapons_equipped)
                            message = "You dont have anything that can deal damage in your hands! Equip weapons or offensive spells to deal damage";

                        send_random_context(message, false);
                    }


                    if (MiscThings::is_immortal(target_actor) && target_actor->GetActorValue(RE::ActorValue::kHealth) < 2)
                    {
                        auto attackers = MiscThings::get_player_attackers(false, target_ref);

                        if (std::size(attackers) > 0)
                        {
                            //there are other targets nearby. switch target
                            return true;
                        }
                        else
                        {
                            if (MiscThings::player_brawling())
                            {
                                return true;
                            }
                        }
                    }

                }
                    
            }
            else
            {
                if (no_weapons_equipped)
                {
                    result = true;
                }
                else
                {
                    if (attacking_inanimate_object_time > 3.0f)
                    {
                        if (MiscThings::get_destructible_state(target_ref) != 0)
                        {
                            right_attack_cancel();
                            left_attack_cancel();

                            attacking_inanimate_object_time = 0.0f;

                            result = true;
                        }
                        else
                            attacking_inanimate_object_time += dtime;

                        if (attacking_inanimate_object_time > 5.0f)
                        {
                            right_attack_cancel();
                            left_attack_cancel();

                            attacking_inanimate_object_time = 0.0f;

                            result = true;
                        }

                    }
                    else
                    {
                        if (actually_attacked)
                            attacking_inanimate_object_time += dtime;
                    }
                }
                
            }
        }
        


        return result;
    }



    bool interact_with_target(float dtime)
    {
        bool result = false;

        auto result_target = get_targeted_ref();

        if ((interaction_after_walk == 1 || interaction_after_walk == 2) && !(looking_mode || MiscThings::is_intro() || MiscThings::is_intro2() || location_mode || (result_target == target_ref) || !target_is_interactive()))//(quest_mode && !target_is_interactive())))
            return false;


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();

        switch (interaction_after_walk) {

        case (1):
        {
            if (MiscThings::dont_interact_with(result_target))
                return true;


            if (confirming_closed_door_interaction)
            {
                if (door_is_closed_choice_valid)
                {
                    if (door_is_closed_choice)
                    {
                        confirm(); //lockpick it

                        //then lockpicking could fail or succeed...
                        //TODO: go on

                        return true;
                        ;// reset_walker();
                    }
                    else
                    {
                        register_allowed_actions();
                        //cut_navmesh_on_target(get_targeted_ref());
                        walk_backward_a_little = true;
                        //reset_walker(); //resets after walk
                    }
                }
            }
            else
            {
                if ((is_door(target_ref) || is_container(target_ref)) && is_targeted_door_locked())
                {
                    if (MiscThings::is_door_superlocked(target_ref))
                    {
                        send_random_context("Cannot lockpick this door. It requires a key", false);
                        reset_walker();
                        return true;
                    }

                    if (MiscThings::get_picks_amount_int() <= 0)
                    {
                        send_random_context("You dont have any lockpicks to open the lock", false);

                        if (is_door(target_ref))
                            cut_navmesh_on_target(get_targeted_ref());

                        reset_walker();
                        return true;
                    }

                    if (!confirming_closed_door_interaction)
                    {
                        

                        auto attackers = MiscThings::get_player_attackers();
                        if (std::size(attackers) > 0)
                        {
                            confirm(); //lockpick it
                            return true;
                        }
                        else
                        {
                            unregister_all_actions();

                            if (is_door(target_ref))
                            {
                                if (force_choice({ {0, "No"}, {1, "Yes"} }, get_locked_door_force_message(target_ref), force_type::closed_door_choice))
                                    confirming_closed_door_interaction = true;
                            }
                            else
                                if (force_choice({ {0, "No"}, {1, "Yes"} }, get_locked_container_force_message(target_ref), force_type::closed_door_choice))
                                    confirming_closed_door_interaction = true;
                        }

                    }

                }
                else
                {

                    if (target_ref)
                    {
                        if (MiscThings::is_stealing(target_ref) != "")
                        {
                            if ((crouch_timeout < 5.0f) && (player && !player->IsSneaking()))
                            {
                                crouch();
                                pause_pre_stealing = true;
                                pause_pre_stealing_time = 0.0f;
                                //set_universal_block(0.5f);
                                crouch_timeout += dtime;


                                return false;
                            }

                            if (player && player->IsSneaking())
                                crouch_timeout = 0.0f;
                                

                            auto player_actor = (RE::Actor*)player_ref;


                            //if (target_ref->IsCrimeToActivate())
                            //auto test = player->lastSeenTime;


                            //if (player_actor->WouldBeStealing(target_ref))
                            if (!MiscThings::is_player_hidden())
                            {
                                if (stealing_timer > 4.0f)
                                {
                                    if (!stealing_confirmed)
                                    {
                                        confirm_stealing = true;
                                        return false;
                                    }
                                }
                                else
                                {
                                    stealing_timer += dtime;
                                    return false;
                                }
                            }
                        }
                        else
                        {
                            if (player && player->IsSneaking() && get_targeted_ref() && get_targeted_ref()->IsActor())
                            {
                                crouch(); //uncrouch
                                set_universal_block(0.5f);
                                stop_sneaking = true;
                                return false;
                            }
                        }
                    }

                    if (!interaction_context_sent)
                    {



                        interaction_context_sent = true;
                        if (target_ref)
                        {
                            std::string target_name = MiscThings::insert_object_into_list_and_get_info(target_ref);//target_ref->GetDisplayFullName();
                            auto base_obj = target_ref->GetBaseObject();
                            if (base_obj->IsInventoryObject())
                            {
                                send_random_context("[Picking up " + target_name + "...]", false);
                                /* //TODO it ruins the list a little, observer starts spamming. figure it out later
                                if (MiscThings::is_objects_around_valid())
                                {
                                    auto objects_around = MiscThings::get_p_objects_around();
                                    for (std::pair<int, RE::TESObjectREFR*> object : *objects_around)
                                    {
                                        if (object.second == target_ref)
                                        {
                                            objects_around->erase(object.first);
                                            break;
                                        }
                                    }
                                }
                                */

                                

                                if (!backup_pickup)
                                {
                                    if (backup_pickup_attempts <= 3)
                                    {
                                        backup_pickup_attempts++;
                                        backup_pickup = true;
                                        backup_pickup_object = target_ref;
                                    }
                                    else
                                    {
                                        backup_pickup_attempts = 0;
                                        backup_pickup = false;
                                        backup_pickup_object = nullptr;
                                        backup_pickup_time = 0.0f;
                                    }
                                }

                                
                            }
                            else
                            {
                                if (is_container(target_ref) || (target_ref->IsActor() && target_ref->IsDead()))
                                {
                                    //even though its meant for picking up objects, it should technically work for containers or dead bodies too
                                    if (!backup_pickup)
                                    {
                                        if (backup_pickup_attempts <= 3)
                                        {
                                            backup_pickup_attempts++;
                                            backup_pickup = true;
                                            backup_pickup_object = target_ref;
                                        }
                                        else
                                        {
                                            backup_pickup_attempts = 0;
                                            backup_pickup = false;
                                            backup_pickup_object = nullptr;
                                            backup_pickup_time = 0.0f;
                                        }
                                    }
                                }




                                if (base_obj)
                                {
                                    bool is_harvestable = false;

                                    if (base_obj->GetFormType() == RE::FormType::Tree)
                                    {
                                        auto tree_form = (RE::TESObjectTREE*)base_obj;

                                        auto test_flags = target_ref->AsReference()->GetFormFlags();

                                        bool already_harvested = false;

                                        if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
                                            already_harvested = true;

                                        if (test_flags & 2048) //this is potentially only one we need here
                                            already_harvested = true;




                                        if (tree_form->produceItem && !already_harvested)
                                            is_harvestable = true;
                                    }

                                    if (base_obj->GetFormType() == RE::FormType::Flora)
                                    {
                                        auto tree_form = (RE::TESFlora*)base_obj;

                                        auto test_flags = target_ref->AsReference()->GetFormFlags();
                                        bool already_harvested = false;
                                        if (test_flags & RE::TESObjectREFR::RecordFlags::kHarvested) //THIS FLAG IS POTENTIALLY INCORRECT.
                                            already_harvested = true;

                                        if (tree_form->produceItem && !already_harvested)
                                            is_harvestable = true;
                                    }


                                    if (is_harvestable)
                                    {
                                        if (!backup_pickup)
                                        {
                                            if (backup_pickup_attempts <= 3)
                                            {
                                                backup_pickup_attempts++;
                                                backup_pickup = true;
                                                backup_pickup_object = target_ref;
                                            }
                                            else
                                            {
                                                backup_pickup_attempts = 0;
                                                backup_pickup = false;
                                                backup_pickup_object = nullptr;
                                                backup_pickup_time = 0.0f;
                                            }
                                        }
                                    }
                                }

                                

                                if (MiscThings::is_insect(target_ref))
                                {
                                    //register backup-pickup for them
                                    if (!backup_pickup)
                                    {
                                        if (backup_pickup_attempts <= 3)
                                        {
                                            backup_pickup_attempts++;
                                            backup_pickup = true;
                                            backup_pickup_object = target_ref;
                                        }
                                        else
                                        {
                                            backup_pickup_attempts = 0;
                                            backup_pickup = false;
                                            backup_pickup_object = nullptr;
                                            backup_pickup_time = 0.0f;
                                        }
                                    }
                                }



                                std::string no_result = "";
                                if (!dont_tell_result)
                                {
                                    if (!target_is_interactive())
                                        no_result = " Nothing happens...";

                                    if (MiscThings::get_pillar_face_name(target_ref, 1) == "") // for pillars there is separate message outside of interaction
                                    {
                                        std::string lever_advice = MiscThings::lever_interaction_advice(target_ref);

                                        if (backup_pickup_attempts <= 1)
                                            send_random_context("[Interacting with " + target_name + "..." + no_result + lever_advice + "]", true);
                                    }


                                }

                            }
                                
                        }
                    }

                  
                    

                    if (is_workbench(target_ref))
                    {
                        //confirm_workbench(); //HOOK Activate() or just try it with random arguments. seems like it might work.
                        
                        //auto workbench = (RE::TESFurniture*)target_ref;


                        auto furniture_ref = (RE::TESObjectREFR*)(target_ref->data.objectReference);

                        if (furniture_ref && player_ref)
                        {
                            confirm(); //clear_leftclick();
                            //furniture_ref->Activate(target_ref, player_ref, 0, nullptr, 1); //THIS WORKS. use for something else if needed
                            ;// workbench->Activate(target_ref, player_ref, 0, )
                        }


                        //return false;
                    }
                    else
                    {
                        if (MiscThings::get_door_teleport(target_ref) != "" && quest_mode)
                            just_teleported = true;

                        confirm();
                    }
                        

                    return true;
                }
            }
            

            break;
        }

        case (2):
        {
            //pickpocket

            auto player = RE::PlayerCharacter::GetSingleton();

            auto actor_obj = (RE::Actor*)target_ref;

            bool is_walking = actor_obj->IsWalking() || actor_obj->actorState1.movingBack; //movingBack is true when they walk forward (sometimes?)


            if (wait_and_start_pickpocket)
            {
                if ((waiting_pickpocket_time > 0.7f && is_walking) || waiting_pickpocket_time > 1.7f)
                {
                    //if (was_slowwalking)
                    //    unslow_walk();

                    confirm();
                    return true;
                }
                else
                {
                    if (is_walking)
                        ;// walk_forward();// cursor_up();


                    waiting_pickpocket_time += dtime;
                }
            }
            else
            {
                if (player && !player->IsSneaking())
                {
                    crouch(); //uncrouch
                }

                auto actor_obj = (RE::Actor*)target_ref;

                //if (actor_obj->IsWalking() && !was_slowwalking)// || actor_obj->IsRunning())
                //{
                //    was_slowwalking = true;
                //    slow_walk();
                //}
                    

                wait_and_start_pickpocket = true;
            }

            //confirm();
            return false; //will be handleed differently


            break;
        }


        case (3):
        {

            bool still_alive = target_ref->IsActor() && !target_ref->IsDead();

            if (!attacking_done)
            {
                Observer::set_threat_action_taken();
                locking_failed = false;
                start_attacking = true;
            }
            else
            {
                low_mana_detected = false;

                bool ignore_alive = false;

                if (target_ref->IsActor())
                {
                    auto target_actor = (RE::Actor*)target_ref;
                    int cur_health = target_actor->GetActorValue(RE::ActorValue::kHealth);
                    ignore_alive = MiscThings::is_immortal(target_actor) && cur_health < 2;


                }

                if (still_alive && !ignore_alive)
                    attacking_done = false; //cleanup from walk_again.
            }

            //else
            //    return true;
           
            //bool still_alive = target_ref->IsActor() && !target_ref->IsDead();


            return attacking_done;// && !still_alive;//attack_target(dtime);


            break;
        }



        default:
            result = true;
        }


        return result;
    }



    std::string get_success_message()
    {
        std::string result = "";
        std::string target_name = target_ref->GetDisplayFullName();

        if (MiscThings::is_intro())
        {
            if (location_mode)
                result = "You cannot walk yet. Looking at location"; //default
            else
                result = "You cannot walk yet. Looking at " + MiscThings::insert_object_into_list_and_get_info(target_ref);
        }
        else
        {
            if (location_mode)
                result = "You walked up to location"; //default
            else
            {

                target_name = MiscThings::insert_object_into_list_and_get_info(target_ref);
                if (quest_mode && (target_name == "")) //not guaranteed that insert_object will give us a name
                    target_name = "quest target point";

                auto karthspire_pillars = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xad059);

                if (karthspire_pillars)
                {
                    if (target_ref == karthspire_pillars)
                    {
                        target_name = "some unusual puzzle pillars... Looks like you need to turn all pillars to correct positions to proceed";
                        walk_backward_a_little = true;
                    }
                }

                result = "You walked up to " + target_name; //default

                switch (interaction_after_walk)
                {
                case 1:
                {
                    if (target_ref->IsInventoryObject())
                        result = "Picked up " + MiscThings::insert_object_into_list_and_get_info(target_ref);
                    else
                        result += " and interacted";// with this object";

                    result = ""; //have another notification in interact_with_object

                    if (MiscThings::is_intro2() && target_ref->IsActor())
                    {
                        result = "You walked up to " + target_name + " and started following them";
                    }

                    break;
                }

                case 2:
                {
                    result += " and tried to pickpocket";
                    break;
                }

                case 3:
                {
                    result += " and started attacking";
                    break;
                }

                default:
                {
                    if (MiscThings::is_intro2() && target_ref->IsActor())
                    {
                        result = "You walked up to " + target_name + " and started following them";
                    }

                    break;
                }
                }



            }
        }

        if (result != "")
            result = "[" + result + "]";

        return result;
    }


    RE::NiPoint3 last_player_pos{};
   


    bool almost_stuck()
    {
        return time_stuck > 1.0f;
    }

    bool detect_stuck(float dtime)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            auto player_pos = player->GetPosition();
            auto pos_dif = player_pos - last_player_pos;

            if (pos_dif.Length() > 50.0f)
            {
                last_player_pos = player_pos;
                time_stuck = 0.0f;
            }
            else
            {
                time_stuck += dtime;

                if (time_stuck > 1.5f)
                {
                    result = true;
                }
            }
        }


        return result;
    }


    bool detect_stuck_walk_again_if_good(float dtime) //!!!
    {
        bool result = false;


        time_blind_walk += dtime;

        if (time_blind_walk > 6.5f)
        {
            walk_retries = 0; //questionable
            walk_again();
            return false;
        }
            

        

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            auto player_pos = player->GetPosition();
            auto pos_dif = player_pos - last_player_pos;

            if (pos_dif.Length() > 200.0f)
            {
                last_player_pos = player_pos;
                time_stuck = 0.0f;
            }
            else
            {
                time_stuck += dtime;

                if (time_stuck > 1.5f)
                {
                    result = true;
                }
            }
        }


        return result;
    }



    bool path_record_mode = false;
    std::vector<RE::NiPoint3> custom_path_record{};



    void path_is_blocked_result(RE::TESObjectREFR* result_target, bool last_try = false)
    {
        std::string blocking_object_name = "Something";

        bool potential_puzzle = false;

        if (result_target)
        {
            std::string result_target_name = result_target->GetDisplayFullName();
            if (result_target_name != "")
            {
                blocking_object_name = MiscThings::insert_object_into_list_and_get_info(result_target);

            }

        }
        else
        {
            std::string check_name = MiscThings::get_potential_blocking_object();
            if (check_name != "")
            {
                potential_puzzle = true;
                blocking_object_name = check_name;
            }
            else
            {
                if (!last_try)
                {
                    //nothing found. run a raycast ignoring scan
                    if (!MiscThings::is_serving_jail())
                        Observer::detect_interesting_objects(0.016, true);


                    return path_is_blocked_result(result_target, true);
                }
            }
                
        }

        if (MiscThings::is_on_horse())
        {
            auto player = RE::PlayerCharacter::GetSingleton();
            auto player_actor = (RE::Actor*)player->AsReference();
            RE::ActorPtr mount_ptr;
            player_actor->GetMount(mount_ptr);
            std::string mount_name = "Horse";
            if (mount_ptr)
            {
                RE::TESObjectREFR* mount_refr = (RE::TESObjectREFR*)mount_ptr.get();
                mount_name = MiscThings::insert_object_into_list_and_get_info(mount_refr);
            }
            send_random_context("Cannot do this while on mount: " + mount_name + ". Interact with mount to get off it first", false);
        }
        else
        {
            if (blocking_object_name == "Something")
                send_random_context("[" + blocking_object_name + " blocks the object. Maybe it is inaccessible at all]", false);
            else
                send_random_context("[" + blocking_object_name + " blocks the object]", false);

        }
            

        remove_navmesh_cutter();
        reset_walker();//there is something but it cannot be moved
    }




    std::pair<bool, std::string> make_spins(int amount, int speed)
    {
        std::pair<bool, std::string> result{};


        

        if (amount <= 0)// || amount >= 100)
        {
            result.first = false;
            result.second = "Cannot make this amount of spins";
            return result;
        }

        if (speed == 0 || speed > 5 || speed < -5)
        {
            result.first = false;
            result.second = "Invalid speed. Valid range: from -5 to 5, excluding 0";
            return result;
        }


        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            result.first = false;
            result.second = cant_walk_reason;
            return result;
        }

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            result.first = false;
            result.second = "You cannot spin yet";
            return result;
        }

        auto camera = RE::PlayerCamera::GetSingleton();
        spins_start_camera_dir = camera->cameraRoot.get()->world.rotate;

        do_spins = true;
        spin_speed = speed;
        amount_of_spins = amount;
        amount_of_spins_done = 0;
        spin_timeout = 0.0f;

        result.first = true;
        result.second = "[You start spinning...]";

        unregister_spin_action();

        return result;


    }



    bool ustengrev_off_the_cliff()
    {
        std::pair<bool, std::string> result{};

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            return false;
        }


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            return false;
        }


        RE::TESObjectREFR* target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70b4f); //dragon wall

        if (target)
        {
            if (have_target_to_walk)
            {
                reset_walker();
            }

            quicksave();

            unregister_all_actions();

            send_random_context("You walk towards the cliff...", false);

            using_custom_path = true;
            walk_again_when_finished = true;
            custom_path = CustomWalkerPaths::ustengrev_off_the_cliff;

            ustengrev_cliff_mode = true;

            right_attack_cancel();
            left_attack_cancel();

            target_ref = target;
            have_target_to_walk = true;
            interaction_after_walk = 0;

            auto player = RE::PlayerCharacter::GetSingleton();
            reminder_target_name = "Cliff";
            reminder_start_pos = player->GetPosition();

            return true;
        }

        return false;
    }





    bool ustengrev_puzzle_get_ready(int mode)
    {
        std::pair<bool, std::string> result{};

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            return false;
        }


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            return false;
        }


        RE::TESObjectREFR* target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x36bf0);

        if (target)
        {
            if (have_target_to_walk)
            {
                reset_walker();
            }



            unregister_all_actions();

            send_random_context("You get into position...", false);

            using_custom_path = true;
            walk_again_when_finished = true;
            custom_path = CustomWalkerPaths::ustengrev_start_point;

            if (mode == 0)
                ustengrev_get_ready_mode = true;

            if (mode == 1)
                ustengrev_run_only_mode = true;

            if (mode == 2)
                ustengrev_wrong_order_mode = true;

            right_attack_cancel();
            left_attack_cancel();

            target_ref = target;
            have_target_to_walk = true;
            interaction_after_walk = 0;

            auto player = RE::PlayerCharacter::GetSingleton();
            reminder_target_name = "";
            reminder_start_pos = player->GetPosition();

            return true;
        }

        return false;
    }


    /*
    bool ustengrev_finish_line()
    {
        std::pair<bool, std::string> result{};

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            return false;
        }


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            return false;
        }



        unregister_all_actions();
        //using_custom_path = true;
        //walk_again_when_finished = true;
        //custom_path = CustomWalkerPaths::ustengrev_finish_line;


        RE::TESObjectREFR* target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xeb20c); //some random trigger zone after gates

        if (target)
        {
            if (have_target_to_walk)
            {
                using_custom_path = false; //prevent quicksave
                reset_walker();
            }

            ustengrev_get_ready_mode = true;

            right_attack_cancel();
            left_attack_cancel();

            target_ref = target;
            have_target_to_walk = true;
            interaction_after_walk = 0;

            auto player = RE::PlayerCharacter::GetSingleton();
            reminder_target_name = "";
            reminder_start_pos = player->GetPosition();

            return true;
        }

        return false;
    }
    */


    bool ustengrev_puzzle_run_and_shout()
    {
        std::pair<bool, std::string> result{};

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            return false;
        }


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_actor = (RE::Actor*)player->AsReference();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
        bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

        //if (player_actor && !player_actor->movementController->controlsDriven)
        if (!can_walk && !can_look)
        {
            return false;
        }





        RE::TESObjectREFR* target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x36bf0);

        if (target)
        {
            if (have_target_to_walk)
            {
                using_custom_path = false; //prevent quicksave
                reset_walker();
            }

            unregister_all_actions();
            using_custom_path = true;
            walk_again_when_finished = true;
            custom_path = CustomWalkerPaths::ustengrev_shout_point;
            ustengrev_shout_mode = true;


            right_attack_cancel();
            left_attack_cancel();

            target_ref = target;
            have_target_to_walk = true;
            interaction_after_walk = 0;

            auto player = RE::PlayerCharacter::GetSingleton();
            reminder_target_name = "";
            reminder_start_pos = player->GetPosition();

            return true;
        }

        return false;
    }





    std::pair<bool, std::string> surrender_to_guards()
    {
        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();

        auto attackers = MiscThings::get_player_attackers();

        bool player_can_be_arrested = false;
        RE::TESObjectREFR* closest_guard = nullptr;

        if (std::size(attackers) != 0 && !MiscThings::have_force_only_menu_open())
        {

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

                if (!tried_to_yield && attacker->IsGuard())
                {
                    //didnt try to yield yet and attacker is guard. can try to yield
                    player_can_be_arrested = true;
                    if (!closest_guard)
                        closest_guard = attacker;
                }

            }
        }

        if (player_can_be_arrested && closest_guard)
        {
            return walk_to_object_by_refr(closest_guard, 0, true);
        }
        else
        {
            result.first = false;
            result.second = "You cannot surrender to guards anymore. ";
            unregister_surrender_to_guards();
            return result;
        }
    }



    std::pair<bool, std::string> shout_at_target(RE::TESObjectREFR* target, RE::TESShout* shout, bool is_gate_shout, RE::TESObjectREFR* refr_to_activate_when_done)
    {
        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();

        bool have_this_shout = MiscThings::player_has_spell((RE::SpellItem*)shout);

        if (have_this_shout)
        {
            gate_shout = is_gate_shout;
            shout_mode = true;
            shout_to_use = shout;
            activate_refr_after_walker_is_done = refr_to_activate_when_done;
            return walk_to_object_by_refr(target, 3);
        }
        else
        {
            result.first = false;
            result.second = "You dont have this shout unlocked ";
            return result;
        }
    }


    

    void walk_whiterun_prison_grate()
    {
        if (have_target_to_walk)
        {
            Observer::reset_threats();
            reset_walker();
        }


        unregister_all_actions();
        using_custom_path = true;
        custom_path = CustomWalkerPaths::whiterun_prison_grate;
        path_valid = true;
        path = CustomWalkerPaths::whiterun_prison_grate;

        

        target_ref = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9DC81);

        reset_after_walk = true;
        have_target_to_walk = true;
        interaction_after_walk = -1;

        right_attack_cancel();
        left_attack_cancel();

    }




    void walk_karthspire_plates()
    {
        if (have_target_to_walk)
        {
            Observer::reset_threats();
            reset_walker();
        }


        unregister_all_actions();
        using_custom_path = true;
        custom_path = CustomWalkerPaths::karthspire_plates;
        path_valid = true;
        path = CustomWalkerPaths::karthspire_plates;



        target_ref = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x511a4); //chain

        activate_refr_after_walker_is_done = target_ref;

        reset_after_walk = true;
        have_target_to_walk = true;
        //interaction_after_walk = 1;
        always_shift = true;
        karthspire_plates = true;

        right_attack_cancel();
        left_attack_cancel();

    }




    std::pair<bool, std::string> escape_prison()
    {
        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto parent_cell = player->GetParentCell();

        bool stop_here = false;
        //0x56e88 - solitude prison
        //0x54592 - solitude escape ladder

        RE::TESObjectREFR* escape_target = nullptr;
        if (parent_cell)
        {
            if (parent_cell->GetFormID() == 0x56e88) //solitude prison
            {
                if (prefer_solitude_prison_escape_route())
                {
                    escape_target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x54592);
                    if (escape_target)
                    {
                        return walk_to_object_by_refr(escape_target, 1);
                    }
                }
                else
                {
                    escape_target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0xBF22C);
                    if (escape_target)
                    {
                        return walk_to_object_by_refr(escape_target, 1);
                    }
                }



            }

            if (parent_cell->GetFormID() == 0x4A376) //whiterun prison
            {
                escape_target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x80FBC);
                if (escape_target)
                {
                    return walk_to_object_by_refr(escape_target, 1);
                }
            }


            if (parent_cell->GetFormID() == 0x961ff) //winterhold prison
            {
                escape_target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x96248);
                if (escape_target)
                {
                    return walk_to_object_by_refr(escape_target, 1);
                }
            }



            //else
            return run_away();
        }








        result.first = false;
        result.second = "Cannot find any way to escape from here...";
        return result;
    }




    void get_into_carriage(RE::TESObjectREFR* seat)
    {
        getting_into_carriage = true;
        walk_to_object_by_refr(seat, 1);
    }

    bool is_getting_into_carriage()
    {
        return getting_into_carriage;
    }



    void reset_backup_pickup()
    {
        //this is called after load of save
        walker_inactive_time = 0.0f;

        backup_pickup_attempts = 0;
        backup_pickup = false;
        backup_pickup_object = nullptr;
        backup_pickup_time = 0.0f;
    }





    std::pair<bool, std::string> set_ruin_pillar_choice(int id)
    {
        std::pair<bool, std::string> result{};

        if (!fuckup_pillar_confirm_request_sent)
        {
            result.first = true;
            result.second = "[Error]";
        }
        else
        {
            if (id == 0 || id == 1)
            {
                fuckup_pillar_confirm_choice_valid = true;
                fuckup_pillar_confirm_choice = id;
                result.first = true;
                result.second = "[Processing...]";
            }
            else
            {
                result.first = false;
                result.second = "[Invalid choice ID]";
            }
        }
        return result;
    }


    std::pair<bool, std::string> set_change_quest_course_choice(int id)
    {
        std::pair<bool, std::string> result{};

        if (!change_quest_course_request_sent)
        {
            result.first = true;
            result.second = "[Error]";
        }
        else
        {
            if (id == 0 || id == 1)
            {
                change_quest_course_choice_valid = true;
                change_quest_course_choice = id;
                result.first = true;
                result.second = "[Processing...]";
            }
            else
            {
                result.first = false;
                result.second = "[Invalid choice ID]";
            }
        }
        return result;
    }


    std::pair<bool, std::string> set_stealing_choice(int id)
    {
        std::pair<bool, std::string> result{};

        if (!stealing_request_sent)
        {
            result.first = true;
            result.second = "[Error]";
        }
        else
        {
            if (id == 0 || id == 1)
            {
                stealing_choice_valid = true;
                stealing_choice = id;
                result.first = true;
                result.second = "[Processing...]";
            }
            else
            {
                result.first = false;
                result.second = "[Invalid choice ID]";
            }
        }
        return result;
    }



    std::pair<bool, std::string> set_attack_friend_choice(int id)
    {
        std::pair<bool, std::string> result{};

        if (!attack_friend_request_sent)
        {
            result.first = true;
            result.second = "[Error]";
        }
        else
        {
            if (id == 0 || id == 1)
            {
                attack_friend_choice_valid = true;
                attack_friend_choice = id;
                result.first = true;
                result.second = "[Processing...]";
            }
            else
            {
                result.first = false;
                result.second = "[Invalid choice ID]";
            }
        }
        return result;
    }




    std::pair<bool, std::string> set_multiple_path_quest_choice(int id)
    {
        std::pair<bool, std::string> result{};

        if (!multiple_paths_quest_request_sent)
        {
            result.first = true;
            result.second = "[Error]";
        }
        else
        {
            bool found = false;
            for (auto allowed_id : allowed_multiple_paths_quest_choices)
            {
                if (allowed_id == id)
                {
                    found = true;
                    break;
                }
            }

            if (found)
            {
                multiple_paths_quest_choice_valid = true;
                multiple_paths_quest_choice = id;
                result.first = true;
                result.second = "[Processing...]";
            }
            else
            {
                result.first = false;
                result.second = "[Invalid choice ID]";
            }
        }
        return result;
    }




	float walker_processor_timer = 0.0f;



    void lower_processor(float dtime)
    {
        if (backup_pickup)
        {
            if (get_cant_walk_reason() != "")
            {
                reset_backup_pickup();
            }
        }
    }


    void reset_multiple_paths_quest()
    {
        multiple_paths_quest_choice_confirming = false;
        multiple_paths_quest_choice_confirmed = false;
        multiple_paths_quest_request_sent = false;
        multiple_paths_quest_choice_valid = false;
        multiple_paths_quest_choice = -1;
        allowed_multiple_paths_quest_choices.clear();
    }


	void processor(float dtime)
	{

        if (have_target_to_walk && interaction_after_walk != 0 && MiscThings::is_on_horse() && close_enough())
        {
            confirm();
            set_universal_block(1.0f);
            return;
        }


        if (multiple_paths_quest_choice_confirming)
        {
            if (!multiple_paths_quest_request_sent)
            {
                std::vector<MenuOption> options{};

                auto temp_result = MiscThings::get_current_quests(); //refresh

                if (!MiscThings::is_quest_list_valid())
                {
                    reset_multiple_paths_quest();
                    return;
                }


                auto p_quests = MiscThings::get_p_quest_list();

                for (auto a_quest : *p_quests)
                {
                    std::string quest_name = a_quest.name + ": " + a_quest.displaytext + " (" + a_quest.target_name + ")";
                    options.push_back({ a_quest.id, quest_name });
                    allowed_multiple_paths_quest_choices.push_back(a_quest.id);


                }

                if (std::size(options) <= 0)
                {
                    reset_multiple_paths_quest();
                    return;
                }

                unregister_all_actions();

                if (force_choice(options, "This quest offers you a choice. You must specify what path to choose", force_type::specify_quest_path))
                {
                    multiple_paths_quest_request_sent = true;
                }
            }
            else
            {
                if (multiple_paths_quest_choice_valid)
                {
                    int choice_to_remember = multiple_paths_quest_choice;
                    register_allowed_actions();
                    reset_multiple_paths_quest();
                    multiple_paths_quest_choice_confirmed = true;
                    walk_to_quest_by_index(choice_to_remember, false, false); //skip confirm is false so it can clear old choice
                    return;
                }
            }


            return;
        }
        





        if (backup_pickup)
        {
            if (get_cant_walk_reason() != "")
            {
                reset_backup_pickup();
            }
            else
            {
                if (backup_pickup_time > 0.3f)
                {
                    if (MiscThings::is_object_still_valid(backup_pickup_object))
                    {
                        backup_pickup = false;
                        walk_to_object_by_refr(backup_pickup_object, 1);
                        backup_pickup_time = 0.0f;
                    }
                    else
                    {
                        //all good
                        reset_backup_pickup();
                    }
                }
                else
                    backup_pickup_time += dtime;

            }
            
            return;
        }


        if (getting_into_carriage)
        {
            if (getting_into_carriage_time > 20.0f)
            {
                reset_walker();
                register_allowed_actions();
            }
            else
                getting_into_carriage_time = 0.0f;
        }
        else
            getting_into_carriage_time = 0.0f;


        auto ui = RE::UI::GetSingleton();

        reminder_walk_time += dtime;
        

        if (path_record_mode)
        {
            auto player = RE::PlayerCharacter::GetSingleton();
            auto player_pos = player->GetPosition();

            if (std::size(custom_path_record) == 0)
            {
                custom_path_record.push_back(player_pos);
            }
            else
            {
                if (player_pos.GetDistance(custom_path_record.at(std::size(custom_path_record) - 1)) > 100.0f)
                {
                    custom_path_record.push_back(player_pos);
                }
            }

        }


        if (low_mana_detected)
        {
            if (!low_mana_notified)
            {
                low_mana_notified = true;
                send_random_context("[You dont have enough mana to cast the spell! You can try to replenish mana with potions or equip some other weapon instead of spells]", false);
            }
            else
            {
                if (low_mana_notify_time > 10.0f)
                {
                    low_mana_notified = false;
                    low_mana_notify_time = 0.0f;
                    low_mana_detected = false;
                }
                else
                    low_mana_notify_time += dtime;
            }
        }
        /*
        else
        {
            low_mana_notified = false;
            low_mana_notify_time = 0.0f;
        }
        */

        

        try
        {
            auto player = RE::PlayerCharacter::GetSingleton();
            auto player_ref = player->AsReference();

            auto parent_cell = player->GetParentCell();

            auto control_map = RE::ControlMap::GetSingleton();
            bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
            bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
            bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
            bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);


            


            //if (player_actor && !player_actor->movementController->controlsDriven)
            if (!can_walk && !can_look)
            {
                reset_walker();

            }


            if (surrender_mode)
            {
                if (surrender_time > 20.0f)
                {
                    reset_walker();
                }
                else
                    surrender_time += dtime;
            }




            if (do_spins)
            {
                if (spin_timeout > 20.0f)
                {
                    do_spins = false;
                    amount_of_spins = 0;
                    spin_step_one = false;
                    send_random_context("[Spinning done]", false);
                    spin_timeout = 0.0f;
                    register_allowed_actions();
                    return;
                }
                else
                    spin_timeout += dtime;


                auto camera = RE::PlayerCamera::GetSingleton();
                auto camera_dir = camera->cameraRoot.get()->world.rotate;

                auto camera_dirX = camera_dir.GetVectorX();
                auto camera_dirY = camera_dir.GetVectorY();
                auto camera_dirZ = camera_dir.GetVectorZ();

                auto start_camera_dirX = spins_start_camera_dir.GetVectorX();
                auto start_camera_dirY = spins_start_camera_dir.GetVectorY();
                auto start_camera_dirZ = spins_start_camera_dir.GetVectorZ();

                auto difX = camera_dirX - start_camera_dirX;
                auto difY = camera_dirY - start_camera_dirY;
                auto difZ = camera_dirZ - start_camera_dirZ;

                

                if (difX.Length() > 0.3f || difY.Length() > 0.3f || difZ.Length() > 0.3f)
                    spin_step_one = true;

                bool spin_step_two = difX.Length() < 0.1f && difY.Length() < 0.1f && difZ.Length() < 0.1f;

                if (amount_of_spins <= 0)
                {
                    send_random_context("[Spinning done]", false);
                    do_spins = false;
                    amount_of_spins = 0;
                    spin_step_one = false;
                    spin_step_two = false;
                    spin_timeout = 0.0f;

                    register_allowed_actions();
                }
                else
                {
                    if (spin_step_one && spin_step_two)
                    {
                        amount_of_spins--;
                        amount_of_spins_done++;
                        spin_step_one = false;
                        spin_step_two = false;
                        send_random_context("[Did " + std::to_string(amount_of_spins_done) + " spins]", false);


                        if (amount_of_spins_done > 4)
                        {
                            amount_of_spins = 0;
                            spin_timeout = 0.0f;
                            spin_step_one = false;


                            std::string message = "[You feel dizzy";

                            
                            

                            auto caster_123 = player->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant);
                            auto paralysis_spell = RE::TESForm::LookupByID(0x5AD5F)->As<RE::SpellItem>();

                            if (caster_123 && paralysis_spell && !MiscThings::is_on_horse())
                            {

                                auto old_delivery = paralysis_spell->GetDelivery();
                                auto old_duration = paralysis_spell->effects[0]->effectItem.duration;
                                auto old_cost = paralysis_spell->effects[0]->cost;

                                paralysis_spell->SetDelivery(RE::MagicSystem::Delivery::kSelf);
                                paralysis_spell->effects[0]->effectItem.duration = 1;
                                paralysis_spell->effects[0]->cost = 0;

                                caster_123->CastSpellImmediate(paralysis_spell, false, player_ref, 1.0f, false, 1.0f, nullptr);

                                paralysis_spell->SetDelivery(old_delivery);
                                paralysis_spell->effects[0]->effectItem.duration = old_duration;
                                paralysis_spell->data.costOverride = old_cost;

                                //auto event = new RE::TESMagicEffectApplyEvent;
                                //RE::TESMagicEffectApplyEvent event{};
                                //event.caster = player_ref->GetHandle().get();
                                //event.target = event.caster;
                                //event.magicEffect = 0x73f30;
                                //RE::ScriptEventSourceHolder::GetSingleton()->SendEvent<RE::TESMagicEffectApplyEvent>(&event);

                               
                                /*
                                		NiPointer<TESObjectREFR> target;       // 00
		                                NiPointer<TESObjectREFR> caster;       // 08
		                                FormID                   magicEffect;  // 10
		                                std::uint32_t            pad14;        // 14
        */

                                    //BSTEventSource<TESMagicEffectApplyEvent>
                                //RE::BSTEventSink<RE::TESMagicEffectApplyEvent>::ProcessEvent()
                                message += " and fall";
                                unregister_all_actions();
                                set_universal_block(5.0f);

                            }

                            send_random_context(message + "]", false);
                        }
                    }

                    mouse_look(spin_speed * 50.0f, 0.0f);
                }

                return;
            }


            if (!RE::UI::GetSingleton()->IsMenuOpen(RE::TweenMenu::MENU_NAME) && !RE::UI::GetSingleton()->IsMenuOpen(RE::LevelUpMenu::MENU_NAME) && !RE::UI::GetSingleton()->IsMenuOpen(RE::StatsMenu::MENU_NAME) && (init_delay || true))
            {
                //path_valid = true;
                //target_ref = RE::TESObjectREFR::LookupByID(0x7001834)->AsReference();
                //walk_to_point();

                if (search_next_fight_target)
                {
                    auto next_targets = MiscThings::get_player_attackers(false, target_ref);

                    if (std::size(next_targets) > 0)
                    {
                        reset_walker();
                        auto new_target = next_targets.at(0);
                        std::string new_target_name = MiscThings::insert_object_into_list_and_get_info(new_target);

                        right_attack_cancel();
                        left_attack_cancel();

                        chill_with_context = true;

                        walk_to_object_by_refr(new_target, 3);
                        send_random_context("[You started fighting next target: " + new_target_name + "]", false);
                    }
                    else
                    {
                        if (search_next_target_timer > 1.0f)
                        {
                            std::string result_header = "[Fight ended";

                            if (was_already_dead || (target_ref && !target_ref->IsActor()))
                                result_header = "[You finished attacking " + reminder_target_name;

                            Observer::reset_threats();

                            reset_walker();
                            std::string advice = "";

                            if (MiscThings::is_objects_around_valid())
                                advice = "walk somewhere, interact with something, loot the bodies of defeated";

                            if (MiscThings::have_any_quests())
                                if (advice != "")
                                    advice += ", follow some quest";
                                else
                                    advice = "follow some quest";


                            if (advice != "")
                                advice = "You can " + advice;

                            

                            if (!shout_mode)
                                send_random_context(result_header + ". Choose next action to do. " + advice + "]", false);
                        }
                        else
                            search_next_target_timer += dtime;
                    }

                    return;
                }
                

                if (ustengrev_finish_line_mode)
                {
                    if (walk_fixed_time(true, 7.77f, dtime))
                    {
                        auto player_pos = player->GetPosition();
                        if (player_pos.x > 600.0f)
                        {
                            send_random_context("You passed the gate challenge!", false);
                            quicksave();
                        }
                        else
                        {
                            send_random_context("You didnt make it through the gates... Maybe try different approach?");
                        }

                        reset_walker();
                    }
                    return;
                }



                if (trying_to_attack_friend)
                {
                    if (!attack_friend_request_sent)
                    {
                        std::vector<MenuOption> options{};
                        options.push_back({ 0, "No" });
                        options.push_back({ 1, "Yes" });

                        unregister_all_actions();

                        if (force_choice(options, "Are you sure you want to attack " + attack_friend_name + "? They are not your enemy.", force_type::confirm_attack_friend))
                        {
                            attack_friend_request_sent = true;
                        }
                    }
                    else
                    {
                        if (attack_friend_choice_valid)
                        {
                            register_allowed_actions();
                            if (attack_friend_choice)
                            {


                                attack_friend_confirmed = true;

                                auto for_context = walk_to_object_by_index(attack_friend_id, attack_friend_interaction);

                                trying_to_attack_friend = false;
                                attack_friend_request_sent = false;
                                attack_friend_choice_valid = false;
                                attack_friend_choice = -1;
                                attack_friend_interaction = -1;
                                attack_friend_id = -1;
                                attack_friend_confirmed = false;

                                send_random_context(for_context.second);

                            }
                            else
                            {
                                //continue doing whatever we were doing
                                trying_to_attack_friend = false;
                                attack_friend_request_sent = false;
                                attack_friend_choice_valid = false;
                                attack_friend_choice = -1;
                                attack_friend_interaction = -1;
                                attack_friend_id = -1;
                                attack_friend_confirmed = false;

                            }
                        }
                    }

                    if (!is_fighting())
                        return;
                }




                if (trying_to_change_quest_course)
                {
                    if (!change_quest_course_request_sent)
                    {
                        std::vector<MenuOption> options{};
                        options.push_back({ 0, "No" });
                        options.push_back({ 1, "Yes" });

                        unregister_all_actions();

                        if (force_choice(options, "Previously you were following another quest: " + last_quest_chosen_name + ". Are you sure you want to change course and follow new quest: " + new_quest_chosen_name + "?", force_type::confirm_change_quest_course))
                        {
                            change_quest_course_request_sent = true;
                        }
                    }
                    else
                    {
                        if (change_quest_course_choice_valid)
                        {
                            register_allowed_actions();
                            if (change_quest_course_choice)
                            {
                                trying_to_change_quest_course = false;
                                change_quest_course_choice_valid = false;
                                change_quest_course_choice = -1;
                                change_quest_course_request_sent = false;

                                last_quest_chosen = new_quest_chosen; //so walk_to_quest function doesnt panic
                                auto for_context = walk_to_quest_by_index(get_quest_id_by_refr(new_quest_chosen), false);

                                send_random_context(for_context.second);

                            }
                            else
                            {
                                //continue doing whatever we were doing
                                trying_to_change_quest_course = false;
                                change_quest_course_choice_valid = false;
                                change_quest_course_choice = -1;
                                change_quest_course_request_sent = false;
                            }
                        }
                    }

                    if (!is_fighting())
                        return;
                }





                if (target_ref && (!target_ref->data.objectReference || target_ref == player_ref))
                    reset_walker();

                RE::ObjectRefHandle my_handle{};
                if (target_ref)
                    my_handle = target_ref->GetHandle();

                if (target_ref && (!my_handle || !my_handle.get() || !my_handle.get().get()))
                    reset_walker();



                //700af26 - cut sphere
                //700af27 - xmarker
                //15d48 - door

                //RE::TESObjectREFR* cut_sphere = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x15d48); //lets leave it on forever for now

                RE::TESObjectREFR* alftand_door = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x15d48);
                RE::TESObjectREFR* redirect_marker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x700af27);

                if (target_ref == alftand_door)
                {
                    //doing some redirections 

                    if (!MiscThings::player_inside_of_alftand_goodbox())
                    {
                        if (alftand_counter == 5)
                        {
                            dont_check_quest_target_change = true;
                            target_ref = redirect_marker;
                            walk_again();
                            return;
                        }
                        else
                            alftand_counter++;

                    }
                }
                else
                {
                    if (target_ref == redirect_marker)
                    {
                        if (MiscThings::player_inside_of_alftand_goodbox())
                        {
                            if (alftand_counter == 0)
                            {
                                reset_walker();
                                unregister_all_actions();
                                target_ref = alftand_door;
                                have_target_to_walk = true;
                                using_custom_path = true;
                                walk_again_when_finished = true;
                                custom_path = { player->GetPosition(), alftand_door->GetPosition()};
                                return;
                            }
                            else
                                alftand_counter--;
                        }
                            
                    }
                }
                




                if (pause_pre_stealing)
                {
                    //crouch pause
                    lock_camera_onto_target(target_ref, dtime);

                    if (pause_pre_stealing_time > 0.5f)
                    {
                        pause_pre_stealing_time = 0.0f;
                        pause_pre_stealing = false;
                    }
                    else
                        pause_pre_stealing_time += dtime;

                    return;
                }

                if (confirm_stealing)
                {
                    if (!stealing_request_sent)
                    {
                        std::vector<MenuOption> options{};
                        options.push_back({ 0, "No" });
                        options.push_back({ 1, "Yes" });

                        unregister_all_actions();

                        if (force_choice(options, "You are trying to steal something, but you cannot do it sneakily. Somebody sees you. Do you want to take it anyway? They might not like that...", force_type::confirm_stealing))
                        {
                            stealing_request_sent = true;
                        }
                        return;
                    }
                    else
                    {
                        if (stealing_choice_valid)
                        {
                            register_allowed_actions();

                            if (stealing_choice)
                            {
                                stealing_confirmed = true;
                            }
                            else
                            {
                                reset_walker();
                                return;
                            }
                        }
                        else
                            return;
                    }
                }


                if (just_teleported && !MiscThings::is_loading())
                {
                    just_teleported_timeout += dtime;

                    if (just_teleported_timeout > 5.0f)
                    {
                        just_teleported_timeout = 0.0f;
                        just_teleported = false;
                    }
                }
                else
                {
                    just_teleported_timeout = 0.0f;
                }

                if (make_clairvoyance_cast)
                {
                    if (MiscThings::is_loading() || just_teleported)
                        return;


                    //TODO: what if we didnt finish and processor stops?

                    bool cast_result = cast_pathfinding(dtime);

                    if (cast_result)
                        make_clairvoyance_cast = false;
                }


                if (attack_paused)
                {
                    if (start_attacking)
                    {
                        lock_camera_onto_target(target_ref, dtime);
                    }
                    return;
                }



                



                if (have_target_to_walk)
                {

                    auto parthurnax_walk_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ204");

                    auto parthurnax_target = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x34e03);
                    auto parthurnax_target2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x3c57d);
                    auto parthurnax_target3 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x9413e);

                    if (parthurnax_walk_quest)
                    {
                        auto parthurnax_walk_quest_stage = parthurnax_walk_quest->GetCurrentStageID();

                        if (quest_mode && (target_ref == parthurnax_target || target_ref == parthurnax_target2 || target_ref == parthurnax_target3) && !shout_mode && (parthurnax_walk_quest_stage == 86 || parthurnax_walk_quest_stage == 90))
                        {
                            auto hazard_0 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x105375);
                            auto hazard_1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x105965);
                            auto hazard_2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x10f764);

                            auto player = RE::PlayerCharacter::GetSingleton();

                            if (player)
                            {
                                auto player_pos = player->GetPosition();
                                auto clear_skies_shout = (RE::TESShout*)RE::TESForm::LookupByID(0x3CD34);

                                if (MiscThings::player_has_spell((RE::SpellItem*)clear_skies_shout))
                                {

                                    if (hazard_0)
                                    {
                                        auto hazard_0_pos = hazard_0->GetPosition();
                                        auto hazard_0_blizzard = MiscThings::get_linked_ref_children(hazard_0, 0);

                                        if (hazard_0_blizzard && !hazard_0_blizzard->IsDisabled())
                                        {
                                            if (player_pos.GetDistance(hazard_0_pos) < 1200.0f)
                                            {
                                                WalkerProcessor::shout_at_target(hazard_0_blizzard, clear_skies_shout);
                                            }
                                        }
                                    }

                                    if (hazard_1)
                                    {
                                        auto hazard_1_pos = hazard_1->GetPosition();
                                        //auto hazard_1_blizzard = MiscThings::get_linked_ref_children(hazard_1, 0);

                                        if (!hazard_1->IsDisabled())
                                        {
                                            if (player_pos.GetDistance(hazard_1_pos) < 1200.0f)
                                            {
                                                WalkerProcessor::shout_at_target(hazard_1, clear_skies_shout);
                                            }
                                        }
                                    }

                                    if (hazard_2)
                                    {
                                        auto hazard_2_pos = hazard_2->GetPosition();
                                        //auto hazard_2_blizzard = MiscThings::get_linked_ref_children(hazard_2, 0);

                                        if (!hazard_2->IsDisabled())
                                        {
                                            if (player_pos.GetDistance(hazard_2_pos) < 1200.0f)
                                            {
                                                WalkerProcessor::shout_at_target(hazard_2, clear_skies_shout);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    






                    if (interaction_after_walk > 1 && interaction_after_walk <= 3)
                        walker_inactive_time = 0.0f;
                    else
                        walker_inactive_time += dtime;

                    if (MiscThings::is_intro() || looking_mode)
                    {
                        if (lock_camera_onto_target(target_ref, dtime, true))
                        {
                            reset_walker();
                        }
                        else
                        {
                            if (intro_look_timeout > 2.0f)
                            {
                                reset_walker();
                            }
                            else
                                intro_look_timeout += dtime;
                        }

                        return;
                    }


                    if (trying_to_fuckup_correct_pillar)
                    {
                        if (!fuckup_pillar_confirm_request_sent)
                        {
                            std::vector<MenuOption> options{};
                            options.push_back({ 0, "No" });
                            options.push_back({ 1, "Yes" });

                            if (force_choice(options, "It is already in correct position. Are you sure you want to touch it?", force_type::confirm_pillar))
                            {
                                fuckup_pillar_confirm_request_sent = true;
                            }
                        }
                        else
                        {
                            if (fuckup_pillar_confirm_choice_valid)
                            {
                                if (fuckup_pillar_confirm_choice)
                                {
                                    trying_to_fuckup_correct_pillar = false;
                                    fuckup_pillar_confirm_request_sent = false;
                                    fuckup_pillar_confirm_choice_valid = false;
                                    fuckup_pillar_confirm_choice = 0;
                                }
                                else
                                {
                                    reset_walker();
                                }
                            }
                        }

                        return;
                    }
                    

                    backup_input_cancel = false;

                    
                    if (got_close_for_pickpocket)
                    {
                        auto actor_obj = (RE::Actor*)target_ref;
                        bool is_walking = actor_obj->IsWalking() || actor_obj->actorState1.movingBack; //movingBack is true when they walk forward (sometimes?)

                        if (lock_camera_onto_target(target_ref, dtime) && is_walking)
                            walk_forward();// cursor_up();

                        //lock_camera_onto_target(target_ref, dtime);
                        if (lock_camera_onto_target(target_ref, dtime) && close_enough())
                        {
                            interact_with_target(dtime);

                            pickpocket_timeout += dtime;

                            if (pickpocket_timeout > 5.0f)
                            {
                                reset_walker();
                                send_random_context("You cannot find a good position for pickpocket", false);
                                return;
                            }
                        }
                        else
                        {
                            if (!close_enough())
                                walk_again();
                        }
                            
                        return;
                    }
                    

                    if (too_high_notified && !(interaction_after_walk == 3))// && close_enough())) //attacks handled differently
                    {
                        if (target_is_too_high())
                        {
                            lock_camera_onto_target(target_ref, dtime);
                            return;
                        }
                        else
                            walk_again();
                    }


                    if (start_attacking)
                    {
                        if (attack_target(dtime))
                        {
                            attacking_done = true;
                            start_attacking = false;
                        }
                    }
                    else
                        bool stop_here = false;





                    auto player = RE::PlayerCharacter::GetSingleton();
                    auto player_actor = (RE::Actor*)player->AsReference();

                    if (player_actor && (!shout_mode && player_actor->IsWeaponDrawn() || player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing) && interaction_after_walk != 3 && !input_wants_to_cast())
                    {
                        if (!tried_to_draw_weapon1 || draw_weapon_check_time1 > 2.0f)
                        {
                            draw_weapon_check_time1 = 0.0f;
                            ready_weapon();
                            tried_to_draw_weapon1 = true;
                        }
                        else
                            draw_weapon_check_time1 += dtime;   
                    }
                    else
                    {
                        tried_to_draw_weapon1 = false;
                        draw_weapon_check_time1 = 0.0f;
                    }

                    if (!shout_mode && player_actor && !player_actor->IsWeaponDrawn() && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing) && interaction_after_walk == 3)
                    {
                        if (!tried_to_draw_weapon2 || draw_weapon_check_time2 > 2.0f)
                        {
                            draw_weapon_check_time2 = 0.0f;
                            ready_weapon();
                            tried_to_draw_weapon2 = true;
                        }
                        else
                            draw_weapon_check_time2 += dtime;
                    }
                    else
                    {
                        tried_to_draw_weapon2 = false;
                        draw_weapon_check_time2 = 0.0f;
                    }



                    if (catch_door_result)
                    {
                        if (catch_door_result_time < 1.0f)
                        {
                            catch_door_result_time += dtime;

                            if (!get_targeted_ref() || !is_targeted_door_locked()) //we still have the door targeted and it is unlocked now. or it was unlocked.
                            {
                                //door is opened (now?)
                                register_allowed_actions();
                                catch_door_result = false;
                                if (!get_targeted_ref() || is_door(get_targeted_ref()))
                                {
                                    if (!get_targeted_ref() || target_ref == get_targeted_ref())
                                    {
                                        if (target_ref == get_targeted_ref() && is_door(target_ref) && (MiscThings::get_door_teleport(target_ref) != "") && quest_mode)
                                        {
                                            just_teleported = true;
                                            catch_door_result = false;
                                            catch_door_result_time = 0.0f;
                                            set_universal_block(1.5f);
                                        }
                                        else
                                            reset_walker();//;// walk_forward_a_little = true;
                                    }
                                    else
                                    {
                                        if (backup_interaction_made)
                                            backup_interaction_made = false; 

                                        catch_door_result = false; //proceed whatever we did before after pause
                                        catch_door_result_time = 0.0f;
                                        set_universal_block(1.5f);
                                    }
                                        
                                }
                                    
                            }

                            std::string result = "";

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
                                                                    {
                                                                        std::string result_string = subvar.GetString();
                                                                        if (result_string.find("You are out of lockpicks") || result_string.find("This lock cannot be picked")) //no lockpicks or no key //TODO: GET FULL MESSAGES
                                                                        {
                                                                            if (result_string != "")
                                                                                MiscThings::update_old_topleft_nofification(result_string);

                                                                            register_allowed_actions();
                                                                            send_random_context("[" + result_string + "]", false);
                                                                            reset_walker();
                                                                        }
                                                                    }
                                                                        
                                                }
                                            }
                        }
                        else
                        {
                            //send_random_context("[Interaction with object failed]", false);
                            reset_walker();
                            register_allowed_actions();
                        }
                            

                        return;
                    }



                    if (try_unstuck)
                    {
                        if (walk_unstuck(dtime))
                        {
                            walk_unstuck_time = 0.0f;
                            try_unstuck = false;
                            walk_again();
                        }
                        return;
                    }



                    if (wiggle_body_then_walk_again)
                    {
                        

                        //if (walk_fixed_time(true, 0.2f, dtime))
                        if (walk_unstuck(dtime))
                        {
                            walk_unstuck_time = 0.0f;
                            wiggle_body_then_walk_again = false;
                            walk_again();
                        }
                            

                        return;
                    }


                    if (walk_forward_a_little)
                    {
                        if (walk_fixed_time(true, 0.7f, dtime))
                            reset_walker();
                    }
                    else
                    {
                        if (walk_backward_a_little)
                        {
                            if (walk_fixed_time(false, 0.7f, dtime))
                                reset_walker();
                        }
                        else
                        {

                            if (looking_mode || path_valid || use_last_point_of_last_path)
                            {
                                if (!use_last_point_of_last_path && (current_path_point == -1))
                                {
                                    if (!looking_mode && std::size(path) > 0)
                                    {
                                        current_path_point = 0;
                                        last_point_posZ = path.at(current_path_point).z;
                                    }
                                    else
                                    {
                                        if (!looking_mode && (int)std::size(path) < 3 && ((walk_retries < 4 && !location_mode && !quest_mode) || (walk_retries < 10 && (location_mode || quest_mode)))) //IF IT BROKE - CHECK THIS
                                        {
                                            wiggle_body_then_walk_again = true;

                                            if (MiscThings::is_dragon(target_ref) && MiscThings::is_flying(target_ref) && MiscThings::is_fighting_dragons_allowed())
                                            {
                                                if (walk_retries > 2)
                                                {
                                                    send_random_context("You keep following the dragon...", false); 
                                                    walk_retries = 0;
                                                }
                                                
                                            }
                                                

                                            walk_retries++;
                                            //walk_again();

                                            return;
                                        }
                                    }
                                }


                                
                                //if (path_point_reached() || (!using_custom_path && close_enough() && (current_path_point > (int)std::size(path) - 5)) || (close_enough() && interaction_after_walk == 3) || MiscThings::is_intro())
                                if (looking_mode || MiscThings::is_intro() || path_point_reached(dtime) || (!using_custom_path && close_enough() && (current_path_point > (int)std::size(path) - 5)) || (close_enough() && interaction_after_walk == 3))
                                {
                                    path_point_reached_timeout = 0.0f;
                                    //time_stuck = 0.0f;

                                    if (use_last_point_of_last_path)
                                    {
                                        if (path_valid)
                                        {
                                            if (!silly_walk_mode)
                                                walk_to_point(dtime);
                                            use_last_point_of_last_path = false;
                                            current_path_point = -1;
                                            return; //use new path
                                        }
                                        else
                                        {
                                            if (!detect_stuck(dtime))
                                            {
                                                walk_to_point(dtime);
                                                //cast_pathfinding(dtime); //rebuild path
                                                make_clairvoyance_cast = true;
                                            }
                                            else
                                            {
                                                //stuck detected. wiggle body.
                                                if (unstuck_attempts < 5)
                                                {
                                                    time_stuck = 0.0f;
                                                    try_unstuck = true;
                                                    unstuck_attempts++;
                                                }
                                                else
                                                {
                                                    std::string fail_text = "[Cant walk there. Maybe this target is not accessible. Do something else or try again later]";
                                                    std::string potential_block = MiscThings::get_potential_blocking_object();
                                                    if (potential_block != "")
                                                    {
                                                        fail_text = "[ " + potential_block + " blocks the path. ";

                                                        if (potential_block.find("estructib") != std::string::npos)
                                                            fail_text += "You can try attacking it to destroy it]";
                                                        else
                                                            fail_text += "Maybe you need to interact with something nearby to go past it]";
                                                    }
                                                    else
                                                    {
                                                        if (!MiscThings::is_serving_jail())
                                                            Observer::detect_interesting_objects(0.016, true);
                                                    }

                                                    send_random_context(fail_text, false);
                                                    reset_walker();
                                                }
                                            }
                                            return; //wait for path to be built
                                        }

                                    }

                                    if (!use_last_point_of_last_path && (current_path_point < (int)std::size(path)))
                                    {
                                        if (current_path_point >= 0 && std::size(path) > 0)
                                        {
                                            last_point_posZ = path.at(current_path_point).z;
                                            current_path_point++;
                                            correct_marker_pos();

                                            if (current_path_point < std::size(path))
                                                if (!silly_walk_mode)
                                                    walk_to_point(dtime);

                                        }
                                        else
                                        {
                                            if ((int)std::size(path) < 3 && ((walk_retries < 4 && !location_mode && !quest_mode) || (walk_retries < 10 && (location_mode || quest_mode)))) //IF IT BROKE - CHECK THIS
                                            {
                                                wiggle_body_then_walk_again = true;

                                                walk_retries++;
                                                //walk_again();

                                                if (MiscThings::is_dragon(target_ref) && MiscThings::is_flying(target_ref) && MiscThings::is_fighting_dragons_allowed())
                                                {
                                                    if (walk_retries > 2)
                                                    {
                                                        send_random_context("You keep following the dragon...", false);
                                                        walk_retries = 0;
                                                    }

                                                }

                                                return;
                                            }
                                        }
                                    }


                                    if (quest_mode && last_quest && last_quest_objective && detect_quest_target_changed_and_walk())
                                    {
                                        had_successful_walk = false;
                                        return;
                                    }


                                    if (!using_custom_path && !use_last_point_of_last_path && ((int)std::size(path) > 5) && (current_path_point == std::size(path) - 1) && !close_enough()) //prelast point. need to rebuild next path segment before we reach the end.
                                    {
                                        if (std::size(path) > 0)
                                        {
                                            use_last_point_of_last_path = true;
                                            last_point_of_last_path = path.at((int)std::size(path) - 1);
                                            path.clear();
                                            current_path_point = -1;
                                            path_valid = false;
                                            correct_marker_pos();
                                            //cast_pathfinding(dtime); //rebuild path
                                            make_clairvoyance_cast = true;
                                        }
                                        
                                    }
                                    else
                                    {
                                        //if (use_last_point_of_last_path || current_path_point >= (int)std::size(path) || (close_enough() && !using_custom_path) || MiscThings::is_intro())
                                        if (looking_mode || MiscThings::is_intro() || use_last_point_of_last_path || (std::size(path) == 0) || current_path_point >= (int)std::size(path) || (close_enough() && !using_custom_path && (current_path_point > (int)std::size(path) - 5)))
                                        {
                                            dead_point_time = 0.0f;
                                            if (reset_after_walk)
                                            {
                                                register_allowed_actions();
                                                reset_walker();
                                                return;
                                            }
                                               

                                            if (walk_again_when_finished)
                                            {



                                                if (ustengrev_run_only_mode)
                                                {
                                                    if (lock_camera_onto_target(target_ref, dtime))
                                                    {
                                                        if (ustengrev_run_condition(dtime))
                                                        {
                                                            send_random_context("You running towards the gates through keystones...");
                                                            using_custom_path = false; //prevent quicksave
                                                            reset_walker();
                                                            ustengrev_finish_line_mode = true;
                                                            
                                                        }
                                                    }

                                                    return;
                                                }

                                                if (ustengrev_wrong_order_mode)
                                                {
                                                    if (lock_camera_onto_target(target_ref, dtime))
                                                    {
                                                        if (ustengrev_run_condition(dtime))
                                                        {
                                                            send_random_context("You using shout and try to run through gates afterwards...");
                                                            auto shout_form = (RE::TESShout*)RE::TESForm::LookupByID(0x2f7ba);
                                                            MiscThings::cast_spell_by_refr((RE::SpellItem*)shout_form, true);
                                                            using_custom_path = false; //prevent quicksave
                                                            //ustengrev_finish_line();
                                                            reset_walker();
                                                            ustengrev_finish_line_mode = true;
                                                            return;

                                                        }
                                                    }

                                                    return;
                                                }


                                                if (ustengrev_get_ready_mode)
                                                {
                                                    if (lock_camera_onto_target(target_ref, dtime))
                                                    {
                                                        if (ustengrev_run_condition(dtime))
                                                        {
                                                            ustengrev_puzzle_run_and_shout();
                                                            return;
                                                        }
                                                    }
                                                }
                                                else
                                                {
                                                    if (ustengrev_shout_mode)
                                                    {
                                                        walk_forward();
                                                        //if (lock_camera_onto_target(target_ref, dtime))
                                                        {
                                                            //walk_forward();
                                                            auto shout_form = (RE::TESShout*)RE::TESForm::LookupByID(0x2f7ba);
                                                            send_random_context("You are using the shout...");
                                                            MiscThings::cast_spell_by_refr((RE::SpellItem*)shout_form, true);
                                                            using_custom_path = false; //prevent quicksave
                                                            //ustengrev_finish_line();
                                                            reset_walker();
                                                            ustengrev_finish_line_mode = true;
                                                            return;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        using_custom_path = false;
                                                        register_allowed_actions();
                                                        quicksave();
                                                        walk_again();
                                                    }
                                                }
                                            }
                                                

                                            if (got_close_for_pickpocket || close_enough() || MiscThings::is_intro() || looking_mode)
                                            {
                                                if (interaction_after_walk == 2)
                                                {
                                                    got_close_for_pickpocket = true;
                                                }

                                                had_successful_walk = true;
                                                unstuck_attempts = 0;

                                                if (!try_to_remove_obstacle_mode)
                                                {
                                                    if (quest_mode && last_quest && last_quest_objective && detect_quest_target_changed_and_walk())
                                                    {
                                                        had_successful_walk = false;
                                                        return; 
                                                    }
                                                        
                                                    if (get_targeted_ref() == target_ref)
                                                    {
                                                        stable_target++;
                                                    }
                                                    else
                                                        stable_target = 0;

                                                    if (looking_mode || MiscThings::is_intro() || locking_failed || ((get_targeted_ref() == target_ref) && stable_target > 2) || lock_camera_onto_target(target_ref, dtime) || location_mode)
                                                    {
                                                        auto result_target = get_targeted_ref();

                                                        //if (!result_target && ((interaction_after_walk == 3 && has_ranged_weapon_equipped(true)) || shout_mode))
                                                        if (!result_target && ((interaction_after_walk == 3) || shout_mode))
                                                        {
                                                            result_target = target_ref;
                                                        }


                                                        //

                                                        if (looking_mode || MiscThings::is_intro() || MiscThings::is_intro2() || location_mode || (result_target == target_ref) || !target_is_interactive())//(quest_mode && !target_is_interactive()))
                                                        {
                                                            //all good

                                                            if (result_target == target_ref)
                                                                locking_failed = false;


                                                            if (!walk_finished_context_sent)
                                                            {
                                                                walk_finished_context_sent = true;
                                                                if (target_ref)
                                                                {
                                                                    //std::string target_name = target_ref->GetDisplayFullName();
                                                                    if (!chill_with_context && !looking_mode)
                                                                        send_random_context(get_success_message(), true);
                                                                }

                                                            }
                                                            if (interaction_after_walk > 0)
                                                            {
                                                                if (paused_before_interaction)
                                                                {
                                                                    if ((quest_mode && backup_interaction_made) || interact_with_target(dtime))
                                                                    {
                                                                        //if it was a door, walk forward a little after its opened
                                                                        if (interaction_after_walk == 1 && (is_door(result_target) || is_container(result_target)))
                                                                            catch_door_result = true; //this will reset after its done
                                                                        else
                                                                        {
                                                                            if (interaction_after_walk == 3)
                                                                            {
                                                                                //reset_walker();
                                                                                search_next_fight_target = true;
                                                                                    
                                                                            }
                                                                            else
                                                                            {
                                                                                if (!quest_mode)
                                                                                    reset_walker();
                                                                                else
                                                                                {
                                                                                    backup_interaction_made = true;//dont reset in quest mode. the target might be moving and interaction might have done nothing.

                                                                                    backup_interaction_time += dtime;

                                                                                    if (backup_interaction_time > 10.0f)
                                                                                        reset_walker();

                                                                                    lock_camera_onto_target(target_ref, dtime, true);

                                                                                }
                                                                                    
                                                                            }
                                                                                

                                                                        }
                                                                            
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    //clear_input_queue();
                                                                    paused_before_interaction = true;
                                                                    //set_universal_block(0.0f);
                                                                }


                                                            }
                                                            else
                                                            {
                                                                if (ustengrev_get_ready_mode)
                                                                {
                                                                    if (lock_camera_onto_target(target_ref, dtime))
                                                                    {
                                                                        if (ustengrev_run_condition(dtime))
                                                                        {
                                                                            ustengrev_puzzle_run_and_shout();
                                                                            return;
                                                                        }
                                                                    }
                                                                }
                                                                else
                                                                {
                                                                    if (ustengrev_shout_mode)
                                                                    {
                                                                        walk_forward();
                                                                        //if (lock_camera_onto_target(target_ref, dtime))
                                                                        {
                                                                            auto shout_form = (RE::TESShout*)RE::TESForm::LookupByID(0x2f7ba);
                                                                            send_random_context("You are using the shout...");
                                                                            MiscThings::cast_spell_by_refr((RE::SpellItem*)shout_form, true);
                                                                            //ustengrev_finish_line();
                                                                            reset_walker();
                                                                            ustengrev_finish_line_mode = true;
                                                                            return;
                                                                        }
                                                                    }
                                                                    else
                                                                        if (!locking_failed)
                                                                        {
                                                                            if (location_mode)// || MiscThings::is_intro())
                                                                                reset_walker();
                                                                            else
                                                                            {
                                                                                if (target_ref->IsActor() && !close_enough())
                                                                                    walk_again(); //follow it if its actor;
                                                                                else
                                                                                {
                                                                                    lock_camera_onto_target(target_ref, dtime); //keep camera locked in until the walker is reset externally if no action specified
                                                                                    
                                                                                    if (quest_mode && result_target)
                                                                                    {
                                                                                        bool dont_autointerract = false;

                                                                                        if (target_ref && target_ref->IsActor())
                                                                                        {
                                                                                            auto target_actor = (RE::Actor*)target_ref;

                                                                                            if (target_actor->boolFlags.all(RE::Actor::BOOL_FLAGS::kScenePackage))
                                                                                                dont_autointerract = true;
                                                                                            
                                                                                        }

                                                                                        //attempts to not autointerract with npc's that want us to follow them. interaction will stop walking and start (probably useless) dialogue
                                                                                        if (!dont_autointerract)
                                                                                            interaction_after_walk = 1; //interact if there is something and it was quest
                                                                                        else
                                                                                        {
                                                                                            //this also refreshes on walk_again so if we are actually following someone it will keep doing it
                                                                                            backup_interaction_time += dtime;

                                                                                            if (backup_interaction_time > 10.0f)
                                                                                                reset_walker();
                                                                                        }


                                                                                    }
                                                                                    if (explore_mode)
                                                                                        explore_mode = false; //so we can explore again
                                                                                }
                                                                            }
                                                                        }
                                                                }
                                                                    
                                                            }

                                                                    
                                                        }
                                                        else
                                                        {
                                                            //something isnt right
                                                            //locking on target doesnt help.
                                                            //maybe its a door?
                                                            //if its an object that can be moved - move it 
                                                            //TODO: if not movable - try to wiggle around or walk around

                                                            if (confirming_closed_door_interaction)
                                                            {
                                                                if (door_is_closed_choice_valid)
                                                                {
                                                                    if (door_is_closed_choice)
                                                                    {
                                                                        confirm(); //lockpick it
                                                                        catch_door_result = true;
                                                                        //set_universal_block(1.5f); //wait a little. then it should lock again and maybe it will lead to success.
                                                                    }
                                                                    else
                                                                    {
                                                                        register_allowed_actions();
                                                                        //cut_navmesh_on_target(get_targeted_ref());
                                                                        walk_backward_a_little = true;
                                                                        //reset_walker(); //resets after walk
                                                                    }
                                                                }
                                                            }
                                                            else
                                                            {
                                                                if (!confirming_closed_door_interaction && (is_door(result_target) || is_container(result_target)) && is_targeted_door_locked())
                                                                {
                                                                    
                                                                    
                                                                    {
                                                                        auto attackers = MiscThings::get_player_attackers();
                                                                        if (std::size(attackers) > 0)
                                                                        {
                                                                            confirm(); //lockpick it
                                                                        }
                                                                        else
                                                                        {
                                                                            if (MiscThings::is_door_superlocked(target_ref))
                                                                            {
                                                                                send_random_context("Cannot lockpick this door. It requires a key", false);
                                                                                reset_walker();
                                                                                return;
                                                                            }

                                                                            if (MiscThings::get_picks_amount_int() <= 0)
                                                                            {
                                                                                send_random_context("You dont have any lockpicks to open the lock", false);

                                                                                if (is_door(result_target))
                                                                                    cut_navmesh_on_target(result_target);

                                                                                reset_walker();
                                                                                return;
                                                                            }

                                                                            unregister_all_actions();
                                                                            if (is_door(result_target))
                                                                            {
                                                                                if (force_choice({ {0, "No"}, {1, "Yes"} }, get_locked_door_force_message(result_target), force_type::closed_door_choice))
                                                                                    confirming_closed_door_interaction = true;
                                                                            }
                                                                            else
                                                                                if (force_choice({ {0, "No"}, {1, "Yes"} }, get_locked_container_force_message(result_target), force_type::closed_door_choice))
                                                                                    confirming_closed_door_interaction = true;
                                                                        }
                                                                    }

                                                                }
                                                                else
                                                                {
                                                                    if (is_door(result_target) && !is_targeted_door_locked() && opening_door_attempts <= 3)
                                                                    {
                                                                        opening_door_attempts++;
                                                                        confirm(); //open the door
                                                                        set_universal_block(1.5f);
                                                                    }  
                                                                    else
                                                                    {
                                                                        if (result_target && !move_obstacle_failed)
                                                                            if (result_target->CanBeMoved())
                                                                            {
                                                                                bool is_target_an_alive_actor = false;
                                                                                if (result_target->IsActor() && !result_target->IsDead())
                                                                                    is_target_an_alive_actor = true;

                                                                                if (!is_target_an_alive_actor)
                                                                                {
                                                                                    if (attempts_to_move_obstacle < 3)
                                                                                        try_to_remove_obstacle_mode = true;
                                                                                    else
                                                                                        move_obstacle_failed = true;
                                                                                }
                                                                                else
                                                                                    move_obstacle_failed = true; //cant move alive actors

                                                                                locking_failed = false;
                                                                            }
                                                                            else
                                                                            {
                                                                                if (interaction_after_walk == 3)
                                                                                {
                                                                                    reset_walker();
                                                                                    auto next_targets = MiscThings::get_player_attackers();

                                                                                    if (std::size(next_targets) > 0)
                                                                                    {
                                                                                        auto new_target = next_targets.at(std::size(next_targets) - 1);
                                                                                        std::string new_target_name = MiscThings::insert_object_into_list_and_get_info(new_target);
                                                                                        right_attack_cancel();
                                                                                        left_attack_cancel();
                                                                                        walk_to_object_by_refr(new_target, 3);
                                                                                        send_random_context("[You started fighting next target: " + new_target_name + "]", false);
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                        search_next_fight_target = true;
                                                                                        //path_is_blocked_result(result_target);
                                                                                    }

                                                                                }
                                                                                else
                                                                                {
                                                                                    path_is_blocked_result(result_target);
                                                                                }
                                                                                
                                                                            }
                                                                        else
                                                                        {
                                                                            if (!tried_to_come_closer)
                                                                            {
                                                                                if (backup_pickup_attempts <= 1 && !walk_fixed_time(true, 0.4f, dtime)) //was 0.2, revert if bad
                                                                                {
                                                                                    ;
                                                                                }
                                                                                else
                                                                                    tried_to_come_closer = true;
                                                                            }
                                                                            else
                                                                            {
                                                                                //no target. probably a wall blocking. wiggle camera a little trying to catch the thing. and if fail - its a wall.
                                                                                                                                                            //TODO: figure out that its a wall and the thing is behind the corner, walk around it
                                                                                locking_failed = true;
                                                                                //check if we caught it

                                                                                if (wiggle_camera(dtime)) //OTHERWISE WE WILL CATCH IT ABOVE
                                                                                {
                                                                                    //timeout
                                                                                    if (lock_camera_onto_target(target_ref, dtime)) //last focus
                                                                                    {
                                                                                        if (interaction_after_walk == 3)
                                                                                        {
                                                                                            reset_walker();
                                                                                            auto next_targets = MiscThings::get_player_attackers();

                                                                                            if (std::size(next_targets) > 0)
                                                                                            {
                                                                                                auto new_target = next_targets.at(std::size(next_targets) - 1);
                                                                                                std::string new_target_name = MiscThings::insert_object_into_list_and_get_info(new_target);
                                                                                                right_attack_cancel();
                                                                                                left_attack_cancel();
                                                                                                walk_to_object_by_refr(new_target, 3);
                                                                                                send_random_context("[You started fighting next target: " + new_target_name + "]", false);
                                                                                            }
                                                                                            else
                                                                                            {
                                                                                                search_next_fight_target = true;
                                                                                                //path_is_blocked_result(result_target);
                                                                                            }

                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            result_target = get_targeted_ref();
                                                                                            path_is_blocked_result(result_target);
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
                                                {
                                                    if (try_to_move_obstacle(dtime))
                                                    {
                                                        attempts_to_move_obstacle++;
                                                        try_to_remove_obstacle_mode = false;
                                                    }
                                                }

                                            }
                                            else
                                            {

                                                if (target_is_too_high())
                                                {
                                                    if (!too_high_notified)
                                                    {
                                                        too_high_notified = true;
                                                        send_random_context(MiscThings::insert_object_into_list_and_get_info(target_ref) + " is too high! Looking at it instead. ", false);
                                                    }
                                                    lock_camera_onto_target(target_ref, dtime);
                                                    return;
                                                }

                                                if (false && ((((int)std::size(path) > 2) || (interaction_after_walk == 2) || had_successful_walk) && (walk_retries < 7)))
                                                {
                                                    if (MiscThings::is_dragon(target_ref) && MiscThings::is_flying(target_ref) && MiscThings::is_fighting_dragons_allowed())
                                                    {
                                                        if (walk_retries > 5)
                                                        {
                                                            send_random_context("You keep following the dragon...", false);
                                                            walk_retries = 0;
                                                        }

                                                    }

                                                    if ((interaction_after_walk == 2) && ((int)std::size(path) < 3) && !had_successful_walk)
                                                        walk_retries++;

                                                    walk_again();
                                                } 
                                                else
                                                {
                                                    //maybe its a location switch door?

                                                    if (!MiscThings::is_intro2() && walk_timeout < 1.1f && (have_doors_nearby() || MiscThings::get_potential_blocking_object(400.0f, target_ref) != "")) //test if its a door for 1 sec, "cant walk there" and reset if no doors in sight
                                                    {
                                                        if (!test_about_to_be_blocked_by_door(dtime))
                                                        {

                                                            std::string blocking_name = test_about_to_be_blocked_by_blocking(dtime, target_ref);
                                                            if (blocking_name != "")
                                                                bool stop_here = false;

                                                            if (blocking_name != "")
                                                            {
                                                                //blocking object_detected
                                                                std::string fail_text = "[ " + blocking_name + " blocks the path. ";

                                                                if (blocking_name.find("estructib") != std::string::npos)
                                                                    fail_text += "You can try attacking it to destroy it]";
                                                                else
                                                                    fail_text += "Maybe you need to interact with something nearby to go past it]";

                                                                
                                                                send_random_context(fail_text, false);

                                                                if (blocking_name.find("Ancient Nordic Door") != std::string::npos)
                                                                    walk_backward_a_little = true;
                                                                else
                                                                    reset_walker();
                                                                return;
                                                            }


                                                            //walk_to_point();
                                                            walk_timeout += dtime;
                                                        }
                                                        else
                                                        {
                                                            //blocking door detected. if door is unlocked - just open it. if locked - force ask if we want to lockpick it

                                                            if (!is_targeted_door_locked())
                                                            {
                                                                ignore_closed_doors_time = 3.0f;
                                                                if (is_targeted_door_closed())
                                                                {
                                                                    confirm(); //just unlock it

                                                                    if (MiscThings::get_door_teleport(get_targeted_ref()) != "" && (quest_mode || runaway_mode))
                                                                        just_teleported = true;

                                                                }
                                                                    
                                                            }
                                                            else
                                                            {
                                                                if (!door_is_closed_request_sent)
                                                                {
                                                                    //send_random_context("The path is blocked by a locked door!");
                                                                    
                                                                    auto attackers = MiscThings::get_player_attackers();
                                                                    if (std::size(attackers) > 0)
                                                                    {
                                                                        confirm(); //lockpick it
                                                                    }
                                                                    else
                                                                    {

                                                                        if (MiscThings::is_door_superlocked(target_ref))
                                                                        {
                                                                            send_random_context("The path is blocked by a locked door. Cannot lockpick this door. It requires a key", false);
                                                                            reset_walker();
                                                                            return;
                                                                        }

                                                                        if (MiscThings::get_picks_amount_int() <= 0)
                                                                        {
                                                                            send_random_context("The path is blocked by a locked door and you dont have any lockpicks to open the lock", false);

                                                                            if (is_door(target_ref))
                                                                                cut_navmesh_on_target(get_targeted_ref());

                                                                            reset_walker();
                                                                            return;
                                                                        }

                                                                        unregister_all_actions();
                                                                        if (force_choice({ {0, "No"}, {1, "Yes"} }, get_locked_door_force_message(get_targeted_ref()), force_type::closed_door_choice))
                                                                            door_is_closed_request_sent = true;
                                                                    }
                                                                    

                                                                }
                                                                else
                                                                {
                                                                    if (door_is_closed_choice_valid)
                                                                    {
                                                                        if (door_is_closed_choice)
                                                                        {
                                                                            confirm(); //lockpick it

                                                                            catch_door_result = true; //this will reset after its done 

                                                                            //then lockpicking could fail or succeed...
                                                                            //TODO: go on


                                                                            ;// reset_walker();
                                                                        }
                                                                        else
                                                                        {
                                                                            register_allowed_actions();
                                                                            cut_navmesh_on_target(get_targeted_ref());
                                                                            walk_backward_a_little = true;
                                                                            //reset_walker(); //resets after walk
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }

                                                        return;
                                                    }


                                                    


                                                    if ((((int)std::size(path) > 2) || (interaction_after_walk == 2) || had_successful_walk) && (walk_retries < 7))
                                                    {
                                                        if (MiscThings::is_dragon(target_ref) && MiscThings::is_flying(target_ref) && MiscThings::is_fighting_dragons_allowed())
                                                        {
                                                            if (walk_retries > 5)
                                                            {
                                                                send_random_context("You keep following the dragon...", false);
                                                                walk_retries = 0;
                                                            }

                                                        }

                                                        had_successful_walk = false;
                                                        if ((interaction_after_walk == 2) && ((int)std::size(path) < 3) && !had_successful_walk)
                                                            walk_retries++;
                                                        else
                                                        {
                                                            if (std::size(path) > 0)
                                                            {
                                                                auto current_paths_best_guess = path.at(std::size(path) - 1);
                                                                if ((last_paths_best_guess != RE::NiPoint3::Zero()) && last_paths_best_guess.GetDistance(current_paths_best_guess) > 150.0f)
                                                                    walk_retries = 0;

                                                                last_paths_best_guess = current_paths_best_guess;
                                                            }
                                                            
                                                        }
                                                            

                                                        walk_again();
                                                    }
                                                    else
                                                    {
                                                        if ((int)std::size(path) < 3 && ((walk_retries < 4 && !location_mode && !quest_mode) || (walk_retries < 10 && (location_mode || quest_mode)))) //IF IT BROKE - CHECK THIS
                                                        {
                                                            wiggle_body_then_walk_again = true;
                                                            
                                                            walk_retries++;
                                                            //walk_again();
                                                            
                                                            if (MiscThings::is_dragon(target_ref) && MiscThings::is_flying(target_ref) && MiscThings::is_fighting_dragons_allowed())
                                                            {
                                                                if (walk_retries > 8)
                                                                {
                                                                    send_random_context("You keep following the dragon...", false);
                                                                    walk_retries = 0;
                                                                }

                                                            }

                                                            return;
                                                        }

                                                        if (probe_failed() && !had_any_path_found_this_run && !detect_stuck_walk_again_if_good(dtime)) //only if there was no navmesh path
                                                            walk_forward();
                                                        else
                                                        {
                                                            std::string fail_text = "[Cant walk there. Maybe this target is not accessible. Do something else or try again later]";
                                                            std::string potential_block = MiscThings::get_potential_blocking_object();
                                                            if (potential_block != "")
                                                            {
                                                                fail_text = "[ " + potential_block + " blocks the path. ";

                                                                if (potential_block.find("estructib") != std::string::npos)
                                                                    fail_text += "You can try attacking it to destroy it]";
                                                                else
                                                                    fail_text += "Maybe you need to interact with something nearby to go past it]";
                                                            }
                                                            else
                                                            {
                                                                if (!MiscThings::is_serving_jail())
                                                                    Observer::detect_interesting_objects(0.016, true);
                                                            }

                                                            send_random_context(fail_text, false);
                                                            remove_navmesh_cutter();
                                                            reset_walker();
                                                        }
                                                    }
                                                }
                                            }

                                        }
                                        else
                                        {
                                            if (dead_point_time > 10.0f)
                                            {
                                                std::string fail_text = "[Cant walk there. Maybe this target is not accessible. Do something else or try again later]";
                                                std::string potential_block = MiscThings::get_potential_blocking_object();
                                                if (potential_block != "")
                                                {
                                                    fail_text = "[ " + potential_block + " blocks the path. ";

                                                    if (potential_block.find("estructib") != std::string::npos)
                                                        fail_text += "You can try attacking it to destroy it]";
                                                    else
                                                        fail_text += "Maybe you need to interact with something nearby to go past it]";
                                                }
                                                else
                                                {
                                                    if (!MiscThings::is_serving_jail())
                                                        Observer::detect_interesting_objects(0.016, true);
                                                }

                                                send_random_context(fail_text, false);
                                                reset_walker();
                                            }
                                            else
                                                dead_point_time += dtime;

                                        }
                                    }
                                }
                                else
                                {
                                    dead_point_time = 0.0f;
                                    if (true)//walk_timeout < 30.0f) //TODO: maybe remove later. idk if 30 seconds is good or bad
                                    {
                                        if (!test_about_to_be_blocked_by_door(dtime))
                                        {
                                            if (!detect_stuck(dtime))
                                            {
                                                std::string blocking_name = test_about_to_be_blocked_by_blocking(dtime);

                                                if (blocking_name != "" && almost_stuck())
                                                {
                                                    //blocking object_detected
                                                    std::string fail_text = "[ " + blocking_name + " blocks the path. ";

                                                    if (blocking_name.find("estructib") != std::string::npos)
                                                        fail_text += "You can try attacking it to destroy it]";
                                                    else
                                                        fail_text += "Maybe you need to interact with something nearby to go past it]";

                                                    send_random_context(fail_text, false);

                                                    if (blocking_name.find("Ancient Nordic Door") != std::string::npos)
                                                        walk_backward_a_little = true;
                                                    else
                                                        reset_walker();

                                                    return;
                                                }

                                                walk_to_point(dtime);
                                                //cast_pathfinding(dtime); //rebuild path
                                            }
                                            else
                                            {
                                                //stuck detected. wiggle body.
                                                if (unstuck_attempts < 5)
                                                {
                                                    time_stuck = 0.0f;
                                                    try_unstuck = true;
                                                    unstuck_attempts++;
                                                }
                                                else
                                                {
                                                    std::string fail_text = "[Cant walk there. Maybe this target is not accessible. Do something else or try again later]";
                                                    std::string potential_block = MiscThings::get_potential_blocking_object();
                                                    if (potential_block != "")
                                                    {
                                                        fail_text = "[ " + potential_block + " blocks the path. ";

                                                        if (potential_block.find("estructib") != std::string::npos)
                                                            fail_text += "You can try attacking it to destroy it]";
                                                        else
                                                            fail_text += "Maybe you need to interact with something nearby to go past it]";
                                                    }
                                                    else
                                                    {
                                                        if (!MiscThings::is_serving_jail())
                                                            Observer::detect_interesting_objects(0.016, true);
                                                    }

                                                    send_random_context(fail_text, false);
                                                    reset_walker();
                                                }

                                            }


                                            //walk_to_point();
                                            walk_timeout += dtime;
                                        }
                                        else
                                        {
                                            //blocking door detected. if door is unlocked - just open it. if locked - force ask if we want to lockpick it

                                            if (!is_targeted_door_locked())
                                            {
                                                ignore_closed_doors_time = 3.0f;
                                                if (is_targeted_door_closed())
                                                {
                                                    confirm(); //just unlock it
                                                    if (MiscThings::get_door_teleport(get_targeted_ref()) != "" && (quest_mode || runaway_mode))
                                                        just_teleported = true;
                                                }
                                                    
                                            }
                                            else
                                            {
                                                if (!door_is_closed_request_sent)
                                                {
                                                    
                                                    //send_random_context("The path is blocked by a locked door!");
                                                    
                                                    auto attackers = MiscThings::get_player_attackers();
                                                    if (std::size(attackers) > 0 && runaway_mode)
                                                    {
                                                        confirm(); //lockpick it
                                                    }
                                                    else
                                                    {

                                                        if (MiscThings::is_door_superlocked(target_ref))
                                                        {
                                                            send_random_context("The path is blocked by a locked door. Cannot lockpick this door. It requires a key", false);
                                                            reset_walker();
                                                            return;
                                                        }

                                                        if (MiscThings::get_picks_amount_int() <= 0)
                                                        {
                                                            send_random_context("The path is blocked by a locked door and you dont have any lockpicks to open the lock", false);

                                                            if (is_door(target_ref))
                                                                cut_navmesh_on_target(get_targeted_ref());

                                                            reset_walker();
                                                            return;
                                                        }

                                                        unregister_all_actions();
                                                        if (force_choice({ {0, "No"}, {1, "Yes"} }, get_locked_door_force_message(get_targeted_ref()), force_type::closed_door_choice))
                                                            door_is_closed_request_sent = true;
                                                    }
                                                }
                                                else
                                                {
                                                    if (door_is_closed_choice_valid)
                                                    {
                                                        if (door_is_closed_choice)
                                                        {
                                                            confirm(); //lockpick it

                                                            catch_door_result = true; //this will reset after its done 
                                                            //then lockpicking could fail or succeed...
                                                            //TODO: go on


                                                            // reset_walker();
                                                        }
                                                        else
                                                        {
                                                            register_allowed_actions();
                                                            cut_navmesh_on_target(get_targeted_ref());
                                                            walk_backward_a_little = true;
                                                            //reset_walker(); //resets after walk
                                                        }
                                                    }
                                                    else
                                                    {
                                                        ;// lock_camera_onto_target(get_targeted_ref(), dtime);
                                                    }
                                                }
                                            }
                                        }

                                    }
                                    else
                                    {
                                        reset_walker();
                                    }
                                }
                            }
                            else
                            {
                                make_clairvoyance_cast = true;
                            }
                        }
                    }
                }
                else
                {
                    //no target_to_walk
                    walker_inactive_time += dtime;
                    reset_walker();
                }
                    

            }

        }
        catch (const std::out_of_range& e) {
            return; //path is broken
        }



		if (walker_processor_timer > 7.005f)
		{

            init_delay = true;


			walker_processor_timer = 0.0f;

		}
		else
			walker_processor_timer += dtime;
	}













}











//why is it so long