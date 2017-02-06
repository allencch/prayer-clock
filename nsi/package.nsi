Name "Prayer Clock"
OutFile "prayer-clock-setup.exe"

InstallDir "$PROGRAMFILES\Prayer Clock"

RequestExecutionLevel admin

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Var "Title"

Section "Prayer Clock (required)"
	StrCpy $Title "Prayer Clock"

	SectionIn RO
	
	SetOutPath $INSTDIR
	File /r ".\prayer-clock\*.*"
	
	; Write the installation path into the registry
	WriteRegStr HKLM "SOFTWARE\$Title" "Install_Dir" "$INSTDIR"
	
	; Start menu
	CreateDirectory "$SMPROGRAMS\$Title"
	CreateShortCut "$SMPROGRAMS\$Title\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
	CreateShortCut "$SMPROGRAMS\$Title\$Title.lnk" "$INSTDIR\bin\prayer-clock.exe" "" "$INSTDIR\bin\prayer-clock.exe" 0
  
	; Write the uninstall keys for Windows
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$Title" "DisplayName" "$Title"
	WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$Title" "UninstallString" '"$INSTDIR\uninstall.exe"'
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$Title" "NoModify" 1
	WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$Title" "NoRepair" 1
	WriteUninstaller "uninstall.exe"
SectionEnd

Section "Startup"
	StrCpy $Title "Prayer Clock"
	CreateShortCut "$SMSTARTUP\$Title.lnk" "$INSTDIR\bin\prayer-clock.exe" "" "$INSTDIR\bin\prayer-clock.exe" 0
SectionEnd

Section "Uninstall"
	StrCpy $Title "Prayer Clock"
	
	; Remove registry keys
	DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$Title"
	DeleteRegKey HKLM SOFTWARE\$Title
	
	; Remove files and uninstaller
	Delete $INSTDIR\*
	
	; Remove shortcuts, if any
	Delete "$SMPROGRAMS\$Title\*.*"
	
	; Remove startup
	Delete "$SMSTARTUP\$Title.lnk"
	
	; Remove directories used
	RMDir "$SMPROGRAMS\$Title"
	RMDir "$INSTDIR"
SectionEnd
