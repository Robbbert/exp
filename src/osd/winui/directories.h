// For licensing and usage information, read docs/winui_license.txt
//****************************************************************************

#ifndef DIRECTORIES_H
#define DIRECTORIES_H

/* Dialog return codes */
#define DIRDLG_ROMS         0x0010
#define DIRDLG_SAMPLES      0x0020
#define DIRDLG_INI          0x0040
#define DIRDLG_CFG          0x0100
#define DIRDLG_IMG          0x0400
#define DIRDLG_INP          0x0800
#define DIRDLG_CTRLR        0x1000
#define DIRDLG_SOFTWARE     0x2000
#define DIRDLG_COMMENT      0x4000
#define DIRDLG_CHEAT        0x8000

#define DIRLIST_NEWENTRYTEXT "<               >"

#include "mui_opts.h"
#include "optionsms.h"

typedef struct
{
	LPCSTR   lpName;
	LPCSTR   (*pfnGetTheseDirs)(void);
	void     (*pfnSetTheseDirs)(LPCSTR lpDirs);
	BOOL     bMulti;
	int      nDirDlgFlags;
}
DIRECTORYINFO;

const DIRECTORYINFO g_directoryInfo[] =
{
	{ "ROMs",                  GetRomDirs,         SetRomDirs,         TRUE,  DIRDLG_ROMS },
	{ "Samples",               GetSampleDirs,      SetSampleDirs,      TRUE,  DIRDLG_SAMPLES },
	{ "Software",              GetSoftwareDirs,    SetSoftwareDirs,    TRUE,  DIRDLG_SOFTWARE },
	{ "Artwork",               GetArtDir,          SetArtDir,          TRUE, 0 },
	{ "Cabinets",              GetCabinetDir,      SetCabinetDir,      FALSE, 0 },
	{ "Cheats",                GetCheatDir,        SetCheatDir,        TRUE, DIRDLG_CHEAT },
	{ "Comment Files",         GetCommentDir,      SetCommentDir,      TRUE, DIRDLG_COMMENT },
	{ "Config",                GetCfgDir,          SetCfgDir,          FALSE, DIRDLG_CFG },
	{ "Control Panels",        GetControlPanelDir, SetControlPanelDir, FALSE, 0 },
	{ "Controller Files",      GetCtrlrDir,        SetCtrlrDir,        TRUE, DIRDLG_CTRLR },
	{ "Crosshairs",            GetCrosshairDir,    SetCrosshairDir,    TRUE, 0 },
	{ "Folders",               GetFolderDir,       SetFolderDir,       FALSE, 0 },
	{ "Fonts",                 GetFontDir,         SetFontDir,         TRUE, 0 },
	{ "Flyers",                GetFlyerDir,        SetFlyerDir,        FALSE, 0 },
	{ "Hash",                  GetHashDirs,        SetHashDirs,        TRUE, 0 },
	{ "Hard Drive Difference", GetDiffDir,         SetDiffDir,         TRUE, 0 },
	{ "Icons",                 GetIconsDir,        SetIconsDir,        FALSE, 0 },
	{ "Ini Files",             GetIniDir,          SetIniDir,          FALSE, DIRDLG_INI },
	{ "Input files",           GetInpDir,          SetInpDir,          TRUE, DIRDLG_INP },
	{ "Marquees",              GetMarqueeDir,      SetMarqueeDir,      FALSE, 0 },
	{ "NVRAM",                 GetNvramDir,        SetNvramDir,        TRUE, 0 },
	{ "PCBs",                  GetPcbDir,          SetPcbDir,          FALSE, 0 },
	{ "Snapshots",             GetImgDir,          SetImgDir,          FALSE, DIRDLG_IMG },
	{ "State",                 GetStateDir,        SetStateDir,        TRUE, 0 },
	{ "Titles",                GetTitlesDir,       SetTitlesDir,       FALSE, 0 },
	{ NULL }
};


INT_PTR CALLBACK DirectoriesDialogProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif /* DIRECTORIES_H */


