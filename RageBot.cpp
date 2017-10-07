#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"
#define TIME_TO_TICKS( dt )	( ( int )( 0.5f + ( float )( dt ) / Interfaces::Globals->interval_per_tick ) )

void CRageBot::Init()
{
	IsAimStepping = false;
	IsLocked = false;
	Globals::TargetID = -1;
}

void CRageBot::Draw()
{

}

bool IsAbleToShoot(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

float hitchance(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());
	float hitchance = 101;
	if (!pWeapon) return 0;

	if (Menu::Window.RageBotTab.AccuracyHitchance.GetValue() > 1)
	{
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;
	}
	return hitchance;
}

bool CanOpenFire()
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (!pLocalEntity)
		return false;

	CBaseCombatWeapon* entwep = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocalEntity->GetActiveWeaponHandle());

	float flServerTime = (float)pLocalEntity->GetTickBase() * Interfaces::Globals->interval_per_tick;
	float flNextPrimaryAttack = entwep->GetNextPrimaryAttack();

	std::cout << flServerTime << " " << flNextPrimaryAttack << std::endl;

	return !(flNextPrimaryAttack > flServerTime);
}

void CRageBot::Move(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocalEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!pLocalEntity || !pLocalEntity->IsAlive())
		return;

	if (!Menu::Window.RageBotTab.AimbotEnable.GetState())
		return;

	if (Menu::Window.RageBotTab.AntiAimEnable.GetIndex() == 1 || Menu::Window.RageBotTab.AntiAimEnable.GetIndex() == 2)
	{
		static int ChokedPackets = -1;

		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
		if (!pWeapon)
			return;

		if (ChokedPackets < 1 && pLocalEntity->GetLifeState() == LIFE_ALIVE && pCmd->buttons & IN_ATTACK && CanOpenFire() && GameUtils::IsKnife(pWeapon))
		{
			bSendPacket = false;
		}
		else
		{
			if (pLocalEntity->GetLifeState() == LIFE_ALIVE)
			{
				RageAntiAim(pCmd, bSendPacket);
			}
			ChokedPackets = -1;
		}
	}

	if (Menu::Window.RageBotTab.DisableInterpolation.GetState())
		DisableInterpolation(pCmd);

	if (Menu::Window.RageBotTab.BackTrackingCreditsSuper.GetState()) // Simple Lag Compensation
	{
		Cache();
		ProcessCmd(Globals::TargetID, pCmd);
	}

	if (Menu::Window.RageBotTab.AimbotEnable.GetState())
		DoAimbot(pCmd, bSendPacket);

	if (Menu::Window.RageBotTab.AccuracyRecoil.GetState())
		DoNoRecoil(pCmd);

	if (Menu::Window.MiscTab.FakeLagEnable.GetState())
	{
		DoFakelag(pCmd, bSendPacket);
	}

	if (Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = pCmd->viewangles - LastAngle;
		if (AddAngs.Length2D() > 25.f)
		{
			Normalize(AddAngs, AddAngs);
			AddAngs *= 25;
			pCmd->viewangles = LastAngle + AddAngs;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
	LastAngle = pCmd->viewangles;
}

void CRageBot::DisableInterpolation(CUserCmd* pCmd)
{
	static auto g_InterpolationList = *(DWORD*)(Utilities::Memory::FindPatternV2("client.dll", "39 9F B6") + 0xA);
	if (g_InterpolationList)
	{
		*reinterpret_cast<DWORD*>(static_cast<DWORD>(g_InterpolationList) + 18) = 0; // array size/list gotta clear it
		*reinterpret_cast<DWORD*>(static_cast<DWORD>(g_InterpolationList) + 12) = 0xFFFF; // invalidate the head interpolation
	}
}

void CRageBot::SyncWeaponHitchance() // Custom weapon settings
{
	IClientEntity* pLocal = hackManager.pLocal();
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pWeapon || GameUtils::IsRevolver(pWeapon))
	{
		WeaponHitchance = NULL;
		return;
	}

	if (GameUtils::IsPistol(pWeapon))
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponPistolHitchance.GetValue();
	}
	else if (GameUtils::IsSniper(pWeapon))
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponSniperHitchance.GetValue();
	}
	else if (GameUtils::IsRifle(pWeapon))
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponRifleHitchance.GetValue();
	}
	else if (GameUtils::IsMP(pWeapon))
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponSMGHitchance.GetValue();
	}
	else
	{
		WeaponHitchance = Menu::Window.RageBotTab.WeaponOtherHitchance.GetValue();
	}
}

