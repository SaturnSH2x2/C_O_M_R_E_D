/*
 * Licensed under the WTFPL
 * 
DO WHAT THE F*** YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE F*** YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE F*** YOU WANT TO.

*/

#ifndef RF_H
#define RF_H

#include <iostream>
#include <string>
#include <fstream>

#include <stdlib.h>

using namespace std;

namespace rf {
    int number_of_lines(std::string filename);
    
    std::string *read_from_file(std::string filename) {
        std::string *contents;
        int i;
        
        // get number of lines in file, then allocate enough space to hold
        // the file's contents
        int lines = number_of_lines(filename);
        contents = new std::string[lines];
        
        ifstream file(filename.c_str());
        
        if (file.is_open()) {
            for (i = 0; i < lines; i++) {
                std::getline(file, contents[i]);
            }
        } else {
            std::cout << "Error reading file!";
            return NULL;
        }
        
        return contents;
    }
    
    bool does_file_exist(std::string filename);
    
    // reads designated font colors from config file
    void read_theme_config(std::string theme, u32 &top_scr, u32 &bot_scr, u32 &numpad, u32 &selection) {
        std::string DESCRIPTORS[4];
        DESCRIPTORS[0] = "[topScreenFontColor]";
        DESCRIPTORS[1] = "[bottomScreenFontColor]";
        DESCRIPTORS[2] = "[numpadFontColor]";
        DESCRIPTORS[3] = "[selectionFontColor]";
        
        stringstream ss;
        
        std::string strbuf;
        
        int line_it = 0;
        int descriptor_it;
        int lines = number_of_lines("/data/C_O_M_R_E_D/themes/" + theme + "/theme.txt");
        
        int R;
        int G;
        int B;
        
        ifstream file("/data/C_O_M_R_E_D/themes/" + theme + "/theme.txt");
        
        if (file.is_open()) {
            while (line_it < lines) {
                std::getline(file, strbuf);
                strbuf.erase(strbuf.size() - 1);
                
                for (descriptor_it = 0; descriptor_it < 4; descriptor_it++) {
                    if (strbuf.compare(DESCRIPTORS[descriptor_it]) == 0) {
                        line_it += 3;
                        
                        // reads as hex values
                        std::getline(file, strbuf); ss << std::hex << strbuf;  ss >> R;  
                        std::getline(file, strbuf); ss << std::hex << strbuf;  ss >> G;
                        std::getline(file, strbuf); ss << std::hex << strbuf;  ss >> B;
                        
                        if (R > 0xFF) break;
                        if (G > 0xFF) break;
                        if (B > 0xFF) break;
                        
                        switch (descriptor_it) {
                            case 0:
                                top_scr = RGBA8(R, G, B, 0xFF);
                                break;
                            case 1:
                                bot_scr = RGBA8(R, G, B, 0xFF);
                                break;
                            case 2:
                                numpad = RGBA8(R, G, B, 0xFF);
                                break;
                            case 3:
                                selection = RGBA8(R, G, B, 0x80);
                                break;
                            default:
                                return;
                                break;
                        };
                    }
                }
                
                line_it += 1;
            }                        
            file.close();
        }
    }
    
    bool does_file_exist(std::string filename) {
        ifstream file(filename.c_str(), std::ios::binary);
        
        if (file.is_open()) {
            return true;
        } else {
            return false;
        }
    }
    
    char *read_bin(std::string path) {
        char *buffer;
        streampos size;
        
        ifstream file(path.c_str(), ios::binary|ios::in|ios::ate);
        
        if (file.is_open()) {
            size = file.tellg();
            
            buffer = new char[size];
            
            file.seekg(0, ios::beg);
            file.read(buffer, size);
            file.close();
            
            return buffer;
        } else {
            return NULL;
        }
    }
    
    unsigned long read_size(std::string path) {
        //char *buffer;
        streampos size;
        
        ifstream file(path.c_str(), ios::binary|ios::in|ios::ate);
        
        if (file.is_open()) {
            size = file.tellg();
            
            return size;
        } else {
            return 1;
        }
    }
    
    int number_of_lines(std::string filename) {
        int number_of_lines = 0;
        std::string dummy;
        
        ifstream f (filename.c_str());
        
        if (f.is_open()) {
            while (std::getline(f, dummy)) {
                number_of_lines += 1;
            }
        } else {
            std::cout << "Error reading file!\n";
        }
        
        return number_of_lines;
    }
    
    void save_page(int index, std::string path) {
        std::string full_path = path + "/page.txt";
        
        std::stringstream ss;
        std::string str_index;
        
        ss << index;
        ss >> str_index;
        
        ofstream file (full_path.c_str(), ios::trunc);
        
        if (file.is_open()) {
            file << str_index;
            file.close();
        } 
    }
    
    int load_page(std::string path) {
        int index;
        std::string str_index;
        
        path = path + "/page.txt";
        
        ifstream file (path.c_str());
        
        if (file.is_open()) {
            std::getline(file, str_index);
            file.close();
        } else {
            return 1;
        }
        
        index = atoi(str_index.c_str());
        
        return index;
    }
}


#endif /* RF_H */

