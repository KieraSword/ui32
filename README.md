# ui32
A Win32 utility library.

Wraps around existing Win32 APIs to make them easier to use (maybe even "higher-level").
Note: define EventCallback() even if your program doesn't use WM_COMMAND, as the linker complains if you don't. (or name a font variable `font`, as the compiler thinks that name is referring the type macro)

# Functions:
## Window:
RegisterAppClass() (Parameters: Icon Path)  
Window() (Parameters: Title, x, y, Width, Height)  
SetTitle() (Parameters: Window, New Title)  
SetWindowBg() (Parametres: RGB Brush)  
## Widgets:
SetText() (Parameters: Widget, New Text/Label)  
SetWidgetColors (Parameters: Widget, Foreground Color, Background Color)  
Label() (Parameters: Parent, Label, x, y, width, height)  
Button() (Parameters: Parent, Label, ID, x, y, width, height)  
TextField() (Parameters: Parent, Preset Text, x, y, width, height)  
ImageBox() (Parameters: Parent, Image, x, y, width, height)  
## Font:
Font() (Parameters: Font Name, Height, Bold (Bool), Italics (Bool))  
SetFont() (Parameters: Widget, Font)
## Misc:
Image() (Parameters: Image Path)  
CmdExec() (Parameters: Command, Hidden? (Bool))  
isWinXP() (returns true if the current system is running NT 5.1)  
# Types:
widget/HWidget: A preprocessor macro to HWND  
bitmap/HBitmap: A preprocessor macro to HBITMAP  
font/HFont: A preprocessor macro to HFONT
