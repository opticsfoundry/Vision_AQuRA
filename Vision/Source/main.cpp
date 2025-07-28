#include <owl/applicat.h>
#include <owl/framewin.h>

class TOWLNextApp : public TApplication
{
  public:
    TOWLNextApp() : TApplication() {}

    void InitMainWindow()
    {
        SetMainWindow(new TFrameWindow(0, _T("Sample ObjectWindows Program")));
    }
};

int
OwlMain(int /* argc */, TCHAR* /* argv */ [])
{
  return TOWLNextApp().Run();
}