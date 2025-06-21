#include <displayControl.h>

const int backLightPin = 27;

TFT_eSPI tft = TFT_eSPI();
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 20];

LV_FONT_DECLARE(lv_font_montserrat_14);
LV_FONT_DECLARE(lv_font_montserrat_18);
LV_FONT_DECLARE(lv_font_montserrat_20);
LV_FONT_DECLARE(lv_font_montserrat_28);

void renderDisplay(lv_obj_t *table, EMUSerial emuSerial) {
  emu_data_t emuData = emuSerial.emu_data;

  lv_table_set_cell_value(table, 0, 1, String(emuData.RPM).c_str());
  lv_table_set_cell_value(table, 0, 3, (String(emuData.vssSpeed) + " KM/H").c_str());
  lv_table_set_cell_value(table, 1, 1, String(emuData.wboAFR).c_str());
  lv_table_set_cell_value(table, 3, 1, (String(emuData.MAP) + " KPA").c_str());
  // calculated boost, seems incorrect as it should take into considerationt baro
  lv_table_set_cell_value(table, 3, 3, (String((emuData.MAP / 100) - 1.0132f) + " BAR").c_str());
  lv_table_set_cell_value(table, 2, 1, (String(emuData.TPS) + " %").c_str());
  lv_table_set_cell_value(table, 1, 3, (String(emuData.CLT) + " °C").c_str());
  lv_table_set_cell_value(table, 4, 3, (String(emuData.IgnAngle) + " °").c_str());
  lv_table_set_cell_value(table, 4, 1, (String(emuData.injDC) + " %").c_str());
  lv_table_set_cell_value(table, 2, 3, (String(emuData.Batt) + " V").c_str());
  lv_table_set_cell_value(table, 5, 1, emuSerial.decodeCel().c_str());

  lv_obj_invalidate(table);
  lv_timer_handler();
}

// Cell alignment fix
void my_table_event_cb(lv_event_t *e) {
  lv_obj_t *table = lv_event_get_target(e);
  lv_obj_draw_part_dsc_t *dsc = (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);

  if (dsc->part == LV_PART_ITEMS) {
    uint16_t row = dsc->id / lv_table_get_col_cnt(table);
    uint16_t col = dsc->id % lv_table_get_col_cnt(table);

    if (col == 0 || col == 2) {
      dsc->label_dsc->font = &lv_font_montserrat_18;
    } else if (col == 1 || col == 3) {
      dsc->label_dsc->font = &lv_font_montserrat_20;
    }
    dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
    if ((row == 0 && col == 1) || (row == 0 && col == 3) || (row == 1 && col == 1) || (row == 1 && col == 3) || (row == 2 && col == 1) || (row == 2 && col == 3) || (row == 3 && col == 1) || (row == 3 && col == 3) ||
        (row == 4 && col == 1) || (row == 4 && col == 3)) {
      dsc->label_dsc->align = LV_TEXT_ALIGN_RIGHT;
    }
    if (row == 5 && col == 1) {
      dsc->label_dsc->align = LV_TEXT_ALIGN_CENTER;
    }
  }
}

static void table_event_cb_bg(lv_event_t *e) {
  lv_obj_t *table = lv_event_get_target(e);
  lv_obj_draw_part_dsc_t *dsc = (lv_obj_draw_part_dsc_t *)lv_event_get_param(e);

  // Ensure dsc and rect_dsc are valid
  if (!dsc || !dsc->rect_dsc) return;

  // Only modify table cell backgrounds
  if (dsc->part == LV_PART_ITEMS) {
    uint16_t row = dsc->id / lv_table_get_col_cnt(table);
    uint16_t col = dsc->id % lv_table_get_col_cnt(table);

    const char *value_str = lv_table_get_cell_value(table, row, col);

    // Check if value_str is null or empty before conversion
    float value = 0.0f;  // Default value
    if (value_str != nullptr && value_str[0] != '\0') {
      try {
        value = std::stof(value_str);  // Convert string to float safely
      } catch (...) {
        value = 0.0f;  // Handle invalid conversions
      }
    }

    // Default cell color
    lv_color_t bg_color = lv_color_make(30, 30, 30);
    lv_color_t text_color = lv_color_white();

    if (row == 0 && col == 1 && value > 7000.00) {
      bg_color = lv_color_make(0, 0, 255);
      text_color = lv_color_white();
    }
    if (row == 1 && col == 3 && value > 100.00) {
      bg_color = lv_color_make(0, 0, 255);
      text_color = lv_color_white();
    }
    if (row == 1 && col == 3 && value < 55.00 && value > 01.00) {
      bg_color = lv_color_make(0, 255, 255);
      text_color = lv_color_black();
    }
    if (row == 2 && col == 3 && value < 12.00 && value > 01.00) {
      bg_color = lv_color_make(0, 0, 255);
      text_color = lv_color_white();
    }
    if (row == 3 && col == 3 && value > 1.10) {
      bg_color = lv_color_make(0, 0, 255);
      text_color = lv_color_white();
    }
    if (row == 5 && col == 1 && value_str != nullptr && value_str[0] != '\0') {
      bg_color = lv_color_make(0, 0, 255);
      text_color = lv_color_white();
    }

    // Apply background color to the cell
    dsc->rect_dsc->bg_color = bg_color;
    dsc->rect_dsc->bg_opa = LV_OPA_COVER;
    dsc->label_dsc->color = text_color;
  }
}

