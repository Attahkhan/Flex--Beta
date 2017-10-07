#include "RageBot.h"
#include "RenderManager.h"
#include "Resolver.h"
#include "Autowall.h"
#include <iostream>
#include "UTIL Functions.h"

#define RandomInt(min, max) (rand() % (max - min + 1) + min)

bool IsAbleToCoon(IClientEntity* pLocal)
{
	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(pLocal->GetActiveWeaponHandle());

	if (!pLocal)
		return false;

	if (!pWeapon)
		return false;

	float flServerTime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

	return (!(pWeapon->GetNextPrimaryAttack() > flServerTime));
}

namespace AntiAimPitch {

	void CustomPitch(CUserCmd *pCmd, bool &bSendPacket)
	{
		int realcustompitch = Menu::Window.RageBotTab.AntiAimPitchSlider.GetValue();
		pCmd->viewangles.y = realcustompitch;
	}

	void SwitchShots(CUserCmd* pCmd, bool& bSendPacket) {

		IClientEntity* pLocal = hackManager.pLocal();

		if (pCmd->command_number % 3)
		{
			int value = (pLocal->GetShotsFired()) % 4;
			switch (value) {
			case 0:pCmd->viewangles.x + 89; break;
			case 1:pCmd->viewangles.x - 44; break;
			case 2:pCmd->viewangles.x + 44; break;
			case 3:pCmd->viewangles.x - 89.; break;
			}
		}
	}
	
}

void AntiAimBuilder(CUserCmd* pCmd, bool& bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	if (pCmd->command_number % 2)
	{
		float server_time = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;


		int maxJitter;
		float temp;

		int random = rand() % 100;
		int random2 = rand() % 1000;
		static bool dir;
		static float current_y = (pCmd->viewangles[1]);

		static bool bFlipYaw;
		float flInterval = Interfaces::Globals->interval_per_tick;
		float flTickcount = pCmd->tick_count;
		float flTime = flInterval * flTickcount;

		if (std::fmod(flTime, 1) == 0.f)
			bFlipYaw = !bFlipYaw;

		switch (Menu::Window.RageBotTab.AAYawReal.GetIndex()) // Real yaws - bSendPacket = false
		{
		case 1:
			bSendPacket = false;
			pCmd->viewangles[1] -= 180.00f; // backward
			break;

		case 2:
			bSendPacket = false;
			pCmd->viewangles[1] -= 0.00f; // forward
			break;

		case 3:
			bSendPacket = false;
			pCmd->viewangles[1] += 90.00f; // left
			break;

		case 4:
			bSendPacket = false;
			pCmd->viewangles[1] -= 90.00f; // right
			break;

		case 5:
			bSendPacket = false;
			(pCmd->viewangles[1]) = (float)(fmod(server_time / 0.19f * 360.0f, 360.0f)); // spin fast
			break;

		case 6:
			bSendPacket = false;
			(pCmd->viewangles[1]) += bFlipYaw ? -(float)(fmod(server_time / 0.19f * 360.0f, 360.0f)) : (float)(fmod(server_time / 0.19f * 360.0f, 360.0f));
			break;

		case 7:
			bSendPacket = false;
			(pCmd->viewangles[1]) += bFlipYaw ? 90.f : -90.f;
			break;

		case 8:
			bSendPacket = false;
			if (random == 1) dir = !dir;
			if (dir)
				current_y -= 100.9;
			else
				current_y += 100.9;
			(pCmd->viewangles[1]) = current_y;
			if (random == random2)
				(pCmd->viewangles[1]) -= random;
			break;

		case 9:
			{
			bSendPacket = false;
			pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 180.f;
			}
		}
	}
	else
	{
		float server_time = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

		int random = rand() % 100;
		int random2 = rand() % 1000;
		static bool dir;
		static float current_y = (pCmd->viewangles[1]);

		static bool bFlipYaw;
		float flInterval = Interfaces::Globals->interval_per_tick;
		float flTickcount = pCmd->tick_count;
		float flTime = flInterval * flTickcount;

		if (std::fmod(flTime, 1) == 0.f)
			bFlipYaw = !bFlipYaw;

		switch (Menu::Window.RageBotTab.AAYawFake.GetIndex())
		{
		case 1:
			bSendPacket = true;
			pCmd->viewangles[1] -= 180.00f; // backward
			break;

		case 2:
			bSendPacket = true;
			pCmd->viewangles[1] -= 0.00f; // forward
			break;

		case 3:
			bSendPacket = true;
			pCmd->viewangles[1] += 90.00f; // left
			break;

		case 4:
			bSendPacket = true;
			pCmd->viewangles[1] -= 90.00f; // right
			break;

		case 5:
			bSendPacket = true;
			(pCmd->viewangles[1]) = (float)(fmod(server_time / 0.19f * 360.0f, 360.0f)); // spin fast
			break;

		case 6:
			bSendPacket = true;
			(pCmd->viewangles[1]) += bFlipYaw ? -(float)(fmod(server_time / 0.19f * 360.0f, 360.0f)) : (float)(fmod(server_time / 0.19f * 360.0f, 360.0f));
			break;

		case 7:
			bSendPacket = true;
			(pCmd->viewangles[1]) += bFlipYaw ? 90.f : -90.f;
			break;

		case 8:
			bSendPacket = true;
			pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw();
			break;
		}
	}
}


