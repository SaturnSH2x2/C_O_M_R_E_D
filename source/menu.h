/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   menu.h
 * Author: fluffy
 *
 * Created on August 17, 2016, 12:53 PM
 */

#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <exception>
#include <string>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>
#include <sftd.h>

#include "./sdfs.h"

#include "rf.h"
#include "app.h"
#include "button.h"

#include "clear_png.h"

namespace menu {
    PrintConsole top, bottom;
    
    const int LIST_LIMIT   = 1000;
    const int MAX_ONSCREEN = 20;
    
    enum MM_CALLBACK_T {NADA, READ, DOWNLOAD, OPTIONS, EXIT};
    MM_CALLBACK_T menu_cb;        
    
    sftd_font *main_font;
    
    sf2d_texture *image;
    
    // thanks, Marcus!
    struct directory_listing
    {
        u32 filecount;
        char files[LIST_LIMIT][262];
        char extensions[LIST_LIMIT][16];
        int max_entries;
    
        void Clear()
        {
            memset(files, 0, sizeof(files));
            memset(extensions, 0, sizeof(extensions));
            filecount = 0;
        }
    };
    
    directory_listing *mlisting = new directory_listing;
    
    struct Theme {
        sf2d_texture *top_scr_bg_mm;
        sf2d_texture *bot_scr_bg_mm;
        sf2d_texture *top_scr_bg_opt;
        
        sftd_font *font;
        
        btn::Button *read_btn;
        btn::Button *dwnld_btn;
        btn::Button *opt_btn;
        btn::Button *exit_btn;
        
        std::string theme_name;
    };
    
    Theme main_theme;
    
    sf2d_texture *load_theme_image(std::string theme, std::string filename);        
    
    void free_theme(Theme theme) {
        sf2d_free_texture(theme.top_scr_bg_mm);
        sf2d_free_texture(theme.bot_scr_bg_mm);
        sf2d_free_texture(theme.top_scr_bg_opt);
        
        sftd_free_font(theme.font);
        
        delete theme.read_btn;
        delete theme.dwnld_btn;
        delete theme.opt_btn;
        delete theme.exit_btn;
    }
    
    std::string to_string(int i) {
        std::stringstream ss;
        std::string result;
        
        ss << i;
        ss >> result;
        
        return result;
    }
    
    void reboot_sf2d() {
        //printf("Prepare to perform operations...\n");
        
        sf2d_fini();
        sf2d_init();
        
        //printf("Operations finished successfulyy.\n");
    }
    
    void flush_out_sf2d() {
        sf2d_texture *flush;
        int i;
        
        for (i = 0; i < 20; i++) {
            flush = sfil_load_PNG_buffer(clear_png, SF2D_PLACE_RAM);
            sf2d_free_texture(flush);
        }
        
        //if (flush) delete flush;
        
    }
    
    void read() {
        menu_cb = READ;
    }
    
    void download() {
        menu_cb = DOWNLOAD;
    }
    
    void options() {
        menu_cb = OPTIONS;
    }
    
    void exit() {
        menu_cb = EXIT;
    }
    
    Theme load_theme(std::string theme_name) {
        Theme theme;
        
        theme.top_scr_bg_mm = load_theme_image(theme_name,  "mm_top_scr_bg.png");
        theme.bot_scr_bg_mm = load_theme_image(theme_name,  "mm_bot_scr_bg.png");
        theme.top_scr_bg_opt = load_theme_image(theme_name, "opt_top_scr_bg.png");
        
        theme.font = sftd_load_font_file(("/data/C_O_M_R_E_D/themes/" + theme_name + "/font.ttf").c_str());
        
        theme.read_btn  = new btn::Button (theme_name, "read_btn.png", read, 20, 20);
        theme.dwnld_btn = new btn::Button (theme_name, "download_btn.png", download, 20, 60);
        theme.opt_btn   = new btn::Button (theme_name, "options_btn.png", options, 20, 100);
        theme.exit_btn  = new btn::Button (theme_name, "exit_btn.png", exit, 20, 140);
        
        theme.theme_name = theme_name;
        
        return theme;
    }
    
    int number_of_entries(char list[][262], const int LIST_LIMIT) {
        int i;
        int number_of_unempty_entries = 0;
        
        for (i = 0; i < LIST_LIMIT; i++) {
            if (strcmp(list[i], "\0") == 0) {
                break;
            } else {
                number_of_unempty_entries += 1;
            }    
        }
        
        return number_of_unempty_entries;
    }
    
