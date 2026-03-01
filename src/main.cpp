
#pragma warning( disable : 4305 )

//crucial:

//TODO remove dropped gear from enemies from object list (its lootable from their inventory, and picking it up from ground is buggy)


//TODO fix long distance run away (maybe take a bunch of objects with grid-like map distribution and take 2nd closest one) POTENTIALLY FIXED? test more
//TODO add info about current weapons in hands to both spell and inventory

//TODO fix attempts to interact with objects that are just too high/flying (for example: butterflies)

//TODO inventory weight control (maybe force throw away things?)
//TODO raycast object detection (!!! uuh maybe leave as is)
//TODO: check all menus after force/choice merge
//TODO: non-hostile spells will be cast excessively especially in right hand. need to figure out that (maybe irrelevant now that fights give nice info about what you are attacking with?)
//TODO: add notifications when someone attacks you


//TODO MAYBE FIXED. REMOVE IF NOT ENCOUNTERED FOR LONG TIME container menu bugged, after taking 1st item it doesnt force and looks like the scan is broken in this case
//TODO MAYBE FIXED. REMOVE IF NOT ENCOUNTERED FOR LONG TIME if attack ralof, sometimes it holds block forever (looks like when target got too far away while we were blocking - the block isnt interrupted)


//TODO combine close-lying little objects into one context entry


//TODO option to stop fight and surrender to guards

//TODO test all spells
//TODO solitude prison. check other prisons too
//TODO catch animation events on all objects
//TODO reanimating event catch + draugr wakes up from its grave
//TODO weather info when outside, time info
//TODO weird butterfly catching.. test it more
//TODO more puzzles (different kind of pillars, check metal spike traps, lifting floor trap, normal bear trap)
//TODO dvemer mechanisms/traps
//TODO spin mechanic. spin 3 times = knocked down/paralyzed and falls
//TODO probably fully automate after-talk get-in into the travel carriage
//TODO when an item is taken - it is added into inventory. but it doesnt update any list automatically - have to call inventory to know its id. and it may overlap with world object... figure out distinction
//TODO slaughterfish fights
//TODO underwater + oxygen control



//TODO potentially worth looking into:
//  button events like in race menu for text. maybe can finally use perfect key up/down events?
// it will probably require disabling actual inputs because it polls real devices which have all keys up.


//arbitrary:
//TODO maybe slam all barter items into one category like in alchemy
//TODO subtitles from far away that arent actually audible/visible 
//TODO running when path is along straight line
//TODO put things into container/gift menu (i think for followers its give not take)









//#include <string>

#include "InputActions.hpp"
#include "main.hpp"
#include <neurosdk.h>
#include "DialogueProcessor.hpp"
#include "BarterProcessor.hpp"
#include "LockpickProcessor.hpp"
#include "LevelupProcessor.hpp"
#include "PerksProcessor.hpp"
#include "AlchemyProcessor.hpp"
#include "EnchantProcessor.hpp"
#include "SmithingProcessor.hpp"
#include "ContainerProcessor.hpp"
#include "MapProcessor.hpp"
#include "WalkerProcessor.hpp"
#include "RandomMessageBoxProcessor.hpp"
#include "Observer.hpp"
#include "GiftProcessor.hpp"
#include "RaceProcessor.hpp"
#include "SleepWaitProcessor.hpp"
#include "BookProcessor.hpp"
#include "TrainingProcessor.hpp"


#include "Socket.hpp"
#include <vector>


//REMOVE ON RELEASE (i mean if we wont used it)
#include <fstream>

//i think these are not needed anymore, it just works without blocking actual user controls
bool API_CONTROL_DIALOGUE = false;
bool API_CONTROL_BARTER = false;
bool API_CONTROL_LOCKPICK = false;
bool API_CONTROL_CRAFTING = false;


bool do_debug_scan = false;

std::unique_ptr<neuro::NeuroSocket> m_neuroSocket{};




//universal force choice
bool force_choice(std::vector<MenuOption> options, std::string message, int force_type)
{
    if (get_active_force() == -1)
    {
        StringView force_name = Capabilities::SelectForceChoice::Name;

        if (force_type == force_type::alchemy_ingredients)
            force_name = Capabilities::SelectForceChoiceMultiple::Name;

        if (force_type == force_type::character_name)
            force_name = Capabilities::SelectForceChoiceString::Name;

        set_active_force(force_type);

        MiscThings::clean_controls_from_string(&message);
        set_universal_block(1.0f);
        std::string json{};
        bool result = !glz::write_json(options, json);
        m_neuroSocket->SendForcedAction(force_name,
            message.c_str(),
            json.c_str(),
            "medium");
        return true;
    }
    else
        return false;
}





/////////////////////////////////////////////////////////
//clean this mess later
int update_cycle = 0;
float cycle_timer = 0.0;
bool connected = false;
bool reconnect = false;
float reconnect_pause = 0.0f;
bool universal_block = false;
float universal_block_time = 0.0f;
float universal_block_time_threshold = 0.0f;
std::string speech_context_old = "";
std::vector<std::string> subtitle_msg_old_vector{"", "", ""};

float subtitle_history_clear_time = 0.0f;
/////////////////////////////////////////////////////////










bool is_universal_blocked() {return universal_block;}
void set_universal_block(float time) { universal_block = true; universal_block_time_threshold = time; }


extern neurosdk_action ActionsList[];

bool InitializeConnection()
{
    m_neuroSocket.reset();
    m_neuroSocket = std::make_unique<neuro::NeuroSocket>();

    return m_neuroSocket->Initialize();
}



void tick_socket(float dtime)
{
    if (!reconnect)
    {
        if (!connected)
        {
            if (!InitializeConnection())
                reconnect = true;
            else
                connected = true;
        }
        else
            if (!m_neuroSocket->Tick())
            {
                connected = false;
                reconnect = true;
                reconnect_pause = 0.0f;
            }
    }

    if (reconnect)
    {
        reconnect_pause += dtime;
        if (reconnect_pause > 3.0f)
        {
            connected = false;
            reconnect = false;
            reconnect_pause = 0.0f;
        }
    }
}



bool move_down = false;
bool move_up = false;
bool say = false;



#define MAX_QUEUE 100

std::vector<neurosdk_action> action_queue;

void action_queue_add(neurosdk_action action_name)
{
    if (action_queue.size() < (MAX_QUEUE))
    {
        action_queue.push_back(action_name);
    }
}

void queue_pop()
{
    action_queue.pop_back();
}


int allowed_events = 0;

void set_allowed_events(int amount) {   allowed_events = amount;    }