// LVGL Display Flush Callback
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  uint16_t w = area->x2 - area->x1 + 1;
  uint16_t h = area->y2 - area->y1 + 1;
  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors((uint16_t *)&color_p->full, w * h, true);
  tft.endWrite();
  lv_disp_flush_ready(disp);
}

// Initialize LVGL Table
lv_obj_t *initDisplay() {
  lv_obj_t *table;

  // init TFT
  tft.init();
  pinMode(backLightPin, OUTPUT);
  digitalWrite(backLightPin, LOW);
  tft.fillScreen(TFT_DARKGREY);
  tft.setRotation(1);

  // Initialize LVGL
  lv_init();
  lv_refr_now(NULL);
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  // Setup LVGL Display Driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 320;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  // init grid
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_make(30, 30, 30), LV_PART_MAIN);

  table = lv_table_create(lv_scr_act());
  lv_obj_align(table, LV_ALIGN_CENTER, -1, -1);
  lv_obj_set_style_text_opa(table, LV_OPA_COVER, 0);
  lv_obj_clear_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scrollbar_mode(table, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_text_color(table, lv_color_white(), LV_PART_ITEMS);
  lv_obj_set_style_bg_color(table, lv_color_make(30, 30, 30), LV_PART_MAIN);
  // lv_obj_set_style_text_font(table, &lv_font_montserrat_20, LV_PART_ITEMS);

  static lv_style_t style_cell0;
  lv_style_init(&style_cell0);
  lv_style_set_pad_top(&style_cell0, 12.8);
  lv_style_set_pad_bottom(&style_cell0, 12.8);
  lv_style_set_pad_left(&style_cell0, 4);
  lv_style_set_pad_right(&style_cell0, 4);
  lv_obj_add_style(table, &style_cell0, LV_PART_ITEMS);

  lv_table_set_col_cnt(table, 4);
  lv_table_set_row_cnt(table, 6);

  lv_obj_set_style_border_width(table, 1, LV_PART_ITEMS);
  lv_obj_set_style_border_color(table, lv_color_white(), LV_PART_ITEMS);
  lv_obj_set_style_border_side(table, LV_BORDER_SIDE_FULL, LV_PART_ITEMS);

  lv_table_set_col_width(table, 0, 53);
  lv_table_set_col_width(table, 1, 105);
  lv_table_set_col_width(table, 2, 47);
  lv_table_set_col_width(table, 3, 115);

  lv_table_add_cell_ctrl(table, 5, 1, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
  lv_table_add_cell_ctrl(table, 5, 2, LV_TABLE_CELL_CTRL_MERGE_RIGHT);
  lv_table_add_cell_ctrl(table, 5, 3, LV_TABLE_CELL_CTRL_MERGE_RIGHT);

  lv_table_set_cell_value(table, 0, 0, "RPM");
  lv_table_set_cell_value(table, 0, 2, "SPD");
  lv_table_set_cell_value(table, 1, 0, "AFR");
  lv_table_set_cell_value(table, 1, 2, "CLT");
  lv_table_set_cell_value(table, 2, 0, "TPS");
  lv_table_set_cell_value(table, 2, 2, "BAT");
  lv_table_set_cell_value(table, 3, 0, "MAP");
  lv_table_set_cell_value(table, 3, 2, "BST");
  lv_table_set_cell_value(table, 4, 0, "INJ");
  lv_table_set_cell_value(table, 4, 2, "IGN");
  lv_table_set_cell_value(table, 5, 0, "CEL");

  lv_obj_add_event_cb(table, my_table_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
  lv_obj_add_event_cb(table, table_event_cb_bg, LV_EVENT_DRAW_PART_BEGIN, NULL);

  lv_timer_handler();
  digitalWrite(backLightPin, HIGH);

  return table;
}
