/*
Tim's FlexBeta Framework 2017
*/

#pragma once

#include "GUI.h"
#include "Controls.h"

struct PlayerListItem_t
{
	bool Friendly;
	bool AimPrio;
	bool Callout;
};

DWORD GetPlayerListIndex(int EntId);
bool IsFriendly(int EntId);
bool IsAimPrio(int EntId);
bool IsCalloutTarget(int EntId);


extern std::map<DWORD, PlayerListItem_t> PlayerList;

class CRageBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;

	// Aimbot Settings
	CGroupBox WindowGroup;
	CGroupBox AimbotGroup;
	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CSlider	  AimbotFov;
	CCheckBox Silentaim;
	CCheckBox PSilentaim;
	CCheckBox AimbotPerfectSilentAim;

	CCheckBox AimbotPredictVelocity;

	CCheckBox AimbotAimStep;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CGroupBox TargetGroup;
	CCheckBox VelocityPred;
	CComboBox TargetSelection;
	CComboBox TargetHitbox;
	CSlider   AccuracyHitchance;
	CComboBox TargetHitscan;
	CSliderFloat   TargetPointscale;
	CSlider   AccuracySmart;
	CSlider	  AccuracyMinimumDamage;
	CCheckBox AccuracyAutoWall;
	CCheckBox AimbotAutoPistol;
	CCheckBox AccuracyAutoScope;
	CCheckBox TargetMultipoint;
	CSlider MultipointIndexes;
	CCheckBox TargetFriendlyFire;
	CCheckBox AutoRevolver;
	CKeyBind AimbotStopKey;
	CCheckBox SmartMinDamage;

	CCheckBox BackTrackingCreditsSuper;

	CGroupBox WeaponSettingsGroup;
	CCheckBox WeaponSettingsEnable;

	//Settings

	//Hitchance
	CGroupBox HitNigger;

	CCheckBox SyncHitChance;

	CSlider WeaponPistolHitchance;
	CSlider WeaponSniperHitchance;
	CSlider WeaponRifleHitchance;
	CSlider WeaponSMGHitchance;
	CSlider WeaponOtherHitchance;

	// Accuracy Settings
	CGroupBox AccuracyGroup;
	CCheckBox AccuracyRecoil;
	CComboBox AccuracyAutoStop;
	CCheckBox AccuracyAutoCrouch;
	CComboBox AccuracyResolveYaw;
	CComboBox AccuracyResolvePitch;
	CCheckBox AccuracyPositionAdjustment;
	CCheckBox AccuracySpreadLimit;
	CSlider	  AccuracyMinimumSpread;
	CCheckBox DisableInterpolation;
	CComboBox PreferBodyAim;

	// Anti-Aim Settings
	CGroupBox AntiAimGroup;
	CComboBox AntiAimEnable;
	CComboBox AntiAimPitch;
	CComboBox AntiAimYaw;
	CComboBox FakeYawAA;
	CComboBox AntiAimRoll;
	CCheckBox AntiAimKnife;
	CCheckBox AntiAimTarget;
	CSlider   AntiAimPitchSlider;
	CSlider   RealAntiAimYawSlider;
	CSlider   FakeAntiAimYawSlider;
	CSlider   AntiAimOffset;

	CComboBox AAYawReal;
	CComboBox AAYawFake;
};

class CLegitBotTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Aimbot Settings
	CGroupBox AimbotGroup;

	CCheckBox AimbotEnable;
	CCheckBox AimbotAutoFire;
	CCheckBox AimbotFriendlyFire;
	CCheckBox AimbotKeyPress;
	CKeyBind  AimbotKeyBind;
	CCheckBox AimbotAutoPistol;
	CCheckBox AimbotChicken;
	CCheckBox AimbotSmokeCheck;

	// Main
	CGroupBox TriggerGroup;

	CCheckBox TriggerEnable;
	CCheckBox TriggerKeyPress;
	CKeyBind  TriggerKeyBind;
	CCheckBox TriggerHitChance;
	CSlider2   TriggerHitChanceAmmount;
	CSlider2   TriggerDelay;
	CSlider2   TriggerBurst;
	CSlider2   TriggerBreak;
	CCheckBox TriggerRecoil;

	// Trigger Filter
	CGroupBox TriggerFilterGroup;

	CCheckBox TriggerHead;
	CCheckBox TriggerChest;
	CCheckBox TriggerStomach;
	CCheckBox TriggerArms;
	CCheckBox TriggerLegs;
	CCheckBox TriggerTeammates;

	// Rifles
	CGroupBox WeaponMainGroup;

	CSlider2   WeaponMainSpeed;
	CSlider2   WeaponMainFoV;
	CCheckBox WeaponMainRecoil;
	CComboBox2 WeaponMainHitbox;
	CSlider2 WeaponMainAimtime;
	CSlider2 WeaoponMainStartAimtime;
	CSlider2  WeaponMainRecoilAmountX;
	CSlider2  WeaponMainRecoilAmountY;

	// Pistol
	CGroupBox WeaponPistGroup;

	CSlider2   WeaponPistSpeed;
	CSlider2   WeaponPistFoV;
	CCheckBox WeaponPistRecoil;
	CComboBox2 WeaponPistHitbox;
	CSlider2 WeaponPistAimtime;
	CSlider2 WeaoponPistStartAimtime;
	CSlider2  WeaponPistRecoilAmountX;
	CSlider2  WeaponPistRecoilAmountY;

	// Sniper
	CGroupBox WeaponSnipGroup;

	CSlider2   WeaponSnipSpeed;
	CSlider2   WeaponSnipFoV;
	CCheckBox WeaponSnipRecoil;
	CComboBox2 WeaponSnipHitbox;
	CSlider2 WeaponSnipAimtime;
	CSlider2 WeaoponSnipStartAimtime;

	CSlider2  WeaponSnipRecoilAmountX;
	CSlider2  WeaponSnipRecoilAmountY;


	// MPs
	CGroupBox WeaponMpGroup;

	CSlider2   WeaponMpSpeed;
	CSlider2   WeaponMpFoV;
	CCheckBox WeaponMpRecoil;
	CComboBox2 WeaponMpHitbox;
	CSlider2 WeaponMpAimtime;
	CSlider2 WeaoponMpStartAimtime;
	CSlider2  WeaponMPRecoilAmountX;
	CSlider2  WeaponMPRecoilAmountY;

	// Shotguns
	CGroupBox WeaponShotgunGroup;

	CSlider2   WeaponShotgunSpeed;
	CSlider2   WeaponShotgunFoV;
	CSlider2 WeaponShotgunRecoil;
	CComboBox2 WeaponShotgunHitbox;
	CSlider2 WeaponShotgunAimtime;
	CSlider2 WeaoponShotgunStartAimtime;

	// Machineguns
	CGroupBox WeaponMGGroup;

	CSlider2   WeaponMGSpeed;
	CSlider2   WeaponMGFoV;
	CSlider2 WeaponMGRecoil;
	CComboBox2 WeaponMGHitbox;
	CSlider2 WeaponMGAimtime;
	CSlider2 WeaoponMGStartAimtime;
};

class CVisualTab : public CTab
{
public:
	void Setup();

	// Master Switch
	CLabel ActiveLabel;
	CCheckBox Active;

	// Options Settings
	CGroupBox WindowGroup;
	CGroupBox OptionsGroup;
	CComboBox OptionsBox;
	CCheckBox OptionsName;
	CComboBox OptionsHealth;
	CCheckBox OptionsWeapon;
	CCheckBox OptionsMoney;
	CCheckBox HasC4;
	CCheckBox IsDefusing;
	CCheckBox HasDefuser;
	CCheckBox IsScoped;
	CCheckBox OptionsInfo;
	CCheckBox OptionsHelmet;
	CCheckBox OptionsKit;
	CCheckBox OptionsFill;
	CComboBox OptionsOutline;
	CCheckBox OptionsAr;
	CCheckBox OptionsDefuse;
	CCheckBox OptionsGlow;
	CComboBox OptionsChams;
	CCheckBox OptionsSkeleton;
	CCheckBox OptionsSnap;
	CCheckBox OptionsAimSpot;
	

	CComboBox OtherHandFX;
	CComboBox OtherWeaponFX;

	// Filters Settings
	CGroupBox FiltersGroup;
	CCheckBox FiltersAll;
	CCheckBox FiltersPlayers;
	CCheckBox FiltersEnemiesOnly;
	CCheckBox FiltersChamsAlpha;
	CCheckBox FiltersWeapons;
	CCheckBox FiltersChickens;
	CCheckBox FiltersC4;
	CCheckBox Grenades;