float CRageBot::hitchance(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	float hitchance = 101.f;

	if (!pWeapon) return 0;

	if (WeaponHitchance > 1.f)
	{
		float inaccuracy = pWeapon->GetInaccuracy();
		if (inaccuracy == 0) inaccuracy = 0.0000001;
		inaccuracy = 1 / inaccuracy;
		hitchance = inaccuracy;
	}
	return hitchance;
}

void CRageBot::DoAimbot(CUserCmd *pCmd, bool &bSendPacket) 
{
	IClientEntity* LocalPlayer = hackManager.pLocal();
	CSWeaponInfo* CSWeaponInfo = ((CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeaponHandle()))->GetCSWpnData();
	CBaseCombatWeapon* CSWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(LocalPlayer->GetActiveWeaponHandle());

	bool FindNewTarget = true;
	Vector Point;

	if (CSWeapon)
	{
		if (CSWeapon->GetAmmoInClip() == 0 || !GameUtils::IsKnife(CSWeapon))
		{
			return;
		}
	}

	if (GameUtils::IsRevolver(CSWeapon) && Menu::Window.RageBotTab.AutoRevolver.GetState())
	{
		float flPostponeFireReady = CSWeapon->GetFireReadyTime();

		if (flPostponeFireReady > 0 && flPostponeFireReady < Interfaces::Globals->curtime)
		{
			pCmd->buttons &= ~IN_ATTACK;
		}

		static int delay = 0;
		delay++;

		if (delay <= 15)
			pCmd->buttons |= IN_ATTACK;
		else
			delay = 0;
	}


	if (IsLocked && Globals::TargetID >= 0 && HitBox >= 0)
	{
		Globals::Target = Interfaces::EntList->GetClientEntity(Globals::TargetID);

		if (Globals::Target && TargetMeetsRequirements(Globals::Target))
		{
			HitBox = HitScan(Globals::Target);

			if (HitBox >= 0)
			{
				Vector View; Interfaces::Engine->GetViewAngles(View);
				float FoV = FovToPlayer(LocalPlayer->GetEyePos(), View, Globals::Target, HitBox);
				if (FoV < Menu::Window.RageBotTab.AimbotFov.GetValue())
					FindNewTarget = false;
			}
		}
	}

	if (FindNewTarget)
	{
		Globals::TargetID = 0;
		Globals::Target = nullptr;
		HitBox = -1;

		switch (Menu::Window.RageBotTab.TargetSelection.GetIndex())
		{
		case 0:
			Globals::TargetID = GetTargetCrosshair();
			break;
		case 1:
			Globals::TargetID = GetTargetDistance();
			break;
		case 2:
			Globals::TargetID = GetTargetHealth();
			break;
		}

		if (Globals::TargetID >= 0)
		{
			Globals::Target = Interfaces::EntList->GetClientEntity(Globals::TargetID);
		}
		else
		{
			Globals::Target = nullptr;
			HitBox = -1;
			Globals::Shots = 0;
		}
	}

	if (Globals::TargetID >= 0 && Globals::Target)
	{
		HitBox = HitScan(Globals::Target);

		if (!CanOpenFire()) // silent = not stare xd
			return;

		if (Menu::Window.RageBotTab.AimbotKeyPress.GetState())
		{
			int Key = Menu::Window.RageBotTab.AimbotKeyBind.GetKey();
			if (Key >= 0 && !GUI.GetKeyState(Key))
			{
				Globals::TargetID = -1;
				Globals::Target = nullptr;
				HitBox = -1;
				return;
			}
		}

		//Vector AimPoint = GetHitboxPosition(Globals::Target, HitBox);
		Vector AimPoint = GetHitBoxPosV2(Globals::Target, HitBox);

		if (Menu::Window.RageBotTab.AimbotPredictVelocity.GetState())
		{
			Point = Globals::Target->GetPredicted(AimPoint);
		}
		else
		{
			Point = AimPoint;
		}

		if (Menu::Window.RageBotTab.WeaponSettingsEnable.GetState())
		{
			SyncWeaponHitchance();
		}
		else
		{
			WeaponHitchance = 0;
		}

		if (Menu::Window.RageBotTab.WeaponSettingsEnable.GetState())
		{
			SyncWeaponHitchance();
		}
		else
		{
			WeaponHitchance = 0;
		}

		if (GameUtils::IsScopedWeapon(CSWeapon) && !CSWeapon->IsScoped() && Menu::Window.RageBotTab.AccuracyAutoScope.GetState()) /*Auto Scope*/
		{
			pCmd->buttons |= IN_ATTACK2;
		}
		else
		{
			if ((WeaponHitchance * 2 <= hitchance(LocalPlayer)) || WeaponHitchance == 0)
			{
				if (AimAtPoint(LocalPlayer, Point, pCmd, bSendPacket))
				{
					if (Menu::Window.RageBotTab.AimbotAutoFire.GetState() && !(pCmd->buttons & IN_ATTACK))
					{
						pCmd->buttons |= IN_ATTACK;
					}
				}
				else if (Menu::Window.RageBotTab.AimbotAutoFire.GetState())
				{
					pCmd->buttons |= IN_ATTACK;
				}
			}
		}
	}

	if (IsAbleToShoot(LocalPlayer) && pCmd->buttons & IN_ATTACK)
		Globals::Shots += 1;

	if (GameUtils::IsPistol(CSWeapon) && Menu::Window.RageBotTab.AimbotAutoPistol.GetState())
	{
		if ((pCmd->buttons & IN_ATTACK) && (CSWeapon->GetNextPrimaryAttack() - ((float)LocalPlayer->GetTickBase()
			* Interfaces::Globals->interval_per_tick) > 0)) pCmd->buttons &= ~IN_ATTACK;
	}
}