namespace AntiAimYaw {

	int random;
	int maxJitter;
	float temp;

	void Static(CUserCmd* pCmd) {

		static bool flip;
		static bool flip2;
		flip = !flip;
		flip2 = !flip2;

		if (flip)
		{
			if (flip2)
				pCmd->viewangles.y += 90.0f;
			else
				pCmd->viewangles.y -= 90.0f;
		}
		else
		{
			pCmd->viewangles.y += 180.0f;
		}
	}

	void Jitter(CUserCmd* pCmd) {

		if (pCmd->command_number % 120)
			pCmd->viewangles.y -= 90.f;
		else
			pCmd->viewangles.y += 90.f;
	}

	void FakeLowerBody1(CUserCmd *pCmd, bool &bSendPacket)
	{
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = hackManager.pLocal()->GetLowerBodyYaw() - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
		{
			bSendPacket = false;
			pCmd->viewangles.y -= temp;
		}

		else if (random < 85 + (rand() % 15))
		{
			bSendPacket = true;
			pCmd->viewangles.y += temp;
		}
	}

	void FakeJitter(CUserCmd* pCmd, bool& bSendPacket) {

		static int jitterangle = 0;

		if (jitterangle <= 1)
		{
			pCmd->viewangles.y += 135;
		}
		else if (jitterangle > 1 && jitterangle <= 3)
		{
			pCmd->viewangles.y += 225;
		}

		static int iChoked = -1;
		iChoked++;
		if (iChoked < 1)
		{
			bSendPacket = false;
			if (jitterangle <= 1)
			{
				pCmd->viewangles.y += 45;
				jitterangle += 1;
			}
			else if (jitterangle > 1 && jitterangle <= 3)
			{
				pCmd->viewangles.y -= 45;
				jitterangle += 1;
			}
			else
			{
				jitterangle = 0;
			}
		}
		else
		{
			bSendPacket = true;
			iChoked = -1;
		}
	}

	void FakeSpin(CUserCmd* pCmd, bool& bSendPacket) {

		int random = rand() % 100;
		int random2 = rand() % 1000;
		static bool dir;
		static float current_y = pCmd->viewangles.y;
		IClientEntity* pLocal = hackManager.pLocal();
		float server_time = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;

		if (bSendPacket) {
			pCmd->viewangles.y += (float)(fmod(server_time / 0.39f * 360.0f, 360.0f));
		}
		else
		{
			pCmd->viewangles.y -= (float)(fmod(server_time / 0.39f * 360.0f, 360.0f));
		}
	}

