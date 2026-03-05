//#include <Context/Context.hpp>


#include "Socket.hpp"
#include <neurosdk.h>
#include <string>
#include <string_view>

//using namespace Red;


extern bool is_in_game();
extern bool resend_active_force();







neurosdk_action ActionsList[] = {  
                                    
                                    Capabilities::WalkToObject::Action,
                                    Capabilities::LookAtObject::Action,
                                    Capabilities::ExploreWorld::Action,

                                    Capabilities::GetCurrentQuests::Action,
                                    Capabilities::FollowQuest::Action,
                                    
                                    Capabilities::GetLocations::Action,
                                    

                                    Capabilities::GetSpells::Action,
                                    Capabilities::CastEquipSpell::Action,
                                    Capabilities::UnlockShoutLevel::Action, //TODO: probably calculate ourselves and force just like levelup

                                    Capabilities::GetInventory::Action,
                                    Capabilities::UseInventoryItem::Action,
                                    Capabilities::CallWaitMenu::Action,
                                    Capabilities::OpenMap::Action,
                                    Capabilities::GetGold::Action,
                                    
                                    Capabilities::SelectForceChoice::Action,
                                    Capabilities::SelectForceChoiceMultiple::Action,
                                    Capabilities::SelectForceChoiceString::Action,

                                    Capabilities::GetObjectsAround::Action, //idk about this one
                                    Capabilities::GoToLocation::Action
                                };

neurosdk_action ActionsListNoForces[] = {

                                    Capabilities::WalkToObject::Action,
                                    Capabilities::LookAtObject::Action,
                                    Capabilities::ExploreWorld::Action,

                                    Capabilities::GetCurrentQuests::Action,
                                    Capabilities::FollowQuest::Action,

                                    Capabilities::GetLocations::Action,


                                    Capabilities::GetSpells::Action,
                                    Capabilities::CastEquipSpell::Action,
                                    Capabilities::UnlockShoutLevel::Action, //TODO: probably calculate ourselves and force just like levelup

                                    Capabilities::GetInventory::Action,
                                    Capabilities::UseInventoryItem::Action,
                                    Capabilities::CallWaitMenu::Action,
                                    Capabilities::OpenMap::Action,
                                    Capabilities::GetGold::Action,

                                    Capabilities::GetObjectsAround::Action, //idk about this one
                                    Capabilities::GoToLocation::Action
};



neurosdk_action ActionsListNoForces2[] = {

                                    //Capabilities::WalkToObject::Action,
                                    //Capabilities::LookAtObject::Action,
                                    //Capabilities::ExploreWorld::Action,

                                    //Capabilities::GetCurrentQuests::Action,
                                    //Capabilities::FollowQuest::Action,

                                    //Capabilities::GetLocations::Action,


                                    //Capabilities::GetSpells::Action,
                                    Capabilities::CastEquipSpell::Action,
                                   // Capabilities::UnlockShoutLevel::Action, //TODO: probably calculate ourselves and force just like levelup

                                    //Capabilities::GetInventory::Action,
                                    Capabilities::UseInventoryItem::Action,
                                    Capabilities::CallWaitMenu::Action,
                                    Capabilities::OpenMap::Action,
                                    //Capabilities::GetGold::Action,

                                    //Capabilities::GetObjectsAround::Action, //idk about this one
                                    //Capabilities::GoToLocation::Action
};



/*
neurosdk_action ActionsList[] = { QueryQuestContext::Action,
                                 QueryQuests::Action,
                                 QueryWaypoints::Action,
                                 QueryInventory::Action,
                                 QueryPlayerInfo::Action,
                                 QueryMoney::Action,
                                 QueryQuickhackableTargets::Action,
                                 DriveToDestination::Action,
                                 SelectChoiceOption::Action,
                                 SelectSMSResponse::Action,
                                 RunQuickhackOnTarget::Action,
                                 TrackQuest::Action,
                                 SummonCar::Action };
*/

constexpr auto ActionsCount = std::size(ActionsList);
constexpr auto ActionsCountNoForces = std::size(ActionsListNoForces);
constexpr auto ActionsCountNoForces2 = std::size(ActionsListNoForces2);

neuro::NeuroSocket::NeuroSocket()
    : m_context(nullptr)
{
}

neuro::NeuroSocket::~NeuroSocket()
{
    if (IsAlive())
    {
        neurosdk_context_destroy(&m_context);
    }
}





