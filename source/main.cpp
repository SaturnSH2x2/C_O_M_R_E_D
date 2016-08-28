#include <iostream>

#include <3ds.h>
#include <sf2d.h>
#include <sfil.h>

#include "app.h"
#include "button.h"
#include "menu.h"

using namespace std;
using namespace btn;

int main(int argc, char **argv)
{   
	//Initialize the sf2d lib
	sf2d_init();
        //acInit();
	//Set the background color
	//sf2d_set_clear_color(RGBA8(0x40, 0x40, 0x40, 0xFF));

	// Main loop
	//while (true)
	//{
          //  string path = "/data/C_O_M_R_E_D/test.png";
           // string theme = "test";
            
            //sf2d_texture *img = sfil_load_PNG_file(path.c_str(), SF2D_PLACE_RAM);
            
            //app::CALLBACK_T test;
            //test = app::run_img(img, theme, app::ONE_SCREEN, true);
            
            //menu::MM_CALLBACK_T test2;
            //test2 = menu::main_menu(theme);
            
            //if (test2) break;
         //   menu::run();
	//}
        
        menu::run();

	sf2d_fini();
	return 0;
}
