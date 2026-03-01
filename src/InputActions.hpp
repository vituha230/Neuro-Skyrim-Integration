#pragma once


void local_map_toggle();
void cursor_down();
void cursor_up();
void left();
void right();
void confirm();
void confirm_craft();
void cancel();
void cancel10();
void leftclick();
void forward_lockpick();
void cancel_forward_lockpick();
void mouse_mouse_x(float angle);
void mouse_mouse_y(float angle);
void mouse_mouse_x_y(float x, float y);
void perk_left();
void perk_right();
void perk_up();
void perk_down();
void perk_left_cancel();
void perk_right_cancel();
void perk_up_cancel();
void perk_down_cancel();
void crouch();
void carry();
void jump();
void uncarry();
void slow_walk();
void unslow_walk();
void confirm_workbench();
void clear_leftclick();

void mouse_mouse_x_y_limited(float x, float y);
void walk_forward_limited();

void clear_input_queue();


void ready_weapon();
void left_attack();
void right_attack();
void left_attack_spell();
void right_attack_spell();
void left_attack_cancel();
void right_attack_cancel();

void sprint();
void unsprint();

void use_ult();


void walk_forward();


void confirm_noblock();


void press_t();



void input_processor(float dtime);
void make_long_ult_cast();
void reset_input_processor();


void right_attack_bow();

void quicksave();


void dialogue_cursor_down();
void dialogue_cursor_up();