bool neuro::NeuroSocket::RespondToAction(StringView aActionId, StringView aMsg, bool aSuccess)
{
    neurosdk_message_t msg{
        .kind = NeuroSDK_MessageKind_ActionResult,
        .value = {.action_result = {.id = aActionId.Data(), .success = aSuccess, .message = aMsg.Data()}} };

    return neurosdk_context_send(&m_context, &msg) == NeuroSDK_None;
}

bool neuro::NeuroSocket::SendContext(StringView aContext, bool aSilent)
{
    neurosdk_message_t msg{ .kind = NeuroSDK_MessageKind_Context,
                           .value = {.context = {.message = aContext.Data(), .silent = aSilent}} };

    return neurosdk_context_send(&m_context, &msg) == NeuroSDK_None;
}

bool neuro::NeuroSocket::SendForcedAction(StringView aActionName, StringView aQuery, StringView aState,
    StringView aPriority)
{
    const char* tempActionNames[] = { aActionName.Data() };

    neurosdk_message_t msg{ .kind = NeuroSDK_MessageKind_ActionsForce,
                           .value = {.actions_force = {.state = aState.Data(),
                                                       .query = aQuery.Data(),
                                                       .priority = aPriority.Data(),
                                                       .ephemeral_context = false,
                                                       .action_names = tempActionNames,
                                                       .action_names_len = 1}} };

    return neurosdk_context_send(&m_context, &msg) == NeuroSDK_None;
}


bool neuro::NeuroSocket::Initialize()
{
    // Note: this is a FPS and we're polling every frame (which we shouldn't do, but...)
    // We can't have poll timeout be more than ~10ms without rethinking architecture
    neurosdk_context_create_desc_t desc{.url = "ws://localhost:8000", //TODO: why was this not filled?
                                        .game_name = "Skyrim",
                                        .poll_ms = PollRateMs,
                                        .flags = (neurosdk_context_create_flags_e)NEUROSDK_CONTEXT_CREATE_FLAGS_DEBUG,
                                        .callback_log = neuro::NeuroSocket::LogNeuro };

    // Retry with fallback flag (for Tony/other less conformant API implementations)
    *(uint32_t*)(&desc.flags) |= NeuroSDK_ContextCreateFlags_FallbackToNonRFCImplementation;

    if (const auto status = neurosdk_context_create(&m_context, &desc); status != NeuroSDK_None)
    {
        // Try: does this crash if we don't create context twice?
        *(uint32_t*)(&desc.flags) &= (~NeuroSDK_ContextCreateFlags_FallbackToNonRFCImplementation);

        if (const auto status = neurosdk_context_create(&m_context, &desc); status != NeuroSDK_None)
        {

            //Context::PluginLogger->ErrorF(Context::PluginHandle, "Failed to create Neuro SDK context! Error %s",
            //   neurosdk_error_string(status));

            return false;
        }
    }

    // Tell backend we started up
    neurosdk_message_t connectionMessage{ .kind = NeuroSDK_MessageKind_Startup };

    if (const auto status = neurosdk_context_send(&m_context, &connectionMessage); status != NeuroSDK_None)
    {
        //Context::PluginLogger->ErrorF(Context::PluginHandle, "Failed to inform backend of startup! Error %s",
        //    neurosdk_error_string(status));

        return false;
    }

    /* //now filled when necessary
    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsRegister,
                                         .value = {.actions_register = {.actions = ActionsList,
                                                                        .actions_len = ActionsCount}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        //Context::PluginLogger->ErrorF(Context::PluginHandle, "Failed to inform backend of capabilities! Error %s",
        //    neurosdk_error_string(status));

        return false;
    }
    */


    register_allowed_actions(true);


    std::string in_game_text = "You are ingame. Use commands to interact with the world. ";

    if (!is_in_game())
        in_game_text = "You are not in game. Wait for game to start. ";


    return SendContext(("You are playing Skyrim, an action RPG. " + in_game_text + "Try to have a fun adventure. ").c_str(),
        true) &&
        IsAlive();
}



bool neuro::NeuroSocket::register_actions(neurosdk_action actions[], int size)
{
    if (!IsAlive())
        return false;

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsRegister,
                                         .value = {.actions_register = {.actions = actions,
                                                                        .actions_len = size}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }

    return true;
}



bool neuro::NeuroSocket::unregister_actions(const char** action_names, int size)
{
    if (!IsAlive())
        return false;

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsUnregister,
                                         .value = {.actions_unregister = {.action_names = action_names,
                                                                        .action_names_len = size}}};

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }

    return true;
}


