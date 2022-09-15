#include <Windows.h>

//StepS tools
typedef unsigned long long QWORD;
#define MAKELONGLONG(lo,hi) ((LONGLONG(DWORD(lo) & 0xffffffff)) | LONGLONG(DWORD(hi) & 0xffffffff) << 32 )
#define QV(V1, V2, V3, V4) MAKEQWORD(V4, V3, V2, V1)
#define MAKEQWORD(LO2, HI2, LO1, HI1) MAKELONGLONG(MAKELONG(LO2,HI2),MAKELONG(LO1,HI1))
QWORD GetModuleVersion(HMODULE hModule)
{
	char WApath[MAX_PATH]; DWORD h;
	GetModuleFileNameA(hModule,WApath,MAX_PATH);
	DWORD Size = GetFileVersionInfoSizeA(WApath,&h);
	if(Size)
	{
		void* Buf = malloc(Size);
		GetFileVersionInfoA(WApath,h,Size,Buf);
		VS_FIXEDFILEINFO *Info; DWORD Is;
		if(VerQueryValueA(Buf, "\\", (LPVOID*)&Info, (PUINT)&Is))
		{
			if(Info->dwSignature==0xFEEF04BD)
			{
				return MAKELONGLONG(Info->dwFileVersionLS, Info->dwFileVersionMS);
			}
		}
		free(Buf);
	}
	return 0;
}

int waVersionCheck() {
	auto version = GetModuleVersion((HMODULE)0);
	if(version == QV(3,8,1,0)) {
		return 1;
	}
	MessageBoxA(0, "Sorry, but wkTrackMeBetter381 is not compatible with your version of the game.", "wkTrackMeBetter381 error", MB_OK | MB_ICONERROR);
	return 0;
}

void patchCamera(DWORD funcAddr, int confinement) {
	unsigned char shift;
	if(confinement < 100) shift = 15;
	else if(confinement < 200) shift = 16;
	else shift = 17;
	*(BYTE *)(funcAddr + 0x3) = shift;
	*(BYTE *)(funcAddr + 0xF) = shift;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
	switch(ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			int enabled = GetPrivateProfileIntA("TrackMeBetter", "EnableModule", 1, "./wkTrackMeBetter.ini");
			if(!enabled || !waVersionCheck()) return TRUE;
			int confinement = GetPrivateProfileIntA("TrackMeBetter", "ConfinementPercentage", 1, "./wkTrackMeBetter.ini");

			DWORD base = (DWORD)GetModuleHandle(0);
			DWORD funcAddr = base + (0x542F70 - 0x400000);

			DWORD dwLastProtection;
			if (!VirtualProtect((void*)funcAddr, 0x40, PAGE_EXECUTE_READWRITE, &dwLastProtection)) {
				MessageBoxA(0, "VirtualProtect for W:A code failed!.", "wkTrackMeBetter381 error", MB_OK | MB_ICONERROR);
				return TRUE;
			}
			patchCamera(funcAddr, confinement);
			VirtualProtect((void*)funcAddr, 0x40, dwLastProtection, &dwLastProtection);
		}
		break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
		default:
			break;
	}
	return TRUE;
}
