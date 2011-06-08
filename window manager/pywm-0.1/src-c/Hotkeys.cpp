//@+leo
//@+node:0::@file src-c/Hotkeys.cpp
//@+body
//@@language c
// Hotkeys.C

// If you want to change what the hotkeys are, see the table at the bottom!


//@<< Hotkeys #includes >>
//@+node:1::<< Hotkeys #includes >>
//@+body
#include <stdio.h>

#include "config.h"
#include "Frame.h"
#include "Desktop.h"

#include "pycallbacks.h"


//@-body
//@-node:1::<< Hotkeys #includes >>


//@<< Hotkeys declarations >>
//@+node:2::<< Hotkeys declarations >>
//@+body

extern void ShowMenu();
extern void ShowTabMenu(int tab);

//@-body
//@-node:2::<< Hotkeys declarations >>


//@+others
//@+node:3::NextWindow
//@+body

#if STANDARD_HOTKEYS

void NextWindow() { // Alt+Tab
  ShowTabMenu(1);
}

//@-body
//@-node:3::NextWindow
//@+node:4::PreviousWindow
//@+body

void PreviousWindow() { // Alt+Shift+Tab
  ShowTabMenu(-1);
}

//@-body
//@-node:4::PreviousWindow
//@+node:5::NextDesk
//@+body

#endif

#if DESKTOPS

void NextDesk() {
  if (Desktop::current()) {
    Desktop::current(Desktop::current()->next?
		     Desktop::current()->next:Desktop::first);
  } else {
    Desktop::current(Desktop::first);
  }
}

//@-body
//@-node:5::NextDesk
//@+node:6::PreviousDesk
//@+body

void PreviousDesk() {
  Desktop* search=Desktop::first;
  while (search->next && search->next!=Desktop::current()){
    search=search->next;
  }
  Desktop::current(search);
}

//@-body
//@-node:6::PreviousDesk
//@+node:7::DeskNumber
//@+body
//@+at
//  warning: this assummes it is bound to Fn key:

//@-at
//@@c

static void DeskNumber() {
  Desktop::current(Desktop::number(Fl::event_key()-FL_F, 1));
}

//@-body
//@-node:7::DeskNumber
//@+node:8::Raise
//@+body
#endif

#if WMX_HOTKEYS || CDE_HOTKEYS

static void Raise() { // Alt+Up
  Frame* f = Frame::activeFrame();
  if (f) f->raise();
}

//@-body
//@-node:8::Raise
//@+node:9::Lower
//@+body

static void Lower() { // Alt+Down
  Frame* f = Frame::activeFrame();
  if (f) f->lower();
}

//@-body
//@-node:9::Lower
//@+node:10::Iconize
//@+body

static void Iconize() { // Alt+Enter
  Frame* f = Frame::activeFrame();
  if (f) f->iconize();
  else ShowMenu(); // so they can deiconize stuff
}

//@-body
//@-node:10::Iconize
//@+node:11::Close
//@+body

static void Close() { // Alt+Delete
  Frame* f = Frame::activeFrame();
  if (f) f->close();
}

//@-body
//@-node:11::Close
//@+node:12::keybindings[]
//@+body
////////////////////////////////////////////////////////////////