bool neuro::NeuroSocket::unregister_all()
{
    if (!IsAlive())
        return false;


    const char* action_names[ActionsCountNoForces];

    for (int i = 0; i < ActionsCountNoForces; i++)
    {
        action_names[i] = ActionsListNoForces[i].name;
    }

    if (ActionsCountNoForces <= 0)
        return true;

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsUnregister,
                                         .value = {.actions_unregister = {.action_names = action_names,
                                                                        .action_names_len = ActionsCountNoForces}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }

    return true;
}


bool neuro::NeuroSocket::unregister_all2()
{
    if (!IsAlive())
        return false;


    const char* action_names[ActionsCountNoForces2];

    for (int i = 0; i < ActionsCountNoForces2; i++)
    {
        action_names[i] = ActionsListNoForces2[i].name;
    }

    if (ActionsCountNoForces2 <= 0)
        return true;

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsUnregister,
                                         .value = {.actions_unregister = {.action_names = action_names,
                                                                        .action_names_len = ActionsCountNoForces2}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }

    return true;
}



neurosdk_action actions_to_register[ActionsCount]{};

bool neuro::NeuroSocket::register_allowed_actions(bool reconnect)
{
    if (MiscThings::is_in_main_menu())
        return false;

    if (!IsAlive())
        return false;


    int action_pos = 0;


    auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
    int unbound_quest_stage = threshold_quest->GetCurrentStageID();

    int active_force = get_active_force();

    if (active_force == -1)
    {
        if (!reconnect || !MiscThings::have_force_only_menu_open())
        {
            if (unbound_quest_stage >= 15)
            {
                if (!MiscThings::is_intro()) //must be watched to refresh
                {

                    if (MiscThings::is_objects_around_valid())
                    {
                        actions_to_register[action_pos] = Capabilities::WalkToObject::Action; action_pos++;
                        actions_to_register[action_pos] = Capabilities::GetObjectsAround::Action; action_pos++;
                    }

                    if (MiscThings::have_any_quests())
                    {
                        actions_to_register[action_pos] = Capabilities::GetCurrentQuests::Action; action_pos++;
                        actions_to_register[action_pos] = Capabilities::FollowQuest::Action; action_pos++;
                    }


                    if (!MiscThings::is_intro2()) //must be watched to refresh
                    {
                        actions_to_register[action_pos] = Capabilities::GetSpells::Action; action_pos++;
                        actions_to_register[action_pos] = Capabilities::CastEquipSpell::Action; action_pos++;

                        if (MiscThings::player_has_shouts_to_unlock()) //must be watched to refresh
                        {
                            actions_to_register[action_pos] = Capabilities::UnlockShoutLevel::Action; action_pos++;
                        }

                        actions_to_register[action_pos] = Capabilities::GetInventory::Action; action_pos++;
                        actions_to_register[action_pos] = Capabilities::UseInventoryItem::Action; action_pos++;

                        if (MiscThings::escaped_helgen()) //refreshed automatically when we switch location
                        {
                            actions_to_register[action_pos] = Capabilities::ExploreWorld::Action; action_pos++;
                            actions_to_register[action_pos] = Capabilities::CallWaitMenu::Action; action_pos++;
                        }

                        if (MapProcessor::map_is_allowed()) //must be watched to refresh
                        {
                            actions_to_register[action_pos] = Capabilities::OpenMap::Action; action_pos++;
                        }

                        if (!MiscThings::is_interior_cell()) //refreshed automatically when we switch location
                        {
                            actions_to_register[action_pos] = Capabilities::GoToLocation::Action; action_pos++;
                        }

                    }

                    bool stop_here = false;
                }
                else
                {
                    if (MiscThings::is_objects_around_valid())
                    {
                        actions_to_register[action_pos] = Capabilities::LookAtObject::Action; action_pos++;
                        actions_to_register[action_pos] = Capabilities::GetObjectsAround::Action; action_pos++;
                    }

                }
            }
        }
    }
    else
    {
        if (reconnect)
            resend_active_force();
    }
    

    if (action_pos <= 0)
        return true;

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsRegister,
                                         .value = {.actions_register = {.actions = actions_to_register,
                                                                        .actions_len = action_pos}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }

    return true;
}








bool neuro::NeuroSocket::IsAlive()
{
    return neurosdk_context_connected(&m_context);
}

