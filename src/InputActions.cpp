

#include "WalkerProcessor.hpp"
#include "InputActions.hpp"
#include "main.hpp"
#include "Misc.hpp"


//extern bool was_casting_clairvoyance();

//from barter

//


//test clear


bool long_cast_ult = false;
float use_ult_time = 0.0f;
bool canceled_inputs = false;
bool were_casting_something_right = false;
bool were_casting_something_left = false;


bool do_cast = false;
bool right_hand_cast = false;
bool notified_cast = false;
float spell_cast_time = 0.0f;


float fishing_timer = 0.0f;


bool input_wants_to_cast()
{
    return do_cast;
}


void reset_input_processor()
{
    were_casting_something_left = false;
    were_casting_something_right = false;
    long_cast_ult = false;
    use_ult_time = 0.0f;
    canceled_inputs = false;

    do_cast = false;
    right_hand_cast = false;
    notified_cast = false;
    spell_cast_time = 0.0f;

    fishing_timer = 0.0f;

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

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0f);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void right_attack_bow()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0f, 100000000.0f);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    set_allowed_events(2);
}


void right_attack_spell()
{
    were_casting_something_right = true;

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

        int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->leftAttack, RE::INPUT_DEVICES::kGamepad);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 1.0, 1000.0f);
        //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
        set_allowed_events(2);
    }
}

void right_attack_cancel()
{
    were_casting_something_right = false;

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

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->readyWeapon, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0); //i hope this works
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);
}
*/

void unslow_walk()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->run, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);
}

void slow_walk()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->run, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1.0); //i hope this works
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);
}

void press_t()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->wait, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}


void uncarry()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //i hope this works
    set_allowed_events(2);
}

void carry()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 1.0); //i hope this works
    set_allowed_events(2);
}

void crouch()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sneak, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void jump()
{

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

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->localMap, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}


void cursor_down()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->back, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void cursor_up()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}


void left()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeLeft, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void right()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeRight, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void perk_right()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeRight, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
}

void perk_right_cancel()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeRight, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}

void perk_left()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeLeft, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
}

void perk_left_cancel()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->strafeLeft, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}



void perk_up()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomIn, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);

    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}


void perk_down()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomOut, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);


    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->back, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0); //with this it cannot even move
}


void perk_up_cancel()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomIn, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);

    //int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
}


void perk_down_cancel()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomOut, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
}

void confirm_dialogue()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 100.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);
}



void confirm()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

}

void confirm_noblock()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);

}

void confirm_workbench()
{
    //wtf does it want

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->activate, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);

}


void confirm_craft()
{

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

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->pause, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);
}


void cancel10()
{

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

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);
}



//from lockpicking. these are a bit different
void forward_lockpick()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    set_allowed_events(1);
}

void cancel_forward_lockpick()
{

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
    RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y, "Cursor");
}




void mouse_mouse_x(float angle)
{
    auto test = RE::ControlMap::GetSingleton();

    //mouse_mouse_x_y(angle, 0.0f);

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

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->rightAttack, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
    set_universal_block(0.5f);
}



void mouse_mouse_x_y_limited(float x, float y)
{

    //clear_input_queue();


    if (RE::BSInputEventQueue::GetSingleton()->mouseEventCount < 5) {
        RE::BSInputEventQueue::GetSingleton()->AddMouseMoveEvent(x, y);
    }
}

void walk_forward_limited()
{

    //clear_input_queue();


    if (RE::BSInputEventQueue::GetSingleton()->buttonEventCount < 5) {
        int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->forward, RE::INPUT_DEVICES::kKeyboard);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 1.0, 0.0);
        RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    }

}




void sprint()
{

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

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->sprint, RE::INPUT_DEVICES::kGamepad);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kGamepad, my_key, 0.0, 0.0);


    //static auto kEvent1 = RE::ButtonEvent::Create(RE::INPUT_DEVICE::kNone, "shout", 0, 1.0f, 0.0f);


    //RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kKeyboard, my_key, 0.0, 0.0);
    //set_allowed_events(2);
    //set_universal_block(0.5f);
}



void dialogue_cursor_down()
{

    int32_t my_key = RE::ControlMap::GetSingleton()->GetMappedKey(RE::UserEvents::GetSingleton()->zoomOut, RE::INPUT_DEVICES::kMouse);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 1.0, 0.0);
    RE::BSInputEventQueue::GetSingleton()->AddButtonEvent(RE::INPUT_DEVICES::kMouse, my_key, 0.0, 0.0);
    set_allowed_events(2);
}

void dialogue_cursor_up()
{

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




void try_casting_hand(bool right)
{
    if (!do_cast)
    {
        do_cast = true;
        right_hand_cast = right;
        spell_cast_time = 0.0f;
    }

}


bool make_long_cast_spell_hand(bool right, float dtime)
{
    auto player = RE::PlayerCharacter::GetSingleton();
    auto player_actor = (RE::Actor*)player->AsReference();

    if (player_actor && !player_actor->IsWeaponDrawn() && !(player_actor->actorState2.weaponState == RE::WEAPON_STATE::kDrawing))
    {
        ready_weapon();
        set_universal_block(1.0f);
        return false;
    }


    bool low_mana_detected = false;
    bool dont_check_mana = false;

    dont_check_mana = !WalkerProcessor::is_concentration_spell(right) && WalkerProcessor::is_casting_walker(right);

    bool low_mana_check = (!dont_check_mana && WalkerProcessor::has_spell_equipped(right) && (low_mana_detected || (MiscThings::get_player_mana() < WalkerProcessor::get_spell_cost(right))));

    bool check_time = !MiscThings::is_self_healing_spell(right);

    spell_cast_time += dtime;

    if (low_mana_check || (check_time && (spell_cast_time > WalkerProcessor::get_attack_time(right))) || (WalkerProcessor::has_spell_equipped(right) && MiscThings::is_self_healing_spell(right) && MiscThings::player_hp_more_than(100.0f)))
    {

        //set_universal_block(1.0f);
        if (right)
            right_attack_cancel();
        else
            left_attack_cancel();

        return true;
    }
    else
    {
        if (!notified_cast)
        {
            notified_cast = true;
            std::string cast_info = "[You are casting ";
            cast_info += WalkerProcessor::get_equipped_spell_name(right) + "]";
            send_random_context(cast_info);
        }



        if (right)
            right_attack_spell();
        else
            left_attack_spell();

        

        return false;
    }


    

}




void make_long_ult_cast()
{
    long_cast_ult = true;
    use_ult_time = 0.0f;
}



void input_processor(float dtime)
{

    //ccBGSSSE001_FishingSystemQuest
    //ccBGSSSE001_FishingSystemScript


    auto fishing_quest = (RE::TESQuest*)RE::TESForm::LookupByEditorID("ccBGSSSE001_FishingSystemQuest");

    if (fishing_quest)
    {
        auto object_p = MiscThings::General::Script::GetObject(fishing_quest, "ccBGSSSE001_FishingSystemScript");

        RE::BSFixedString prop_name = "currentSystemState";
        int fishing_state = MiscThings::General::Script::GetVariable<int>(object_p, prop_name);

        if (fishing_state == 4)
        {
            if (fishing_timer > 2.0f)
                confirm();
            else
                fishing_timer += dtime;
        }
        else
            fishing_timer = 0.0f;
    }

    if (do_cast)
    {
        if (make_long_cast_spell_hand(right_hand_cast, dtime))
        {
            do_cast = false;
            right_hand_cast = false;
            notified_cast = false;
        }
    }



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