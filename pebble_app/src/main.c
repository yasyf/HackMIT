#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "http.h"


PBL_APP_INFO(HTTP_UUID,
             "HackMIT", "Yasyf Mohamedali",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_STANDARD_APP);

Window window;
TextLayer source_layer;
TextLayer message_layer;


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
