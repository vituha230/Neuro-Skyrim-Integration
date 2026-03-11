

//TODO: all new opened menus reset_walking



#include "InputActions.hpp"
#include "WalkerProcessor.hpp"
#include "main.hpp"
#include "Misc.hpp"
#include <numbers>
#include "CustomWalkerPaths.hpp"
#include "Observer.hpp"

namespace WalkerProcessor {



    bool dont_use_bounds_for_close_enough = false;
    bool getting_into_carriage = false;
    float getting_into_carriage_time = 0.0f;


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
    float attack_action_time = 0.0f;


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
    float min_dist = 20000.0f;

    bool explore_mode_notified = false;


    bool surrender_mode = false;

    bool walk_again_when_finished = false;

    bool reset_after_walk = false;


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
        if (player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowDrawn || player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowDraw || player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowAttached || //player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowReleasing || player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowFollowThrough || player_actor->GetAttackState() == RE::ATTACK_STATE_ENUM::kBowReleased || 
            is_casting_walker(true) || is_casting_walker(false))
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
                attack_action_time = 0.0f;
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
                    marker_ref->SetPosition(custom_path.at(0));
                else
                    marker_ref->SetPosition(marker_ref->GetPosition() + shift + pickpocket_shift);
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



    namespace Hooks {

        struct Pathing {

            //VTABLE_BSPathingRequest

            //119CC80 - need to hook this.

            