void CRageBot::DoFakelag(CUserCmd *pCmd, bool& bSendPacket)
{
	if (Menu::Window.MiscTab.FakeLagWhileShooting.GetState() && pCmd->buttons & IN_ATTACK) return;

	bool WasShooting = false;
	int iChoke = Menu::Window.MiscTab.FakeLagChoke.GetValue();
	static int iFakeLag = 0;

	if (!WasShooting) // Normal
	{
		if (iFakeLag <= iChoke && iFakeLag > 0)
		{
			bSendPacket = false;
		}
		else
		{
			bSendPacket = true;
			iFakeLag = 0;
		}
	}
	else
	{
		if (bSendPacket)
			WasShooting = false;
		else
			WasShooting = true;
	}
	iFakeLag++;
}

bool CRageBot::TargetMeetsRequirements(IClientEntity* pEntity)
{
	if (pEntity && pEntity->IsDormant() == false && pEntity->IsAlive() && pEntity->GetIndex() != hackManager.pLocal()->GetIndex())
	{
		ClientClass *pClientClass = pEntity->GetClientClass();
		player_info_t pinfo;
		if (pClientClass->m_ClassID == (int)CSGOClassID::CCSPlayer && Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &pinfo))
		{
			if (pEntity->GetTeamNum() != hackManager.pLocal()->GetTeamNum() || Menu::Window.RageBotTab.TargetFriendlyFire.GetState())
			{
				if (!pEntity->HasGunGameImmunity())
				{
					return true;
				}
			}
		}
	}
	return false;
}

float CRageBot::FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int aHitBox)
{
	CONST FLOAT MaxDegrees = 180.f;
	Vector Angles = View;
	Vector Origin = ViewOffSet;

	// Create and intiialize vectors for calculations below
	Vector Delta(0, 0, 0);
	Vector Forward(0, 0, 0);

	// Convert angles to normalized directional forward vector
	AngleVectors(Angles, &Forward);
	Vector AimPos = GetHitboxPosition(pEntity, aHitBox);

	// Get delta vector between our local eye position and passed vector
	VectorSubtract(AimPos, Origin, Delta);
	Delta = AimPos - Origin;

	Normalize(Delta, Delta);

	FLOAT DotProduct = Forward.Dot(Delta);

	// Time to calculate the field of view
	return (acos(DotProduct) * (MaxDegrees / PI));
}

