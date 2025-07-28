; IMPORTANT INFO ABOUT GETTING STARTED: Lines that start with a
; semicolon, such as this one, are comments.  They are not executed.

; This script has a special filename and path because it is automatically
; launched when you run the program directly.  Also, any text file whose
; name ends in .ahk is associated with the program, which means that it
; can be launched simply by double-clicking it.  You can have as many .ahk
; files as you want, located in any folder.  You can also run more than
; one ahk file simultaneously and each will get its own tray icon.

; SAMPLE HOTKEYS: Below are two sample hotkeys.  The first is Win+Z and it
; launches a web site in the default browser.  The second is Control+Alt+N
; and it launches a new Notepad window (or activates an existing one).  To
; try out these hotkeys, run AutoHotkey again, which will load this file.


^!h::
WinWait, Vision, 
IfWinNotActive, Vision Harvester, , WinActivate, Vision Harvester, 
WinWaitActive, Vision Harvester, 
Send +{PrintScreen}
Run, mspaint.exe
WinWait, untitled - Paint
WinMaximize
Send ^v
MouseClick, left,  17,  68
Sleep, 10
MouseClick, left,  42,  67
Sleep, 10
MouseClickDrag, left, 734,  646, 1206, 854
Sleep, 10
Send, {CTRLDOWN}c{CTRLUP}
Send, {CTRLDOWN}n{CTRLUP}
Send, {ALTDOWN}n{ALTUP}
Send, {CTRLDOWN}w{CTRLUP}
WinWait, Stretch and Skew, 
IfWinNotActive, Stretch and Skew, , WinActivate, Stretch and Skew, 
WinWaitActive, Stretch and Skew, 
Send, 1{BACKSPACE}10{TAB}10{ENTER}
Send, {CTRLDOWN}v{CTRLUP}
Send, {CTRLDOWN}p{CTRLUP}
WinWait, Print, 
IfWinNotActive, Print, , WinActivate, Print, 
WinWaitActive, Print, 
Send, {ALTDOWN}p{ALTUP}
WinWait, untitled - Paint, 
IfWinNotActive, untitled - Paint, , WinActivate, untitled - Paint, 
WinWaitActive, untitled - Paint, 
Send, {ALTDOWN}{F4}{ALTUP}
WinWait, Paint, 
IfWinNotActive, Paint, , WinActivate, Paint, 
WinWaitActive, Paint, 
Send, {ALTDOWN}n{ALTUP}
return

^!p::
WinWait, Vision, 
IfWinNotActive, Vision, , WinActivate, Vision, 
WinWaitActive, Vision, 
Send +{PrintScreen}
Run, mspaint.exe
WinWait, untitled - Paint
WinMaximize
Send ^v
MouseClick, left,  17,  68
Sleep, 10
MouseClick, left,  42,  67
Sleep, 10
MouseClickDrag, left, 734,  646, 1206, 854
Sleep, 10
Send, {CTRLDOWN}c{CTRLUP}
Send, {CTRLDOWN}n{CTRLUP}
Send, {ALTDOWN}n{ALTUP}
Send, {CTRLDOWN}w{CTRLUP}
WinWait, Stretch and Skew, 
IfWinNotActive, Stretch and Skew, , WinActivate, Stretch and Skew, 
WinWaitActive, Stretch and Skew, 
Send, 1{BACKSPACE}10{TAB}10{ENTER}
Send, {CTRLDOWN}v{CTRLUP}
Send, {CTRLDOWN}p{CTRLUP}
WinWait, Print, 
IfWinNotActive, Print, , WinActivate, Print, 
WinWaitActive, Print, 
Send, {ALTDOWN}p{ALTUP}
WinWait, untitled - Paint, 
IfWinNotActive, untitled - Paint, , WinActivate, untitled - Paint, 
WinWaitActive, untitled - Paint, 
Send, {ALTDOWN}{F4}{ALTUP}
WinWait, Paint, 
IfWinNotActive, Paint, , WinActivate, Paint, 
WinWaitActive, Paint, 
Send, {ALTDOWN}n{ALTUP}
return


^!c::
WinWait, Vision, 
IfWinNotActive, Vision, , WinActivate, Vision, 
WinWaitActive, Vision, 
Send +{PrintScreen}
Run, mspaint.exe
WinWait, untitled - Paint
WinMaximize
Send ^v
MouseClick, left,  17,  68
Sleep, 10
MouseClick, left,  42,  67
Sleep, 10
MouseClickDrag, left, 734,  646, 1206, 854
Sleep, 10
Send, {CTRLDOWN}c{CTRLUP}
WinWait, untitled - Paint, 
IfWinNotActive, untitled - Paint, , WinActivate, untitled - Paint, 
WinWaitActive, untitled - Paint, 
Send, {ALTDOWN}{F4}{ALTUP}
WinWait, Paint, 
IfWinNotActive, Paint, , WinActivate, Paint, 
WinWaitActive, Paint, 
Send, {ALTDOWN}n{ALTUP}
return

