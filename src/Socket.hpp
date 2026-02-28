#pragma once

#include <neurosdk.h>
#include "main.hpp"
#include "Misc.hpp"
#include "DialogueProcessor.hpp"
#include "BarterProcessor.hpp"
#include "LockpickProcessor.hpp"
#include "LevelupProcessor.hpp"
#include "PerksProcessor.hpp"
#include "AlchemyProcessor.hpp"
#include "EnchantProcessor.hpp"
#include "SmithingProcessor.hpp"
#include "MapProcessor.hpp"
#include "ContainerProcessor.hpp"
#include "RandomMessageBoxProcessor.hpp"
#include "RaceProcessor.hpp"
#include "SleepWaitProcessor.hpp"
#include "BookProcessor.hpp"
#include "TrainingProcessor.hpp"

#include "GiftProcessor.hpp"

#include "WalkerProcessor.hpp"
#include "Observer.hpp"


//#include <RED4ext/RED4ext.hpp>
//#include <RedLib.hpp>
//#include <RED4ext/StringView.hpp>


//TODO: must deal with json.
// set up glaze or find alternative.


#include <glaze/glaze.hpp>




namespace Capabilities
{

    namespace SelectForceChoice
    {
        constexpr char Name[] = "select_choice";
        constexpr char Desc[] =
            R"(Select an option. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "Can only be used when you are asked to choose something. The ID of the selected option.", "type": "integer" } }, "required": ["id"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace SelectForceChoiceMultiple
    {
        constexpr char Name[] = "select_choice_multiple";
        constexpr char Desc[] =
            R"(Select multiple options. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id1": { "description": "Can only be used when you are asked to choose something. The ID of the selected option.", "type": "integer" }, "id2": { "description": "The ID of the selected option.", "type": "integer" }, "id3": { "description": "The ID of the selected option.", "type": "integer" } }, "required": ["id1", "id2"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace SelectForceChoiceString
    {
        constexpr char Name[] = "character_creation_choose_name";
        constexpr char Desc[] =
            R"(Give a choice in string format. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "choice": { "description": "Can only be used when asked while creating your character. Choose your character's name", "type": "string" } }, "required": ["choice"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    /////////////////////////////////////////////////////

    /*
    namespace DoorIsClosedChoice
    {
        constexpr char Name[] = "choose_what_to_do_with_closed_door";
        constexpr char Desc[] =
            R"(A closed door is blocking the path. It can be lockpicked. This command sets the choice. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "0 to cancel walking, 1 to try lockpicking", "type": "integer" } }, "required": ["id"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } 

    namespace SelectMessageBoxOption
    {
        constexpr char Name[] = "select_messsage_box_option";
        constexpr char Desc[] =
            R"(Select one of provided options)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the selected option from the provided options. ", "type": "integer" } }, "required": ["id"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption
    */

    namespace WalkToObject
    {
        constexpr char Name[] = "walk_to_object_and_interact";
        constexpr char Desc[] =
            R"(Walk to object specified by its ID and do action ID)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id1": { "description": "The ID of the object. ", "type": "integer" }, "id2": { "description": "The ID of the action to do after walk. 0 - do nothing (walk up and stare at), 1 - interact, 2 - pickpocket (works only on humanoids), 3 - attack ", "type": "integer" } }, "required": ["id1", "id2"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }


    namespace GetObjectsAround
    {
        constexpr char Name[] = "get_objects_around";
        constexpr char Desc[] =
            R"(Get a list of objects around you)";
        constexpr char JsonSchema[] = "{}"; //R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of object type to get. If not specified, all objects will be given. ", "type": "integer" } }, "required": [] })";


        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption



    namespace UseInventoryItem
    {
        constexpr char Name[] = "use_inventory_item";
        constexpr char Desc[] =
            R"(Use inventory item. Requires item ID and action ID. )";
        constexpr char JsonSchema[] = 
            R"({ "additionalProperties": false, "type": "object", "properties": { "id1": { "description": "The ID of the object to use. Use get_inventory to get list of object IDs. ", "type": "integer" }, "id2": { "description": "The ID of the action to do on item. Available actions: 1 - equip/use, 2 - drop", "type": "integer" } }, "required": ["id1", "id2"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    //CastEquipSpell

    namespace CastEquipSpell
    {
        constexpr char Name[] = "cast_or_equip_spell";
        constexpr char Desc[] =
            R"(Cast or equip spell. Requires spell ID. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the spell. Use get_available_spells to get list of spell IDs. ", "type": "integer" } }, "required": ["id"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption

    namespace UnlockShoutLevel
    {
        constexpr char Name[] = "unlock_shout_level";
        constexpr char Desc[] =
            R"(Unlock new level of a shout. The shout must have known but not unlocked words and you must have at least one dragon soul. Requires shoud ID)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the shout to unlock. Use get_available_spells to get list of shout IDs. ", "type": "integer" } }, "required": ["id"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption



    namespace GetCurrentQuests
    {
        constexpr char Name[] = "get_current_quests";
        constexpr char Desc[] =
            R"(Get list of current quests)";
        constexpr char JsonSchema[] = "{}";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace FollowQuest
    {
        constexpr char Name[] = "follow_quest";
        constexpr char Desc[] =
            R"(Walk towards the quest objective. Requires quest ID. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the quest to walk to. Use get_current_quests to get list of available quests. ", "type": "integer" } }, "required": ["id"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace GoToLocation
    {
        constexpr char Name[] = "go_to_location";
        constexpr char Desc[] =
            R"(Walk towards the location. Requires location ID. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the location to walk to. You will get available location ID's from context. ", "type": "integer" } }, "required": ["id"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


   
    namespace OpenMap
    {
        constexpr char Name[] = "open_map";
        constexpr char Desc[] =
            R"(Open the map. Can be used to fast-travel or place markers. )";
        constexpr char JsonSchema[] = "{}";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption



    namespace GetInventory
    {
        constexpr char Name[] = "get_inventory";
        constexpr char Desc[] =
            R"(Get contents of your inventory)";
        constexpr char JsonSchema[] = "{}";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace GetSpells
    {
        constexpr char Name[] = "get_available_spells";
        constexpr char Desc[] =
            R"(Get a list of spells you can use)";
        constexpr char JsonSchema[] = "{}";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption





    namespace CallWaitMenu
    {
        constexpr char Name[] = "call_wait_menu_to_skip_ingame_time";
        constexpr char Desc[] =
            R"(Call wait menu)";
        constexpr char JsonSchema[] = "{}";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption





    namespace GetLocations
    {
        constexpr char Name[] = "get_locations_around";
        constexpr char Desc[] =
            R"(Get locations visible on compass)";
        constexpr char JsonSchema[] = "{}";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption



    namespace GetGold
    {
        constexpr char Name[] = "get_gold";
        constexpr char Desc[] =
            R"(Get amount of gold you have)";
        constexpr char JsonSchema[] = "{}";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption

} // namespace Capabilities



class StringView {

private:
    //std::string data = "";
    const char* data;

public:
    const char* Data()
    {
        return data;
    }

    StringView(const char* str = "") {
        data = str;
    }


    StringView& operator=(const StringView& other) 
    {
        data = other.data;
        return *this; 
    }

};

namespace Impl
{
    namespace JSON
    {

        struct NeuroChoiceJson
        {
            int id{};
        };

        struct NeuroChoiceJson2
        {
            int id1{};
            int id2{};
        };

        struct NeuroChoiceJson3
        {
            int id1{};
            int id2{};
            int id3{};
        };

        struct NeuroStringChoice
        {
            std::string choice{};
        };



    }
}




namespace neuro {

    /*
     * \brief Provides communication between AI and game.
     */

    struct NeuroSocket {
        // How many milliseconds can we poll until we should quit?
        static constexpr auto PollRateMs = 5;

        // Neuro API context
        neurosdk_context_t m_context;

        NeuroSocket();
        ~NeuroSocket();

        /**
        * \brief Send some context on what's happening ingame to Neuro (dialogue, we got in a car/out of a car, we got in a fight, we died...).
        * \param aContext The context to be told Neuro.
        * \param aSilent Whether Neuro should react to the provided context. Default is false.
        * 
        * \return The success of the operation.
        */
        bool SendContext(StringView aContext, bool aSilent = false);

        /**
        * \brief Respond to an action Neuro made. To prevent retries, the action is marked as a success by default.
        * \param aActionId The ID of the action Neuro made.
        * \param aMsg The message to be sent to Neuro.
        * \param aSuccess Whether the action was successful and thus shouldn't be retried.
        * 
        * \return The success of the operation.
        */
        bool RespondToAction(StringView aActionId, StringView aMsg, bool aSuccess = true);

        /**
         * \brief Send a message telling Neuro to do something.
         * 
         * \param aActionName The name of the action to take. We don't support multiple actions for this.
         * \param aQuery A short description for what Neuro should do.
         * \param aState A string describing the state of the game.
         * 
         * \return The success of the operation.
         */
        bool SendForcedAction(StringView aActionName, StringView aQuery, StringView aState, StringView aPriority = "medium");

        /**
         * \brief Poll Neuro's socket and check for new messages.
         * 
         * \param aClosure The processor function. NOTE: The strings in the action message are owned by the message and should be copied before use.
         * 
         * \return Whether or not all operations succeeded. If this returns false, you should *probably* kill the socket and create a new one.
         */


        
        //template<typename L>
        //    requires Red::Detail::IsClosure<L, void, const neurosdk_message_action_t&>



        

        bool Tick() //const neurosdk_message_action_t& aClosure)
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





                    if (name == Capabilities::SelectForceChoice::Name)
                    {
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
                                command_result = { true, "You dont have any choices to make" };
                            }
                        }
                    }

                    if (name == Capabilities::SelectForceChoiceMultiple::Name)
                    {

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
                            command_result = { true, "You dont have any choices to make. " };
                        }

                    }

                    if (name == Capabilities::SelectForceChoiceString::Name)
                    {

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
                        }
                    }

                    if (command_result.first && !dont_reset_force) //if got positive result above - force has been cleared
                        set_active_force(-1);

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
                                        command_result = MiscThings::unlock_shout_level(json.id);
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
        


        /**
         * \brief Create Neuro context and inform Neuro of capabilities.
         * \param aProcessor The callback function for handling Neuro's actions.
         * \return Whether or not initialization succeeded.
         */
        bool Initialize();

        // Is context still OK?
        bool IsAlive();

        static void LogNeuro(neurosdk_severity_e aSeverity, char *aMsg, void* aUserData);
    };
}
