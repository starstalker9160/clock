# clock

![screenshot](./Assets/hero%20image.png)

## Overview
Full-screen clock app built in C++ using Qt. Diplays time in 24h format (`13:02:31` as opposed to `01:02:31 pm`) and supports customizable foreground and background colours, font, and font size via a JSON configuration file.

## Config
You can customize how the clock app looks by editing the `config.json` file in the released build  
  
Example `config.json` (used in the image above):
```json
{
    "foreground": "#EFDFBB",
    "background": "#722F37",
    "font": "font.ttf",
    "fontSize": 80
}
```
  

### Windows
The config file lives in the same folder as the `clock.exe` file.
### MacOS
The config file lives within `clock.app` at `.../clock.app/Contents/Resources/config.json`  
  

where;  
```
foreground  =  text colour    =  HEX code [string]  
background  =  window colour  =  HEX code [string]  
font        =  text font      =  path to font file (.ttf or .otf preferred) [string]  
fontSize    =  font size      =  . [int]
```

## License
This application uses Qt, which is licensed under the GNU Lesser General Public License (LGPL). A copy of the LGPL is included with the release builds.