/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   image.h
 * Author: fluffy
 *
 * Created on August 6, 2016, 12:09 PM
 */

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <cmath>

#include <stdio.h>
#include <math.h>

#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>

#include "button.h"

namespace img {
    std::string theme;
    int screen_gap;
   
    enum IMAGE_TYPE_T {TOP_BG = 1, BOT_BG, NXT_IC, PRV_IC, ZM_IN, ZM_OUT, IMG}; 
   
    void draw_upper_screen_img(sf2d_texture *image, float loc_x, float loc_y, float scale) {   
        sf2d_draw_texture_part_scale(image, 0.0F, 0.0F, loc_x, loc_y, image->width - loc_x, image->height - loc_y, 400.0 / scale, 240.0 / scale);
    }
    
    void draw_lower_screen_img(sf2d_texture *image, float loc_x, float loc_y, float scale) {
        sf2d_draw_texture_part_scale(image, -40.0F, 0.0F, loc_x, loc_y + scale + screen_gap, image->width - loc_x, image->height - (loc_y + scale + screen_gap), 400.0 / scale, 240.0 / scale);
    }
    
    void draw_upper_screen_img_rot(sf2d_texture *image, float loc_x, float loc_y, float scale, float rot) {
        sf2d_draw_texture_part_rotate_scale(image, 200 + loc_x, 120 + loc_y, rot, 0.0F, 0.0F, image->width , image->height, 400.0 / scale, 240.0 / scale);
    }
    
    void draw_lower_screen_img_rot_r(sf2d_texture *image, float loc_x, float loc_y, float scale) {
        sf2d_draw_texture_part_rotate_scale(image, 160 + loc_x, loc_y - (screen_gap) - 120, M_PI / 2.0, 0.0F, 0.0F, image->width, image->height, 400.0 / scale, 240.0 / scale);        
    }
    
    void draw_lower_screen_img_rot_l(sf2d_texture *image, float loc_x, float loc_y, float scale) {
        sf2d_draw_texture_part_rotate_scale(image, 160 + loc_x, loc_y - (screen_gap) - 120, (3.0 * M_PI) / 2.0, 0.0F, 0.0F, image->width, image->height, 400.0 / scale, 240.0 / scale);           
    }
    
    void draw_lower_screen_prev(sf2d_texture *img, float loc_x, float loc_y, float scale) {
        // scale by which to resize the images
        float s_width  = 250.0 / (float) img->width;
        float s_height = 150.0 / (float) img->height;
        
        // half the scaled size of the image
        float half_width  = (s_width * img->width);
        float half_height = (s_height * img->height);
        
        // coords that place the image in the center of the screen
        float midpoint_x = 190.0 - (half_width  / 2);
        float midpoint_y = 120.0 - (half_height / 2);
        
        // coords for drawing the rectangle
        int sc_x0 = (int) (loc_x * s_width ) + midpoint_x;
        int sc_y0 = (int) (loc_y * s_height) + midpoint_y;
        
        float sc_x1 = (int) sc_x0 + (scale * s_width );
        float sc_y1 = (int) sc_y0 + (scale * s_height);
        
        // color of rectangle
        u32 color = RGBA8(0x40, 0x40, 0x40, 0xFF);
        
        sf2d_draw_texture_scale(img, midpoint_x, midpoint_y, s_width, s_height);
        
        // top line
        sf2d_draw_line(sc_x0, sc_y0, sc_x1, sc_y0, 10.0, color);
        
        // left line
        sf2d_draw_line(sc_x0, sc_y0, sc_x0, sc_y1, 10.0, color);
        
        // right line
        sf2d_draw_line(sc_x1, sc_y0, sc_x1, sc_y1, 10.0, color);
        
        // bottom line        
        //printf("Drawing");
        sf2d_draw_line(sc_x0, sc_y1, sc_x1, sc_y1, 10.0, color);
    }
   