    void display_directories(char list[][262], const int LIST_LIMIT, const int MAX_ONSCREEN, int index, int cursor, const char *arg) {
        //consoleClear();
            
        int scr_indx = 0;
        //int pscr_indx = scr_indx + 1;
        int i;
        
        std::string buf = "";
        
        //printf("\x1b[0;0H");
        sftd_draw_text(main_theme.font, 4, 0, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, arg);
        sftd_draw_text(main_theme.font, 4, 15, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, "----------------------------------");
            
        for (i = index; i < index + MAX_ONSCREEN; i++) {
            buf = "";
            
            if (cursor == i) {
                buf = "> " + buf;
            } else {
                buf = buf + "  ";
            }
            
            buf = buf + list[i];
            
            sftd_draw_text(main_theme.font, 4, ((i - index) * 15) + 30, RGBA8(0xFF, 0xFF, 0xFF, 0xFF), 12, buf.c_str());
                
            scr_indx += 1;
            //pscr_indx = scr_indx + 1;
        }
    }
    
    bool is_comic_directory(std::string path, std::string &zeroes) {
        const int ext_list_size = 6;
        
        std::string cur_file;
        std::string cur_path;
        std::string contents;
        //std::string zeroes = "";
        
        std::string ext_list[ext_list_size];
        ext_list[0] = "1.jpg";
        ext_list[1] = "1.JPG";
        ext_list[2] = "1.png";
        ext_list[3] = "1.PNG";
        ext_list[4] = "1.bmp";
        ext_list[5] = "1.BMP";
        printf("Variables initialized.\n");
        
        for (int i = 0; i < ext_list_size; i++) {
            cur_file = ext_list[i];
            zeroes = "";
            
            for (int i2 = 0; i2 < 10; i2++) {
                //printf(cur_file.c_str());
                //printf("\n");
                //printf(zeroes.c_str());
                //printf("\n");                
                
                cur_path = path + "/" + cur_file ;
                //printf("Looking for: ");
                //printf(cur_path.c_str());
                //printf("\n");
                if (rf::does_file_exist(cur_path.c_str())) {
                    //printf("File found!\n");
                    return true;
                }    
                zeroes = "0" + zeroes;
                cur_file = "0" + cur_file;
            }
        }
        
        //printf("File not found!\n");
        return false;
    }
    
    void switch_dir(directory_listing *listing, char *path, char *ext) {
        sdfs_init(path); 
        listing->Clear();
        
        sdfs_scandir("/", listing->files, listing->extensions, &listing->filecount, ext);
        listing->max_entries = number_of_entries(listing->files, LIST_LIMIT);
    }
    
    sf2d_texture *load_theme_image(std::string theme, std::string filename) {
        std::string path = "/data/C_O_M_R_E_D/themes/" + theme + "/" + filename;
        //printf(path_c);
        //printf("\n");
        
        sf2d_texture *tex = sfil_load_PNG_file((char*) path.c_str(), SF2D_PLACE_RAM);
        return tex;
    }
    
    void display_image(sf2d_texture *tex) {
        sf2d_draw_texture(tex, 0, 0);
    }
    