//maybe add some info about speaker too (his job or bio)
void send_speech_context(RE::TESObjectREFR* speaker, std::string speech_text)
{
    //THIS IS NPC'S RESPONSE SENT AS CONTEXT.
                               //"Dialogue: [Type \(lineDataDeref.type)] \(localizedSpeakerName) says \"\(line)\"";

    
    std::string speaker_name = MiscThings::insert_into_list_and_get_info(speaker);//speaker->GetName();


    if (speaker_name[0] == '\0')
    {
        speaker_name = "unknown speaker";
        return;//it gave subtitles to things that appear very far across the map and are not actually visible yet
    }
        

    auto actor = (RE::Actor*)speaker;
    auto emotion = actor->emotionType;
    std::string emotion_text = "";
    auto emotion_value = actor->emotionValue;

    if (emotion_value > 40) //maybe not the best threshold
    {
        switch (emotion) {
        case (RE::EmotionType::kAnger): emotion_text = "[Angry]"; break;
        case (RE::EmotionType::kDisgust): emotion_text = "[Disgusted]"; break;
        case (RE::EmotionType::kFear): emotion_text = "[Scared]"; break;
        case (RE::EmotionType::kHappy): emotion_text = "[Happy]"; break;
        case (RE::EmotionType::kPuzzled): emotion_text = "[Confused]"; break;
        case (RE::EmotionType::kSad): emotion_text = "[Sad]"; break;
        case (RE::EmotionType::kSurprise): emotion_text = "[Surprised]"; break;
        }
    }

    if (speech_text[0] != '\0')
    {
        std::string speech_context = speaker_name + emotion_text + " says \"" + speech_text + "\"";
        if (DialogueProcessor::is_in_dialogue(speaker))
            speech_context = "[Dialogue] " + speech_context;

        if (m_neuroSocket)
            m_neuroSocket->SendContext(speech_context.c_str(), true);
    }
}


void send_random_context(std::string context)
{
    if (context.find("No direct path seen") != std::string::npos)
        return;

    if (m_neuroSocket)
        m_neuroSocket->SendContext(context.c_str(), true);
}





//lets try do trading or alchemy
//i think its better to start with alchemy





class DialogueMenuEx final : public RE:: DialogueMenu //this allowes skip dialogue any time. someone's else plugin. credit or remove later.
{
public:
    static void Install()
    {
        REL::Relocation<uintptr_t> vtbl(RE::VTABLE_DialogueMenu[0]);
        _ProcessMessageFn = vtbl.write_vfunc(0x4, &ProcessMessageEx);
    }

    RE::UI_MESSAGE_RESULTS ProcessMessageEx(RE::UIMessage& a_message)
    {

        std::string msg = "";



        if (a_message.type == RE::UI_MESSAGE_TYPE::kUpdate) 
        {
            doAllowProgressFix();
        }
        
        if (a_message.type == RE::UI_MESSAGE_TYPE::kHide)
            send_random_context("[The dialogue ended]");


        if (a_message.type == RE::UI_MESSAGE_TYPE::kShow) {
            doAllowProgressFix();

            if (const auto ui = RE::UI::GetSingleton(); ui) {
                if (const auto menu = ui->GetMenu(RE::DialogueMenu::MENU_NAME); menu) {
                    //menu->menuFlags.set(RE::UI_MENU_FLAGS::kPausesGame); //works
                    menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works


                    //clear_input_queue();
                    //right_attack_cancel();
                    //left_attack_cancel();
                    //perk_down_cancel();
                    //perk_up_cancel();
                    //mouse_mouse_x_y(0.0f, 0.0f);


                    send_random_context("[Started a dialogue]");

                    //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::CursorMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);

                    menu->uiMovie->SetVariable("_root.DialogueMenu_mc.bAllowProgress", true);
                }
            }
        }
        


        if (API_CONTROL_DIALOGUE)
        {
            bool dont_skip_this_message = false;

            if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent)
            {
                if (const auto data = static_cast<RE::BSUIScaleformData*>(a_message.data); data)
                {
                    RE::GFxEvent* event = (RE::GFxEvent*)(data->scaleformEvent);
                    if (event && event->type == RE::GFxEvent::EventType::kMouseDown)
                        if (!DialogueProcessor::is_selected_line_correct())
                            return RE::UI_MESSAGE_RESULTS::kHandled;
                        else
                            DialogueProcessor::dialogue_choice_reset();
                    if (event && event->type == RE::GFxEvent::EventType::kMouseUp || event->type == RE::GFxEvent::EventType::kKeyUp) //always let button ups
                        dont_skip_this_message = true;
                }
            }

            RE::UI* ui = RE::UI::GetSingleton();
            if (!dont_skip_this_message && a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent && (allowed_events == 0) && !ui->IsMenuOpen(RE::BarterMenu::MENU_NAME) && !ui->IsMenuOpen(RE::TutorialMenu::MENU_NAME))
            {
                return RE::UI_MESSAGE_RESULTS::kHandled; //works
            }

            if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent && allowed_events > 0) //i like this. now only integration messages count
                allowed_events--;
        }

       

        return _ProcessMessageFn(this, a_message);
    }

private:
    using ProcessMessageFn = decltype(&RE::DialogueMenu::ProcessMessage);

    inline static REL::Relocation<ProcessMessageFn> _ProcessMessageFn;

    static void doAllowProgressFix()
    {
        if (const auto ui = RE::UI::GetSingleton(); ui) {
            if (const auto menu = ui->GetMenu(RE::DialogueMenu::MENU_NAME); menu) {
                menu->uiMovie->SetVariable("_root.DialogueMenu_mc.bAllowProgress", true);
                RE::GFxValue var1;
            }
        }
    }
};










namespace Hooks {


    struct SleepWaitProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::SleepWaitMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("SLEEPWAIT MENU WAS OPENED");

                menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); 

            }

            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_SleepWaitMenu[0]).write_vfunc(0x4, thunk); }
    };


    struct MessageBoxProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::MessageBoxMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("MESSAGEBOX MENU WAS OPENED");

                menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works

                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }

            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_MessageBoxMenu[0]).write_vfunc(0x4, thunk); }
    };


    struct LevelupProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::LevelUpMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("LEVELUP MENU WAS OPENED");

                menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works

                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }

            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_LevelUpMenu[0]).write_vfunc(0x4, thunk); }
    };


    struct StatsMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::StatsMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("STATS MENU WAS OPENED");

                menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works

                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }

            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_StatsMenu[0]).write_vfunc(0x4, thunk); }
    };


    struct ContainerMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::ContainerMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("ONTAINER MENU WAS OPENED");

                menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works

                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }

            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_ContainerMenu[0]).write_vfunc(0x4, thunk); }
    };


    struct TweenMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::TweenMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("TWEEN MENU WAS OPENED");

                menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works

                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }

            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_TweenMenu[0]).write_vfunc(0x4, thunk); }
    };


    struct LoadingMenuProcessMessage { 
        static RE::UI_MESSAGE_RESULTS thunk(RE::LoadingMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("LOADING MENU WAS OPENED");

                auto time_of_death = MiscThings::get_time_of_death();

                auto now = std::chrono::steady_clock::now().time_since_epoch().count();
                float delta_death = (double)(now - time_of_death) / 1000000000.0;

                bool probably_death = delta_death < 10.0f;

                if (!probably_death)
                {
                    MiscThings::reset_misc(); //clear compas locations on location change
                }

                WalkerProcessor::reset_walker();
                MiscThings::clear_object_list();
                Observer::reset_threats();
                Observer::reset_observer();
                Observer::clear_objects_to_track();
                MapProcessor::reset_menu2();
                SleepWaitProcessor::saveload_reset();
                ContainerProcessor::reset_pickpocketing();
                set_active_force(-1);

                reset_input_processor();
                clear_input_queue();

                send_random_context("[The game is loading. You are not in game yet]");
            }

            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kHide) {
                RE::ConsoleLog::GetSingleton()->Print("LOADING MENU WAS CLOSED");

                auto player = RE::PlayerCharacter::GetSingleton();

                if (player)
                {
                    auto cell = player->GetParentCell();
                    if (cell)
                    {
                        std::string cell_name1 = cell->GetFullName();
                        std::string cell_name2 = cell->GetName();

                        std::string location_name = "Tamriel";

                        if (cell_name1 != "")
                            location_name = cell_name1;
                        else
                            if (cell_name2 != "")
                                location_name = cell_name2;
                            else
                            {
                                RE::BSTArray<RE::ObjectRefHandle> map_markers = player->currentMapMarkers;
                                float min_distance = std::numeric_limits<float>::max();
                                std::string sublocation_name = "";

                                for (auto marker : map_markers)
                                {
                                    if (marker.get())
                                    {
                                        auto real_marker = marker.get().get();
                                        auto data = (RE::ExtraMapMarker*)real_marker->extraList.GetByType(RE::ExtraDataType::kMapMarker);
                                        if (data && data->mapData && data->mapData->flags)
                                        {
                                            std::string marker_name = data->mapData->locationName.GetFullName();
                                            if (marker_name != "")
                                            {
                                                auto distance = real_marker->GetDistance(player);
                                                if (distance < min_distance)
                                                {
                                                    min_distance = distance;
                                                    sublocation_name = marker_name;
                                                }
                                            }
                                        }
                                    }
                                }

                                if (sublocation_name != "")
                                    location_name += ". " + sublocation_name;
                            }
                        
                        

                        send_random_context("[You are in game. Current location: " + location_name + ". Use commands to interact with the game. ]");
                    }
                }
            }


            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kUpdate)
            {
                //idk if i'll find updater that works in loading menu. there should be one somewhere..
                ;//tick_socket(0.016); //this doesnt work
            }


            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_LoadingMenu[0]).write_vfunc(0x4, thunk); }
    };

    struct HUDMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::HUDMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("HUDMenu WAS OPENED");

                //WalkerProcessor::reset_walker(); //this shit doesnt actually work. hud menu stays loaded in loading it seems

                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }
           
            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_HUDMenu[0]).write_vfunc(0x4, thunk); }
    };



    struct ProcessEvent {

        static RE::UI_MESSAGE_RESULTS thunk(RE::RaceSexMenu* menu, RE::UIMessage& a_message)
        //static bool thunk(RE::RaceSexMenu* menu, RE::ButtonEvent* a_event)
        {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kChatterEvent)
            {
                bool test_123 = false;
            }

            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kScaleformEvent)
            {
                auto msg = (RE::BSUIScaleformData*)(a_message.data);
                auto key_event = (RE::GFxKeyEvent*)(msg->scaleformEvent);




                bool test_123 = false;
            }
                

            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kUserEvent)
            {
                auto msg = (RE::BSUIMessageData*)(a_message.data);

                bool test_123 = false;
                
            }

            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_RaceSexMenu[0]).write_vfunc(0x4, thunk); }
    };




    struct CursorMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::CursorMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("MAP MENU WAS OPENED");
                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
                //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works
                //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kAssignCursorToRenderer);

                //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUpdateUsesCursor); //works

               // auto submenu = &menu->localMapMenu;


                auto test_new_input3 = RE::ControlMap::GetSingleton();

                for (int i = 0; i < 20; i++)
                {
                    auto ii = static_cast<RE::UserEvents::INPUT_CONTEXT_ID>(i);

                    //test_new_input3->PopInputContext(ii);
                }
                //test_new_input3->PushInputContext(RE::UserEvents::INPUT_CONTEXT_ID::kMap);

            }
            else
                if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent)
                {
                    if (const auto data = static_cast<RE::BSUIScaleformData*>(a_message.data); data)
                    {
                        RE::GFxEvent* event = (RE::GFxEvent*)(data->scaleformEvent);
                        if (event && event->type == RE::GFxEvent::EventType::kMouseDown)
                            if (!DialogueProcessor::is_selected_line_correct())
                                ;// return RE::UI_MESSAGE_RESULTS::kHandled;
                            else
                                ;//dialogue_choice_reset();

                        RE::UI* ui = RE::UI::GetSingleton();
                        //if (ui->IsMenuOpen(RE::MapMenu::MENU_NAME))
                        //    if (event && event->type == RE::GFxEvent::EventType::kMouseMove) //always let button ups
                        //        ;// return RE::UI_MESSAGE_RESULTS::kHandled; //works
                    }
                }

            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_CursorMenu[0]).write_vfunc(0x4, thunk); }
    };



    struct MapMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::MapMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("MAP MENU WAS OPENED");
                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
                //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works
                //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kAssignCursorToRenderer);
                //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUpdateUsesCursor); //works

                
                //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works


                //auto player = RE::PlayerCharacter::GetSingleton();
                auto target = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x7003887);
                if (target)
                {
                    RE::TESWorldSpace* worldSpace = target->GetWorldspace();
                    if (worldSpace)
                    {
                        worldSpace->worldMapData.cameraData.minHeight = 40000.0f;
                        worldSpace->worldMapData.cameraData.initialPitch = 90.0f;
                        worldSpace->worldMapData.nwCellX = -200;
                        worldSpace->worldMapData.nwCellY = 200;
                        worldSpace->worldMapData.seCellX = 200;
                        worldSpace->worldMapData.seCellY = -400;
                    }
                }
                


            }
            else
                if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kHide) {
                    RE::ConsoleLog::GetSingleton()->Print("MAP MENU WAS OPENED");
                    //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::MapMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
                    //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works
                    //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kAssignCursorToRenderer);
                    //menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUpdateUsesCursor); //works

                    //auto player = RE::PlayerCharacter::GetSingleton();
                    auto target = (RE::TESObjectREFR*)RE::TESForm::LookupByID(0x7003887);
                    if (target)
                    {
                        RE::TESWorldSpace* worldSpace = target->GetWorldspace();
                        if (worldSpace)
                        {
                            worldSpace->worldMapData.cameraData.minHeight = 50000.0f;
                            worldSpace->worldMapData.cameraData.initialPitch = 50.0f;
                            worldSpace->worldMapData.nwCellX = -30; //original values
                            worldSpace->worldMapData.nwCellY = 15;
                            worldSpace->worldMapData.seCellX = 40;
                            worldSpace->worldMapData.seCellY = -40;
                        }
                    }



                    /*
        nwCellX	-30	short
        nwCellY	15	short
        seCellX	40	short
        seCellY	-40	short

        */

                }
                else
                {
                    if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent)
                    {
                        if (const auto data = static_cast<RE::BSUIScaleformData*>(a_message.data); data)
                        {
                            RE::GFxEvent* event = (RE::GFxEvent*)(data->scaleformEvent);
                            if (event && event->type == RE::GFxEvent::EventType::kMouseDown)
                                if (!DialogueProcessor::is_selected_line_correct())
                                    ;// return RE::UI_MESSAGE_RESULTS::kHandled;
                                else
                                    ;//dialogue_choice_reset();

                            if (event && event->type == RE::GFxEvent::EventType::kMouseMove) //always let button ups
                                ;// return RE::UI_MESSAGE_RESULTS::kHandled; //works
                        }
                    }
                }


            /*else
                if (a_message.type.get() != RE::UI_MESSAGE_TYPE::kUpdate)
                {
                    std::string msg = "Map menu message: ";
                    msg += std::to_string((uint32_t)a_message.type.get());
                    auto test1 = (RE::BSUIScaleformData*)a_message.data;

                    if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kHide)
                    {
                        RE::UIMessageQueue::GetSingleton()->AddMessage(RE::CursorMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr);
                        auto test_new_input3 = RE::ControlMap::GetSingleton();

                        auto priority_stack = test_new_input3->contextPriorityStack;
                        auto linked_mappings = test_new_input3->linkedMappings;

                        for (int i = 0; i < 20; i++)
                        {
                            auto ii = static_cast<RE::UserEvents::INPUT_CONTEXT_ID>(i);

                            test_new_input3->PopInputContext(ii);
                        }

                        test_new_input3->PushInputContext(RE::UserEvents::INPUT_CONTEXT_ID::kGameplay);

                    }
                    if (map_processor::abandon_all_messages())
                        return RE::UI_MESSAGE_RESULTS::kHandled;
                }
*/

            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_MapMenu[0]).write_vfunc(0x4, thunk); }
    };




    //this works. but how do i check time if the game is paused?
    struct TutorialMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::TutorialMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("TUTORIAL MENU WAS OPENED");
                RE::UIMessageQueue::GetSingleton()->AddMessage(RE::TutorialMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus
            
                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }
            else
                if (a_message.type.get() != RE::UI_MESSAGE_TYPE::kUpdate) 
                {
                    std::string msg = "Tutorial menu message: ";
                    msg += std::to_string((uint32_t)a_message.type.get());
                    
                    /*
                    BSUIMessageData
                    BSUIScaleformData
                    ConsoleData
                    HUDData
                    InventoryUpdateData
                    MessageBoxData
                    */

                    auto test1 = (RE::BSUIScaleformData*)a_message.data;


                    //we want kUserEvent message which has "Cancel"



                    //IUIMessageData try all of this type
                    //auto test = a_message.type.;

                   


                    //RE::ConsoleLog::GetSingleton()->Print(msg.c_str());
                }
            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_TutorialMenu[0]).write_vfunc(0x4, thunk); }
    };

  


    struct BarterMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::BarterMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kHide) {
                RE::ConsoleLog::GetSingleton()->Print("Barter MENU WAS CLOSED");
                DialogueProcessor::clean_old_dialogue(); //so dialogue continues on barter closed
                universal_block = true;
            }
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("Barter MENU WAS OPENED");
                menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works

                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::TutorialMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }
            else
                if (API_CONTROL_BARTER)
                {
                    bool dont_skip_this_message = false;

                    if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent)
                    {
                        if (const auto data = static_cast<RE::BSUIScaleformData*>(a_message.data); data)
                        {
                            RE::GFxEvent* event = (RE::GFxEvent*)(data->scaleformEvent);
                            if (event && event->type == RE::GFxEvent::EventType::kMouseDown)
                                if (!DialogueProcessor::is_selected_line_correct())
                                    ;// return RE::UI_MESSAGE_RESULTS::kHandled;
                                else
                                    ;//dialogue_choice_reset();

                            if (event && event->type == RE::GFxEvent::EventType::kMouseUp || event->type == RE::GFxEvent::EventType::kKeyUp) //always let button ups
                                dont_skip_this_message = true;
                        }
                    }

                    RE::UI* ui = RE::UI::GetSingleton();
                    if (!dont_skip_this_message && a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent && (allowed_events == 0) && !ui->IsMenuOpen(RE::TutorialMenu::MENU_NAME))
                    {
                        return RE::UI_MESSAGE_RESULTS::kHandled; //works
                    }

                    if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent && allowed_events > 0) //i like this. now only integration messages count
                        allowed_events--;
                }

            //get_category_list();


            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_BarterMenu[0]).write_vfunc(0x4, thunk); }
    };


    struct LockpickingMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::LockpickingMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kHide) {
                RE::ConsoleLog::GetSingleton()->Print("Lockpicking MENU WAS CLOSED");
                DialogueProcessor::clean_old_dialogue(); //so dialogue continues on barter closed
                universal_block = true;
            }
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("Lockpicking MENU WAS OPENED");
                menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works

                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::TutorialMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }
            else
                if (API_CONTROL_LOCKPICK)
                {
                    bool dont_skip_this_message = false;

                    if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent)
                    {
                        if (const auto data = static_cast<RE::BSUIScaleformData*>(a_message.data); data)
                        {
                            RE::GFxEvent* event = (RE::GFxEvent*)(data->scaleformEvent);
                            if (event && event->type == RE::GFxEvent::EventType::kMouseUp || event->type == RE::GFxEvent::EventType::kKeyUp) //always let button ups
                                dont_skip_this_message = true;
                        }
                    }

                    RE::UI* ui = RE::UI::GetSingleton();
                    if (!dont_skip_this_message && a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent && (allowed_events == 0) && !ui->IsMenuOpen(RE::TutorialMenu::MENU_NAME))
                    {
                        return RE::UI_MESSAGE_RESULTS::kHandled; //works
                    }

                    if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent && allowed_events > 0) //i like this. now only integration messages count
                        allowed_events--;
                }

            //get_category_list();


            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_LockpickingMenu[0]).write_vfunc(0x4, thunk); }
    };


    struct CraftingMenuProcessMessage {
        static RE::UI_MESSAGE_RESULTS thunk(RE::CraftingMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kHide) {
                RE::ConsoleLog::GetSingleton()->Print("Crafting MENU WAS CLOSED");
                DialogueProcessor::clean_old_dialogue(); //so dialogue continues on barter closed
                universal_block = true;
            }
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("Crafting MENU WAS OPENED");
                menu->menuFlags.reset(RE::UI_MENU_FLAGS::kUsesCursor); //works

                //RE::UIMessageQueue::GetSingleton()->AddMessage(RE::TutorialMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }
            else
                if (API_CONTROL_CRAFTING)
                {
                    bool dont_skip_this_message = false;

                    if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent)
                    {
                        if (const auto data = static_cast<RE::BSUIScaleformData*>(a_message.data); data)
                        {
                            RE::GFxEvent* event = (RE::GFxEvent*)(data->scaleformEvent);
                            if (event && event->type == RE::GFxEvent::EventType::kMouseUp || event->type == RE::GFxEvent::EventType::kKeyUp) //always let button ups
                                dont_skip_this_message = true;
                        }
                    }

                    RE::UI* ui = RE::UI::GetSingleton();
                    if (!dont_skip_this_message && a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent && (allowed_events == 0) && !ui->IsMenuOpen(RE::TutorialMenu::MENU_NAME))
                    {
                        return RE::UI_MESSAGE_RESULTS::kHandled; //works
                    }

                    if (a_message.type == RE::UI_MESSAGE_TYPE::kScaleformEvent && allowed_events > 0) //i like this. now only integration messages count
                        allowed_events--;
                }

            //get_category_list();


            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_CraftingMenu[0]).write_vfunc(0x4, thunk); }
    };









    //this works. but how do i check time if the game is paused?
    struct RandomHook {
        static RE::UI_MESSAGE_RESULTS thunk(RE::TutorialMenu* menu, RE::UIMessage& a_message) {
            if (a_message.type.get() == RE::UI_MESSAGE_TYPE::kShow) {
                RE::ConsoleLog::GetSingleton()->Print("TUTORIAL MENU WAS OPENED");
                RE::UIMessageQueue::GetSingleton()->AddMessage(RE::TutorialMenu::MENU_NAME, RE::UI_MESSAGE_TYPE::kHide, nullptr); //OKAY this shit just autocloses tutorial menus

                //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->cancel, RE::INPUT_DEVICES::kKeyboard);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
                //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

            }
            else
                if (a_message.type.get() != RE::UI_MESSAGE_TYPE::kUpdate)
                {
                    std::string msg = "Tutorial menu message: ";
                    msg += std::to_string((uint32_t)a_message.type.get());

                    /*
                    BSUIMessageData
                    BSUIScaleformData
                    ConsoleData
                    HUDData
                    InventoryUpdateData
                    MessageBoxData
                    */

                    auto test1 = (RE::BSUIScaleformData*)a_message.data;


                    //we want kUserEvent message which has "Cancel"



                    //IUIMessageData try all of this type
                    //auto test = a_message.type.;




                    //RE::ConsoleLog::GetSingleton()->Print(msg.c_str());
                }
            return originalFunction(menu, a_message);
        }
        static inline REL::Relocation<decltype(thunk)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_TutorialMenu[0]).write_vfunc(0x4, thunk); }
    };





    /*
    struct HookMain {
        static RE::BSEventNotifyControl ProcessEvent_ex(const RE::BSGamerProfileEvent* a_event, RE::BSTEventSource<RE::BSGamerProfileEvent>* a_eventSource)
        {
            //get_category_list();

            return originalFunction(a_event, a_eventSource);
        }
        static inline REL::Relocation<decltype(ProcessEvent_ex)> originalFunction;
        static inline void Install() { originalFunction = REL::Relocation<std::uintptr_t>(RE::VTABLE_Main[0]).write_vfunc(0x1, ProcessEvent_ex); }
    };
    */



}


