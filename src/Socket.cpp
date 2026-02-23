//#include <Context/Context.hpp>


#include "Socket.hpp"
#include <neurosdk.h>
#include <string>
#include <string_view>

//using namespace Red;









neurosdk_action ActionsList[] = {  
                                    
                                    Capabilities::GetObjectsAround::Action,
                                    Capabilities::WalkToObject::Action,

                                    Capabilities::GetCurrentQuests::Action,
                                    Capabilities::FollowQuest::Action,
                                    
                                    Capabilities::GetLocations::Action,
                                    Capabilities::GoToLocation::Action,

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

    neurosdk_message_t capabilityMessage{ .kind = NeuroSDK_MessageKind_ActionsRegister,
                                         .value = {.actions_register = {.actions = ActionsList,
                                                                        .actions_len = ActionsCount}} };

    if (const auto status = neurosdk_context_send(&m_context, &capabilityMessage); status != NeuroSDK_None)
    {
        //Context::PluginLogger->ErrorF(Context::PluginHandle, "Failed to inform backend of capabilities! Error %s",
        //    neurosdk_error_string(status));

        return false;
    }

    return SendContext("You are playing Skyrim, an action RPG. Commands will only give reasonable output once "
        "you are ingame.",
        true) &&
        IsAlive();
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