    void read_comic(std::string path, std::string zeroes, std::string theme) {
        
        app::CALLBACK_T cb;
        app::VIEWMODE_T vm; vm = app::TWO_SCREEN;
        
        char *cpath = new char;
        cpath = (char*) path.c_str();
        
        char *img_buffer;
        unsigned long img_size;

        mlisting->Clear();
        switch_dir(mlisting, cpath, 0);
        
        int counter = 1;
        //int i = 0;
        
        std::string full_path;   
        
        std::string chr_counter;
        std::string chr_counter_p1;
        std::string chr_counter_m1;
        
        bool is_loop = true;
        
        flush_out_sf2d();
        free_theme(main_theme);
        sf2d_swapbuffers();        
        
        while (true) {
            chr_counter =    to_string(counter);
            chr_counter_p1 = to_string(counter + 1);
            chr_counter_m1 = to_string(counter - 1);
            
            full_path = path + "/" + zeroes + chr_counter + mlisting->extensions[counter];
            sf2d_pool_reset();
            
            img_buffer = rf::read_bin(full_path);
            img_size   = rf::read_size(full_path);
            
            if (cb != app::SWITCH_MODE) flush_out_sf2d();
            
            if (strcmp(mlisting->extensions[counter], ".png") == 0 || strcmp(mlisting->extensions[counter], ".PNG") == 0) {
                image = sfil_load_PNG_buffer(img_buffer, SF2D_PLACE_RAM);
            } else if (strcmp(mlisting->extensions[counter], ".jpg") == 0 || strcmp(mlisting->extensions[counter], ".JPG") == 0) {
                image = sfil_load_JPEG_buffer(img_buffer, img_size, SF2D_PLACE_RAM);
            } else if (strcmp(mlisting->extensions[counter], ".bmp") == 0 || strcmp(mlisting->extensions[counter], ".BMP") == 0) {
                image = sfil_load_BMP_buffer(img_buffer, SF2D_PLACE_RAM);
            } else {
                printf("Error!\n");
                return;
            }
            
            //sf2d_pool_reset();
            
            //sf2d_draw_texture(image, 0, 0);
            //sf2d_swapbuffers();
            
            //while (i < 50000) {
            //    i++;
            //}            
            
            //image = sfil_load_PNG_file("/data/C_O_M_R_E_D/test.png", SF2D_PLACE_RAM);
            
            cb = app::run_img(image, theme, vm, false);
            
            switch (cb) {
                case app::NEXT:
                    if (rf::does_file_exist(path + "/" + zeroes + chr_counter_p1 + mlisting->extensions[counter + 1]) != false) counter += 1;
                    break;
                case app::PREV:
                    if (rf::does_file_exist(path + "/" + zeroes + chr_counter_m1 + mlisting->extensions[counter - 1]) != false) counter -= 1;
                    break;
                case app::MENU:
                    is_loop = false;
                    break;
                default:
                    break;
            };
            
            C3D_TexDelete(&image->tex);
            delete img_buffer;
           
            if (is_loop == false) break;
        }
        
        mlisting->Clear();
        sdfs_free();        
        
        printf("Exited loop.\n");
   
        if (cpath) {
            delete cpath;
            printf("Clear path.\n");
        } else {
            printf("No path to delete.\n");
        }
        
        if (img_buffer) {
            delete img_buffer;
        } 
        
        load_theme(theme);

        printf("Exiting function...\n");
    }
    
    void main_menu(std::string theme) {        
        menu_cb = NADA;
        
        int tx, ty;
        
        touchPosition touch;
        
        while (aptMainLoop()) {
            hidScanInput();
            
            u32 kDown = hidKeysDown();
            //u32 kHeld = hidKeysHeld();
            
            hidTouchRead(&touch);                       
            
            sf2d_start_frame(GFX_TOP, GFX_LEFT);
            display_image(main_theme.top_scr_bg_mm);
            sf2d_end_frame();
            
            sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            display_image(main_theme.bot_scr_bg_mm);
            main_theme.read_btn->draw_button();
            main_theme.dwnld_btn->draw_button();
            main_theme.opt_btn->draw_button();
            main_theme.exit_btn->draw_button();
            sf2d_end_frame();
            
            sf2d_swapbuffers();
            
             if (kDown & KEY_TOUCH) {
                tx = touch.px;
                ty = touch.py;
                
                main_theme.read_btn->button_touch_cb (tx, ty);
                main_theme.dwnld_btn->button_touch_cb(tx, ty);
                main_theme.opt_btn->button_touch_cb  (tx, ty);
                main_theme.exit_btn->button_touch_cb (tx, ty);
            }
            
            if (kDown & KEY_B) menu_cb = EXIT;
            
            if (menu_cb != NADA) break;
        }
    }
    
