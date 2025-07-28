Changes:
- In visiondlg.cpp: verify all sqrt() commands and assure that argument is >0
- In measure.cpp: Load(): Add code to load from arbitrary directory
		char buf[200];
      strcpy(buf,hpoints->GetText("Filename"));
		int i=strlen(buf)-2;
	   while ((i>0) && (buf[i]!='\\')) i--;
      char help[200];
      char help2[200];
      strcpy(help,path);
      strcpy(help2,&buf[i+1]);
      strcat(help,help2);
      hpoints->SetText("Filename",help);
- In picture.cpp: GetBitmap changed to inculde Zoom and Averaging over Pixels
    visindlg.cpp and the menu has to be modified accordingly
- In point.cpp: CMyPoint::GetMinMaxXY() to avoid stalling program because of
   endless loop. visiondlg.cpp has to be changed accordingly
- In point.cpp: Save: change %12g to %12.10g twice
- Apogee camera included   
