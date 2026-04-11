//#include <Context/Context.hpp>


#include "Socket.hpp"
#include <neurosdk.h>
#include <string>
#include <string_view>

//using namespace Red;


extern bool is_in_game();
extern bool resend_active_force();



bool had_connection = false;
bool ever_registered = false;
bool something_is_registered = false;

std::string greet_phrase = "You are playing Skyrim, an action RPG. Use commands to interact with the world. Try to have a fun adventure. ";

long long timestamp_greet_sent = 0;


std::map<std::string, bool> actions_status{};


void set_action_status(std::string action_name, bool status)
{
    auto action_status = actions_status.find(action_name);

    if (action_status != actions_status.end())
        action_status->second = status;
    else
    {
        actions_status.insert({ action_name, status });
    }
}


bool neuro::get_action_status(std::string action_name)
{
    auto action_status = actions_status.find(action_name);
    if (action_status != actions_status.end())
        return action_status->second;

    return false;
}


bool neuro::is_something_registered()
{
    return something_is_registered;
}



void clear_actions_status()
{
    actions_status.clear();
}



neurosdk_action ActionsList[] = {
                                    Capabilities::WalkToObjectDoNothing::Action,
                                    Capabilities::WalkToObject::Action,
                                    Capabilities::LookAtObject::Action,
                                    Capabilities::AttackObject::Action,
                                    Capabilities::PickpocketObject::Action,

                                    Capabilities::ExploreWorld::Action,
                                    Capabilities::ExitDungeon::Action,
                                    Capabilities::SurrenderToGuards::Action,
                                    Capabilities::EscapePrison::Action,
                                    Capabilities::InterestingPlaces::Action,


                                    Capabilities::GetCurrentQuests::Action,
                                    Capabilities::FollowQuest::Action,
                                    Capabilities::ChangeQuest::Action,


                                    Capabilities::GetLocations::Action,
                                    

                                    Capabilities::GetSpells::Action,
                                    Capabilities::CastSpell::Action,
                                    Capabilities::EquipSpell::Action,
                                    Capabilities::UnlockShoutLevel::Action, //TODO: probably calculate ourselves and force just like levelup

                                    Capabilities::GetInventory::Action,
                                    Capabilities::UseInventoryItem::Action,
                                    Capabilities::DropInventoryItem::Action,
                                    Capabilities::CallWaitMenu::Action,
                                    Capabilities::OpenMap::Action,
                                    Capabilities::GetGold::Action,
                                    Capabilities::Spin::Action,

                                    Capabilities::SelectForceChoice::Action,
                                    Capabilities::SelectForceChoiceMultiple::Action,
                                    Capabilities::SelectForceChoiceString::Action,
                                    Capabilities::SelectForceChoiceArray::Action,
                                    


                                    Capabilities::GetObjectsAround::Action, //idk about this one
                                    Capabilities::GoToLocation::Action
                                };

neurosdk_action ActionsListNoForces[] = {

                                    Capabilities::WalkToObjectDoNothing::Action,
                                    Capabilities::WalkToObject::Action,
                                    Capabilities::LookAtObject::Action,
                                    Capabilities::AttackObject::Action,
                                    Capabilities::PickpocketObject::Action,
                                    Capabilities::ExploreWorld::Action,
                                    Capabilities::ExitDungeon::Action,
                                    Capabilities::SurrenderToGuards::Action,
                                    Capabilities::EscapePrison::Action,
                                    Capabilities::InterestingPlaces::Action,

                                    Capabilities::GetCurrentQuests::Action,
                                    Capabilities::FollowQuest::Action,
                                    Capabilities::ChangeQuest::Action,

                                    Capabilities::GetLocations::Action,


                                    Capabilities::GetSpells::Action,
                                    Capabilities::CastSpell::Action,
                                    Capabilities::EquipSpell::Action,
                                    Capabilities::UnlockShoutLevel::Action, //TODO: probably calculate ourselves and force just like levelup

                                    Capabilities::GetInventory::Action,
                                    Capabilities::UseInventoryItem::Action,
                                    Capabilities::DropInventoryItem::Action,
                                    Capabilities::CallWaitMenu::Action,
                                    Capabilities::OpenMap::Action,
                                    Capabilities::GetGold::Action,
                                    Capabilities::Spin::Action,

                                    Capabilities::GetObjectsAround::Action, //idk about this one
                                    Capabilities::GoToLocation::Action
};