int CRageBot::GetTargetCrosshair()
{
	int target = -1;
	float minFoV = Menu::Window.RageBotTab.AimbotFov.GetValue();

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + *pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (fov < minFoV)
				{
					minFoV = fov;
					target = i;
				}
			}
		}
	}
	return target;
}

int CRageBot::GetTargetDistance()
{
	int target = -1;
	int minDist = 8192;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + *pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				Vector Difference = pLocal->GetOrigin() - pEntity->GetOrigin();
				int Distance = Difference.Length();
				float fov = FovToPlayer(pLocal->GetEyePos(), View, pEntity, 0);
				if (Distance < minDist && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minDist = Distance;
					target = i;
				}
			}
		}
	}
	return target;
}

int CRageBot::GetTargetHealth()
{
	int target = -1;
	int minHealth = 101;

	IClientEntity* pLocal = hackManager.pLocal();
	Vector ViewOffset = pLocal->GetOrigin() + *pLocal->GetViewOffset();
	Vector View; Interfaces::Engine->GetViewAngles(View);

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			int NewHitBox = HitScan(pEntity);
			if (NewHitBox >= 0)
			{
				int Health = pEntity->GetHealth();
				float fov = FovToPlayer(ViewOffset, View, pEntity, 0);
				if (Health < minHealth && fov < Menu::Window.RageBotTab.AimbotFov.GetValue())
				{
					minHealth = Health;
					target = i;
				}
			}
		}
	}
	return target;
}

int CRageBot::HitScan(IClientEntity* pEntity)
{
	IClientEntity* pLocal = hackManager.pLocal();
	std::vector<int> HitBoxesToScan;

	int HitScanMode = Menu::Window.RageBotTab.TargetHitscan.GetIndex();
	int iSmart = Menu::Window.RageBotTab.AccuracySmart.GetValue();
	bool Multipoint = Menu::Window.RageBotTab.TargetMultipoint.GetState();

	if (Menu::Window.RageBotTab.PreferBodyAim.GetIndex() == 2) // Damage based Body aim
	{
		HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);       // 125% (1.25X)
		HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);      // 125% (1.25X)

		HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);    // 	100% (1X)
		HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);         // 	100% (1X)
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);  // 	100% (1X)
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm); // 	100% (1X)

		HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);    // 75% (0.75X)
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);     // 75% (0.75X)
		HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);     // 75% (0.75X)
		HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);      // 75% (0.75X)
	}
	else
	{
		if (HitScanMode == 0) /*Single Hitbox Setups*/
		{
			switch (Menu::Window.RageBotTab.TargetHitbox.GetIndex())
			{
			case 0:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				break;
			case 1:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				break;
			case 2:
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				break;
			case 3:
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				break;
			case 4:
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				break;
			}
		}
		else
		{
			switch (HitScanMode) /*Multibox Setups*/
			{
			case 1: /*Normal*/
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				break;

			case 2: /*All Medium*/
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				break;

			case 3://All high Credits chubakli


				HitBoxesToScan.push_back((int)CSGOHitboxID::Head); // 400% (4X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck); // 400% (4X) Chance
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);  // 125% (1.25X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach); // 125% (1.25X)

				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);    //     100% (1X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);         //     100% (1X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);    //     100% (1X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);     //     100% (1X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);  //     100% (1X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm); //     100% (1X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);  //     100% (1X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm); //     100% (1X)

				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh); // 75% (0.75X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);  // 75% (0.75X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);  // 75% (0.75X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);   // 75% (0.75X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);   // 75% (0.75X)
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);  // 75% (0.75X)
				break;

			case 4:/*All Extreme*/
				HitBoxesToScan.push_back((int)CSGOHitboxID::Head);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Neck);
				HitBoxesToScan.push_back((int)CSGOHitboxID::NeckLower);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Stomach);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Pelvis);
				HitBoxesToScan.push_back((int)CSGOHitboxID::UpperChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::Chest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LowerChest);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightUpperArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightThigh);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightHand);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightFoot);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightShin);
				HitBoxesToScan.push_back((int)CSGOHitboxID::LeftLowerArm);
				HitBoxesToScan.push_back((int)CSGOHitboxID::RightLowerArm);
				break;
			}
		}
	}

	bool AWall = Menu::Window.RageBotTab.AccuracyAutoWall.GetState();

	for (auto HitBoxID : HitBoxesToScan)
	{
		if (AWall)
		{
			Vector Point = GetHitboxPosition(pEntity, HitBoxID);
			float Damage = 0.f;

			if (CanHit(Point, &Damage))
			{
				float DamageCheck = Menu::Window.RageBotTab.AccuracyMinimumDamage.GetValue();

				if (Menu::Window.RageBotTab.SmartMinDamage.GetState())
				{
					if (Damage >= pEntity->GetHealth()) DamageCheck = pEntity->GetHealth();
				}

				if (Damage >= DamageCheck)
				{
					return HitBoxID;
				}
			}
		}
		else
		{
			if (GameUtils::IsVisible(hackManager.pLocal(), pEntity, HitBoxID))
				return HitBoxID;
		}
	}
	return -1;
}

