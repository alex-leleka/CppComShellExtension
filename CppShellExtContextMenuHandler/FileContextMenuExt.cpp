/****************************** Module Header ******************************\
Module Name:  FileContextMenuExt.cpp
Project:      CppShellExtContextMenuHandler

This module implements class FileContextMenuExt with IUnknown, IShellExtInit, 
IContextMenu interfaces.  It creates shell contex menu handler. This module 
defines when context menu item will be shown and reaction on item click.


\***************************************************************************/

#include "FileContextMenuExt.h"
#include "..//LogFileWriter//LogFileWriter.h"
#include "resource.h"
#include <strsafe.h>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")


extern HINSTANCE g_hInst;
extern long g_cDllRef;

#define IDM_DISPLAY             0  // The command's identifier offset

FileContextMenuExt::FileContextMenuExt(void) : m_cRef(1), 
    m_pszMenuText(L"&Create Log"),
    m_pszVerb("create_log_file"),
    m_pwszVerb(L"create_log_file"),
    m_pszVerbCanonicalName("CreateLogShellExt"),
    m_pwszVerbCanonicalName(L"CreateLogShellExt"),
    m_pszVerbHelpText("Create Log"),
    m_pwszVerbHelpText(L"Create Log")
{
	m_pDataObj = 0;
    InterlockedIncrement(&g_cDllRef);
}

FileContextMenuExt::~FileContextMenuExt(void)
{
    InterlockedDecrement(&g_cDllRef);
}


void FileContextMenuExt::OnVerbCreateLogFile(HWND hWnd)
{
	if(0 == m_pDataObj)
		return;
	FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stm;

    if ((m_pDataObj->GetData(&fe, &stm)) >= 0)
    {
        // The HDROP handle of  for enumerating the selected files and 
		// folders.
        HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop != NULL)
        {
            UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			if(nFiles < 1)
				return;
			wchar_t ** fileNames = new wchar_t*[nFiles];
			for(UINT i = 0; i < nFiles; i++)
			{
				UINT file_name_buf_size =  DragQueryFile(hDrop, i, NULL, 0); //DragQueryFile returns the required size, in characters.
				fileNames[i] = new wchar_t[file_name_buf_size + 1]; // create file name buffer + 1 for ending zero
				if(file_name_buf_size == DragQueryFile(hDrop, i, fileNames[i], file_name_buf_size + 1))
					continue;
			}
			LogFileWriter logWriter = LogFileWriter(fileNames, nFiles);

			wchar_t * logName = L"FilesInfoShExtLog.log";
			logWriter.CreateAndWrite(logName);
			// free files name buffers
			for(UINT i = 0; i < nFiles; i++)
			{
				delete[] fileNames[i];
			}
			delete[] fileNames;
            GlobalUnlock(stm.hGlobal);
        }

        ReleaseStgMedium(&stm);
    }
}


#pragma region IUnknown

// Query to the interface the component supported.
IFACEMETHODIMP FileContextMenuExt::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(FileContextMenuExt, IContextMenu),
        QITABENT(FileContextMenuExt, IShellExtInit), 
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

// Increase the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) FileContextMenuExt::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

// Decrease the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) FileContextMenuExt::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

#pragma endregion


#pragma region IShellExtInit

// Initialize the context menu handler.
IFACEMETHODIMP FileContextMenuExt::Initialize(
    LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hKeyProgID)
{
    if (NULL == pDataObj)
    {
        return E_INVALIDARG;
    }

    HRESULT hr = E_FAIL;

    FORMATETC fe = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM stm;

    // The pDataObj pointer contains the objects being acted upon.
    if (SUCCEEDED(pDataObj->GetData(&fe, &stm)))
    {
        // Get an HDROP handle for enumerating the selected files.
        HDROP hDrop = static_cast<HDROP>(GlobalLock(stm.hGlobal));
        if (hDrop != NULL)
        {
            // Determine how many files are involved in this operation.
            UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
            if (nFiles >= 1)
            {
                hr = S_OK;
				m_pDataObj = pDataObj;
            }

            GlobalUnlock(stm.hGlobal);
        }

        ReleaseStgMedium(&stm);
    }

    // If any value other than S_OK is returned from the method, the context 
    // menu item is not displayed.
    return hr;
}

#pragma endregion


#pragma region IContextMenu

//
//   FUNCTION: FileContextMenuExt::QueryContextMenu
//
//   PURPOSE: The Shell calls IContextMenu::QueryContextMenu to allow the 
//            context menu handler to add its menu items to the menu. It 
//            passes in the HMENU handle in the hmenu parameter. The 
//            indexMenu parameter is set to the index to be used for the 
//            first menu item that is to be added.
//
IFACEMETHODIMP FileContextMenuExt::QueryContextMenu(
    HMENU hMenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    // If uFlags include CMF_DEFAULTONLY then we should not do anything.
    if (CMF_DEFAULTONLY & uFlags)
    {
        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
    }

    MENUITEMINFO mii = { sizeof(mii) };
    mii.fMask = MIIM_BITMAP | MIIM_STRING | MIIM_FTYPE | MIIM_ID | MIIM_STATE;
    mii.wID = idCmdFirst + IDM_DISPLAY;
    mii.fType = MFT_STRING;
    mii.dwTypeData = m_pszMenuText;
    mii.fState = MFS_ENABLED;
    mii.hbmpItem = NULL;
    if (!InsertMenuItem(hMenu, indexMenu, TRUE, &mii))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // Add a separator.
    MENUITEMINFO sep = { sizeof(sep) };
    sep.fMask = MIIM_TYPE;
    sep.fType = MFT_SEPARATOR;
    if (!InsertMenuItem(hMenu, indexMenu + 1, TRUE, &sep))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // Return an HRESULT value with the severity set to SEVERITY_SUCCESS. 
    // Set the code value to the offset of the largest command identifier 
    // that was assigned, plus one (1).
    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(IDM_DISPLAY + 1));
}