static struct {int key; void (*func)();} keybindings[] = {

#if STANDARD_HOTKEYS || MINIMUM_HOTKEYS
  // these are very common and tend not to conflict, due to Windoze:
  {FL_ALT+FL_Escape,	ShowMenu},
  {FL_ALT+FL_Menu,	ShowMenu},
#endif

#if STANDARD_HOTKEYS
  {FL_ALT+FL_Tab+FL_SHIFT,	NextWindow},
  {FL_ALT+FL_Tab,PreviousWindow},
  {FL_ALT+FL_SHIFT+0xfe20,PreviousWindow}, // XK_ISO_Left_Tab
#endif

#if KWM_HOTKEYS && DESKTOPS // KWM uses these to switch desktops
//   {FL_CTRL+FL_Tab,	NextDesk},
//   {FL_CTRL+FL_SHIFT+FL_Tab,PreviousDesk},
//   {FL_CTRL+FL_SHIFT+0xfe20,PreviousDesk}, // XK_ISO_Left_Tab
  {FL_CTRL+FL_F+1,	DeskNumber},
  {FL_CTRL+FL_F+2,	DeskNumber},
  {FL_CTRL+FL_F+3,	DeskNumber},
  {FL_CTRL+FL_F+4,	DeskNumber},
  {FL_CTRL+FL_F+5,	DeskNumber},
  {FL_CTRL+FL_F+6,	DeskNumber},
  {FL_CTRL+FL_F+7,	DeskNumber},
  {FL_CTRL+FL_F+8,	DeskNumber},
  {FL_CTRL+FL_F+9,	DeskNumber},
  {FL_CTRL+FL_F+10,	DeskNumber},
  {FL_CTRL+FL_F+11,	DeskNumber},
  {FL_CTRL+FL_F+12,	DeskNumber},
#endif

#if WMX_HOTKEYS
  // wmx also sets all these, they seem pretty useful:
  {FL_ALT+FL_SHIFT+FL_Up,	Raise},
  {FL_ALT+FL_SHIFT+FL_Down,	Lower},
  {FL_ALT+FL_SHIFT+FL_Enter,	Iconize},
  {FL_ALT+FL_SHIFT+FL_Delete,	Close},
  //{FL_ALT+FL_Page_Up,	ToggleMaxH},
  //{FL_ALT+FL_Page_Down,ToggleMaxW},
#endif

#if WMX_DESK_HOTKEYS && DESKTOPS
  // these wmx keys are not set by default as they break NetScape:
  {FL_ALT+FL_Left,	PreviousDesk},
  {FL_ALT+FL_Right,	NextDesk},
#endif

#if CDE_HOTKEYS
  // CDE hotkeys (or at least what SGI's 4DWM uses):
  {FL_ALT+FL_F+1,	Raise},
//{FL_ALT+FL_F+2,	unknown}, // KWM uses this to run a typed-in command
  {FL_ALT+FL_F+3,	Lower},
  {FL_ALT+FL_F+4,	Close}, // this matches KWM
//{FL_ALT+FL_F+5,	Restore}, // useless because no icons visible
//{FL_ALT+FL_F+6,	unknown}, // ?
//{FL_ALT+FL_F+7,	Move}, // grabs the window for movement
//{FL_ALT+FL_F+8,	Resize}, // grabs the window for resizing
  {FL_ALT+FL_F+9,	Iconize},
//{FL_ALT+FL_F+10,	Maximize},
//{FL_ALT+FL_F+11,	unknown}, // ?
  {FL_ALT+FL_F+12,	Close}, // actually does "quit"
#else
#if DESKTOPS && DESKTOP_HOTKEYS
  // seem to be common to Linux window managers
  {FL_ALT+FL_F+1,	DeskNumber},
  {FL_ALT+FL_F+2,	DeskNumber},
  {FL_ALT+FL_F+3,	DeskNumber},
  {FL_ALT+FL_F+4,	DeskNumber},
  {FL_ALT+FL_F+5,	DeskNumber},
  {FL_ALT+FL_F+6,	DeskNumber},
  {FL_ALT+FL_F+7,	DeskNumber},
  {FL_ALT+FL_F+8,	DeskNumber},
  {FL_ALT+FL_F+9,	DeskNumber},
  {FL_ALT+FL_F+10,	DeskNumber},
  {FL_ALT+FL_F+11,	DeskNumber},
  {FL_ALT+FL_F+12,	DeskNumber},
#endif
#endif
  {0}};


//@-body
//@-node:12::keybindings[]
//@+node:13::Handle_Hotkey
//@+body

#endif

