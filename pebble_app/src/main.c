/*
 * Adapted from https://github.com/pebble/hackmit-2013/blob/ac954efc153646a093bc0cbe1a6864d90d86980d/isstracker-http/src/http.c
 */
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "http.h"

#define USERID "5250a011dabae068d13ee5f4"
#define URL "http://yasyf.scripts.mit.edu:8080/api/notification/get/"
  
PBL_APP_INFO(HTTP_UUID,
             "HackMIT", "Yasyf Mohamedali",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;
TextLayer source_layer;
TextLayer message_layer;
static char* source = "HackMIT";
static char* message = "No New Messages";
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
  Tuple *source_tuple = dict_find(sent, 1);
  Tuple *message_tuple = dict_find(sent, 2);
  if (source_tuple && message_tuple) {
    source = source_tuple->value->cstring;
  message = message_tuple->value->cstring;
  error = 0;
  }
  else {
  source = "Error";
  message = "Could not connect to API";
  }
  text_layer_set_text(&source_layer, source);
  text_layer_set_text(&message_layer, message);
}

void handle_http_failure(int32_t request_id, int http_status, void* context) {
  error = http_status;
}

void handle_tick(AppContextRef app_ctx, PebbleTickEvent *t) {
  start_http_request();
}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "HackMIT");
  window_stack_push(&window, true /* Animated */);
  
  text_layer_init(&source_layer, GRect(0, 0, 144, 30));
  text_layer_set_text_alignment(&source_layer, GTextAlignmentCenter);
  text_layer_set_text(&source_layer, source);
  text_layer_set_font(&source_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &source_layer.layer);
  
  text_layer_init(&message_layer, GRect(0,40, 144, 30));
  text_layer_set_text_alignment(&message_layer, GTextAlignmentCenter);
  text_layer_set_text(&message_layer, message);
  text_layer_set_font(&message_layer, fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
  layer_add_child(&window.layer, &message_layer.layer);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .tick_info = {
      .tick_handler = &handle_tick,
      .tick_units = 30
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
