#include <Windows.h>
//#include <Shlwapi.h>
#include "ContextMenu.h"

ContextMenu::ContextMenu(PCIDLIST_ABSOLUTE folder, IDataObject *dataObj)
{
	//IShellFolder* shellFolder;
	//SHGetDesktopFolder(&shellFolder);

	//STRRET folderPath;
	//shellFolder->GetDisplayNameOf(folder, SHGDN_NORMAL | SHGDN_FORPARSING, &folderPath);
	//shellFolder->Release();

	//char* displayName;
	//StrRetToStr(&folderPath, folder, &displayName);

	//CoTaskMemFree(displayName);

	char buf[MAX_PATH];
	unsigned long size = sizeof(buf) / sizeof(buf[0]);
	RegGetValue(HKEY_CLASSES_ROOT, "CLSID\\{315A6644-9940-435A-A4AA-AA177CFB6214}\\InProcServer32", NULL, RRF_RT_ANY, NULL, buf, &size);

	std::string dllFullPath = buf;
	binDir = dllFullPath.substr(0, dllFullPath.rfind('\\') + 1);

	FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	dataObj->GetData(&fmt, &stg);
	HDROP drop = (HDROP)GlobalLock(stg.hGlobal);

	int numFiles = DragQueryFile(drop, 0xFFFFFFFF, 0, 0);

	for (int i = 0; i < numFiles; i++)
	{
		char buf[MAX_PATH];
		DragQueryFile(drop, i, buf, sizeof(buf) / sizeof(buf[0]));

		std::string fullPath(buf);
		int d = fullPath.rfind('.');
		std::string postfix = fullPath.substr(d != -1 ? d+1 : fullPath.length());

		if (postfix == "tga"
			|| postfix == "TGA"
			)
		{
			std::string fileName = fullPath.substr(fullPath.rfind('\\') + 1);
			selectedTextureFiles.push_back(fileName);

			workingDir = fullPath.substr(0, fullPath.rfind('\\') + 1);
		}
	}

	if (selectedTextureFiles.size())
	{
		MenuItem menuItem1 = { "创建用于模型的纹理贴图资源包", &ContextMenu::makeTextureMapPackageForMesh };
		menuItemList.push_back(menuItem1);
	}
}

void ContextMenu::makeTextureMapPackageForMesh()
{
	for (std::string src : selectedTextureFiles)
	{
		std::string name = src.substr(0, src.rfind('.'));
		std::string etc_ktx = name + ".etc.ktx";
		std::string dxt_ktx = name + ".dxt.ktx";
		std::string etc_pnt = name + ".etc.PNT";
		std::string dxt_pnt = name + ".dxt.PNT";
		std::string package = name + ".TPM";

		PROCESS_INFORMATION pi1, pi2;

		{
			char command[512];
			sprintf_s(command, "%sPVRTexToolCLI.exe -m -f ETC2_RGB -q etcfastperceptual -i %s -o %s", binDir.c_str(), src.c_str(), etc_ktx.c_str());

			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);

			CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, workingDir.c_str(), &si, &pi1);
		}

		{
			char command[512];
			sprintf_s(command, "%sPVRTexToolCLI.exe -m -f BC1 -i %s -o %s", binDir.c_str(), src.c_str(), dxt_ktx.c_str());

			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);

			CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, workingDir.c_str(), &si, &pi2);
		}

		WaitForSingleObject(pi1.hProcess, INFINITE);
		CloseHandle(pi1.hProcess);

		WaitForSingleObject(pi2.hProcess, INFINITE);
		CloseHandle(pi2.hProcess);

		{
			char command[512];
			sprintf_s(command, "%sktx2pnt.exe %s %s", binDir.c_str(), etc_ktx.c_str(), etc_pnt.c_str());

			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);

			CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, workingDir.c_str(), &si, &pi1);
		}

		{
			char command[512];
			sprintf_s(command, "%sktx2pnt.exe %s %s", binDir.c_str(), dxt_ktx.c_str(), dxt_pnt.c_str());

			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);

			CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, workingDir.c_str(), &si, &pi2);
		}

		WaitForSingleObject(pi1.hProcess, INFINITE);
		CloseHandle(pi1.hProcess);

		WaitForSingleObject(pi2.hProcess, INFINITE);
		CloseHandle(pi2.hProcess);

		{
			char command[512];
			sprintf_s(command, "%sminizip.exe -9 %s %s %s %s", binDir.c_str(), package.c_str(), src.c_str(), etc_pnt.c_str(), dxt_pnt.c_str());

			STARTUPINFO si;
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);

			CreateProcess(NULL, command, NULL, NULL, FALSE, 0, NULL, workingDir.c_str(), &si, &pi1);
		}

		WaitForSingleObject(pi1.hProcess, INFINITE);
		CloseHandle(pi1.hProcess);

		std::string fullName;

		fullName = workingDir + etc_ktx;
		DeleteFile(fullName.c_str());

		fullName = workingDir + dxt_ktx;
		DeleteFile(fullName.c_str());

		fullName = workingDir + etc_pnt;
		DeleteFile(fullName.c_str());

		fullName = workingDir + dxt_pnt;
		DeleteFile(fullName.c_str());
	}
}
