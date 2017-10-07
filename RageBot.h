/*
Supers oneless aimbot framework 7/17/2017
*/

#pragma once

#include "Hacks.h"

#define TICK_INTERVAL			(Interfaces::Globals->interval_per_tick) // Source SDK 2013


#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) ) // Source SDK 2013
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) ) // Source SDK 2013
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )// Source SDK 2013
#define TICK_NEVER_THINK		(-1) // Source SDK 2013
#define MAX_PLAYERS 64

template< class T, class Y >
T Clamp(T const &val, Y const &minVal, Y const &maxVal)
{
	if (val < minVal)
		return minVal;
	else if (val > maxVal)
		return maxVal;
	else
		return val;
}

template <class T>
FORCEINLINE T Lerp(float flPercent, T const &A, T const &B)
{
	return A + (B - A) * flPercent;
}

struct LagRecord_t
{
	LagRecord_t();
	float						m_flSimulationTime;
	bool						m_bValidRecord;
	Vector						m_vecAngles;
	Vector						m_vecOrigin;
	Vector						m_vecPreviousAngles;
	Vector						m_vecPreviousOrigin;
	float						m_flPreviousSimulationTime;
};

class CRageBot : public CHack
{
public:
	void Init();
	void Draw();
	void Move(CUserCmd *pCmd, bool &bSendPacket);

	// ============ Backtracking Definitons ============ //

	void						Cache();
	bool						ProcessCmd(int iTargetIndex, CUserCmd* pCmd);
	std::vector< LagRecord >	m_PlayerTrack[128];
	bool					    m_bNeedToRestore;
	LagRecord				    m_RestoreData[128];	// player data before we moved him back
	LagRecord				    m_ChangeData[128];	// player data where we moved him back
	IClientEntity				*m_pCurrentPlayer;	// The player we are doing lag compensation for
	float					    m_flTeleportDistanceSqr;
	bool					    m_isCurrentlyDoingCompensation;	// Sentinel to prevent calling StartLagCompensation a second time before a Finish.
	void                        BacktrackEntity(IClientEntity *pPlayer, float flTargetTime);

	// ================================================= //

private:

	int GetTargetCrosshair();
	int GetTargetDistance();
	int GetTargetHealth();
	bool TargetMeetsRequirements(IClientEntity* pEntity);
	void DoFakelag(CUserCmd *pCmd, bool& bSendPacket);
	float FovToPlayer(Vector ViewOffSet, Vector View, IClientEntity* pEntity, int HitBox);
	int HitScan(IClientEntity* pEntity);
	void SyncWeaponHitchance();
	float                       WeaponHitchance;
	bool AimAtPoint(IClientEntity* pLocal, Vector point, CUserCmd *pCmd, bool &bSendPacket);
	float hitchance(IClientEntity* pLocal);

	// Functionality
	void DoAimbot(CUserCmd *pCmd, bool &bSendPacket);
	void DoNoRecoil(CUserCmd *pCmd);
	void PositionAdjustment(CUserCmd* pCmd);


	/*Anti-Aim Functions*/
	void RageAntiAim(CUserCmd *pCmd, bool&bSendPacket);
	void aimAtPlayer(CUserCmd* pCmd);
	void DisableInterpolation(CUserCmd* pCmd);

	bool IsAimStepping;
	Vector LastAimstepAngle;
	Vector LastAngle;

	// Aimbot
	bool IsLocked;
	int HitBox;
	Vector AimPoint;
	LagRecord_t					m_PlayerTable[MAX_PLAYERS];
	LagRecord_t					m_PrevPlayerTable[MAX_PLAYERS];

	int iMaxChokedticks = 14;
	int iChokedticks = -1;
};