   void draw_screens_os(sf2d_texture *img, float loc_x, float loc_y, float scale, btn::Button *b1, btn::Button *b2, btn::Button*b3, btn::Button *b4, btn::Button *b5, btn::Button *b6) {
       u8 is_2DS;
       CFGU_GetModelNintendo2DS(&is_2DS);
       
       if (is_2DS == 0) {
           screen_gap = 20;
       } else {
           screen_gap = 40;
       }
       
       // upper screen
        sf2d_start_frame(GFX_TOP, GFX_LEFT);
        draw_upper_screen_img(img, loc_x, loc_y, scale);
        sf2d_end_frame();

        // lower screen
        sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        draw_lower_screen_prev(img, loc_x, loc_y, scale);
        
        b1->draw_button();
        b2->draw_button();
        b3->draw_button();
        b4->draw_button();
        b5->draw_button();
        b6->draw_button();
        
        sf2d_end_frame();
    }
   
   void draw_screens_ts (sf2d_texture *img, float loc_x, float loc_y, float scale, btn::Button *b1, btn::Button *b2, btn::Button *b3, btn::Button *b4, btn::Button *b5, btn::Button *b6) {
       u8 is_2DS;
       CFGU_GetModelNintendo2DS(&is_2DS);
       
       if (is_2DS == 0) {
           screen_gap = 20;
       } else {
           screen_gap = 40;
       }
       // upper screen
       sf2d_start_frame(GFX_TOP, GFX_LEFT);
       draw_upper_screen_img(img, loc_x, loc_y, scale);
       sf2d_end_frame();
       
       // lower screen
       sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
       draw_lower_screen_img(img, loc_x, loc_y, scale);
       
       b1->draw_button();
       b2->draw_button();
       b3->draw_button();
       b4->draw_button();
       b5->draw_button();
       b6->draw_button();
       sf2d_end_frame();
   }
   
   void draw_screens_tsr (sf2d_texture *img, float loc_x, float loc_y, float scale, btn::Button *b1, btn::Button *b2, btn::Button *b3, btn::Button *b4, btn::Button *b5, btn::Button *b6) {
       u8 is_2DS;
       CFGU_GetModelNintendo2DS(&is_2DS);
       
       if (is_2DS == 0) {
           screen_gap = 20;
       } else {
           screen_gap = 40;
       }
       
       // upper screen
       sf2d_start_frame(GFX_TOP, GFX_LEFT);
       draw_upper_screen_img_rot(img, loc_x, loc_y, scale, (M_PI) / 2.0);
       sf2d_end_frame();
       
       sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
       draw_lower_screen_img_rot_r(img, loc_x, loc_y, scale);
       
       b1->draw_button(M_PI / 2.0);
       b2->draw_button(M_PI / 2.0);
       b3->draw_button(M_PI / 2.0);
       b4->draw_button(M_PI / 2.0);
       b5->draw_button(M_PI / 2.0);
       b6->draw_button(M_PI / 2.0);
       sf2d_end_frame();
   }
   
   void draw_screens_tsl(sf2d_texture *img, float loc_x, float loc_y, float scale, btn::Button *b1, btn::Button *b2, btn::Button *b3, btn::Button *b4, btn::Button *b5, btn::Button *b6) {
       u8 is_2DS;
       CFGU_GetModelNintendo2DS(&is_2DS);
       
       if (is_2DS) {
           screen_gap = 20;
       } else {
           screen_gap = 40;
       }
       
       sf2d_start_frame(GFX_TOP, GFX_LEFT);
       draw_upper_screen_img_rot(img, loc_x, loc_y, scale, (3.0 * M_PI) / 2.0);
       sf2d_end_frame();
       
       sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
       draw_lower_screen_img_rot_l(img, loc_x, loc_y, scale);
       
       b1->draw_button((3.0 * M_PI) / 2.0);
       b2->draw_button((3.0 * M_PI) / 2.0);
       b3->draw_button((3.0 * M_PI) / 2.0);
       b4->draw_button((3.0 * M_PI) / 2.0);
       b5->draw_button((3.0 * M_PI) / 2.0);
       b6->draw_button((3.0 * M_PI) / 2.0);
       sf2d_end_frame();
   }
}


#endif /* IMAGE_H */

