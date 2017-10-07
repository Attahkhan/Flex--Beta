/*
Tim's FlexBeta Framework 2017
*/

#include "Menu.h"
#include "Controls.h"
#include "Hooks.h" // for the unload meme
#include "Interfaces.h"
#include "CRC32.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 547

FlexBetaWindow Menu::Window;

void UpdatePlayerList();

std::map<DWORD, PlayerListItem_t> PlayerList;

void SaveConfigCallbk()
{
	switch (Menu::Window.ConfigBox.GetIndex())
	{
	case 0:
		GUI.SaveWindowState(&Menu::Window, "RageMM.cfg");
		break;
	case 1:
		GUI.SaveWindowState(&Menu::Window, "NS.cfg");
		break;
	case 2:
		GUI.SaveWindowState(&Menu::Window, "MMHVH.cfg");
		break;
	case 3:
		GUI.SaveWindowState(&Menu::Window, "Legit1.cfg");
		break;
	case 4:
		GUI.SaveWindowState(&Menu::Window, "Legit2.cfg");
		break;
	}
}

void LoadConfigCallbk()
{
	switch (Menu::Window.ConfigBox.GetIndex())
	{
	case 0:
		GUI.LoadWindowState(&Menu::Window, "RageMM.cfg");
		break;
	case 1:
		GUI.LoadWindowState(&Menu::Window, "NS.cfg");
		break;
	case 2:
		GUI.LoadWindowState(&Menu::Window, "MMHVH.cfg");
		break;
	case 3:
		GUI.LoadWindowState(&Menu::Window, "Legit1.cfg");
		break;
	case 4:
		GUI.LoadWindowState(&Menu::Window, "Legit2.cfg");
		break;
	}
}

void UnLoadCallbk()
{
	DoUnload = true;
}

void SaveCustom()
{
	std::string savename = Menu::Window.MiscTab.SettingsName.getText();
	GUI.SaveWindowState(&Menu::Window, (savename + ".cfg"));
}


void LoadCustom()
{
	std::string loadname = Menu::Window.MiscTab.SettingsName.getText();
	GUI.LoadWindowState(&Menu::Window, (loadname + ".cfg"));
}

void KnifeApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
	//bGlovesNeedUpdate = true;
}

void GloveApplyCallbk()
{
	static ConVar* Meme = Interfaces::CVar->FindVar("cl_fullupdate");
	Meme->nFlags &= ~FCVAR_CHEAT;
	Interfaces::Engine->ClientCmd_Unrestricted("cl_fullupdate");
	bGlovesNeedUpdate = true;
}

void FlexBetaWindow::Setup()
{
	SetPosition(10, 10);
	SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	SetTitle("FlexBeta.club");

	RegisterTab(&RageBotTab);
	RegisterTab(&LegitBotTab);
	RegisterTab(&VisualsTab);
	RegisterTab(&MiscTab);
	RegisterTab(&ColorsTab);
	RegisterTab(&MenuTab);
	RegisterTab(&SkinchangerTab);
	RegisterTab(&PlayersTab);

	RECT Client = GetClientArea();
	Client.bottom -= 29;

	RageBotTab.Setup();
	LegitBotTab.Setup();
	VisualsTab.Setup();
	MiscTab.Setup();
	ColorsTab.Setup();
	MenuTab.Setup();
	SkinchangerTab.Setup();
	PlayersTab.Setup();

#pragma region Bottom Buttons

	ConfigBox.SetFileId("cfg_box");
	ConfigBox.AddItem("Rage MM");
	ConfigBox.AddItem("NS");
	ConfigBox.AddItem("MMHVH");
	ConfigBox.AddItem("Legit1");
	ConfigBox.AddItem("Legit2");
	ConfigBox.SetSize(225, 550);
	ConfigBox.SetPosition(265, Client.bottom - 900);

	SaveButton.SetText("Save");
	SaveButton.SetCallback(SaveConfigCallbk);
	SaveButton.SetSize(225, 550);
	SaveButton.SetPosition(265, Client.bottom - 750);

	LoadButton.SetText("Load");
	LoadButton.SetCallback(LoadConfigCallbk);
	LoadButton.SetSize(225, 550);
	LoadButton.SetPosition(265, Client.bottom - 720);

	/*ConfigBox.SetFileId("cfg_box");
	ConfigBox.AddItem("Rage-MM");
	ConfigBox.AddItem("Rage-NS");
	ConfigBox.AddItem("Other");
	ConfigBox.SetSize(112, 350);
	ConfigBox.SetPosition(30, Client.bottom - 51);

	SaveButton.SetText("Save");
	SaveButton.SetCallback(SaveConfigCallbk);
	SaveButton.SetSize(112, 350);
	SaveButton.SetPosition(30, Client.bottom - 78);

	LoadButton.SetText("Load");
	LoadButton.SetCallback(LoadConfigCallbk);
	LoadButton.SetSize(112, 350);
	LoadButton.SetPosition(30, Client.bottom - 14);

	UnloadButton.SetText("Unload");
	UnloadButton.SetCallback(UnLoadCallbk);
	UnloadButton.SetSize(112, 389);
	UnloadButton.SetPosition(30, Client.bottom - 42);*/

	///*RageBotTab.RegisterControl(&SaveButton);
	//LegitBotTab.RegisterControl(&SaveButton);
	//VisualsTab.RegisterControl(&SaveButton);*/
	MiscTab.RegisterControl(&SaveButton);
	///*PlayersTab.RegisterControl(&SaveButton);
	//ColorsTab.RegisterControl(&SaveButton);
	//SkinchangerTab.RegisterControl(&SaveButton);*/

	////RageBotTab.RegisterControl(&LoadButton);
	////LegitBotTab.RegisterControl(&LoadButton);
	////VisualsTab.RegisterControl(&LoadButton);
	MiscTab.RegisterControl(&LoadButton);
	////PlayersTab.RegisterControl(&LoadButton);
	////ColorsTab.RegisterControl(&LoadButton);
	////SkinchangerTab.RegisterControl(&LoadButton);

	///*RageBotTab.RegisterControl(&UnloadButton);
	//LegitBotTab.RegisterControl(&UnloadButton);
	//VisualsTab.RegisterControl(&UnloadButton);*/
	//MiscTab.RegisterControl(&UnloadButton);
	///*PlayersTab.RegisterControl(&UnloadButton);
	//ColorsTab.RegisterControl(&UnloadButton);
	//SkinchangerTab.RegisterControl(&UnloadButton);*/

	///*RageBotTab.RegisterControl(&ConfigBox);
	//LegitBotTab.RegisterControl(&ConfigBox);
	//VisualsTab.RegisterControl(&ConfigBox);*/
	MiscTab.RegisterControl(&ConfigBox);
	////PlayersTab.RegisterControl(&ConfigBox);
	////ColorsTab.RegisterControl(&ConfigBox);
	////SkinchangerTab.RegisterControl(&ConfigBox);

	//RADAR
	Radar->SetPosition(Menu::Window.VisualsTab.OtherRadarXPosition.GetValue(), Menu::Window.VisualsTab.OtherRadarYPosition.GetValue());
	Radar->SetSize(250, 250);
	Radar->SetTitle("    Radar");

#pragma endregion Setting up the settings buttons
}

void CRageBotTab::Setup()
{
	SetTitle("RAGE");


#pragma region Aimbot

	/*AimbotGroup.SetPosition(12, 5);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(230, 480);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceCheckBox("Enable", this, &AimbotEnable);*/


	AimbotGroup.SetPosition(11, 20);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(377, 233);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl("Auto Fire", this, &AimbotAutoFire);

	AimbotAutoPistol.SetFileId("aim_autopistol");
	AimbotGroup.PlaceLabledControl("Auto Pistol", this, &AimbotAutoPistol);

	AimbotPredictVelocity.SetFileId("aim_prediction_velocity");
	AimbotGroup.PlaceLabledControl("Velocity Prediction", this, &AimbotPredictVelocity);

	Silentaim.SetFileId("aim_Silent");
	AimbotGroup.PlaceLabledControl("Silent Aim", this, &Silentaim);

	PSilentaim.SetFileId("aim_perfsilent");
	AimbotGroup.PlaceLabledControl("Perfect Silent", this, &PSilentaim);

	BackTrackingCreditsSuper.SetFileId("aim_backtrack_by_super");
	AimbotGroup.PlaceLabledControl("Backtrack", this, &BackTrackingCreditsSuper);
	


	
	HitNigger.SetPosition(270, 263);
	HitNigger.SetText(XorStr("Settings"));
	HitNigger.SetSize(252, 233);
	RegisterControl(&HitNigger);

	SyncHitChance.SetFileId(XorStr("w_enablesettings"));
	HitNigger.PlaceLabledControl(XorStr("Weapon Hitchance"), this, &SyncHitChance);


	WeaponPistolHitchance.SetFileId(XorStr("w_pistolhitchance"));
	WeaponPistolHitchance.SetBoundaries(0, 100);
	WeaponPistolHitchance.SetValue(0);
	WeaponPistolHitchance.SetFormat(FORMAT_PERC);
	HitNigger.PlaceLabledControl(XorStr("Pistol"), this, &WeaponPistolHitchance);

	WeaponSniperHitchance.SetFileId(XorStr("w_sniperhitchance"));
	WeaponSniperHitchance.SetBoundaries(0, 100);
	WeaponSniperHitchance.SetValue(0);
	WeaponSniperHitchance.SetFormat(FORMAT_PERC);
	HitNigger.PlaceLabledControl(XorStr("Sniper"), this, &WeaponSniperHitchance);

	WeaponRifleHitchance.SetFileId(XorStr("w_riflehitchance"));
	WeaponRifleHitchance.SetBoundaries(0, 100);
	WeaponRifleHitchance.SetValue(0);
	WeaponRifleHitchance.SetFormat(FORMAT_PERC);
	HitNigger.PlaceLabledControl(XorStr("Rifle"), this, &WeaponRifleHitchance);

	WeaponSMGHitchance.SetFileId(XorStr("w_smghitchance"));
	WeaponSMGHitchance.SetBoundaries(0, 100);
	WeaponSMGHitchance.SetValue(0);
	WeaponSMGHitchance.SetFormat(FORMAT_PERC);
	HitNigger.PlaceLabledControl(XorStr("SMG"), this, &WeaponSMGHitchance);

	WeaponOtherHitchance.SetFileId(XorStr("w_otherhitchance"));
	WeaponOtherHitchance.SetBoundaries(0, 100);
	WeaponOtherHitchance.SetValue(0);
	WeaponOtherHitchance.SetFormat(FORMAT_PERC);
	HitNigger.PlaceLabledControl(XorStr("Other"), this, &WeaponOtherHitchance);

	//=========================================

	//AimbotSilentAim.SetFileId("aim_silent");
	//AimbotSilentAim.AddItem("Disabled"); //0
	//AimbotSilentAim.AddItem("Silent"); // 1
	//AimbotSilentAim.AddItem("Silent Choked"); // 2
	//AimbotGroup.PlaceLabledControl("Silent Method", this, &AimbotSilentAim);



	


	//AccuracySmart.SetFileId("acc_smart");
	//AccuracySmart.SetBoundaries(0, 20);
	//AccuracySmart.SetValue(1);
	//AimbotGroup.PlaceLabledControl("Smart Aim", this, &AccuracySmart);


	//TargetPointscale.SetFileId("tgt_pointscale");
	//TargetPointscale.SetBoundaries(0.f, 1.f);
	//TargetPointscale.SetValue(0.f);
	//AimbotGroup.PlaceLabledControl("Pointscale", this, &TargetPointscale);
	


	/*AimbotPerfectSilentAim.SetFileId("aim_psilent");
	AimbotGroup.PlaceCheckBox("Perfect Silent", this, &AimbotPerfectSilentAim);*/



	//	AimbotAimStep.SetFileId("aim_aimstep");
	//	AimbotGroup.PlaceCheckBox("Aim Step", this, &AimbotAimStep);
	//
	/*AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceCheckBox("On Key Press", this, &AimbotKeyPress);*/

	/*AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceCheckBox("", this, &AimbotKeyBind);*/
	/*TargetFriendlyFire.SetFileId("tgt_friendlyfire");
	AimbotGroup.PlaceCheckBox("Friendly Damage", this, &TargetFriendlyFire);*/

	

#pragma endregion Targetting controls 

	TargetGroup.SetPosition(11, 263);
	TargetGroup.SetText("Target");
	TargetGroup.SetSize(257, 233);
	RegisterControl(&TargetGroup);

	TargetSelection.SetFileId("tgt_selection");
	TargetSelection.AddItem("Crosshair");
	TargetSelection.AddItem("Distance");
	TargetSelection.AddItem("Health");
	TargetGroup.PlaceLabledControl("Selection", this, &TargetSelection);

	AccuracyMinimumDamage.SetFileId("acc_mindmg");
	AccuracyMinimumDamage.SetBoundaries(1.f, 100.f);
	AccuracyMinimumDamage.SetValue(1.f);
	TargetGroup.PlaceLabledControl("Minimum Damage", this, &AccuracyMinimumDamage);

	TargetMultipoint.SetFileId("tgt_multipoint");
	AimbotGroup.PlaceLabledControl("Multipoint", this, &TargetMultipoint);

	MultipointIndexes.SetFileId("tgt_mp_indexes");
	MultipointIndexes.SetBoundaries(1, 25);
	MultipointIndexes.SetValue(1);
	AimbotGroup.PlaceLabledControl("Multipoint indexes", this, &MultipointIndexes); 

	//AccuracyHitchance.SetFileId("acc_chance");
	//AccuracyHitchance.SetBoundaries(0, 100);
	//AccuracyHitchance.SetValue(0);
	//TargetGroup.PlaceLabledControl("Minimum Hit Chance", this, &AccuracyHitchance);


	AimbotFov.SetFileId("aim_fov");
	AimbotFov.SetBoundaries(0.f, 180.f);
	AimbotFov.SetValue(180.f);
	TargetGroup.PlaceLabledControl("Maximum FOV", this, &AimbotFov);

	TargetHitbox.SetFileId("tgt_hitbox");
	TargetHitbox.AddItem("Head");
	TargetHitbox.AddItem("Neck");
	TargetHitbox.AddItem("Chest");
	TargetHitbox.AddItem("Stomach");
	TargetHitbox.AddItem("Shins");
	TargetGroup.PlaceLabledControl("Hitbox", this, &TargetHitbox);

	TargetHitscan.SetFileId("tgt_hitscan");
	TargetHitscan.AddItem("Disabled"); //0
	TargetHitscan.AddItem("Normal"); // 1
	TargetHitscan.AddItem("All Medium"); // 2
	TargetHitscan.AddItem("All High"); // 3
	TargetHitscan.AddItem("All Extreme"); // 4
	TargetGroup.PlaceLabledControl("Multi-Box", this, &TargetHitscan);


#pragma region Accuracy
	AccuracyGroup.SetPosition(400, 20);
	AccuracyGroup.SetText("Accuracy");
	AccuracyGroup.SetSize(377, 233);
	RegisterControl(&AccuracyGroup);

	AccuracyRecoil.SetFileId("acc_norecoil");
	AccuracyGroup.PlaceLabledControl("Anti Recoil", this, &AccuracyRecoil);

	AccuracyAutoCrouch.SetFileId("acc_crouch");
	AccuracyGroup.PlaceLabledControl("Automatic Crouch", this, &AccuracyAutoCrouch);

	//DisableInterpolation.SetFileId("acc_dis_interpj");
	//AccuracyGroup.PlaceLabledControl("Disable Interp", this, &DisableInterpolation);

	AutoRevolver.SetFileId("aim_autorev");
	AccuracyGroup.PlaceLabledControl("Automatic Revolver", this, &AutoRevolver);

	AccuracyAutoScope.SetFileId("acc_scope");
	AccuracyGroup.PlaceLabledControl("Automatic Scope", this, &AccuracyAutoScope);

	AccuracyAutoWall.SetFileId("acc_awall");
	AccuracyGroup.PlaceLabledControl("Auto Wall", this, &AccuracyAutoWall);
	
	AccuracyResolveYaw.SetFileId("acc_resolver_yaw");
	AccuracyResolveYaw.AddItem("Disabled");
	AccuracyResolveYaw.AddItem("Default");
	AccuracyResolveYaw.AddItem("Smart Force");
	AccuracyGroup.PlaceLabledControl("Yaw Corrections ", this, &AccuracyResolveYaw);

	AccuracyResolvePitch.SetFileId("acc_resolver_pitch");
	AccuracyResolvePitch.AddItem("Disabled");
	AccuracyResolvePitch.AddItem("Automatic");
	AccuracyGroup.PlaceLabledControl("Pitch Corrections", this, &AccuracyResolvePitch);

	PreferBodyAim.SetFileId("acc_body1");
	PreferBodyAim.AddItem("Disabled");
	PreferBodyAim.AddItem("Unresolved");
	PreferBodyAim.AddItem("Always");
	AccuracyGroup.PlaceLabledControl("Prefer BAIM ", this, &PreferBodyAim);

	/*AimbotAutoPistol.SetFileId("aim_autopistol");
	AccuracyGroup.PlaceCheckBox("Automatic Pistol", this, &AimbotAutoPistol);*/




#pragma endregion  Accuracy controls get Setup in here

#pragma region AntiAim
	AntiAimGroup.SetPosition(520, 263); //344
	AntiAimGroup.SetText("Anti-Aimbot");
	AntiAimGroup.SetSize(256, 233);
	RegisterControl(&AntiAimGroup);

	AntiAimTarget.SetFileId("aa_attarget");
	AntiAimGroup.PlaceLabledControl("To Enemy", this, &AntiAimTarget);

	AntiAimEnable.SetFileId("aa_enable");
	AntiAimEnable.AddItem("Disabled");
	AntiAimEnable.AddItem("Preset");
	AntiAimEnable.AddItem("Builder");
	AntiAimGroup.PlaceLabledControl("Enable", this, &AntiAimEnable);

	/*RegisterTab(&PitchTab);
	RegisterTab(&YawTab);

	PitchTab.Setup();
	YawTab.Setup();*/

	AntiAimPitch.SetFileId("aa_x");
	AntiAimPitch.AddItem("Disabled");
	AntiAimPitch.AddItem("Emotion");
	AntiAimPitch.AddItem("Down");
	AntiAimPitch.AddItem("Halfdown");
	AntiAimPitch.AddItem("Switch Shots");
	AntiAimPitch.AddItem("[NS]FakeDown");
	AntiAimPitch.AddItem("[NS]1080");
	//AntiAimPitch.AddItem("Custom Pitch");
	
	AntiAimGroup.PlaceLabledControl("Pitch", this, &AntiAimPitch);

	AntiAimYaw.SetFileId("aa_y");
	AntiAimYaw.AddItem("Disabled");
	AntiAimYaw.AddItem("Backward");
	AntiAimYaw.AddItem("Static");
	AntiAimYaw.AddItem("Half-Side");
	AntiAimYaw.AddItem("Jitter");
	AntiAimYaw.AddItem("Fake LBY");
	AntiAimYaw.AddItem("FJitter");
	AntiAimYaw.AddItem("Fake Spin");
	AntiAimYaw.AddItem("Fake Backward");
	AntiAimYaw.AddItem("Fake Forward");
	AntiAimYaw.AddItem("Fake Static");
	AntiAimYaw.AddItem("Fake-Genesis");
	AntiAimYaw.AddItem("LBYInverse");
	AntiAimYaw.AddItem("LBYSPin");
	AntiAimYaw.AddItem("LBY Breaker");
	AntiAimYaw.AddItem("LBYJitter");
	AntiAimYaw.AddItem("Fake Sync");
	AntiAimYaw.AddItem("Fake Sync 2");
	//AntiAimYaw.AddItem("Custom Yaw");
	//AntiAimYaw.AddItem("Test Yaw");
	AntiAimGroup.PlaceLabledControl("Yaw Preset", this, &AntiAimYaw);

	AAYawReal.SetFileId("aa_real_y");
	AAYawReal.AddItem("Disabled");
	AAYawReal.AddItem("Backward");
	AAYawReal.AddItem("Forward");
	AAYawReal.AddItem("Side Left");
	AAYawReal.AddItem("Side Right");
	AAYawReal.AddItem("Normal Spin");
	AAYawReal.AddItem("Switch Spin");
	AAYawReal.AddItem("Side Switch");
	AAYawReal.AddItem("Static Jitter");
	AntiAimGroup.PlaceLabledControl("Yaw [Real]", this, &AAYawReal);

	AAYawFake.SetFileId("aa_fake_y");
	AAYawFake.AddItem("Disabled");
	AAYawFake.AddItem("Backward");
	AAYawFake.AddItem("Forward");
	AAYawFake.AddItem("Side Left");
	AAYawFake.AddItem("Side Right");
	AAYawFake.AddItem("Normal Spin");
	AAYawFake.AddItem("Switch Spin");
	AAYawFake.AddItem("Side Switch");
	AAYawFake.AddItem("Static Jitter");
	AAYawFake.AddItem("Back");
	AAYawFake.AddItem("Lby INVERSE");
	AntiAimGroup.PlaceLabledControl("Yaw [Fake]", this, &AAYawFake);




	

	

	AntiAimKnife.SetFileId("aa_knife");
	AntiAimGroup.PlaceLabledControl("Anti Aim Knife", this, &AntiAimKnife);

	//AntiAimPitchSlider.SetFileId("aa_yaa");
	//AntiAimPitchSlider.SetBoundaries(0, 360);
	//AntiAimPitchSlider.SetValue(0);
	//AntiAimGroup.PlaceLabledControl("Custom Pitch", this, &AntiAimPitchSlider);

	/*RealAntiAimYawSlider.SetFileId("aa_yaa");
	RealAntiAimYawSlider.SetBoundaries(0, 360);
	RealAntiAimYawSlider.SetValue(0);
	AntiAimGroup.PlaceLabledControl("Custom Real Yaw", this, &RealAntiAimYawSlider);*/



#pragma endregion  AntiAim controls get setup in here
}

