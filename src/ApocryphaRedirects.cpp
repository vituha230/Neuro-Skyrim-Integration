

#include "ApocryphaCustomPaths.hpp"
#include "ApocryphaRedirects.hpp"
#include "Misc.hpp"

namespace Apocrypha {


    bool in_apocrypha(RE::TESObjectREFR* object)
    {

        auto player = RE::PlayerCharacter::GetSingleton();

        if (!object)
            object = player;

        if (object)
        {

            auto object_worldspace = object->GetWorldspace();

            if (object_worldspace && object_worldspace->formID == 0x04) //TODO!!! THIS IS WRONG. need to find that location where worldspace is used
                return true;


            auto loc = object->GetCurrentLocation();

            RE::BGSLocation* apocrypha_location = (RE::BGSLocation*)RE::TESForm::LookupByID(0x04016E2B);

            if (loc)
            {
                if (loc == apocrypha_location)
                {
                    return true;
                }

                unsigned int safeguard = 0;

                auto* parent = loc->parentLoc;

                while (parent && safeguard < 100)
                {
                    if (parent == apocrypha_location)
                    {
                        return true;
                    }
                    parent = parent->parentLoc;

                    safeguard++;
                }

                //kind of bad but whatever
                //if (safeguard == 100)
                {
                    //   return false
                }
            }
        }



        return false;
    }







    RE::NiPoint3 get_swinging_pass_tip_pos(RE::TESObjectREFR* swinging_pass)
    {
        RE::NiPoint3 result = RE::NiPoint3::Zero();

        if (swinging_pass)
        {
            auto the_3d = swinging_pass->Get3D();

            if (the_3d)
            {
                auto asnode = the_3d->AsNode();

                if (asnode)
                {
                    auto temp1 = MiscThings::niav_recurse_names(asnode);
                    auto temp2 = MiscThings::niav_recurse(asnode);

                    auto tip_bone = asnode->GetObjectByName("bone09");

                    if (tip_bone)
                    {
                        return tip_bone->world.translate;
                    }
                }
            }
        }


        return result;
    }



    RE::NiPoint3 get_swinging_pass_tip_pos2(RE::TESObjectREFR* swinging_pass)
    {
        RE::NiPoint3 result = RE::NiPoint3::Zero();

        if (swinging_pass)
        {
            auto the_3d = swinging_pass->Get3D();

            if (the_3d)
            {
                auto asnode = the_3d->AsNode();

                if (asnode)
                {
                    auto temp1 = MiscThings::niav_recurse_names(asnode);
                    auto temp2 = MiscThings::niav_recurse(asnode);

                    auto tip_bone = asnode->GetObjectByName("bone08");

                    if (tip_bone)
                    {
                        return tip_bone->world.translate;
                    }
                }
            }
        }


        return result;
    }




    bool check_custom_path_end = false;
    RE::NiPoint3 custom_path_end{};
    int action_after_custom_path_end = 0;

    RE::TESObjectREFR* activator_to_check = nullptr;
    RE::TESObjectREFR* custom_correction_object = nullptr;

    float special_threshold = 0.0f;

    void reset_apocrypha_redirects()
    {
        check_custom_path_end = false;
        custom_path_end = RE::NiPoint3::Zero();
        action_after_custom_path_end = 0;
        activator_to_check = nullptr;
        custom_correction_object = nullptr;
        special_threshold = 0.0f;
    }



