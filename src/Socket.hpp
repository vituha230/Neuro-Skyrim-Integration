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
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the selected option", "type": "integer" } }, "required": ["id"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace SelectForceChoiceMultiple
    {
        constexpr char Name[] = "select_choice_multiple";
        constexpr char Desc[] =
            R"(Select multiple options. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id1": { "description": "The ID of the selected option", "type": "integer" }, "id2": { "description": "The ID of the selected option", "type": "integer" }, "id3": { "description": "The ID of the selected option", "type": "integer" } }, "required": ["id1", "id2"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace SelectForceChoiceString
    {
        constexpr char Name[] = "character_creation_choose_name";
        constexpr char Desc[] =
            R"(Give a choice in string format. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "choice": { "description": "Choose your character's name", "type": "string" } }, "required": ["choice"] })";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption



    namespace SelectForceChoiceArray
    {
        constexpr char Name[] = "select_choice_array";
        constexpr char Desc[] =
            R"(Select multiple options)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "ids_array": { "description": "Array of IDs of options", "type": "array", "items" : { "type": "integer" } }}, "required": ["ids_array"] })";//

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



    namespace Spin
    {
        constexpr char Name[] = "spin";
        constexpr char Desc[] =
            R"(Do some spins. Requires id1 (amount of spins) and id2 (speed))";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id1": { "description": "Amount of spins to do ", "type": "integer" }, "id2": { "description": "Speed. Valid range: from 1 to 5 ", "type": "integer" } }, "required": ["id1", "id2"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }



    namespace ExploreWorld
    {
        constexpr char Name[] = "explore";
        constexpr char Desc[] =
            R"([WILL CANCEL CURRENT WALKING OPERATION] Walk in some direction, exploring the surroundings)";
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    }


    namespace SurrenderToGuards
    {
        constexpr char Name[] = "surrender_to_guards";
        constexpr char Desc[] =
            R"(Try to surrender to guards and pay for your crimes)";
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    }



    namespace EscapePrison
    {
        constexpr char Name[] = "search_for_prison_exit";
        constexpr char Desc[] =
            R"(Try to find exit from prison to escape. )";
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    }


    namespace ExitDungeon
    {
        constexpr char Name[] = "exit_dungeon";
        constexpr char Desc[] =
            R"([WILL CANCEL CURRENT WALKING OPERATION] Walk towards the exit)";
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    }

    namespace WalkToObjectDoNothing
    {
        constexpr char Name[] = "walk_to_object";
        constexpr char Desc[] =
            R"(Walk to object specified by its ID)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the object. ", "type": "integer" } }, "required": ["id"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }


    namespace WalkToObject
    {
        constexpr char Name[] = "walk_to_object_and_interact";
        constexpr char Desc[] =
            R"(Walk to object specified by its ID and interact with it)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the object. ", "type": "integer" } }, "required": ["id"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }

    namespace AttackObject
    {
        constexpr char Name[] = "attack";
        constexpr char Desc[] =
            R"(Walk to object specified by its ID and attack it with equipped weapons or spells)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the object. ", "type": "integer" } }, "required": ["id"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }


    namespace PickpocketObject
    {
        constexpr char Name[] = "pickpocket";
        constexpr char Desc[] =
            R"(Walk to object specified by its ID and pickpocket it. Works only on humanoids)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the object. ", "type": "integer" } }, "required": ["id"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }



    namespace LookAtObject
    {
        constexpr char Name[] = "look_at_object";
        constexpr char Desc[] =
            R"(Look at object specified by its ID)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the object. ", "type": "integer" } }, "required": ["id"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    }


    namespace GetObjectsAround
    {
        constexpr char Name[] = "get_objects_around";
        constexpr char Desc[] =
            R"(Get a list of objects around you)";
        //constexpr char JsonSchema[] = null; //R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of object type to get. If not specified, all objects will be given. ", "type": "integer" } }, "required": [] })";


        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption



    namespace UseInventoryItem
    {
        constexpr char Name[] = "use_inventory_item";
        constexpr char Desc[] =
            R"(Use inventory item. Requires item ID)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the object to use. Use get_inventory to get list of object IDs. ", "type": "integer" } }, "required": ["id"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace DropInventoryItem
    {
        constexpr char Name[] = "drop_inventory_items";
        constexpr char Desc[] =
            R"(Drop inventory items. Requires array of item IDs)";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "ids_array": { "description": "Array of IDs of objects to drop. Use get_inventory to get list of object IDs. ", "type": "array", "items" : { "type": "integer" } }}, "required": ["ids_array"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    //CastEquipSpell

    namespace CastSpell
    {
        constexpr char Name[] = "cast_spell";
        constexpr char Desc[] =
            R"(Cast spell. Requires spell ID. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the spell. Use get_available_spells to get list of spell IDs. ", "type": "integer" } }, "required": ["id"] })";//

        constexpr neurosdk_action Action = { .name = Name, .description = Desc, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace EquipSpell
    {
        constexpr char Name[] = "equip_spell";
        constexpr char Desc[] =
            R"(Equip spell in hand. Requires spell ID. )";
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
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace FollowQuest
    {
        constexpr char Name[] = "follow_quest";
        constexpr char Desc[] =
            R"(Walk towards current quest objective)";

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };
    } // namespace SelectChoiceOption



    namespace ChangeQuest
    {
        constexpr char Name[] = "change_quest_to_follow";
        constexpr char Desc[] =
            R"(Change current quest and start following new quest. Requires quest ID. )";
        constexpr char JsonSchema[] =
            R"({ "additionalProperties": false, "type": "object", "properties": { "id": { "description": "The ID of the new quest to walk to. Use get_current_quests to get list of available quests. ", "type": "integer" } }, "required": ["id"] })";

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
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption



    namespace GetInventory
    {
        constexpr char Name[] = "get_inventory";
        constexpr char Desc[] =
            R"(Get contents of your inventory)";
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption


    namespace GetSpells
    {
        constexpr char Name[] = "get_available_spells";
        constexpr char Desc[] =
            R"(Get a list of spells you can use)";
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption





    namespace CallWaitMenu
    {
        constexpr char Name[] = "call_wait_menu_to_skip_ingame_time";
        constexpr char Desc[] =
            R"(Call wait menu)";
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption





    namespace GetLocations
    {
        constexpr char Name[] = "get_locations_around";
        constexpr char Desc[] =
            R"(Get locations visible on compass)";
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
    } // namespace SelectChoiceOption



    namespace GetGold
    {
        constexpr char Name[] = "get_gold";
        constexpr char Desc[] =
            R"(Get amount of gold you have)";
        //constexpr char JsonSchema[] = null;

        constexpr neurosdk_action Action = { .name = Name, .description = Desc };//, .json_schema = JsonSchema };
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


        struct NeuroChoiceJsonArrayInts
        {
            std::vector<int> ids_array{};
        };

        struct NeuroChoiceJsonArrayStrings
        {
            std::vector<std::string> ids_array{};
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



        bool Tick(float dtime); //const neurosdk_message_action_t& aClosure)





        /**
         * \brief Create Neuro context and inform Neuro of capabilities.
         * \param aProcessor The callback function for handling Neuro's actions.
         * \return Whether or not initialization succeeded.
         */
        bool Initialize();

        // Is context still OK?
        bool IsAlive();



        bool register_actions(neurosdk_action actions[], int size);
        bool unregister_actions(const char** action_names, int size);
        bool unregister_all();
        bool unregister_all2();
        bool register_allowed_actions(bool reconnect = false);

        bool action_register_watchdog(float dtime);


        


        static void LogNeuro(neurosdk_severity_e aSeverity, char* aMsg, void* aUserData);
    };


    void add_message_to_delayed_queue(std::string message);
}


