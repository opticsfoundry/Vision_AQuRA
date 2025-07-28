#include <applicat.h>
#include <decframe.h>
#include <messageb.h>
#include "visiondg.h"
#include "vision.rh"
#include "setup.h"
#include "rdsetup.h"
#include "LeakDetect.h"
#include "stdio.h"
//#include <ocf/ocreg.h>

bool running;

//*** VisionApp ***
class VisionApp : public TApplication {
public:
  VisionApp(const char far *name = "Vision") : TApplication(name) {};
  virtual void InitMainWindow();
};

void VisionApp::InitMainWindow()  {
//  EnableBWCC();
//  EnableCtl3d();
  TMessageBar* mb = new TMessageBar();
  TVisionDialog* dialog=new TVisionDialog(mb,!running);
  TDecoratedFrame *frame = new TDecoratedFrame( 0,Name, dialog );
  frame->Insert(*mb, TDecoratedFrame::Bottom);
  frame->Attr.Style &=~(WS_MAXIMIZEBOX | WS_THICKFRAME );
  frame->Attr.W=WindowSize; //1980;//WindowSize;
  frame->Attr.H=WindowHeight;
  frame->Attr.X=WindowOriginX;
  frame->Attr.Y=WindowOriginY;   
  frame->SetIcon(this,"VisionIc");
  frame->AssignMenu(LASERMENU);
  MainWindow=frame;
}

int OwlMain (int , char* []) {
  HANDLE hMapping;
  running=false;
  hMapping = CreateFileMapping( (HANDLE) 0xffffffff,

                                 NULL,
                                 PAGE_READONLY,
                                 0,
                                 32,
                                 "Vision2TestMap" );
  if( hMapping ) {
    if( GetLastError() == ERROR_ALREADY_EXISTS ) {
      //MessageBox( NULL, "Vision is already running.", "Vision", MB_OK );
      running=true;
    }
  } else {
     MessageBox( NULL, "Error creating mapping", "Vision", MB_OK );
     ExitProcess(1);
  }
  int help;
  if (running) help=VisionApp("Vision Harvester").Run(); else help=VisionApp().Run();
//  LeakDetect.LeakDump("LeakDump.dat");
  return help;
}
