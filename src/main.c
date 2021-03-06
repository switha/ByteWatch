// If you wish to enable second digits uncomment all coments starting with /*!* 

#include <pebble.h>
/*offset from the top and between the bytes (digits) */
#define top_offset 20

/* spacing between digits */
#define hours_spacing 6
/* spacing from left and right of device*/
#define hours_spacing_offset 10
/* size of digit*/
#define hours_side 20
  
#define minutes_spacing 8
#define minutes_spacing_offset 7
#define minutes_side 15
  
/*to invert color scheme, switch these two*/	
#define front_color GColorClear
#define bg_color GColorBlack


	
static Window *s_main_window;

static Layer *s_canvas_layer_hours;
static Layer *s_canvas_layer_minutes;
/*static Layer *s_canvas_layer_seconds; */

static TextLayer *s_day_layer;
static TextLayer *s_date_layer;

/*!*
static void seconds_canvas_update_proc(Layer *this_layer, GContext *ctx){
  time_t temp = time(NULL);
  int seconds = localtime(&temp)->tm_sec;
  
  for(int i = 5; i >= 0; i--){
    if(seconds & 1) graphics_context_set_fill_color(ctx, front_color);
    else graphics_context_set_fill_color(ctx,bg_color);
    seconds /= 2;
    graphics_fill_rect(ctx, GRect(i*(minutes_spacing+minutes_side)+minutes_spacing_offset, 0, minutes_side, minutes_side),0,GCornerNone);
  }
  graphics_context_set_fill_color(ctx, front_color);
  for(int i = 5; i >= 0; i--){
    graphics_draw_rect(ctx, GRect(i*(minutes_spacing+minutes_side)+minutes_spacing_offset, 0, minutes_side, minutes_side));
  }
}
*/

static void minutes_canvas_update_proc(Layer *this_layer, GContext *ctx){
  time_t temp = time(NULL);
	//get the time in minutes
	int minutes = localtime(&temp)->tm_min;
	 //draw the relevant digits
  for(int i = 5; i >= 0; i--){
    if(minutes & 1) graphics_context_set_fill_color(ctx, front_color);
    else graphics_context_set_fill_color(ctx,bg_color);
    minutes /= 2;
    graphics_fill_rect(ctx, GRect(i*(minutes_spacing+minutes_side)+minutes_spacing_offset, 0, minutes_side, minutes_side),0,GCornerNone);
  }
	//and finish with the borders
  graphics_context_set_stroke_color(ctx, front_color);
  for(int i = 5; i >= 0; i--){
    graphics_draw_rect(ctx, GRect(i*(minutes_spacing+minutes_side)+minutes_spacing_offset, 0, minutes_side, minutes_side));
  }
}


static void hours_canvas_update_proc(Layer *this_layer, GContext *ctx){
	//same as minutes_canvas_update_proc
  time_t temp = time(NULL);
  int hours = localtime(&temp)->tm_hour;
  for(int i = 4; i >= 0; i--){
    if(hours & 1) graphics_context_set_fill_color(ctx, front_color);
    else graphics_context_set_fill_color(ctx,bg_color);
    hours /= 2;
    graphics_fill_rect(ctx, GRect(i*(hours_spacing+hours_side)+hours_spacing_offset, 0, hours_side, hours_side),0,GCornerNone);
  }
  
   graphics_context_set_stroke_color(ctx, front_color);
  for(int i = 4; i >= 0; i--){
        graphics_draw_rect(ctx, GRect(i*(hours_spacing+hours_side)+hours_spacing_offset, 0, hours_side, hours_side));

  }
  
}


static void update_date(){
	//get the time
	time_t temp = time(NULL);
	struct tm *time = localtime(&temp);
	//initialize the buffers, the values should never appear ...
	static char *day_buffer = "Err";
	static char *date_buffer = "FallBack";
	
	//convert the day and date to string (for formating see http://www.cplusplus.com/reference/ctime/strftime/ )
	strftime(day_buffer, 4, "%a", time);
	strftime(date_buffer, 12, "%e %b %Y", time);

	//set the to the layers
	text_layer_set_text(s_day_layer, day_buffer);
	text_layer_set_text(s_date_layer, date_buffer);
	
}