//
//   FUNCTION: FileContextMenuExt::InvokeCommand
//
//   PURPOSE: This method is called when a user clicks a menu item to tell 
//            the handler to run the associated command. The lpcmi parameter 
//            points to a structure that contains the needed information.
//
IFACEMETHODIMP FileContextMenuExt::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{
    BOOL fUnicode = FALSE;

    // Determine which structure is being passed in, CMINVOKECOMMANDINFO or 
    // CMINVOKECOMMANDINFOEX based on the cbSize member of lpcmi. Although 
    // the lpcmi parameter is declared in Shlobj.h as a CMINVOKECOMMANDINFO 
    // structure, in practice it often points to a CMINVOKECOMMANDINFOEX 
    // structure. This struct is an extended version of CMINVOKECOMMANDINFO 
    // and has additional members that allow Unicode strings to be passed.
    if (pici->cbSize == sizeof(CMINVOKECOMMANDINFOEX))
    {
        if (pici->fMask & CMIC_MASK_UNICODE)
        {
            fUnicode = TRUE;
        }
    }

    // Determines whether the command is identified by its offset or verb.
    // For the ANSI case, if the high-order word is not zero, the command's 
    // verb string is in lpcmi->lpVerb. 
    if (!fUnicode && HIWORD(pici->lpVerb))
    {
        // Is the verb supported by this context menu extension?
        if (StrCmpIA(pici->lpVerb, m_pszVerb) == 0)
        {
            OnVerbCreateLogFile(pici->hwnd);
        }
        else
        {
            // If the verb is not recognized by the context menu handler, it 
            // must return E_FAIL to allow it to be passed on to the other 
            // context menu handlers that might implement that verb.
            return E_FAIL;
        }
    }

    // For the Unicode case, if the high-order word is not zero, the 
    // command's verb string is in lpcmi->lpVerbW. 
    else if (fUnicode && HIWORD(((CMINVOKECOMMANDINFOEX*)pici)->lpVerbW))
    {
        // Is the verb supported by this context menu extension?
        if (StrCmpIW(((CMINVOKECOMMANDINFOEX*)pici)->lpVerbW, m_pwszVerb) == 0)
        {
            OnVerbCreateLogFile(pici->hwnd);
        }
        else
        {
            // If the verb is not recognized by the context menu handler, it 
            // must return E_FAIL to allow it to be passed on to the other 
            // context menu handlers that might implement that verb.
            return E_FAIL;
        }
    }

    // If the command cannot be identified through the verb string, then 
    // check the identifier offset.
    else
    {
        // Is the command identifier offset supported by this context menu 
        // extension?
        if (LOWORD(pici->lpVerb) == IDM_DISPLAY)
        {
            OnVerbCreateLogFile(pici->hwnd);
        }
        else
        {
            // If the verb is not recognized by the context menu handler, it 
            // must return E_FAIL to allow it to be passed on to the other 
            // context menu handlers that might implement that verb.
            return E_FAIL;
        }
    }

    return S_OK;
}


//
//   FUNCTION: CFileContextMenuExt::GetCommandString
//
//   PURPOSE: If a user highlights one of the items added by a context menu 
//            handler, the handler's IContextMenu::GetCommandString method is 
//            called to request a Help text string that will be displayed on 
//            the Windows Explorer status bar. This method can also be called 
//            to request the verb string that is assigned to a command.
//
IFACEMETHODIMP FileContextMenuExt::GetCommandString(UINT_PTR idCommand, 
    UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
    HRESULT hr = E_INVALIDARG;

    if (idCommand == IDM_DISPLAY)
    {
        switch (uFlags)
        {
        case GCS_HELPTEXTW:
            // Only useful for pre-Vista versions of Windows that have a 
            // Status bar.
            hr = StringCchCopy(reinterpret_cast<PWSTR>(pszName), cchMax, 
                m_pwszVerbHelpText);
            break;

        case GCS_VERBW:
            // GCS_VERBW is an optional feature that enables a caller to 
            // discover the canonical name for the verb passed in through 
            // idCommand.
            hr = StringCchCopy(reinterpret_cast<PWSTR>(pszName), cchMax, 
                m_pwszVerbCanonicalName);
            break;

        default:
            hr = S_OK;
        }
    }

    // If the command (idCommand) is not supported by this context menu 
    // extension handler, return E_INVALIDARG.
    return hr;
}

#pragma endregion