auto lasttime = std::chrono::steady_clock::now().time_since_epoch().count();






//this update works in pause
class OnUpdateHook {
public:
    static void Install() {
        auto address = REL::ID(36564).address();
        auto offset = REL::Offset(0xC26).offset();
        OnUpdate = REL::GetTrampoline().write_call<5>(address + offset, OnUpdateMod);
    }
private:
    static void OnUpdateMod() {
        // call original function
        //get_category_list();

        long long now = std::chrono::steady_clock::now().time_since_epoch().count();
        float dtime = (double)(now - lasttime) / 1000000000.0;
        lasttime = now;
        
        if (universal_block)
        {
            if (universal_block_time > universal_block_time_threshold)
            {
                universal_block = false;
                universal_block_time = 0.0f;
            }
            else
                universal_block_time += dtime;
        }



        
        tick_socket(dtime);

        

        if (!universal_block)
        {
            const auto ui = RE::UI::GetSingleton();
            if (ui && !ui->IsMenuOpen(RE::Console::MENU_NAME))
            {
                if (do_debug_scan) debug_scan(dtime);
                BarterProcessor::processor(dtime);
                LockpickProcessor::processor(dtime);
                LevelupProcessor::processor(dtime);
                PerksProcessor::processor(dtime);
                AlchemyProcessor::processor(dtime);
                EnchantProcessor::processor(dtime);
                SmithingProcessor::processor(dtime);
                MapProcessor::processor(dtime);
                ContainerProcessor::processor(dtime);
                GiftProcessor::processor(dtime);
                RandomMessageBoxProcessor::processor(dtime);

                Observer::detect_threats(dtime); //uses pause. must be here
                Observer::detect_interesting_objects(dtime);
                Observer::detect_events(dtime);
                Observer::player_state_monitor(dtime);
                Observer::detect_locations(dtime);

                RaceProcessor::processor(dtime);

                MiscThings::notifications();

                SleepWaitProcessor::processor(dtime);
                BookProcessor::processor(dtime);
                TrainingProcessor::processor(dtime);
                ;
            }
                
        }
        

        OnUpdate();
        // do my stuff
    }
    static inline REL::Relocation<decltype(OnUpdateMod)> OnUpdate;
};





