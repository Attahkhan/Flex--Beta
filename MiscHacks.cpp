/*
Tim's FlexBeta Framework 2017
*/

#define _CRT_SECURE_NO_WARNINGS

#include "MiscHacks.h"
#include "Interfaces.h"
#include "RenderManager.h"

#include <time.h>

template<class T, class U>
inline T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

inline float bitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

inline float FloatNegate(float f)
{
	return bitsToFloat(FloatBits(f) ^ 0x80000000);
}

Vector AutoStrafeView;
float RightMovement;
bool IsActive;
float StrafeAngle;

float GetTraceFractionWorldProps(Vector startpos, Vector endpos) {

	Ray_t ray;
	trace_t tr;
	CTraceFilterWorldOnly filter;

	ray.Init(startpos, endpos);

	Interfaces::Trace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	return tr.fraction;
}

void Start(CUserCmd* pCmd)
{
	auto pLocal = hackManager.pLocal();
	StrafeAngle = 0;
	IsActive = true;

	QAngle CurrentAngles;
	Interfaces::Engine->GetViewAngles(CurrentAngles);
	CurrentAngles.x = 0;
	Vector Forward = CurrentAngles.Direction();
	Vector Right = Forward.Cross(Vector(0, 0, 1));
	Vector Left = Vector(Right.x * -1.0f, Right.y * -1.0f, Right.z);

	float LeftPath = GetTraceFractionWorldProps(pLocal->GetOrigin() + Vector(0, 0, 10), pLocal->GetOrigin() + Left * 250.0f + Vector(0, 0, 10));
	float RightPath = GetTraceFractionWorldProps(pLocal->GetOrigin() + Vector(0, 0, 10), pLocal->GetOrigin() + Right * 250.0f + Vector(0, 0, 10));

	if (LeftPath > RightPath)
		RightMovement = -1;
	else
		RightMovement = 1;
}

void Strafe(CUserCmd* pCmd)
{
	auto pLocal = hackManager.pLocal();
	Vector Velocity = *pLocal->GetVelocity();
	Velocity.z = 0;
	float Speed = Velocity.Length();
	if (Speed < 45) Speed = 45;

	float FinalPath = GetTraceFractionWorldProps(pLocal->GetOrigin() + Vector(0, 0, 10), pLocal->GetOrigin() + Vector(0, 0, 10) + Velocity / 3.0f);
	float DeltaAngle = RightMovement * fmax((275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / Interfaces::Globals->interval_per_tick)) * 4.20f, 2.0f);
	StrafeAngle += DeltaAngle;

	if (!(*pLocal->GetFlags() & FL_ONGROUND))
		pCmd->buttons &= ~IN_JUMP;
	else
		pCmd->buttons |= IN_JUMP;

	if (fabs(StrafeAngle) >= 360.0f)
	{
		StrafeAngle = 0.0f;
		IsActive = false;
		RightMovement = 0;
	}
	else
	{
		pCmd->forwardmove = cos((StrafeAngle + 90 * RightMovement) * (M_PI / 180.0f)) * pLocal->GetMaxSpeed();
		pCmd->sidemove = sin((StrafeAngle + 90 * RightMovement) * (M_PI / 180.0f)) * pLocal->GetMaxSpeed();
	}
}

void CMiscHacks::AmbientLighting()
{
	float AmbientRedAmount = Menu::Window.VisualsTab.AmbientRed.GetValue();
	float AmbientGreenAmount = Menu::Window.VisualsTab.AmbientGreen.GetValue();
	float AmbientBlueAmount = Menu::Window.VisualsTab.AmbientBlue.GetValue();
	bool AsusWallsAmount = Menu::Window.VisualsTab.AsusWalls.GetState();

	ConVar* AmbientRedCvar = Interfaces::CVar->FindVar("mat_ambient_light_r");
	*(float*)((DWORD)&AmbientRedCvar->fnChangeCallback + 0xC) = 0;
	AmbientRedCvar->SetValue(AmbientRedAmount);

	ConVar* AmbientGreenCvar = Interfaces::CVar->FindVar("mat_ambient_light_g");
	*(float*)((DWORD)&AmbientGreenCvar->fnChangeCallback + 0xC) = 0;
	AmbientGreenCvar->SetValue(AmbientGreenAmount);

	ConVar* AmbientBlueCvar = Interfaces::CVar->FindVar("mat_ambient_light_b");
	*(float*)((DWORD)&AmbientBlueCvar->fnChangeCallback + 0xC) = 0;
	AmbientBlueCvar->SetValue(AmbientBlueAmount);

	

	/*ConVar* NigsetgayCvar = Interfaces::CVar->FindVar("mat_fullbright");
	*(float*)((DWORD)&NigsetgayCvar->fnChangeCallback + 0xC) = NULL;
	NigsetgayCvar->SetValue("2");*/
}

