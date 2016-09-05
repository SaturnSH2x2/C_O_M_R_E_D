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
    void load_config(std::string &theme, app::VIEWMODE_T &mode) {
        const int head_max = 2;
        const int desc_max = 3;
        int num_lines;
        
        int cur_line = 0;
        int header_index;
        int desc_index;
        int match_index = -1;
        
        std::string headers[2];
        headers[0] = "[defaultTheme]";
        headers[1] = "[defaultOrientation]";
                
        std::string descriptors[3];
        descriptors[0] = "upright";
        descriptors[1] = "left_hand";
        descriptors[2] = "right_hand";

        std::string buf;        

        ifstream file("/data/C_O_M_R_E_D/config.txt");
        num_lines = rf::number_of_lines("/data/C_O_M_R_E_D/config.txt");
        
        if (file.is_open()) {
            printf("File is open!\n");
        while (cur_line < num_lines) {
            if (!(cur_line < num_lines)) break;
            
            std::getline(file, buf);
            buf.erase(buf.size() - 1);
            
            for (header_index = 0; header_index < head_max; header_index++) {
                if (buf.compare(headers[header_index]) == 0) {                    
                    std::getline(file, buf);
                    buf.erase(buf.size() - 1);
                    cur_line += 1;
                    
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
                        default:
                            break;
                    };
                    
                }
            }
            
            cur_line += 1;;
        }
            
        }
        //printf("Theme - ");
        //printf(theme.c_str());
        //printf("\n");
        
        //printf("Orientation - ");
        //printf(descriptors[match_index].c_str());
        file.close();
    }
}

#endif /* OPTIONS_H */