///////////////////////////////////
//scan for vars
std::string constraints = "constraints"; //this shit is recursive, gives infinite result
std::string focusTarget = "focusTarget";
std::string iSelectedIndex = "iSelectedCategory";

std::string text_name = "text";


std::string visit_all_members_BIG(std::ofstream& fs, const RE::GFxValue& in, int& tabs)
{
    std::string result = "";

    in.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
        {
            for (int i = 0; i < tabs; i++)
                 fs << " ";
            fs << name;

            std::string name_str = name;
            
            if (a_value.IsObject())
            {
                if (name_str == constraints || name_str == focusTarget)
                {
                     fs << "{...}\n";
                }
                else
                {
                     fs << "\n";

                    for (int i = 0; i < tabs; i++)
                         fs << " ";

                     fs << "{\n";

                    tabs += 1;

                    result += visit_all_members_BIG(fs, a_value, tabs);

                    tabs -= 1;

                    for (int i = 0; i < tabs; i++)
                         fs << " ";

                     fs << "}\n";
                }

            }
            else
                if (name_str == iSelectedIndex)
                {
                    result += "\n";
                    result += iSelectedIndex + " = ";
                    result += std::to_string(a_value.GetNumber());

                    fs << iSelectedIndex << " = ";
                    fs << a_value.GetNumber();
                    fs << "\n";
                }
                else
                     fs << "\n";
        });

    return result;
}

std::string visit_all_members(bool do_dump, std::ofstream& fs, const RE::GFxValue& in, std::string* search_var, int depth)
{
    if (depth > 20)
        return "&";

    std::string result = "";

    int amount_of_members = 0;
    bool first_member = true;

    in.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
        {
            if (first_member)
            {
                first_member = false;
                if (do_dump) fs << "\n";
            }
            amount_of_members++;

            std::string name_str = name;
            std::string name_orig = name;

            if (a_value.IsArray())
                name_str += "[" + std::to_string(a_value.GetArraySize()) + "]";

            for (int i = 0; i < depth; i++)
                if (do_dump) fs << " ";

            if (do_dump) fs << name_str;

            if (a_value.IsObject())
            {
                if (name_str == constraints || name_str == focusTarget)
                {
                    if (do_dump) fs << "{...}\n";
                }
                else
                {
                    if (do_dump) fs << "{";

                    


                    if (std::string subresult = visit_all_members(do_dump, fs, a_value, search_var, depth + 1); subresult != "")
                    {
                        if (subresult == "^^")
                        {
                            if (do_dump)
                                result = subresult;
                        }
                        else
                        {
                            if (subresult == "&")
                            {
                                if (do_dump) fs << " {&&&}";
                                result = subresult;
                            }
                            else
                                result = name_str + "." + subresult;
                        }

                    }
                        

                    if (result != "&")
                    {
                        if (result != "^^")
                            for (int i = 0; i < depth; i++)
                                if (do_dump) fs << " ";

                        if (do_dump) fs << "}";
                    }


                    if (do_dump) fs << "\n";
                }
            }
            else
                if (name_orig == *search_var)
                {

                    if (do_dump) fs << "\n";


                    if (result == "" || result == "^^" || result == "&")
                        result = name_str;
                }
                else
                {
                    if (a_value.GetType() == RE::GFxValue::ValueType::kString)
                    {
                        if (do_dump) fs << " = ";
                        if (do_dump) fs << a_value.GetString();
                    }

                    if (a_value.GetType() == RE::GFxValue::ValueType::kNumber)
                    {
                        if (do_dump) fs << " = ";
                        if (do_dump) fs << std::to_string(a_value.GetNumber());
                    }


                    if (do_dump) fs << "\n";

                }

        });

        if (amount_of_members > 0)
            if (result == "^^")
                return ""; //we had members but got this, its not from this layer. cancel it
            else
                return result;
        else
            if (result == "")
                return "^^";
            else
                return result;
}