void CLegitBotTab::Setup()
{
	SetTitle("LEGIT");

	ActiveLabel.SetPosition(16, 16);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(66, 16);
	RegisterControl(&Active);

#pragma region Aimbot
	AimbotGroup.SetPosition(16, 48);
	AimbotGroup.SetText("Aimbot");
	AimbotGroup.SetSize(180, 210);
	RegisterControl(&AimbotGroup);

	AimbotEnable.SetFileId("aim_enable");
	AimbotGroup.PlaceLabledControl2("Enable", this, &AimbotEnable);

	AimbotAutoFire.SetFileId("aim_autofire");
	AimbotGroup.PlaceLabledControl2("Auto Fire", this, &AimbotAutoFire);

	AimbotFriendlyFire.SetFileId("aim_friendfire");
	AimbotGroup.PlaceLabledControl2("Friendly Fire", this, &AimbotFriendlyFire);

	AimbotKeyPress.SetFileId("aim_usekey");
	AimbotGroup.PlaceLabledControl2("On Key Press", this, &AimbotKeyPress);



	AimbotAutoPistol.SetFileId("aim_apistol");
	AimbotGroup.PlaceLabledControl2("Auto Pistol", this, &AimbotAutoPistol);

	AimbotSmokeCheck.SetFileId("otr_smokecheck");
	AimbotGroup.PlaceLabledControl2("Smoke Check", this, &AimbotSmokeCheck);

	AimbotKeyBind.SetFileId("aim_key");
	AimbotGroup.PlaceLabledControl2("Key Bind", this, &AimbotKeyBind);
#pragma endregion Aimbot shit

#pragma region Triggerbot
	TriggerGroup.SetPosition(570, 48);
	TriggerGroup.SetText("Triggerbot");
	TriggerGroup.SetSize(196, 432);
	RegisterControl(&TriggerGroup);

	TriggerEnable.SetFileId("trig_enable");
	TriggerGroup.PlaceLabledControl2("Enable", this, &TriggerEnable);

	TriggerKeyPress.SetFileId("trig_onkey");
	TriggerGroup.PlaceLabledControl2("On Key Press", this, &TriggerKeyPress);

	TriggerKeyBind.SetFileId("trig_key");
	TriggerGroup.PlaceLabledControl2("Key Bind", this, &TriggerKeyBind);

	TriggerRecoil.SetFileId("trig_recoil");
	TriggerGroup.PlaceLabledControl2("Recoil", this, &TriggerRecoil);

	TriggerHitChance.SetFileId("trig_hitchance");
	TriggerGroup.PlaceLabledControl2("Hitchance", this, &TriggerHitChance);

	TriggerHitChanceAmmount.SetFileId("trig_hitchanceamount");
	TriggerHitChanceAmmount.SetBoundaries(1, 100);
	TriggerHitChanceAmmount.SetValue(50);
	TriggerGroup.PlaceLabledControl2("Hitchance Amount", this, &TriggerHitChanceAmmount);

	TriggerDelay.SetFileId("trig_delay");
	TriggerDelay.SetBoundaries(0, 100);
	TriggerDelay.SetValue(1);
	TriggerGroup.PlaceLabledControl2("Delay", this, &TriggerDelay);

	TriggerBurst.SetFileId("trig_burst");
	TriggerBurst.SetBoundaries(0, 5);
	TriggerBurst.SetValue(1);
	TriggerGroup.PlaceLabledControl2("Burst", this, &TriggerBurst);

	TriggerBreak.SetFileId("trig_break");
	TriggerBreak.SetBoundaries(0, 100);
	TriggerBreak.SetValue(1);
	TriggerGroup.PlaceLabledControl2("Break", this, &TriggerBreak);

#pragma endregion Triggerbot stuff

#pragma region TriggerbotFilter
	TriggerFilterGroup.SetPosition(384, 270);
	TriggerFilterGroup.SetText("Triggerbot Filter");
	TriggerFilterGroup.SetSize(180, 210);
	RegisterControl(&TriggerFilterGroup);

	TriggerHead.SetFileId("trig_head");
	TriggerFilterGroup.PlaceLabledControl2("Head", this, &TriggerHead);

	TriggerChest.SetFileId("trig_chest");
	TriggerFilterGroup.PlaceLabledControl2("Chest", this, &TriggerChest);

	TriggerStomach.SetFileId("trig_stomach");
	TriggerFilterGroup.PlaceLabledControl2("Stomach", this, &TriggerStomach);

	TriggerArms.SetFileId("trig_arms");
	TriggerFilterGroup.PlaceLabledControl2("Arms", this, &TriggerArms);

	TriggerLegs.SetFileId("trig_legs");
	TriggerFilterGroup.PlaceLabledControl2("Legs", this, &TriggerLegs);

	TriggerTeammates.SetFileId("trig_teammates");
	TriggerFilterGroup.PlaceLabledControl2("Friendly Fire", this, &TriggerTeammates);
#pragma endregion TriggerbotFilter stuff

#pragma region Main Weapon
	WeaponMainGroup.SetPosition(16, 270);
	WeaponMainGroup.SetText("Rifles");
	WeaponMainGroup.SetSize(180, 210);
	RegisterControl(&WeaponMainGroup);

	WeaponMainHitbox.SetFileId("main_hitbox");
	WeaponMainHitbox.AddItem("Head");
	WeaponMainHitbox.AddItem("Neck");
	WeaponMainHitbox.AddItem("Chest");
	WeaponMainHitbox.AddItem("Stomach");
	WeaponMainHitbox.AddItem("Multihitbox");
	WeaponMainGroup.PlaceLabledControl2("Hitbox", this, &WeaponMainHitbox);

	WeaponMainSpeed.SetFileId("main_speed");
	WeaponMainSpeed.SetBoundaries(0.f, 100.f);
	WeaponMainSpeed.SetValue(1.f);
	WeaponMainGroup.PlaceLabledControl2("Max Speed", this, &WeaponMainSpeed);

	WeaponMainFoV.SetFileId("main_fov");
	WeaponMainFoV.SetBoundaries(0.f, 30.f);
	WeaponMainFoV.SetValue(3.5f);
	WeaponMainGroup.PlaceLabledControl2("FOV", this, &WeaponMainFoV);

	WeaponMainRecoilAmountX.SetFileId("main_recoilamount_x");
	WeaponMainRecoilAmountX.SetBoundaries(0.0f, 100.f);
	WeaponMainRecoilAmountX.SetValue(0.0f);
	WeaponMainGroup.PlaceLabledControl2(XorStr("RCS Pitch"), this, &WeaponMainRecoilAmountX);

	WeaponMainRecoilAmountY.SetFileId("main_recoilamount_y");
	WeaponMainRecoilAmountY.SetBoundaries(0.0f, 100.f);
	WeaponMainRecoilAmountY.SetValue(0.0f);
	WeaponMainGroup.PlaceLabledControl2(XorStr("RCS Yaw"), this, &WeaponMainRecoilAmountY);

	WeaponMainRecoil.SetFileId("main_recoil");
	WeaponMainGroup.PlaceLabledControl2(XorStr("Enable RCS"), this, &WeaponMainRecoil);

	WeaponMainAimtime.SetFileId("main_aimtime");
	WeaponMainAimtime.SetBoundaries(0, 3);
	WeaponMainAimtime.SetValue(0);
	WeaponMainGroup.PlaceLabledControl2("Aim Time", this, &WeaponMainAimtime);

	WeaoponMainStartAimtime.SetFileId("main_aimstart");
	WeaoponMainStartAimtime.SetBoundaries(0, 5);
	WeaoponMainStartAimtime.SetValue(0);
	WeaponMainGroup.PlaceLabledControl2("Start Aim Time", this, &WeaoponMainStartAimtime);
#pragma endregion

#pragma region Pistols
	WeaponPistGroup.SetPosition(200, 48);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(180, 210);
	RegisterControl(&WeaponPistGroup);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistHitbox.AddItem("Multihitbox");
	WeaponPistGroup.PlaceLabledControl2("Hitbox", this, &WeaponPistHitbox);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.f, 100.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponPistGroup.PlaceLabledControl2("Max Speed", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.f, 30.f);
	WeaponPistFoV.SetValue(3.f);
	WeaponPistGroup.PlaceLabledControl2("FOV", this, &WeaponPistFoV);

	WeaponPistRecoilAmountX.SetFileId("pist_recoilamount_x");
	WeaponPistRecoilAmountX.SetBoundaries(0.f, 100.f);
	WeaponPistRecoilAmountX.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl2(XorStr("RCS Pitch"), this, &WeaponPistRecoilAmountX);

	WeaponPistRecoilAmountY.SetFileId("pist_recoilamount_y");
	WeaponPistRecoilAmountY.SetBoundaries(0.f, 100.f);
	WeaponPistRecoilAmountY.SetValue(0.f);
	WeaponPistGroup.PlaceLabledControl2(XorStr("RCS Yaw"), this, &WeaponPistRecoilAmountY);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistGroup.PlaceLabledControl2(XorStr("Enable RCS"), this, &WeaponPistRecoil);

	WeaponPistAimtime.SetFileId("pist_aimtime");
	WeaponPistAimtime.SetBoundaries(0, 3);
	WeaponPistAimtime.SetValue(0);
	WeaponPistGroup.PlaceLabledControl2("Aim Time", this, &WeaponPistAimtime);

	WeaoponPistStartAimtime.SetFileId("pist_aimstart");
	WeaoponPistStartAimtime.SetBoundaries(0, 5);
	WeaoponPistStartAimtime.SetValue(0);
	WeaponPistGroup.PlaceLabledControl2("Start Aim Time", this, &WeaoponPistStartAimtime);
#pragma endregion

#pragma region Snipers
	WeaponSnipGroup.SetPosition(384, 48);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(180, 210);
	RegisterControl(&WeaponSnipGroup);

	WeaponSnipHitbox.SetFileId("snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipHitbox.AddItem("Multihitbox");
	WeaponSnipGroup.PlaceLabledControl2("Hitbox", this, &WeaponSnipHitbox);

	WeaponSnipSpeed.SetFileId("snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.f, 100.f);
	WeaponSnipSpeed.SetValue(1.5f);
	WeaponSnipGroup.PlaceLabledControl2("Max Speed", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("snip_fov");
	WeaponSnipFoV.SetBoundaries(0.f, 30.f);
	WeaponSnipFoV.SetValue(2.f);
	WeaponSnipGroup.PlaceLabledControl2("FOV", this, &WeaponSnipFoV);

	WeaponSnipRecoilAmountX.SetFileId("snip_recoilamount_x");
	WeaponSnipRecoilAmountX.SetBoundaries(0.f, 100.f);
	WeaponSnipRecoilAmountX.SetValue(0.f);
	WeaponSnipGroup.PlaceLabledControl2(XorStr("RCS Pitch"), this, &WeaponSnipRecoilAmountX);

	WeaponSnipRecoilAmountY.SetFileId("snip_recoilamount_y");
	WeaponSnipRecoilAmountY.SetBoundaries(0.f, 100.f);
	WeaponSnipRecoilAmountY.SetValue(0.f);
	WeaponSnipGroup.PlaceLabledControl2(XorStr("RCS Yaw"), this, &WeaponSnipRecoilAmountY);

	WeaponSnipRecoil.SetFileId("snip_recoil");
	WeaponSnipGroup.PlaceLabledControl2(XorStr("Enable RCS"), this, &WeaponSnipRecoil);

	WeaponSnipAimtime.SetFileId("snip_aimtime");
	WeaponSnipAimtime.SetBoundaries(0, 3);
	WeaponSnipAimtime.SetValue(0);
	WeaponSnipGroup.PlaceLabledControl2("Aim Time", this, &WeaponSnipAimtime);

	WeaoponSnipStartAimtime.SetFileId("pist_aimstart");
	WeaoponSnipStartAimtime.SetBoundaries(0, 5);
	WeaoponSnipStartAimtime.SetValue(0);
	WeaponSnipGroup.PlaceLabledControl2("Start Aim Time", this, &WeaoponSnipStartAimtime);
#pragma endregion

#pragma region MPs
	WeaponMpGroup.SetPosition(200, 270);
	WeaponMpGroup.SetText("MPs");
	WeaponMpGroup.SetSize(180, 210);
	RegisterControl(&WeaponMpGroup);

	WeaponMpHitbox.SetFileId("mps_hitbox");
	WeaponMpHitbox.AddItem("Head");
	WeaponMpHitbox.AddItem("Neck");
	WeaponMpHitbox.AddItem("Chest");
	WeaponMpHitbox.AddItem("Stomach");
	WeaponMpHitbox.AddItem("Multihitbox");
	WeaponMpGroup.PlaceLabledControl2("Hitbox", this, &WeaponMpHitbox);

	WeaponMpSpeed.SetFileId("mps_speed");
	WeaponMpSpeed.SetBoundaries(0.f, 100.f);
	WeaponMpSpeed.SetValue(1.0f);
	WeaponMpGroup.PlaceLabledControl2("Max Speed", this, &WeaponMpSpeed);

	WeaponMpFoV.SetFileId("mps_fov");
	WeaponMpFoV.SetBoundaries(0.f, 30.f);
	WeaponMpFoV.SetValue(4.f);
	WeaponMpGroup.PlaceLabledControl2("FOV", this, &WeaponMpFoV);

	WeaponMpRecoil.SetFileId("main_recoil");
	WeaponMpGroup.PlaceLabledControl2(XorStr("Enable RCS"), this, &WeaponMpRecoil);

	WeaponMpAimtime.SetFileId("mps_aimtime");
	WeaponMpAimtime.SetBoundaries(0, 3);
	WeaponMpAimtime.SetValue(0);
	WeaponMpGroup.PlaceLabledControl2("Aim Time", this, &WeaponMpAimtime);

	WeaoponMpStartAimtime.SetFileId("mps_aimstart");
	WeaoponMpStartAimtime.SetBoundaries(0, 5);
	WeaoponMpStartAimtime.SetValue(0);
	WeaponMpGroup.PlaceLabledControl2("Start Aim Time", this, &WeaoponMpStartAimtime);
#pragma endregion

#pragma region Shotguns
	/*WeaponShotgunGroup.SetPosition(272, 460);
	WeaponShotgunGroup.SetText("Shotguns");
	WeaponShotgunGroup.SetSize(240, 176);
	RegisterControl(&WeaponShotgunGroup);

	WeaponShotgunHitbox.SetFileId("shotgun_hitbox");
	WeaponShotgunHitbox.AddItem("Head");
	WeaponShotgunHitbox.AddItem("Neck");
	WeaponShotgunHitbox.AddItem("Chest");
	WeaponShotgunHitbox.AddItem("Stomach");
	WeaponShotgunHitbox.AddItem("Multihitbox");
	WeaponShotgunGroup.PlaceLabledControl2("Hitbox", this, &WeaponShotgunHitbox);

	WeaponShotgunSpeed.SetFileId("shotgun_speed");
	WeaponShotgunSpeed.SetBoundaries(0.f, 100.f);
	WeaponShotgunSpeed.SetValue(1.0f);
	WeaponShotgunGroup.PlaceLabledControl2("Max Speed", this, &WeaponShotgunSpeed);

	WeaponShotgunFoV.SetFileId("shotgun_fov");
	WeaponShotgunFoV.SetBoundaries(0.f, 30.f);
	WeaponShotgunFoV.SetValue(3.f);
	WeaponShotgunGroup.PlaceLabledControl2("FOV", this, &WeaponShotgunFoV);

	WeaponShotgunRecoil.SetFileId("shotgun_recoil");
	WeaponShotgunRecoil.SetBoundaries(0.f, 2.f);
	WeaponShotgunRecoil.SetValue(1.f);
	WeaponShotgunGroup.PlaceLabledControl2("Recoil", this, &WeaponShotgunRecoil);

	WeaponShotgunAimtime.SetFileId("shotgun_aimtime");
	WeaponShotgunAimtime.SetBoundaries(0, 3);
	WeaponShotgunAimtime.SetValue(0);
	WeaponShotgunGroup.PlaceLabledControl2("Aim Time", this, &WeaponShotgunAimtime);

	WeaoponShotgunStartAimtime.SetFileId("shotgun_aimstart");
	WeaoponShotgunStartAimtime.SetBoundaries(0, 5);
	WeaoponShotgunStartAimtime.SetValue(0);
	WeaponShotgunGroup.PlaceLabledControl2("Start Aim Time", this, &WeaoponShotgunStartAimtime);
#pragma endregion

#pragma region Machineguns
	WeaponMGGroup.SetPosition(528, 460);
	WeaponMGGroup.SetText("Machineguns");
	WeaponMGGroup.SetSize(240, 176);
	RegisterControl(&WeaponMGGroup);

	WeaponMGHitbox.SetFileId("mg_hitbox");
	WeaponMGHitbox.AddItem("Head");
	WeaponMGHitbox.AddItem("Neck");
	WeaponMGHitbox.AddItem("Chest");
	WeaponMGHitbox.AddItem("Stomach");
	WeaponMGHitbox.AddItem("Multihitbox");
	WeaponMGGroup.PlaceLabledControl2("Hitbox", this, &WeaponMGHitbox);

	WeaponMGSpeed.SetFileId("mg_speed");
	WeaponMGSpeed.SetBoundaries(0.f, 100.f);
	WeaponMGSpeed.SetValue(1.0f);
	WeaponMGGroup.PlaceLabledControl2("Max Speed", this, &WeaponMGSpeed);

	WeaponMGFoV.SetFileId("mg_fov");
	WeaponMGFoV.SetBoundaries(0.f, 30.f);
	WeaponMGFoV.SetValue(4.f);
	WeaponMGGroup.PlaceLabledControl2("FOV", this, &WeaponMGFoV);

	WeaponMGRecoil.SetFileId("mg_recoil");
	WeaponMGRecoil.SetBoundaries(0.f, 2.f);
	WeaponMGRecoil.SetValue(1.f);
	WeaponMGGroup.PlaceLabledControl2("Recoil", this, &WeaponMGRecoil);

	WeaponMGAimtime.SetFileId("mg_aimtime");
	WeaponMGAimtime.SetBoundaries(0, 3);
	WeaponMGAimtime.SetValue(0);
	WeaponMGGroup.PlaceLabledControl2("Aim Time", this, &WeaponMGAimtime);

	WeaoponMGStartAimtime.SetFileId("mg_aimstart");
	WeaoponMGStartAimtime.SetBoundaries(0, 5);
	WeaoponMGStartAimtime.SetValue(0);
	WeaponMGGroup.PlaceLabledControl2("Start Aim Time", this, &WeaoponMGStartAimtime);*/
#pragma endregion

#pragma region Pistols
	/*WeaponPistGroup.SetPosition(308, 20);
	WeaponPistGroup.SetText("Pistols");
	WeaponPistGroup.SetSize(247, 230);
	RegisterControl(&WeaponPistGroup);

	WeaponPistSpeed.SetFileId("pist_speed");
	WeaponPistSpeed.SetBoundaries(0.1f, 2.f);
	WeaponPistSpeed.SetValue(1.0f);
	WeaponPistGroup.PlaceLabledControl2("Max Speed", this, &WeaponPistSpeed);

	WeaponPistFoV.SetFileId("pist_fov");
	WeaponPistFoV.SetBoundaries(0.1f, 30.f);
	WeaponPistFoV.SetValue(5.f);
	WeaponPistGroup.PlaceLabledControl2("FoV", this, &WeaponPistFoV);

	WeaponPistRecoil.SetFileId("pist_recoil");
	WeaponPistGroup.PlaceCheckBox("Recoil", this, &WeaponPistRecoil);

	WeaponPistPSilent.SetFileId("pist_psilent");
	WeaponPistGroup.PlaceCheckBox("Perfect Silent", this, &WeaponPistPSilent);

	WeaponPistInacc.SetFileId("pist_inacc");
	WeaponPistInacc.SetBoundaries(0.f, 15.f);
	WeaponPistGroup.PlaceLabledControl2("Inaccuracy", this, &WeaponPistInacc);

	WeaponPistHitbox.SetFileId("pist_hitbox");
	WeaponPistHitbox.AddItem("Head");
	WeaponPistHitbox.AddItem("Neck");
	WeaponPistHitbox.AddItem("Chest");
	WeaponPistHitbox.AddItem("Stomach");
	WeaponPistGroup.PlaceLabledControl2("Hitbox", this, &WeaponPistHitbox);*/
#pragma endregion

#pragma region Snipers
	/*WeaponSnipGroup.SetPosition(308, 255);
	WeaponSnipGroup.SetText("Snipers");
	WeaponSnipGroup.SetSize(247, 160);
	RegisterControl(&WeaponSnipGroup);

	WeaponSnipSpeed.SetFileId("snip_speed");
	WeaponSnipSpeed.SetBoundaries(0.1f, 2.f);
	WeaponSnipSpeed.SetValue(1.0f);
	WeaponSnipGroup.PlaceLabledControl2("Max Speed", this, &WeaponSnipSpeed);

	WeaponSnipFoV.SetFileId("snip_fov");
	WeaponSnipFoV.SetBoundaries(0.1f, 30.f);
	WeaponSnipFoV.SetValue(5.f);
	WeaponSnipGroup.PlaceLabledControl2("FoV", this, &WeaponSnipFoV);

	WeaponSnipRecoil.SetFileId("snip_recoil");
	WeaponSnipGroup.PlaceCheckBox("Recoil", this, &WeaponSnipRecoil);

	WeaponSnipPSilent.SetFileId("snip_psilent");
	WeaponSnipGroup.PlaceCheckBox("Perfect Silent", this, &WeaponSnipPSilent);

	WeaponSnipInacc.SetFileId("snip_inacc");
	WeaponSnipInacc.SetBoundaries(0.f, 15.f);
	WeaponSnipGroup.PlaceLabledControl2("Inaccuracy", this, &WeaponSnipInacc);

	WeaponSnipHitbox.SetFileId("snip_hitbox");
	WeaponSnipHitbox.AddItem("Head");
	WeaponSnipHitbox.AddItem("Neck");
	WeaponSnipHitbox.AddItem("Chest");
	WeaponSnipHitbox.AddItem("Stomach");
	WeaponSnipGroup.PlaceLabledControl2("Hitbox", this, &WeaponSnipHitbox);*/
#pragma endregion
}

void CVisualTab::Setup()
{
	SetTitle("VISUALS");

	ActiveLabel.SetPosition(128, 3);
	ActiveLabel.SetText("Active");
	RegisterControl(&ActiveLabel);

	Active.SetFileId("active");
	Active.SetPosition(179, 3);
	RegisterControl(&Active);

#pragma region Options
	OptionsGroup.SetText("Player ESP");
	OptionsGroup.SetPosition(12, 20);
	OptionsGroup.SetSize(252, 485);
	RegisterControl(&OptionsGroup);



	OptionsFill.SetFileId("opt_box_fill");
	OptionsGroup.PlaceLabledControl("Fill", this, &OptionsFill);

	//OptionsOutline.SetFileId("opt_Ol");
	//OptionsOutline.AddItem("Off");
	//OptionsOutline.AddItem("On");
	//OptionsGroup.PlaceLabledControl("Box Outline", this, &OptionsOutline);

	OptionsName.SetFileId("opt_name");
	OptionsGroup.PlaceLabledControl("Name", this, &OptionsName);

	OptionsWeapon.SetFileId("opt_weapon");
	OptionsGroup.PlaceLabledControl("Weapon", this, &OptionsWeapon);



	OptionsAr.SetFileId("opt_ar");
	OptionsGroup.PlaceLabledControl("Armor", this, &OptionsAr);

	OptionsMoney.SetFileId("opt_cash");
	OptionsGroup.PlaceLabledControl("Money", this, &OptionsMoney);

	HasC4.SetFileId("opt_c4carrier");
	OptionsGroup.PlaceLabledControl("Has C4", this, &HasC4);

	IsDefusing.SetFileId("opt_defusing");
	OptionsGroup.PlaceLabledControl("Is Defusing", this, &IsDefusing);

	HasDefuser.SetFileId("opt_HasDef");
	OptionsGroup.PlaceLabledControl("Has Defuser", this, &HasDefuser);

	IsScoped.SetFileId("opt_scoped");
	OptionsGroup.PlaceLabledControl("Is Scoped", this, &IsScoped);

	/*OptionsInfo.SetFileId("opt_info");
	OptionsGroup.PlaceCheckBox("Info", this, &OptionsInfo);
	OptionsGlow.SetFileId("opt_glow");
	OptionsGroup.PlaceCheckBox("Glow", this, &OptionsGlow);
	*/
	OptionsGlow.SetFileId("opt_glow");
	OptionsGroup.PlaceLabledControl("Glow", this, &OptionsGlow);
	
	//OptionsChams.SetFileId("opt_glow");
	//OptionsGroup.PlaceLabledControl("Glow", this, &OptionsChams);

	

	OptionsSkeleton.SetFileId("opt_bone");
	OptionsGroup.PlaceLabledControl("Skeleton", this, &OptionsSkeleton);

	OptionsSnap.SetFileId("opt_snap");
	OptionsGroup.PlaceLabledControl("Snaplines", this, &OptionsSnap);

	//OptionsAimSpot.SetFileId("opt_aimspot");
	//OptionsGroup.PlaceCheckBox("Head Cross", this, &OptionsAimSpot);

	OptionsBox.SetFileId("opt_box");
	OptionsBox.AddItem("OFF");
	OptionsBox.AddItem("Normal");
	OptionsBox.AddItem("Bold");
	OptionsBox.AddItem("Corners");
	OptionsGroup.PlaceLabledControl2("Box-Style", this, &OptionsBox);

	OptionsChams.SetFileId("opt_chams");
	OptionsChams.AddItem("Off");
	OptionsChams.AddItem("Normal");
	OptionsChams.AddItem("Flat");
	OptionsGroup.PlaceLabledControl2("Player-Texture", this, &OptionsChams);

	OtherWeaponFX.SetFileId("otr_wpnfx");
	OtherWeaponFX.AddItem("Off");
	OtherWeaponFX.AddItem("Wireframe");
	OtherWeaponFX.AddItem("Chams");
	OtherWeaponFX.AddItem("Black");
	OtherWeaponFX.AddItem("Black Wireframe");
	//OtherWeaponFX.AddItem("Transparent");
	//OtherWeaponFX.AddItem("Rainbow Chams");
	//OtherWeaponFX.AddItem("Rainbow Wireframe");
	OptionsGroup.PlaceLabledControl2("Weapon-Texture", this, &OtherWeaponFX);
	
	OptionsHealth.SetFileId("opt_hp");
	OptionsHealth.AddItem("OFF");
	OptionsHealth.AddItem("Text");
	OptionsHealth.AddItem("Left Bar");
	OptionsHealth.AddItem("Right Bar");
	OptionsHealth.AddItem("Bottom");
	OptionsGroup.PlaceLabledControl2("Health-Style", this, &OptionsHealth);


#pragma endregion Setting up the Options controls

#pragma region Filters
	FiltersGroup.SetText("Filters");
	FiltersGroup.SetPosition(565, 20);
	FiltersGroup.SetSize(209, 219);
	RegisterControl(&FiltersGroup);

	/*FiltersAll.SetFileId("ftr_all");
	FiltersGroup.PlaceCheckBox("All", this, &FiltersAll);*/

	FiltersPlayers.SetFileId("ftr_players");
	FiltersGroup.PlaceLabledControl("Players", this, &FiltersPlayers);

	FiltersEnemiesOnly.SetFileId("ftr_enemyonly");
	FiltersGroup.PlaceLabledControl("Enemies Only", this, &FiltersEnemiesOnly);

	OptionsInfo.SetFileId("opt_info");
	FiltersGroup.PlaceLabledControl("Flags", this, &OptionsInfo);
	

	FiltersWeapons.SetFileId("ftr_weaps");
	FiltersGroup.PlaceLabledControl("Weapons", this, &FiltersWeapons);

	Grenades.SetFileId("opt_nade");
	FiltersGroup.PlaceLabledControl("Nade ESP", this, &Grenades);


	/*FiltersChickens.SetFileId("ftr_chickens");
	FiltersGroup.PlaceCheckBox("Chickens", this, &FiltersChickens);*/

	FiltersC4.SetFileId("ftr_c4");
	FiltersGroup.PlaceLabledControl("C4", this, &FiltersC4);
#pragma endregion Setting up the Filters controls

#pragma region World
	WorldGroup.SetText("Effects");
	WorldGroup.SetPosition(290, 20);
	WorldGroup.SetSize(252, 485);
	RegisterControl(&WorldGroup);

	

	OtherHitMarker.SetFileId("opt_hitmarker");
	WorldGroup.PlaceLabledControl("HitMarker", this, &OtherHitMarker);

	OtherNightMode.SetFileId("otr_mode");
	OtherNightMode.SetState(false);
	WorldGroup.PlaceLabledControl("Night-Mode", this, &OtherNightMode);

	OtherNoVisualRecoil.SetFileId("otr_visrecoil");
	WorldGroup.PlaceLabledControl("No Visual Recoil", this, &OtherNoVisualRecoil);

	OtherNoFlash.SetFileId("otr_noflash");
	WorldGroup.PlaceLabledControl("No Flash", this, &OtherNoFlash);

	OtherNoSmoke.SetFileId("otr_nosmoke");
	WorldGroup.PlaceLabledControl("No Smoke", this, &OtherNoSmoke);

	NoScope.SetFileId("otr_removescope");
	WorldGroup.PlaceLabledControl("Remove Scope", this, &NoScope);

	AmbientRed.SetFileId("otr_ambientred");
	AmbientRed.SetBoundaries(0.f, .5f);
	AmbientRed.SetValue(0.f);
	WorldGroup.PlaceLabledControl2("Ambient-Red", this, &AmbientRed);

	AmbientGreen.SetFileId("otr_ambientgreen");
	AmbientGreen.SetBoundaries(0.f, .5f);
	AmbientGreen.SetValue(0.f);
	WorldGroup.PlaceLabledControl2("Ambient-Green", this, &AmbientGreen);

	AmbientBlue.SetFileId("otr_ambientblue");
	AmbientBlue.SetBoundaries(0.f, .5f);
	AmbientBlue.SetValue(0.f);
	WorldGroup.PlaceLabledControl2("Ambient-Blue", this, &AmbientBlue);

	AmbientExposure.SetFileId("otr_ambientexposure");
	AmbientExposure.SetBoundaries(0.f, .5f);
	AmbientExposure.SetValue(.5f);
	WorldGroup.PlaceLabledControl2("AmbientExposure", this, &AmbientExposure);

	AmbientSkybox.SetFileId("otr_skyboxchanger");
	AmbientSkybox.AddItem("Disabled");
	AmbientSkybox.AddItem("Disable Skybox");
	AmbientSkybox.AddItem("Skybox-Night");
	AmbientSkybox.AddItem("Skybox-Night-B");
	AmbientSkybox.AddItem("Skybox-Jungle");
	WorldGroup.PlaceLabledControl2("Skybox", this, &AmbientSkybox);

	

	OtherNoHands.SetFileId("otr_hands");
	OtherNoHands.AddItem("Off");
	OtherNoHands.AddItem("None");
	OtherNoHands.AddItem("Transparent");
	OtherNoHands.AddItem("WireFrame");
	OtherNoHands.AddItem("Rainbow");
	WorldGroup.PlaceLabledControl2("Hand Texture", this, &OtherNoHands);

	OtherViewmodelFOV.SetFileId("otr_viewfov");
	OtherViewmodelFOV.SetBoundaries(0.f, 180.f);
	OtherViewmodelFOV.SetValue(0.f);
	WorldGroup.PlaceLabledControl2("Viewmodel FOV", this, &OtherViewmodelFOV);

	OtherFOV.SetFileId("otr_fov");
	OtherFOV.SetBoundaries(0.f, 180.f);
	OtherFOV.SetValue(90.f);
	WorldGroup.PlaceLabledControl2("Render FOV", this, &OtherFOV);

	Glow.SetText("Glow No Chams");
	Glow.SetPosition(565, 250);
	Glow.SetSize(209, 219);
	RegisterControl(&Glow);


	GlowR.SetFileId("GlowR");
	GlowR.SetBoundaries(0.f, 255.f);
	GlowR.SetValue(255.f);
	Glow.PlaceLabledControl2("Glow R", this, &GlowR);

	GlowG.SetFileId("GlowG");
	GlowG.SetBoundaries(0.f, 255.f);
	GlowG.SetValue(255.f);
	Glow.PlaceLabledControl2("Glow G", this, &GlowG);

	GlowB.SetFileId("GlowB");
	GlowB.SetBoundaries(0.f, 255.f);
	GlowB.SetValue(255.f);
	Glow.PlaceLabledControl2("Glow B", this, &GlowB);

	//GlowZ.SetFileId("GlowZ");
	//GlowZ.SetBoundaries(0.f, 255.f);
	//GlowZ.SetValue(235.f);
	//Glow.PlaceLabledControl2("Glow Opacity", this, &GlowZ);

#pragma endregion World End

#pragma region Other
	/*OtherGroup.SetText("Other ESP");
	OtherGroup.SetPosition(195, 20);
	OtherGroup.SetSize(190, 284);
	RegisterControl(&OtherGroup);*/

	//OtherCrosshair.SetFileId("otr_xhair");
	//OtherGroup.PlaceCheckBox("Crosshair", this, &OtherCrosshair);

	//OtherRecoilCrosshair.SetFileId("otr_recoilhair");
	//OtherRecoilCrosshair.AddItem("Off");
	//OtherRecoilCrosshair.AddItem("Recoil Position");
	//OtherRecoilCrosshair.AddItem("Radius");
	//OtherGroup.PlaceLabledControl("Recoil Crosshair", this, &OtherRecoilCrosshair);

	//OtherRadar.SetFileId("otr_radar");
	//OtherGroup.PlaceCheckBox("Radar", this, &OtherRadar);

	

	/*OtherRadar.SetFileId("otr_radar");
	OtherGroup.PlaceCheckBox("Radar", this, &OtherRadar);

	OtherRadarEnemyOnly.SetFileId("otr_radar_enemy_only");
	OtherGroup.PlaceCheckBox("Radar Enemy Only", this, &OtherRadarEnemyOnly);

	OtherRadarVisibleOnly.SetFileId("otr_radar_visible_only");
	OtherGroup.PlaceCheckBox("Radar Visible Only", this, &OtherRadarVisibleOnly);

	OtherRadarXPosition.SetFileId("otr_radar_position_x");
	OtherRadarYPosition.SetFileId("otr_radar_position_y");

	OtherRadarScale.SetFileId("otr_radar_scale");
	OtherRadarScale.SetBoundaries(0, 10000);
	OtherRadarScale.SetValue(10000);
	OtherGroup.PlaceCheckBox("Radar Range", this, &OtherRadarScale);*/
#pragma endregion Setting up the Other controls
}

void CMiscTab::Setup()
{
	SetTitle("MISC");

	//#pragma region Knife
	//	KnifeGroup.SetPosition(16, 16);
	//	KnifeGroup.SetSize(360, 126);
	//	KnifeGroup.SetText("Knife Changer");
	//	RegisterControl(&KnifeGroup);
	//
	//	KnifeEnable.SetFileId("knife_enable");
	//	KnifeGroup.PlaceCheckBox("Enable", this, &KnifeEnable);
	//
	//	KnifeModel.SetFileId("knife_model");
	//	KnifeModel.AddItem("Karam");
	//	KnifeModel.AddItem("Bayo");
	//	KnifeGroup.PlaceCheckBox("Knife", this, &KnifeModel);
	//
	//	KnifeSkin.SetFileId("knife_skin");
	//	KnifeSkin.AddItem("Doppler Sapphire");
	//	KnifeSkin.AddItem("Doppler Ruby");
	//	KnifeSkin.AddItem("Tiger");
	//	KnifeSkin.AddItem("Lore");
	//	KnifeGroup.PlaceCheckBox("Skin", this, &KnifeSkin);
	//
	//	KnifeApply.SetText("Apply Knife");
	//	KnifeApply.SetCallback(KnifeApplyCallbk);
	//	KnifeGroup.PlaceCheckBox("", this, &KnifeApply);
	//
	//#pragma endregion

#pragma region Other
	OtherGroup.SetText("Other");
	OtherGroup.SetPosition(12, 5);
	OtherGroup.SetSize(240, 480);
	RegisterControl(&OtherGroup);

	//SaveButton.SetText("Save");
	//SaveButton.SetCallback(SaveConfigCallbk);
	//SaveButton.SetSize(112, 350);
	//SaveButton.SetPosition(50, 320);
	//RegisterControl(&SaveButton);

	//LoadButton.SetText("Load");
	//LoadButton.SetCallback(LoadConfigCallbk);
	//LoadButton.SetSize(112, 350);
	//LoadButton.SetPosition(50, 360);
	//RegisterControl(&LoadButton);

	//UnloadButton.SetText("Unload");
	//UnloadButton.SetCallback(UnLoadCallbk);
	//UnloadButton.SetSize(112, 389);
	//UnloadButton.SetPosition(50, 400);
	//RegisterControl(&UnloadButton);

	/*ConfigBox.SetFileId("cfg_box");
	ConfigBox.AddItem("Rage-MM");
	ConfigBox.AddItem("Rage-NS");
	ConfigBox.AddItem("Other");
	ConfigBox.SetSize(112, 350);
	ConfigBox.SetPosition(50, 440);
	RegisterControl(&ConfigBox);*/

	//==========================================================

	//GloveApply.SetText("Apply Gloves");
	//GloveApply.SetCallback(GloveApplyCallbk);
	//GloveApply.SetPosition(300, 520);
	//GloveApply.SetSize(231, 110);
	//RegisterControl(&GloveApply);

	

	OtherEdgeJump.SetFileId("otr_edgejump");
	OtherGroup.PlaceLabledControl("Edge Jump", this, &OtherEdgeJump);

	OtherSafeMode.SetFileId("otr_safemode");
	OtherSafeMode.SetState(true);
	OtherGroup.PlaceLabledControl("Anti Untrust", this, &OtherSafeMode);

	/*OtherAirStuck.SetFileId("otr_astuck");
	OtherGroup.PlaceLabledControl("Air Stuck", this, &OtherAirStuck);*/

	OtherSpectators.SetFileId("otr_speclist");
	OtherGroup.PlaceLabledControl("Spectators List", this, &OtherSpectators);

	//OtherCompRank.SetFileId("otr_comprank");
	//OtherGroup.PlaceLabledControl("Rank Reveal", this, &OtherCompRank);

	OtherThirdperson.SetFileId("aa_thirdpsr");
	OtherGroup.PlaceLabledControl("Thirdperson", this, &OtherThirdperson);

	ThirdpersonAAType.SetFileId("otr_aatype");
	ThirdpersonAAType.AddItem(XorStr("See Fake")); //0
	ThirdpersonAAType.AddItem(XorStr("See Real")); // 1
	OtherGroup.PlaceLabledControl(XorStr("Thirdperson Angles"), this, &ThirdpersonAAType);

	OtherCircleKey.SetFileId("otr_circle");
	OtherGroup.PlaceLabledControl(XorStr("Circle Key"), this, &OtherCircleKey);

	/*NoInterp.SetFileId("otr_nointerp");
	OtherGroup.PlaceCheckBox("Server Crash Test", this, &NoInterp);*/

	//OtherBulletReg.SetFileId("Sv_bullet");
	//OtherGroup.PlaceLabledControl("Bullet Indicator HVH ONLY UNTRUSTED", this, &OtherBulletReg);

	/*OptionsHitMarker.SetFileId("opt_hitmarker");
	OtherGroup.PlaceCheckBox("HitMarker", this, &OptionsHitMarker);*/

	PostProcessing.SetFileId("fps_boost");
	OtherGroup.PlaceLabledControl("Disable Postprocessing", this, &PostProcessing);

	OtherAutoJump.SetFileId("otr_autojump");
	OtherAutoJump.AddItem("Off");
	OtherAutoJump.AddItem("Normal");
	OtherGroup.PlaceLabledControl2("Auto Jump", this, &OtherAutoJump);

	OtherAirGlide.SetFileId("otr_glide");
	OtherGroup.PlaceLabledControl2("b1G gL1de", this, &OtherAirGlide);

	OtherAutoStrafe.SetFileId("otr_strafe");
	OtherAutoStrafe.AddItem("Off");
	OtherAutoStrafe.AddItem("Legit");
	OtherAutoStrafe.AddItem("Rage");
	OtherGroup.PlaceLabledControl2("Auto Strafer", this, &OtherAutoStrafe);

	//OtherChatSpam.SetFileId("otr_spam");
	//OtherChatSpam.AddItem("Off");
	//OtherChatSpam.AddItem("Namestealer");
	//OtherChatSpam.AddItem("Regular");
	//OtherChatSpam.AddItem("FlexBeta");
	//OtherChatSpam.AddItem("Disperse Name");
	//OtherGroup.PlaceLabledControl("Chat Spam", this, &OtherChatSpam);

	OtherClantag.SetFileId("otr_spam");
	OtherClantag.AddItem("Off");
	OtherClantag.AddItem("FlexBeta.CLUB");
	OtherGroup.PlaceLabledControl("Custom Clantag", this, &OtherClantag);

	//OtherTeamChat.SetFileId("otr_teamchat");
	//OtherGroup.PlaceCheckBox("Team Chat Only", this, &OtherTeamChat);

	//OtherChatDelay.SetFileId("otr_chatdelay");
	//OtherChatDelay.SetBoundaries(0.1, 3.0);
	//OtherChatDelay.SetValue(0.5);
	//OtherGroup.PlaceLabledControl("Spam Delay", this, &OtherChatDelay);

	

#pragma endregion other random options

#pragma region FakeLag
	FakeLagGroup.SetPosition(260, 390);
	FakeLagGroup.SetSize(266, 120);
	FakeLagGroup.SetText("Fake Lag");
	RegisterControl(&FakeLagGroup);

	FakeLagEnable.SetFileId("fakelag_enable");
	FakeLagGroup.PlaceLabledControl("Enable", this, &FakeLagEnable);

	FakeLagChoke.SetFileId("fakelag_choke");
	FakeLagChoke.SetBoundaries(0, 16);
	FakeLagChoke.SetValue(0);
	FakeLagGroup.PlaceLabledControl("Choke-Factor", this, &FakeLagChoke);

	FakeLagWhileShooting.SetFileId("fakelag_shooting");
	FakeLagGroup.PlaceLabledControl("Disable On Fire", this, &FakeLagWhileShooting);
	
#pragma endregion fakelag shit

	Settings.SetText("Settings");
	Settings.SetPosition(260, 20);
	Settings.SetSize(330, 360);
	RegisterControl(&Settings);


	//ConfigGroup.SetPosition(340, 259); //344
	//ConfigGroup.SetText("Presets");
	//ConfigGroup.SetSize(268, 263);
	//RegisterControl(&ConfigGroup);

	//#pragma region Teleport
	//	TeleportGroup.SetPosition(16, 316);
	//	TeleportGroup.SetSize(360, 75);
	//	TeleportGroup.SetText("Teleport");
	//	RegisterControl(&TeleportGroup);
	//
	//	TeleportEnable.SetFileId("teleport_enable");
	//	TeleportGroup.PlaceCheckBox("Enable", this, &TeleportEnable);
	//
	//	TeleportKey.SetFileId("teleport_key");
	//	TeleportGroup.PlaceCheckBox("Key", this, &TeleportKey);
	//
	//#pragma endregion

	/*#pragma region OverideFov
	FOVGroup.SetPosition(16, 365);
	FOVGroup.SetSize(360, 75);
	FOVGroup.SetText("FOV Changer");
	RegisterControl(&FOVGroup);

	FOVEnable.SetFileId("fov_enable");
	FOVGroup.PlaceCheckBox("Enable", this, &FOVEnable);

	FOVSlider.SetFileId("fov_slider");
	FOVSlider.SetBoundaries(0, 200);
	FOVSlider.SetValue(0);
	FOVGroup.PlaceCheckBox("FOV Amount", this, &FOVSlider);

	#pragma endregion*/
}

/*void CPlayersTab::Setup()
{
SetTitle("PlayerList");

#pragma region PList

pListGroup.SetPosition(16, 16);
pListGroup.SetSize(680, 200);
pListGroup.SetText("Player List");
pListGroup.SetColumns(2);
RegisterControl(&pListGroup);

pListPlayers.SetPosition(26, 46);
pListPlayers.SetSize(640, 50);
pListPlayers.SetHeightInItems(20);
RegisterControl(&pListPlayers);

#pragma endregion

#pragma region Options

OptionsGroup.SetPosition(16, 257);
OptionsGroup.SetSize(450, 120);
OptionsGroup.SetText("Player Options");
RegisterControl(&OptionsGroup);

OptionsFriendly.SetFileId("pl_friendly");
OptionsGroup.PlaceCheckBox("Friendly", this, &OptionsFriendly);

OptionsAimPrio.SetFileId("pl_priority");
OptionsGroup.PlaceCheckBox("Priority", this, &OptionsAimPrio);

OptionsCalloutSpam.SetFileId("pl_callout");
OptionsGroup.PlaceCheckBox("Callout Spam", this, &OptionsCalloutSpam);

#pragma endregion
}

DWORD GetPlayerListIndex(int EntId)
{
player_info_t pinfo;
Interfaces::Engine->GetPlayerInfo(EntId, &pinfo);

// Bot
if (pinfo.guid[0] == 'B' && pinfo.guid[1] == 'O')
{
char buf[64]; sprintf_s(buf, "BOT_420%sAY", pinfo.name);
return CRC32(buf, 64);
}
else
{
return CRC32(pinfo.guid, 32);
}
}

bool IsFriendly(int EntId)
{
DWORD plistId = GetPlayerListIndex(EntId);
if (PlayerList.find(plistId) != PlayerList.end())
{
return PlayerList[plistId].Friendly;
}

return false;
}

bool IsAimPrio(int EntId)
{
DWORD plistId = GetPlayerListIndex(EntId);
if (PlayerList.find(plistId) != PlayerList.end())
{
return PlayerList[plistId].AimPrio;
}

return false;
}

bool IsCalloutTarget(int EntId)
{
DWORD plistId = GetPlayerListIndex(EntId);
if (PlayerList.find(plistId) != PlayerList.end())
{
return PlayerList[plistId].Callout;
}

return false;
}

void UpdatePlayerList()
{
IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
{
Menu::Window.Playerlist.pListPlayers.ClearItems();

// Loop through all active entitys
for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
{
// Get the entity

player_info_t pinfo;
if (i != Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerInfo(i, &pinfo))
{
IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(i);
int HP = 100; char* Location = "Unknown";
char *Friendly = " ", *AimPrio = " ";

DWORD plistId = GetPlayerListIndex(Menu::Window.Playerlist.pListPlayers.GetValue());
if (PlayerList.find(plistId) != PlayerList.end())
{
Friendly = PlayerList[plistId].Friendly ? "Friendly" : "";
AimPrio = PlayerList[plistId].AimPrio ? "AimPrio" : "";
}

if (pEntity && !pEntity->IsDormant())
{
HP = pEntity->GetHealth();
Location = pEntity->GetLastPlaceName();
}

char nameBuffer[512];
sprintf_s(nameBuffer, "%-24s %-10s %-10s [%d HP] [Last Seen At %s]", pinfo.name, IsFriendly(i) ? "Friend" : " ", IsAimPrio(i) ? "AimPrio" : " ", HP, Location);
Menu::Window.Playerlist.pListPlayers.AddItem(nameBuffer, i);

}

}

DWORD meme = GetPlayerListIndex(Menu::Window.Playerlist.pListPlayers.GetValue());

// Have we switched to a different player?
static int PrevSelectedPlayer = 0;
if (PrevSelectedPlayer != Menu::Window.Playerlist.pListPlayers.GetValue())
{
if (PlayerList.find(meme) != PlayerList.end())
{
Menu::Window.Playerlist.OptionsFriendly.SetState(PlayerList[meme].Friendly);
Menu::Window.Playerlist.OptionsAimPrio.SetState(PlayerList[meme].AimPrio);
Menu::Window.Playerlist.OptionsCalloutSpam.SetState(PlayerList[meme].Callout);

}
else
{
Menu::Window.Playerlist.OptionsFriendly.SetState(false);
Menu::Window.Playerlist.OptionsAimPrio.SetState(false);
Menu::Window.Playerlist.OptionsCalloutSpam.SetState(false);

}
}
PrevSelectedPlayer = Menu::Window.Playerlist.pListPlayers.GetValue();

PlayerList[meme].Friendly = Menu::Window.Playerlist.OptionsFriendly.GetState();
PlayerList[meme].AimPrio = Menu::Window.Playerlist.OptionsAimPrio.GetState();
PlayerList[meme].Callout = Menu::Window.Playerlist.OptionsCalloutSpam.GetState();
}
}*/

void CColorsTab::Setup()
{
	SetTitle("COLORS");
#pragma region Visual Colors
	ColorsGroup.SetPosition(12, 20);
	ColorsGroup.SetText("Counter Terrorist Colors");
	ColorsGroup.SetSize(250, 243);
	RegisterControl(&ColorsGroup);

	CTColorVisR.SetFileId("ct_color_vis_r");
	CTColorVisR.SetBoundaries(0.f, 255.f);
	CTColorVisR.SetValue(120.f);
	ColorsGroup.PlaceLabledControl("CT Visible R", this, &CTColorVisR);

	CTColorVisG.SetFileId("ct_color_vis_g");
	CTColorVisG.SetBoundaries(0.f, 255.f);
	CTColorVisG.SetValue(210.f);
	ColorsGroup.PlaceLabledControl("CT Visible G", this, &CTColorVisG);

	CTColorVisB.SetFileId("ct_color_vis_b");
	CTColorVisB.SetBoundaries(0.f, 255.f);
	CTColorVisB.SetValue(26.f);
	ColorsGroup.PlaceLabledControl("CT Visible B", this, &CTColorVisB);

	CTColorNoVisR.SetFileId("ct_color_no_vis_r");
	CTColorNoVisR.SetBoundaries(0.f, 255.f);
	CTColorNoVisR.SetValue(15.f);
	ColorsGroup.PlaceLabledControl("CT Not Visible/ Glow R", this, &CTColorNoVisR);

	CTColorNoVisG.SetFileId("ct_color_no_vis_g");
	CTColorNoVisG.SetBoundaries(0.f, 255.f);
	CTColorNoVisG.SetValue(110.f);
	ColorsGroup.PlaceLabledControl("CT Not Visible/ Glow G", this, &CTColorNoVisG);

	CTColorNoVisB.SetFileId("ct_color_no_vis_b");
	CTColorNoVisB.SetBoundaries(0.f, 255.f);
	CTColorNoVisB.SetValue(220.f);
	ColorsGroup.PlaceLabledControl("CT Not Visible/ Glow B", this, &CTColorNoVisB);
	//----------------------------------------------------------------------------------------------------------------------------------------------------
	ColorsTGroup.SetPosition(280, 20);
	ColorsTGroup.SetText("Terrorist Colors");
	ColorsTGroup.SetSize(250, 243);
	RegisterControl(&ColorsTGroup);

	TColorVisR.SetFileId("t_color_vis_r");
	TColorVisR.SetBoundaries(0.f, 255.f);
	TColorVisR.SetValue(235.f);
	ColorsTGroup.PlaceLabledControl("T Visible R", this, &TColorVisR);

	TColorVisG.SetFileId("t_color_vis_g");
	TColorVisG.SetBoundaries(0.f, 255.f);
	TColorVisG.SetValue(200.f);
	ColorsTGroup.PlaceLabledControl("T Visible G", this, &TColorVisG);

	TColorVisB.SetFileId("t_color_vis_b");
	TColorVisB.SetBoundaries(0.f, 255.f);
	TColorVisB.SetValue(0.f);
	ColorsTGroup.PlaceLabledControl("T Visible B", this, &TColorVisB);


	TColorNoVisR.SetFileId("t_color_no_vis_r");
	TColorNoVisR.SetBoundaries(0.f, 255.f);
	TColorNoVisR.SetValue(235.f);
	ColorsTGroup.PlaceLabledControl("T Not Visible/ Glow R", this, &TColorNoVisR);

	TColorNoVisG.SetFileId("t_color_no_vis_g");
	TColorNoVisG.SetBoundaries(0.f, 255.f);
	TColorNoVisG.SetValue(50.f);
	ColorsTGroup.PlaceLabledControl("T Not Visible/ Glow G", this, &TColorNoVisG);

	TColorNoVisB.SetFileId("t_color_no_vis_b");
	TColorNoVisB.SetBoundaries(0.f, 255.f);
	TColorNoVisB.SetValue(0.f);
	ColorsTGroup.PlaceLabledControl("T Not Visible/ Glow B", this, &TColorNoVisB);

	//GlowZ.SetFileId("glow_color_z");
	//GlowZ.SetBoundaries(0.f, 255.f);
	//GlowZ.SetValue(0.f);
	//ColorsTGroup.PlaceLabledControl("Glow Z", this, &GlowZ);

	

	//Glow.SetPosition(12, 259);
	//Glow.SetText("Glow Colors");
	//Glow.SetSize(291, 263);
	//RegisterControl(&Glow);

	////GlowR.SetFileId("glow_color_r");
	////GlowR.SetBoundaries(0.f, 255.f);
	////GlowR.SetValue(235.f);
	////Glow.PlaceLabledControl("Glow R", this, &GlowR);

	////GlowG.SetFileId("glow_color_g");
	////GlowG.SetBoundaries(0.f, 255.f);
	////GlowG.SetValue(200.f);
	////Glow.PlaceLabledControl("Glow G", this, &GlowG);

	////GlowB.SetFileId("glow_color_b");
	////GlowB.SetBoundaries(0.f, 255.f);
	////GlowB.SetValue(0.f);
	////Glow.PlaceLabledControl("Glow B", this, &GlowB);

	////GlowZ.SetFileId("glow_color_z");
	////GlowZ.SetBoundaries(0.f, 255.f);
	////GlowZ.SetValue(0.f);
	////Glow.PlaceLabledControl("Glow Z", this, &GlowZ);

	//CTGlowR.SetFileId("glow_color_r");
	//CTGlowR.SetBoundaries(0.f, 255.f);
	//CTGlowR.SetValue(235.f);
	//Glow.PlaceLabledControl("Glow R", this, &CTGlowR);

	//CTGlowG.SetFileId("glow_color_g");
	//CTGlowG.SetBoundaries(0.f, 255.f);
	//CTGlowG.SetValue(200.f);
	//Glow.PlaceLabledControl("Glow G", this, &CTGlowG);

	//CTGlowB.SetFileId("glow_color_b");
	//CTGlowB.SetBoundaries(0.f, 255.f);
	//CTGlowB.SetValue(0.f);
	//Glow.PlaceLabledControl("Glow B", this, &CTGlowB);

	//TGlowR.SetFileId("glow_color_r");
	//TGlowR.SetBoundaries(0.f, 255.f);
	//TGlowR.SetValue(235.f);
	//Glow.PlaceLabledControl("Glow R", this, &TGlowR);

	//TGlowG.SetFileId("glow_color_g");
	//TGlowG.SetBoundaries(0.f, 255.f);
	//TGlowG.SetValue(200.f);
	//Glow.PlaceLabledControl("Glow G", this, &TGlowG);

	//TGlowB.SetFileId("glow_color_b");
	//TGlowB.SetBoundaries(0.f, 255.f);
	//TGlowB.SetValue(0.f);
	//Glow.PlaceLabledControl("Glow B", this, &TGlowB);

	Arms.SetPosition(12, 279);
	Arms.SetText("Wireframe Colors");
	Arms.SetSize(250, 230);
	RegisterControl(&Arms);

	ArmsR.SetFileId("ct_Arms_R");
	ArmsR.SetBoundaries(0.f, 255.f);
	ArmsR.SetValue(120.f);
	Arms.PlaceLabledControl("Arms R", this, &ArmsR);

	ArmsG.SetFileId("ct_Arms_G");
	ArmsG.SetBoundaries(0.f, 255.f);
	ArmsG.SetValue(210.f);
	Arms.PlaceLabledControl("Arms G", this, &ArmsG);

	ArmsB.SetFileId("ct_Arms_B");
	ArmsB.SetBoundaries(0.f, 255.f);
	ArmsB.SetValue(26.f);
	Arms.PlaceLabledControl("Arms B", this, &ArmsB);

	GunsR.SetFileId("gun_R");
	GunsR.SetBoundaries(0.f, 255.f);
	GunsR.SetValue(120.f);
	Arms.PlaceLabledControl("Gun R", this, &GunsR);

	GunsG.SetFileId("gun_G");
	GunsG.SetBoundaries(0.f, 255.f);
	GunsG.SetValue(210.f);
	Arms.PlaceLabledControl("Gun G", this, &GunsG);

	GunsB.SetFileId("gun_B");
	GunsB.SetBoundaries(0.f, 255.f);
	GunsB.SetValue(26.f);
	Arms.PlaceLabledControl("Gun B", this, &GunsB);

#pragma endregion Visual Colors
}

void CMenucolors::Setup()
{
	SetTitle("MENU");
#pragma region Visual Colors
	
//===============================Menu Clear=================================================//

	//ColorsClearsGroup.SetPosition(280, 279);
	//ColorsClearsGroup.SetText("Menu Colors");
	//ColorsClearsGroup.SetSize(250, 230);
	//RegisterControl(&ColorsClearsGroup);

	//MenuClearR.SetFileId("menu_color_r");
	//MenuClearR.SetBoundaries(0.f, 255.f);
	//MenuClearR.SetValue(27.f);
	//ColorsClearsGroup.PlaceLabledControl("Menu R", this, &MenuClearR);

	//MenuClearG.SetFileId("menu_color_g");
	//MenuClearG.SetBoundaries(0.f, 255.f);
	//MenuClearG.SetValue(27.f);
	//ColorsClearsGroup.PlaceLabledControl("Menu G", this, &MenuClearG);

	//MenuClearB.SetFileId("menu_color_b");
	//MenuClearB.SetBoundaries(0.f, 255.f);
	//MenuClearB.SetValue(27.f);
	//ColorsClearsGroup.PlaceLabledControl("Menu B", this, &MenuClearB);

//=================================Menu Groupbox ============================================//

	ColorsBoxGroup.SetPosition(12, 20);
	ColorsBoxGroup.SetText("GroupBox Colors");
	ColorsBoxGroup.SetSize(250, 240);
	RegisterControl(&ColorsBoxGroup);

	//GroupboxR.SetFileId("G_color_r");
	//GroupboxR.SetBoundaries(0.f, 255.f);
	//GroupboxR.SetValue(0.f);
	//ColorsBoxGroup.PlaceLabledControl("GBox R", this, &GroupboxR);

	//GroupboxG.SetFileId("G_color_g");
	//GroupboxG.SetBoundaries(0.f, 255.f);
	//GroupboxG.SetValue(0.f);
	//ColorsBoxGroup.PlaceLabledControl("GBox G", this, &GroupboxG);

	//GroupboxB.SetFileId("G_color_b");
	//GroupboxB.SetBoundaries(0.f, 255.f);
	//GroupboxB.SetValue(0.f);
	//ColorsBoxGroup.PlaceLabledControl("GBox B", this, &GroupboxB);

//=====================================text=================================//

	GrouptextR.SetFileId("text_color_r");
	GrouptextR.SetBoundaries(0.f, 255.f);
	GrouptextR.SetValue(0.f);
	ColorsBoxGroup.PlaceLabledControl("Text R", this, &GrouptextR);

	GrouptextG.SetFileId("text_color_g");
	GrouptextG.SetBoundaries(0.f, 255.f);
	GrouptextG.SetValue(0.f);
	ColorsBoxGroup.PlaceLabledControl("Text G", this, &GrouptextG);

	GrouptextB.SetFileId("text_color_b");
	GrouptextB.SetBoundaries(0.f, 255.f);
	GrouptextB.SetValue(255.f);
	ColorsBoxGroup.PlaceLabledControl("Text B", this, &GrouptextB);

//======================================= Control Colors===========================================//

	ColorsControlsGroup.SetPosition(12, 279);
	ColorsControlsGroup.SetText("Control Colors");
	ColorsControlsGroup.SetSize(250, 230);
	RegisterControl(&ColorsControlsGroup);

	MenuR.SetFileId("C_color_r");
	MenuR.SetBoundaries(0.f, 255.f);
	MenuR.SetValue(0.f);
	ColorsControlsGroup.PlaceLabledControl("Con R", this, &MenuR);

	MenuG.SetFileId("C_color_g");
	MenuG.SetBoundaries(0.f, 255.f);
	MenuG.SetValue(0.f);
	ColorsControlsGroup.PlaceLabledControl("Con G", this, &MenuG);

	MenuB.SetFileId("C_color_b");
	MenuB.SetBoundaries(0.f, 255.f);
	MenuB.SetValue(255.f);
	ColorsControlsGroup.PlaceLabledControl("Con B", this, &MenuB);

//==========================================Tabarea============================================//

	/*ColorsTabGroup.SetPosition(12, 20);
	ColorsTabGroup.SetText("GroupBox Colors");
	ColorsTabGroup.SetSize(250, 230);
	RegisterControl(&ColorsTabGroup);

	TabareaR.SetFileId("G_color_r");
	TabareaR.SetBoundaries(0.f, 255.f);
	TabareaR.SetValue(15.f);
	ColorsTabGroup.PlaceLabledControl("T Area R", this, &TabareaR);

	TabareaG.SetFileId("G_color_g");
	TabareaG.SetBoundaries(0.f, 255.f);
	TabareaG.SetValue(15.f);
	ColorsTabGroup.PlaceLabledControl("T Area G", this, &TabareaG);

	TabareaB.SetFileId("G_color_b");
	TabareaB.SetBoundaries(0.f, 255.f);
	TabareaB.SetValue(15.f);
	ColorsTabGroup.PlaceLabledControl("T Area B", this, &TabareaB);*/
	

#pragma endregion Visual Colors
}

void CSkinchangerTab::Setup()
{
	SetTitle("SKINS");

	SkinActive.SetPosition(128, 3);
	SkinActive.SetText("Active");
	RegisterControl(&SkinActive);

	SkinEnable.SetFileId("active");
	SkinEnable.SetPosition(179, 3);
	RegisterControl(&SkinEnable);

	/*SkinApply.SetText("Apply Skins");
	SkinApply.SetCallback(KnifeApplyCallbk);
	SkinApply.SetPosition(300, 400);
	SkinApply.SetSize(231, 110);
	RegisterControl(&SkinApply);*/

	GloveApply.SetText("Apply Gloves");
	GloveApply.SetCallback(GloveApplyCallbk);
	GloveApply.SetPosition(400, 460);
	GloveApply.SetSize(231, 110);
	RegisterControl(&GloveApply);

#pragma region Knife
	KnifeGroup.SetPosition(12, 24);
	KnifeGroup.SetText("Knife");
	KnifeGroup.SetSize(331, 80);
	RegisterControl(&KnifeGroup);

	KnifeModel.SetFileId("knife_model");
	KnifeModel.AddItem("Bayonet");
	KnifeModel.AddItem("Bowie Knife");
	KnifeModel.AddItem("Butterfly Knife");
	KnifeModel.AddItem("Falchion Knife");
	KnifeModel.AddItem("Flip Knife");
	KnifeModel.AddItem("Gut Knife");
	KnifeModel.AddItem("Huntsman Knife");
	KnifeModel.AddItem("Karambit");
	KnifeModel.AddItem("M9 Bayonet");
	KnifeModel.AddItem("Shadow Daggers");
	KnifeGroup.PlaceLabledControl2("Knife", this, &KnifeModel);

	KnifeSkin.SetFileId("knife_skin");
	KnifeSkin.AddItem("None");
	KnifeSkin.AddItem("Crimson Web");
	KnifeSkin.AddItem("Bone Mask");
	KnifeSkin.AddItem("Fade");
	KnifeSkin.AddItem("Night");
	KnifeSkin.AddItem("Blue Steel");
	KnifeSkin.AddItem("Stained");
	KnifeSkin.AddItem("Case Hardened");
	KnifeSkin.AddItem("Slaughter");
	KnifeSkin.AddItem("Safari Mesh");
	KnifeSkin.AddItem("Boreal Forest");
	KnifeSkin.AddItem("Ultraviolet");
	KnifeSkin.AddItem("Urban Masked");
	KnifeSkin.AddItem("Scorched");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Tiger Tooth");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Damascus Steel");
	KnifeSkin.AddItem("Marble Fade");
	KnifeSkin.AddItem("Rust Coat");
	KnifeSkin.AddItem("Doppler Ruby");
	KnifeSkin.AddItem("Doppler Sapphire");
	KnifeSkin.AddItem("Doppler Blackpearl");
	KnifeSkin.AddItem("Doppler Phase 1");
	KnifeSkin.AddItem("Doppler Phase 2");
	KnifeSkin.AddItem("Doppler Phase 3");
	KnifeSkin.AddItem("Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Phase 1");
	KnifeSkin.AddItem("Gamma Doppler Phase 2");
	KnifeSkin.AddItem("Gamma Doppler Phase 3");
	KnifeSkin.AddItem("Gamma Doppler Phase 4");
	KnifeSkin.AddItem("Gamma Doppler Emerald");
	KnifeSkin.AddItem("Lore");
	KnifeGroup.PlaceLabledControl("Skin", this, &KnifeSkin);
#pragma endregion

	GloveGroup.SetPosition(371, 24);
	GloveGroup.SetText("Gloves");
	GloveGroup.SetSize(331, 80);
	RegisterControl(&GloveGroup);

	GloveModel.SetFileId("glove_model");
	GloveModel.AddItem("OFF");
	GloveModel.AddItem("BloodHound");
	GloveModel.AddItem("HandWrap");
	GloveModel.AddItem("Driver");
	GloveModel.AddItem("Sport");
	GloveModel.AddItem("Motorcycle");
	GloveModel.AddItem("Specialist");
	GloveGroup.PlaceLabledControl("Glove Model", this, &GloveModel);

	GloveSkin.SetFileId("glove_skin");
	GloveSkin.AddItem("None");
	GloveSkin.AddItem("BloodHound - Snakebite"); //10007
	GloveSkin.AddItem("BloodHound - Charred");// 10006
	GloveSkin.AddItem("BloodHound - Guerrilla");// 10039
	GloveSkin.AddItem("BloodHound - Bronzed");// 10008
	GloveSkin.AddItem("HandWrap - Slaughter"); // 10021
	GloveSkin.AddItem("HandWrap - Badlands");// 10036
	GloveSkin.AddItem("HandWrap - Leather");// 10009
	GloveSkin.AddItem("HandWrap - Spruce DDPAT");// 10010
	GloveSkin.AddItem("Driver - Crimson Weave");// 10016
	GloveSkin.AddItem("Driver - Lunar Weave");// 10013
	GloveSkin.AddItem("Driver - Diamondback");// 10040
	GloveSkin.AddItem("Driver - Convoy");// 10015
	GloveSkin.AddItem("Sport - Pandora's Box");// 10037
	GloveSkin.AddItem("Sport - Hedge Maze");// 10038
	GloveSkin.AddItem("Sport - Superconductor");// 10018
	GloveSkin.AddItem("Sport - Arid");// 10019
	GloveSkin.AddItem("Motorcycle - Spearmint");// 10026
	GloveSkin.AddItem("Motorcycle - Cool Mint");// 10028
	GloveSkin.AddItem("Motorcycle - Boom");// 10027
	GloveSkin.AddItem("Motorcycle - Eclipse");// 10024
	GloveSkin.AddItem("Specialist - Crimson Kimono");// 10033
	GloveSkin.AddItem("Specialist - Emerald Web");// 10034
	GloveSkin.AddItem("Specialist - Foundation");// 10035
	GloveSkin.AddItem("Specialist - Forest DDPAT");// 10030
	GloveGroup.PlaceLabledControl("Skin", this, &GloveSkin);

	/*#pragma region Machineguns
	MachinegunsGroup.SetPosition(621, 20);
	MachinegunsGroup.SetText("Machineguns");
	MachinegunsGroup.SetSize(261, 80);
	RegisterControl(&MachinegunsGroup);

	NEGEVSkin.SetFileId("negev_skin");
	NEGEVSkin.AddItem("Anodized Navy");
	NEGEVSkin.AddItem("Man-o'-war");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("VariCamo");
	NEGEVSkin.AddItem("Palm");
	NEGEVSkin.AddItem("CaliCamo");
	NEGEVSkin.AddItem("Terrain");
	NEGEVSkin.AddItem("Army Sheen");
	NEGEVSkin.AddItem("Bratatat");
	NEGEVSkin.AddItem("Desert-Strike");
	NEGEVSkin.AddItem("Nuclear Waste");
	NEGEVSkin.AddItem("Loudmouth");
	NEGEVSkin.AddItem("Power Loader");
	MachinegunsGroup.PlaceLabledControl("Negev", this, &NEGEVSkin);

	M249Skin.SetFileId("m249_skin");
	M249Skin.AddItem("Contrast Spray");
	M249Skin.AddItem("Blizzard Marbleized");
	M249Skin.AddItem("Jungle DDPAT");
	M249Skin.AddItem("Gator Mesh");
	M249Skin.AddItem("Magma");
	M249Skin.AddItem("System Lock");
	M249Skin.AddItem("Shipping Forecast");
	M249Skin.AddItem("Impact Drill");
	M249Skin.AddItem("Nebula Crusader");
	M249Skin.AddItem("Spectre");
	MachinegunsGroup.PlaceLabledControl("M249", this, &M249Skin);

	#pragma endregion*/

#pragma region Snipers
	Snipergroup.SetPosition(12, 120);
	Snipergroup.SetText("Snipers");
	Snipergroup.SetSize(331, 155);
	RegisterControl(&Snipergroup);

	AWPSkin.SetFileId("awp_skin");
	AWPSkin.AddItem("BOOM");
	AWPSkin.AddItem("Dragon Lore");
	AWPSkin.AddItem("Pink DDPAT");
	AWPSkin.AddItem("Snake Camo");
	AWPSkin.AddItem("Lightning Strike");
	AWPSkin.AddItem("Safari Mesh");
	AWPSkin.AddItem("Corticera");
	AWPSkin.AddItem("Redline");
	AWPSkin.AddItem("Man-o'-war");
	AWPSkin.AddItem("Graphite");
	AWPSkin.AddItem("Electric Hive");
	AWPSkin.AddItem("Pit Viper");
	AWPSkin.AddItem("Asiimov");
	AWPSkin.AddItem("Worm God");
	AWPSkin.AddItem("Medusa");
	AWPSkin.AddItem("Sun in Leo");
	AWPSkin.AddItem("Hyper Beast");
	AWPSkin.AddItem("Elite Build");
	Snipergroup.PlaceLabledControl("AWP", this, &AWPSkin);

	SSG08Skin.SetFileId("sgg08_skin");
	SSG08Skin.AddItem("Lichen Dashed");
	SSG08Skin.AddItem("Dark Water");
	SSG08Skin.AddItem("Blue Spruce");
	SSG08Skin.AddItem("Sand Dune");
	SSG08Skin.AddItem("Palm");
	SSG08Skin.AddItem("Mayan Dreams");
	SSG08Skin.AddItem("Blood in the Water");
	SSG08Skin.AddItem("Tropical Storm");
	SSG08Skin.AddItem("Acid Fade");
	SSG08Skin.AddItem("Slashed");
	SSG08Skin.AddItem("Detour");
	SSG08Skin.AddItem("Abyss");
	SSG08Skin.AddItem("Big Iron");
	SSG08Skin.AddItem("Necropos");
	SSG08Skin.AddItem("Ghost Crusader");
	SSG08Skin.AddItem("Dragonfire");
	Snipergroup.PlaceLabledControl("SGG 08", this, &SSG08Skin);

	SCAR20Skin.SetFileId("scar20_skin");
	SCAR20Skin.AddItem("Splash Jam");
	SCAR20Skin.AddItem("Storm");
	SCAR20Skin.AddItem("Contractor");
	SCAR20Skin.AddItem("Carbon Fiber");
	SCAR20Skin.AddItem("Sand Mesh");
	SCAR20Skin.AddItem("Palm");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Crimson Web");
	SCAR20Skin.AddItem("Cardiac");
	SCAR20Skin.AddItem("Army Sheen");
	SCAR20Skin.AddItem("Cyrex");
	SCAR20Skin.AddItem("Grotto");
	SCAR20Skin.AddItem("Emerald");
	SCAR20Skin.AddItem("Green Marine");
	SCAR20Skin.AddItem("Outbreak");
	SCAR20Skin.AddItem("Bloodsport");
	SCAR20Skin.AddItem("Slaughter");
	SCAR20Skin.AddItem("Tiger Tooth");
	SCAR20Skin.AddItem("MFade");
	Snipergroup.PlaceLabledControl("SCAR-20", this, &SCAR20Skin);

	G3SG1Skin.SetFileId("g3sg1_skin");
	G3SG1Skin.AddItem("Desert Storm");
	G3SG1Skin.AddItem("Arctic Camo");
	G3SG1Skin.AddItem("Bone Mask");
	G3SG1Skin.AddItem("Contractor");
	G3SG1Skin.AddItem("Safari Mesh");
	G3SG1Skin.AddItem("Polar Camo");
	G3SG1Skin.AddItem("Jungle Dashed");
	G3SG1Skin.AddItem("VariCamo");
	G3SG1Skin.AddItem("Predator");
	G3SG1Skin.AddItem("Demeter");
	G3SG1Skin.AddItem("Azure Zebra");
	G3SG1Skin.AddItem("Green Apple");
	G3SG1Skin.AddItem("Orange Kimono");
	G3SG1Skin.AddItem("Neon Kimono");
	G3SG1Skin.AddItem("Murky");
	G3SG1Skin.AddItem("Chronos");
	G3SG1Skin.AddItem("Flux");
	G3SG1Skin.AddItem("The Executioner");
	G3SG1Skin.AddItem("Orange Crash");
	G3SG1Skin.AddItem("Slaughter");
	G3SG1Skin.AddItem("Tiger Tooth");
	G3SG1Skin.AddItem("MFade");
	Snipergroup.PlaceLabledControl("G3SG1", this, &G3SG1Skin);
#pragma endregion

	/*#pragma region Shotguns
	Shotgungroup.SetPosition(621, 200);
	Shotgungroup.SetText("Shotguns");
	Shotgungroup.SetSize(261, 125);
	RegisterControl(&Shotgungroup);

	MAG7Skin.SetFileId("mag7_skin");
	MAG7Skin.AddItem("Counter Terrace");
	MAG7Skin.AddItem("Metallic DDPAT");
	MAG7Skin.AddItem("Silver");
	MAG7Skin.AddItem("Storm");
	MAG7Skin.AddItem("Bulldozer");
	MAG7Skin.AddItem("Heat");
	MAG7Skin.AddItem("Sand Dune");
	MAG7Skin.AddItem("Irradiated Alert");
	MAG7Skin.AddItem("Memento");
	MAG7Skin.AddItem("Hazard");
	MAG7Skin.AddItem("Heaven Guard");
	MAG7Skin.AddItem("Firestarter");
	MAG7Skin.AddItem("Seabird");
	MAG7Skin.AddItem("Cobalt Core");
	MAG7Skin.AddItem("Praetorian");
	Shotgungroup.PlaceLabledControl("Mag-7", this, &MAG7Skin);

	XM1014Skin.SetFileId("xm1014_skin");
	XM1014Skin.AddItem("Blaze Orange");
	XM1014Skin.AddItem("VariCamo Blue");
	XM1014Skin.AddItem("Bone Mask");
	XM1014Skin.AddItem("Blue Steel");
	XM1014Skin.AddItem("Blue Spruce");
	XM1014Skin.AddItem("Grassland");
	XM1014Skin.AddItem("Urban Perforated");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("VariCamo");
	XM1014Skin.AddItem("Fallout Warning");
	XM1014Skin.AddItem("Jungle");
	XM1014Skin.AddItem("CaliCamo");
	XM1014Skin.AddItem("Pit Viper");
	XM1014Skin.AddItem("Tranquility");
	XM1014Skin.AddItem("Red Python");
	XM1014Skin.AddItem("Heaven Guard");
	XM1014Skin.AddItem("Red Leather");
	XM1014Skin.AddItem("Bone Machine");
	XM1014Skin.AddItem("Quicksilver");
	XM1014Skin.AddItem("Scumbria");
	XM1014Skin.AddItem("Teclu Burner");
	XM1014Skin.AddItem("Black Tie");
	Shotgungroup.PlaceLabledControl("XM1014", this, &XM1014Skin);

	SAWEDOFFSkin.SetFileId("sawedoff_skin");
	SAWEDOFFSkin.AddItem("First Class");
	SAWEDOFFSkin.AddItem("Forest DDPAT");
	SAWEDOFFSkin.AddItem("Contrast Spray");
	SAWEDOFFSkin.AddItem("Snake Camo");
	SAWEDOFFSkin.AddItem("Orange DDPAT");
	SAWEDOFFSkin.AddItem("Fade");
	SAWEDOFFSkin.AddItem("Copper");
	SAWEDOFFSkin.AddItem("Origami");
	SAWEDOFFSkin.AddItem("Sage Spray");
	SAWEDOFFSkin.AddItem("VariCamo");
	SAWEDOFFSkin.AddItem("Irradiated Alert");
	SAWEDOFFSkin.AddItem("Mosaico");
	SAWEDOFFSkin.AddItem("Serenity");
	SAWEDOFFSkin.AddItem("Amber Fade");
	SAWEDOFFSkin.AddItem("Full Stop");
	SAWEDOFFSkin.AddItem("Highwayman");
	SAWEDOFFSkin.AddItem("The Kraken");
	SAWEDOFFSkin.AddItem("Rust Coat");
	SAWEDOFFSkin.AddItem("Bamboo Shadow");
	SAWEDOFFSkin.AddItem("Bamboo Forest");
	SAWEDOFFSkin.AddItem("Yorick");
	SAWEDOFFSkin.AddItem("Fubar");
	SAWEDOFFSkin.AddItem("Wasteland Princess");
	Shotgungroup.PlaceLabledControl("Sawed-Off", this, &SAWEDOFFSkin);

	NOVASkin.SetFileId("nova_skin");
	NOVASkin.AddItem("Candy Apple");
	NOVASkin.AddItem("Blaze Orange");
	NOVASkin.AddItem("Modern Hunter");
	NOVASkin.AddItem("Forest Leaves");
	NOVASkin.AddItem("Bloomstick");
	NOVASkin.AddItem("Sand Dune");
	NOVASkin.AddItem("Polar Mesh");
	NOVASkin.AddItem("Walnut");
	NOVASkin.AddItem("Predator");
	NOVASkin.AddItem("Tempest");
	NOVASkin.AddItem("Graphite");
	NOVASkin.AddItem("Ghost Camo");
	NOVASkin.AddItem("Rising Skull");
	NOVASkin.AddItem("Antique");
	NOVASkin.AddItem("Green Apple");
	NOVASkin.AddItem("Caged Steel");
	NOVASkin.AddItem("Koi");
	NOVASkin.AddItem("Moon in Libra");
	NOVASkin.AddItem("Ranger");
	NOVASkin.AddItem("HyperBeast");
	Shotgungroup.PlaceLabledControl("Nova", this, &NOVASkin);
	#pragma endregion*/

#pragma region Rifles
	Riflegroup.SetPosition(12, 295);
	Riflegroup.SetText("Rifles");
	Riflegroup.SetSize(331, 200);
	RegisterControl(&Riflegroup);

	AK47Skin.SetFileId("ak47_skin");
	AK47Skin.AddItem("First Class");
	AK47Skin.AddItem("Red Laminate");
	AK47Skin.AddItem("Case Hardened");
	AK47Skin.AddItem("Black Laminate");
	AK47Skin.AddItem("Fire Serpent");
	AK47Skin.AddItem("Cartel");
	AK47Skin.AddItem("Emerald Pinstripe");
	AK47Skin.AddItem("Blue Laminate");
	AK47Skin.AddItem("Redline");
	AK47Skin.AddItem("Vulcan");
	AK47Skin.AddItem("Jaguar");
	AK47Skin.AddItem("Jet Set");
	AK47Skin.AddItem("Wasteland Rebel");
	AK47Skin.AddItem("Elite Build");
	AK47Skin.AddItem("Hydroponic");
	AK47Skin.AddItem("Aquamarine Revenge");
	AK47Skin.AddItem("Frontside Misty");
	AK47Skin.AddItem("Point Disarray");
	AK47Skin.AddItem("Fuel Injector");
	AK47Skin.AddItem("Neon Revolution");
	AK47Skin.AddItem("Slaughter");
	AK47Skin.AddItem("Tiger Tooth");
	AK47Skin.AddItem("MFade");
	Riflegroup.PlaceLabledControl("AK-47", this, &AK47Skin);

	M41SSkin.SetFileId("m4a1s_skin");
	M41SSkin.AddItem("Dark Water");
	M41SSkin.AddItem("Hyper Beast");
	M41SSkin.AddItem("Boreal Forest");
	M41SSkin.AddItem("VariCamo");
	M41SSkin.AddItem("Nitro");
	M41SSkin.AddItem("Bright Water");
	M41SSkin.AddItem("Atomic Alloy");
	M41SSkin.AddItem("Blood Tiger");
	M41SSkin.AddItem("Guardian");
	M41SSkin.AddItem("Master Piece");
	M41SSkin.AddItem("Knight");
	M41SSkin.AddItem("Cyrex");
	M41SSkin.AddItem("Basilisk");
	M41SSkin.AddItem("Icarus Fell");
	M41SSkin.AddItem("Hot Rod");
	M41SSkin.AddItem("Golden Coi");
	M41SSkin.AddItem("Chantico's Fire");
	M41SSkin.AddItem("Mecha Industries");
	M41SSkin.AddItem("Flashback");
	M41SSkin.AddItem("Slaughter");
	M41SSkin.AddItem("Tiger Tooth");
	M41SSkin.AddItem("MFade");
	Riflegroup.PlaceLabledControl("M4A1-S", this, &M41SSkin);

	M4A4Skin.SetFileId("m4a4_skin");
	M4A4Skin.AddItem("Bullet Rain");
	M4A4Skin.AddItem("Zirka");
	M4A4Skin.AddItem("Asiimov");
	M4A4Skin.AddItem("Howl");
	M4A4Skin.AddItem("X-Ray");
	M4A4Skin.AddItem("Desert-Strike");
	M4A4Skin.AddItem("Griffin");
	M4A4Skin.AddItem("Dragon King");
	M4A4Skin.AddItem("Poseidon");
	M4A4Skin.AddItem("Daybreak");
	M4A4Skin.AddItem("Evil Daimyo");
	M4A4Skin.AddItem("Royal Paladin");
	M4A4Skin.AddItem("The BattleStar");
	M4A4Skin.AddItem("Desolate Space");
	M4A4Skin.AddItem("Buzz Kill");
	M4A4Skin.AddItem("Slaughter");
	M4A4Skin.AddItem("Tiger Tooth");
	M4A4Skin.AddItem("MFade");
	Riflegroup.PlaceLabledControl("M4A4", this, &M4A4Skin);

	AUGSkin.SetFileId("aug_skin");
	AUGSkin.AddItem("Bengal Tiger");
	AUGSkin.AddItem("Hot Rod");
	AUGSkin.AddItem("Chameleon");
	AUGSkin.AddItem("Torque");
	AUGSkin.AddItem("Radiation Hazard");
	AUGSkin.AddItem("Asterion");
	AUGSkin.AddItem("Daedalus");
	AUGSkin.AddItem("Akihabara Accept");
	AUGSkin.AddItem("Ricochet");
	AUGSkin.AddItem("Fleet Flock");
	AUGSkin.AddItem("Syd Mead");
	Riflegroup.PlaceLabledControl("AUG", this, &AUGSkin);

	FAMASSkin.SetFileId("famas_skin");
	FAMASSkin.AddItem("Contrast Spray");
	FAMASSkin.AddItem("Colony");
	FAMASSkin.AddItem("Cyanospatter");
	FAMASSkin.AddItem("Djinn");
	FAMASSkin.AddItem("Afterimage");
	FAMASSkin.AddItem("Doomkitty");
	FAMASSkin.AddItem("Spitfire");
	FAMASSkin.AddItem("Teardown");
	FAMASSkin.AddItem("Hexane");
	FAMASSkin.AddItem("Pulse");
	FAMASSkin.AddItem("Sergeant");
	FAMASSkin.AddItem("Styx");
	FAMASSkin.AddItem("Neural Net");
	FAMASSkin.AddItem("Survivor");
	FAMASSkin.AddItem("Valence");
	FAMASSkin.AddItem("Roll Cage");
	FAMASSkin.AddItem("Mecha Industries");
	Riflegroup.PlaceLabledControl("FAMAS", this, &FAMASSkin);

	GALILSkin.SetFileId("galil_skin");
	GALILSkin.AddItem("Forest DDPAT");
	GALILSkin.AddItem("Contrast Spray");
	GALILSkin.AddItem("Orange DDPAT");
	GALILSkin.AddItem("Eco");
	GALILSkin.AddItem("Winter Forest");
	GALILSkin.AddItem("Sage Spray");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("VariCamo");
	GALILSkin.AddItem("Chatterbox");
	GALILSkin.AddItem("Shattered");
	GALILSkin.AddItem("Kami");
	GALILSkin.AddItem("Blue Titanium");
	GALILSkin.AddItem("Urban Rubble");
	GALILSkin.AddItem("Hunting Blind");
	GALILSkin.AddItem("Sandstorm");
	GALILSkin.AddItem("Tuxedo");
	GALILSkin.AddItem("Cerberus");
	GALILSkin.AddItem("Aqua Terrace");
	GALILSkin.AddItem("Rocket Pop");
	GALILSkin.AddItem("Stone Cold");
	GALILSkin.AddItem("Firefight");
	Riflegroup.PlaceLabledControl("GALIL", this, &GALILSkin);

	SG553Skin.SetFileId("sg552_skin");
	SG553Skin.AddItem("Bulldozer");
	SG553Skin.AddItem("Ultraviolet");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Damascus Steel");
	SG553Skin.AddItem("Pulse");
	SG553Skin.AddItem("Army Sheen");
	SG553Skin.AddItem("Traveler");
	SG553Skin.AddItem("Fallout Warning");
	SG553Skin.AddItem("Cyrex");
	SG553Skin.AddItem("Tiger Moth");
	SG553Skin.AddItem("Atlas");
	Riflegroup.PlaceLabledControl("SG552", this, &SG553Skin);
#pragma endregion

	/*#pragma region MPs
	MPGroup.SetPosition(621, 335);
	MPGroup.SetText("MPs");
	MPGroup.SetSize(261, 165);
	RegisterControl(&MPGroup);

	MAC10Skin.SetFileId("mac10_skin");
	MAC10Skin.AddItem("Tornado");
	MAC10Skin.AddItem("Candy Apple");
	MAC10Skin.AddItem("Silver");
	MAC10Skin.AddItem("Forest DDPAT");
	MAC10Skin.AddItem("Urban DDPAT");
	MAC10Skin.AddItem("Fade");
	MAC10Skin.AddItem("Neon Rider");
	MAC10Skin.AddItem("Ultraviolet");
	MAC10Skin.AddItem("Palm");
	MAC10Skin.AddItem("Graven");
	MAC10Skin.AddItem("Tatter");
	MAC10Skin.AddItem("Amber Fade");
	MAC10Skin.AddItem("Heat");
	MAC10Skin.AddItem("Curse");
	MAC10Skin.AddItem("Indigo");
	MAC10Skin.AddItem("Commuter");
	MAC10Skin.AddItem("Nuclear Garden");
	MAC10Skin.AddItem("Malachite");
	MAC10Skin.AddItem("Rangeen");
	MAC10Skin.AddItem("Lapis Gator");
	MPGroup.PlaceLabledControl("MAC-10", this, &MAC10Skin);

	P90Skin.SetFileId("p90_skin");
	P90Skin.AddItem("Leather");
	P90Skin.AddItem("Virus");
	P90Skin.AddItem("Contrast Spray");
	P90Skin.AddItem("Storm");
	P90Skin.AddItem("Cold Blooded");
	P90Skin.AddItem("Glacier Mesh");
	P90Skin.AddItem("Sand Spray");
	P90Skin.AddItem("Death by Kitty");
	P90Skin.AddItem("Ash Wood");
	P90Skin.AddItem("Fallout Warning");
	P90Skin.AddItem("Scorched");
	P90Skin.AddItem("Emerald Dragon");
	P90Skin.AddItem("Teardown");
	P90Skin.AddItem("Blind Spot");
	P90Skin.AddItem("Trigon");
	P90Skin.AddItem("Desert Warfare");
	P90Skin.AddItem("Module");
	P90Skin.AddItem("Asiimov");
	P90Skin.AddItem("Elite Build");
	P90Skin.AddItem("Shapewood");
	P90Skin.AddItem("Shallow Grave");
	MPGroup.PlaceLabledControl("P90", this, &P90Skin);

	UMP45Skin.SetFileId("ump45_skin");
	UMP45Skin.AddItem("Blaze");
	UMP45Skin.AddItem("Forest DDPAT");
	UMP45Skin.AddItem("Gunsmoke");
	UMP45Skin.AddItem("Urban DDPAT");
	UMP45Skin.AddItem("Grand Prix");
	UMP45Skin.AddItem("Carbon Fiber");
	UMP45Skin.AddItem("Caramel");
	UMP45Skin.AddItem("Fallout Warning");
	UMP45Skin.AddItem("Scorched");
	UMP45Skin.AddItem("Bone Pile");
	UMP45Skin.AddItem("Delusion");
	UMP45Skin.AddItem("Corporal");
	UMP45Skin.AddItem("Indigo");
	UMP45Skin.AddItem("Labyrinth");
	UMP45Skin.AddItem("Minotaur's Labyrinth");
	UMP45Skin.AddItem("Riot");
	UMP45Skin.AddItem("Primal Saber");
	MPGroup.PlaceLabledControl("UMP-45", this, &UMP45Skin);

	BIZONSkin.SetFileId("bizon_skin");
	BIZONSkin.AddItem("Blue Streak");
	BIZONSkin.AddItem("Modern Hunter");
	BIZONSkin.AddItem("Forest Leaves");
	BIZONSkin.AddItem("Bone Mask");
	BIZONSkin.AddItem("Carbon Fiber");
	BIZONSkin.AddItem("Sand Dashed");
	BIZONSkin.AddItem("Urban Dashed");
	BIZONSkin.AddItem("Brass");
	BIZONSkin.AddItem("VariCamo");
	BIZONSkin.AddItem("Irradiated Alert");
	BIZONSkin.AddItem("Rust Coat");
	BIZONSkin.AddItem("Water Sigil");
	BIZONSkin.AddItem("Night Ops");
	BIZONSkin.AddItem("Cobalt Halftone");
	BIZONSkin.AddItem("Antique");
	BIZONSkin.AddItem("Rust Coat");
	BIZONSkin.AddItem("Osiris");
	BIZONSkin.AddItem("Chemical Green");
	BIZONSkin.AddItem("Bamboo Print");
	BIZONSkin.AddItem("Bamboo Forest");
	BIZONSkin.AddItem("Fuel Rod");
	BIZONSkin.AddItem("Photic Zone");
	BIZONSkin.AddItem("Judgement of Anubis");
	MPGroup.PlaceLabledControl("PP-Bizon", this, &BIZONSkin);

	MP7Skin.SetFileId("mp7_skin");
	MP7Skin.AddItem("Groundwater");
	MP7Skin.AddItem("Whiteout");
	MP7Skin.AddItem("Forest DDPAT");
	MP7Skin.AddItem("Anodized Navy");
	MP7Skin.AddItem("Skulls");
	MP7Skin.AddItem("Gunsmoke");
	MP7Skin.AddItem("Contrast Spray");
	MP7Skin.AddItem("Bone Mask");
	MP7Skin.AddItem("Ossified");
	MP7Skin.AddItem("Orange Peel");
	MP7Skin.AddItem("VariCamo");
	MP7Skin.AddItem("Army Recon");
	MP7Skin.AddItem("Groundwater");
	MP7Skin.AddItem("Ocean Foam");
	MP7Skin.AddItem("Full Stop");
	MP7Skin.AddItem("Urban Hazard");
	MP7Skin.AddItem("Olive Plaid");
	MP7Skin.AddItem("Armor Core");
	MP7Skin.AddItem("Asterion");
	MP7Skin.AddItem("Nemesis");
	MP7Skin.AddItem("Special Delivery");
	MP7Skin.AddItem("Impire");
	MPGroup.PlaceLabledControl("MP7", this, &MP7Skin);

	MP9Skin.SetFileId("mp9_skin");
	MP9Skin.AddItem("Ruby Poison Dart");
	MP9Skin.AddItem("Bone Mask");
	MP9Skin.AddItem("Hot Rod");
	MP9Skin.AddItem("Storm");
	MP9Skin.AddItem("Bulldozer");
	MP9Skin.AddItem("Hypnotic");
	MP9Skin.AddItem("Sand Dashed");
	MP9Skin.AddItem("Orange Peel");
	MP9Skin.AddItem("Dry Season");
	MP9Skin.AddItem("Dark Age");
	MP9Skin.AddItem("Rose Iron");
	MP9Skin.AddItem("Green Plaid");
	MP9Skin.AddItem("Setting Sun");
	MP9Skin.AddItem("Dart");
	MP9Skin.AddItem("Deadly Poison");
	MP9Skin.AddItem("Pandora's Box");
	MP9Skin.AddItem("Bioleak");
	MP9Skin.AddItem("Airlock");
	MPGroup.PlaceLabledControl("MP9", this, &MP9Skin);

	#pragma endregion*/

#pragma region Pistols
	PistolGroup.SetPosition(371, 150);
	PistolGroup.SetText("Pistols");
	PistolGroup.SetSize(331, 345);
	RegisterControl(&PistolGroup);

	GLOCKSkin.SetFileId("glock_skin");
	GLOCKSkin.AddItem("Groundwater");
	GLOCKSkin.AddItem("Candy Apple");
	GLOCKSkin.AddItem("Fade");
	GLOCKSkin.AddItem("Night");
	GLOCKSkin.AddItem("Dragon Tattoo");
	GLOCKSkin.AddItem("Twilight Galaxy");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Brass");
	GLOCKSkin.AddItem("Catacombs");
	GLOCKSkin.AddItem("Sand Dune");
	GLOCKSkin.AddItem("Steel Disruption");
	GLOCKSkin.AddItem("Blue Fissure");
	GLOCKSkin.AddItem("Death Rattle");
	GLOCKSkin.AddItem("Water Elemental");
	GLOCKSkin.AddItem("Reactor");
	GLOCKSkin.AddItem("Grinder");
	GLOCKSkin.AddItem("Bunsen Burner");
	GLOCKSkin.AddItem("Wraith");
	GLOCKSkin.AddItem("Royal Legion");
	GLOCKSkin.AddItem("Weasel");
	GLOCKSkin.AddItem("Wasteland Rebel");
	PistolGroup.PlaceLabledControl("Glock", this, &GLOCKSkin);

	USPSSkin.SetFileId("usps_skin");
	USPSSkin.AddItem("Forest Leaves");
	USPSSkin.AddItem("Dark Water");
	USPSSkin.AddItem("VariCamo");
	USPSSkin.AddItem("Overgrowth");
	USPSSkin.AddItem("Caiman");
	USPSSkin.AddItem("Blood Tiger");
	USPSSkin.AddItem("Serum");
	USPSSkin.AddItem("Night Ops");
	USPSSkin.AddItem("Stainless");
	USPSSkin.AddItem("Guardian");
	USPSSkin.AddItem("Orion");
	USPSSkin.AddItem("Road Rash");
	USPSSkin.AddItem("Royal Blue");
	USPSSkin.AddItem("Business Class");
	USPSSkin.AddItem("Para Green");
	USPSSkin.AddItem("Torque");
	USPSSkin.AddItem("Kill Confirmed");
	USPSSkin.AddItem("Lead Conduit");
	USPSSkin.AddItem("Cyrex");
	PistolGroup.PlaceLabledControl("USP-S", this, &USPSSkin);

	DEAGLESkin.SetFileId("deagle_skin");
	DEAGLESkin.AddItem("Blaze");
	DEAGLESkin.AddItem("Pilot");
	DEAGLESkin.AddItem("Midnight Storm");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Forest DDPAT");
	DEAGLESkin.AddItem("Crimson Web");
	DEAGLESkin.AddItem("Urban DDPAT");
	DEAGLESkin.AddItem("Night");
	DEAGLESkin.AddItem("Hypnotic");
	DEAGLESkin.AddItem("Mudder");
	DEAGLESkin.AddItem("VariCamo");
	DEAGLESkin.AddItem("Golden Koi");
	DEAGLESkin.AddItem("Cobalt Disruption");
	DEAGLESkin.AddItem("Urban Rubble");
	DEAGLESkin.AddItem("Naga");
	DEAGLESkin.AddItem("Hand Cannon");
	DEAGLESkin.AddItem("Heirloom");
	DEAGLESkin.AddItem("Meteorite");
	DEAGLESkin.AddItem("Conspiracy");
	DEAGLESkin.AddItem("Bronze Deco");
	DEAGLESkin.AddItem("Sunset Storm");
	DEAGLESkin.AddItem("Corinthian");
	DEAGLESkin.AddItem("Kumicho Dragon");
	DEAGLESkin.AddItem("Slaughter");
	DEAGLESkin.AddItem("Tiger Tooth");
	DEAGLESkin.AddItem("MFade");
	PistolGroup.PlaceLabledControl("Deagle", this, &DEAGLESkin);

	R8Skin.SetFileId("deagle_skin");
	R8Skin.AddItem("Bone Mask");
	R8Skin.AddItem("Crimson Web");
	R8Skin.AddItem("Fade");
	R8Skin.AddItem("Amber Fade");
	R8Skin.AddItem("Slaughter");
	R8Skin.AddItem("Tiger Tooth");
	R8Skin.AddItem("MFade");
	//R8Skin.AddItem("");
	PistolGroup.PlaceLabledControl("Deagle", this, &R8Skin);

	DUALSSkin.SetFileId("duals_skin");
	DUALSSkin.AddItem("Anodized Navy");
	DUALSSkin.AddItem("Ossified");
	DUALSSkin.AddItem("Stained");
	DUALSSkin.AddItem("Contractor");
	DUALSSkin.AddItem("Colony");
	DUALSSkin.AddItem("Demolition");
	DUALSSkin.AddItem("Black Limba");
	DUALSSkin.AddItem("Red Quartz");
	DUALSSkin.AddItem("Cobalt Quartz");
	DUALSSkin.AddItem("Hemoglobin");
	DUALSSkin.AddItem("Urban Shock");
	DUALSSkin.AddItem("Marina");
	DUALSSkin.AddItem("Panther");
	DUALSSkin.AddItem("Retribution");
	DUALSSkin.AddItem("Briar");
	DUALSSkin.AddItem("Duelist");
	DUALSSkin.AddItem("Moon in Libra");
	DUALSSkin.AddItem("Dualing Dragons");
	DUALSSkin.AddItem("Cartel");
	DUALSSkin.AddItem("Ventilators");
	PistolGroup.PlaceLabledControl("Duals", this, &DUALSSkin);

	FIVESEVENSkin.SetFileId("fiveseven_skin");
	FIVESEVENSkin.AddItem("Candy Apple");
	FIVESEVENSkin.AddItem("Bone Mask");
	FIVESEVENSkin.AddItem("Case Hardened");
	FIVESEVENSkin.AddItem("Contractor");
	FIVESEVENSkin.AddItem("Forest Night");
	FIVESEVENSkin.AddItem("Orange Peel");
	FIVESEVENSkin.AddItem("Jungle");
	FIVESEVENSkin.AddItem("Nitro");
	FIVESEVENSkin.AddItem("Red Quartz");
	FIVESEVENSkin.AddItem("Anodized Gunmetal");
	FIVESEVENSkin.AddItem("Nightshade");
	FIVESEVENSkin.AddItem("Silver Quartz");
	FIVESEVENSkin.AddItem("Kami");
	FIVESEVENSkin.AddItem("Copper Galaxy");
	FIVESEVENSkin.AddItem("Neon Kimono");
	FIVESEVENSkin.AddItem("Fowl Play");
	FIVESEVENSkin.AddItem("Hot Shot");
	FIVESEVENSkin.AddItem("Urban Hazard");
	FIVESEVENSkin.AddItem("Monkey Business");
	FIVESEVENSkin.AddItem("Retrobution");
	FIVESEVENSkin.AddItem("Triumvirate");
	PistolGroup.PlaceLabledControl("Five-Seven", this, &FIVESEVENSkin);

	TECNINESkin.SetFileId("tec9_skin");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Groundwater");
	TECNINESkin.AddItem("Forest DDPAT");
	TECNINESkin.AddItem("Terrace");
	TECNINESkin.AddItem("Urban DDPAT");
	TECNINESkin.AddItem("Ossified");
	TECNINESkin.AddItem("Hades");
	TECNINESkin.AddItem("Brass");
	TECNINESkin.AddItem("VariCamo");
	TECNINESkin.AddItem("Nuclear Threat");
	TECNINESkin.AddItem("Red Quartz");
	TECNINESkin.AddItem("Tornado");
	TECNINESkin.AddItem("Blue Titanium");
	TECNINESkin.AddItem("Army Mesh");
	TECNINESkin.AddItem("Titanium Bit");
	TECNINESkin.AddItem("Sandstorm");
	TECNINESkin.AddItem("Isaac");
	TECNINESkin.AddItem("Toxic");
	TECNINESkin.AddItem("Bamboo Forest");
	TECNINESkin.AddItem("Avalanche");
	TECNINESkin.AddItem("Jambiya");
	TECNINESkin.AddItem("Re-Entry");
	TECNINESkin.AddItem("Fuel Injector");
	PistolGroup.PlaceLabledControl("Tec-9", this, &TECNINESkin);

	P2000Skin.SetFileId("p2000_skin");
	P2000Skin.AddItem("Grassland Leaves");
	P2000Skin.AddItem("Silver");
	P2000Skin.AddItem("Granite Marbleized");
	P2000Skin.AddItem("Forest Leaves");
	P2000Skin.AddItem("Ossified");
	P2000Skin.AddItem("Handgun");
	P2000Skin.AddItem("Fade");
	P2000Skin.AddItem("Scorpion");
	P2000Skin.AddItem("Grassland");
	P2000Skin.AddItem("Corticera");
	P2000Skin.AddItem("Ocean Foam");
	P2000Skin.AddItem("Pulse");
	P2000Skin.AddItem("Amber Fade");
	P2000Skin.AddItem("Red FragCam");
	P2000Skin.AddItem("Chainmail");
	P2000Skin.AddItem("Coach Class");
	P2000Skin.AddItem("Ivory");
	P2000Skin.AddItem("Fire Elemental");
	P2000Skin.AddItem("Asterion");
	P2000Skin.AddItem("Pathfinder");
	P2000Skin.AddItem("Imperial");
	P2000Skin.AddItem("Oceanic");
	P2000Skin.AddItem("Imperial Dragon");
	PistolGroup.PlaceLabledControl("P2000", this, &P2000Skin);

	P250Skin.SetFileId("p250_skin");
	P250Skin.AddItem("Whiteout");
	P250Skin.AddItem("Metallic DDPAT");
	P250Skin.AddItem("Splash");
	P250Skin.AddItem("Gunsmoke");
	P250Skin.AddItem("Modern Hunter");
	P250Skin.AddItem("Bone Mask");
	P250Skin.AddItem("Boreal Forest");
	P250Skin.AddItem("Sand Dune");
	P250Skin.AddItem("Nuclear Threat");
	P250Skin.AddItem("Mehndi");
	P250Skin.AddItem("Facets");
	P250Skin.AddItem("Hive");
	P250Skin.AddItem("Muertos");
	P250Skin.AddItem("Steel Disruption");
	P250Skin.AddItem("Undertow");
	P250Skin.AddItem("Franklin");
	P250Skin.AddItem("Neon Kimono");
	P250Skin.AddItem("Supernova");
	P250Skin.AddItem("Contamination");
	P250Skin.AddItem("Cartel");
	P250Skin.AddItem("Valence");
	P250Skin.AddItem("Crimson Kimono");
	P250Skin.AddItem("Mint Kimono");
	P250Skin.AddItem("Wing Shot");
	P250Skin.AddItem("Asiimov");
	PistolGroup.PlaceLabledControl("P250", this, &P250Skin);

#pragma endregion

	//#pragma region Skinsettings
	//	SkinsettingsGroup.SetPosition(408, 500);
	//	SkinsettingsGroup.SetText("Settings");
	//	SkinsettingsGroup.SetSize(360, 140);
	//	RegisterControl(&SkinsettingsGroup);
	//
	//	StatTrakEnable.SetFileId("skin_stattrack");
	//	SkinsettingsGroup.PlaceCheckBox("Stat Track", this, &StatTrakEnable);
	//
	//	StatTrackAmount.SetFileId("skin_stamount");
	//	SkinsettingsGroup.PlaceCheckBox("Value", this, &StatTrackAmount);
	//
	//	KnifeName.SetFileId("knife_name");
	//	SkinsettingsGroup.PlaceCheckBox("Knife Name", this, &KnifeName);
	//
	//	SkinName.SetFileId("skin_name");
	//	SkinsettingsGroup.PlaceCheckBox("Skin Name", this, &SkinName);

#pragma endregion

#pragma endregion other random options
}

void CPlayersTab::Setup()
{
	SetTitle("PLAYERS");
	RegisterControl(&Menu::Window.SettingsFile);
	RegisterControl(&Menu::Window.UserLabel);


	grpPList.SetPosition(12, 20);
	grpPList.SetSize(497, 419);
	grpPList.SetText("PlayerList");
	grpPList.SetColumns(2);
	RegisterControl(&grpPList);

	lstPlayers.SetPosition(18, 40);
	lstPlayers.SetSize(440, 50);
	lstPlayers.SetHeightInItems(20);
	RegisterControl(&lstPlayers);

	grpPList.PlaceLabledControlPLayerlist("Friendly", this, &PlayerFriendly, 0, 17);
	grpPList.PlaceLabledControlPLayerlist("Aim-Priorety", this, &PlayerAimPrio, 1, 17);
	grpPList.PlaceLabledControlPLayerlist("Callout Spam", this, &PlayerCalloutSpam, 0, 18);


}

DWORD GetPlayerListIndex(int EntId)
{
	player_info_t pinfo;
	Interfaces::Engine->GetPlayerInfo(EntId, &pinfo);

	// Bot
	if (pinfo.guid[0] == 'B' && pinfo.guid[1] == 'O')
	{
		char buf[64]; sprintf_s(buf, "BOT_420%sAY", pinfo.name);
		return CRC32(buf, 64);
	}
	else
	{
		return CRC32(pinfo.guid, 32);
	}
}

bool IsFriendly(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].Friendly;
	}

	return false;
}