void CRageBot::DoNoRecoil(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();
	if (pLocal)
	{
		Vector AimPunch = pLocal->localPlayerExclusive()->GetAimPunchAngle();
		if (AimPunch.Length2D() > 0 && AimPunch.Length2D() < 150)
		{
			pCmd->viewangles -= AimPunch * 2;
			GameUtils::NormaliseViewAngle(pCmd->viewangles);
		}
	}
}

bool CRageBot::AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket) //brown lick ur armm tooth
{
	bool ReturnValue = false;
	if (point.Length() == 0) return ReturnValue;

	//if (Menu::Window.RageBotTab.TargetPointscale.GetValue() > 0.50)
	//{
	//	float PointScale = Menu::Window.RageBotTab.TargetPointscale.GetValue();//
	//	float maxpsx = PointScale * 4;
	//	point.z += maxpsx - 0.50;

	//}
	//if (Menu::Window.RageBotTab.TargetPointscale.GetValue() < 0.50)
	//{
	//	float PointScale = Menu::Window.RageBotTab.TargetPointscale.GetValue();
	//	float maxpsx = PointScale * 4;
	//	point.z -= 0.50 - maxpsx;
	//}

	Vector angles;
	CalcAngle(pLocal->GetEyePos(), point, angles); // Calc angle aimbot method
	GameUtils::NormaliseViewAngle(angles);


	IsLocked = true;

	// Aim Step Calcs

	if (!IsAimStepping)
		LastAimstepAngle = LastAngle; // Don't just use the viewangs because you need to consider aa

	float fovLeft = FovToPlayer(pLocal->GetEyePos(), LastAimstepAngle, Interfaces::EntList->GetClientEntity(Globals::TargetID), 0);

	if (fovLeft > 25.0f && Menu::Window.RageBotTab.AimbotAimStep.GetState())
	{
		Vector AddAngs = angles - LastAimstepAngle;
		Normalize(AddAngs, AddAngs);
		AddAngs *= 25;
		LastAimstepAngle += AddAngs;
		GameUtils::NormaliseViewAngle(LastAimstepAngle);
		angles = LastAimstepAngle;
	}
	else
	{
		ReturnValue = true;
	}

	if (Menu::Window.RageBotTab.Silentaim.GetState() || Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
	{
		pCmd->viewangles = angles;
	}

	if (!Menu::Window.RageBotTab.Silentaim.GetState() && !Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState())
		Interfaces::Engine->SetViewAngles(angles);

	Vector Oldview = pCmd->viewangles;

	if (Menu::Window.RageBotTab.AimbotPerfectSilentAim.GetState()) {

		Vector oViewangles = pCmd->viewangles;
		float oForwardmove = pCmd->forwardmove;
		float oSidemove = pCmd->sidemove;

		if (CanOpenFire() && pCmd->buttons & IN_ATTACK)
		{
			bSendPacket = false;
			iChokedticks++;
		}

		if (iChokedticks > iMaxChokedticks)
			bSendPacket = true;
		if (bSendPacket)
		{
			iChokedticks = 0;
			pCmd->viewangles = oViewangles;
			pCmd->forwardmove = oForwardmove;
			pCmd->sidemove = oSidemove;
		}
	}

	return ReturnValue;
}



#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / Interfaces::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( Interfaces::Globals->interval_per_tick *( t ) )

#define LC_NONE				0
#define LC_ALIVE			(1<<0)

#define LC_ORIGIN_CHANGED	(1<<8)
#define LC_ANGLES_CHANGED	(1<<9)
#define LC_SIZE_CHANGED		(1<<10)
#define LC_ANIMATION_CHANGED (1<<11)

#define LAG_COMPENSATION_TELEPORTED_DISTANCE_SQR ( 64.0f * 64.0f )
#define LAG_COMPENSATION_EPS_SQR ( 0.1f * 0.1f )
// Allow 4 units of error ( about 1 / 8 bbox width )
#define LAG_COMPENSATION_ERROR_EPS_SQR ( 4.0f * 4.0f )

LagRecord_t::LagRecord_t()
{
	m_vecPreviousOrigin = Vector(0, 0, 0);
	m_flSimulationTime = 0.0f;
	m_bValidRecord = false;
}
template<class T, class U>
T clamp(T in, U low, U high)
{
	if (in <= low)
		return low;

	if (in >= high)
		return high;

	return in;
}

int GetEstimatedServerTickCount(float latency)
{
	return (int)floorf((float)((float)(latency) / (float)((uintptr_t)&Interfaces::Globals->interval_per_tick)) + 0.5) + 1 + (int)((uintptr_t)&Interfaces::Globals->tickcount);
}

float GetNetworkLatency()
{
	// Get true latency
	INetChannelInfo *nci = Interfaces::Engine->GetNetChannelInfo();
	if (nci)
	{
		float IncomingLatency = nci->GetAvgLatency(FLOW_INCOMING);
		//float OutgoingLatency = nci->GetAvgLatency(FLOW_OUTGOING);
		return IncomingLatency;// +OutgoingLatency;
	}
	return 0.0f;
}

void CRageBot::Cache()
{
	IClientEntity* pLP = Interfaces::EntList->GetBaseEntity(Interfaces::Engine->GetLocalPlayer());

	static int s_dwTickbaseoffs = GET_NETVAR(XorStr("DT_BasePlayer"), XorStr("localdata"), XorStr("m_nTickBase"));
	static int s_dwSimtimmeoffs = GET_NETVAR(XorStr("DT_BaseEntity"), XorStr("m_flSimulationTime"));

	int iTickBase = *(int*)((unsigned long)pLP + s_dwTickbaseoffs);
	float flCurrentTime = Interfaces::Globals->interval_per_tick * iTickBase;

	for (int s = 1; s <= Interfaces::Globals->maxClients; ++s)
	{
		m_PlayerTable[s].m_bValidRecord = false;

		IClientEntity* pEntity = Interfaces::EntList->GetBaseEntity(s);

		if (!pEntity)
			continue;
		if (!pEntity->IsValid())
			continue;

		float flSimTime = *(float*)((DWORD)pEntity + s_dwSimtimmeoffs);

		if (m_PlayerTable[s].m_vecPreviousOrigin.Length() > 0) // Origin delta differance rejection
		{
			Vector delta = pEntity->GetAbsOrigin() - m_PlayerTable[s].m_vecPreviousOrigin;

			if (delta.Length2DSqr() > 64)
			{
				m_PlayerTable[s].m_vecPreviousOrigin = pEntity->GetAbsOrigin();
				m_PlayerTable[s].m_vecPreviousAngles = pEntity->GetAbsAngles();
				m_PlayerTable[s].m_flPreviousSimulationTime = *pEntity->GetSimulationTime();
				continue;
			}
		}
		else
		{
			m_PlayerTable[s].m_vecPreviousOrigin = pEntity->GetAbsOrigin();
		}

		m_PlayerTable[s].m_vecOrigin = pEntity->GetAbsOrigin();
		m_PlayerTable[s].m_vecAngles = pEntity->GetAbsAngles();
		m_PlayerTable[s].m_flSimulationTime = *pEntity->GetSimulationTime();

		if (fabs(flSimTime - flCurrentTime) > 1.0f)
			continue;

		float latency = GetNetworkLatency();
		const float SV_MAXUNLAG = 1.0f;
		float m_flLerpTime = Interfaces::CVar->FindVar("cl_interp_ratio")->fValue / Interfaces::CVar->FindVar("cl_updaterate")->fValue;
		int lerpTicks = TIME_TO_TICKS(m_flLerpTime);
		Interfaces::Globals->tickcount += lerpTicks;
		float correct = clamp(latency + TICKS_TO_TIME(lerpTicks), 0.0f, SV_MAXUNLAG);
		int targettick = Interfaces::Globals->tickcount - lerpTicks;
		float deltaTime = correct - TICKS_TO_TIME(GetEstimatedServerTickCount(latency) - targettick);

		if (fabs(deltaTime) > 0.2f)
		{
			continue;
		}

		m_PlayerTable[s].m_flSimulationTime = flSimTime;
		m_PlayerTable[s].m_bValidRecord = true;
		BacktrackEntity(pEntity, targettick); // Backtrack
	}
}

void CRageBot::BacktrackEntity(IClientEntity *pPlayer, float flTargetTime)
{
	Vector org, mins, maxs;
	QAngle ang;
	int pl_index = pPlayer->GetIndex();
	IClientEntity* m_pCurrentPlayer = pPlayer;

	float frac = 0.0f;
	if (m_PlayerTable[pl_index].m_bValidRecord &&
		(m_PlayerTable[pl_index].m_flSimulationTime < flTargetTime) &&
		(m_PlayerTable[pl_index].m_flSimulationTime < m_PrevPlayerTable[pl_index].m_flSimulationTime))
	{
		// we didn't find the exact time but have a valid previous record
		// so interpolate between these two records;

		Assert(prevRecord->m_flSimulationTime > record->m_flSimulationTime);
		Assert(flTargetTime < prevRecord->m_flSimulationTime);

		// calc fraction between both records
		frac = (flTargetTime - m_PlayerTable[pl_index].m_flSimulationTime) /
			(m_PrevPlayerTable[pl_index].m_flSimulationTime - m_PlayerTable[pl_index].m_flSimulationTime);

		Assert(frac > 0 && frac < 1); // should never extrapolate

		ang = Lerp(frac, m_PlayerTable[pl_index].m_vecAngles, m_PrevPlayerTable[pl_index].m_vecAngles);
		org = Lerp(frac, m_PlayerTable[pl_index].m_vecOrigin, m_PrevPlayerTable[pl_index].m_vecOrigin);
	}
	else
	{
		// we found the exact record or no other record to interpolate with
		// just copy these values since they are the best we have
		ang = m_PlayerTable[pl_index].m_vecAngles;
		org = m_PlayerTable[pl_index].m_vecOrigin;
	}

	// See if this represents a change for the player
	int flags = 0;
	LagRecord *restore = &m_RestoreData[pl_index];
	LagRecord *change = &m_ChangeData[pl_index];

	QAngle angdiff = pPlayer->GetAbsAngles() - ang;
	Vector orgdiff = pPlayer->GetAbsOrigin() - org;

	// Always remember the pristine simulation time in case we need to restore it.
	restore->m_flSimulationTime = *pPlayer->GetSimulationTime();

	flags |= LC_ANGLES_CHANGED;
	restore->m_vecAngles = pPlayer->GetAbsAngles();
	//pPlayer->GetAbsAngles(ang);
	change->m_vecAngles = ang;

	flags |= LC_ORIGIN_CHANGED;
	restore->m_vecOrigin = pPlayer->GetAbsOrigin();
	*pPlayer->GetOriginPtr() = org;
	change->m_vecOrigin = org;

	m_bNeedToRestore = true;  // we changed at least one player
	restore->m_fFlags = flags; // we need to restore these flags
	change->m_fFlags = flags; // we have changed these flags
}

bool CRageBot::ProcessCmd(int iTargetIndex, CUserCmd* pCmd)
{
	if (iTargetIndex == -1 || iTargetIndex > Interfaces::Globals->maxClients)
		return false;

	if (!m_PlayerTable[iTargetIndex].m_bValidRecord)
		return false;

	pCmd->tick_count = TIME_TO_TICKS(m_PlayerTable[iTargetIndex].m_flSimulationTime);

	return true;
}