void neuro::NeuroSocket::LogNeuro(neurosdk_severity_e aSeverity, char* aMsg, void* aUserData)
{
    switch (aSeverity)
    {
    case NeuroSDK_Severity_Debug:
        // Ignore debug prints for now, create too much spam in console
        // Context::PluginLogger->DebugF(Context::PluginHandle, "[libneurosdk | Debug] %s", aMsg);
        break;
    case NeuroSDK_Severity_Info:
        //Context::PluginLogger->InfoF(Context::PluginHandle, "[libneurosdk | Info] %s", aMsg);
        break;
    case NeuroSDK_Severity_Warn:
        //Context::PluginLogger->WarnF(Context::PluginHandle, "[libneurosdk | Warn] %s", aMsg);
        break;
    case NeuroSDK_Severity_Error:
        //Context::PluginLogger->ErrorF(Context::PluginHandle, "[libneurosdk | Error] %s", aMsg);
        break;
    }

    if (aSeverity > NeuroSDK_Severity_Info)
    {
        // Just in case, spew in game console as well
        ; //Context::Spew("[libneurosdk | WarnOrError] {}", aMsg);
    }
}




bool neuro::NeuroSocket::Tick() //const neurosdk_message_action_t& aClosure)
{
    if (!IsAlive())
    {
        return false;
    }

    neurosdk_message_t* messageQueue{};
    int messageCount{};
    if (const auto status = neurosdk_context_poll(&m_context, &messageQueue, &messageCount);
        status != NeuroSDK_None)
    {
        return false;
    }

    for (auto i = 0; i < messageCount; i++)
    {
        if (messageQueue[i].kind == NeuroSDK_MessageKind_Action)
        {
            neurosdk_message_t action_result;
            action_result.kind = NeuroSDK_MessageKind_ActionResult;
            action_result.value.action_result.id = messageQueue[i].value.action.id;
            action_result.value.action_result.success = false;
            action_result.value.action_result.message = "error";

            std::string name = messageQueue[i].value.action.name;




            std::pair<bool, std::string> command_result{};

            bool failed_to_parse_json = false;
            bool dont_reset_force = false;




            RE::UI* ui = RE::UI::GetSingleton();

            if (ui->IsMenuOpen(RE::LoadingMenu::MENU_NAME) || ui->IsMenuOpen(RE::MainMenu::MENU_NAME))
            {
                command_result = { false, "You are not in game yet! Wait for the game to start. " };
            }
            else
            {

                neurosdk_action action_to_unregister{};


                if (name == Capabilities::SelectForceChoice::Name)
                {
                    action_to_unregister = Capabilities::SelectForceChoice::Action;

                    Impl::JSON::NeuroChoiceJson json{};

                    // operator bool overload for glz::error_ctx returns true on failure!
                    if (glz::read_json(json, messageQueue[i].value.action.data))
                        failed_to_parse_json = true;
                    else
                    {
                        switch (get_active_force())
                        {
                        case force_type::dialogue_line:
                            command_result = DialogueProcessor::say_chosen_line(json.id); break;

                        case force_type::barter_type_force:
                            command_result = BarterProcessor::set_barter_type(json.id); break;

                        case force_type::barter_category:
                            command_result = BarterProcessor::set_category_choice(json.id); break;

                        case force_type::barter_item:
                            command_result = BarterProcessor::set_item_choice(json.id); break;

                        case force_type::barter_quantity:
                            command_result = BarterProcessor::set_slider_choice(json.id); break;

                        case force_type::barter_vendor_not_enough_gold:
                            command_result = BarterProcessor::set_vendor_not_enough_gold_choice(json.id); break;

                        case force_type::lockpick_angle:
                            command_result = LockpickProcessor::set_angle_choice(json.id); break;

                        case force_type::levelup_attribute:
                            command_result = LevelupProcessor::levelup_attritube(json.id); break;

                        case force_type::perk_tree:
                            command_result = PerksProcessor::choose_skill_tree(json.id); break;

                        case force_type::perk_perk:
                        {
                            if (json.id == -2) //it sends another force right away, dont want to reset it
                                dont_reset_force = true;
                            command_result = PerksProcessor::choose_perk(json.id); break;
                        }

                        case force_type::perk_confirm:
                            command_result = PerksProcessor::choose_perk_confirm(json.id); break;

                        case force_type::map_undiscovered:
                            command_result = MapProcessor::set_undiscovered_choice(json.id); break;

                        case force_type::alchemy_amount_more:
                            command_result = AlchemyProcessor::choose_craft_more(json.id); break;

                        case force_type::enchant_type:
                            command_result = EnchantProcessor::set_enchant_type(json.id); break;

                        case force_type::enchant_item:
                            command_result = EnchantProcessor::set_item_choice(json.id); break;

                        case force_type::enchant_strength:
                            command_result = EnchantProcessor::set_slider_choice(json.id); break;

                        case force_type::smithing_category:
                            command_result = SmithingProcessor::set_category_choice(json.id); break;

                        case force_type::smithing_item:
                            command_result = SmithingProcessor::set_item_choice(json.id); break;

                        case force_type::map_location:
                            command_result = MapProcessor::set_location_choice(json.id); break;

                        case force_type::container_item:
                            command_result = ContainerProcessor::set_item_choice(json.id); break;

                        case force_type::gift_item:
                            command_result = GiftProcessor::set_item_choice(json.id); break;

                        case force_type::messagebox_option:
                            command_result = RandomMessageBoxProcessor::set_message_box_choice(json.id); break;

                        case force_type::threat_response:
                            command_result = Observer::set_threat_response_choice(json.id); break;

                        case force_type::closed_door_choice:
                            command_result = WalkerProcessor::set_closed_door_choice(json.id); break;

                        case force_type::race:
                            command_result = RaceProcessor::set_character_choice(json.id); break;

                        case force_type::sleepwait_time:
                            command_result = SleepWaitProcessor::set_sleepwait_choice(json.id); break;

                        case force_type::book:
                            command_result = BookProcessor::set_book_choice(json.id); break;

                        case force_type::training_choice:
                            command_result = TrainingProcessor::set_training_choice(json.id); break;

                        default:
                        {
                            command_result = { true, "You dont have any choices to make" };
                            register_allowed_actions();
                            break;
                        }
                            
                        }
                    }
                }

                if (name == Capabilities::SelectForceChoiceMultiple::Name)
                {
                    action_to_unregister = Capabilities::SelectForceChoiceMultiple::Action;

                    if (get_active_force() == force_type::alchemy_ingredients)
                    {
                        Impl::JSON::NeuroChoiceJson2 json2{};
                        Impl::JSON::NeuroChoiceJson3 json3{};

                        if (glz::read_json(json2, messageQueue[i].value.action.data))
                        {
                            if (glz::read_json(json3, messageQueue[i].value.action.data))
                                failed_to_parse_json = true;
                            else
                                command_result = AlchemyProcessor::choose_ingredients({ json3.id1, json3.id2, json3.id3 });
                        }
                        else
                        {
                            command_result = AlchemyProcessor::choose_ingredients({ json2.id1, json2.id2 });
                        }
                    }
                    else
                    {
                        command_result = { true, "You dont have any choices to make" };
                        register_allowed_actions();
                    }

                }

                if (name == Capabilities::SelectForceChoiceString::Name)
                {
                    action_to_unregister = Capabilities::SelectForceChoiceString::Action;

                    if (get_active_force() == force_type::character_name)
                    {
                        Impl::JSON::NeuroStringChoice json{};

                        if (glz::read_json(json, messageQueue[i].value.action.data))
                            failed_to_parse_json = true;
                        else
                            command_result = RaceProcessor::set_character_name(json.choice);
                    }
                    else
                    {
                        command_result = { false, "This command is not for talking! You will be notified when you are in dialogue. You can try starting a dialogue by interacting with someone (use walk_to_object, with target ID and action 1 (interact)). " };
                        register_allowed_actions();
                    }
                }

                if (command_result.first && !dont_reset_force) //if got positive result above - force has been cleared
                {
                    
                    const char* action_names[] = { action_to_unregister.name };

                    unregister_actions(action_names, std::size(action_names)); //lets try not caring about result... surely it will work 1st time?
                    set_active_force(-1);

                    if (command_result.second == "[Error]")
                        register_allowed_actions();
                }


                //////////////END OF FORCE RESPONSE


                if (WalkerProcessor::is_walking_important_path() && command_result.second == "")
                {
                    command_result.first = false;
                    command_result.second = "You are concentrated on walking right now. Wait until you reach the target. ";

                }
                else
                {
                    if (get_active_force() != -1 && command_result.second == "") //only if unhandled above
                    {
                        command_result.first = false;
                        command_result.second = "You must make a choice first!";
                    }
                    else
                    {
                        auto player = RE::PlayerCharacter::GetSingleton();
                        if (player->IsDead())
                        {
                            command_result.first = false;
                            command_result.second = "Your character is dead! Wait for game to load last save...";
                        }
                        else
                        {

                            if (name == Capabilities::ExploreWorld::Name)
                            {

                                command_result = WalkerProcessor::explore_world(false);

                            }



                            if (name == Capabilities::WalkToObject::Name)
                            {
                                Impl::JSON::NeuroChoiceJson json{};
                                Impl::JSON::NeuroChoiceJson2 json2{};

                                // operator bool overload for glz::error_ctx returns true on failure!
                                if (glz::read_json(json2, messageQueue[i].value.action.data))
                                {
                                    if (glz::read_json(json, messageQueue[i].value.action.data))
                                        failed_to_parse_json = true;
                                    else
                                        command_result = WalkerProcessor::walk_to_object_by_index(json.id, 0);
                                }
                                else
                                    command_result = WalkerProcessor::walk_to_object_by_index(json2.id1, json2.id2);
                            }

                            if (name == Capabilities::LookAtObject::Name)
                            {
                                Impl::JSON::NeuroChoiceJson json{};

                                // operator bool overload for glz::error_ctx returns true on failure!

                                if (glz::read_json(json, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                    command_result = WalkerProcessor::look_at_object_by_index(json.id);
                            }


                            if (name == Capabilities::FollowQuest::Name)
                            {
                                Impl::JSON::NeuroChoiceJson json{};

                                if (glz::read_json(json, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                    command_result = WalkerProcessor::walk_to_quest_by_index(json.id, false);
                            }



                            if (name == Capabilities::GoToLocation::Name)
                            {
                                Impl::JSON::NeuroChoiceJson json{};

                                if (glz::read_json(json, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                    command_result = WalkerProcessor::walk_to_location_by_index(json.id);
                            }





                            if (name == Capabilities::GetObjectsAround::Name)
                            {

                                //Impl::JSON::NeuroChoiceJson json{};

                                //if (glz::read_json(json, messageQueue[i].value.action.data))
                                //    failed_to_parse_json = true;
                                //else

                                command_result = MiscThings::GetObjectsAround(-1);

                            }


                            if (name == Capabilities::GetInventory::Name)
                            {
                                command_result = MiscThings::GetInventory();
                            }


                            if (name == Capabilities::GetSpells::Name)
                            {
                                command_result = MiscThings::get_available_spells();
                            }



                            if (name == Capabilities::CastEquipSpell::Name)
                            {
                                Impl::JSON::NeuroChoiceJson json{};

                                if (glz::read_json(json, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                    command_result = MiscThings::use_spell_by_index(json.id);
                            }


                            if (name == Capabilities::UnlockShoutLevel::Name)
                            {
                                Impl::JSON::NeuroChoiceJson json{};

                                if (glz::read_json(json, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                {
                                    command_result = MiscThings::unlock_shout_level(json.id);
                                    if (command_result.first && !MiscThings::player_has_shouts_to_unlock())
                                    {
                                        const char* action_names[] = { Capabilities::UnlockShoutLevel::Name };
                                        unregister_actions(action_names, std::size(action_names));
                                    }
                                }
                                    
                            }


                            if (name == Capabilities::OpenMap::Name)
                            {
                                command_result = MapProcessor::open_menu();
                            }


                            if (name == Capabilities::GetCurrentQuests::Name)
                            {
                                command_result = MiscThings::get_current_quests();
                            }


                            if (name == Capabilities::GetLocations::Name)
                            {
                                command_result = MiscThings::get_locations_around();
                            }


                            if (name == Capabilities::UseInventoryItem::Name)
                            {
                                Impl::JSON::NeuroChoiceJson2 json2{};

                                if (glz::read_json(json2, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                    command_result = MiscThings::activate_inventory_object_by_index(json2.id1, json2.id2);

                            }

                            if (name == Capabilities::GetGold::Name)
                            {
                                command_result = MiscThings::GetGold();
                            }


                            if (name == Capabilities::CallWaitMenu::Name)
                            {
                                command_result = SleepWaitProcessor::call_wait_menu();
                            }
                        }
                    }
                }
            }


            //...//

            if (failed_to_parse_json)
            {
                RespondToAction(action_result.value.action_result.id,
                    "Failed to parse action data JSON.",
                    false
                );
            }
            else
            {
                action_result.value.action_result.success = command_result.first;
                action_result.value.action_result.message = command_result.second.c_str();


                RespondToAction(action_result.value.action_result.id,
                    action_result.value.action_result.message,
                    action_result.value.action_result.success
                );
            }



            neurosdk_message_destroy(&messageQueue[i]);
        }
    }

    return true;
}