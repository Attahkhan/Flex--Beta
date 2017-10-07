
#include "LegitBot.h"
#include "RenderManager.h"

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10

#define RADPI 57.295779513082f

void CLegitBot::Init()
{
	IsLocked = false;
	TargetID = -1;
	HitBox = -1;
}

void CLegitBot::Draw()
{
	if (Menu::Window.LegitBotTab.Active.GetState() && fovCircle > 0.f)
	{
		RECT View = Render::GetViewport();
		int MidX = View.right / 2;
		int MidY = View.bottom / 2;

		Interfaces::Surface->DrawOutlinedCircle(MidX, MidY, fovCircle, RADPI);
	}
}

static int CustomDelay = 0;
static int CustomBreak = 0;
void CLegitBot::Move(CUserCmd *pCmd, bool& bSendPacket)
{
	// Master switch
	if (!Menu::Window.LegitBotTab.Active.GetState())
		return;

	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	// Aimbot
	static int CustomAimTime = 0;
	static int CustomAimStart = 0;
	if (Menu::Window.LegitBotTab.AimbotEnable.GetState() && !GUI.GetKeyState(Menu::Window.LegitBotTab.TriggerKeyBind.GetKey()))
	{
		if (StartAim > 0)
		{
			if (CustomAimStart < (StartAim * 333))
			{
				CustomAimStart++;
			}
			else
			{
				if (Aimtime > 0)
				{
					if (CustomAimTime < (Aimtime * 333))
					{
						DoAimbot(pCmd);
						CustomAimTime++;
					}
					if (!GUI.GetKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
					{
						CustomAimTime = 0;
						CustomAimStart = 0;
					}
				}
				else
				{
					DoAimbot(pCmd);
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}

			if (!GUI.GetKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
			{
				CustomAimStart = 0;
				CustomAimTime = 0;
			}
		}
		else
		{
			if (Aimtime > 0)
			{
				if (CustomAimTime < (Aimtime * 333))
				{
					DoAimbot(pCmd);
					CustomAimTime++;
				}
				if (!GUI.GetKeyState(Menu::Window.LegitBotTab.AimbotKeyBind.GetKey()) || Menu::Window.LegitBotTab.AimbotAutoFire.GetState())
				{
					CustomAimTime = 0;
					CustomAimStart = 0;
				}
			}
			else
			{
				DoAimbot(pCmd);
				CustomAimTime = 0;
				CustomAimStart = 0;
			}
		}
	}

	//// Burst
	//static int CustomBurst = 0;
	//if (shoot && Menu::Window.LegitBotTab.TriggerBurst.GetValue() > 0 && !GameUtils::IsPistol(pWeapon) && !GameUtils::IsShotgun(pWeapon) && !GameUtils::IsSniper(pWeapon))
	//{
	//	if (CustomBurst < (Menu::Window.LegitBotTab.TriggerBurst.GetValue() * 30))
	//	{
	//		CustomBurst++;
	//		pCmd->buttons |= IN_ATTACK;

	//	}
	//	else
	//	{
	//		CustomBurst = 0;
	//		shoot = false;

	//	}
	//}
	//else
	//{

	//	shoot = false;
	//	CustomBurst = 0;
	//}

	/*if (!GUI.GetKeyState(Menu::Window.LegitBotTab.TriggerKeyBind.GetKey()))
	{
	CustomDelay = 0;

	}*/

	// Triggerbot
	if (Menu::Window.LegitBotTab.TriggerEnable.GetState() && ((Menu::Window.LegitBotTab.TriggerKeyPress.GetState() && GUI.GetKeyState(Menu::Window.LegitBotTab.TriggerKeyBind.GetKey()) || !Menu::Window.LegitBotTab.TriggerKeyPress.GetState())))
	{
		DoTrigger(pCmd);
	}

	// Auto Pistol
	static bool WasFiring = false;
	if (GameUtils::IsPistol(pWeapon) && Menu::Window.LegitBotTab.AimbotAutoPistol.GetState())
	{
		if ((pCmd->buttons & IN_ATTACK) && (pWeapon->GetNextPrimaryAttack() - ((float)pLocal->GetTickBase()
			* Interfaces::Globals->interval_per_tick) > 0)) pCmd->buttons &= ~IN_ATTACK;
	}

	SyncWeaponSettings();
}

void CLegitBot::SyncWeaponSettings()
{
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pWeapon)
		return;

	if (GameUtils::IsPistol(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponPistSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponPistFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponPistRecoil.GetState();
		fovCircle = Menu::Window.LegitBotTab.WeaponPistFoV.GetValue() * 2;
		RecoilAmountX = Menu::Window.LegitBotTab.WeaponPistRecoilAmountX.GetValue() / 50;
		RecoilAmountY = Menu::Window.LegitBotTab.WeaponPistRecoilAmountY.GetValue() / 50;

		switch (Menu::Window.LegitBotTab.WeaponPistHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponPistAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponPistStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponSnipSpeed.GetValue();
		FoV = Menu::Window.LegitBotTab.WeaponSnipFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponSnipRecoil.GetState();

		fovCircle = Menu::Window.LegitBotTab.WeaponSnipFoV.GetValue() * 2;
		RecoilAmountX = Menu::Window.LegitBotTab.WeaponSnipRecoilAmountX.GetValue() / 50;
		RecoilAmountY = Menu::Window.LegitBotTab.WeaponSnipRecoilAmountY.GetValue() / 50;



		switch (Menu::Window.LegitBotTab.WeaponSnipHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponSnipAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponSnipStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsMachinegun(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponMGSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMGFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMGRecoil.GetValue();

		fovCircle = Menu::Window.LegitBotTab.WeaponMGFoV.GetValue() * 2;

		switch (Menu::Window.LegitBotTab.WeaponMGHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponMGAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMGStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsShotgun(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponShotgunSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponShotgunFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponShotgunRecoil.GetValue();

		fovCircle = Menu::Window.LegitBotTab.WeaponShotgunFoV.GetValue() * 2;

		switch (Menu::Window.LegitBotTab.WeaponShotgunHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponShotgunAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponShotgunStartAimtime.GetValue() / 10;
	}
	else if (GameUtils::IsMP(pWeapon))
	{
		Speed = Menu::Window.LegitBotTab.WeaponMpSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMpFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMpRecoil.GetState();

		fovCircle = Menu::Window.LegitBotTab.WeaponMpFoV.GetValue() * 2;

		switch (Menu::Window.LegitBotTab.WeaponMpHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}

		Aimtime = Menu::Window.LegitBotTab.WeaponMpAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMpStartAimtime.GetValue() / 10;
	}
	else
	{
		Speed = Menu::Window.LegitBotTab.WeaponMainSpeed.GetValue() / 100;
		FoV = Menu::Window.LegitBotTab.WeaponMainFoV.GetValue() * 2;
		RecoilControl = Menu::Window.LegitBotTab.WeaponMainRecoil.GetState();
		RecoilAmountX = Menu::Window.LegitBotTab.WeaponMainRecoilAmountX.GetValue() / 50;
		RecoilAmountY = Menu::Window.LegitBotTab.WeaponMainRecoilAmountY.GetValue() / 50;

		fovCircle = Menu::Window.LegitBotTab.WeaponMainFoV.GetValue() * 2;

		switch (Menu::Window.LegitBotTab.WeaponMainHitbox.GetIndex())
		{
		case 0:
			HitBox = ((int)CSGOHitboxID::Head);
			Multihitbox = false;
			break;
		case 1:
			HitBox = ((int)CSGOHitboxID::Neck);
			Multihitbox = false;
			break;
		case 2:
			HitBox = ((int)CSGOHitboxID::Chest);
			Multihitbox = false;
			break;
		case 3:
			HitBox = ((int)CSGOHitboxID::Stomach);
			Multihitbox = false;
			break;
		case 4:
			Multihitbox = true;
			break;
		}
		Aimtime = Menu::Window.LegitBotTab.WeaponMainAimtime.GetValue() / 10;
		StartAim = Menu::Window.LegitBotTab.WeaoponMainStartAimtime.GetValue() / 10;
	}
}

void CLegitBot::DoAimbot(CUserCmd *pCmd)
{
	IClientEntity* pTarget = nullptr;
	IClientEntity* pLocal = hackManager.pLocal();
	bool FindNewTarget = true;
	//IsLocked = false;

	// Don't aimbot with the knife..
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	if (pWeapon)
	{
		if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsKnife(pWeapon))
		{
			//TargetID = 0;
			//pTarget = nullptr;
			//HitBox = -1;
			return;
		}
		SyncWeaponSettings();

	}
	else
		return;

	// Make sure we have a good target
	if (IsLocked && TargetID >= 0 && HitBox >= 0)
	{
		pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		if (pTarget  && TargetMeetsRequirements(pTarget))
		{
			SyncWeaponSettings();
			if (HitBox >= 0)
			{
				Vector ViewOffset = pLocal->GetOrigin() + *pLocal->GetViewOffset();
				Vector View; Interfaces::Engine->GetViewAngles(View);
				if (pLocal->GetVelocity()->Length() > 45.f);
				View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * RecoilControl;
				float nFoV = FovToPlayer(ViewOffset, View, pTarget, HitBox);
				if (nFoV < FoV)
					FindNewTarget = false;
			}
		}
	}

	// Find a new target, apparently we need to
	if (FindNewTarget)
	{
		TargetID = 0;
		pTarget = nullptr;
		HitBox = -1;

		TargetID = GetTargetCrosshair();

		// Memes
		if (TargetID >= 0)
		{
			pTarget = Interfaces::EntList->GetClientEntity(TargetID);
		}
		else
		{
			pTarget = nullptr;
			HitBox = -1;
		}
	}

	SyncWeaponSettings();

	// If we finally have a good target
	if (TargetID >= 0 && pTarget)
	{
		//HitBox = (int)CSGOHitboxID::Head;//
		SyncWeaponSettings();

		// Key
		if (Menu::Window.LegitBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.LegitBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				TargetID = -1;
				pTarget = nullptr;
				HitBox = -1;
				return;
			}
		}

		Vector AimPoint;

		if (Multihitbox)
		{
			AimPoint = GetHitboxPosition(pTarget, besthitbox);
		}
		else
		{
			AimPoint = GetHitboxPosition(pTarget, HitBox);
		}

		if (AimAtPoint(pLocal, AimPoint, pCmd))
		{
			//IsLocked = true;
			if (Menu::Window.LegitBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
			{
				pCmd->buttons |= IN_ATTACK;
			}
		}
	}
}

bool CLegitBot::TargetMeetsTriggerRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex() && pEntity->GetIndex() < 65)
	{
		// Team Check
		if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
		{
			// Spawn Check
			if (!pEntity->HasGunGameImmunity())
			{
				return true;
			}
		}
	}

	// They must have failed a requirement
	return false;
}

//===============================================================================
//Math stuff:

//===============================================================================

void CLegitBot::DoTrigger(CUserCmd *pCmd)
{
	IClientEntity* LocalPlayer = hackManager.pLocal();

	//===============================================================================
	auto LocalPlayerWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(LocalPlayer->GetWeaponHandle());
	auto WeaponEntity = (IClientEntity*)LocalPlayerWeapon;

	if (LocalPlayerWeapon) {
		if (LocalPlayerWeapon->GetAmmoInClip() == 0)
			return;

		auto ClientClass = WeaponEntity->GetClientClass2();
		if (ClientClass->m_ClassID == (int)CSGOClassID::CKnife ||
			ClientClass->m_ClassID == (int)CSGOClassID::CHEGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CDecoyGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CIncendiaryGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CSmokeGrenade ||
			ClientClass->m_ClassID == (int)CSGOClassID::CC4) {
			return;
		}
	}
	else
		return;
	//===============================================================================

	Vector ViewAngles = pCmd->viewangles;
	if (Menu::Window.LegitBotTab.TriggerRecoil.GetState())
		ViewAngles += LocalPlayer->GetAimPunch() * 2.0f;

	Vector CrosshairForward;
	AngleVectors(ViewAngles, &CrosshairForward);
	//CrosshairForward *= LocalPlayerWeaponData->m_flRange;
	CrosshairForward *= 8000.f;


	Vector TraceSource = LocalPlayer->GetEyePos();
	Vector TraceDestination = TraceSource + CrosshairForward;

	Ray_t Ray;
	trace_t Trace;
	CTraceFilter Filter;

	Filter.pHit = LocalPlayer;

	Ray.Init(TraceSource, TraceDestination);
	Interfaces::Trace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	if (!Trace.m_pEnt)
		return;
	if (!Trace.m_pEnt->IsAlive())
		return;
	if (Trace.m_pEnt->GetHealth() <= 0 || Trace.m_pEnt->GetHealth() > 100)
		return;
	if (Trace.m_pEnt->IsImmune())
		return;

	if (!Menu::Window.LegitBotTab.TriggerTeammates.GetState()) {
		if (LocalPlayer->GetTeamNum() == Trace.m_pEnt->GetTeamNum())
			return;
	}

	if ((Menu::Window.LegitBotTab.TriggerHead.GetState() && Trace.hitgroup == HITGROUP_HEAD) ||
		(Menu::Window.LegitBotTab.TriggerChest.GetState() && Trace.hitgroup == HITGROUP_CHEST) ||
		(Menu::Window.LegitBotTab.TriggerStomach.GetState() && Trace.hitgroup == HITGROUP_STOMACH) ||
		(Menu::Window.LegitBotTab.TriggerArms.GetState() && (Trace.hitgroup == HITGROUP_LEFTARM || Trace.hitgroup == HITGROUP_RIGHTARM)) ||
		(Menu::Window.LegitBotTab.TriggerLegs.GetState() && (Trace.hitgroup == HITGROUP_LEFTLEG || Trace.hitgroup == HITGROUP_RIGHTLEG))) {
		pCmd->buttons |= IN_ATTACK;
	}

	///Original code:
	//IClientEntity* pLocal = hackManager.pLocal();

	//CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	//if (pWeapon)
	//{
	//	if (pWeapon->GetAmmoInClip() == 0 || !GameUtils::IsBallisticWeapon(pWeapon))
	//	{
	//		return;
	//	}
	//}
	//else
	//	return;

	//Vector ViewAngles = pCmd->viewangles;
	//if (Menu::Window.LegitBotTab.TriggerRecoil.GetState())
	//	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	//Vector src, dst, forward;
	//trace_t tr;
	//Ray_t ray;
	//CTraceFilter filter;

	//AngleVectors(ViewAngles, &forward);
	//forward *= 8192;

	//filter.pSkip = pLocal;
	//src = pLocal->GetOrigin() + pLocal->GetViewOffset();
	//dst = src + forward;

	//ray.Init(src, dst);

	//Interfaces::Trace->TraceRay(ray, 0x46004003, &filter, &tr);

	//if (!tr.m_pEnt)
	//	return;

	//int hitgroup = tr.hitgroup;
	//bool didHit = false;

	//if (Menu::Window.LegitBotTab.TriggerHead.GetState() && hitgroup == HITGROUP_HEAD)
	//	didHit = true;
	//if (Menu::Window.LegitBotTab.TriggerChest.GetState() && hitgroup == HITGROUP_CHEST)
	//	didHit = true;
	//if (Menu::Window.LegitBotTab.TriggerStomach.GetState() && hitgroup == HITGROUP_STOMACH)
	//	didHit = true;
	//if (Menu::Window.LegitBotTab.TriggerArms.GetState() && (hitgroup == HITGROUP_LEFTARM || hitgroup == HITGROUP_RIGHTARM))
	//	didHit = true;
	//if (Menu::Window.LegitBotTab.TriggerLegs.GetState() && (hitgroup == HITGROUP_LEFTLEG || hitgroup == HITGROUP_RIGHTLEG))
	//	didHit = true;

	//float hitchance = 75.f + (Menu::Window.LegitBotTab.TriggerHitChanceAmmount.GetValue() / 4);
	//if (didHit && (!Menu::Window.LegitBotTab.TriggerHitChance.GetState() || (1.0f - pWeapon->GetAccuracyPenalty()) * 100.f >= hitchance))
	//{
	//	if (TargetMeetsTriggerRequirements(tr.m_pEnt))
	//	{
	//		if (Menu::Window.LegitBotTab.TriggerDelay.GetValue() > 1)
	//		{
	//			if (CustomDelay >= Menu::Window.LegitBotTab.TriggerDelay.GetValue() / 30)
	//			{
	//				CustomDelay = 0;
	//				shoot = true;
	//				if (*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() != 64)
	//					pCmd->buttons |= IN_ATTACK;
	//				else
	//					pCmd->buttons |= IN_ATTACK2;
	//			}
	//			else
	//			{
	//				CustomDelay++;
	//				return;
	//			}
	//		}
	//		else
	//		{
	//			CustomDelay = 0;
	//			shoot = true;
	//			if (*pWeapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() != 64)
	//				pCmd->buttons |= IN_ATTACK;
	//			else
	//				pCmd->buttons |= IN_ATTACK2;
	//		}
	//	}
	//}
	//else
	//{
	//	if (Menu::Window.LegitBotTab.TriggerBreak.GetValue() > 1)
	//	{
	//		if (CustomBreak <= Menu::Window.LegitBotTab.TriggerBreak.GetValue())
	//			CustomBreak = 0;
	//		shoot = false;
	//	}
	//}
}

bool CLegitBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	// Is a valid player
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		// Entity Type checks
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			// Team Check
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.LegitBotTab.AimbotFriendlyFire.GetState())
			{
				/*if (Menu::Window.LegitBotTab.AimbotSmokeCheck.GetState()) {
					typedef bool(__cdecl* GoesThroughSmoke)(Vector, Vector);

					static uint32_t GoesThroughSmokeOffset = (uint32_t)Utilities::Memory::FindPatternV2("client.dll", "55 8B EC 83 EC 44 8B 15");
					static GoesThroughSmoke GoesThroughSmokeFunction = (GoesThroughSmoke)GoesThroughSmokeOffset;

					if (GoesThroughSmokeFunction(hackManager.pLocal()->GetEyePosition(), pEntity->GetBonePos(6)))
						return false;
					else
						return true;
				}*/

				if (Multihitbox)
				{
					if (!pEntity->HasGunGameImmunity())
					{
						if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, 0) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 6) || GameUtils::IsVisible(hackManager.pLocal(), pEntity, 4))
							return true;
						else
							return false;
					}
				}
				else
				{
					// Spawn Check
					if (!pEntity->HasGunGameImmunity() && GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBox))
					{
						return true;
					}
				}
			}
		}
	}

	// They must have failed a requirement
	return false;
}

