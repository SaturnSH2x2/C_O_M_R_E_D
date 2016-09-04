# C_O_M_R_E_D
Open source theme-able comic reader for the 3DS.  

In it's current state, this homebrew is unusable.  The app crashes when exiting a comic and opening another comic.  Only a theme named "test" is loaded from the SD Card (and it has totally unstylish design choices).  The code's a complete and utter mess.  Do NOT expect this to be a release of any sort.  However, if you want to try it out anyways, you'll need to compile it.  Be sure to put the /data/ folder at the root of your SD Card.

Currently Implemented Features:
* Read a folder full of images as a comic (supported extensions are .jpg, .png, and .bmp)
* Orientation options: Hold the 3DS Upright and Sideways (Left- and Right-Hand Support)
* Zooming
* Page Selection (jump to a certain page in the comic)
* Partial theme support - see "Themes"

Planned Features:
* .cbz / .cbr support
* Comic/manga downloading feature
* A working options menu
* Automatic re-scaling of images (see "Known Issues")
* Default Orientation Loading (no need to switch before reading!)
* Better Icons (seriously, the app is ugly af right now)

Known Issues:
* There's a slight bug with the page selection that will kick you out of the comic if you jump to the last page. This should be fixed in later releases
* Due to the limitations of the 3DS, images larger than 1024x1024px will load up distorted. In the future, I'll try to add automatic scaling for said images so they'll load in-tact on the 3DS. However, for now, use Fotosizer to manually resize your images: http://www.fotosizer.com/
* (CIA Release only) If you try to exit by closing the app from the Home Menu, the 3DS will freeze. This is probably due to the fact that the RSF is unoptimized, this will be fixed later

More information at GBATemp Thread : http://gbatemp.net/threads/alpha-release-c_o_m_r_e_d-open-source-comic-manga-reader-for-your-3ds.440387/

Dependencies (not including dependencies for dependencies):

ctrulib - https://github.com/smealum/ctrulib

sf2dlib - https://github.com/xerpi/sf2dlib

sfillib - https://github.com/xerpi/sfillib

sftdlib - https://github.com/xerpi/sftdlib