neurosdk_action ActionsListNoForces2[] = { //these are for moments when we cant look (mining ore, sitting on chair, etc)

                                    //Capabilities::WalkToObject::Action,
                                    //Capabilities::LookAtObject::Action,
                                    //Capabilities::ExploreWorld::Action,

                                    //Capabilities::GetCurrentQuests::Action,
                                    //Capabilities::FollowQuest::Action,

                                    //Capabilities::GetLocations::Action,


                                    //Capabilities::GetSpells::Action,
                                    Capabilities::CastSpell::Action,
                                    Capabilities::EquipSpell::Action,
                                    Capabilities::Spin::Action,
                                   // Capabilities::UnlockShoutLevel::Action, //TODO: probably calculate ourselves and force just like levelup

                                    //Capabilities::GetInventory::Action,
                                    Capabilities::UseInventoryItem::Action,
                                    Capabilities::DropInventoryItem::Action,
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
    StringView aPriority, bool ephemeral)
{
    const char* tempActionNames[] = { aActionName.Data() };

    neurosdk_message_t msg{ .kind = NeuroSDK_MessageKind_ActionsForce,
                           .value = {.actions_force = {.state = aState.Data(),
                                                       .query = aQuery.Data(),
                                                       .priority = aPriority.Data(),
                                                       .ephemeral_context = ephemeral,//.ephemeral_context = false,
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


    //std::string in_game_text = " Use commands to interact with the world. ";

    //if (!is_in_game())
    //    ;// in_game_text = "You are not in game. Wait for game to start. ";


    had_connection = true;


    auto now = std::chrono::steady_clock::now().time_since_epoch().count();

    timestamp_greet_sent = now;

    

    //return SendContext(("You are playing Skyrim, an action RPG. " + in_game_text + "Try to have a fun adventure. ").c_str(),
    return SendContext(greet_phrase.c_str(),
        true) &&
        IsAlive();
}



bool neuro::NeuroSocket::SendGreeting()
{
    if (!IsAlive())
    {
        something_is_registered = false;
        ever_registered = false;
        had_connection = false;
        return false;
    }


    auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    timestamp_greet_sent = now;

    return SendContext(greet_phrase.c_str(),
        true) &&
        IsAlive();
}



bool neuro::NeuroSocket::register_actions(neurosdk_action actions[], int size)
{
    if (!IsAlive())
    {
        something_is_registered = false;
        ever_registered = false;
        had_connection = false;
        return false;
    }
        
    if (size <= 0)
        return true;

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsRegister,
                                         .value = {.actions_register = {.actions = actions,
                                                                        .actions_len = size}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }

    std::string action_name = actions[0].name;
    std::string force_name_1 = Capabilities::SelectForceChoice::Name;
    std::string force_name_2 = Capabilities::SelectForceChoiceMultiple::Name;
    std::string force_name_3 = Capabilities::SelectForceChoiceString::Name;
    std::string force_name_4 = Capabilities::SelectForceChoiceArray::Name;

    if (!(size == 1 && (action_name == force_name_1 || action_name == force_name_2 || action_name == force_name_3 || action_name == force_name_4))) //exclude forces
        something_is_registered = true;


    for (int i = 0; i < size; i++)
    {
        set_action_status(actions[i].name, true);
    }



    return true;
}



bool neuro::NeuroSocket::unregister_actions(const char** action_names, int size)
{
    if (!IsAlive())
    {
        something_is_registered = false;
        ever_registered = false;
        had_connection = false;
        return false;
    }

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsUnregister,
                                         .value = {.actions_unregister = {.action_names = action_names,
                                                                        .action_names_len = size}}};

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }


    for (int i = 0; i < size; i++)
    {
        set_action_status(action_names[i], false);
    }


    return true;
}


bool neuro::NeuroSocket::unregister_all()
{
    if (!IsAlive())
    {
        something_is_registered = false;
        ever_registered = false;
        had_connection = false;
        return false;
    }


    const char* action_names[ActionsCountNoForces];

    for (int i = 0; i < ActionsCountNoForces; i++)
    {
        action_names[i] = ActionsListNoForces[i].name;
    }

    if (ActionsCountNoForces <= 0)
    {
        for (int i = 0; i < ActionsCountNoForces; i++)
        {
            set_action_status(action_names[i], false);
        }

        return true;
    }
        

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsUnregister,
                                         .value = {.actions_unregister = {.action_names = action_names,
                                                                        .action_names_len = ActionsCountNoForces}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }

    something_is_registered = false;


    for (int i = 0; i < ActionsCountNoForces; i++)
    {
        set_action_status(action_names[i], false);
    }

    return true;
}


bool neuro::NeuroSocket::unregister_all2()
{
    if (!IsAlive())
    {
        something_is_registered = false;
        ever_registered = false;
        had_connection = false;
        return false;
    }


    const char* action_names[ActionsCountNoForces2];

    for (int i = 0; i < ActionsCountNoForces2; i++)
    {
        action_names[i] = ActionsListNoForces2[i].name;
    }

    if (ActionsCountNoForces2 <= 0)
    {
        for (int i = 0; i < ActionsCountNoForces2; i++)
        {
            set_action_status(action_names[i], false);
        }
        return true;
    }
        

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsUnregister,
                                         .value = {.actions_unregister = {.action_names = action_names,
                                                                        .action_names_len = ActionsCountNoForces2}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }

    for (int i = 0; i < ActionsCountNoForces2; i++)
    {
        set_action_status(action_names[i], false);
    }

    return true;
}




std::vector<std::string> delayed_socket_messages{};

void neuro::add_message_to_delayed_queue(std::string message)
{
    delayed_socket_messages.push_back(message);
}

neurosdk_action actions_to_register[ActionsCount]{};

bool neuro::NeuroSocket::register_allowed_actions(bool reconnect)
{
    if (MiscThings::is_in_main_menu())
        return false;

    if (!IsAlive())
    {
        something_is_registered = false;
        ever_registered = false;
        had_connection = false;
        return false;
    }


    auto player = RE::PlayerCharacter::GetSingleton();
    auto control_map = RE::ControlMap::GetSingleton();
    bool can_walk = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kMovement);
    bool can_look = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kLooking) || (player ? player->IsInRagdollState() : false);
    bool can_interact = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kActivate);
    bool can_fight = control_map->enabledControls.any(RE::UserEvents::USER_EVENT_FLAG::kFighting);


    int action_pos = 0;


    auto threshold_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("MQ101");
    int unbound_quest_stage = threshold_quest->GetCurrentStageID();

    int active_force = get_active_force();



    if (active_force == -1)
    {
        if (!reconnect || !MiscThings::have_force_only_menu_open()) //???
        {
            if (unbound_quest_stage >= 15)
            {
                if (!MiscThings::is_intro()) //must be watched to refresh
                {

                    //if (MiscThings::is_objects_around_valid())
                    //{
                        if (MiscThings::is_intro2())
                        {
                            actions_to_register[action_pos] = Capabilities::WalkToObjectDoNothing::Action; action_pos++;
                        }
                        else
                        {
                            actions_to_register[action_pos] = Capabilities::WalkToObject::Action; action_pos++;
                            if (MiscThings::is_in_settlement())
                            {
                                actions_to_register[action_pos] = Capabilities::InterestingPlaces::Action; action_pos++;
                            }
                        }
                        
                        actions_to_register[action_pos] = Capabilities::GetObjectsAround::Action; action_pos++;
                        if (Observer::can_surrender_to_guards())
                        {
                            actions_to_register[action_pos] = Capabilities::SurrenderToGuards::Action; action_pos++;
                        }
                            
                    //}

                    if (MiscThings::have_any_quests() && !MiscThings::is_serving_jail())
                    {
                        actions_to_register[action_pos] = Capabilities::FollowQuest::Action; action_pos++;
                        actions_to_register[action_pos] = Capabilities::GetCurrentQuests::Action; action_pos++;
                        actions_to_register[action_pos] = Capabilities::ChangeQuest::Action; action_pos++;
                    }


                    if (!MiscThings::is_intro2()) //must be watched to refresh
                    {
                        if (can_fight)
                        {
                            actions_to_register[action_pos] = Capabilities::AttackObject::Action; action_pos++;
                            actions_to_register[action_pos] = Capabilities::PickpocketObject::Action; action_pos++;
                            actions_to_register[action_pos] = Capabilities::GetSpells::Action; action_pos++;
                            actions_to_register[action_pos] = Capabilities::CastSpell::Action; action_pos++;
                            actions_to_register[action_pos] = Capabilities::EquipSpell::Action; action_pos++;
                        }

                        actions_to_register[action_pos] = Capabilities::Spin::Action; action_pos++;


                        if (MiscThings::player_has_shouts_to_unlock()) //must be watched to refresh
                        {
                            actions_to_register[action_pos] = Capabilities::UnlockShoutLevel::Action; action_pos++;
                        }

                        actions_to_register[action_pos] = Capabilities::GetInventory::Action; action_pos++;
                        actions_to_register[action_pos] = Capabilities::UseInventoryItem::Action; action_pos++;
                        actions_to_register[action_pos] = Capabilities::DropInventoryItem::Action; action_pos++;


                        if (MiscThings::escaped_helgen()) //refreshed automatically when we switch location
                        {
                            if (!was_exit_dungeon_registered())
                            {
                                actions_to_register[action_pos] = Capabilities::ExploreWorld::Action; action_pos++;
                            }

                            if (!MiscThings::is_serving_jail())
                            {
                                actions_to_register[action_pos] = Capabilities::CallWaitMenu::Action; action_pos++;
                            }
                                
                        }

                        if (MapProcessor::map_is_allowed() && can_fight) //must be watched to refresh
                        {
                            actions_to_register[action_pos] = Capabilities::OpenMap::Action; action_pos++;
                        }


                        auto player_worldspace = player->GetWorldspace();
                        auto tamriel_worldspace = RE::TESForm::LookupByID(0x3c);

                        if (!MiscThings::is_interior_cell() && can_fight && player_worldspace == tamriel_worldspace) //refreshed automatically when we switch location
                        {
                            actions_to_register[action_pos] = Capabilities::GoToLocation::Action; action_pos++;
                            if (!MiscThings::have_any_quests())
                            {
                                actions_to_register[action_pos] = Capabilities::GetLocations::Action; action_pos++;
                            }
                                
                        }
                        else
                        {
                            if (was_exit_dungeon_registered())
                            {
                                actions_to_register[action_pos] = Capabilities::ExitDungeon::Action; action_pos++;
                            }
                                
                        }

                        if (MiscThings::player_escaping_jail())
                        {
                            actions_to_register[action_pos] = Capabilities::EscapePrison::Action; action_pos++;
                        }

                    }

                    bool stop_here = false;
                }
                else
                {
                    if (MiscThings::is_objects_around_valid())
                    {
                        actions_to_register[action_pos] = Capabilities::LookAtObject::Action; action_pos++;
                        //actions_to_register[action_pos] = Capabilities::GetObjectsAround::Action; action_pos++;
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

    ever_registered = true;
    something_is_registered = true;







    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsRegister,
                                         .value = {.actions_register = {.actions = actions_to_register,
                                                                        .actions_len = action_pos}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        return false;
    }



    for (auto action_to_register : actions_to_register)
    {
        if (action_to_register.name)
            set_action_status(action_to_register.name, true);
    }




    std::string delayed_context = "";

    std::string last_message = "";

    for (auto delayed_message : delayed_socket_messages)
    {
        if (delayed_message == last_message)
            continue;

        last_message = delayed_message;

        if (delayed_context != "")
            delayed_context += "; ";

        

        delayed_context += delayed_message;
    }

    if (delayed_context != "")
    {
        delayed_socket_messages.clear();
        send_random_context(delayed_context, false);
    }
        



    return true;
}



float action_watchdog_timer = 0.0f;

bool neuro::NeuroSocket::action_register_watchdog(float dtime)
{
    if (had_connection && ever_registered && !something_is_registered && (get_active_force() == -1) && !MiscThings::have_force_only_menu_open() && !MiscThings::is_intro())
    {
        if (action_watchdog_timer > 20.0f)
        {
            if (MiscThings::is_game_paused())
            {
                MiscThings::unpause_game();
            }

            action_watchdog_timer = 0.0f;
            register_allowed_actions();
            return true;
        }
        else
            action_watchdog_timer += dtime;
    }
    else
        action_watchdog_timer = 0.0f;


    return false;
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





float time_no_commands = 0.0f;
float time_no_menus = 0.0f;


int afk_threshold = 0;
float time_threshold = 7.77f;


float delayed_poke_time = 0.0f;
bool make_delayed_poke = false;


void neuro::reset_poke()
{
    time_no_commands = 0.0f;
    time_no_menus = 0.0f;
    delayed_poke_time = 0.0f;
    make_delayed_poke = false;
    WalkerProcessor::reset_inactive_timer();
}


bool neuro::NeuroSocket::Tick(float dtime) //const neurosdk_message_action_t& aClosure)
{
    if (!IsAlive())
    {
        something_is_registered = false;
        ever_registered = false;
        had_connection = false;
        return false;
    }



    auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    float delta_greet = (double)(now - timestamp_greet_sent) / 1000000000.0;

    if (delta_greet > 600.0f)
    {
        SendGreeting();
    }

    time_no_commands += dtime;
    float time_walker_inactive = WalkerProcessor::get_walker_inactive_time();
    bool have_any_menus_open = MiscThings::have_force_only_menu_open();

    if (!have_any_menus_open && something_is_registered)
        time_no_menus += dtime;
    else
        time_no_menus = 0.0f;
    
    //float time_threshold = afk_threshold * 5.0f + 10.0f;

    
    if (have_any_menus_open)
    {
        make_delayed_poke = false;
        delayed_poke_time = 0.0f;
    }



    if (make_delayed_poke)
    {
        if (delayed_poke_time > 1.0f)
        {
            if (!have_any_menus_open && something_is_registered)
            {
                send_random_context("[Choose next action to do]", false);// . " + advice + "]", false);
                delayed_poke_time = 0.0f;
                make_delayed_poke = false;

                WalkerProcessor::reset_inactive_timer();
                time_no_commands = 0.0f;
                time_no_menus = 0.0f;
            }
            else
            {
                delayed_poke_time = 0.0f;
                make_delayed_poke = false;
            }

        }
        else
            delayed_poke_time += dtime;
    }

    if (time_no_commands > time_threshold && time_walker_inactive > time_threshold && time_no_menus > time_threshold)
    {
        afk_threshold++;

        time_no_menus = 0.0f;
        time_no_commands = 0.0f;
        WalkerProcessor::reset_inactive_timer();

        std::string advice = "";
        if (MiscThings::is_objects_around_valid())
            advice = "walk somewhere, interact with something";

        if (MiscThings::have_any_quests())
            if (advice != "")
                advice += ", follow some quest";
            else
                advice = "follow some quest";
        else
        {
            if (!MiscThings::is_interior_cell())
            if (advice != "")
                advice += ", explore, go to location or use map to travel";
            else
                advice = "explore, go to location or use map to travel";
        }


        if (advice != "")
            advice = "You can " + advice;

        send_random_context("[Choose next action to do]", false);// . " + advice + "]", false);

        time_threshold = (float)std::rand() / RAND_MAX * 4 + 6;
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
        time_no_commands = 0.0f;
        afk_threshold = 0.0f;

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
                            //if (json.id == -2) //BAD it sends another force right away, dont want to reset it
                             //   dont_reset_force = true;
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

                        case force_type::container_quantity:
                            command_result = ContainerProcessor::set_slider_choice(json.id); break;

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

                        case force_type::timed_quest_puzzle:
                            command_result = Observer::set_quest_puzzle_choice(json.id); break;

                        case force_type::confirm_pillar:
                            command_result = WalkerProcessor::set_ruin_pillar_choice(json.id); break;

                        case force_type::confirm_change_quest_course:
                            command_result = WalkerProcessor::set_change_quest_course_choice(json.id); break;

                        case force_type::confirm_stealing:
                            command_result = WalkerProcessor::set_stealing_choice(json.id); break;

                        case force_type::specify_quest_path:
                            command_result = WalkerProcessor::set_multiple_path_quest_choice(json.id); break;

                        case force_type::confirm_attack_friend:
                            command_result = WalkerProcessor::set_attack_friend_choice(json.id); break;

                        case force_type::visit_places:
                            command_result = MiscThings::visit_interesting_place_by_index(json.id); break;


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

                
                if (name == Capabilities::SelectForceChoiceArray::Name)
                {
                    action_to_unregister = Capabilities::SelectForceChoiceArray::Action;

                    Impl::JSON::NeuroChoiceJsonArrayInts json{};

                    if (glz::read_json(json, messageQueue[i].value.action.data))
                    {
                        command_result.first = false;
                        command_result.second = "Couldn't find any valid IDs in provided array";
                    }
                    else
                    {
                        switch (get_active_force())
                        {


                        case (force_type::container_item_array):
                            command_result = ContainerProcessor::set_item_choice_array(json.ids_array); break;

                        case (force_type::barter_item_array):
                            command_result = BarterProcessor::set_item_choice_array(json.ids_array); break;

                        default:
                        {
                            command_result = { true, "You dont have any choices to make" };
                            register_allowed_actions();
                            break;
                        }



                        }

                    }


                }


                

                                


                                








                if (command_result.first)// && !dont_reset_force) //if got positive result above - force has been cleared
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



                            if (name == Capabilities::SurrenderToGuards::Name)
                            {
                                command_result = WalkerProcessor::surrender_to_guards();
                            }


                            if (name == Capabilities::Spin::Name)
                            {
                                Impl::JSON::NeuroChoiceJson2 json2{};

                                // operator bool overload for glz::error_ctx returns true on failure!
                                if (glz::read_json(json2, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                    command_result = WalkerProcessor::make_spins(json2.id1, json2.id2);
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



                            if (name == Capabilities::GetObjectsAround::Name)
                            {

                                //Impl::JSON::NeuroChoiceJson json{};

                                //if (glz::read_json(json, messageQueue[i].value.action.data))
                                //    failed_to_parse_json = true;
                                //else

                                command_result = MiscThings::GetObjectsAround(-1);

                                make_delayed_poke = true;

                            }


                            if (name == Capabilities::GetInventory::Name)
                            {
                                command_result = MiscThings::GetInventory();

                                make_delayed_poke = true;

                            }

                            if (name == Capabilities::UseInventoryItem::Name)
                            {
                                Impl::JSON::NeuroChoiceJsonArrayInts json{};

                                if (glz::read_json(json, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                    command_result = MiscThings::activate_array_of_inventory_objects(json.ids_array);

                                make_delayed_poke = true;

                            }

                            if (name == Capabilities::DropInventoryItem::Name)
                            {
                                

                                Impl::JSON::NeuroChoiceJsonArrayInts json{};

                                if (glz::read_json(json, messageQueue[i].value.action.data))
                                {
                                    failed_to_parse_json = true;
                                    //command_result.first = false;
                                    //command_result.second = "Couldn't find any valid IDs in provided array";
                                }
                                else
                                {
                                    command_result = MiscThings::drop_array_of_inventory_objects(json.ids_array);
                                }

                                make_delayed_poke = true;
                            }


                            if (name == Capabilities::GetSpells::Name)
                            {
                                command_result = MiscThings::get_available_spells();

                                make_delayed_poke = true;
                            }


                            if (name == Capabilities::CastSpell::Name)
                            {
                                Impl::JSON::NeuroChoiceJson json{};

                                if (glz::read_json(json, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                    command_result = MiscThings::cast_spell_by_index(json.id);
                            }


                            if (name == Capabilities::EquipSpell::Name)
                            {
                                Impl::JSON::NeuroChoiceJson json{};

                                if (glz::read_json(json, messageQueue[i].value.action.data))
                                    failed_to_parse_json = true;
                                else
                                    command_result = MiscThings::equip_spell_by_index(json.id);
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

                                    make_delayed_poke = true;
                                }


                            }

                            if (name == Capabilities::GetCurrentQuests::Name)
                            {
                                command_result = MiscThings::get_current_quests();

                                make_delayed_poke = true;
                            }


                            if (name == Capabilities::GetLocations::Name)
                            {
                                command_result = MiscThings::get_locations_around();

                                make_delayed_poke = true;
                            }




                            if (name == Capabilities::GetGold::Name)
                            {
                                command_result = MiscThings::GetGold();

                                make_delayed_poke = true;
                            }


                            if (name == Capabilities::CallWaitMenu::Name)
                            {
                                command_result = SleepWaitProcessor::call_wait_menu();
                            }






                            int overweight = MiscThings::player_overencumbered_by();

                            if (command_result.second == "" && overweight)
                            {
                                command_result.first = false;
                                command_result.second = "Cannot do that while overencumbered. Drop something from the inventory first. Excess weight: " + std::to_string(overweight) + ". Make sure to drop enough weight";
                            }
                            else
                            {
                                if (name == Capabilities::ChangeQuest::Name)
                                {
                                    Impl::JSON::NeuroChoiceJson json{};

                                    if (glz::read_json(json, messageQueue[i].value.action.data))
                                        failed_to_parse_json = true;
                                    else
                                        command_result = WalkerProcessor::walk_to_quest_by_index(json.id, false);
                                }

                                if (name == Capabilities::FollowQuest::Name)
                                {
                                   command_result = WalkerProcessor::walk_to_current_quest();
                                }



                                if (name == Capabilities::EscapePrison::Name)
                                {
                                    command_result = WalkerProcessor::escape_prison();
                                }


                                if (name == Capabilities::ExploreWorld::Name)
                                {

                                    command_result = WalkerProcessor::explore_world(false);

                                }

                                if (name == Capabilities::ExitDungeon::Name)
                                {

                                    command_result = WalkerProcessor::exit_dungeon();

                                }

                                if (name == Capabilities::WalkToObject::Name)
                                {
                                    Impl::JSON::NeuroChoiceJson json{};
                                    if (glz::read_json(json, messageQueue[i].value.action.data))
                                        failed_to_parse_json = true;
                                    else
                                        command_result = WalkerProcessor::walk_to_object_by_index(json.id, 1);
                                }


                                if (name == Capabilities::WalkToObjectDoNothing::Name)
                                {
                                    Impl::JSON::NeuroChoiceJson json{};
                                    if (glz::read_json(json, messageQueue[i].value.action.data))
                                        failed_to_parse_json = true;
                                    else
                                        command_result = WalkerProcessor::walk_to_object_by_index(json.id, 0);
                                }


                                if (name == Capabilities::AttackObject::Name)
                                {
                                    Impl::JSON::NeuroChoiceJson json{};

                                    if (glz::read_json(json, messageQueue[i].value.action.data))
                                        failed_to_parse_json = true;
                                    else
                                        command_result = WalkerProcessor::walk_to_object_by_index(json.id, 3);
                                }


                                if (name == Capabilities::PickpocketObject::Name)
                                {
                                    Impl::JSON::NeuroChoiceJson json{};

                                    if (glz::read_json(json, messageQueue[i].value.action.data))
                                        failed_to_parse_json = true;
                                    else
                                        command_result = WalkerProcessor::walk_to_object_by_index(json.id, 2);
                                }


                                if (name == Capabilities::GoToLocation::Name)
                                {
                                    Impl::JSON::NeuroChoiceJson json{};

                                    if (glz::read_json(json, messageQueue[i].value.action.data))
                                        failed_to_parse_json = true;
                                    else
                                        command_result = WalkerProcessor::walk_to_location_by_index(json.id);
                                }


                                if (name == Capabilities::OpenMap::Name)
                                {
                                    command_result = MapProcessor::open_menu();
                                }


                                if (name == Capabilities::InterestingPlaces::Name)
                                {
                                    command_result = MiscThings::check_interesting_places();
                                }

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
                command_result.second = MiscThings::fix_book_description(command_result.second); //clear all <> parts


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