    void read_cb(std::string theme) {
        //sf2d_texture *top_scr_bg = load_theme_image(theme, "opt_top_scr_bg.png");
        //consoleInit(GFX_BOTTOM, &bottom);    
        //consoleClear();
        
        std::string path = "/data/C_O_M_R_E_D/comics";
        //char *cwd  = "/";
       
        int index = 0;
        int cursor = 0;
        //int prev_cursor;
        
        std::string zeroes;
        std::string select;
        
        bool is_comdir;
        bool selected = false;
        
        //directory_listing* listing = new directory_listing;
        
        mlisting->Clear();
        switch_dir(mlisting, (char*) path.c_str(), (char*)-1);
        
        //sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
        //sf2d_end_frame();
        
        while (aptMainLoop()) {
            hidScanInput();
            
            u32 kDown = hidKeysDown();
            //u32 kHeld = hidKeysHeld(); 
            
            if (kDown & KEY_B) break;
            
            if (kDown & KEY_UP)   cursor -= 1;
            if (kDown & KEY_DOWN) cursor += 1;                        
            
            if (cursor > index + MAX_ONSCREEN) index += 1;
            if (cursor < index)                index -= 1;
            
            if (cursor < 0) {
                index = 0;  cursor = 0;
            }
            
            if (cursor > mlisting->max_entries - 1) {
                cursor = mlisting->max_entries - 1;
                if (index > 0) index -= 1;
            }
            
            select = path;
            select = select + mlisting->files[cursor];
            
            sf2d_start_frame(GFX_TOP, GFX_LEFT);
            display_image(main_theme.top_scr_bg_opt);
            sf2d_end_frame();           
            
            sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            display_directories(mlisting->files, LIST_LIMIT, MAX_ONSCREEN, index, cursor, "Select a comic/manga.");
            sf2d_end_frame();
            
            //printf("\x1b[15;0H%d  %d  %d", listing->max_entries, index, cursor);
            //if (prev_cursor != cursor) consoleClear();
            
            sf2d_swapbuffers();
            
            if (kDown & KEY_A) {
                selected = true;
                break;
            } 
        }
        
        sdfs_free();
        
        if (selected) {
            is_comdir = is_comic_directory(select, zeroes);
                
            if (is_comdir) {                
                read_comic(select, zeroes, theme);
            }
        }
    }
    
    void download_cb() {
        //sf2d_texture *top_scr_bg = load_theme_image(theme, "opt_top_scr_bg.png");
        //consoleInit(GFX_BOTTOM, &bottom);
        //consoleClear();
        
        u32 kDown;
        
        while (aptMainLoop()) {
            hidScanInput();
            
            kDown = hidKeysDown();
            //u32 kHeld = hidKeysHeld();
            
            if (kDown & KEY_B) break;
            
            sf2d_start_frame(GFX_TOP, GFX_LEFT);
            display_image(main_theme.top_scr_bg_opt);
            sf2d_end_frame();
            
            sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            sf2d_end_frame();
            
            //printf("\x1b[0;0Hayy sample text");
            
            sf2d_swapbuffers();
        }
        
        //printf("Deleting top_scr_bg pointer...\n");
        //sf2d_free_texture(top_scr_bg); //delete top_scr_bg;
        //printf("Deleted top_scr_bg successfully.\n");
        //reboot_sf2d();
        //consoleInit(GFX_BOTTOM, &bottom);
        //printf("Ended function.\n");
        //consoleExit();
    }
    
    void options_cb() {
        //sf2d_texture *top_scr_bg = load_theme_image(theme, "opt_top_scr_bg.png");
        //consoleInit(GFX_BOTTOM, &bottom);
        //consoleClear();
        
        u32 kDown;
        
        while (aptMainLoop()) {
            hidScanInput();
            
            kDown = hidKeysDown();
            //u32 kHeld = hidKeysHeld();
            
            if (kDown & KEY_B) break;
            
            sf2d_start_frame(GFX_TOP, GFX_LEFT);
            display_image(main_theme.top_scr_bg_opt);
            sf2d_end_frame();
            
            sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
            sf2d_end_frame();
            
            //printf("\x1b[0;0Hayy sample text");
            
            sf2d_swapbuffers();
        }
        
        //printf("Deleting top_scr_bg pointer...\n");
        //sf2d_free_texture(top_scr_bg); //delete top_scr_bg;
        //printf("Deleted top_scr_bg successfully.\n");
        //reboot_sf2d();
        //consoleInit(GFX_BOTTOM, &bottom);
        //printf("Ended function.\n");
        //consoleExit();
    }
    
    void run() {
        sftd_init();
        std::string theme = "test";
        //main_font = sftd_load_font_file(("/data/C_O_M_R_E_D/themes/" + theme + "/font.ttf").c_str());
        
        main_theme = load_theme(theme);
        
        //MM_CALLBACK_T opt;
        
        while (true) {
            //printf("Starting a new loop iteration...\n");
            main_menu(theme);
        
            switch (menu_cb) {
                case READ:
                    read_cb(theme);
                    break;
                case DOWNLOAD:
                    download_cb();
                    break;
                case OPTIONS:
                    options_cb();
                    break;
                case EXIT:
                    return;
                    break;
                default:
                    break;
            };
            
           
            //printf("End loop iteration.\n");
        }
        
        //sftd_free_font(main_font);
        free_theme(main_theme);
        sftd_fini();
        
        sf2d_free_texture(image);
        
        mlisting->Clear();
        delete mlisting;
        
    }

}

#endif /* MENU_H */

