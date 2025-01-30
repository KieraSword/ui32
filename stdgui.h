#include <windows.h>
#include <stdlib.h>
#include <stdbool.h>
// Utility Library for Win32 (styled to look like stdio functions (aka all lowercaps, less functions))
// ui32.h (stdgui.h)
#define wgt HWND
#define fon HFONT
#define bmp HBITMAP
#define col COLORREF
void event_callback(int id); // User defines this to use WM_COMMAND functionality without actually touching WM_COMMAND or WinProc
int widgets_amount = 0;
HWND *widgets = NULL;
HBRUSH WndBgColor = COLOR_WINDOW; // System's current theme's window background color
typedef struct {
    HWND hwnd;           
    COLORREF fgColor;    
    COLORREF bgColor;    
} WidgetInfo;
WidgetInfo *widgetInfos = NULL;
int widgetInfosAmount = 0;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_DESTROY:
			for(int i=0;i<widgets_amount; i++){
				LOGFONT lf;
    				int is_font = GetObject(widgets[i], sizeof(LOGFONT), &lf);
				if(is_font){
					DeleteObject(widgets[i]);
				} else {
					DestroyWindow(widgets[i]);
				}
			
			}
			free(widgets);
			free(widgetInfos); 
            PostQuitMessage(0);
            return 0;
        case WM_ERASEBKGND:
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
			FillRect(hdc, &ps.rcPaint, WndBgColor);
            EndPaint(hwnd, &ps);
            return 0;
        } 

		case WM_CTLCOLORSTATIC: {
			HDC hdcStatic = (HDC)wParam;
		    HWND hwndControl = (HWND)lParam;
		    
		    // Search for the widget's color info
		    for (int i = 0; i < widgetInfosAmount; i++) {
		        if (widgetInfos[i].hwnd == hwndControl) {
		            // Set the custom foreground and background color for the widget
		            SetTextColor(hdcStatic, widgetInfos[i].fgColor);
		            SetBkMode(hdcStatic, TRANSPARENT); // Ensure transparent background
		            return (LRESULT)CreateSolidBrush(widgetInfos[i].bgColor);  // Custom background brush
		        }
		    }
		    
		    // If no custom color is set, return the current window background
			return (LRESULT)WndBgColor;
		}

        case WM_COMMAND:{
        	if(EventCallback != NULL){
				EventCallback(LOWORD(wParam));
			}
			return 0;
		}
	}
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
WNDCLASS wc = {0};

