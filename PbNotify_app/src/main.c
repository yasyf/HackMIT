/*
 * Developed by Yasyf Mohamedali @ HackMIT 2013
 * https://github.com/yasyf/HackMIT
 * 
 * Adapted from https://github.com/pebble/hackmit-2013/blob/ac954efc153646a093bc0cbe1a6864d90d86980d/isstracker-http/src/http.c
 *
 */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "http.h"
#include "main.h"

#define USERID "5250a011dabae068d13ee5f4"
#define GETURL "http://yasyf.scripts.mit.edu:8080/api/notification/get/"
#define DELIVEREDURL "http://yasyf.scripts.mit.edu:8080/api/notification/delivered/"
  
PBL_APP_INFO(HTTP_UUID,
             "PbNotify", "Yasyf Mohamedali",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE); //or APP_INFO_STANDARD_APP, your call

Window window;
ScrollLayer scroll_layer; 
TextLayer source_layer;
TextLayer message_layer;
static char* source = "PbNotify";
static char* message = "No New Messages";
int error = 0;
bool confirming = false;
int counter = 0;

void start_http_request() {
  DictionaryIterator *out;
  HTTPResult result = http_out_get(GETURL USERID, 0, &out);
  if (result != HTTP_OK) {
    error = result;
    return;
  }
  result = http_out_send();
  if (result != HTTP_OK) {
    error = result;
    return;
  }
}

void start_http_request_2() {
  DictionaryIterator *out;
  http_out_get(DELIVEREDURL USERID, 0, &out);
  http_out_send();
  confirming = true;
}

void handle_http_success(int32_t request_id, int http_status, DictionaryIterator* sent, void* context) {
  if(confirming == true){
    confirming = false;
    return;
  }
  Tuple *source_tuple = dict_find(sent, 1);
  Tuple *message_tuple = dict_find(sent, 2);
  if (source_tuple && message_tuple) {
  vibes_short_pulse();
    source = source_tuple->value->cstring;
  message = message_tuple->value->cstring;
  start_http_request_2();
  error = 0;
  }
  else if (error != 0) {
  source = "Error";
  message = "Could not connect to API";
  }
  else {
  //source = "HackMIT";
  //message = "No New Messages";
  return;
  }
  text_layer_set_text(&source_layer, source);
  text_layer_set_text(&message_layer, message);
}

void handle_http_failure(int32_t request_id, int http_status, void* context) {
  error = http_status;
}

void handle_tick(AppContextRef app_ctx, PebbleTickEvent *t) {
  if(counter > 10){
    counter = 0;
    start_http_request();
  }
  else{
    counter++;
  }
  
}

void config_provider(ClickConfig **config, Window *window) {
  config[BUTTON_ID_SELECT]->click.handler = (ClickHandler) select_single_click_handler;
}

void handle_init(AppContextRef ctx) {
  (void)ctx;
  
  window_init(&window, "HackMIT");
  window_stack_push(&window, true /* Animated */);
  
  window_set_click_config_provider(&window, (ClickConfigProvider) config_provider);
  
  text_layer_init(&source_layer, GRect(-5, 0, 144, 30));
  text_layer_set_text_alignment(&source_layer, GTextAlignmentCenter);
  text_layer_set_text(&source_layer, source);
  text_layer_set_font(&source_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &source_layer.layer);
  
  scroll_layer_init(&scroll_layer, GRect(-5, 30, 155, 110));
  scroll_layer_set_click_config_onto_window(&scroll_layer, &window);
  
  text_layer_init(&message_layer, GRect(0, 0, 150, 2000));
  text_layer_set_text_alignment(&message_layer, GTextAlignmentCenter);
  text_layer_set_font(&message_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  text_layer_set_text(&message_layer, message);
  
  GSize max_size = text_layer_get_max_used_size(app_get_current_graphics_context(), &message_layer);
  scroll_layer_set_content_size(&scroll_layer, GSize(150, max_size.h));
  
  scroll_layer_add_child(&scroll_layer, &message_layer.layer);
  layer_add_child(&window.layer, (Layer*)&scroll_layer);

  start_http_request();
}

void select_single_click_handler(ClickRecognizerRef recognizer, Window *window) {
  start_http_request();
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = SECOND_UNIT
    },
    .messaging_info = {
      .buffer_sizes = {
        .inbound = 124,
        .outbound = 124,
      }
    }
  };
  HTTPCallbacks http_callbacks = {
    .failure = handle_http_failure,
    .success = handle_http_success
  };
  http_register_callbacks(http_callbacks, NULL);

  app_event_loop(params, &handlers);
}