int Handle_Hotkey()
{
  int key = Fl::event_key();

  //if (Fl::test_shortcut(FL_SHIFT + FL_ALT + key))
  //{
  //   printf("SHIFT-ALT %x\n");
  //   key += FL_SHIFT + FL_ALT;
  //}

  if (Fl::test_shortcut(FL_SHIFT + key))
    key |= FL_SHIFT + key;
  if (Fl::test_shortcut(FL_CTRL + key))
    key |= FL_CTRL + key;
  if (Fl::test_shortcut(FL_ALT + key))
    key |= FL_ALT + key;
  if (Fl::test_shortcut(FL_META + key))
    key |= FL_META + key;

  if (Fl::test_shortcut(FL_SHIFT + FL_CTRL + key))
    key |= FL_SHIFT + FL_CTRL + key;
  if (Fl::test_shortcut(FL_SHIFT + FL_ALT + key))
    key |= FL_SHIFT + FL_ALT + key;
  if (Fl::test_shortcut(FL_SHIFT + FL_META + key))
    key |= FL_SHIFT + FL_META + key;
  if (Fl::test_shortcut(FL_CTRL + FL_ALT + key))
    key |= FL_CTRL + FL_ALT + key;
  if (Fl::test_shortcut(FL_CTRL + FL_META + key))
    key |= FL_CTRL + FL_META + key;
  if (Fl::test_shortcut(FL_ALT + FL_META + key))
    key |= FL_ALT + FL_META + key;
  if (Fl::test_shortcut(FL_SHIFT + FL_CTRL + FL_ALT + key))
    key |= FL_SHIFT + FL_CTRL + FL_ALT + key;
  if (Fl::test_shortcut(FL_SHIFT + FL_CTRL + FL_META + key))
    key |= FL_SHIFT + FL_CTRL + FL_META + key;
  if (Fl::test_shortcut(FL_SHIFT + FL_ALT + FL_META + key))
    key |= FL_SHIFT + FL_ALT + FL_META + key;
  if (Fl::test_shortcut(FL_CTRL + FL_ALT + FL_META + key))
    key |= FL_CTRL + FL_ALT + FL_META + key;
  if (Fl::test_shortcut(FL_SHIFT + FL_CTRL + FL_ALT + FL_META + key))
    key |= FL_SHIFT + FL_CTRL + FL_ALT + FL_META + key;

  //printf("Handle_Hotkey: entered - key=%x\n", key);

  if (py_on_keyEvent(pyClient, (long)key))
      return 1;
                       
  for (int i = 0; keybindings[i].key; i++) {
    if (Fl::test_shortcut(keybindings[i].key)
        || ((keybindings[i].key & 0xFFFF) == FL_Delete
            	&& Fl::event_key() == FL_BackSpace// fltk bug?
             )
	)
    {
      keybindings[i].func();
      return 1;
    }
  }
  return 0;
}

//@-body
//@-node:13::Handle_Hotkey
//@+node:14::Grab_Hotkeys
//@+body

extern Fl_Window* Root;

void Grab_Hotkeys() {
  Window root = fl_xid(Root);
  for (int i = 0; keybindings[i].key; i++)
  {
    int k = keybindings[i].key;
    int keycode = XKeysymToKeycode(fl_display, k & 0xFFFF);
    if (!keycode)
        continue;
    // Silly X!  we need to ignore caps lock & numlock keys by grabbing
    // all the combinations:
    XGrabKey(fl_display, keycode, k>>16,     root, 0, 1, 1);
    XGrabKey(fl_display, keycode, (k>>16)|2, root, 0, 1, 1); // CapsLock
    XGrabKey(fl_display, keycode, (k>>16)|16, root, 0, 1, 1); // NumLock
    XGrabKey(fl_display, keycode, (k>>16)|18, root, 0, 1, 1); // both
  }
}

//@-body
//@-node:14::Grab_Hotkeys
//@+node:15::Hotkeys_bindKey
//@+body

void Hotkeys_bindKey(long key)
{
  Window root = fl_xid(Root);

  int keycode = XKeysymToKeycode(fl_display, key & 0xFFFF);
  if (!keycode)
     return;
 
  // Silly X!  we need to ignore caps lock & numlock keys by grabbing
  // all the combinations:
  XGrabKey(fl_display, keycode, key >>16, root, 0, 1, 1);
  //printf("Hotkeys:bindKey:ok - key=%x, keycode=%x\n", key, keycode);
}


//@-body
//@-node:15::Hotkeys_bindKey
//@-others


//@-body
//@-node:0::@file src-c/Hotkeys.cpp
//@-leo