	// World Settings
	CGroupBox WorldGroup;
	CSlider AmbientExposure;
	CSlider AmbientRed;
	CSlider AmbientGreen;
	CSlider AmbientBlue;
	CCheckBox AsusWalls;
	CComboBox AmbientSkybox;
	CCheckBox SnowMode;
	CCheckBox OtherNightMode;
	CCheckBox OtherHitMarker;
	CComboBox WeaponChams;

	// Other Settings
	CGroupBox OtherGroup;
	CCheckBox OtherCrosshair;
	CComboBox OtherRecoilCrosshair;
	CCheckBox OtherHitmarker;
	CCheckBox OtherNoVisualRecoil;
	CCheckBox OtherNoSky; 
	CCheckBox OtherNoFlash; 
	CCheckBox OtherNoSmoke;
	CCheckBox OtherAsusWalls;
	CComboBox OtherNoHands;
	CSlider WeaponBlend;
	CCheckBox NoScope;
	CSlider OtherViewmodelFOV;
	CSlider OtherFOV;
	CCheckBox OtherRadar;
	CCheckBox OtherRadarEnemyOnly;
	CCheckBox OtherRadarVisibleOnly;
	CSlider	  OtherRadarScale;

	CSlider OtherRadarXPosition;
	CSlider OtherRadarYPosition;

	//glow settings

	CGroupBox Glow;
	
	CSlider GlowR;
	CSlider GlowG;
	CSlider GlowB;
	CSlider GlowZ;
};

class CMiscTab : public CTab
{
public:
	void Setup();

	// Knife Changer
	CGroupBox KnifeGroup;
	CCheckBox KnifeEnable;
	CComboBox KnifeModel;
	CComboBox KnifeSkin;
	CButton   KnifeApply;

	CButton   UnloadButton;
	CButton   SaveButton;
	CButton   LoadButton;
	CComboBox ConfigBox;
	CTextField SettingsName;

	CGroupBox Settings;
	CButton SettingsSave;
	CButton SettingsLoad;

	// Other Settings
	CGroupBox WindowGroup;
	CGroupBox OtherGroup;
	CComboBox OtherAutoJump;
	CCheckBox OtherEdgeJump;
	CComboBox OtherAutoStrafe;
	CSlider CircleFactor;
	CCheckBox OtherSafeMode;
	CComboBox OtherChatSpam;
	CCheckBox OtherTeamChat;
	CSlider	  OtherChatDelay;
	CKeyBind  OtherAirGlide;
	CCheckBox OtherSpectators;
	CCheckBox NoInterp;
	CCheckBox OtherThirdperson;
	CComboBox ThirdpersonAAType;
	CKeyBind  OtherCircleKey;
	CCheckBox SkinChanger;
	CCheckBox OtherBulletReg;
	CCheckBox PostProcessing;
	//CCheckBox OtherAutoAccept;
	CCheckBox OtherWalkbot;
	CComboBox OtherClantag;	
	//CCheckBox OptionsHitMarker;
	CCheckBox OtherCompRank;

	// Fake Lag Settings
	CGroupBox FakeLagGroup;
	CCheckBox FakeLagEnable;
	CSlider   FakeLagChoke;
	CCheckBox FakeLagWhileShooting;


	CCheckBox OtherEnginePrediction;
	//CCheckBox FakeLagWhileShooting;

	CGroupBox ConfigGroup;


};

class CColorsTab : public CTab
{
public:
	void Setup();
	CGroupBox WindowGroup;



	// Colors
	CGroupBox ColorsGroup;
	CSlider CTColorVisR;
	CSlider CTColorVisG;
	CSlider CTColorVisB;

	CSlider CTColorNoVisR;
	CSlider CTColorNoVisG;
	CSlider CTColorNoVisB;


	CGroupBox ColorsTGroup;

	CSlider TColorVisR;
	CSlider TColorVisG;
	CSlider TColorVisB;

	CSlider TColorNoVisR;
	CSlider TColorNoVisG;
	CSlider TColorNoVisB;

	CGroupBox Glow;

	CSlider GlowR;
	CSlider GlowG;
	CSlider GlowB;
	CSlider GlowZ;

	CSlider TGlowR;
	CSlider TGlowG;
	CSlider TGlowB;

	CSlider CTGlowR;
	CSlider CTGlowG;
	CSlider CTGlowB;

	CGroupBox Arms;

	CSlider ArmsR;
	CSlider ArmsG;
	CSlider ArmsB;

	CSlider GunsR;
	CSlider GunsG;
	CSlider GunsB;

	CSlider WeaponR;
	CSlider WeaponG;
	CSlider WeaponB;


};

