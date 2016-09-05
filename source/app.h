/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   app.h
 * Author: fluffy
 *
 * Created on August 6, 2016, 12:09 PM
 */

#ifndef APP_H
#define APP_H

#include <iostream>

#include <stdlib.h>

#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>

#include "image.h"
#include "button.h"

using namespace btn;

namespace app {
    enum CALLBACK_T {NADA = 0, PREV, NEXT, MENU, SWITCH_MODE, SWITCH_PAGE};
    enum VIEWMODE_T {ONE_SCREEN, TWO_SCREEN, TWO_SCREEN_L, TWO_SCREEN_R};
    
    const int IN  = 0;
    const int OUT = 1;
    
    CALLBACK_T function_cb = NADA;
    
    int tx, ty;
    int ptx = 0, pty = 0;
        
    float loc_x = 0.0;
    float loc_y = 0.0;
    float scale = 400.0;
        
    const float SPEED = 8.0;
    const float SC_SPEED = 10.0;
    
    bool is_button_touched;
    
    CALLBACK_T run_img(sf2d_texture *image, std::string theme, VIEWMODE_T &mode);
    
    void zoom(int func) {
        switch (func) {
            case IN:
                scale -= SC_SPEED;
                break;
            case OUT:
                scale += SC_SPEED;
                break;
            default:
                //printf("PANIC\n");
                break;
        };
        
        if (scale < 150.0)  scale = 150.0;
        if (scale > 900.0)  scale = 900.0;
        
        return;
    }
    
    void zoom_in() {
        zoom(IN);
    }
    
    void zoom_out() {
        zoom(OUT);
    }
    
    void next_pg() {
        function_cb = NEXT;
    }
    
    void prev_pg() {
        function_cb = PREV;
    }
    
    void to_menu() {
        function_cb = MENU;
    }
    
    void switch_mode() {
        function_cb = SWITCH_MODE;
    }
    
    void switch_page() {
        function_cb = SWITCH_PAGE;
    }
    
    /// TODO:  fix this?  touch support is a little weird
    void convert_coords(sf2d_texture *img, int tx, int ty) {
        float s_width  = (float) img->width / 400.0;
        float s_height = (float) img->height / 240.0;
        
        float half_width  = ((250.0 / (float) img->width) * img->width);
        float half_height = ((150.0 / (float) img->height) * img->height);
      
        loc_x = (tx * s_width)  - (half_width / 2);
        loc_y = (ty * s_height) - (half_height / 2);
    }
  
    PrintConsole top, bottom;
    