^!w::
WinWait, Vision, 
IfWinNotActive, Vision, , WinActivate, Vision, 
WinWaitActive, Vision, 
Send +{PrintScreen}
Run, mspaint.exe
WinWait, untitled - Paint
WinMaximize
Send ^v
MouseClick, left,  17,  68
Sleep, 10
MouseClick, left,  42,  67
Sleep, 10
MouseClickDrag, left, 1595,  399, 1590,  445
Sleep, 10
MouseClickDrag, left, 72,  53, 662,  1038
Sleep, 10
Send, {CTRLDOWN}c{CTRLUP}
Send, {CTRLDOWN}n{CTRLUP}
Send, {ALTDOWN}n{ALTUP}
Send, {CTRLDOWN}w{CTRLUP}
WinWait, Stretch and Skew, 
IfWinNotActive, Stretch and Skew, , WinActivate, Stretch and Skew, 
WinWaitActive, Stretch and Skew, 
Send, 1{BACKSPACE}10{TAB}10{ENTER}
Send, {CTRLDOWN}v{CTRLUP}
Send, {CTRLDOWN}p{CTRLUP}
WinWait, Print, 
IfWinNotActive, Print, , WinActivate, Print, 
WinWaitActive, Print, 
Sleep, 10
MouseClick, left,  402,  187
WinWait, Printing Preferences, 
IfWinNotActive, Printing Preferences, , WinActivate, Printing Preferences, 
WinWaitActive, Printing Preferences, 
MouseClick, left,  168,  45
Sleep, 100
MouseClick, left,  80,  255
Sleep, 100
Send, {BACKSPACE}{BACKSPACE}{BACKSPACE}50{ENTER}{ENTER}
WinWait, untitled - Paint, 
IfWinNotActive, untitled - Paint, , WinActivate, untitled - Paint, 
WinWaitActive, untitled - Paint, 
Send, {ALTDOWN}{F4}{ALTUP}
WinWait, Paint, 
IfWinNotActive, Paint, , WinActivate, Paint, 
WinWaitActive, Paint, 
Send, {ALTDOWN}n{ALTUP}
return

^!f::
WinWait, Vision, 
IfWinNotActive, Vision, , WinActivate, Vision, 
WinWaitActive, Vision, 
Send +{PrintScreen}
Run, mspaint.exe
WinWait, untitled - Paint
WinMaximize
Send ^v
MouseClick, left,  17,  68
Sleep, 10
MouseClick, left,  42,  67
Sleep, 10
MouseClickDrag, left, 73,  109, 663,  871
Sleep, 10
Send, {CTRLDOWN}c{CTRLUP}
Send, {CTRLDOWN}n{CTRLUP}
Send, {ALTDOWN}n{ALTUP}
Send, {CTRLDOWN}w{CTRLUP}
WinWait, Stretch and Skew, 
IfWinNotActive, Stretch and Skew, , WinActivate, Stretch and Skew, 
WinWaitActive, Stretch and Skew, 
Send, 1{BACKSPACE}10{TAB}10{ENTER}
Send, {CTRLDOWN}v{CTRLUP}
Send, {CTRLDOWN}p{CTRLUP}
WinWait, Print, 
IfWinNotActive, Print, , WinActivate, Print, 
WinWaitActive, Print, 
Sleep, 10
MouseClick, left,  402,  187
WinWait, Printing Preferences, 
IfWinNotActive, Printing Preferences, , WinActivate, Printing Preferences, 
WinWaitActive, Printing Preferences, 
MouseClick, left,  168,  45
Sleep, 100
MouseClick, left,  80,  255
Sleep, 100
Send, {BACKSPACE}{BACKSPACE}{BACKSPACE}50{ENTER}{ENTER}
WinWait, untitled - Paint, 
IfWinNotActive, untitled - Paint, , WinActivate, untitled - Paint, 
WinWaitActive, untitled - Paint, 
Send, {ALTDOWN}{F4}{ALTUP}
WinWait, Paint, 
IfWinNotActive, Paint, , WinActivate, Paint, 
WinWaitActive, Paint, 
Send, {ALTDOWN}n{ALTUP}
return