class CMenucolors : public CTab
{
public:
	void Setup();
	//CGroupBox WindowGroup;

	CGroupBox ColorsClearsGroup;
	CSlider   MenuClearR;
	CSlider   MenuClearG;
	CSlider   MenuClearB;

	// Colors
	CGroupBox ColorsBoxGroup;
	CSlider GroupboxR;
	CSlider GroupboxG;
	CSlider GroupboxB;

	CSlider GrouptextR;
	CSlider GrouptextG;
	CSlider GrouptextB;

	CGroupBox ColorsTabGroup;
	CSlider TabareaR;
	CSlider TabareaG;
	CSlider TabareaB;

	CGroupBox ColorsControlsGroup;
	CSlider   MenuR;
	CSlider   MenuG;
	CSlider   MenuB;






};

class CSkinchangerTab : public CTab
{
public:
	void Setup();
	CGroupBox WindowGroup;
	// Knife Changer/Skin Changer
	CLabel SkinActive;
	CCheckBox SkinEnable;
	CButton   SkinApply;
	CButton   GloveApply;
	CListBox lstWeapon;
	CListBox lstSkins;

	// Knife
	CGroupBox KnifeGroup;
	CComboBox2 KnifeModel;
	CComboBox2 KnifeSkin;

	//Gloves
	CGroupBox GloveGroup;
	CComboBox2 GloveModel;
	CComboBox2 GloveSkin;

	// Pistols
	CGroupBox PistolGroup;
	CComboBox2 GLOCKSkin;
	CComboBox2 USPSSkin;
	CComboBox2 DEAGLESkin;
	CComboBox2 MAGNUMSkin;
	CComboBox2 DUALSSkin;
	CComboBox2 FIVESEVENSkin;
	CComboBox2 TECNINESkin;
	CComboBox2 P2000Skin;
	CComboBox2 P250Skin;
	CComboBox2 R8Skin;

	// MPs
	CGroupBox MPGroup;
	CComboBox2 MAC10Skin;
	CComboBox2 P90Skin;
	CComboBox2 UMP45Skin;
	CComboBox2 BIZONSkin;
	CComboBox2 MP7Skin;
	CComboBox2 MP9Skin;

	// Rifles
	CGroupBox Riflegroup;
	CComboBox2 M41SSkin;
	CComboBox2 M4A4Skin;
	CComboBox2 AK47Skin;
	CComboBox2 AUGSkin;
	CComboBox2 FAMASSkin;
	CComboBox2 GALILSkin;
	CComboBox2 SG553Skin;


	// Machineguns
	CGroupBox MachinegunsGroup;
	CComboBox2 NEGEVSkin;
	CComboBox2 M249Skin;

	// Snipers
	CGroupBox Snipergroup;
	CComboBox2 SCAR20Skin;
	CComboBox2 G3SG1Skin;
	CComboBox2 SSG08Skin;
	CComboBox2 AWPSkin;

	// Shotguns
	CGroupBox Shotgungroup;
	CComboBox MAG7Skin;
	CComboBox XM1014Skin;
	CComboBox SAWEDOFFSkin;
	CComboBox NOVASkin;

	// Skinsettings
	CGroupBox SkinsettingsGroup;
	CCheckBox StatTrakEnable;
	CTextField StatTrackAmount;
	CTextField SkinName;
	CTextField KnifeName;
};

class CPlayersTab : public CTab
{
public:
	void Setup();
	CGroupBox WindowGroup;
	CGroupBox grpPList;
	CListBox lstPlayers;

	CCheckBox PlayerFriendly;
	CCheckBox PlayerAimPrio;
	CCheckBox PlayerCalloutSpam;

};

class CGnode : public CTab
{
public:
	void Setup();
    
	CButton Niggertown;

};

class FlexBetaWindow : public CWindow
{
public:
	void Setup();

	CRageBotTab RageBotTab;
	CLegitBotTab LegitBotTab;
	CVisualTab VisualsTab;
	CMiscTab MiscTab;
	CColorsTab ColorsTab;
	CSkinchangerTab SkinchangerTab;
	CMenucolors MenuTab;
	CPlayersTab PlayersTab;
	CGnode    Nodeville;

	CComboBox SettingsFile;
	CLabel UserLabel;

	CButton SaveButton;
	CButton LoadButton;
	CButton UnloadButton;
	CComboBox ConfigBox;
};

namespace Menu
{
	void SetupMenu();
	void DoUIFrame();

	extern FlexBetaWindow Window;
};