void CMiscHacks::AmbientExposure()
	{
		float AmbientExposureMax = Menu::Window.VisualsTab.AmbientExposure.GetValue();
		float AmbientExposureMin = Menu::Window.VisualsTab.AmbientExposure.GetValue();

		ConVar* AmbientExposure2 = Interfaces::CVar->FindVar("mat_autoexposure_max_multiplier");
		*(float*)((DWORD)&AmbientExposure2->fnChangeCallback + 0xC) = 0;
		AmbientExposure2->SetValue(AmbientExposureMax);

		ConVar* AmbientExposure3 = Interfaces::CVar->FindVar("mat_autoexposure_min");
		*(float*)((DWORD)&AmbientExposure3->fnChangeCallback + 0xC) = 0;
		AmbientExposure3->SetValue(AmbientExposureMin);
	}

void CMiscHacks::Init()
{
	// Any init
}

void CMiscHacks::Draw()
{
	// Any drawing	
	// Spams
	switch (Menu::Window.MiscTab.OtherChatSpam.GetIndex())
	{
	case 0:
		// No Chat Spam
		break;
	case 1:
		// Namestealer
		ChatSpamName();
		break;
	case 2:
		// Regular
		ChatSpamRegular();
		break;
	case 3:
		// Interwebz
		ChatSpamInterwebz();
		break;
	case 4:
		// Report Spam
		ChatSpamDisperseName();
		break;
	}
}

void CMiscHacks::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	// Any Move Stuff

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) /*Auto-Disable Post-Processing*/
		AmbientLighting();

	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame())
		AmbientExposure();

	// Bhop
	switch (Menu::Window.MiscTab.OtherAutoJump.GetIndex())
	{
	case 0:
		break;
	case 1:
		AutoJump(pCmd);
		break;
	}

	// AutoStrafe
	Interfaces::Engine->GetViewAngles(AutoStrafeView);
	switch (Menu::Window.MiscTab.OtherAutoStrafe.GetIndex())
	{
	case 0:
		// Off
		break;
	case 1:
		LegitStrafe(pCmd);
		break;
	case 2:
		RageStrafe(pCmd);
		break;
	}

	switch (Menu::Window.VisualsTab.AmbientSkybox.GetIndex())
	{
	case 0:
		/*Disabled*/
		break;
	case 1:
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			ConVar* NoSkybox = Interfaces::CVar->FindVar("sv_skyname"); /*No-Skybox*/
			*(float*)((DWORD)&NoSkybox->fnChangeCallback + 0xC) = 0;
			NoSkybox->SetValue("sky_l4d_rural02_ldr");
		}
		break;

	case 2:
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			ConVar* NightSkybox1 = Interfaces::CVar->FindVar("sv_skyname"); /*Night-Skybox*/
			*(float*)((DWORD)&NightSkybox1->fnChangeCallback + 0xC) = 0;
			NightSkybox1->SetValue("sky_csgo_night02b");
		}
		break;

	case 3:
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			ConVar* NightSkybox2 = Interfaces::CVar->FindVar("sv_skyname"); /*Night-Skybox*/
			*(float*)((DWORD)&NightSkybox2->fnChangeCallback + 0xC) = 0;
			NightSkybox2->SetValue("sky_csgo_night02");
		}
		break;

	case 4:
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			ConVar* SkyBoxJungle = Interfaces::CVar->FindVar("sv_skyname"); /*Night-Skybox*/
			*(float*)((DWORD)&SkyBoxJungle->fnChangeCallback + 0xC) = 0;
			SkyBoxJungle->SetValue("jungle");
		}
		break;
	}

	switch (Menu::Window.VisualsTab.SnowMode.GetState())
		if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame()) {
			ConVar* mat_fullbright = Interfaces::CVar->FindVar("mat_fullbright"); /*Night-Skybox*/
			*(float*)((DWORD)&mat_fullbright->fnChangeCallback + 0xC) = 0;
			mat_fullbright->SetValue("2");

			//Fake Lag
			if (Menu::Window.MiscTab.FakeLagEnable.GetState())
				Fakelag(pCmd, bSendPacket);
		}
	//Bullet Indicator
	if (Menu::Window.MiscTab.OtherBulletReg.GetState()) {
		BulletIndicator();
	}
	//SnowMode
	if (Menu::Window.MiscTab.PostProcessing.GetState()) {
		PostProcessing();
	}
	if (Menu::Window.MiscTab.PostProcessing.GetState()) {
		PostProcessing();
	}
	if (Menu::Window.MiscTab.NoInterp.GetState())
	{
		CMiscHacks::interp(pCmd);
	}

	IClientEntity* pLocal = hackManager.pLocal();

	int CircleKey = Menu::Window.MiscTab.OtherCircleKey.GetKey();

	if (!IsActive && CircleKey > 0 && GUI.GetKeyState(CircleKey) && !(pLocal->GetMoveType() == MOVETYPE_NOCLIP) || pLocal->GetMoveType() == MOVETYPE_LADDER)
		Start(pCmd);

	if (CircleKey > 0 && GUI.GetKeyState(CircleKey) && IsActive)
	{
		Strafe(pCmd);
		return;
	}
	else if (IsActive)
		IsActive = false;

}