^!t::
WinWait, Vision, 
IfWinNotActive, Vision, , WinActivate, Vision, 
WinWaitActive, Vision, 
Send +{PrintScreen}
Run, mspaint.exe
WinWait, untitled - Paint
WinMaximize
Send ^v
MouseClick, left,  17,  68
Sleep, 10
MouseClick, left,  42,  67
Sleep, 10
MouseClick, left,  1531,  1092
Sleep, 100
MouseClickDrag, left, 1595,  169, 1595,  215
Sleep, 10
MouseClickDrag, left, 1126,  54, 1575,  1080
Sleep, 10
Send, {CTRLDOWN}c{CTRLUP}
Send, {CTRLDOWN}n{CTRLUP}
Send, {ALTDOWN}n{ALTUP}
Send, {CTRLDOWN}w{CTRLUP}
WinWait, Stretch and Skew, 
IfWinNotActive, Stretch and Skew, , WinActivate, Stretch and Skew, 
WinWaitActive, Stretch and Skew, 
Send, 1{BACKSPACE}10{TAB}10{ENTER}
Send, {CTRLDOWN}v{CTRLUP}
Send, {CTRLDOWN}w{CTRLUP}
WinWait, Stretch and Skew, 
IfWinNotActive, Stretch and Skew, , WinActivate, Stretch and Skew, 
WinWaitActive, Stretch and Skew, 
Send, 1{BACKSPACE}80{TAB}80{ENTER}
Send, {CTRLDOWN}c{CTRLUP}
Send, {CTRLDOWN}n{CTRLUP}
Send, {ALTDOWN}n{ALTUP}
Send, {CTRLDOWN}v{CTRLUP}
Send, {CTRLDOWN}p{CTRLUP}
WinWait, Print, 
IfWinNotActive, Print, , WinActivate, Print, 
WinWaitActive, Print, 
Send, {ALTDOWN}p{ALTUP}
WinWait, untitled - Paint, 
IfWinNotActive, untitled - Paint, , WinActivate, untitled - Paint, 
WinWaitActive, untitled - Paint, 
Send, {ALTDOWN}{F4}{ALTUP}
WinWait, Paint, 
IfWinNotActive, Paint, , WinActivate, Paint, 
WinWaitActive, Paint, 
Send, {ALTDOWN}n{ALTUP}
return

^!e::
WinWait, Vision, 
IfWinNotActive, Vision, , WinActivate, Vision, 
WinWaitActive, Vision, 
Send +{PrintScreen}
Run, mspaint.exe
WinWait, untitled - Paint
WinMaximize
Send ^v
MouseClick, left,  17,  68
Sleep, 10
MouseClick, left,  42,  67
Sleep, 10
MouseClick, left,  1531,  1092
Sleep, 100
MouseClickDrag, left, 1595,  169, 1595,  215
Sleep, 10
MouseClickDrag, left, 1126,  54, 1575,  1080
Sleep, 10
Send, {CTRLDOWN}c{CTRLUP}
Send, {CTRLDOWN}n{CTRLUP}
Send, {ALTDOWN}n{ALTUP}
Send, {CTRLDOWN}w{CTRLUP}
WinWait, Stretch and Skew, 
IfWinNotActive, Stretch and Skew, , WinActivate, Stretch and Skew, 
WinWaitActive, Stretch and Skew, 
Send, 1{BACKSPACE}10{TAB}10{ENTER}
Send, {CTRLDOWN}v{CTRLUP}
Send, {CTRLDOWN}p{CTRLUP}
WinWait, Print, 
IfWinNotActive, Print, , WinActivate, Print, 
WinWaitActive, Print, 
Sleep, 10
MouseClick, left,  402,  187
WinWait, Printing Preferences, 
IfWinNotActive, Printing Preferences, , WinActivate, Printing Preferences, 
WinWaitActive, Printing Preferences, 
MouseClick, left,  168,  45
Sleep, 100
MouseClick, left,  80,  255
Sleep, 100
Send, {BACKSPACE}{BACKSPACE}{BACKSPACE}80{ENTER}{ENTER}
WinWait, untitled - Paint, 
IfWinNotActive, untitled - Paint, , WinActivate, untitled - Paint, 
WinWaitActive, untitled - Paint, 
Send, {ALTDOWN}{F4}{ALTUP}
WinWait, Paint, 
IfWinNotActive, Paint, , WinActivate, Paint, 
WinWaitActive, Paint, 
Send, {ALTDOWN}n{ALTUP}
return

; Note: From now on whenever you run AutoHotkey directly, this script
; will be loaded.  So feel free to customize it to suit your needs.

; Please read the QUICK-START TUTORIAL near the top of the help file.
; It explains how to perform common automation tasks such as sending
; keystrokes and mouse clicks.  It also explains more about hotkeys.

