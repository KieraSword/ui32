# ui32
A Win32 utility library.

Wraps around existing Win32 APIs to make them easier to use (mabye even "higher-level").
Note: define EventCallback() even if your program doesn't use WM_COMMAND, as the linker complains if you don't.

# Types:
widget/HWidget: A preprocessor macro to HWND  
bitmap/HBitmap: A preprocessor macro to HBITMAP  
font/HFont: A preprocessor macro to HFONT
