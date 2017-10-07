#include "Resolver.h"

int Globals::Shots = 0;
bool Globals::change;
CUserCmd* Globals::UserCmd;
int Globals::TargetID;
IClientEntity* Globals::Target;

bool Globals::AutoBody = false;

bool Globals::DrawLBY = false;
bool Globals::LBYUpdated = false;

float oldlowerbodyyaw;

void R::Resolve()
{
	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	for (int i = 0; i < Interfaces::Engine->GetMaxClients(); ++i)
	{
		IClientEntity *pEntity = Interfaces::EntList->GetClientEntity(i);

		if (!pEntity || pEntity->IsDormant() || !pEntity->IsAlive())
			continue;

		player_info_t pTemp;
		if (!Interfaces::Engine->GetPlayerInfo(i, &pTemp))
			continue;

		Vector* eyeAngles = pEntity->EyeAnglesPointer();

		if (Menu::Window.RageBotTab.AccuracyResolveYaw.GetIndex() == 1) // Delta Jitter
		{
			static bool jitter = true;
			jitter = !jitter;

			static bool isMoving;
			float PlayerIsMoving = abs(pEntity->GetVelocity()->Length());
			if (PlayerIsMoving > 0.1) isMoving = true;
			else if (PlayerIsMoving <= 0.1) isMoving = false;

			static float StoredLBY = 0.0f;
			static bool bLowerBodyIsUpdated;
			if (pEntity->GetLowerBodyYaw() != StoredLBY) bLowerBodyIsUpdated = true;
			else bLowerBodyIsUpdated = false;

			float bodyeyedelta = eyeAngles->y - pEntity->GetLowerBodyYaw();

			if (bLowerBodyIsUpdated || isMoving || fabsf(bodyeyedelta) >= 35.0f)
			{
				eyeAngles->y = pEntity->GetLowerBodyYaw();
				StoredLBY = pEntity->GetLowerBodyYaw();
			}
			else
			{
				if (fabsf(bodyeyedelta) < 35.0f && fabsf(bodyeyedelta) > 0.0f)
				{
					if (bodyeyedelta > 0.0f)
					{
						if (jitter)
							eyeAngles->y = pEntity->GetLowerBodyYaw() + fabsf(bodyeyedelta - 35.0f);
						else
							eyeAngles->y = pEntity->GetLowerBodyYaw() + fabsf(bodyeyedelta + 15.0f);
					}
					if (bodyeyedelta < 0.0f)
					{
						if (jitter)
							eyeAngles->y = pEntity->GetLowerBodyYaw() - fabsf(bodyeyedelta + 35.0f);
						else
							eyeAngles->y = pEntity->GetLowerBodyYaw() - fabsf(bodyeyedelta - 15.0f);
					}
				}
			}
		}



		if (Menu::Window.RageBotTab.PreferBodyAim.GetIndex() == 1)
		{
			if (pEntity != Globals::Target) continue;

			static bool isMoving;
			float PlayerIsMoving = abs(pEntity->GetVelocity()->Length());
			if (PlayerIsMoving > 0.1f) isMoving = false;
			else if (PlayerIsMoving <= 0.1f) isMoving = false;

			if (pEntity->GetEyeAngles().y != !pEntity->GetLowerBodyYaw() && !isMoving)
			{
				Globals::AutoBody = true;
			}
			else
			{
				Globals::AutoBody = false;
			}
		}


		if (Menu::Window.RageBotTab.AccuracyResolvePitch.GetIndex() == 1)
		{
			std::string strPitch = std::to_string(eyeAngles->x);

			if (eyeAngles->x < -179.f) eyeAngles->x += 360.f;
			else if (eyeAngles->x > 90.0 || eyeAngles->x < -90.0) eyeAngles->x = 89.f;
			else if (eyeAngles->x > 89.0 && eyeAngles->x < 91.0) eyeAngles->x -= 90.f;
			else if (eyeAngles->x > 179.0 && eyeAngles->x < 181.0) eyeAngles->x -= 180;
			else if (eyeAngles->x > -179.0 && eyeAngles->x < -181.0) eyeAngles->x += 180;
			else if (fabs(eyeAngles->x) == 0) eyeAngles->x = std::copysign(89.0f, eyeAngles->x);
		}
	}
}