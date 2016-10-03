/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   options.h
 * Author: fluffy
 *
 * Created on September 5, 2016, 4:51 PM
 */

#ifndef OPTIONS_H
#define OPTIONS_H

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

#include "app.h"
#include "rf.h"

namespace opt {
    void load_config(std::string &theme, app::VIEWMODE_T &mode, app::DPADMODE_T &dpad) {
        const int head_max = 3;
        const int desc_max = 3;
        const int desc2_max = 2;
        int num_lines;
        
        int cur_line = 0;
        int header_index;
        int desc_index;
        int match_index = -1;
        int newline_index;
        
        printf("Variables initialized.\n");
        
        std::string headers[3];
        headers[0] = "[defaultTheme]";
        headers[1] = "[defaultOrientation]";
        headers[2] = "[dPadOrientation]";
                
        std::string descriptors[3];
        descriptors[0] = "upright";
        descriptors[1] = "left_hand";
        descriptors[2] = "right_hand";

        std::string desc2[2];
        desc2[0] = "us";
        desc2[1] = "jp";
        
        printf("All descriptors initialized.\n");
        
        std::string buf;        

        ifstream file("/data/C_O_M_R_E_D/config.txt");
        num_lines = rf::number_of_lines("/data/C_O_M_R_E_D/config.txt");
        
        printf("File read successfully.\n");
        
        if (file.is_open()) {
            
            printf("File is open.\n");
            dpad = app::NOT_READ;
            
            file.clear();
            file.seekg(0, ios::beg);
            
        while (cur_line < num_lines) {
            if (!(cur_line < num_lines)) break;
            
            std::getline(file, buf);
            
            newline_index = buf.find("\n");
            
            if (newline_index != string::npos) {
                buf.erase(newline_index);
                printf("Removed newline.\n");
            }
                
            printf("Read line.\n");
            
            for (header_index = 0; header_index < head_max; header_index++) {
                if (buf.compare(headers[header_index]) == 0) {  
                    printf("Header match!\n");
                    
                    std::getline(file, buf);
                    
                    newline_index = buf.find("\n");
                    
                    if (newline_index != string::npos) {
                        buf.erase(newline_index);
                        printf("Removed newline.\n");
                    }
                    
                    cur_line += 1;
                    
                    printf("Read next line: ");
                    printf(buf.c_str());
                    printf("\n");
                    
                    switch (header_index) {
                        case 0:
                            theme = buf;
                            break;
                        case 1:
                            for (desc_index = 0; desc_index < desc_max; desc_index++) {
                                if (buf.compare(descriptors[desc_index]) == 0) {
                                    match_index = desc_index;
                                    break;
                                }
                            }
                            
                            // nested switch-case statements, fun fun fun
                            switch (match_index) {
                                case 0:
                                    mode = app::TWO_SCREEN;
                                    break;
                                case 1:
                                    mode = app::TWO_SCREEN_L;
                                    break;
                                case 2:
                                    mode = app::TWO_SCREEN_R;
                                    break;
                                default:
                                    mode = app::TWO_SCREEN;
                                    break;
                            };
                            
                            break;
                        case 2:
                            for (desc_index = 0; desc_index < desc2_max; desc_index++) {
                                if (buf.compare(desc2[desc_index]) == 0) {
                                    match_index = desc_index;
                                    break;
                                }
                            }
                            switch (match_index) {
                                case 0:
                                    printf("Left to right reading.\n");
                                    dpad = app::LEFT_TO_RIGHT;
                                    break;
                                case 1:
                                    printf("Right to left reading.\n");
                                    dpad = app::RIGHT_TO_LEFT;
                                    break;
                                default:
                                    printf("Couldn't read orientation!\n");
                                    dpad = app::DPADMODE_T::NADA2;
                                    break;
                            };
                            
                            break;
                        default:
                            break;
                    };
                    
                } else {
                    printf("Not a match - ");
                    printf(buf.c_str());
                    printf("\n");
                }
            }
            
            printf("Increment line.\n");
            cur_line += 1;;
        }
            
            printf("Broke out of file reading loop.\n");
        }

        file.close();
        
        printf("Close file.\n");
    }
    
    void save_config(int orient_index, int dpad_index, std::string theme_name) {
        ofstream file ("/data/C_O_M_R_E_D/config.txt", ios::trunc);
        
        std::string orient_desc[3];
        orient_desc[0] = "upright";
        orient_desc[1] = "left_hand";
        orient_desc[2] = "right_hand";
        
        std::string dpad_desc[2];
        dpad_desc[0] = "us";
        dpad_desc[1] = "jp";
        
        if (file.is_open()) {
            file << "[defaultTheme]\n";
            file << theme_name;
            file << "\n\n";
            
            file << "[defaultOrientation]\n";
            file << orient_desc[orient_index];
            file << "\n\n";
            
            file << "[dPadOrientation]\n";
            file << dpad_desc[dpad_index];
            file << "\n\n";
            
            file.close();
        }
    }

    struct opt_listing {
        int num_of_entries = -1;
        int cursor = 0;
        
        std::string description;
        std::string entries[10];  // yes, the limit is arbitrary
        
        void (*cb)() = NULL;
    };
    
    // TODO: rewrite this so it doesn't crash
    opt_listing create_listing(std::string desc, int num_of_entries, std::string entries[]) {
        opt_listing new_list;
        
        new_list.description = desc;
        new_list.num_of_entries = num_of_entries;
        
        //new_list.entries = entries;
        //new_list.entries = new std::string[num_of_entries];
        
        for (int i = 0; i < num_of_entries; i++) {
            if (i >= 10) break;
            new_list.entries[i] = entries[i];
        }
        
        return new_list;
    }
    
    opt_listing create_listing(std::string desc, void (*cb)()) {
        opt_listing new_list;
        
        new_list.description = desc;
        new_list.cb = cb;
        
        return new_list;
    }
    
    //void delete_listing(opt_listing list) {
    //    if (list.entries != NULL) delete list.entries;
    //}
    
    void display(opt_listing *list, int y, sftd_font* font, u32 color) {
        sftd_draw_text(font, 0, y, color, 12, list->description.c_str());
        if (list->num_of_entries != -1) sftd_draw_text(font, (list->description.length() * 6) + 5, y, color, 12, list->entries[list->cursor].c_str());        
    }
    
    void read_input_cb(opt_listing *list, u32 kDown) {
        if (kDown & KEY_LEFT)  list->cursor -= 1;
        if (kDown & KEY_RIGHT) list->cursor += 1;
        
        if (list->cursor > list->num_of_entries - 1) list->cursor = 0;
        if (list->cursor < 0)                       list->cursor = list->num_of_entries - 1;
        
        if ((kDown & KEY_A ) && list->cb != NULL) list->cb();
    }
    
    
}

#endif /* OPTIONS_H */