static void main_window_load(Window *window){
  //get the window and bounds
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);
  
//create the hours and minutes graphical layers
  s_canvas_layer_hours = layer_create(GRect(0, top_offset, window_bounds.size.w, hours_side));
  s_canvas_layer_minutes = layer_create(GRect(0, top_offset*2+hours_side, window_bounds.size.w, minutes_side));
  
/*!*	
	s_canvas_layer_seconds = layer_create(GRect(0, top_offset*3+hours_side+minutes_side, window_bounds.size.w,minutes_side));
  layer_add_child(window_layer, s_canvas_layer_seconds); 
 layer_set_update_proc(s_canvas_layer_seconds, seconds_canvas_update_proc);
 */
		
	//add the graphical layers to the window
  layer_add_child(window_layer, s_canvas_layer_hours);
  layer_add_child(window_layer, s_canvas_layer_minutes);
  
	//and set the drawing procedure
  layer_set_update_proc(s_canvas_layer_hours, hours_canvas_update_proc);
  layer_set_update_proc(s_canvas_layer_minutes, minutes_canvas_update_proc);
  
	//create the text layers for day and date
	s_day_layer = text_layer_create(GRect(0, 90, window_bounds.size.w-minutes_spacing_offset, 32));
	s_date_layer = text_layer_create(GRect(0,130,window_bounds.size.w-minutes_spacing_offset,30));

	//make it nice
	text_layer_set_background_color(s_day_layer, bg_color);
	text_layer_set_background_color(s_date_layer, bg_color);
	text_layer_set_text_color(s_day_layer, front_color);
	text_layer_set_text_color(s_date_layer, front_color);
	
	text_layer_set_text(s_day_layer,"MON");
	text_layer_set_text(s_date_layer,"04-NOV-2011");
	
	
	text_layer_set_font(s_day_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_font(s_date_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	
	text_layer_set_text_alignment(s_day_layer, GTextAlignmentRight);
	text_layer_set_text_alignment(s_date_layer, GTextAlignmentRight);
	//and add the layers to the window
	layer_add_child(window_layer, text_layer_get_layer(s_day_layer));
	layer_add_child(window_layer, text_layer_get_layer(s_date_layer));
	
}

static void main_window_unload(Window *window){
  layer_destroy(s_canvas_layer_hours);
  layer_destroy(s_canvas_layer_minutes);
  /*!* layer_destroy(s_canvas_layer_seconds);*/
  text_layer_destroy(s_day_layer);
  text_layer_destroy(s_date_layer);

}


/**
* updates all the display values
* called on startup
*/
static void update_time(){
	/*!*
  layer_mark_dirty(s_canvas_layer_seconds);
  */
  layer_mark_dirty(s_canvas_layer_minutes);
  layer_mark_dirty(s_canvas_layer_hours);
  update_date();
}

/**
*updates the relevant digits based on TimeUnits changed
*/
static void tick_handler(struct tm *tick_time, TimeUnits units_changed){
  /*!* if(units_changed & SECOND_UNIT){
      layer_mark_dirty(s_canvas_layer_seconds);
    }  */ 
  if(units_changed & MINUTE_UNIT){
      layer_mark_dirty(s_canvas_layer_minutes);
    }
  if(units_changed & HOUR_UNIT){
    layer_mark_dirty(s_canvas_layer_hours);
  }
  if(units_changed & DAY_UNIT){
    update_date();
  }
}


static void init(){
    s_main_window = window_create(); 
  
    window_set_window_handlers(s_main_window, (WindowHandlers){
      .load = main_window_load,
      .unload = main_window_unload
      
    });
	
	window_set_background_color(s_main_window, bg_color);
   window_stack_push(s_main_window , true);
  
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
    update_time();
}

static void deinit(){
  window_destroy(s_main_window);
}

int main(void){
  init();
  app_event_loop();
  deinit();
}