	void FakeBackward(CUserCmd* pCmd, bool& bSendPacket) {

		if (pCmd->command_number % 2)
		{
			bSendPacket = false;
			pCmd->viewangles.y -= 58.f;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y += 180.f;
		}
	}

	void FakeForward(CUserCmd* pCmd, bool& bSendPacket) {

		if (pCmd->command_number % 2)
		{
			bSendPacket = false;
			pCmd->viewangles.y += 125.f;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 0.f;
		}
	}

	void FakeStatic(CUserCmd* pCmd, bool& bSendPacket) {

		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false;
			static int y2 = -179;
			int spinBotSpeedFast = 360.0f / 1.618033988749895f;;

			y2 += spinBotSpeedFast;

			if (y2 >= 179)
				y2 = -179;

			pCmd->viewangles.y = y2;
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y -= 180;
			ChokedPackets = -1;
		}
	}

	void FakeGenesis(CUserCmd *pCmd, bool& bSendPacket)
	{
		static bool flip;
		flip = !flip;
		if (flip)
		{
			pCmd->viewangles.y += 125;
			bSendPacket = false;
		}
		else
		{
			pCmd->viewangles.y -= 125;
			bSendPacket = true;
		}
	}
	void LBY(CUserCmd* cmd)
	{
		cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw();
	}
	void LBYInverse(CUserCmd* cmd, bool& packet)
	{
		static bool ySwitch;
		ySwitch = !ySwitch;

		if (ySwitch)
		{
			cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 180.f;
			packet = false;
		}
		else
		{
			cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw();
			packet = true;
		}
	}
	void LBYSpin(CUserCmd* cmd, bool& packet)
	{
		static bool ySwitch;
		ySwitch = !ySwitch;

		if (ySwitch)
		{
			float CalculatedCurTime = (Interfaces::Globals->curtime * 3500.0);
			cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + CalculatedCurTime;
			packet = false;
		}
		else
		{
			cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw();
			packet = true;
		}
	}
	void BreakLBY1(CUserCmd *pCmd)
	{
		int flip = (int)floorf(Interfaces::Globals->curtime / 1.1) % 2;
		pCmd->viewangles.y += -90.0f + flip * 90.0f;
		// Look backwards
		pCmd->viewangles.y += 90.f;
	}

	void LBYJitter(CUserCmd* cmd, bool& packet)
	{
		static bool ySwitch;
		static bool jbool;
		static bool jboolt;
		ySwitch = !ySwitch;
		jbool = !jbool;
		jboolt = !jbool;
		if (ySwitch)
		{
			if (jbool)
			{
				if (jboolt)
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() - 90.f;
					packet = false;
				}
				else
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 90.f;
					packet = false;
				}
			}
			else
			{
				if (jboolt)
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() - 125.f;
					packet = false;
				}
				else
				{
					cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + 125.f;
					packet = false;
				}
			}
		}
		else
		{
			cmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw();
			packet = true;
		}
	}
	void FakeBasic(CUserCmd *pCmd, bool &bSendPacket)
	{
		if (hackManager.pLocal()->GetVelocity()->Length() > 0)
		{
			int var1;
			int var2;
			float var3;

			pCmd->viewangles.y += 180;
			var1 = rand() % 100;
			var2 = rand() % (10 - 6 + 1) + 10;
			var3 = var2 - (rand() % var2);
			if (var1 < 60 + (rand() % 15))
				pCmd->viewangles.y -= var3;
			else if (var1 < 100 + (rand() % 15))
				pCmd->viewangles.y += var3;
		}
		else {
			static bool lby;
			lby = !lby;

			if (lby)
			{
				bSendPacket = false;
				pCmd->viewangles.y += 180.f;
			}
			else {
				bSendPacket = true;
				pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + 170;
			}
		}
	}

	void FakeBasic2(CUserCmd *pCmd, bool &bSendPacket)
	{

		static bool bfake;
		bfake = !bfake;

		if (bfake)
		{
			bSendPacket = false;
			pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() + RandomInt(165, 195);
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y = hackManager.pLocal()->GetLowerBodyYaw() - 90.f;
		}
	}

	bool jitter = false;

	void SideDistortion(CUserCmd *pCmd, bool &bSendPacket)
			{
				if (bSendPacket)
				{
					float flRandom = rand() % 5 + 1.f;
					switch (pCmd->tick_count % 4)
					{
					case 0:
						pCmd->viewangles.y -= 170.f - flRandom;
						break;
					case 3:
					case 1:
						pCmd->viewangles.y += 180.f;
						break;
					case 2:
						pCmd->viewangles.y = 170.f; + flRandom;
						break;
					}
				}
			}
		




	//FakeYaw

	//fake yaw
	void Arizona1(CUserCmd *pCmd, bool &bSendPacket) //Jitter Movement mit static bool Fast :)
	{
		static bool Fast = false;
		if (Fast)
		{
			bSendPacket = false; //true angle
			pCmd->viewangles.y = pCmd->viewangles.y - 134.0;
		}
		else
		{
			bSendPacket = true; //fake angle
			pCmd->viewangles.y = pCmd->viewangles.y - 226.0;
		}
		Fast = !Fast;
	}

	void Backwards1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y -= 180.0f;
	}

	void Forward1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 180.0f;
	}

	void SidewaysLeft1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y -= 90.0f;
	}

	void SidewaysRight1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 90.0f;
	}

	void SpinSlow1(CUserCmd *pCmd, bool &bSendPacket)
	{

		int random = rand() % 100;
		int random2 = rand() % 1000;

		static bool dir;
		static float current_y = pCmd->viewangles.y;

		if (random == 1) dir = !dir;

		if (dir)
			current_y += 5.0f;
		else
			current_y -= 5.0f;
		bSendPacket = true; //fake angle
		pCmd->viewangles.y = current_y;

		if (random == random2)
			pCmd->viewangles.y += random;

	}

	void SpinFast1(CUserCmd *pCmd, bool &bSendPacket)
	{
		int random = 160 + rand() % 40;
		static float current_y = pCmd->viewangles.y;
		current_y += random;
		bSendPacket = true; //fake angle
		pCmd->viewangles.y = current_y;
	}

	void JitterBackward1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y -= 180.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}

	void JitterForward1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 180.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}

	void JitterSideways1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y += 90.0f;
		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = maxJitter - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
			pCmd->viewangles.y -= temp;
		else if (random < 85 + (rand() % 15))
			pCmd->viewangles.y += temp;
	}

	void YawZero1(CUserCmd *pCmd, bool &bSendPacket)
	{
		bSendPacket = true; //fake angle
		pCmd->viewangles.y = 0.f;
	}


	void CustomFakeYaw1(CUserCmd *pCmd, bool &bSendPacket)
	{
		int pitchOffset = Menu::Window.RageBotTab.RealAntiAimYawSlider.GetValue(); //AA Offset
		static int ChokedPackets = -1;
		ChokedPackets++;
		if (ChokedPackets < 1)
		{
			bSendPacket = false; // +=180?
		}
		else
		{
			bSendPacket = true;
			pCmd->viewangles.y = pitchOffset;
			ChokedPackets = -1;
		}
	}



	void LowerBody1(CUserCmd *pCmd, bool &bSendPacket)
	{
		static bool f_flip = true;
		f_flip = !f_flip;

		if (f_flip)
		{
			//pCmd->viewangles.y -= hackManager.pLocal()->GetLowerBodyYaw() + 90.00f; //Yaw setzt sich zusammen aus LowerBody + 90° Drehung
			bSendPacket = false;
		}
		else if (!f_flip)
		{
			pCmd->viewangles.y += hackManager.pLocal()->GetLowerBodyYaw() - 90.00f;
			bSendPacket = true;
		}
		f_flip = !f_flip;
	}

	void FakeLowerBody21(CUserCmd *pCmd, bool &bSendPacket)
	{

		random = rand() % 100;
		maxJitter = rand() % (85 - 70 + 1) + 70;
		temp = hackManager.pLocal()->GetLowerBodyYaw() - (rand() % maxJitter);
		if (random < 35 + (rand() % 15))
		{
			bSendPacket = false;
			//pCmd->viewangles.y -= temp;
		}

		else if (random < 85 + (rand() % 15))
		{
			bSendPacket = true;
			pCmd->viewangles.y += temp;
		}
	}

	void FakeEdge(CUserCmd *pCmd, bool &bSendPacket)
	{
		IClientEntity* pLocal = hackManager.pLocal();

		Vector vEyePos = pLocal->GetOrigin() + *pLocal->GetViewOffset();

		CTraceFilter filter;
		filter.pHit = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

		for (int y = 0; y < 360; y++)
		{
			Vector qTmp(10.0f, pCmd->viewangles.y, 0.0f);
			qTmp.y += y;

			if (qTmp.y > 180.0)
				qTmp.y -= 360.0;
			else if (qTmp.y < -180.0)
				qTmp.y += 360.0;

			GameUtils::NormaliseViewAngle(qTmp);

			Vector vForward;

			VectorAngles(qTmp, vForward);

			float fLength = (19.0f + (19.0f * sinf(DEG2RAD(10.0f)))) + 7.0f;
			vForward *= fLength;

			trace_t tr;

			Vector vTraceEnd = vEyePos + vForward;

			Ray_t ray;

			ray.Init(vEyePos, vTraceEnd);
			Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &tr);

			if (tr.fraction != 1.0f)
			{
				Vector angles;

				Vector vNegative = Vector(tr.plane.normal.x * -1.0f, tr.plane.normal.y * -1.0f, tr.plane.normal.z * -1.0f);

				VectorAngles(vNegative, angles);

				GameUtils::NormaliseViewAngle(angles);

				qTmp.y = angles.y;

				GameUtils::NormaliseViewAngle(qTmp);

				trace_t trLeft, trRight;

				Vector vLeft, vRight;
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vLeft);
				VectorAngles(qTmp + Vector(0.0f, 30.0f, 0.0f), vRight);

				vLeft *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));
				vRight *= (fLength + (fLength * sinf(DEG2RAD(30.0f))));

				vTraceEnd = vEyePos + vLeft;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trLeft);

				vTraceEnd = vEyePos + vRight;

				ray.Init(vEyePos, vTraceEnd);
				Interfaces::Trace->TraceRay(ray, MASK_PLAYERSOLID_BRUSHONLY, &filter, &trRight);

				if ((trLeft.fraction == 1.0f) && (trRight.fraction != 1.0f))
					qTmp.y -= 90.f;
				else if ((trLeft.fraction != 1.0f) && (trRight.fraction == 1.0f))
					qTmp.y += 90.f;

				if (qTmp.y > 180.0)
					qTmp.y -= 360.0;
				else if (qTmp.y < -180.0)
					qTmp.y += 360.0;

				pCmd->viewangles.y = qTmp.y;

				int offset = Menu::Window.RageBotTab.AntiAimOffset.GetValue();

				static int ChokedPackets = -1;
				ChokedPackets++;
				if (ChokedPackets < 1)
				{
					bSendPacket = false; // +=180?
				}
				else
				{
					bSendPacket = true;
					pCmd->viewangles.y -= offset;
					ChokedPackets = -1;
				}
				return;
			}
		}
		pCmd->viewangles.y += 360.0f;
	}
	void CustomRealYaw(CUserCmd *pCmd, bool &bSendPacket)
	{
		int realcustomyaw = Menu::Window.RageBotTab.RealAntiAimYawSlider.GetValue();
		pCmd->viewangles.y = realcustomyaw;
	}
}


