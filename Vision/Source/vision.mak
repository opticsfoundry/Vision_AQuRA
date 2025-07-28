#
# Borland C++ IDE generated makefile
# Generated 10/26/2012 at 4:15:20 PM 
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCC32   = Bcc32 +BccW32.cfg 
BCC32I  = Bcc32i +BccW32.cfg 
TLINK32 = TLink32
ILINK32 = Ilink32
TLIB    = TLib
BRC32   = Brc32
TASM32  = Tasm32
#
# IDE macros
#


#
# Options
#
IDE_LinkFLAGS32 =  -L..\..\..\..\LIB
IDE_ResFLAGS32 = 
LinkerLocalOptsAtW32_CcbSRBECbVISIONSRBECbEXEbvisiondexe =  -LC:\BC5\LIB -Tpe -aa -V4.0 -c
ResLocalOptsAtW32_CcbSRBECbVISIONSRBECbEXEbvisiondexe = 
BLocalOptsAtW32_CcbSRBECbVISIONSRBECbEXEbvisiondexe = 
CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe = -IC:\BC5\INCLUDE;C:\BC5\INCLUDE\OWL;C:\BC5\INCLUDE\CLASSLIB -D_RTLDLL;_BIDSDLL;_OWLDLL;STRICT;_OWLPCH;_OWLALLPCH;
LinkerInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe = -x
LinkerOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe = $(LinkerLocalOptsAtW32_CcbSRBECbVISIONSRBECbEXEbvisiondexe)
ResOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe = $(ResLocalOptsAtW32_CcbSRBECbVISIONSRBECbEXEbvisiondexe)
BOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe = $(BLocalOptsAtW32_CcbSRBECbVISIONSRBECbEXEbvisiondexe)

#
# Dependency List
#
Dep_vision = \
   C:\SRBEC\VISIONSRBEC\EXE\vision.exe

vision : BccW32.cfg $(Dep_vision)
  echo MakeNode

Dep_CcbSRBECbVISIONSRBECbEXEbvisiondexe = \
   C:\SRBEC\VISIONSRBEC\OBJ\comm.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\netandor.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\leakdetect.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\names.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\network.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\measure.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\picture.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\serialnet.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\rdsetup.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\densprof.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\params.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\messpoint.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\point.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\butonbox.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\tga.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\framegbr.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\vision.res\
   C:\SRBEC\VISIONSRBEC\OBJ\textbox.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\serial.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\scale.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\profil.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\picturew.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\piclist.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\fitmath.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\colortab.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\bmp.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\atommath.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\vision.obj\
   C:\SRBEC\VISIONSRBEC\OBJ\visiondg.obj

