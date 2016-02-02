# etm_renamer
Simple program to rename Army (possible other services) Electronic Technical Manuals to their actual names.

You see manuals in the Army are named using a simple number scheme for use in the horrible ETM manager program (not actual name).
Most people prefer to browse directly in the folder with the manuals (PDFs). This means you must open the etm.txt file and decode the actual file name from inside the etm.txt file. This takes lots of time, there are hundreds of manuals.

This program simply does this for you.

Use:
* Copy PDF folder contents from ETM cd to where ever you keep your manuals. Preferably into a folder named after the vehicle or item the manuals cover ie. - tank manuals should go into an M1 folder.
* Copy the etm.txt file to the same folder.
* Copy etm.exe into the folder that all your manuals are in.
* Run etm.exe. It will prompt you with what files were changed. It will go through each subfolder and process each etm.txt file.

After it's done be sure to check out its handy work.

Here are some notes -
* If you want to see a well formatted version of the etm.txt file, check out description.txt
* Some of the etm.txt files have errors, there is no way to correct these, you just have to fix whatever you find as you go along.
