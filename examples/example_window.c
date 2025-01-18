#include "ui32.h"
widget window = NULL;

void EventCallback(int id){
	if(id == 0){
		MessageBox(NULL, "Hello, World!", "My Message Box", MB_OK | MB_ICONINFORMATION);
		SetTitle(window, "New Title");
	}
}

int main(){
	RegisterAppClass(NULL); // "C:\\Users\\Kiera\\Pictures\\the sun.ico"); // RegisterAppClass() Parameters: Icon Path
	window = Window("Window", 10, 10, 350, 320); // Window() Parameters: Title, x, y, Width, Height
	widget label = Label(window, "Hello, Win32!", 20, 30, 50, 50);	// Label() Parameters: Parent, Label, x, y, width, height
	widget button = Button(window, "Click Me", 0, 20, 70, 60, 25); // Button() Parameters: Parent, Label, ID, x, y, width, height
	bitmap image_gasmask = Image(NULL); // "C:\\Users\\Kiera\\Pictures\\en_gasmask.bmp"); // Image() Parameters: Path
	widget imagebox = ImageBox(window, image_gasmask, 20, 120, 50, 50); // ImageBox() Parameters: Parent, Image, x, y, width, height
  	widget edit = TextField(window, "", 190, 20, 120, 120); // TextField() Parameters: Parent, Preset Text, x, y, width, height
	font tahoma = Font("Tahoma", 18, 0, 0); // Font() Parameters: Font Name, Height, Bold (Bool), Italics (Bool)
	SetFont(label, tahoma); // SetFont() Parameters: Widget, Font
	SetVisible(window, true); // SetVisible() Parameters: Widget, Visibility (Bool)
	WindowLoop(window); // WindowLoop Parameters(): Window
}