    apocrypha_result book2_zone2(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        if (target_pos.GetDistance({ 9545.43750, -10321.0703, -2632.41431 }) > 2700.0f)
        {
            RE::TESObjectREFR* scrye1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037cfa);
            RE::TESObjectREFR* scrye2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037ce6);
            RE::TESObjectREFR* bridge1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037cae);
            RE::TESObjectREFR* gate1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037cc4);
            RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d18);

            if (!scrye1 || !bridge1 || !scrye2 || !gate1 || !exit_book)
                return result;

            if (MiscThings::two_state_activator_state(bridge1) != 0)
            {
                if (player->GetDistance(scrye1) < 100.0f)
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = scrye1;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
                else
                {
                    if (current_action == 0)
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(scrye1->GetPosition());

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = scrye1;
                        result.interaction = 1;
                        return result;
                    }
                }
            }
            else
            {
                if (MiscThings::two_state_activator_state(gate1) != 0)
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = scrye2;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
                else
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = exit_book;
                    result.interaction = 1;
                    //result.clear_path = true;

                    return result;
                }
            }
        }


        return result;

    }








    apocrypha_result book2_zone1(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        RE::NiPoint3 start_platform = { 11306.3486, -4396.10889, -2736.18945 };


        RE::TESObjectREFR* bridge1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c56);
        RE::TESObjectREFR* bridge1_activator = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c57);
        RE::TESObjectREFR* swinging_pass_part1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c12);

        RE::TESObjectREFR* bridge2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c62);
        RE::TESObjectREFR* bridge2_activator = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c61);
        RE::TESObjectREFR* swinging_pass_part2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c11);

        RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d15);
        

        RE::NiPoint3 bridge1_start = { 10867.0840, -4383.05762, -2682.35352 };
        RE::NiPoint3 bridge1_end = { 10260.9609, -4367.77734, -2684.21875 };
        RE::NiPoint3 pass_center = { 7919.30664, -4359.89111, -2666.61523 };
        RE::NiPoint3 bridge2_start = { 4901.15137, -4358.97705, -2669.25537 };
        RE::NiPoint3 bridge2_end = { 5592.51611, -4379.97217, -2684.11206 };

        RE::NiPoint3 island2_edge = { 6421.02246, -2961.57056, -2668.45312 };




        if (!bridge1 || !bridge1_activator || !swinging_pass_part1 || !dummy || !exit_book || !bridge2 || !bridge2_activator || !swinging_pass_part2)
            return result;

        auto book_pos = exit_book->GetPosition();


        auto pass1_tip_pos = get_swinging_pass_tip_pos(swinging_pass_part1);
        auto pass2_tip_pos = get_swinging_pass_tip_pos(swinging_pass_part2);


        if (current_action == 1 && activator_to_check)
        {
            if (MiscThings::two_state_activator_state(activator_to_check) == 0)
            {
                result.action = -999; //end of this section
                return result;
            }
            else
            {
                //continue
                result.action = 1; //continue
                result.interaction = 1;
                result.target = target; //continue
                result.dont_save_interaction = true;
                result.dont_save_target = true;
                return result;
            }
        }


        if (player_pos.GetDistance(custom_path_end) < 80.0f)
            {
                result.action = action_after_custom_path_end; //end of this section
                reset_apocrypha_redirects();
                return result;
            }


        if (current_action == 2)
        {
            if (current_apocrypha_id == 10)
            {
                if (player_pos.GetDistance(bridge2_end) > 400.0f && player_pos.GetDistance(pass2_tip_pos) < 100.0f)
                {
                    result.dont_save_id = true;
                    result.action = 3;

                    MiscThings::SetPosition_moveto(dummy, bridge2_end);

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
            }

            if (current_apocrypha_id == 12)
            {
                if (player_pos.GetDistance(bridge2_end) > 300.0f && player_pos.GetDistance(pass2_tip_pos) < 100.0f)
                {
                    result.dont_save_id = true;
                    result.action = 3;

                    MiscThings::SetPosition_moveto(dummy, bridge2_end);

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
                else
                {
                    if (player_pos.GetDistance(bridge2_end) <= 300.0f)
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(bridge2_start);

                        MiscThings::SetPosition_moveto(dummy, bridge2_start);

                        check_custom_path_end = true;
                        custom_path_end = bridge2_start;
                        action_after_custom_path_end = -999;

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = exit_book;
                        result.interaction = 1;
                        return result;
                    }
                }
            }




            if (current_apocrypha_id == 11 && custom_correction_object)
            {
                auto correct_pos = get_swinging_pass_tip_pos(custom_correction_object);

                ApocryphaCustomPaths::template_path.clear();
                ApocryphaCustomPaths::template_path.push_back(player_pos);
                ApocryphaCustomPaths::template_path.push_back(correct_pos);

                result.custom_path = ApocryphaCustomPaths::template_path;

                if (player_pos.GetDistance(island2_edge) > 400.0f && player_pos.GetDistance(pass2_tip_pos) < 100.0f)
                {
                    result.dont_save_id = true;
                    result.action = 3;

                    MiscThings::SetPosition_moveto(dummy, island2_edge);

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
                else
                {
                    if (player_pos.GetDistance(island2_edge) <= 400.0f)
                    {
                        //walk off to island2

                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(island2_edge);

                        MiscThings::SetPosition_moveto(dummy, island2_edge);

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = dummy;

                        check_custom_path_end = true;
                        custom_path_end = island2_edge;
                        action_after_custom_path_end = -999;
                        result.id = 0;
                        result.interaction = 1;
                        return result;

                    }
                    else
                    {
                        //tell walker to correct its path
                        result.correct_custom_path = true;
                        return result;
                    }
                }



                return result;
            }




            //pause on the tip a little
        }


        if (current_action == 0)
        {
            if (player_pos.GetDistance(start_platform) < 1000.0f && target_pos.GetDistance(start_platform) > 1000.0f)
            {
                //first platform. lower the bridge if not lowered, walk to end of bridge, wait for pass to be in the middle
                if (MiscThings::two_state_activator_state(bridge1) != 0)
                {
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = bridge1_activator;
                    result.interaction = 1;
                    
                    activator_to_check = bridge1;

                    return result;
                }
                else
                {
                    //bridge is down. stand on its end

                    if (player_pos.GetDistance(bridge1_end) > 100.0f)
                    {
                        //go to bridge end
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.custom_path = ApocryphaCustomPaths::book2_zone1_bridge1;
                        result.target = dummy;
                        
                        check_custom_path_end = true;
                        custom_path_end = bridge1_end;
                        action_after_custom_path_end = -999;

                        result.interaction = 1;
                        return result;
                    }
                    else
                    {
                        MiscThings::SetPosition_moveto(dummy, pass1_tip_pos);


                        if (pass1_tip_pos.GetDistance(player_pos) < 500.0f)
                        {
                            //go to bridge center
                            result.action = 2; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;
                            result.custom_path = ApocryphaCustomPaths::book2_zone1_pass1;
                            result.target = dummy;
                            
                            check_custom_path_end = true;
                            custom_path_end = pass_center;
                            action_after_custom_path_end = -888;

                            result.interaction = 1;
                            return result;
                        }
                        else
                        {
                            result.action = 3;
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;
                            result.target = dummy;
                            result.reset_curent_action = true;
                            return result;
                        }
                    }

                }
            }
            else
            {
                //center of pass
                if (player_pos.GetDistance(pass_center) < 1000.0f)
                {
                    //target on first island
                    if (target_pos.GetDistance(start_platform) < 1000.0f)
                    {
                        //go to bridge end
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.custom_path = ApocryphaCustomPaths::book2_zone1_bridge1_inv;
                        result.target = dummy;
                        
                        check_custom_path_end = true;
                        custom_path_end = bridge1_start;
                        action_after_custom_path_end = -999;

                        result.interaction = 1;
                        return result;
                    }
                    else
                    {
                        //target also here
                        if (target_pos.GetDistance(pass_center) < 600.0f)
                        {
                            //go to bridge end
                            result.action = 2; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;

                            ApocryphaCustomPaths::template_path.clear();
                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                            ApocryphaCustomPaths::template_path.push_back(target_pos);

                            result.custom_path = ApocryphaCustomPaths::template_path;
                            result.target = target;
                            result.interaction = 1;
                            return result;
                        }
                        else
                        {
                            //target on island2
                            if (target_pos.GetDistance(island2_edge) < 1000.0f)
                            {
                                //go to side island
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(player_pos);
                                ApocryphaCustomPaths::template_path.push_back(pass2_tip_pos);

                                MiscThings::SetPosition_moveto(dummy, island2_edge);

                                result.custom_path = ApocryphaCustomPaths::template_path;
                                result.target = dummy;
                                
                                check_custom_path_end = true;
                                custom_path_end = island2_edge;
                                action_after_custom_path_end = -999;

                                result.id = 11;
                                custom_correction_object = swinging_pass_part2;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                //target not on island2 and not on start island
                                if (MiscThings::two_state_activator_state(bridge2) != 0)
                                {
                                    //go to island2
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(pass2_tip_pos);

                                    MiscThings::SetPosition_moveto(dummy, island2_edge);

                                    check_custom_path_end = true;
                                    custom_path_end = island2_edge;
                                    action_after_custom_path_end = -999;

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;
                                    result.interaction = 1;

                                    result.id = 11;
                                    custom_correction_object = swinging_pass_part2;

                                    return result;
                                }
                                else
                                {
                                    //go to book
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(bridge2_start);

                                    MiscThings::SetPosition_moveto(dummy, bridge2_start);

                                    check_custom_path_end = true;
                                    custom_path_end = bridge2_start;
                                    action_after_custom_path_end = -999;

                                    result.id = 10;

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = exit_book;
                                    result.interaction = 1;
                                    return result;

                                }
                            }
                        }
                    }
                }
                else
                {
                    if (player_pos.GetDistance(island2_edge) < 1000.0f)
                    {
                        //island2

                        
                        if (target_pos.GetDistance(island2_edge) > 1000.0f)
                        {
                            //target is not on island2. check if bridge2 is down, activate it if not. then go to island center

                            if (MiscThings::two_state_activator_state(bridge2) != 0)
                            {
                                result.action = 1; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.custom_path = {};
                                result.target = bridge2_activator;
                                result.interaction = 1;

                                activator_to_check = bridge2;

                                result.clear_path = true;

                                return result;
                            }
                            else
                            {
                                if (player_pos.GetDistance(island2_edge) > 100.0f)
                                {
                                    //go to island edge
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(island2_edge);

                                    MiscThings::SetPosition_moveto(dummy, bridge2_start);

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = island2_edge;
                                    action_after_custom_path_end = -999;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                    if (pass2_tip_pos.GetDistance(player_pos) < 300.0f)
                                    {
                                        //go to swinging pass
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                                        ApocryphaCustomPaths::template_path.push_back(pass2_tip_pos);

                                        MiscThings::SetPosition_moveto(dummy, pass2_tip_pos);

                                        result.custom_path = ApocryphaCustomPaths::template_path;

                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = pass2_tip_pos;
                                        action_after_custom_path_end = -999;

                                        result.id = 12;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        MiscThings::SetPosition_moveto(dummy, pass2_tip_pos);
                                        result.action = 3;
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.target = dummy;
                                        result.reset_curent_action = true;
                                        return result;
                                    }


                                }
                            }
                        }
                    }
                    else
                    {
                        if (player_pos.GetDistance(book_pos) < 1000.0f)
                        {
                            //on end island.
                            if (target_pos.GetDistance(book_pos) > 1000.0f && target_pos.GetDistance(pass_center) > 1000.0f && target_pos.GetDistance(island2_edge) > 1000.0f && target_pos.GetDistance(start_platform) > 1000.0f)
                            {
                                //target in different zone. book
                                result.action = 1; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.custom_path = {};
                                result.target = exit_book;
                                result.interaction = 1;
                                result.clear_path = true;

                                return result;
                            }
                            else
                            {
                                if (target_pos.GetDistance(book_pos) > 1000.0f)
                                {
                                    //target somewhere in this zone. bride2 edge, then wait for platform, get on it, go to center

                                    if (player_pos.GetDistance(bridge2_end) > 100.0f)
                                    {
                                        //go to bridge end
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(bridge2_start);
                                        ApocryphaCustomPaths::template_path.push_back(bridge2_end);

                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bridge2_end;
                                        action_after_custom_path_end = -999;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        MiscThings::SetPosition_moveto(dummy, pass2_tip_pos);


                                        if (pass2_tip_pos.GetDistance(player_pos) < 500.0f)
                                        {
                                            //go to pass center
                                            result.action = 2; //initiate
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;
                                            
                                            ApocryphaCustomPaths::template_path.clear();
                                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                                            ApocryphaCustomPaths::template_path.push_back(pass_center);

                                            result.target = dummy;

                                            check_custom_path_end = true;
                                            custom_path_end = pass_center;
                                            action_after_custom_path_end = -888;

                                            result.interaction = 1;
                                            return result;
                                        }
                                        else
                                        {
                                            result.action = 3;
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;
                                            result.target = dummy;
                                            result.reset_curent_action = true;
                                            return result;
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







    










    bool inside_book2_zone3_segment2_top(RE::NiPoint3 point)
    {
        if (point.z < -439.0f || point.z > 600.0f)
            return false;


        RE::NiPoint2 a = { 7388.79736, 13421.2568 };
        RE::NiPoint2 b = { 7388.79736, 15674.8564 };
        RE::NiPoint2 c = { 6014.57568, 15674.8564 };
        RE::NiPoint2 d = { 6014.57568, 13421.2568 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }

    bool inside_book2_zone3_segment2_bottom(RE::NiPoint3 point)
    {
        if (point.GetDistance({ 4921.83105, 16014.5312, -482.837341 }) < 1264.0f)
            return true;

        if (point.z > -439.0f)
            return false;

        RE::NiPoint2 a = { 3339.12549, 13715.2393 };
        RE::NiPoint2 b = { 3339.12549, 17682.6328 };
        RE::NiPoint2 c = { 6340.84082, 17682.6328 };
        RE::NiPoint2 d = { 6340.84082, 13715.2393 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }



    //it does not check if object is in zone3; only call for objects that are guaranteed to be in zone3


    bool inside_book2_zone3_segment1(RE::NiPoint3 point)
    {

        RE::TESObjectREFR* bridge2_bottom = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c03);

        if (bridge2_bottom && MiscThings::two_state_activator_state(bridge2_bottom) == 0)
            if (inside_book2_zone3_segment2_bottom(point))
                return true; //blend these zones together if bridge is down



        if (point.z > 600.0f)
            return false;

        if (point.x < 3451.0f)
            return true;


        RE::NiPoint2 a = { 5770.20312, 13700.3896 };
        RE::NiPoint2 b = { 5770.20312, 11761.7949 };
        RE::NiPoint2 c = { 3450.58472, 11761.7949 }; 
        RE::NiPoint2 d = { 3450.58472, 13700.3896 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }




    bool inside_book2_zone3_segment3(RE::NiPoint3 point)
    {
        if (point.z > 600.0f)
            return false;

        RE::NiPoint2 a = { 7784.75537, 13371.0254 };
        RE::NiPoint2 b = { 7784.75537, 11945.2217 };
        RE::NiPoint2 c = { 10360.0078, 11945.2217 };
        RE::NiPoint2 d = { 10360.0078, 13371.0254 };

        RE::NiPoint2 p = { point.x, point.y };
        if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
            return true;

        return false;
    }


    bool inside_book2_zone3_segment_last(RE::NiPoint3 point)
    {
        if (point.z > 650.0f)
            return true;
        else
            return false;
    }




    bool inside_book1_zone1(RE::TESObjectREFR* object)
    {
        if (object)
        {
            auto worldspace = object->GetWorldspace();
            auto object_pos = object->GetPosition();

            if (worldspace && worldspace->formID == 0x401c0b2)
            {
                RE::NiPoint2 a = { 5793.20264, 5111.23926 };
                RE::NiPoint2 b = { 5793.20264, -1604.54480 };
                RE::NiPoint2 c = { -1448.03699, -1604.54480 };
                RE::NiPoint2 d = { -1448.03699, 5111.23926 };

                RE::NiPoint2 p = { object_pos.x, object_pos.y };
                if (MiscThings::is_inside_of_rectangle(p, a, b, c, d))
                    if (object_pos.z < 2100.0f)
                        return true;
            }
        }

        return false;
    }






    apocrypha_result book2_zone3(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};


        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        RE::TESObjectREFR* scrye2_bottom = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c0b);
        RE::TESObjectREFR* bridge2_bottom = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037c03);

        RE::TESObjectREFR* scrye_switch1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d47);
        RE::TESObjectREFR* scrye_switch2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d45);
        RE::TESObjectREFR* gate_switch = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d43);

        RE::TESObjectREFR* passage = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40372e6);
        RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40388de);

        RE::TESObjectREFR* passage_onetime1 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d42);
        RE::TESObjectREFR* passage_onetime2 = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d41);

        RE::TESObjectREFR* bridge_lurker = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x4037d4d);

        RE::TESObjectREFR* final_scrye = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40388c1);
        RE::TESObjectREFR* final_bridge = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40388d1);


        RE::NiPoint3 bus_station1 = { 5363.65576, 12896.8096, -380.083557 };
        RE::NiPoint3 bus_station2 = { 6718.90137, 13571.0391, -380.305176 };
        RE::NiPoint3 bus_station3 = { 8041.59766, 12736.7480, -379.793213 };
        RE::NiPoint3 passage_center = { 6724.47314, 11268.5918, -399.649994 };

        RE::NiPoint3 passage_tip = get_swinging_pass_tip_pos(passage);
        RE::NiPoint3 passage_tip2 = get_swinging_pass_tip_pos2(passage);





        if (!scrye2_bottom || !bridge2_bottom || !scrye_switch1 || !scrye_switch2 || !gate_switch || !passage || !exit_book || !passage_onetime1 || !passage_onetime2 || !bridge_lurker)
            return result;


        float threshold = 80.0f;

        if (special_threshold != 0.0f)
            threshold = special_threshold;

        if (player_pos.GetDistance(custom_path_end) < threshold)
        {
            result.action = action_after_custom_path_end; //end of this section
            reset_apocrypha_redirects();
            return result;
        }



        if (current_action == 1 && activator_to_check)
        {
            if (MiscThings::two_state_activator_state(activator_to_check) == 0)
            {
                result.action = -999; //end of this section
                return result;
            }
            else
            {
                //continue
                result.action = 1; //continue
                result.interaction = 1;
                result.target = target; //continue
                result.dont_save_interaction = true;
                result.dont_save_target = true;
                return result;
            }
        }



        if (current_action == 2)
        {

            if (current_apocrypha_id == 70)
            {
                //onetime passage 1
                if (MiscThings::two_state_activator_state(passage_onetime1) == 2)
                {
                    //wait for it to bend, stare at bridge2

                    result.dont_save_id = true;
                    result.action = 3;

                    MiscThings::SetPosition_moveto(dummy, bridge_lurker->GetPosition());

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
                else
                {
                    if (MiscThings::two_state_activator_state(passage_onetime1) == 0)
                    {
                        result.action = -999; //end of this section
                        return result;
                    }
                    /*
                    if (MiscThings::two_state_activator_state(passage_onetime1) == 0)
                    {
                        //stand in front of bridge and wait for it to lower. there is also that lurker behind it..

                        RE::NiPoint3 stand_here = { 8387.55176, 7848.67188, -399.649994 };

                        MiscThings::SetPosition_moveto(dummy, stand_here);

                        result.action = 1; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;
                        result.custom_path = {};
                        result.target = dummy;
                        result.interaction = 1;

                        activator_to_check = bridge_lurker;

                        return result;
                    }
                    */
                }
            }


            if (current_apocrypha_id == 80)
            {
                //onetime passage 1
                if (MiscThings::two_state_activator_state(passage_onetime2) == 2)
                {
                    //wait for it to bend, stare at skrye2

                    result.dont_save_id = true;
                    result.action = 3;

                    MiscThings::SetPosition_moveto(dummy, scrye_switch2->GetPosition());

                    result.target = dummy;
                    result.dont_save_target = true;
                    result.dont_save_interaction = true;
                    result.dont_save_action = true;
                    return result;
                }
                else
                {
                    if (MiscThings::two_state_activator_state(passage_onetime2) == 0)
                    {
                        result.action = -999; //end of this section
                        return result;
                    }
                }
            }


            if (current_apocrypha_id == 10)
            {
                if (player_pos.GetDistance(passage_tip) < 100.0f)
                {
                    //we should be on the tip of passage and need to go into the center
                    result.action = 2; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;

                    ApocryphaCustomPaths::template_path.clear();
                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                    ApocryphaCustomPaths::template_path.push_back(passage_center);

                    MiscThings::SetPosition_moveto(dummy, passage_center);

                    check_custom_path_end = true;
                    custom_path_end = passage_center;
                    action_after_custom_path_end = -999;

                    result.custom_path = ApocryphaCustomPaths::template_path;
                    result.target = dummy;
                    result.interaction = 1;
                    result.id = 0;
                    return result;
                }
                else
                {
                    //correct
                    auto correct_pos = get_swinging_pass_tip_pos(passage);

                    ApocryphaCustomPaths::template_path.clear();
                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                    ApocryphaCustomPaths::template_path.push_back(correct_pos);

                    result.custom_path = ApocryphaCustomPaths::template_path;
                    result.correct_custom_path = true;
                    return result;
                }

            }


            if (current_apocrypha_id == 12 || current_apocrypha_id == 120)
            {
                if (current_apocrypha_id == 120)
                {
                    if (player_pos.GetDistance(bus_station2) > 300.0f)
                    {
                        result.dont_save_id = true;
                        result.action = 3;

                        MiscThings::SetPosition_moveto(dummy, bus_station2);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(bus_station2);

                        MiscThings::SetPosition_moveto(dummy, bus_station2);

                        check_custom_path_end = true;
                        custom_path_end = bus_station2;
                        action_after_custom_path_end = -999;

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = dummy;
                        result.interaction = 1;
                        return result;
                    }

                }
                else
                {
                    //need to actually get to the point first

                    if (player_pos.GetDistance(passage_tip) < 90.0f)
                    {
                        //result.dont_save_id = true;
                        result.action = 3;
                        result.id = 120;
                        MiscThings::SetPosition_moveto(dummy, bus_station2);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        //correct
                        auto correct_pos = get_swinging_pass_tip_pos(custom_correction_object);
                        auto correct_pos2 = get_swinging_pass_tip_pos2(custom_correction_object);

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos2);


                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.correct_custom_path = true;
                        return result;
                    }

                }
            }


            if (current_apocrypha_id == 13 || current_apocrypha_id == 130)
            {
                if (current_apocrypha_id == 130)
                {
                    if (player_pos.GetDistance(bus_station3) > 270.0f)
                    {
                        result.dont_save_id = true;
                        result.action = 3;

                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(bus_station3);

                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                        check_custom_path_end = true;
                        custom_path_end = bus_station3;
                        action_after_custom_path_end = -999;

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = dummy;
                        result.interaction = 1;
                        return result;
                    }

                }
                else
                {
                    //need to actually get to the point first

                    if (player_pos.GetDistance(passage_tip) < 90.0f)
                    {
                        //result.dont_save_id = true;
                        result.action = 3;
                        result.id = 130;
                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        //correct
                        auto correct_pos = get_swinging_pass_tip_pos(custom_correction_object);
                        auto correct_pos2 = get_swinging_pass_tip_pos2(custom_correction_object);

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos2);


                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.correct_custom_path = true;
                        return result;
                    }

                }
            }



            if (current_apocrypha_id == 11 || current_apocrypha_id == 110)
            {
                if (current_apocrypha_id == 110)
                {
                    if (player_pos.GetDistance(bus_station1) > 270.0f)
                    {
                        result.dont_save_id = true;
                        result.action = 3;

                        MiscThings::SetPosition_moveto(dummy, bus_station1);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        result.action = 2; //initiate
                        result.dont_save_interaction = false;
                        result.dont_save_target = false;

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(bus_station1);

                        MiscThings::SetPosition_moveto(dummy, bus_station1);

                        check_custom_path_end = true;
                        custom_path_end = bus_station1;
                        action_after_custom_path_end = -999;

                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.target = dummy;
                        result.interaction = 1;
                        return result;
                    }

                }
                else
                {
                    //need to actually get to the point first

                    if (player_pos.GetDistance(passage_tip) < 90.0f)
                    {
                        //result.dont_save_id = true;
                        result.action = 3;
                        result.id = 110;
                        MiscThings::SetPosition_moveto(dummy, bus_station1);

                        result.target = dummy;
                        result.dont_save_target = true;
                        result.dont_save_interaction = true;
                        result.dont_save_action = true;
                        return result;
                    }
                    else
                    {
                        //correct
                        auto correct_pos = get_swinging_pass_tip_pos(custom_correction_object);
                        auto correct_pos2 = get_swinging_pass_tip_pos2(custom_correction_object);

                        ApocryphaCustomPaths::template_path.clear();
                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos);
                        ApocryphaCustomPaths::template_path.push_back(correct_pos2);


                        result.custom_path = ApocryphaCustomPaths::template_path;
                        result.correct_custom_path = true;
                        return result;
                    }

                }
                
            }





            //pause on the tip a little
        }






        if (current_action == 0)
        {
            //getaway from bottom segment2
            if (inside_book2_zone3_segment2_bottom(player_pos) && !inside_book2_zone3_segment2_bottom(target_pos))
            {
                if (MiscThings::two_state_activator_state(bridge2_bottom) != 0)
                {
                    //initiation, must be continued because target will change
                    result.action = 1; //initiate
                    result.dont_save_interaction = false;
                    result.dont_save_target = false;
                    result.custom_path = {};
                    result.target = scrye2_bottom;
                    result.interaction = 1;

                    activator_to_check = bridge2_bottom;

                    return result;
                }
            }
            else
            {
                if (inside_book2_zone3_segment1(player_pos) && !inside_book2_zone3_segment1(target_pos))
                {
                    if (inside_book2_zone3_segment2_top(target_pos) || (inside_book2_zone3_segment2_bottom(target_pos) && MiscThings::two_state_activator_state(bridge2_bottom) != 0))
                    {
                        if (player_pos.GetDistance(bus_station1) > 100.0f)
                        {
                            //go to island edge
                            result.action = 2; //initiate
                            result.dont_save_interaction = false;
                            result.dont_save_target = false;

                            ApocryphaCustomPaths::template_path.clear();
                            ApocryphaCustomPaths::template_path.push_back(bus_station1);
                            ApocryphaCustomPaths::template_path.push_back(passage_tip);

                            MiscThings::SetPosition_moveto(dummy, bus_station1);

                            result.custom_path = ApocryphaCustomPaths::template_path;
                            result.target = dummy;

                            check_custom_path_end = true;
                            custom_path_end = bus_station1;
                            action_after_custom_path_end = -999;

                            result.append_to_normal_path = true;

                            result.interaction = 1;
                            return result;
                        }
                        else
                        {
                            //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                            if (passage_tip.GetDistance(player_pos) < 400.0f)
                            {
                                //go to swinging pass
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(player_pos);
                                ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                MiscThings::SetPosition_moveto(dummy, passage_tip);

                                result.custom_path = ApocryphaCustomPaths::template_path;

                                result.target = dummy;

                                check_custom_path_end = true;
                                custom_path_end = bus_station2;
                                action_after_custom_path_end = -999;

                                result.id = 12;

                                custom_correction_object = passage;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                MiscThings::SetPosition_moveto(dummy, passage_tip);
                                result.action = 3;
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;
                                result.target = dummy;
                                result.reset_curent_action = true;
                                return result;
                            }


                        }
                    }
                    else
                    {
                        if (inside_book2_zone3_segment3(target_pos))
                        {
                            if (player_pos.GetDistance(bus_station1) > 100.0f)
                            {
                                //go to island edge
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(bus_station1);
                                ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                MiscThings::SetPosition_moveto(dummy, bus_station1);

                                result.custom_path = ApocryphaCustomPaths::template_path;
                                result.target = dummy;

                                check_custom_path_end = true;
                                custom_path_end = bus_station1;
                                action_after_custom_path_end = -999;

                                result.append_to_normal_path = true;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                if (passage_tip.GetDistance(player_pos) < 400.0f)
                                {
                                    //go to swinging pass
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                    MiscThings::SetPosition_moveto(dummy, passage_tip);

                                    result.custom_path = ApocryphaCustomPaths::template_path;

                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = bus_station2;
                                    action_after_custom_path_end = -999;

                                    result.id = 13;

                                    custom_correction_object = passage;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    MiscThings::SetPosition_moveto(dummy, passage_tip);
                                    result.action = 3;
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    result.target = dummy;
                                    result.reset_curent_action = true;
                                    return result;
                                }


                            }
                        }
                        else
                        {
                            //need to go to passage center, target is not on islands
                            if (player_pos.GetDistance(bus_station1) > 100.0f)
                            {
                                //go to island edge
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(bus_station1);
                                ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                MiscThings::SetPosition_moveto(dummy, bus_station1);

                                result.custom_path = ApocryphaCustomPaths::template_path;
                                result.target = dummy;

                                check_custom_path_end = true;
                                custom_path_end = bus_station1;
                                action_after_custom_path_end = -999;

                                result.append_to_normal_path = true;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                if (passage_tip.GetDistance(player_pos) < 400.0f)
                                {
                                    //go to swinging pass
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                    MiscThings::SetPosition_moveto(dummy, passage_tip);

                                    result.custom_path = ApocryphaCustomPaths::template_path;

                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = passage_center;
                                    action_after_custom_path_end = -999;

                                    result.id = 10;

                                    custom_correction_object = passage;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    MiscThings::SetPosition_moveto(dummy, passage_tip);
                                    result.action = 3;
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    result.target = dummy;
                                    result.reset_curent_action = true;
                                    return result;
                                }


                            }
                        }
                    }
                    
                }
                else
                {
                    if (inside_book2_zone3_segment2_top(player_pos) && !inside_book2_zone3_segment2_top(target_pos) && !inside_book2_zone3_segment2_bottom(target_pos))
                    {
                        if (inside_book2_zone3_segment1(target_pos))
                        {
                            if (player_pos.GetDistance(bus_station2) > 100.0f)
                            {
                                //go to island edge
                                result.action = 2; //initiate
                                result.dont_save_interaction = false;
                                result.dont_save_target = false;

                                ApocryphaCustomPaths::template_path.clear();
                                ApocryphaCustomPaths::template_path.push_back(player_pos);
                                ApocryphaCustomPaths::template_path.push_back(bus_station2);

                                MiscThings::SetPosition_moveto(dummy, bus_station1);

                                result.custom_path = ApocryphaCustomPaths::template_path;
                                result.target = dummy;

                                check_custom_path_end = true;
                                custom_path_end = bus_station2;
                                action_after_custom_path_end = -999;

                                result.append_to_normal_path = true;

                                result.interaction = 1;
                                return result;
                            }
                            else
                            {
                                //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                if (passage_tip.GetDistance(player_pos) < 300.0f)
                                {
                                    //go to swinging pass
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(player_pos);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                    MiscThings::SetPosition_moveto(dummy, passage_tip);

                                    result.custom_path = ApocryphaCustomPaths::template_path;

                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = passage_tip;
                                    action_after_custom_path_end = -999;

                                    result.id = 11;

                                    custom_correction_object = passage;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    MiscThings::SetPosition_moveto(dummy, passage_tip);
                                    result.action = 3;
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    result.target = dummy;
                                    result.reset_curent_action = true;
                                    return result;
                                }


                            }
                        }
                        else
                        {
                            if (inside_book2_zone3_segment3(target_pos))
                            {
                                if (player_pos.GetDistance(bus_station2) > 100.0f)
                                {
                                    //go to island edge
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(bus_station2);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                    MiscThings::SetPosition_moveto(dummy, bus_station2);

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = bus_station2;
                                    action_after_custom_path_end = -999;

                                    result.append_to_normal_path = true;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                    if (passage_tip.GetDistance(player_pos) < 400.0f)
                                    {
                                        //go to swinging pass
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                        MiscThings::SetPosition_moveto(dummy, passage_tip);

                                        result.custom_path = ApocryphaCustomPaths::template_path;

                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bus_station3;
                                        action_after_custom_path_end = -999;

                                        result.id = 13;

                                        custom_correction_object = passage;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        MiscThings::SetPosition_moveto(dummy, passage_tip);
                                        result.action = 3;
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.target = dummy;
                                        result.reset_curent_action = true;
                                        return result;
                                    }


                                }
                            }
                            else
                            {
                                //need to go to passage center, target is not on islands
                                if (player_pos.GetDistance(bus_station2) > 100.0f)
                                {
                                    //go to island edge
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(bus_station2);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                    MiscThings::SetPosition_moveto(dummy, bus_station2);

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = bus_station2;
                                    action_after_custom_path_end = -999;

                                    result.append_to_normal_path = true;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                    if (passage_tip.GetDistance(player_pos) < 400.0f)
                                    {
                                        //go to swinging pass
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                        MiscThings::SetPosition_moveto(dummy, passage_tip);

                                        result.custom_path = ApocryphaCustomPaths::template_path;

                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = passage_center;
                                        action_after_custom_path_end = -999;

                                        result.id = 10;

                                        custom_correction_object = passage;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        MiscThings::SetPosition_moveto(dummy, passage_tip);
                                        result.action = 3;
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.target = dummy;
                                        result.reset_curent_action = true;
                                        return result;
                                    }


                                }
                            }
                        }

                    }
                    else
                    {
                        if (inside_book2_zone3_segment3(player_pos) && !inside_book2_zone3_segment3(target_pos))
                        {
                            if (inside_book2_zone3_segment1(target_pos))
                            {
                                if (player_pos.GetDistance(bus_station3) > 100.0f)
                                {
                                    //go to island edge
                                    result.action = 2; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;

                                    ApocryphaCustomPaths::template_path.clear();
                                    ApocryphaCustomPaths::template_path.push_back(bus_station3);
                                    ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                    MiscThings::SetPosition_moveto(dummy, bus_station3);

                                    result.custom_path = ApocryphaCustomPaths::template_path;
                                    result.target = dummy;

                                    check_custom_path_end = true;
                                    custom_path_end = bus_station3;
                                    action_after_custom_path_end = -999;

                                    result.append_to_normal_path = true;

                                    result.interaction = 1;
                                    return result;
                                }
                                else
                                {
                                    //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                    if (passage_tip.GetDistance(player_pos) < 400.0f)
                                    {
                                        //go to swinging pass
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(player_pos);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                        MiscThings::SetPosition_moveto(dummy, passage_tip);

                                        result.custom_path = ApocryphaCustomPaths::template_path;

                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bus_station1;
                                        action_after_custom_path_end = -999;

                                        result.id = 11;

                                        custom_correction_object = passage;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        MiscThings::SetPosition_moveto(dummy, passage_tip);
                                        result.action = 3;
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.target = dummy;
                                        result.reset_curent_action = true;
                                        return result;
                                    }


                                }
                            }
                            else
                            {
                                if (inside_book2_zone3_segment2_top(target_pos) || inside_book2_zone3_segment2_bottom(target_pos))
                                {
                                    if (player_pos.GetDistance(bus_station3) > 100.0f)
                                    {
                                        //go to island edge
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(bus_station3);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                                        result.custom_path = ApocryphaCustomPaths::template_path;
                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bus_station3;
                                        action_after_custom_path_end = -999;

                                        result.append_to_normal_path = true;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                        if (passage_tip.GetDistance(player_pos) < 400.0f)
                                        {
                                            //go to swinging pass
                                            result.action = 2; //initiate
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;

                                            ApocryphaCustomPaths::template_path.clear();
                                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                                            ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                            ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                            MiscThings::SetPosition_moveto(dummy, passage_tip);

                                            result.custom_path = ApocryphaCustomPaths::template_path;

                                            result.target = dummy;

                                            check_custom_path_end = true;
                                            custom_path_end = bus_station2;
                                            action_after_custom_path_end = -999;

                                            result.id = 12;

                                            custom_correction_object = passage;

                                            result.interaction = 1;
                                            return result;
                                        }
                                        else
                                        {
                                            MiscThings::SetPosition_moveto(dummy, passage_tip);
                                            result.action = 3;
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;
                                            result.target = dummy;
                                            result.reset_curent_action = true;
                                            return result;
                                        }


                                    }
                                }
                                else
                                {
                                    //need to go to passage center, target is not on islands
                                    if (player_pos.GetDistance(bus_station3) > 100.0f)
                                    {
                                        //go to island edge
                                        result.action = 2; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;

                                        ApocryphaCustomPaths::template_path.clear();
                                        ApocryphaCustomPaths::template_path.push_back(bus_station3);
                                        ApocryphaCustomPaths::template_path.push_back(passage_tip);

                                        MiscThings::SetPosition_moveto(dummy, bus_station3);

                                        result.custom_path = ApocryphaCustomPaths::template_path;
                                        result.target = dummy;

                                        check_custom_path_end = true;
                                        custom_path_end = bus_station3;
                                        action_after_custom_path_end = -999;

                                        result.append_to_normal_path = true;

                                        result.interaction = 1;
                                        return result;
                                    }
                                    else
                                    {
                                        //we are on the edge of island. wait for pass to swing close to us, then go to pass edge

                                        if (passage_tip.GetDistance(player_pos) < 400.0f)
                                        {
                                            //go to swinging pass
                                            result.action = 2; //initiate
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;

                                            ApocryphaCustomPaths::template_path.clear();
                                            ApocryphaCustomPaths::template_path.push_back(player_pos);
                                            ApocryphaCustomPaths::template_path.push_back(passage_tip);
                                            ApocryphaCustomPaths::template_path.push_back(passage_tip2);

                                            MiscThings::SetPosition_moveto(dummy, passage_tip);

                                            result.custom_path = ApocryphaCustomPaths::template_path;

                                            result.target = dummy;

                                            check_custom_path_end = true;
                                            custom_path_end = passage_center;
                                            action_after_custom_path_end = -999;

                                            result.id = 10;

                                            custom_correction_object = passage;

                                            result.interaction = 1;
                                            return result;
                                        }
                                        else
                                        {
                                            MiscThings::SetPosition_moveto(dummy, passage_tip);
                                            result.action = 3;
                                            result.dont_save_interaction = false;
                                            result.dont_save_target = false;
                                            result.target = dummy;
                                            result.reset_curent_action = true;
                                            return result;
                                        }


                                    }
                                }
                            }

                        }
                        else
                        {
                            if (inside_book2_zone3_segment_last(player_pos))
                            {
                                if (MiscThings::two_state_activator_state(final_bridge) != 0)
                                {
                                    result.action = 1; //initiate
                                    result.dont_save_interaction = false;
                                    result.dont_save_target = false;
                                    result.custom_path = {};
                                    result.target = final_scrye;
                                    result.interaction = 1;
                                    //result.clear_path = true;

                                    return result;
                                }
                            }
                            else
                            {
                                bool target_in_last_segment = inside_book2_zone3_segment_last(target_pos);

                                auto target_pos_actual = target_pos;

                                if (target_in_last_segment)
                                    target_pos = exit_book->GetPosition(); //pretend its our goal

                                

                                if ((target_pos.y < 10170.0f) && player_pos.y > 10170.0f)
                                {
                                    if (MiscThings::two_state_activator_state(gate_switch) == 0)
                                    {
                                        result.action = 1; //initiate
                                        result.dont_save_interaction = false;
                                        result.dont_save_target = false;
                                        result.custom_path = {};
                                        result.target = scrye_switch1;
                                        result.interaction = 1;
                                        //result.clear_path = true;

                                        return result;
                                    }
                                }
                                else
                                {
                                    if (player_pos.y < 10170.0f)
                                    {
                                        if (target_pos.x < 6520.0f && target_pos.y < 10170.0f)
                                        {
                                            if (MiscThings::two_state_activator_state(gate_switch) == 1)
                                            {
                                                if (MiscThings::two_state_activator_state(passage_onetime1) == 1)
                                                {
                                                    //some custom path
                                                    result.action = 2; //initiate
                                                    result.dont_save_interaction = false;
                                                    result.dont_save_target = false;

                                                    result.custom_path = ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass1;
                                                    result.target = scrye_switch2;

                                                    check_custom_path_end = true;
                                                    custom_path_end = ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass1.at(std::size(ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass1) - 1);
                                                    action_after_custom_path_end = -999;

                                                    result.append_to_normal_path = true;


                                                    result.id = 70;


                                                    result.interaction = 1;
                                                    return result;

                                                }
                                                else
                                                {
                                                    if (MiscThings::two_state_activator_state(passage_onetime1) == 0)
                                                    {
                                                        //stand in front of bridge and wait for it to lower. there is also that lurker behind it..

                                                        if (MiscThings::two_state_activator_state(bridge_lurker) != 0)
                                                        {
                                                            RE::NiPoint3 stand_here = { 8387.55176, 7848.67188, -399.649994 };

                                                            MiscThings::SetPosition_moveto(dummy, stand_here);

                                                            result.action = 1; //initiate
                                                            result.dont_save_interaction = false;
                                                            result.dont_save_target = false;
                                                            result.custom_path = {};
                                                            result.target = dummy;
                                                            result.interaction = 1;

                                                            activator_to_check = bridge_lurker;

                                                            return result;
                                                        }
                                                        else
                                                        {
                                                            if (MiscThings::two_state_activator_state(passage_onetime2) == 1)
                                                            {
                                                                //some custom path
                                                                result.action = 2; //initiate
                                                                result.dont_save_interaction = false;
                                                                result.dont_save_target = false;

                                                                result.custom_path = ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass2;
                                                                result.target = scrye_switch2;

                                                                check_custom_path_end = true;
                                                                custom_path_end = ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass2.at(std::size(ApocryphaCustomPaths::book2_zone3_onetime_rotating_pass2) - 1);
                                                                action_after_custom_path_end = -999;

                                                                result.append_to_normal_path = true;


                                                                result.id = 80;


                                                                result.interaction = 1;
                                                                return result;


                                                            }
                                                            else
                                                            {
                                                                //to scrye2
                                                                result.action = 1; //initiate
                                                                result.dont_save_interaction = false;
                                                                result.dont_save_target = false;
                                                                result.custom_path = {};
                                                                result.target = scrye_switch2;
                                                                result.interaction = 1;
                                                                //result.clear_path = true;

                                                                return result;
                                                            }
                                                        }
                                                    }





                                                }
                                            }
                                            else
                                            {
                                                //all the way is ready for book. go to wherever we need or redirect to book if target is in last zone
                                                if (inside_book2_zone3_segment_last(target_pos_actual))
                                                {
                                                    //to book
                                                    result.action = 1; //initiate
                                                    result.dont_save_interaction = false;
                                                    result.dont_save_target = false;
                                                    result.custom_path = {};
                                                    result.target = exit_book;
                                                    result.interaction = 1;
                                                    //result.clear_path = true;

                                                    return result;
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



        return result;

    }






    apocrypha_result book1_zone1(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_pos = player->GetPosition();

        auto dummy = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x70c1a25);

        if (!target)
            return result;

        auto target_pos = target->GetPosition();

        if (!inside_book1_zone1(target))
        {
            RE::TESObjectREFR* exit_book = (RE::TESObjectREFR*)RE::TESObjectREFR::LookupByID(0x40339a9);

            if (!exit_book)
                return result;

            result.action = 1; //initiate
            result.dont_save_interaction = false;
            result.dont_save_target = false;
            result.custom_path = {};
            result.target = exit_book;
            result.interaction = 1;
            //result.clear_path = true;

            return result;
        }


        return result;

    }









    int last_zone = 0;



    apocrypha_result apocrypha_redirects(RE::TESObjectREFR* target, int current_action, int current_apocrypha_id)
    {
        apocrypha_result result{};


        if (!Apocrypha::in_apocrypha())
            return result;


        auto player = RE::PlayerCharacter::GetSingleton();

        if (!player)
            return result;


        auto parent_cell = player->GetParentCell();
        auto worldspace = player->GetWorldspace();


        //detect where we are and give redirections accordingly
        auto player_pos = player->GetPosition();


        //book2
        if (parent_cell && parent_cell->formID == 0x401ede2)
        {
            int zone = 0;
            if (player_pos.GetDistance({ 7917.69774, -4353.098635, -2651.0885 }) < 4300.0f)
            {
                zone = 1;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book2_zone1(target, current_action, current_apocrypha_id);
            }

            if (player_pos.GetDistance({ 9545.43750, -10321.0703, -2632.41431 }) < 2700.0f)
            {
                zone = 2;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book2_zone2(target, current_action, current_apocrypha_id);
            }


            if (player_pos.y > 310.0f)
            {
                zone = 3;
                if (last_zone != zone && last_zone != 0)
                {
                    last_zone = zone;
                    result.action = -888;
                    return result;
                }
                else
                    last_zone = zone;

                return book2_zone3(target, current_action, current_apocrypha_id);
            }  
        }
            
        //book1

        if (inside_book1_zone1(player))
        {
            int zone = 1;
            if (last_zone != zone && last_zone != 0)
            {
                last_zone = zone;
                result.action = -888;
                return result;
            }
            else
                last_zone = zone;

            return book1_zone1(target, current_action, current_apocrypha_id);
        }





        return result;
    }






    




}



