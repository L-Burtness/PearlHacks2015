/*
* main.c
* Creates a Window, InverterLayer and animates it around the screen
* from corner to corner. It uses the `.stopped` callback to schedule the
* next stage of the animation.
*/

#include <pebble.h>

#define BOX_SIZE 6
#define TITLE_BAR_SIZE 16

#define ANIM_DURATION 1800
#define ANIM_DELAY 150

static Window *s_main_window;
static InverterLayer *s_box_layer;
static PropertyAnimation *s_box_animation;

static int s_current_stage = 0;

// Function prototype
static void next_animation();

static void anim_stopped_handler(Animation *animation, bool finished, void *context) {
 // Free the animation
 property_animation_destroy(s_box_animation);

 // Schedule the next one, unless the app is exiting
 if (finished) {
   next_animation();
 }
}

static void next_animation() {
 // Determine start and finish positions
 GRect start, finish;
 int num =  rand() % 150;

 switch (s_current_stage) {
   default:
     //start = GRect(144 - BOX_SIZE, 168 - BOX_SIZE - TITLE_BAR_SIZE, BOX_SIZE, BOX_SIZE);
     start = GRect(144 - BOX_SIZE, num, BOX_SIZE, BOX_SIZE);
     finish = GRect(-BOX_SIZE, num, BOX_SIZE, BOX_SIZE);


     break;

 }

 // Schedule the next animation
 s_box_animation = property_animation_create_layer_frame(inverter_layer_get_layer(s_box_layer), &start, &finish);
 animation_set_duration((Animation*)s_box_animation, ANIM_DURATION);
 animation_set_delay((Animation*)s_box_animation, ANIM_DELAY);
 animation_set_curve((Animation*)s_box_animation, AnimationCurveEaseInOut);
 animation_set_handlers((Animation*)s_box_animation, (AnimationHandlers) {
   .stopped = anim_stopped_handler
 }, NULL);
 animation_schedule((Animation*)s_box_animation);

 // Increment stage and wrap
 s_current_stage = (s_current_stage + 1) % 4;
}

static void main_window_load(Window *window) {
 Layer *window_layer = window_get_root_layer(window);

 // Create InverterLayer
 s_box_layer = inverter_layer_create(GRect(0, 0, BOX_SIZE, BOX_SIZE));
 layer_add_child(window_layer, inverter_layer_get_layer(s_box_layer));
}

static void main_window_unload(Window *window) {
 // Destroy InverterLayer
 inverter_layer_destroy(s_box_layer);
}

static void init(void) {
 // Create main Window
 s_main_window = window_create();
 window_set_background_color(s_main_window, GColorBlack);
 window_set_window_handlers(s_main_window, (WindowHandlers) {
   .load = main_window_load,
   .unload = main_window_unload,
 });
  window_stack_push(s_main_window, true);

 // Start animation loop
 next_animation();
}

static void deinit(void) {
 // Stop any animation in progress
 animation_unschedule_all();

 // Destroy main Window
 window_destroy(s_main_window);
}

int main(void) {
 init();
 app_event_loop();
 deinit();
}