float Get3dDistance(Vector me, Vector ent)
{
	return sqrt(pow(double(ent.x - me.x), 2.0) + pow(double(ent.y - me.y), 2.0) + pow(double(ent.z - me.z), 2.0));
}

float CLegitBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	// Anything past 180 degrees is just going to wrap around
	CONST FLOAT MaxDegrees = 180.0f;

	// Get local angles
	Vector Angles = View;

	// Get local view / eye position
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);
	// Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	//Delta = AimPos - Origin;

	// Normalize our delta vector
	Normalize(Delta, Delta);

	// Get dot product between delta position and directional forward vectors
	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int CLegitBot::GetTargetCrosshair()
{
	// Target selection
	SyncWeaponSettings();
	int target = -1;


	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + *pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);
	View += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (TargetMeetsRequirements(pEntity))
		{
			if (Multihitbox)
			{

				float fov1 = FovToPlayer(ViewOffset, View, pEntity, 0);
				float fov2 = FovToPlayer(ViewOffset, View, pEntity, 4);
				float fov3 = FovToPlayer(ViewOffset, View, pEntity, 6);

				if (fov1 < FoV || fov2 < FoV && fov1 < FoV || fov3 < FoV)
				{
					FoV = fov1;
					target = i;
					besthitbox = 0;
				}

				if (fov2 < FoV || fov1 < FoV && fov2 < FoV || fov3 < FoV)
				{
					FoV = fov2;
					target = i;
					besthitbox = 4;
				}

				if (fov3 < FoV || fov1 < FoV && fov3 < FoV || fov2 < FoV)
				{
					FoV = fov3;
					target = i;
					besthitbox = 6;
				}

			}
			else
			{
				int NewHitBox = HitBox;
				if (NewHitBox >= 0)
				{
					float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
					if (fov < FoV)
					{
						FoV = fov;
						target = i;
					}
				}
			}
		}
	}

	return target;
}

bool CLegitBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd)
{
	if (point.Length() == 0) return false;

	Vector angles;
	CalcAngle(pLocal->GetEyePos(), point, angles); // Calc angle aimbot method
	GameUtils::NormaliseViewAngle(angles);

	if (angles[0] != angles[0] || angles[1] != angles[1])
	{
		return false;
	}

	if (RecoilControl)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();

		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			angles.x -= AimPunch.x * RecoilAmountX;
			angles.y -= AimPunch.y * RecoilAmountY;
			GameUtils::NormaliseViewAngle(angles);
		}
	}

	IsLocked = true;
	//-----------------------------------------------

	Vector shit = angles - pCmd->viewangles;
	bool v = false;
	GameUtils::NormaliseViewAngle(shit);
	if (shit.Length() > Speed)
	{
		Normalize(shit, shit);
		shit *= Speed;
	}
	else
	{
		v = true;
	}

	pCmd->viewangles += shit;
	Interfaces::Engine->SetViewAngles(pCmd->viewangles);

	return v;
}