static __declspec(naked) void __cdecl Invoke_NET_SetConVar(void* pfn, const char* cvar, const char* value)
{
	__asm 
	{
		push    ebp
			mov     ebp, esp
			and     esp, 0FFFFFFF8h
			sub     esp, 44h
			push    ebx
			push    esi
			push    edi
			mov     edi, cvar
			mov     esi, value
			jmp     pfn
	}
}
void DECLSPEC_NOINLINE NET_SetConVar(const char* value, const char* cvar)
{
	static DWORD setaddr = Utilities::Memory::FindPattern("engine.dll", (PBYTE)"\x8D\x4C\x24\x1C\xE8\x00\x00\x00\x00\x56", "xxxxx????x");
	if (setaddr != 0) 
	{
		void* pvSetConVar = (char*)setaddr;
		Invoke_NET_SetConVar(pvSetConVar, cvar, value);
	}
}

void change_name(const char* name)
{
	if (Interfaces::Engine->IsInGame() && Interfaces::Engine->IsConnected())
		NET_SetConVar(name, "name");
}

void CMiscHacks::interp(CUserCmd *pCmd)
{
	if (Menu::Window.MiscTab.NoInterp.GetState())
	{
		ConVar* interp = Interfaces::CVar->FindVar("net_fakelag");
		interp->SetValue(500);
	}
	else
	{
		ConVar* interp = Interfaces::CVar->FindVar("net_fakelag");
		interp->SetValue(0);
	}
}