    CALLBACK_T image_one_screen(sf2d_texture *image, std::string theme, VIEWMODE_T mode) {
        switch (mode) {
            case (TWO_SCREEN_R):   // TWO_SCREEN_L/R
                loc_x = -100;
                loc_y = 160;
                break;
            case (TWO_SCREEN_L):
                loc_x = 100;
                loc_y = 100;
                break;
            default:
                loc_x = 0;
                loc_y = 0;
                break;
        }
        
        scale = 400.0;
        
        touchPosition touch;
        
        is_button_touched = false;
        bool button_touch_prev_frame = false;
        
        function_cb = NADA;
              
        // yes, the dimensions of the buttons are hard-coded.  make sure you have your image sizes right.
        btn::Button zm_in_btn (     theme, "zoom_in.png",     zoom_in,     0.0F,   240.0 - 35.0F);
        btn::Button zm_out_btn(     theme, "zoom_out.png",    zoom_out,    40.0F,  240.0F - 35.0F);
        btn::Button prv_pg_btn(     theme, "prev_pg.png",     prev_pg,     80.0F, 240.0F - 35.0F);
        btn::Button nxt_pg_btn(     theme, "next_pg.png",     next_pg,     120.0F, 240.0F - 35.0F);
        btn::Button menu_btn  (     theme, "menu.png",        to_menu,     320.0F - 35.0F, 240.0F - 35.0F);
        btn::Button switch_mode_btn(theme, "switch_mode.png", switch_mode, 160.0F, 240.0F - 35.0F);
        btn::Button switch_page_btn(theme, "switch_page.png", switch_page, 200.0F, 240.0F - 35.0F);
        
        sf2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));
        
        //start_loop:
    
        while (aptMainLoop()) {
            hidScanInput();
            
            u32 kHeld = hidKeysHeld();
            u32 kDown = hidKeysDown();
            
            hidTouchRead(&touch);
            tx = touch.px;
            ty = touch.py;
            
            if ((kDown & KEY_TOUCH) || button_touch_prev_frame == true) {
                // inefficient, i know
                is_button_touched = zm_in_btn.button_touch_cb(tx, ty);
                is_button_touched = (is_button_touched || zm_out_btn.button_touch_cb(tx, ty));
                is_button_touched = (is_button_touched || nxt_pg_btn.button_touch_cb(tx, ty));
                is_button_touched = (is_button_touched || prv_pg_btn.button_touch_cb(tx, ty));
                is_button_touched = (is_button_touched || menu_btn.button_touch_cb(tx, ty));
                is_button_touched = (is_button_touched || switch_mode_btn.button_touch_cb(tx, ty));
                is_button_touched = (is_button_touched || switch_page_btn.button_touch_cb(tx, ty));
                
                button_touch_prev_frame = is_button_touched;                
            } 
                        
            if (kHeld & KEY_TOUCH) {
                if (not is_button_touched) {
                    if (mode == ONE_SCREEN) convert_coords(image, tx, ty);
                    if (mode == TWO_SCREEN ) {
                        if (not (kDown & KEY_TOUCH)) {
                            loc_x -= (tx - ptx);
                            loc_y -= (ty - pty);
                        }
                    } else if (mode == TWO_SCREEN_R) {
                        if (not (kDown & KEY_TOUCH)) {
                            loc_y += (ty - pty);
                            loc_x += (tx - ptx);
                        }
                    } else if (mode == TWO_SCREEN_L) {
                        if (not (kDown & KEY_TOUCH)) {
                            loc_y += (ty - pty);
                            loc_x += (tx - ptx);
                        }
                    }
                }     
            }
            
            ptx = tx;
            pty = ty;
        
            // respond to input
            if (mode == ONE_SCREEN || mode == TWO_SCREEN) {
                if (kHeld & KEY_CPAD_UP)    loc_y -= SPEED;
                if (kHeld & KEY_CPAD_DOWN)  loc_y += SPEED;
                if (kHeld & KEY_CPAD_LEFT)  loc_x -= SPEED;
                if (kHeld & KEY_CPAD_RIGHT) loc_x += SPEED;
                
                if (kDown & KEY_DLEFT)  prev_pg();
                if (kDown & KEY_DRIGHT) next_pg();                
            } else if (mode == TWO_SCREEN_R) {
                if (kHeld & KEY_CPAD_UP)    loc_y += SPEED;
                if (kHeld & KEY_CPAD_DOWN)  loc_y -= SPEED;
                if (kHeld & KEY_CPAD_LEFT)  loc_x += SPEED;
                if (kHeld & KEY_CPAD_RIGHT) loc_x -= SPEED;
                
                if (kDown & KEY_DUP)  prev_pg();
                if (kDown & KEY_DDOWN) next_pg(); 
            } else if (mode == TWO_SCREEN_L) {
                if (kHeld & KEY_CPAD_UP)    loc_y += SPEED;
                if (kHeld & KEY_CPAD_DOWN)  loc_y -= SPEED;
                if (kHeld & KEY_CPAD_LEFT)  loc_x += SPEED;
                if (kHeld & KEY_CPAD_RIGHT) loc_x -= SPEED;
                
                if (kDown & KEY_DDOWN)  prev_pg();
                if (kDown & KEY_DUP) next_pg(); 
            }
            
            if (kHeld & KEY_Y) zoom(IN);
            if (kHeld & KEY_X) zoom(OUT);
            
            if (kDown & KEY_SELECT) function_cb = SWITCH_MODE;
            //if (kHeld & KEY_START) break;
            
            // correct numbers to keep things in check
            if (loc_x < 0.0 - image->width)            loc_x = 0.0 - image->width;
            if (loc_x > image->width)                  loc_x = image->width;
            
            if (loc_y < 0.0 - image->height)             loc_y = 0.0 - image->height;
            if (loc_y > image->height)                   loc_y = image->height;
            
            //printf("\x1b[0;0Hloc_x:  %d, loc_y: %d", loc_x, loc_y);
            
            switch (mode) {
                case ONE_SCREEN:
                    img::draw_screens_os(image, loc_x, loc_y, scale, &zm_in_btn, &zm_out_btn, &nxt_pg_btn, &prv_pg_btn, &menu_btn, &switch_mode_btn);
                    break;
                case TWO_SCREEN:
                    img::draw_screens_ts(image, loc_x, loc_y, scale, &zm_in_btn, &zm_out_btn, &nxt_pg_btn, &prv_pg_btn,  &switch_mode_btn, &menu_btn, &switch_page_btn);
                    break;
                case TWO_SCREEN_R:
                    img::draw_screens_tsr(image, loc_x, loc_y, scale, &zm_in_btn, &zm_out_btn, &nxt_pg_btn, &prv_pg_btn,  &switch_mode_btn, &menu_btn, &switch_page_btn);
                    break;
                case TWO_SCREEN_L:
                    img::draw_screens_tsl(image, loc_x, loc_y, scale, &zm_in_btn, &zm_out_btn, &nxt_pg_btn, &prv_pg_btn,  &switch_mode_btn, &menu_btn, &switch_page_btn);
                    break;
                default:
                    break;              
            };

            
            sf2d_swapbuffers();
            
            if (function_cb != NADA) break;
        }
        
        //delete image;
        return function_cb;
    }
    
    CALLBACK_T run_img (sf2d_texture *image, std::string theme, VIEWMODE_T &mode) {
        CALLBACK_T cb;
        while (true) {
            cb = image_one_screen(image, theme, mode);
            //consoleInit(GFX_BOTTOM, &bottom);
            //consoleClear();
            //printf("Exited loop.\n");
        
            if (cb == SWITCH_MODE) {
                if (mode == ONE_SCREEN) {   
                    mode = TWO_SCREEN;
                    continue;
                }
                else if (mode == TWO_SCREEN) {
                    mode = TWO_SCREEN_L; 
                    continue;
                }    
                else if (mode == TWO_SCREEN_L){
                    mode = TWO_SCREEN_R; 
                    continue;
                }
                else if (mode == TWO_SCREEN_R) {
                    mode = TWO_SCREEN;   
                    continue;
                }
            } else {
                //printf("Exiting loop.\n");
                //sf2d_free_texture(image);
                //delete image;
                //printf("Freed texture\n");
                return cb;
            }
        }

    }
    
    CALLBACK_T run_img (std::string path, char ext[16], std::string theme, VIEWMODE_T &mode) {
        sf2d_texture *image;
        CALLBACK_T cb;
        
        if (strcmp(ext, ".png") == 0 || strcmp(ext, ".PNG") == 0) {
                image = sfil_load_PNG_file(path.c_str(), SF2D_PLACE_RAM);
            } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".JPG") == 0) {
                image = sfil_load_JPEG_file(path.c_str(), SF2D_PLACE_RAM);
            } else if (strcmp(ext, ".bmp") == 0 || strcmp(ext, ".BMP") == 0) {
                image = sfil_load_BMP_file(path.c_str(), SF2D_PLACE_RAM);
            } else {
                printf("Error!\n");
                return MENU;
            }
        
        cb = run_img(image, theme, mode);
        sf2d_free_texture(image);
        
        return cb;
    }
}


#endif /* APP_H */