            static void myStart(RE::GuideEffect* a_guideEffect)
            {
                auto my_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("myscPath");

                if (my_quest)
                {

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


                        //auto target_cell = target_ref->GetParentCell();
                        //marker->AsReference()->SetParentCell(target_cell);

                        //auto pos_target = target_ref->GetPosition();
                        //marker->AsReference()->SetPosition(pos_target);

                        auto marker_ref = marker->AsReference();
                        if (marker_ref)
                        {
                            marker_ref->MoveTo(target_ref);

                            correct_marker_pos();
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

                    //saved_guide_effect = a_guideEffect;
                    originalStart(a_guideEffect); //call original function, let it read fake target, then restore original quest flags
                    


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

                    for (auto hazard : hazards)
                        path.push_back(hazard.get()->data.location);

                    if (std::size(path) > 2)
                    {
                        had_any_path_found_this_run = true;
                        if (explore_mode)
                        {
                            min_dist = 20000.0f;
                            if (!explore_mode_notified)
                            {
                                send_random_context("[You found a direction to explore, and started walking");
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
                                min_dist = 20000.0f;
                                explore_mode = false;
                                reset_walker();

                                if (MiscThings::is_interior_cell())
                                {
                                    register_exit_dungeon();
                                    unregister_explore_action();
                                }

                                send_random_context("You cannot find any interesting direction to explore");
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
                                        send_random_context(MiscThings::insert_object_into_list_and_get_info(target_ref) + " is too high! Looking at it instead. ");
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

    bool use_last_point_of_last_path = false;
    RE::NiPoint3 last_point_of_last_path{};



    void install_hook()
    {
        Hooks::Pathing::Install();
    }


    RE::NiPoint3 along_next_path_points_vector()
    {
        RE::NiPoint3 result{};
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
                        if (player->GetDistance(target_ref) < 300.0f)
                            return result; //close-to-target alongs are fucked because of z shift
                    }


                    if (current_path_point <= (int)std::size(path) - 4)
                    {
                        RE::NiPoint3 current_path_point_pos = path.at(current_path_point);
                        RE::NiPoint3 next_point_pos = path.at(std::min(((int)std::size(path) - 3), current_path_point + 2));

                        RE::NiPoint3 direction_vector = next_point_pos - current_path_point_pos;

                        direction_vector.Unitize();

                        if (MiscThings::is_on_horse())
                            direction_vector.z -= 0.2f;
                        else
                            direction_vector.z -= 0.08f;

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

            if (path_valid && !use_last_point_of_last_path)
            {
                float max_diff = 0.0f;

                for (int i = current_path_point; i < (std::min((int)std::size(path), current_path_point + 2)); i++)
                {
                    float diff = last_point_posZ - path.at(i).z;
                    if (diff > max_diff)
                        max_diff = diff;
                }

                if (max_diff > 100.0f)
                    result = true;

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
        return true;
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

                if (is_about_to_fall())
                {
                    if (player->IsRunning() && !player->IsSneaking() && !was_slowwalking)
                    {
                        was_slowwalking = true;
                        slow_walk(); //if next point is much lower - we probably need to fall. so start walking slow to not get clinged to potential wall next to us
                    }
                        
                }
                else
                    if (!(player->IsRunning()) && !(player->IsSneaking()) && was_slowwalking)
                    {
                        was_slowwalking = false;
                        unslow_walk();
                    }
                        

                if (needs_jump() && !player->IsSneaking())
                    jump(); //if next point is much higher - we probably need to jump.

                //this will probably never work until there is a way to actually hold a button
                
                if (may_sprint())
                {
                    //if (!player->IsSprinting() && !was_sprinting)
                    //if (!player->IsSprinting())
                    //{
                        sprint();
                        was_sprinting = true;
                    //}
                } 
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
                

                //cursor_up(); //TODO: proper walk mechanics. somethimes just holding W is not good




                //right_attack_cancel();
                //left_attack_cancel();

                if (last_walk_reminded_time > 5.0f)
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

                    reminder_message += "Distance walked: " + std::to_string((int)(reminder_distance / 100.0f)) + " m. ";
                    reminder_message += "Walk time: " + std::to_string((int)reminder_walk_time) + " s. ";

                    send_random_context(reminder_message);
                }
                else
                    last_walk_reminded_time += dtime_maybe_bad;

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



    


    RE::TESObjectREFR* get_targeted_ref()
    {
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();

        auto crosshair_data = RE::CrosshairPickData::GetSingleton();

        if (crosshair_data && crosshair_data->target)
        {
            return crosshair_data->target.get().get(); //wtf is this
        }

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
                    if (target_ref->GetDistance(player_ref) < 200.0f)
                    {
                        return target_ref;
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


        auto target_ref = get_targeted_ref();
        if (target_ref)
        {
            auto base_obj = target_ref->GetBaseObject();
            if (base_obj && base_obj->GetFormType() == RE::FormType::Door && !turning_around)
            {
                auto door_object = (RE::TESObjectDOOR*)base_obj;

                std::string model = door_object->GetModel();

                if (model.find("TrapHingeTrigger01") != std::string::npos)
                {
                    return false;
                }



                if (last_targeted_ref == target_ref)
                {
                    if (have_door_targeted_time > 0.15f)
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
                    last_targeted_ref = target_ref;
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



    bool player_has_key(RE::TESKey* key)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();

        if (player)
        {
            auto inventory = player->GetInventory();

            auto key_bound_object = (RE::TESBoundObject*)key;

            if (inventory.find(key_bound_object) != inventory.end())
                result = true;
        }


        return result;
    }


    void cut_navmesh_on_target(RE::TESObjectREFR* target)
    {
        auto avoidsphere_form = RE::TESObjectREFR::LookupByID(0x7002dbf);
        if (avoidsphere_form)
        {
            auto avoidsphere_refr = avoidsphere_form->AsReference();
            if (avoidsphere_refr)
                avoidsphere_refr->MoveTo(target);
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


            auto door_refr = (RE::TESObjectDOOR*)target_refr;
            

            if (auto extra = target_refr->extraList.GetByType(RE::ExtraDataType::kLock); extra)
            {
                auto extra_lock = (RE::ExtraLock*)extra;

                if (auto extra_lock_data = extra_lock->lock; extra_lock_data)
                    if (extra_lock_data->IsLocked())
                        if (auto key = extra_lock_data->key; key)
                        {
                            if (!player_has_key(key))
                                result = true; //locked, no key. TODO: may be inaccessible at all. we wont be able to lockpick it
                        }
                        else
                            result = true; //locked but no key
                            


            }

            if (target_refr->IsLocked())
            {
                auto lock = target_refr->GetLock();

                if (lock)
                {
                    auto key = lock->key;
                    if (!player_has_key(key))
                        result = true; //locked, no key. TODO: may be inaccessible at all. we wont be able to lockpick it

                }
                else
                    result = true; //locked but no lock.. ok..

                
                //if (target_refr->GetLockLevel() != RE::LOCK_LEVEL::kRequiresKey)
                //    return true;
            }
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



    RE::NiPoint3 get_estimate_aim_pos(RE::TESObjectREFR* target)
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

                if (target_actor->currentProcess)
                    if (target_actor->currentProcess->middleHigh)
                        if (target_actor->currentProcess->middleHigh->headNode)
                        {
                            auto torso_pos = target_actor->currentProcess->middleHigh->torsoNode->world.translate;

                            auto lookat_location = torso_pos;

                            auto player_temp_pos = player->GetPosition();

                            target_center = lookat_location;
                            height = 0.0f;
                            specific_shift = RE::NiPoint3::Zero();

                            lookat_used = true;
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


    bool lock_camera_onto_target(RE::TESObjectREFR* target, float dtime)
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
                            if (!player->IsSneaking() && !using_custom_path && !location_mode && !(quest_mode && target->IsActor() && !target->IsDead()))
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


        if (MiscThings::is_intro())
        {
            mouse_x = mulX * 50.0f;
            mouse_y = mulZ * 50.0f;
        }
        else
        {
            mouse_x = mulX * 400.0f;
            mouse_y = mulZ * 400.0f;
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
            if (abs(mouse_y) < 3.0f)
            {
                if (mouse_y < 0.0f)
                    mouse_y = -3.0f;
                else
                    mouse_y = 3.0f;
            }

            if (abs(mouse_x) < 3.0f)
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


    




    bool path_point_reached()
    {
        bool result = false;

        auto player_pos = RE::PlayerCharacter::GetSingleton()->GetPosition();

        try {

            if (use_last_point_of_last_path)
            {
                auto distance = last_point_of_last_path.GetDistance(player_pos);
                if ((int)std::size(path) < 3)
                    result = distance < 60.0f * (1 + MiscThings::is_on_horse() * 4.0f); //100
                else
                    result = distance < 80.0f * (1 + MiscThings::is_on_horse() * 4.0f); //100
            }
            else
            {
                if (path_valid)
                {
                    if (current_path_point < (int)std::size(path))
                    {
                        auto current_path_point_pos = path.at(current_path_point);

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
                            current_path_point_pos.z = 0.0f;
                            player_pos.z = 0.0f;
                            distance = current_path_point_pos.GetDistance(player_pos);

                            result = distance < 80.0f; //100
                        }
                        else
                            result = distance < (60.0f * (1 + MiscThings::is_on_horse() * 4.0f)); //100
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


    void start_casting()
    {
        auto spell_form = RE::TESForm::LookupByID(135491);
        if (spell_form)
        {
            if (spell_form->IsMagicItem())
            {
                auto spell = (RE::MagicItem*)spell_form;

                auto player = RE::PlayerCharacter::GetSingleton();
                if (player && player->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand))
                {
                    auto caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);
                    
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
                if (player && player->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand))
                {
                    auto caster = player->GetMagicCaster(RE::MagicSystem::CastingSource::kLeftHand);

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

        bool result = false;

        if (is_casting_walker(false))
            left_attack_cancel();

        if ((!is_casting_walker(false) && !is_casting_input(false)) || is_casting_clairvoyance)
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
        reset_after_walk = false;

        walk_again_when_finished = false;

        dont_use_bounds_for_close_enough = false;
        getting_into_carriage = false;
        getting_into_carriage_time = 0.0f;

        surrender_mode = false;

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
            min_dist = 20000.0f;

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

        if (using_custom_path)
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
        attack_action_time = 0.0f;

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



    }

    void walk_again()
    {
        //if (!using_custom_path)
        {

            walk_again_when_finished = false;

            too_high_notified = false;

            lock_camera_wants_to_crouch = false;
            path_valid = false;
            current_path_point = -1;
            is_casting_clairvoyance = false;
            start_casting_time = 0.0f;
            walk_timeout = 0.0f;
            attacking_done = true;
            start_attacking = false;
            time_blind_walk = 0.0f;

            last_target_pos = RE::NiPoint3::Zero();
            last_u = RE::NiPoint3::Zero();
            last_target_pos_valid = false;
            last_u_valid = false;
            //was_charging_ranged = false;


            lasttime = 0;
            lasttime_close_enough = 0;

            attack_action_time = 0.0f;
            got_close_for_pickpocket = false;

            //search_next_fight_target = false;
            search_next_target_timer = 0.0f;

            if (was_slowwalking)
            {
                was_slowwalking = false;
                unslow_walk();
            }

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

                if (distance.Length() < 400.0f)
                    return true;
            }
        }


        if (!target_ref || !player)
            return true; //let it fully restart, something is very wrong


        auto player_pos = player->GetPosition();
        auto target_pos = target_ref->GetPosition();


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
                        

                    if (has_ranged_weapon_equipped(get_current_active_hand()))
                    {

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
                        if (start_attacking)
                            range = range * 1.25;


                        auto raycast_ref = MiscThings::GetRaycastRef(camera_pos, delta_pos, range);

                        if (target_ref->IsActor() && target_ref->IsDead())
                            return true;

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

                        if ((target_visible && distance.Length() < range) || distance.Length() < 200.0f)
                            return true;

                        bool stop_here = false;
                    }
                    else
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
                    if (distance.z < 200.0f)
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
        float max_dist = 50000.0f;


        std::pair<bool, std::string> result{};

        auto cant_walk_reason = get_cant_walk_reason();

        if (cant_walk_reason != "")
        {
            min_dist = 20000.0f;
            result.first = false;
            result.second = cant_walk_reason;
            return result;
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
            min_dist = 20000.0f;
            result.first = false;
            result.second = "You cannot walk yet";
            return result;
        }


        if (explore_mode && !internal_call)
        {
            result.first = false;
            result.second = "You are already exploring";
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
            min_dist = 20000.0f;
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
                reset_walker();
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
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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

                if (!MiscThings::is_object_still_valid(object->second))
                {
                    result.first = false;
                    result.second = "This object doesnt exist anymore";
                    return result;
                }

                if ((!object->second->IsHumanoid() || (object->second->IsHumanoid() && object->second->IsDead())) && interaction == 2)
                {
                    result.first = false;
                    result.second = "Cannot pickpocket this target";
                    return result;
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
                        send_random_context("Your hands are bound. Wait for the game to progress before interacting. Looking at the target instead");
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

                        send_random_context(message);
                    }
                        
                    //return result;
                    interaction = 0;

                }
                

                if (have_target_to_walk)
                {
                    Observer::reset_threats();

                    if (target_ref != object->second || interaction != interaction_after_walk)
                        reset_walker();
                    else
                    {
                        result.first = false;
                        result.second = "You are already walking to this object!";
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

                        if (mount_refr == object->second)
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


                target_ref = object->second;

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
                reminder_target_name = MiscThings::insert_object_into_list_and_get_info(object->second);


                if (interaction > 0 && interaction < 4)
                {
                    interaction_after_walk = interaction;

                    if (interaction_after_walk == 3 && target_ref->IsActor() && target_ref->IsDead())
                        was_already_dead = true;
                }
                else
                    interaction_after_walk = -1;

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
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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
                    if (target_ref != object->second)
                        reset_walker();
                    else
                    {
                        result.first = false;
                        result.second = "You are already looking at this object!";
                        return result;
                    }
                }



                right_attack_cancel();
                left_attack_cancel();



                target_ref = object->second;

                have_target_to_walk = true;

                reminder_start_pos = player->GetPosition();
                reminder_start_pos = player->GetPosition();
                reminder_target_name = MiscThings::insert_object_into_list_and_get_info(object->second);

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





    std::pair<bool, std::string> walk_to_location_by_index(int index)
    {
        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();

        
        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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
                            result.first = false;
                            result.second = "You are already walking to that location. ";
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

                    result.first = true;
                    result.second = "[Started walking to location: " + reminder_target_name + "...]";
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
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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
    

    std::pair<bool, std::string> walk_to_quest_by_index(int index, bool ignore_specified_target)
    {

        std::pair<bool, std::string> result{};

        auto player = RE::PlayerCharacter::GetSingleton();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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
                if (quest_entry.id == index)
                {
                    auto the_quest = quest_entry.quest;

                    for (auto objective : the_quest->objectives)
                    {
                        if (quest_entry.objective == objective)
                        {
                            if (objective->state.all(RE::QUEST_OBJECTIVE_STATE::kDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kCompletedDisplayed) && !objective->state.all(RE::QUEST_OBJECTIVE_STATE::kFailedDisplayed))
                            {
                                quest_found = true;
                                break;
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
                if (quest_entry.id == index)
                {
                    quest_found = true;
                    break;
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
                auto quest = quest_list->at(actual_id);

                auto objective = quest.objective;

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
                                if (quest.quest)
                                {
                                    bool conditions_met = false;
                                    //this needs to be figured out
                                    try {
                                        if (target->conditions.head)
                                        {
                                            auto the_condition = target->conditions.head;

                                            conditions_met = MiscThings::recursive_quest_condition_check(the_condition, quest.quest);

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
                                        target->GetTrackingRef(quest_ref_handle, quest.quest); //try tracked
                                        if (!quest_ref_handle)
                                            target->GetTargetRef(quest_ref_handle, false, quest.quest); //no tracked - try actual target

                                        if (!ignore_specified_target && target != quest.target)
                                            continue;

                                        if (quest_ref_handle)
                                            if (quest_ref_handle.get())
                                            {
                                                auto quests_target_ref = quest_ref_handle.get().get();
                                               
                                                auto helgen_tower_marker = RE::TESObjectREFR::LookupByID(0xe24c3);

                                                if (quests_target_ref == helgen_tower_marker)
                                                {
                                                    unregister_all_actions();
                                                    using_custom_path = true;
                                                    custom_path = CustomWalkerPaths::helgen_tower_path;
                                                    //path_valid = true;
                                                }

                                                if (have_target_to_walk)
                                                {
                                                    Observer::reset_threats();

                                                    if (target_ref != quests_target_ref)
                                                        reset_walker();
                                                    else
                                                    {
                                                        result.first = false;
                                                        result.second = "You are already following this quest!";
                                                        return result;
                                                    }
                                                }

                                                quest_mode = true;
                                                target_ref = quests_target_ref;

                                                reminder_target_name = "[id " + std::to_string(quest.id) + "] " + quest.name + ": " + quest.displaytext;
                                                reminder_start_pos = player->GetPosition();

                                               //MiscThings::insert_quest_into_list_and_get_info(quest);


                                                if (target_ref == player_ref)
                                                {
                                                    reset_walker();
                                                    result.first = true;
                                                    result.second = "Error. Cannot follow this quest";
                                                    return result;
                                                }

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


                                                last_quest = quest.quest;
                                                last_quest_objective = objective;

                                                right_attack_cancel();
                                                left_attack_cancel();

                                                result.first = true;
                                                result.second = "[Started following quest: " + reminder_target_name + "...]";
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
                }
            }
            else
                reset_walker();

        }

        result.first = false;
        result.second = "Invalid quest ID. Use get_current_quests to get valid ID list";
        reset_walker();

        return result;

    }


    bool detect_quest_target_changed_and_walk()
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();

        auto control_map = RE::ControlMap::GetSingleton();
        bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
        bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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

                                        conditions_met = MiscThings::recursive_quest_condition_check(the_condition, quest);

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
                                            if (target_ref != quests_target_ref)
                                            {
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


                                                right_attack_cancel();
                                                left_attack_cancel();

                                                target_ref = quests_target_ref; //i think something is excessive here
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
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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
                reset_walker();

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
            reminder_target_name = "running away. ";
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

            if (base_obj && base_type != RE::FormType::Static)
            {
                auto player = RE::PlayerCharacter::GetSingleton();
                auto player_ref = player->AsReference();
                RE::BSString result_string = "";

                base_obj->GetActivateText(player_ref, result_string);

                if (result_string != "")
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






    bool is_offensive_spell(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            RE::MagicItem* spell = (RE::MagicItem*)MiscThings::get_hand_contents(right);

            if (spell && spell->GetFormType() == RE::FormType::Spell)
                if (spell->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
                {
                    for (auto effect : spell->effects)
                    {
                        if (effect->IsHostile())
                        {
                            result = true;
                            break;
                        }
                    }
                }

        }
        return result;
    }


    bool has_spell_equipped(bool right)
    {
        bool result = false;

        auto player = RE::PlayerCharacter::GetSingleton();
        if (player)
        {
            RE::MagicItem* spell = (RE::MagicItem*)MiscThings::get_hand_contents(right);

            if (spell && spell->GetFormType() == RE::FormType::Spell)
                if (spell->GetSpellType() != RE::MagicSystem::SpellType::kEnchantment)
                    result = true;

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


    bool has_something_equipped(bool right)
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
                    result = true;
            }
            else
            {
                if (left_hand)
                    result = true;
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
                        auto speed = effectSetting->data.projectileBase->data.speed;

                        if (speed > 0.0f)
                            return speed;
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
                            auto range2 = effectSetting->data.projectileBase->data.range;

                            if (range2 > 0.0f)
                                return range2;
                        }
                    }
                    else
                        return 999999.0f;

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

        lock_camera_onto_target(target_ref, dtime);

        bool result = false;
        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();
        auto player_actor = (RE::Actor*)player_ref;


        if (!(player_actor->IsWeaponDrawn()) && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing))
        {
            right_attack(); //this is "readyWeapon"
            //set_universal_block(0.5f);
            return false;
        }
            



        if (target_ref && player_ref)
        {
            if (attack_action < 0 || attack_action > 1)
            {
                bool dont_use_right = has_spell_equipped(false) && (!has_something_equipped(true) || (!low_mana_detected && (MiscThings::get_player_mana() > get_spell_cost(false)) && !has_spell_equipped(true)));
                dont_use_right |= (has_ranged_weapon_equipped(true) && no_ammo()) || (has_something_equipped(false) && !has_something_equipped(true));
                
                attack_action = dont_use_right;
            }
                


            //if (close_enough())
            //{
                if (attack_action == 0)
                {

                    if (low_mana_detected && (MiscThings::get_player_mana() > MiscThings::get_player_max_mana() * 0.3f))
                        low_mana_detected = false;


                    bool dont_check_mana = false;

                    dont_check_mana = !is_concentration_spell(true) && is_casting_walker(true);

                    bool low_mana_check = (!dont_check_mana && has_spell_equipped(true) && (low_mana_detected || (MiscThings::get_player_mana() < get_spell_cost(true))));

                    if (low_mana_check)
                        low_mana_detected = true;

                    if (low_mana_check || (has_spell_equipped(true) && !is_offensive_spell(true) && MiscThings::player_hp_more_than(100.0f)))
                    {

                        //set_universal_block(1.0f);
                        right_attack_cancel();
                        //was_charging_ranged = false;
                        attack_action = 1;
                        
                        return false;
                    }

                    if (attack_action_time < get_attack_time(true) && !(has_spell_equipped(true) && !is_offensive_spell(true) && (MiscThings::player_hp_more_than(90.0f) && !is_casting_walker(true))))
                    {
                        std::string target_name = MiscThings::insert_object_into_list_and_get_info(target_ref);
                        
                        std::string attacking_info = "";

                        std::string start_attacking_info = "[You are attacking ";

                        std::string attacking_target = "";
                        std::string attacking_weapon = "";
                        std::string attacking_health = "";

                        bool casting = false;

                        bool skip_cast = false;

                        if (has_spell_equipped(true))
                        {
                            if (attack_action_time > 0.7f);
                            was_charging_ranged = true;

                            if (!is_casting_walker(true))
                            {
                                if (attack_spell_cast_timeout > 1.0f)
                                {
                                    skip_cast = true;
                                    attack_spell_cast_timeout = 0.0f;
                                    right_attack_cancel();
                                    attack_action_time = 0.0f;
                                }
                                else
                                    attack_spell_cast_timeout += dtime;
                            }
                            else
                                attack_spell_cast_timeout = 0.0f;


                            if (!skip_cast)
                            {
                                right_attack_spell();
                                if (!is_offensive_spell(true))
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
                                if (attack_action_time > 0.9f);
                                    was_charging_ranged = true;
                                right_attack_bow();
                            }
                            else
                                right_attack();

                            if (!has_something_equipped(true))
                            {
                                attacking_weapon = "bare fists. You might want to equip some weapon or magic (use get_inventory and use_inventory_item to equip gear). ";
                                if (player->GetDistance(target_ref) > 80.0f * target_ref->GetScale())
                                    cursor_up();
                            }
                            else
                            {
                                if (has_ranged_weapon_equipped(true) && no_ammo())
                                    attacking_weapon = " left fist (you have no ammo to use with your " + get_equipped_weapon_name(true) + "). ";
                                else
                                    attacking_weapon = get_equipped_weapon_name(true) + ". ";

                                if (is_melee_weapon(true) && player->GetDistance(target_ref) > 100.0f * target_ref->GetScale())
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



                            if (attacking_info == "")
                            {
                                if (casting)
                                {
                                    attacking_info = start_attacking_info + attacking_weapon;
                                }
                                else
                                {
                                    if (last_attacking_target != target_name)
                                        attacking_info = start_attacking_info + target_name + " with your " + attacking_weapon + attacking_health;
                                    else
                                        if (last_attacking_weapon != attacking_weapon)
                                            attacking_info = start_attacking_info + target_name + " with your " + attacking_weapon + attacking_health;
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
                                send_random_context(attacking_info + "]");
                        }


                        attack_action_time += dtime;
                    }
                    else
                    {
                        //end of attack
                        attack_spell_cast_timeout = 0.0f;

                        gave_attacking_info = false;
                        was_charging_ranged = false;
                        right_attack_cancel();

                        attack_action_time = 0.0f;

                        float choose_next_action = (float)std::rand() / RAND_MAX;


                        bool dont_use_left = false;

                        if (target_ref->IsActor())
                        {
                            auto target_actor = (RE::Actor*)target_ref;
                            auto target_combat_controller = target_actor->combatController;

                            if (target_combat_controller)
                                dont_use_left = (target_combat_controller->IsFleeing() || target_combat_controller->ignoringCombat || !target_combat_controller->startedCombat || target_combat_controller->stoppedCombat) && left_is_block();
                        }

                        dont_use_left |= has_ranged_weapon_equipped(true);
                        dont_use_left |= has_spell_equipped(true) && (!has_something_equipped(false) || (!low_mana_detected && (MiscThings::get_player_mana() > get_spell_cost(true)) && !has_spell_equipped(false)));
                        bool dont_use_right = has_spell_equipped(false) && (!has_something_equipped(true) || (!low_mana_detected && (MiscThings::get_player_mana() > get_spell_cost(false)) && !has_spell_equipped(true)));

                        dont_use_right |= (has_ranged_weapon_equipped(true) && no_ammo()) || (has_something_equipped(false) && !has_something_equipped(true));

                        if ((choose_next_action < 0.2f && !dont_use_left) || dont_use_right)
                            attack_action = 1;
                        else
                            attack_action = 0;


                        //set_universal_block(0.2f);
                    }
                }
                else
                {

                    if (attack_action == 1)
                    {
                        if (low_mana_detected && (MiscThings::get_player_mana() > MiscThings::get_player_max_mana() * 0.3f))
                            low_mana_detected = false;

                        bool dont_check_mana = false;

                        dont_check_mana = !is_concentration_spell(false) && is_casting_walker(false);

                        bool low_mana_check = (!dont_check_mana && has_spell_equipped(false) && (low_mana_detected || (MiscThings::get_player_mana() < get_spell_cost(false))));

                        if (low_mana_check)
                            low_mana_detected = true;

                        if (low_mana_check || (has_spell_equipped(false) && !is_offensive_spell(false) && MiscThings::player_hp_more_than(100.0f)))// && MiscThings::player_is_full_hp()))
                        {
                            //set_universal_block(1.0f);
                            left_attack_cancel();
                            //was_charging_ranged = false;
                            attack_action = 0;
                            return false;
                        }

                        if (attack_action_time < get_attack_time(false) && !(has_spell_equipped(false) && !is_offensive_spell(false) && (MiscThings::player_hp_more_than(90.0f) && !is_casting_walker(false))))
                        {
                            std::string target_name = MiscThings::insert_object_into_list_and_get_info(target_ref);
                            
                            std::string attacking_info = "";

                            std::string start_attacking_info = "[You are attacking ";

                            std::string attacking_target = "";
                            std::string attacking_weapon = "";
                            std::string attacking_health = "";

                            bool casting = false;

                            if (has_spell_equipped(false))
                            {

                                bool skip_cast = false;

                                if (!is_casting_walker(false))
                                {
                                    if (attack_spell_cast_timeout > 1.0f)
                                    {
                                        skip_cast = true;
                                        attack_spell_cast_timeout = 0.0f;
                                        left_attack_cancel();
                                        attack_action_time = 0.0f;
                                    }
                                        
                                    else
                                        attack_spell_cast_timeout += dtime;
                                }
                                else
                                {
                                    if (attack_action_time > 0.7f);
                                        was_charging_ranged = true;

                                    attack_spell_cast_timeout = 0.0f;
                                }
                                    

                                if (!skip_cast)
                                {
                                    left_attack_spell();
                                    if (!is_offensive_spell(left))
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
                                if (!has_something_equipped(false) && has_something_equipped(true) && is_melee_weapon(true))
                                {
                                    attacking_info = "[You are blocking";
                                    if (player->GetDistance(target_ref) > 100.0f)
                                        cursor_up();
                                }
                                else
                                {
                                    if (has_ranged_weapon_equipped(true) && no_ammo())
                                        attacking_weapon = " left fist (you have no ammo to use with your " + get_equipped_weapon_name(true) + "). ";
                                    else
                                        attacking_weapon = get_equipped_weapon_name(false) + ". ";

                                    if (is_melee_weapon(false) && player->GetDistance(target_ref) > 100.0f * target_ref->GetScale())
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
                                        ;// attacking_health += ". The target is not dying for some reason...";
                                }

                                if (attacking_info == "")
                                {
                                    if (casting)
                                    {
                                        attacking_info = start_attacking_info + attacking_weapon;
                                    }
                                    else
                                    {
                                        if (last_attacking_target != target_name)
                                            attacking_info = start_attacking_info + target_name + " with your " + attacking_weapon + attacking_health;
                                        else
                                            if (last_attacking_weapon != attacking_weapon)
                                                attacking_info = start_attacking_info + target_name + " with your " + attacking_weapon + attacking_health;
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
                                    send_random_context(attacking_info + "]");
                            }

                            attack_action_time += dtime;
                        }
                        else
                        {
                            gave_attacking_info = false;
                            attack_spell_cast_timeout = 0.0f;
                            was_charging_ranged = false;

                            left_attack_cancel();
                            attack_action_time = 0.0f;
                            float choose_next_action = (float)std::rand() / RAND_MAX;

                            bool dont_use_left = false;

                            if (target_ref->IsActor())
                            {
                                auto target_actor = (RE::Actor*)target_ref;
                                auto target_combat_controller = target_actor->combatController;

                                if (target_combat_controller)
                                    dont_use_left = (target_combat_controller->IsFleeing() || target_combat_controller->ignoringCombat || !target_combat_controller->startedCombat || target_combat_controller->stoppedCombat) && left_is_block();
                            }

                            dont_use_left |= has_ranged_weapon_equipped(false);

                            bool dont_use_right = has_spell_equipped(false) && (!has_something_equipped(true) || (!low_mana_detected && (MiscThings::get_player_mana() > get_spell_cost(false)) && !has_spell_equipped(true)));
                            dont_use_right |= (has_ranged_weapon_equipped(true) && no_ammo()) || (has_something_equipped(false) && !has_something_equipped(true));

                            if ((choose_next_action < 0.2f && !dont_use_left) || dont_use_right)
                                attack_action = 1;
                            else
                                attack_action = 0;

                            //set_universal_block(0.2f);
                        }
                    }
                }


           // }
            

            

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

                    if (attacking_inanimate_object_time > 10.0f)
                    {
                        right_attack_cancel();
                        left_attack_cancel();

                        attacking_inanimate_object_time = 0.0f;

                        result = true;
                    }

                }
                else
                {
                    attacking_inanimate_object_time += dtime;
                }
            }
        }
        


        return result;
    }



    bool interact_with_target(float dtime)
    {
        bool result = false;

        auto result_target = get_targeted_ref();

        if ((interaction_after_walk == 1 || interaction_after_walk == 2) && !(MiscThings::is_intro() || MiscThings::is_intro2() || location_mode || (result_target == target_ref) || (quest_mode && !target_is_interactive())))
            return false;

        switch (interaction_after_walk) {

        case (1):
        {
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
                    if (!confirming_closed_door_interaction)
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
                else
                {
                    if (!interaction_context_sent)
                    {
                        interaction_context_sent = true;
                        if (target_ref)
                        {
                            std::string target_name = MiscThings::insert_object_into_list_and_get_info(target_ref);//target_ref->GetDisplayFullName();
                            auto base_obj = target_ref->GetBaseObject();
                            if (base_obj->IsInventoryObject())
                            {
                                send_random_context("[Picking up " + target_name + "...]");
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
                            }
                            else
                                send_random_context("[Interacting with " + target_name + "...]");
                        }
                    }

                  
                    

                    if (is_workbench(target_ref))
                    {
                        //confirm_workbench(); //HOOK Activate() or just try it with random arguments. seems like it might work.
                        
                        //auto workbench = (RE::TESFurniture*)target_ref;
                        auto player = RE::PlayerCharacter::GetSingleton();
                        auto player_ref = player->AsReference();

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
                        confirm();

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
                if (still_alive)
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
                if (quest_mode && (target_name == ""))
                    target_name = "quest target point";
                else
                    target_name = MiscThings::insert_object_into_list_and_get_info(target_ref);

                result = "You walked up to " + target_name; //default

                switch (interaction_after_walk)
                {
                case 1:
                    {
                        if (target_ref->IsInventoryObject())
                            result = "Picked up " + MiscThings::insert_object_into_list_and_get_info(target_ref);
                        else
                            result += " and interacted with this object";

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
        
        

            
        return "[" + result + "]";
    }


    RE::NiPoint3 last_player_pos{};
   

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



    bool path_record_mode = false;
    std::vector<RE::NiPoint3> custom_path_record{};



    void path_is_blocked_result(RE::TESObjectREFR* result_target)
    {
        std::string blocking_object_name = "Something";
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
                blocking_object_name = check_name;
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
            send_random_context("Cannot do this while on mount: " + mount_name);
        }
        else
        {
            if (blocking_object_name == "Something")
                send_random_context("[" + blocking_object_name + " blocks the object. Maybe it is inaccessible at all]");
            else
                send_random_context("[" + blocking_object_name + " blocks the object]");
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
        bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

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



	float walker_processor_timer = 0.0f;

	void processor(float dtime)
	{

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
                send_random_context("[You dont have enough mana to cast the spell!]");
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
            auto parent_cell = player->GetParentCell();

            auto control_map = RE::ControlMap::GetSingleton();
            bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
            bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || player->IsInRagdollState();;
            bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);

            //if (player_actor && !player_actor->movementController->controlsDriven)
            if (!can_walk && !can_look)
            {
                reset_walker();

            }



            if (do_spins)
            {
                if (spin_timeout > 20.0f)
                {
                    do_spins = false;
                    amount_of_spins = 0;
                    spin_step_one = false;
                    send_random_context("[Spinning done]");
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
                    send_random_context("[Spinning done]");
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
                        send_random_context("[Did " + std::to_string(amount_of_spins_done) + " spins]");


                        if (amount_of_spins_done > 4)
                        {
                            amount_of_spins = 0;
                            spin_timeout = 0.0f;
                            spin_step_one = false;


                            std::string message = "[You feel dizzy";

                            
                            auto player_ref = player->AsReference();

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

                            send_random_context(message + "]");
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
                    auto next_targets = MiscThings::get_player_attackers();

                    if (std::size(next_targets) > 0)
                    {
                        reset_walker();
                        auto new_target = next_targets.at(0);
                        std::string new_target_name = MiscThings::insert_object_into_list_and_get_info(new_target);

                        right_attack_cancel();
                        left_attack_cancel();

                        chill_with_context = true;

                        walk_to_object_by_refr(new_target, 3);
                        send_random_context("[You started fighting next target: " + new_target_name + "]");
                    }
                    else
                    {
                        if (search_next_target_timer > 1.0f)
                        {
                            reset_walker();
                            std::string advice = "";

                            if (MiscThings::is_objects_around_valid())
                                advice = "walk somewhere, interact with something";

                            if (MiscThings::have_any_quests())
                                if (advice != "")
                                    advice += ", follow some quest";
                                else
                                    advice = "follow some quest";

                            if (advice != "")
                                advice = "You can " + advice;

                            send_random_context("[Fight ended. Choose next action to do. " + advice + "]");
                        }
                        else
                            search_next_target_timer += dtime;
                    }

                    return;
                }
                


                if (make_clairvoyance_cast)
                {
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

                    if (MiscThings::is_intro())
                    {
                        if (lock_camera_onto_target(target_ref, dtime))
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
                            interact_with_target(dtime);
                        else
                        {
                            if (!close_enough())
                                walk_again();
                        }
                            
                        return;
                    }
                    

                    if (too_high_notified)
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

                    if (player_actor && (player_actor->IsWeaponDrawn() || player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing) && interaction_after_walk != 3 && !input_wants_to_cast())
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

                    if (player_actor && !player_actor->IsWeaponDrawn() && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing) && interaction_after_walk == 3)
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

                            if (!get_targeted_ref() || !is_targeted_door_locked()) //we still have the door targeted and it is unlocked now. or it was unlocked.
                            {
                                //door is opened (now?)
                                register_allowed_actions();
                                catch_door_result = false;
                                if (!get_targeted_ref() || is_door(get_targeted_ref()))
                                {
                                    if (!get_targeted_ref() || target_ref == get_targeted_ref())
                                        reset_walker();//;// walk_forward_a_little = true;
                                    else
                                    {
                                        catch_door_result = false; //proceed whatever we did before after pause
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
                                                                            send_random_context("[" + result_string + "]");
                                                                            reset_walker();
                                                                        }
                                                                    }
                                                                        
                                                }
                                            }
                        }
                        else
                        {
                            send_random_context("[Interaction with object failed]");
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
                            if (path_valid || use_last_point_of_last_path)
                            {



                                if (!use_last_point_of_last_path && (current_path_point == -1))
                                {
                                    current_path_point = 0;
                                    last_point_posZ = path.at(current_path_point).z;
                                }

                                //if (path_point_reached() || (!using_custom_path && close_enough() && (current_path_point > (int)std::size(path) - 5)) || (close_enough() && interaction_after_walk == 3) || MiscThings::is_intro())
                                if (MiscThings::is_intro() || path_point_reached() || (!using_custom_path && close_enough() && (current_path_point > (int)std::size(path) - 5)) || (close_enough() && interaction_after_walk == 3))
                                {
                                    time_stuck = 0.0f;

                                    if (use_last_point_of_last_path)
                                    {
                                        if (path_valid)
                                        {
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
                                                        fail_text = "[ " + potential_block + " blocks the path]";
                                                    send_random_context(fail_text);
                                                    reset_walker();
                                                }
                                            }
                                            return; //wait for path to be built
                                        }

                                    }

                                    if (!use_last_point_of_last_path && (current_path_point < (int)std::size(path)))
                                    {
                                        last_point_posZ = path.at(current_path_point).z;
                                        current_path_point++;
                                        correct_marker_pos();
                                    }

                                    if (!using_custom_path && !use_last_point_of_last_path && ((int)std::size(path) > 5) && (current_path_point == std::size(path) - 1) && !close_enough()) //prelast point. need to rebuild next path segment before we reach the end.
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
                                    else
                                    {
                                        //if (use_last_point_of_last_path || current_path_point >= (int)std::size(path) || (close_enough() && !using_custom_path) || MiscThings::is_intro())
                                        if (MiscThings::is_intro() || use_last_point_of_last_path || current_path_point >= (int)std::size(path) || (close_enough() && !using_custom_path && (current_path_point > (int)std::size(path) - 5)))
                                        {
                                            if (reset_after_walk)
                                            {
                                                register_allowed_actions();
                                                reset_walker();
                                                return;
                                            }
                                               

                                            if (walk_again_when_finished)
                                            {
                                                using_custom_path = false;
                                                register_allowed_actions();
                                                quicksave();
                                                walk_again();
                                            }
                                                

                                            if (got_close_for_pickpocket || close_enough() || MiscThings::is_intro())
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
                                                        
                                                    if (MiscThings::is_intro() || locking_failed || (get_targeted_ref() == target_ref) || lock_camera_onto_target(target_ref, dtime) || location_mode)
                                                    {
                                                        auto result_target = get_targeted_ref();

                                                        if (!result_target && interaction_after_walk == 3 && has_ranged_weapon_equipped(true))
                                                        {
                                                            result_target = target_ref;
                                                        }


                                                        //

                                                        if (MiscThings::is_intro() || MiscThings::is_intro2() || location_mode || (result_target == target_ref) || (quest_mode && !target_is_interactive()))
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
                                                                        send_random_context(get_success_message());
                                                                }

                                                            }
                                                            if (interaction_after_walk > 0)
                                                            {
                                                                if (paused_before_interaction)
                                                                {
                                                                    if ((quest_mode && backup_interaction_made) || interact_with_target(dtime))
                                                                    {
                                                                        //if it was a door, walk forward a little after its opened
                                                                        if (is_door(result_target) || is_container(result_target))
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
                                                                                    backup_interaction_made = true;//dont reset in quest mode. the target might be moving and interaction might have done nothing.
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
                                                                                interaction_after_walk = 1; //interact if there is something and it was quest
                                                                            if (explore_mode)
                                                                                explore_mode = false; //so we can explore again
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
                                                                else
                                                                {
                                                                    if (is_door(result_target) && !is_targeted_door_locked())
                                                                    {
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
                                                                                        send_random_context("[You started fighting next target: " + new_target_name + "]");
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
                                                                                if (!walk_fixed_time(true, 0.4f, dtime)) //was 0.2, revert if bad
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
                                                                                                send_random_context("[You started fighting next target: " + new_target_name + "]");
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
                                                        send_random_context(MiscThings::insert_object_into_list_and_get_info(target_ref) + " is too high! Looking at it instead. ");
                                                    }
                                                    lock_camera_onto_target(target_ref, dtime);
                                                    return;
                                                }

                                                if (false && ((((int)std::size(path) > 2) || (interaction_after_walk == 2) || had_successful_walk) && (walk_retries < 7)))
                                                {
                                                    if ((interaction_after_walk == 2) && ((int)std::size(path) < 3) && !had_successful_walk)
                                                        walk_retries++;

                                                    walk_again();
                                                } 
                                                else
                                                {
                                                    //maybe its a location switch door?

                                                    if (walk_timeout < 2.0f) //test if its a door for 2 sec, "cant walk there" and reset if no doors in sight
                                                    {
                                                        if (!test_about_to_be_blocked_by_door(dtime))
                                                        {
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
                                                                    confirm(); //just unlock it
                                                            }
                                                            else
                                                            {
                                                                if (!door_is_closed_request_sent)
                                                                {
                                                                    //send_random_context("The path is blocked by a locked door!");
                                                                    unregister_all_actions();
                                                                    if (force_choice({ {0, "No"}, {1, "Yes"} }, get_locked_door_force_message(get_targeted_ref()), force_type::closed_door_choice))
                                                                        door_is_closed_request_sent = true;
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

                                                    }
                                                    if ((((int)std::size(path) > 2) || (interaction_after_walk == 2) || had_successful_walk) && (walk_retries < 7))
                                                    {
                                                        if ((interaction_after_walk == 2) && ((int)std::size(path) < 3) && !had_successful_walk)
                                                            walk_retries++;

                                                        walk_again();
                                                    }
                                                    else
                                                    {
                                                        if ((int)std::size(path) < 3 && ((walk_retries < 4 && !location_mode && !quest_mode) || (walk_retries < 10 && (location_mode || quest_mode)))) //IF IT BROKE - CHECK THIS
                                                        {
                                                            wiggle_body_then_walk_again = true;
                                                            
                                                            walk_retries++;
                                                            //walk_again();

                                                            return;
                                                        }

                                                        if (!had_any_path_found_this_run && !detect_stuck_walk_again_if_good(dtime)) //only if there was no navmesh path
                                                            walk_forward();
                                                        else
                                                        {
                                                            std::string fail_text = "[Cant walk there. Maybe this target is not accessible. Do something else or try again later]";
                                                            std::string potential_block = MiscThings::get_potential_blocking_object();
                                                            if (potential_block != "")
                                                                fail_text = "[ " + potential_block + " blocks the path]";
                                                            send_random_context(fail_text);
                                                            remove_navmesh_cutter();
                                                            reset_walker();
                                                        }
                                                    }
                                                }
                                            }

                                        }
                                    }
                                }
                                else
                                {
                                    if (true)//walk_timeout < 30.0f) //TODO: maybe remove later. idk if 30 seconds is good or bad
                                    {
                                        if (!test_about_to_be_blocked_by_door(dtime))
                                        {
                                            if (!detect_stuck(dtime))
                                            {
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
                                                        fail_text = "[ " + potential_block + " blocks the path]";
                                                    send_random_context(fail_text);
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
                                                    confirm(); //just unlock it
                                            }
                                            else
                                            {
                                                if (!door_is_closed_request_sent)
                                                {
                                                    
                                                    //send_random_context("The path is blocked by a locked door!");
                                                    unregister_all_actions();
                                                    if (force_choice({ {0, "No"}, {1, "Yes"} }, get_locked_door_force_message(get_targeted_ref()), force_type::closed_door_choice))
                                                        door_is_closed_request_sent = true;
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
                    reset_walker();

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











