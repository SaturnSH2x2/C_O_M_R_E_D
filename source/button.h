/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   button.h
 * Author: fluffy
 *
 * Created on August 13, 2016, 10:39 PM
 */

#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <exception>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>

using namespace std;

namespace btn {
    class Button {
    public:
        Button(std::string theme, std::string file, void (*cb)(), float px, float py);
        ~Button();
        
        bool button_touch_cb(int tx, int ty);
        
        void draw_button();
        void draw_button(float rot);
        
        sf2d_texture *tex;
        void (*callback)();
        
        float px;
        float py;
    };
    
    Button::Button(std::string theme, std::string file, void (*cb)(), float px, float py) {
        std::string filename = "/data/C_O_M_R_E_D/themes/" + theme + "/" + file;
        const char *filename_c = filename.c_str();
        //int i = 0;
        
        //printf (filename_c);
        //printf("\n");
        
        //while(i < 50000000) {
        //    i += 1;
        //}
        
        this->tex = sfil_load_PNG_file(filename_c, SF2D_PLACE_RAM);
        this->callback = cb;
        
        this->px = px;
        this->py = py;
        
        //printf(this->tex == NULL);
        //printf("\n");
    }
    
    Button::~Button() {
        sf2d_free_texture(this->tex);
    }
    
    bool Button::button_touch_cb(int tx, int ty) {
        float min_x_coord = this->px;
        float min_y_coord = this->py;
        
        float max_x_coord = this->px + this->tex->width;
        float max_y_coord = this->py + this->tex->height;
        
        float ix;
        float iy;
        
        for (ix = min_x_coord; ix < max_x_coord; ix++) {
            for (iy = min_y_coord; iy < max_y_coord; iy++) {
                if ((int) ix == tx && (int) iy == ty) {
                    this->callback();
                    return true;
                }
            }
        }
        
        return false;
    }
    
    void Button::draw_button() {
        sf2d_draw_texture(this->tex, (int)this->px, (int)this->py);
    }
    
    void Button::draw_button(float rot) {
        sf2d_draw_texture_part_rotate_scale(this->tex, (int)this->px + (this->tex->width / 2), (int)this->py + (this->tex->height / 2) + 1.0, rot, 0, 0, this->tex->width, this->tex->height, 1.0, 1.0);
    }
}

#endif /* BUTTON_H */