void add_path_to_all_results(std::vector<std::string>& results, std::string path)
{
    int i = 0;

    for (std::string dummy : results)
    {
        results[i] = path + "." + results[i];
        i++;
    }

}


bool visit_all_members2(std::vector<std::string>& results, const RE::GFxValue& in, std::string* search_var, int depth, std::string current_path, std::vector<std::string>& skip_problematic, std::vector<std::string>& skip_problematic_path, std::string parent_name, std::string grand_parent_name)
{
    if (depth > 10)
        return false;

    bool found_something = false;

    if (!in.IsNull())// && in.IsDisplayObject())
    {
        in.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
            {

                if (!a_value.IsNull())
                {
                    std::string name_str = name;
                    std::string name_orig = name;

                    bool dont_go_deeper = false;


                    if (name_str == grand_parent_name)
                        dont_go_deeper = true;

                    for (std::string problematic_name : skip_problematic)
                    {
                        if (name_str == problematic_name)
                            dont_go_deeper = true;

                    }

                    for (std::string problematic_path : skip_problematic_path)
                    {
                        std::string path_to_test = current_path + "." + name_str;
                        if (path_to_test == problematic_path)
                            dont_go_deeper = true;
                    }

                    if (!dont_go_deeper)
                    {

                        if (a_value.IsArray())
                            name_str += "[" + std::to_string(a_value.GetArraySize()) + "]";

                        if (a_value.IsObject())
                        {
                            if (name_str != constraints && name_str != focusTarget)
                            {
                                if (name == *search_var || name_orig == *search_var)
                                {
                                    results.push_back(current_path + "." + name_str + (a_value.IsNumber() ? " = " + std::to_string(a_value.GetNumber()) : ""));
                                    found_something = true;
                                }
                                else
                                {
                                    if (visit_all_members2(results, a_value, search_var, depth + 1, current_path + "." + name_str, skip_problematic, skip_problematic_path, name_str, parent_name))
                                    {
                                        //add_path_to_all_results(results, name);
                                        found_something = true;
                                    }
                                }
                            }
                        }
                        else
                            if (name == *search_var || name_orig == *search_var || name_orig.find(*search_var) != std::string::npos)
                            {
                                results.push_back(current_path + "." + name_str + (a_value.IsNumber() ? " = " + std::to_string(a_value.GetNumber()) : ""));
                                found_something = true;
                            }
                    }
                }

            });
    }
    


     return found_something;
}


bool visit_all_members3(std::vector<std::string>& results, const RE::GFxValue& in, std::string* search_var, int depth, std::string current_path, std::string val_to_search, std::vector<std::string>& skip_problematic)
{

    if (depth > 20)
        return false;

    bool found_something = false;

    if (!in.IsNull())
    {
        in.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
            {

                if (!a_value.IsNull())
                {
                    std::string name_str = name;
                    std::string name_orig = name;

                    bool dont_go_deeper = false;

                    for (std::string problematic_name : skip_problematic)
                    {
                        if (name_str == problematic_name)
                            dont_go_deeper = true;

                    }

                    if (!dont_go_deeper)
                    {
                        if (a_value.IsArray())
                        {
                            name_str += "[" + std::to_string(a_value.GetArraySize()) + "]";
                        }


                        if (a_value.IsObject())
                        {
                            if (name_str != constraints && name_str != focusTarget)
                            {
                                if (visit_all_members3(results, a_value, search_var, depth + 1, current_path + "." + name_str, val_to_search, skip_problematic))
                                {
                                    //add_path_to_all_results(results, name);
                                    found_something = true;
                                }
                            }
                        }
                        else
                        {
                            if (a_value.IsString())
                            {
                                std::string val = a_value.GetString();
                                if (val.find(val_to_search) != std::string::npos)
                                {
                                    results.push_back(current_path + "." + name_str + " = " + a_value.GetString());
                                    found_something = true;
                                }
                            }
                        }
                    }
                }

            });

    }

    return found_something;
}


bool visit_all_members4(std::vector<std::string>& results, const RE::GFxValue& in, std::string* search_var, int depth, std::string current_path, int val_to_search)
{
    if (depth > 20)
        return "&";

    bool found_something = false;

    if (!in.IsNull())
    {
        in.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
            {
                if (!a_value.IsNull())
                {
                    std::string name_str = name;
                    std::string name_orig = name;

                    if (a_value.IsArray())
                    {
                        name_str += "[" + std::to_string(a_value.GetArraySize()) + "]";
                    }


                    if (a_value.IsObject())
                    {
                        if (name_str != constraints && name_str != focusTarget)
                        {
                            if (visit_all_members4(results, a_value, search_var, depth + 1, current_path + "." + name_str, val_to_search))
                            {
                                add_path_to_all_results(results, name);
                                found_something = true;
                            }
                        }
                    }
                    else
                    {
                        if (a_value.IsNumber())
                        {
                            int val = a_value.GetNumber();
                            if (val == val_to_search)
                            {
                                results.push_back(current_path + "." + name_str + " = " + std::to_string(val));
                                found_something = true;
                            }
                        }
                    }
                }


            });
    }

    return found_something;
}



/*

void walk_forward()
{
    int test = 10;

    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.1f, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

*/



