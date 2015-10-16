
;Copyright 1992-2009 Systems Planning (www.systemsplanning.com)
;
;Licensed under the Apache License, Version 2.0 (the "License"); 
;you may not use this file except in compliance with the License. 
;You may obtain a copy of the License at 
;
;    http://www.apache.org/licenses/LICENSE-2.0
;
;Unless required by applicable law or agreed to in writing, software 
;distributed under the License is distributed on an "AS IS" BASIS, 
;WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
;See the License for the specific language governing permissions and 
;limitations under the License.


;NSIS Modern User Interface

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"
  !define MUI_ICON "d:\marc\marcview\MARCView.ico"
  !define MUI_UNICON "d:\marc\marcview\MARCView.ico"

;--------------------------------
;General -- set attribute values (these are attributes, not functions)

  ;Name and file
  Name "MARCView"
  OutFile "MARCView_3121.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Systems Planning\MARCView"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Systems Planning\MARCView" ""

  BrandingText "MARCView™ Installer"

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "MARCView\MV_License.rtf"
  ; not needed !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections -- each is a component the user can choose. since we
;	don't allow choices, everything is in one section

Section "Main Section" SecMain

  ; we don't create an INI for MV during install. see an MC NSI file for how if needed

  SetOutPath "$INSTDIR"

  File  "MARCView\Brkrtest.mrc"
  File  "MARCView\Collection.xml"
  File	"MARCView\Lc_utf8.mrc"
  File  "MARCView\MARCView.exe"
  File  "MARCView\MV_License.rtf"
  File  "MARCView\MV_Readme.txt"
  File  "MARCView\SP_MARC.dll"

  ;Store installation folder
  ;080302 try to have it appear in Open With
  ;WriteRegStr HKCU "Software\MARCView" "" $INSTDIR
  WriteRegStr HKCU "Software\Systems Planning\MARCView" "" $INSTDIR

  ; write to add/remove pgms
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Systems Planning MARCView" \
      "DisplayName" "Systems Planning MARCView"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Systems Planning MARCView" \
      "UninstallString" "$INSTDIR\Uninstall.exe"

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

;060112 start button
CreateDirectory "$SMPROGRAMS\Systems Planning"
CreateShortCut "$SMPROGRAMS\Systems Planning\MARCView.lnk" "$INSTDIR\MARCView.exe"

SectionEnd

;--------------------------------
;Descriptions

  ;Language strings -- like resource strings
  ; params are name, language, text
  LangString DESC_SecMain ${LANG_ENGLISH} "Main section."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete  "$INSTDIR\Brkrtest.mrc"
  Delete  "$INSTDIR\Collection.xml"
  Delete  "$INSTDIR\Lc_utf8.mrc"
  Delete  "$INSTDIR\MARCView.exe"
  Delete  "$INSTDIR\MV_License.rtf"
  Delete  "$INSTDIR\MV_Readme.txt"
  Delete  "$INSTDIR\SP_MARC.dll"

  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR"

  DeleteRegKey /ifempty HKCU "Software\Systems Planning\MARCView"

  ; remove from add/remove pgms
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Systems Planning MARCView"

SectionEnd
