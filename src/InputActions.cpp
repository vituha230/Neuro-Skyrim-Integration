

#include "WalkerProcessor.hpp"
#include "InputActions.hpp"
#include "main.hpp"


//extern bool was_casting_clairvoyance();

//from barter

//


//test clear


bool long_cast_ult = false;
float use_ult_time = 0.0f;
bool canceled_inputs = false;
bool were_casting_something_right = false;
bool were_casting_something_left = false;


void reset_input_processor()
{
    were_casting_something_left = false;
    were_casting_something_right = false;
    long_cast_ult = false;
    use_ult_time = 0.0f;
    canceled_inputs = false;

}

/*
void set_were_casting_something_left(bool set)
{
    were_casting_something_left = set;
}

void set_were_casting_something_right(bool set)
{
    were_casting_something_right = set;
}
*/

bool is_casting_input(bool right)
{
    if (right)
        return were_casting_something_right;
    else
        return were_casting_something_left;
}


void right_attack()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0f, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void left_attack()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void right_attack_bow()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0f, 100000000.0f);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    set_allowed_events(2);
}


void right_attack_spell()
{
    were_casting_something_right = true;
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kGamepad);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 1.0f, 1000.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void left_attack_spell()
{
    if (!WalkerProcessor::was_casting_clairvoyance())
    {
        were_casting_something_left = true;
        //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
        int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kGamepad);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 1.0, 1000.0f);
        //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
        set_allowed_events(2);
    }
}

void right_attack_cancel()
{
    were_casting_something_right = false;
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0f, 0.0f);
    my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kGamepad);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0f, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void left_attack_cancel()
{
    were_casting_something_left = false;
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0f, 0.0f);
    my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kGamepad);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0f, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}


void clear_input_queue()
{
    auto event_queue = RE::BSInputEventQueue::GetSingleton();

    
    RE::BSInputEventQueue::GetSingleton()->kinectEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->connectEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->thumbstickEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->mouseEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->charEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->buttonEventCount = 0;
    RE::BSInputEventQueue::GetSingleton()->queueTail = nullptr;
    RE::BSInputEventQueue::GetSingleton()->queueHead = nullptr;






}

/*
void ready_weapon()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->readyWeapon, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0); //i hope this works
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);
}
*/

void unslow_walk()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->run, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);
}

void slow_walk()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->run, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1.0); //i hope this works
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);
}

void press_t()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->wait, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}


void uncarry()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);
}

void carry()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1.0); //i hope this works
    set_allowed_events(2);
}

void crouch()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sneak, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void jump()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->jump, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void start_use_ult()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->shout, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 10000.0);
}

void stop_use_ult()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->shout, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}


void use_ult()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->shout, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1000.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}

void local_map_toggle()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->localMap, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}


void cursor_down()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->back, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void cursor_up()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}


void left()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeLeft, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void right()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeRight, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void perk_right()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeRight, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
}

void perk_right_cancel()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeRight, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}

void perk_left()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeLeft, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
}

void perk_left_cancel()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeLeft, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}



void perk_up()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomIn, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}


void perk_down()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomOut, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);

    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->back, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //with this it cannot even move
}


void perk_up_cancel()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomIn, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}


void perk_down_cancel()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomOut, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
}

void confirm_dialogue()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 100.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);
}



void confirm()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

}

void confirm_noblock()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

}

void confirm_workbench()
{
    //wtf does it want
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

}


void confirm_craft()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->readyWeapon, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

}



void ready_weapon()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->readyWeapon, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
}



void cancel()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->pause, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);
}


void cancel10()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->pause, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);
}

void leftclick()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);
}



//from lockpicking. these are a bit different
void forward_lockpick()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(1);
}

void cancel_forward_lockpick()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(1);
}


void lockpick_move_x(float angle)
{
    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->look, RE::INPUT_DEVICES::kGamepad);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0, angle);
    //RE::BSInputEventQueue::GetSingleton()->AddThumbstickEvent(0.0f, 0.0f);
    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(angle, 0.0, "RotatePick");

}


void mouse_look(float x, float y)
{
    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y, "Look");
}


void mouse_cursor_move(float x, float y)
{
    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y);
}