bool IsAimPrio(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].AimPrio;
	}

	return false;
}

bool IsCalloutTarget(int EntId)
{
	DWORD plistId = GetPlayerListIndex(EntId);
	if (PlayerList.find(plistId) != PlayerList.end())
	{
		return PlayerList[plistId].Callout;
	}

	return false;
}

void UpdatePlayerList()
{
	IClientEntity* pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (Interfaces::Engine->IsConnected() && Interfaces::Engine->IsInGame() && pLocal && pLocal->IsAlive())
	{
		Menu::Window.PlayersTab.lstPlayers.ClearItems();

		// Loop through all active entitys
		for (int i = 0; i < Interfaces::EntList->GetHighestEntityIndex(); i++)
		{
			// Get the entity

			player_info_t pinfo;
			if (i != Interfaces::Engine->GetLocalPlayer() && Interfaces::Engine->GetPlayerInfo(i, &pinfo))
			{
				IClientEntity* pEntity = Interfaces::EntList->GetClientEntity(i);
				int HP = 100; char* Location = "Unknown";
				char *Friendly = " ", *AimPrio = " ";

				DWORD plistId = GetPlayerListIndex(Menu::Window.PlayersTab.lstPlayers.GetValue());
				if (PlayerList.find(plistId) != PlayerList.end())
				{
					Friendly = PlayerList[plistId].Friendly ? "Friendly" : "";
					AimPrio = PlayerList[plistId].AimPrio ? "AimPrio" : "";
				}

				if (pEntity && !pEntity->IsDormant())
				{
					HP = pEntity->GetHealth();
					Location = pEntity->GetLastPlaceName();
				}

				char nameBuffer[512];
				sprintf_s(nameBuffer, "%-24s %-10s %-10s ", pinfo.name, IsFriendly(i) ? "Friend" : " ", IsAimPrio(i) ? "AimPrio" : " ", HP, Location);
				Menu::Window.PlayersTab.lstPlayers.AddItem(nameBuffer, i);

			}

		}

		DWORD meme = GetPlayerListIndex(Menu::Window.PlayersTab.lstPlayers.GetValue());

		// Have we switched to a different player?
		static int PrevSelectedPlayer = 0;
		if (PrevSelectedPlayer != Menu::Window.PlayersTab.lstPlayers.GetValue())
		{
			if (PlayerList.find(meme) != PlayerList.end())
			{
				Menu::Window.PlayersTab.PlayerFriendly.SetState(PlayerList[meme].Friendly);
				Menu::Window.PlayersTab.PlayerAimPrio.SetState(PlayerList[meme].AimPrio);
				Menu::Window.PlayersTab.PlayerCalloutSpam.SetState(PlayerList[meme].Callout);

			}
			else
			{
				Menu::Window.PlayersTab.PlayerFriendly.SetState(false);
				Menu::Window.PlayersTab.PlayerAimPrio.SetState(false);
				Menu::Window.PlayersTab.PlayerCalloutSpam.SetState(false);

			}
		}
		PrevSelectedPlayer = Menu::Window.PlayersTab.lstPlayers.GetValue();

		PlayerList[meme].Friendly = Menu::Window.PlayersTab.PlayerFriendly.GetState();
		PlayerList[meme].AimPrio = Menu::Window.PlayersTab.PlayerAimPrio.GetState();
		PlayerList[meme].Callout = Menu::Window.PlayersTab.PlayerCalloutSpam.GetState();

	}
}

void Menu::SetupMenu()
{
	Window.Setup();

	GUI.RegisterWindow(&Window);
	GUI.BindWindow(VK_INSERT, &Window);
}

void Menu::DoUIFrame()
{
	// General Processing

	// If the "all filter is selected tick all the others
	if (Window.VisualsTab.FiltersAll.GetState())
	{
		Window.VisualsTab.FiltersC4.SetState(true);
		Window.VisualsTab.FiltersChickens.SetState(true);
		Window.VisualsTab.FiltersPlayers.SetState(true);
		Window.VisualsTab.FiltersWeapons.SetState(true);
	}

	GUI.Update();
	GUI.Draw();
	UpdatePlayerList();

}