bool EntityIsValid()
{
	for (int i = 0; i <= Interfaces::EntList->GetHighestEntityIndex(); i++) //nice one psilent INT = int
	{
		IClientEntity* pEntity = (IClientEntity*)Interfaces::EntList->GetClientEntity(i);
		if (pEntity == NULL)
			continue;
		if (!pEntity->IsAlive())
			continue;
		if (pEntity == hackManager.pLocal())
			continue;
		if (pEntity->IsDormant())
			continue;
		if (pEntity->GetTeamNum() == hackManager.pLocal()->GetTeamNum())
			continue;
		player_info_t info;
		if (!(Interfaces::Engine->GetPlayerInfo(pEntity->GetIndex(), &info)))
			continue;
		return true;
	}
	return false;
}

void CRageBot::aimAtPlayer(CUserCmd *pCmd)
{
	IClientEntity* pLocal = hackManager.pLocal();

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());

	if (!pLocal || !pWeapon)
		return;

	Vector eye_position = pLocal->GetEyePos();

	float best_dist = pWeapon->GetCSWpnData()->flRange;

	IClientEntity* target = nullptr;

	for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);
		if (TargetMeetsRequirements(pEntity))
		{
			if (Globals::TargetID != -1)
				target = Interfaces::EntList->GetClientEntity(Globals::TargetID);
			else
				target = pEntity;

			Vector target_position = target->GetEyePos();

			float temp_dist = eye_position.DistTo(target_position);

			if (best_dist > temp_dist)
			{
				best_dist = temp_dist;
				CalcAngle(eye_position, target_position, pCmd->viewangles);
			}
		}
	}
}