void visit_perk_tree(std::vector<std::string>* results, RE::BGSSkillPerkTreeNode* node)
{
    if (node && node->perk)
    {
        results->push_back(node->perk->GetName());
        for (auto child : node->children)
            visit_perk_tree(results, child);
    }
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



struct BSSceneGraph : RE::NiNode
{
    RE::NiPointer<RE::NiCamera> spCamera;
    std::uint64_t pVisArray;
    std::uint64_t pCuller;
    bool bMenuSceneGraph;
    float fCurrentFOV;
};

struct SceneGraph : BSSceneGraph
{
    float fCustomNearDistance;
    float fCustomFarDistance;
    bool bCustomNearDistanceActive;
    bool bCustomFarDistanceActive;
};


/*
void ViewPointToRay(RE::NiCamera* cam, float a_1, float a_2, RE::NiPoint3& a_location, RE::NiPoint3& a_aimVector)
{
    using func_t = decltype(ViewPointToRay);
    REL::Relocation<func_t> func{ REL::ID(460613) };
    return func(cam, a_1, a_2, a_location, a_aimVector);
}
*/


// constructor override, but CLib bitches about it so just add it as a separate func
/*
void bhkPickData_CFilter(RE::bhkPickData* pickdata, uint32_t a_collisionFilter, RE::NiPoint3& a_location, RE::NiPoint3& a_aimVector, float a_range)
{
    using func_t = decltype(bhkPickData_CFilter);
    REL::Relocation<func_t> func{ REL::ID(1464124) };
    func(pickdata, a_collisionFilter, a_location, a_aimVector, a_range);
}
*/


//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



RE::ArrowProjectile* the_arrow{};
RE::NiPoint3 last_arrow_pos{};

#include <iostream>

std::ofstream fs("arrowSpeed.txt");


std::map<RE::TESObjectREFR*, std::string> refrs{};


bool test_debug2 = false;
float debug_time2 = 0.0f;
float debug_time3 = 0.0f;
float start_pos = 0.0f;

RE::NiPoint3 original_dif{};


RE::ATTACK_STATE_ENUM last_attack_state;
std::vector<RE::ATTACK_STATE_ENUM> track_attack{};


//this update doesnt work in pause
class MyHook {
    static void Update(RE::PlayerCharacter* a, float dtime) 
    {

        //virtual BSContainer::ForEachResult Visit(BGSPerkEntry* a_perkEntry) = 0;  // 00
        
        /*
        std::vector<std::string> perk_names{};

        for (RE::BGSPerk* perk : a->perks)
        {
            ;// perk_names.push_back(perk->GetName());
        }

        
        for (std::uint32_t formID = 0; formID < 0xFF000000; formID++)
        {
            if (auto form = RE::TESForm::LookupByID(formID); form && (form->GetFormType() == RE::FormType::Perk))
            {
                perk_names.push_back(form->GetName());
            }
        }
        */


        



        std::string result = "";

        auto player = RE::PlayerCharacter::GetSingleton();
        auto player_ref = player->AsReference();
        auto player_actor = (RE::Actor*)player_ref;

        auto test_controls = RE::PlayerControls::GetSingleton();


        auto attack_state = player_actor->GetAttackState();

        if (attack_state != last_attack_state)
        {
            last_attack_state = attack_state;
            track_attack.push_back(attack_state);
        }


        if (false)
        {

            if (debug_time3 < 5.0f)
            {
                if (player_ref)
                {
                    RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 30000.0f,
                        [&](RE::TESObjectREFR* a_ref) {

                            std::string name = "";
                            name = a_ref->GetDisplayFullName();
                            refrs.insert({ a_ref, name });

                            return RE::BSContainer::ForEachResult::kContinue;
                        });
                }
                debug_time3 += dtime;


                auto camera = RE::PlayerCamera::GetSingleton();
                auto camera_dir = camera->cameraRoot.get()->world.rotate;

                auto camera_dirX = camera_dir.GetVectorX();
                auto camera_dirY = camera_dir.GetVectorY();
                auto camera_dirZ = camera_dir.GetVectorZ();

                RE::NiPoint3 camera_dirY_fixed = camera_dirY;// { -camera_dirY.z, camera_dirY.y, camera_dirY.x };

                //auto dir_dif_X = pos_dif_norm - camera_dir_2;

                float mouse_x = 0.0f;
                float mouse_y = 0.0f;

                auto mulX = 0.0f;// camera_dirX* pos_dif_norm;
                auto mulY = 0.0f;
                auto mulZ = 0.0f;

                RE::NiPoint3 desired1 = { 1.0f, 0.0f, 0.0f };
                RE::NiPoint3 desired2 = { 0.0f, 1.0f, 0.0f };
                RE::NiPoint3 desired3 = { 0.0f, 0.0f, 1.0f };


                auto pos_difX = desired1 - camera_dirX;
                auto pos_difY = desired2 - camera_dirY;
                auto pos_difZ = desired3 - camera_dirZ;

                auto pos_difX_norm = pos_difX / pos_difX.Length();
                auto pos_difY_norm = pos_difY / pos_difY.Length();
                auto pos_difZ_norm = pos_difZ / pos_difZ.Length();

                mulX = camera_dirX * pos_difX_norm;
                mulY = camera_dirY * pos_difY_norm;
                mulZ = camera_dirZ * pos_difZ_norm;

                //if (mulY < 0)
                //    mulZ = -mulZ;

                mouse_y = mulZ * 400.0f;

                mouse_mouse_y(-mouse_y);

            }
            else
            {
                if (the_arrow)
                {
                    auto cur_arrow_pos = the_arrow->GetPosition();
                    auto player_pos = player_ref->GetPosition();
                    player_pos += original_dif;

                    auto pos_dif = cur_arrow_pos - player_pos;
                    pos_dif.z = 0.0f;

                    auto distance = pos_dif.Length();


                    if (last_arrow_pos != cur_arrow_pos)
                    {
                        //fs << std::to_string(distance) << "\t" << std::to_string(cur_arrow_pos.z - start_pos) << "\n";
                        fs << std::to_string(distance) << "\t" << std::to_string(dtime) <<  "\n";

                        last_arrow_pos = cur_arrow_pos;
                    }
                    else
                    {
                        fs.close();
                        refrs.clear();
                        the_arrow = nullptr;
                        debug_time3 = 0.0f;
                        start_pos = 0.0f;
                    }


                }
                else
                    if (player_ref)
                    {
                        RE::TES::GetSingleton()->ForEachReferenceInRange(player_ref, 30000.0f,
                            [&](RE::TESObjectREFR* a_ref) {

                                auto base_obj = a_ref->GetBaseObject();
                                auto base_type = base_obj->GetFormType();


                                //if (base_obj && base_type == RE::FormType::ProjectileArrow)
                                if (refrs.find(a_ref) == refrs.end())
                                {
                                    the_arrow = (RE::ArrowProjectile*)a_ref;
                                    bool catched_an_arrow = true;
                                    start_pos = the_arrow->GetPosition().z;

                                    original_dif = a_ref->GetPosition() - player_ref->GetPosition();
                                }

                                return RE::BSContainer::ForEachResult::kContinue;
                            });
                    }
            }

        }
        
        

        







        RE::BSTArray<RE::TESForm*> my_array{};

        my_array = RE::TESDataHandler::GetSingleton()->GetFormArray(RE::FormType::Perk);


        //IsLookingAtSomething(a);

        RE::BSTArray<RE::BGSPerk*> my_array2{};

        for (RE::TESForm* perk : my_array)
        {
            //perk->As()
            auto perk2 = perk->As<RE::BGSPerk>();
            if (!perk2->data.hidden && perk2->data.playable && !perk2->data.trait && perk2->fullName != "")
            {
                if (perk2->perkConditions.IsTrue(a, a))
                    my_array2.push_back(perk2);
            }
        }

       
        auto perk_tree = RE::ActorValueList::GetSingleton()->actorValues;

        auto test = RE::ActorValueList::GetSingleton()->GetActorValueInfo(RE::ActorValue::kSneak);

        //right_attack();
   

        _Update(a, dtime);

        update_cycle++;
        cycle_timer += dtime;

        std::string msg = "";


        /*
        if (false && debug_time2 > 3.0f)
        {
            if (!test_debug2)
            {
                test_debug2 = true;

                auto test_new_input = RE::BSInputDeviceManager::GetSingleton();
                test_new_input->ReinitializeMouse();
                int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);

                test_new_input->GetKeyboard()->SetButtonState(my_key, 1.0f, false, true);

                test_new_input->ReinitializeMouse();

            }
        }
        else
            debug_time2 += dtime;
            */
        
        input_processor(dtime);

        if (!universal_block)
        {
            WalkerProcessor::processor(dtime);
            DialogueProcessor::processor(dtime);
        }

        
        



        ////////////SUBTITLE TESTS///////////////////// seems to work. prints subtitles into console
        std::string subtitle_msg = "";


        if (subtitle_history_clear_time > 2.0f)
        {
            //clear all but last. so it doesnt repeat actual ongoing message
            for (int i = 1; i < std::size(subtitle_msg_old_vector); i++)
                subtitle_msg_old_vector.at(i) = "";

            if (subtitle_history_clear_time > 10.0f)
                subtitle_msg_old_vector.clear(); //clear all. if its a repeat, its been actually repeated.
        }
        else
            subtitle_history_clear_time += dtime;


        RE::SubtitleManager* sub_manager = RE::SubtitleManager::GetSingleton();
        if (sub_manager)
            if (sub_manager->subtitles.data())
            {
                std::string subtitle_speaker = "";

                if (sub_manager->currentSpeaker)
                {
                    subtitle_speaker = sub_manager->currentSpeaker.get()->GetName();
                }

                subtitle_msg = sub_manager->subtitles.data()->subtitle;
                //subtitle_msg = subtitle_speaker + subtitle_msg;



                //std::string current_context = topic_manager->dialogueLis

                bool is_new = true;
                for (auto subtitle_msg_old : subtitle_msg_old_vector)
                {
                    if (subtitle_msg_old == subtitle_msg)
                    {
                        is_new = false;
                        break;
                    }    
                }
                if (is_new)
                {
                    //RE::ConsoleLog::GetSingleton()->Print(subtitle_msg.c_str());
                    if (sub_manager->currentSpeaker.get())
                    {
                        send_speech_context(sub_manager->currentSpeaker.get().get(), subtitle_msg);

                        subtitle_history_clear_time = 0.0f;

                        int subtitle_msg_old_vector_max = 3;

                        
                        for (int i = 0; i < (subtitle_msg_old_vector_max - 1); i++)
                        {
                            if (std::size(subtitle_msg_old_vector) < subtitle_msg_old_vector_max)
                            {
                                if (std::size(subtitle_msg_old_vector) > 0)
                                    subtitle_msg_old_vector.push_back(subtitle_msg_old_vector.at(std::size(subtitle_msg_old_vector) - 1));
                            }
                            else
                            {
                                subtitle_msg_old_vector.at((subtitle_msg_old_vector_max - 1) - i) = subtitle_msg_old_vector.at((subtitle_msg_old_vector_max - 2) - i);
                            }
                        }

                        if (std::size(subtitle_msg_old_vector) > 0) //has 1 or more elements
                            subtitle_msg_old_vector.at(0) = subtitle_msg;
                        else
                            subtitle_msg_old_vector.push_back(subtitle_msg);

                    }

                }

            }
            else
                subtitle_msg_old_vector.clear();
        else
            subtitle_msg_old_vector.clear();

        RE::GFxValue var1;
          


        //visit members tests
        if (const auto ui = RE::UI::GetSingleton(); ui) {
            if (const auto menu = ui->GetMenu(RE::DialogueMenu::MENU_NAME); menu) {
                //menu->uiMovie->SetVariable("_root.DialogueMenu_mc.bAllowProgress", true);
                //menu->uiMovie->SetVariable("_root.DialogueMenu_mc.bAllowProgress", true);
                if (menu->uiMovie->GetVariable(&var1, "_root"))
                {
                    std::string result;

                    if (true)
                    {
                        //std::ofstream fs("dumpVarPaths.txt");

                        int tabs1 = 0;
                        //result = visit_all_members(var1);

                        //fs.close();
                    }


                    //RE::GFxValue var2;
                    
                    //RE::GFxValue::ObjectVisitor visitor;
                    auto test = var1.GetType();


                    

                    var1.VisitMembers([&](const char* name, const RE::GFxValue& a_value)
                        {
                            RE::GFxValue textVal;
                            auto info = a_value.GetType();

                            //a_value.GetMember("text", &textVal);
                            //dbg_breakpoint = true;
                            //if (text = textVal.GetString(); text == "$MOD MANAGER") 
                            //{
                            //   modMenuIndex = index;
                            //}
                            //index++;
                        });

                }
            }
        }
    
        ///////////////////////////////////////////////////


   

    //end of myhookupdate
    }


    static inline REL::Relocation<decltype(Update)> _Update;

public:
    static void Hook() {
        _Update = REL::Relocation<uintptr_t>(RE::VTABLE_PlayerCharacter[0]).write_vfunc(0xAD, Update);
    }
};










