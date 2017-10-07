/*
Tim's FlexBeta Framework 2017
*/

#pragma once

#include "Hacks.h"

class cGameEventManager : public IGameEventListener
{
public:
	void FireGameEvent(IGameEvent *event);
	void RegisterSelf();
	bool ShouldHitmarker();
	bool ShouldRoundStart();

private:
	bool doHitmarker;
	bool doRoundStart;
};


class CEsp : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);
	cGameEventManager EventMan;
private:

	// Other shit
	IClientEntity *BombCarrier;

	struct ESPBox
	{
		int x, y, w, h;
	};

	// Draw a player
	void DrawPlayer(IClientEntity* pEntity, player_info_t pinfo);


	// Get player info
	Color GetPlayerColor(IClientEntity* pEntity);
	bool GetBox(IClientEntity* pEntity, ESPBox &result);
	ESPBox GetBox(IClientEntity* pEntity);

	ESPBox GetBOXX(IClientEntity* pEntity);


	// Draw shit about player
	void SpecList();
	void DrawBox(ESPBox size, Color color);
	void DrawName(player_info_t pinfo, ESPBox size);
	void Barrel(CEsp::ESPBox size, Color color, IClientEntity * pEntity);
	void DrawAr(IClientEntity* pEntity, ESPBox size);
	void Fill(CEsp::ESPBox size, Color color2);
	void DrawHealth(IClientEntity* pEntity, ESPBox size);
	void DrawHealth1(IClientEntity* pEntity, ESPBox size);
	void DrawDistanse(IClientEntity * pEntity, CEsp::ESPBox size);
	void DrawInfo(IClientEntity* pEntity, ESPBox size);
	void DrawInfo2(IClientEntity* pEntity, ESPBox size);
	void DrawCross(IClientEntity* pEntity);
	void DrawSkeleton(IClientEntity* pEntity);

	void DrawSnap(IClientEntity * pEntity);

	void DrawGlow();

	void DrawMolotov(IClientEntity * pEntity, ClientClass * cClass);

	void DrawSmoke(IClientEntity * pEntity, ClientClass * cClass);

	void DrawDecoy(IClientEntity * pEntity, ClientClass * cClass);

	void DrawHE(IClientEntity * pEntity, ClientClass * cClass);

	void DrawChicken(IClientEntity* pEntity, ClientClass* cClass);
	void DrawDrop(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBombPlanted(IClientEntity* pEntity, ClientClass* cClass);
	void DrawBomb(IClientEntity* pEntity, ClientClass* cClass);
};

