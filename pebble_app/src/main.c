/*
 * Adapted from https://github.com/pebble/hackmit-2013/blob/ac954efc153646a093bc0cbe1a6864d90d86980d/isstracker-http/src/http.c
 */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "http.h"

#define USERID "5250a011dabae068d13ee5f4"
#define URL "http://hackmit.yasyf.scripts.mit.edu/pebble_api/api/notification/get/"
  
PBL_APP_INFO(HTTP_UUID,
             "HackMIT", "Yasyf Mohamedali",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;
TextLayer source_layer;
TextLayer message_layer;
static char* source;
static char* message;
int error = 0;

void start_http_request() {
  DictionaryIterator *out;
  HTTPResult result = http_out_get(URL USERID, 0, &out);
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

void handle_http_success(int32_t request_id, int http_status, DictionaryIterator* sent, void* context) {
  Tuple *source_tuple = dict_find(sent, 3);
  Tuple *message_tuple = dict_find(sent, 1);
  if (source_tuple && message_tuple) {
    memcpy(source, source_tuple->value->cstring, source_tuple->length);
  memcpy(message, message_tuple->value->cstring, message_tuple->length);
  }
  error = 0;
}

void handle_http_failure(int32_t request_id, int http_status, void* context) {
  error = http_status;
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "HackMIT");
  window_stack_push(&window, true /* Animated */);
  
  text_layer_init(&source_layer, GRect(0, 0, 144, 30));
  text_layer_set_text_alignment(&source_layer, GTextAlignmentCenter);
  text_layer_set_text(&source_layer, "Source");
  text_layer_set_font(&source_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &source_layer.layer);
  
  text_layer_init(&message_layer, GRect(0,40, 144, 30));
  text_layer_set_text_alignment(&message_layer, GTextAlignmentCenter);
  text_layer_set_text(&message_layer, "Message");
  text_layer_set_font(&message_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &message_layer.layer);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init
  };
  app_event_loop(params, &handlers);
}
