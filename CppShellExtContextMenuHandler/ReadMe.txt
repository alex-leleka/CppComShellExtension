
/////////////////////////////////////////////////////////////////////////////
Setup and Removal:

A. Setup

If you are going to use the Shell extension in a x64 Windows system, please 
configure the Visual C++ project to target 64-bit platforms using project 
configurations (http://msdn.microsoft.com/en-us/library/9yb4317s.aspx). Only 
64-bit extension DLLs can be loaded in the 64-bit Windows Shell. 

If the extension is to be loaded in a 32-bit Windows system, you can use the 
default Win32 project configuration to build the project.

In a command prompt running as administrator, navigate to the folder that 
contains the build result CppShellExtContextMenuHandler.dll and enter the 
command:

    Regsvr32.exe CppShellExtContextMenuHandler.dll

The context menu handler is registered successfully if you see a message box 
saying:

    "DllRegisterServer in CppShellExtContextMenuHandler.dll succeeded."

B. Removal

In a command prompt running as administrator, navigate to the folder that 
contains the build result CppShellExtContextMenuHandler.dll and enter the 
command:

    Regsvr32.exe /u CppShellExtContextMenuHandler.dll

The context menu handler is unregistered successfully if you see a message 
box saying:

    "DllUnregisterServer in CppShellExtContextMenuHandler.dll succeeded."

/////////////////////////////////////////////////////////////////////////////