////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SKSE ZONE

// For every message sent to the plugin by SKSE, log the message.
void MessageListener(SKSE::MessagingInterface::Message* message) {
    switch (message->type) {
        // Descriptions are taken from the original skse64 library
        // See:
        // https://github.com/ianpatt/skse64/blob/09f520a2433747f33ae7d7c15b1164ca198932c3/skse64/PluginAPI.h#L193-L212
    case SKSE::MessagingInterface::kPostLoad:
        REX::INFO("kPostLoad: sent to registered plugins once all plugins have been loaded");
        break;
    case SKSE::MessagingInterface::kPostPostLoad:
        REX::INFO(
            "kPostPostLoad: sent right after kPostLoad to facilitate the correct dispatching/registering of "
            "messages/listeners");
        break;
    case SKSE::MessagingInterface::kPreLoadGame:
        // message->dataLen: length of file path, data: char* file path of .ess savegame file
        REX::INFO("kPreLoadGame: sent immediately before savegame is read");
        break;
    case SKSE::MessagingInterface::kPostLoadGame:
        // You will probably want to handle this event if your plugin uses a Preload callback
        // as there is a chance that after that callback is invoked the game will encounter an error
        // while loading the saved game (eg. corrupted save) which may require you to reset some of your
        // plugin state.
        REX::INFO("kPostLoadGame: sent after an attempt to load a saved game has finished");
        break;
    case SKSE::MessagingInterface::kSaveGame:
        REX::INFO("kSaveGame");
        break;
    case SKSE::MessagingInterface::kDeleteGame:
        // message->dataLen: length of file path, data: char* file path of .ess savegame file
        REX::INFO("kDeleteGame: sent right before deleting the .skse cosave and the .ess save");
        break;
    case SKSE::MessagingInterface::kInputLoaded:
        REX::INFO("kInputLoaded: sent right after game input is loaded, right before the main menu initializes");
        break;
    case SKSE::MessagingInterface::kNewGame:
        // message-data: CharGen TESQuest pointer (Note: I haven't confirmed the usefulness of this yet!)
        REX::INFO("kNewGame: sent after a new game is created, before the game has loaded");
        break;
    case SKSE::MessagingInterface::kDataLoaded:
        REX::INFO("kDataLoaded: sent after the data handler has loaded all its forms");
        MyHook::Hook();
        SKSE::AllocTrampoline(14);
        OnUpdateHook::Install();
        break;
    default:
        REX::INFO("Unknown system message of type: {}", message->type);
        break;
    }
}


SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse);
    DialogueMenuEx::Install(); //THIS IS WORKING DIALOGUE PROCESSOR
    Hooks::LockpickingMenuProcessMessage::Install();
    Hooks::TutorialMenuProcessMessage::Install();
    //Hooks::DialogueMenuProcessMessage::Install();
    Hooks::BarterMenuProcessMessage::Install();
    Hooks::CraftingMenuProcessMessage::Install();
    Hooks::MapMenuProcessMessage::Install();
    Hooks::HUDMenuProcessMessage::Install();
    Hooks::LoadingMenuProcessMessage::Install();
    Hooks::TweenMenuProcessMessage::Install();
    Hooks::CursorMenuProcessMessage::Install();
    Hooks::ContainerMenuProcessMessage::Install();
    Hooks::StatsMenuProcessMessage::Install();
    Hooks::LevelupProcessMessage::Install();
    Hooks::MessageBoxProcessMessage::Install();
    Hooks::SleepWaitProcessMessage::Install();

    Hooks::ProcessEvent::Install();



    WalkerProcessor::install_hook();


    //Hooks::HookMain::Install();
    


	SKSE::GetMessagingInterface()->RegisterListener(MessageListener);

	//REX::INFO("Hello world!");



	return true;
}