void CMiscHacks::AutoJump(CUserCmd *pCmd)
{
	if (pCmd->buttons & IN_JUMP && GUI.GetKeyState(VK_SPACE))
	{
		int iFlags = *hackManager.pLocal()->GetFlags();
		if (!(iFlags & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;

		if (hackManager.pLocal()->GetVelocity()->Length() <= 50)
		{
			pCmd->forwardmove = 450.f;
		}
	}
}

void CMiscHacks::BulletIndicator()
{
	if (Menu::Window.MiscTab.OtherBulletReg.GetState())
	{
		ConVar* Bullet = Interfaces::CVar->FindVar("sv_showimpacts");
		*(float*)((DWORD)&Bullet->fnChangeCallback + 0xC) = NULL;
		Bullet->SetValue("1");
	}
	else
	{
		ConVar* Bullet = Interfaces::CVar->FindVar("sv_showimpacts");
		*(float*)((DWORD)&Bullet->fnChangeCallback + 0xC) = NULL;
		Bullet->SetValue("0");
	}
}

void CMiscHacks::SnowMode()
{
	if (Menu::Window.VisualsTab.OtherNightMode.GetState())
	{
		ConVar* SnowMode = Interfaces::CVar->FindVar("mat_fullbright");
		*(float*)((DWORD)&SnowMode->fnChangeCallback + 0xC) = NULL;
		SnowMode->SetValue("1");
	}
	else
	{
		ConVar* SnowMode = Interfaces::CVar->FindVar("mat_fullbright");
		*(float*)((DWORD)&SnowMode->fnChangeCallback + 0xC) = NULL;
		SnowMode->SetValue("0");
	}
}



void CMiscHacks::PostProcessing()
{
	if (Menu::Window.MiscTab.PostProcessing.GetState())
	{
		ConVar* PostProcessing = Interfaces::CVar->FindVar("mat_postprocess_enable");
		*(float*)((DWORD)&PostProcessing->fnChangeCallback + 0xC) = NULL;
		PostProcessing->SetValue("0");
	}
	else
	{
		ConVar* PostProcessing = Interfaces::CVar->FindVar("mat_postprocess_enable");
		*(float*)((DWORD)&PostProcessing->fnChangeCallback + 0xC) = NULL;
		PostProcessing->SetValue("1");
	}
}

void CMiscHacks::LegitStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (!(*pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = 0.0f;

		if (pCmd->mousedx < 0)
		{
			pCmd->sidemove = -450.0f;
		}
		else if (pCmd->mousedx > 0)
		{
			pCmd->sidemove = 450.0f;
		}
	}
}

void CMiscHacks::RageStrafe(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	bool bKeysPressed = true;
	if (GUI.GetKeyState(0x41) || GUI.GetKeyState(0x57) || GUI.GetKeyState(0x53) || GUI.GetKeyState(0x44)) bKeysPressed = false;
	int CircleKey = (GetAsyncKeyState('C'));
	if (CircleKey)
		return;

	if ((GetAsyncKeyState(VK_SPACE) && !(*pLocal->GetFlags() & FL_ONGROUND)) && bKeysPressed)
	{
		if (pCmd->mousedx > 1 || pCmd->mousedx < -1) {
			pCmd->sidemove = pCmd->mousedx < 0.f ? -450.f : 450.f;
		}
		else {
			static int tick = 0;
			tick += 0.5;
			int handler = tick / 3.14;
			
			pCmd->forwardmove = (1800.f * 4.f) / pLocal->GetVelocity()->Length2D();
			pCmd->sidemove = (pCmd->command_number % 2) == 0 ? -450.f : 450.f;
			if (pCmd->forwardmove > 450.f)
				pCmd->forwardmove = 450.f;
	
		}
	}

/*	IClientEntity* pLocal = hackManager.pLocal();
	static bool bDirection = true;


	static float move = 450; //400.f; // move = max(move, (abs(cmd->move.x) + abs(cmd->move.y)) * 0.5f);
	float s_move = move * 0.5065f;


	if ((pCmd->buttons & IN_JUMP) || !(pLocal->GetFlags() & FL_ONGROUND))
	{
		pCmd->forwardmove = move * 0.015f;
		pCmd->sidemove += (float)(((pCmd->tick_count % 2) * 2) - 1) * s_move;

		if (pCmd->mousedx)
			pCmd->sidemove = (float)clamp(pCmd->mousedx, -1, 1) * s_move;

		static float strafe = pCmd->viewangles.y;

		float rt = pCmd->viewangles.y, rotation;
		rotation = strafe - rt;

		if (rotation < FloatNegate(Interfaces::Globals->interval_per_tick))
			pCmd->sidemove = -s_move;

		if (rotation > Interfaces::Globals->interval_per_tick)
			pCmd->sidemove = s_move;

		strafe = rt;
	} */
}

Vector GetAutostrafeView()
{
	return AutoStrafeView;
}

// …e Õ½Ê¿
void CMiscHacks::ChatSpamInterwebz()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	//static std::string nameBackup = "INTERWEBZ";

	if (wasSpamming)
	{
		static bool useSpace = true;
		if (useSpace)
		{
			change_name ("FlexBeta.club");
			useSpace = !useSpace;
		}
		else
		{
			change_name ("FlexBeta.club");
			useSpace = !useSpace;
		}
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamDisperseName()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	static bool wasSpamming = true;
	//static std::string nameBackup = "INTERWEBZ";

	if (wasSpamming)
	{
		change_name("\n…e…e…e\n");
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamName()
{
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < 0.001)
		return;

	std::vector < std::string > Names;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		// Get the entity
		IClientEntity *entity = Interfaces::EntList->GetClientEntity(i);

		player_info_t pInfo;
		// If it's a valid entity and isn't the player
		if (entity && hackManager.pLocal()->GetTeamNum() == entity->GetTeamNum() && entity != hackManager.pLocal())
		{
			ClientClass* cClass = (ClientClass*)entity->GetClientClass();

			// If entity is a player
			if (cClass->m_ClassID == (int)CSGOClassID::CCSPlayer)
			{
				if (Interfaces::Engine->GetPlayerInfo(i, &pInfo))
				{
					if (!strstr(pInfo.name, "GOTV"))
						Names.push_back(pInfo.name);
				}
			}
		}
	}

	static bool wasSpamming = true;
	//static std::string nameBackup = "INTERWEBZ.CC";

	int randomIndex = rand() % Names.size();
	char buffer[128];
	sprintf_s(buffer, "%s ", Names[randomIndex].c_str());

	if (wasSpamming)
	{
		change_name(buffer);
	}
	else
	{
		change_name ("p$i 1337");
	}

	start_t = clock();
}

void CMiscHacks::ChatSpamRegular()
{
	// Don't spam it too fast so you can still do stuff
	static clock_t start_t = clock();
	int spamtime = Menu::Window.MiscTab.OtherChatDelay.GetValue();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	if (timeSoFar < spamtime)
		return;

	static bool holzed = true;

	if (Menu::Window.MiscTab.OtherTeamChat.GetState())
	{
		SayInTeamChat("INTERWEBZ.CC OWNS ME AND ALL");
	}
	else
	{
		SayInChat("INTERWEBZ.CC OWNS ME AND ALL");
	}

	start_t = clock();
}


void CMiscHacks::Fakelag(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (pCmd->buttons & IN_ATTACK) return;

	int iChoke = Menu::Window.MiscTab.FakeLagChoke.GetValue();

	static int iFakeLag = -1;
	iFakeLag++;

	if (iFakeLag <= iChoke && iFakeLag > -1)
	{
		bSendPacket = true;
	}
	else
	{
		bSendPacket = false;
		iFakeLag = -1;
	}
}