BOOL registerapp(LPCSTR icon) {
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.lpszClassName = "WINDOW";
    if(icon != NULL){
		wc.hIcon = (HICON)LoadImage(NULL, icon, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	}
    return RegisterClass(&wc);
}
HWND window(LPCSTR title, int x, int y, int width, int height){
	return CreateWindowEx(
        0, wc.lpszClassName,
        title,
        WS_OVERLAPPEDWINDOW, x, y, width, height,
        NULL, NULL, wc.hInstance, NULL
    );
}
HWND label(HWND hwnd, LPCSTR text, int x, int y, int width, int height){
	HWND label = CreateWindow(
        "STATIC",        
        text,
        WS_VISIBLE | WS_CHILD,
        x, y, width, height, hwnd, (HMENU)-1, wc.hInstance, NULL
    );
	SendMessage(label, WM_SETFONT, (WPARAM)CreateFont(
    	18, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,       
    	"Tahoma"              
	), TRUE); // Set default font
	widgets_amount++; 
    	widgets = realloc(widgets, widgets_amount * sizeof(HWND));
	widgets[widgets_amount - 1] = label;
	return label;
}
HWND button(HWND hwnd, LPCSTR text, int id, int x, int y, int width, int height){
	HWND button = CreateWindow(
        "BUTTON",        
        text,
        WS_VISIBLE | WS_CHILD,
        x, y, width, height, hwnd, (HMENU)id, wc.hInstance, NULL
    );
	SendMessage(button, WM_SETFONT, (WPARAM)CreateFont(
    	18, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,       
    	"Tahoma"              
	), TRUE); // Set default font
	widgets_amount++; 
    	widgets = realloc(widgets, widgets_amount * sizeof(HWND));
	widgets[widgets_amount - 1] = button;
	return button;

}
HWND textfield(HWND hwnd, LPCSTR text, int x, int y, int width, int height){
	HWND textField = CreateWindow(
        "EDIT",        
        text,
        WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE,
        x, y, width, height, hwnd, (HMENU)-1, wc.hInstance, NULL
    );
	SendMessage(textField, WM_SETFONT, (WPARAM)CreateFont(
    	18, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,       
    	"Tahoma"              
	), TRUE); // Set default font
	widgets_amount++; 
    	widgets = realloc(widgets, widgets_amount * sizeof(HWND));
	widgets[widgets_amount - 1] = textField;
	return textField;
}
HFONT font(LPCSTR fontName, int height, bool bold, bool italic){
	HFONT user_font = NULL;
	// TODO: simplify this "if else" chain
	if(bold && !italic){
		user_font = CreateFont(
	    	height, 0, 0, 0, FW_BOLD, 0, 0, 0, 
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,       
	    	fontName              
		);	
	}
	else if(!bold && italic){
		user_font = CreateFont(
	    	height, 0, 0, 0, FW_NORMAL, 1, 0, 0, 
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,       
	    	fontName              
		);	
	}
	else if(bold && italic){
		user_font = CreateFont(
	    	height, 0, 0, 0, FW_BOLD, 1, 0, 0, 
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,       
	    	fontName              
		);	
	}else{
		user_font = CreateFont(
	    	height, 0, 0, 0, FW_NORMAL, 0, 0, 0, 
			ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,       
	    	fontName              
		);	
	}
	widgets_amount++; 
    	widgets = realloc(widgets, widgets_amount * sizeof(HFONT));
	widgets[widgets_amount - 1] = user_font;
	return user_font;
}
HBITMAP image(LPCSTR path){
	return (HBITMAP)LoadImage(
        NULL, 
        path,
        IMAGE_BITMAP, 
        0, 
        0, 
        LR_LOADFROMFILE
    );
}
HWND imagebox(HWND hwnd, HBITMAP image, int x, int y, int width, int height){
	HWND imageBox = CreateWindow(
    "STATIC",         
    NULL,    
    WS_VISIBLE | WS_CHILD | SS_BITMAP,  
	    x, y, width, height, 
	    hwnd,             
	    NULL,             
	    wc.hInstance,   
	    NULL            
	);
	if (image != NULL) {
        SendMessage(imageBox, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);
    } else {
        MessageBox(hwnd, "Failed to load image.", "Error", MB_OK | MB_ICONERROR);
    }
	widgets_amount++; 
    widgets = realloc(widgets, widgets_amount * sizeof(HWND));
	widgets[widgets_amount - 1] = imageBox;
    return imageBox;
}
void setfont(HWND control, HFONT font){
	SendMessage(control, WM_SETFONT, (WPARAM)font, TRUE); 
}
void settext(HWND control, LPCSTR text){
	SetWindowText(control, TEXT(text));
}
char* gettext(HWND control){
	char buffer[256];
	GetWindowText(control, buffer, sizeof(buffer));
	return buffer;
}
void settitle(HWND hwnd, LPCSTR text){
	if(GetMenu(hwnd) == NULL){
		SetWindowText(hwnd, text);
	}
}
void setimage(HWND control, HBITMAP image){
	SetWindowText(control, image);
}

void setWidgetcolors(HWND widget, COLORREF fg, COLORREF bg) {
    for (int i = 0; i < widgetInfosAmount; i++) {
        if (widgetInfos[i].hwnd == widget) {
            widgetInfos[i].fgColor = fg;
            widgetInfos[i].bgColor = bg;
            return;
        }
    }
    
    widgetInfosAmount++;
    widgetInfos = realloc(widgetInfos, widgetInfosAmount * sizeof(WidgetInfo));
    widgetInfos[widgetInfosAmount - 1].hwnd = widget;
    widgetInfos[widgetInfosAmount - 1].fgColor = fg;
    widgetInfos[widgetInfosAmount - 1].bgColor = bg;
}
void setvisible(HWND widget, bool visible){
	if(visible){
		ShowWindow(widget, SW_SHOW);
	} else {
		ShowWindow(widget, SW_HIDE);
	}
}

void setwindowbg(COLORREF NewBg){
	WndBgColor = CreateSolidBrush(NewBg);
}
void cmdexec(LPCSTR cmd, bool hide){
	if(hide){
		WinExec(cmd, SW_HIDE);
	}
	else{
		WinExec(cmd, SW_SHOWNORMAL);
	}
	
}
const char* getwinver() {
    OSVERSIONINFOEX osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
    if (GetVersionEx((OSVERSIONINFO*)&osvi)) {
	if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
            return "2000"; 
    	}
	if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) {
            return "xp"; 
    	}
	if (osvi.dwMajorVersion == 6) {
            return "vista"; 
        }
	if (osvi.dwMajorVersion == 7) {
            return "7"; 
        }
	if (osvi.dwMajorVersion == 8) {
            return "8"; 
        }
        if (osvi.dwMajorVersion == 10) {
            return "10"; 
        }
    }
    return "null"; // Return null as a failsafe
}
void windowloop(HWND hwnd){
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