C:\SRBEC\VISIONSRBEC\EXE\vision.exe : $(Dep_CcbSRBECbVISIONSRBECbEXEbvisiondexe)
  $(ILINK32) @&&|
 /v $(IDE_LinkFLAGS32) $(LinkerOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(LinkerInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) +
C:\BC5\LIB\c0w32.obj+
C:\SRBEC\VISIONSRBEC\OBJ\comm.obj+
C:\SRBEC\VISIONSRBEC\OBJ\netandor.obj+
C:\SRBEC\VISIONSRBEC\OBJ\leakdetect.obj+
C:\SRBEC\VISIONSRBEC\OBJ\names.obj+
C:\SRBEC\VISIONSRBEC\OBJ\network.obj+
C:\SRBEC\VISIONSRBEC\OBJ\measure.obj+
C:\SRBEC\VISIONSRBEC\OBJ\picture.obj+
C:\SRBEC\VISIONSRBEC\OBJ\serialnet.obj+
C:\SRBEC\VISIONSRBEC\OBJ\rdsetup.obj+
C:\SRBEC\VISIONSRBEC\OBJ\densprof.obj+
C:\SRBEC\VISIONSRBEC\OBJ\params.obj+
C:\SRBEC\VISIONSRBEC\OBJ\messpoint.obj+
C:\SRBEC\VISIONSRBEC\OBJ\point.obj+
C:\SRBEC\VISIONSRBEC\OBJ\butonbox.obj+
C:\SRBEC\VISIONSRBEC\OBJ\tga.obj+
C:\SRBEC\VISIONSRBEC\OBJ\framegbr.obj+
C:\SRBEC\VISIONSRBEC\OBJ\textbox.obj+
C:\SRBEC\VISIONSRBEC\OBJ\serial.obj+
C:\SRBEC\VISIONSRBEC\OBJ\scale.obj+
C:\SRBEC\VISIONSRBEC\OBJ\profil.obj+
C:\SRBEC\VISIONSRBEC\OBJ\picturew.obj+
C:\SRBEC\VISIONSRBEC\OBJ\piclist.obj+
C:\SRBEC\VISIONSRBEC\OBJ\fitmath.obj+
C:\SRBEC\VISIONSRBEC\OBJ\colortab.obj+
C:\SRBEC\VISIONSRBEC\OBJ\bmp.obj+
C:\SRBEC\VISIONSRBEC\OBJ\atommath.obj+
C:\SRBEC\VISIONSRBEC\OBJ\vision.obj+
C:\SRBEC\VISIONSRBEC\OBJ\visiondg.obj
$<,$*
C:\BC5\LIB\owlwfi.lib+
C:\BC5\LIB\bidsfi.lib+
C:\BC5\LIB\import32.lib+
C:\BC5\LIB\cw32i.lib

C:\SRBEC\VISIONSRBEC\OBJ\vision.res

|
C:\SRBEC\VISIONSRBEC\OBJ\comm.obj :  comm.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ comm.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\netandor.obj :  netandor.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ netandor.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\leakdetect.obj :  leakdetect.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ leakdetect.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\names.obj :  names.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ names.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\network.obj :  network.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ network.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\measure.obj :  measure.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ measure.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\picture.obj :  picture.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ picture.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\serialnet.obj :  serialnet.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ serialnet.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\rdsetup.obj :  rdsetup.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ rdsetup.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\densprof.obj :  densprof.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ densprof.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\params.obj :  params.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ params.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\messpoint.obj :  messpoint.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ messpoint.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\point.obj :  point.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ point.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\butonbox.obj :  butonbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ butonbox.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\tga.obj :  tga.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ tga.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\framegbr.obj :  framegbr.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ framegbr.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\vision.res :  vision.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe)  -FO$@ vision.rc
|
C:\SRBEC\VISIONSRBEC\OBJ\winsock.res :  ..\..\..\bc5\include\owl\winsock.rc
  $(BRC32) -R @&&|
 $(IDE_ResFLAGS32) $(ROptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe)  -FO$@ ..\..\..\bc5\include\owl\winsock.rc
|
C:\SRBEC\VISIONSRBEC\OBJ\textbox.obj :  textbox.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ textbox.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\serial.obj :  serial.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ serial.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\scale.obj :  scale.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ scale.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\profil.obj :  profil.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ profil.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\picturew.obj :  picturew.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ picturew.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\piclist.obj :  piclist.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ piclist.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\fitmath.obj :  fitmath.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ fitmath.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\colortab.obj :  colortab.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ colortab.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\bmp.obj :  bmp.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ bmp.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\atommath.obj :  atommath.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ atommath.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\vision.obj :  vision.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ vision.cpp
|

C:\SRBEC\VISIONSRBEC\OBJ\visiondg.obj :  visiondg.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) $(CompInheritOptsAt_CcbSRBECbVISIONSRBECbEXEbvisiondexe) -o$@ visiondg.cpp
|

#Compiler configuration file
BccW32.cfg : 
   Copy &&|
-w
-R
-v
-WM-
-vi
-H
-H=meteor.csm
-K2-
-OS
-Og
-5
-a-
-W
-H"owl\pch.h"
-R-
-H=VISION.CSM
| $@