void CRageBot::RageAntiAim(CUserCmd *pCmd, bool &bSendPacket)
{
	IClientEntity* pLocal = hackManager.pLocal();

	if ((pCmd->buttons & IN_USE) || pLocal->GetMoveType() == MOVETYPE_LADDER || pLocal->GetMoveType() == MOVETYPE_NOCLIP)
		return;

	if ((IsAimStepping || pCmd->buttons & IN_ATTACK) && !Menu::Window.RageBotTab.PSilentaim.GetState())
		return;

	if (Menu::Window.RageBotTab.AntiAimTarget.GetState())
	{
		aimAtPlayer(pCmd);
	}

	CBaseCombatWeapon* CSWeapon = (CBaseCombatWeapon*)Interfaces::EntList->GetClientEntityFromHandle(hackManager.pLocal()->GetActiveWeaponHandle());
	if (CSWeapon)
	{
		CSWeaponInfo* pWeaponInfo = CSWeapon->GetCSWpnData();

		if (!GameUtils::IsKnife(CSWeapon))
		{
			if (Menu::Window.RageBotTab.AntiAimKnife.GetState())
			{
				if (!IsAbleToCoon(pLocal) || pCmd->buttons & IN_ATTACK2)
					return;
			}
			else
			{
				return;
			}
		}
	}

	if (Menu::Window.RageBotTab.AntiAimEnable.GetIndex() == 2)
	{
		AntiAimBuilder(pCmd, bSendPacket);
	}

	switch (Menu::Window.RageBotTab.AntiAimPitch.GetIndex()) {

	case 1:
		pCmd->viewangles.x = 69.69f; /*Emotion*/
		break;

	case 2:
		pCmd->viewangles.x = 89.00f; /*Down [MM]*/
		break;

	case 3:
		pCmd->viewangles.x = 128.571441f; /*Half-Down*/
		break;

	case 4:
		pCmd->viewangles.x = 180.00f; /*Fake Down [NS]*/
		break;

	case 5:
		AntiAimPitch::SwitchShots(pCmd, bSendPacket); /*Switch On Shots*/
		break;

	case 6:
		pCmd->viewangles.x = 1080.f; /*Fake Down [NS]*/

	case 7:
		float SliderPitch = Menu::Window.RageBotTab.AntiAimPitchSlider.GetValue();
		pCmd->viewangles.x = SliderPitch;
		break;


	}



	switch (Menu::Window.RageBotTab.AntiAimYaw.GetIndex()) {

	case 1:
		pCmd->viewangles.y -= 180.00f; /*Backward*/
		Globals::DrawLBY = false;
		break;

	case 2:
		AntiAimYaw::Static(pCmd);
		Globals::DrawLBY = false;
		break;

	case 3:
		pCmd->viewangles.y -= 64.285705f; /*Half Side*/
		Globals::DrawLBY = false;
		break;

	case 4:
		AntiAimYaw::Jitter(pCmd);
		Globals::DrawLBY = false;
		break;

	case 5:
		AntiAimYaw::FakeLowerBody1(pCmd, bSendPacket);
		Globals::DrawLBY = true;
		break;

	case 6:
		AntiAimYaw::FakeJitter(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;

	case 7:
		AntiAimYaw::FakeSpin(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;

	case 8:
		AntiAimYaw::FakeBackward(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;

	case 9:
		AntiAimYaw::FakeForward(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;

	case 10:
		AntiAimYaw::FakeStatic(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;

	case 11:
		AntiAimYaw::FakeGenesis(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;

	case 12:
		AntiAimYaw::LBYInverse(pCmd, bSendPacket);
		Globals::DrawLBY = true;
		break;
	case 13:
		AntiAimYaw::LBYSpin(pCmd, bSendPacket);
		Globals::DrawLBY = true;
		break;
	case 14:
		// Breaker 1
		AntiAimYaw::BreakLBY1(pCmd);
		Globals::DrawLBY = true;
		break;
	case 15:
		AntiAimYaw::LBYJitter(pCmd, bSendPacket);
		Globals::DrawLBY = true;
		break;
	case 16:
		AntiAimYaw::FakeBasic(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 17:
		AntiAimYaw::SideDistortion(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 19:
		pCmd->viewangles.y = 1440.f; /*Emotion*/
		Globals::DrawLBY = false;
		break;
	case 18:
		float SliderYaw = Menu::Window.RageBotTab.RealAntiAimYawSlider.GetValue();
		pCmd->viewangles.y = SliderYaw;
		Globals::DrawLBY = false;
		break;

	}

	switch (Menu::Window.RageBotTab.AntiAimRoll.GetIndex()) {

	case 1:
		pCmd->viewangles.z = 50.f;
		break;

	case 2:
		break;

	case 3:
		break;

	case 4:
		break;





	}

	switch (Menu::Window.RageBotTab.FakeYawAA.GetIndex()) // Magic pitch is 69.69?
	{
	case 0:
		// No Yaw AA
		break;
	case 1:
		//
		AntiAimYaw::Arizona1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 2:
		// 
		AntiAimYaw::Backwards1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 3:
		// 
		AntiAimYaw::Forward1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 4:
		// 
		AntiAimYaw::SidewaysLeft1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 5:
		//
		AntiAimYaw::SidewaysRight1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 6:
		// 
		AntiAimYaw::SpinSlow1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 7:
		// 
		AntiAimYaw::SpinFast1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 8:
		// 
		AntiAimYaw::JitterBackward1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 9:
		// 
		AntiAimYaw::JitterForward1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 10:
		// 
		AntiAimYaw::JitterSideways1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 11:
		// 
		AntiAimYaw::YawZero1(pCmd, bSendPacket);
		Globals::DrawLBY = false;
		break;
	case 12:
		// 
		AntiAimYaw::LowerBody1(pCmd, bSendPacket);
		Globals::DrawLBY = true;
		break;
	case 13:
		// 
		AntiAimYaw::FakeLowerBody1(pCmd, bSendPacket);
		Globals::DrawLBY = true;
		break;
	}

	if (pCmd->viewangles.y - pLocal->GetLowerBodyYaw() >= -15 && pCmd->viewangles.y - pLocal->GetLowerBodyYaw() <= 15)
	{
		Globals::LBYUpdated = true;
	}
	else
	{
		Globals::LBYUpdated = false;
	}
}