void mouse_mouse_x(float angle)
{
    auto test = RE::ControlMap::GetSingleton();

    //mouse_mouse_x_y(angle, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(angle, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(1);
    /*
    auto bsInputEventQueue = RE::BSInputEventQueue::GetSingleton();
    bsInputEventQueue->ClearInputQueue();
    static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);
    static auto kEvent2 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 0.0f, holdTime);
    bsInputEventQueue->PushOntoInputQueue(kEvent1);
    bsInputEventQueue->PushOntoInputQueue(kEvent2);
    set_allowed_events(1);
    */
}


void mouse_mouse_y(float angle)
{
    //mouse_mouse_x_y(0.0f, angle);
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(0.0, angle);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(1);
    /*
    auto bsInputEventQueue = RE::BSInputEventQueue::GetSingleton();
    bsInputEventQueue->ClearInputQueue();
    static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);
    static auto kEvent2 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 0.0f, holdTime);
    bsInputEventQueue->PushOntoInputQueue(kEvent1);
    bsInputEventQueue->PushOntoInputQueue(kEvent2);
    set_allowed_events(1);
    */
}


void mouse_mouse_x_y(float x, float y)
{
    RE::MouseMoveEvent* mouse_event = &RE::BSInputEventQueue::GetSingleton()->mouseEvents[0];
    //mouse_event->mouseInputX = angle_to_move;
    //mouse_event->userEvent = "RotatePick";
    //menu->ProcessMouseMove(mouse_event);



    //clear_input_queue();
    auto test = RE::BSInputEventQueue::GetSingleton();

    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(1);
    /*
    auto bsInputEventQueue = RE::BSInputEventQueue::GetSingleton();
    bsInputEventQueue->ClearInputQueue();
    static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);
    static auto kEvent2 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 0.0f, holdTime);
    bsInputEventQueue->PushOntoInputQueue(kEvent1);
    bsInputEventQueue->PushOntoInputQueue(kEvent2);
    set_allowed_events(1);
    */
}






void clear_leftclick()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);
}



void mouse_mouse_x_y_limited(float x, float y)
{

    //clear_input_queue();

    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    if (RE::BSInputEventQueue::GetSingleton()->mouseEventCount < 5) {
        RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y);
    }
}

void walk_forward_limited()
{

    //clear_input_queue();

    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    if (RE::BSInputEventQueue::GetSingleton()->buttonEventCount < 5) {
        int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    }

}




void sprint()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprintStart, RE::INPUT_DEVICES::kGamepad);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 1.0f, 100000000.0f);

    auto key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kKeyboard);
    auto win32Keyboard = RE::BSInputDeviceManager::GetSingleton()->GetKeyboard();
    auto it = win32Keyboard->deviceButtons.find(key);
    if (it != win32Keyboard->deviceButtons.end())
    {
        it->second->heldDownSecs = 1000.0f;
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, key, 1.0, 1000.0f);
    }
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, key, 0.0, 0.0);


    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    //set_allowed_events(2);
    //set_universal_block(0.5f);
}



void unsprint()
{
    //RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kGamepad);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0, 0.0);


    //static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);


    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    //set_allowed_events(2);
    //set_universal_block(0.5f);
}



void dialogue_cursor_down()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomOut, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void dialogue_cursor_up()
{
    RE::BSInputEventQueue::GetSingleton()->ClearInputQueue();
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomIn, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}




void walk_forward()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 100.0f);// 10000000.0); //ineffective but maybe fix later
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //refuses to walk without it
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0f, 100.0f);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
}


void quicksave()
{
    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->quicksave, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}


void make_long_ult_cast()
{
    long_cast_ult = true;
    use_ult_time = 0.0f;
}

void input_processor(float dtime)
{
    if (long_cast_ult)
    {
        set_universal_block(0.5f);
        if (!canceled_inputs)
        {
            canceled_inputs = true;
            clear_input_queue();
        }
        else
        {
            if (WalkerProcessor::pause_attacking(dtime))
            {
                if (use_ult_time < 2.5f)
                {
                    use_ult_time += dtime;
                    start_use_ult();
                }
                else
                {
                    WalkerProcessor::unpause_attacking();
                    stop_use_ult();
                    reset_input_processor();
                }
            }
                
        }


    }
}