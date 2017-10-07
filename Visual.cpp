/*
Tim's FlexBeta Framework 2017
*/

#include "Visuals.h"
#include "Interfaces.h"
#include "RenderManager.h"

// Any init here
void CVisuals::Init()
{
	// Idk
}

// Don't really need to do anything in here
void CVisuals::Move(CUserCmd *pCmd, bool &bSendPacket) {}

// Main ESP Drawing loop
void CVisuals::Draw()
{
	// Crosshair
	if (Menu::Window.VisualsTab.OtherCrosshair.GetState())
		DrawCrosshair();

	// Recoil Crosshair
	if (Menu::Window.VisualsTab.OtherRecoilCrosshair.GetIndex())
		DrawRecoilCrosshair();

	if (Menu::Window.VisualsTab.NoScope.GetState())
		NoScope();

	
}

void CVisuals::NoScope()
{
	if (Menu::Window.VisualsTab.NoScope.GetState())
	{
		if (hackManager.pLocal()->IsScoped() && hackManager.pLocal()->IsAlive())
		{
			int Width;
			int Height;
			Interfaces::Engine->GetScreenSize(Width, Height);

			Color cColor = Color(0, 0, 0, 255);
			Render::Line(Width / 2, 0, Width / 2, Height, cColor);
			Render::Line(0, Height / 2, Width, Height / 2, cColor);

		}
	}
}

// Draw a basic crosshair
void CVisuals::DrawCrosshair()
{
	if (Menu::Window.VisualsTab.OtherCrosshair.GetState())
	{
		DrawRecoilCrosshair();
	}
	else
	{
		// Crosshair
		RECT View = Render::GetViewport();
		int MidX = View.right / 2;
		int MidY = View.bottom / 2;

		Render::Line(MidX - 4, MidY - 4, MidX + 4, MidY + 4, Color(0, 255, 0, 255));
		Render::Line(MidX + 4, MidY - 4, MidX - 4, MidY + 4, Color(0, 255, 0, 255));
	}
}

// Recoil crosshair
void CVisuals::DrawRecoilCrosshair()
{
	IClientEntity *pLocal = hackManager.pLocal();

	// Get the view with the recoil
	Vector ViewAngles;
	Interfaces::Engine->GetViewAngles(ViewAngles);
	ViewAngles += pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

	// Build a ray going fowards at that angle
	Vector fowardVec;
	AngleVectors(ViewAngles, &fowardVec);
	fowardVec *= 10000;

	// Get ray start / end
	Vector start = pLocal->GetOrigin() + *pLocal->GetViewOffset();
	Vector end = start + fowardVec, endScreen;

	if (Render::WorldToScreen(end, endScreen) && pLocal->IsAlive())
	{
		RECT View = Render::GetViewport();
#define drX endScreen.x
#define drY endScreen.y
		Color color = Color(255, 0, 0, 255);
		if (Menu::Window.VisualsTab.OtherCrosshair.GetState())
		{
			Render::Line(endScreen.x, endScreen.y, endScreen.x, endScreen.y - 5, color);
			Render::Line(drX, drY - 5, drX + 5, drY - 5, color);

			Render::Line(drX, drY, drX + 5, drY, color);
			Render::Line(drX + 5, drY, drX + 5, drY + 5, color);

			Render::Line(drX, drY, drX, drY + 5, color);
			Render::Line(drX, drY + 5, drX - 5, drY + 5, color);

			Render::Line(drX, drY, drX - 5, drY, color);
			Render::Line(drX - 5, drY, drX - 5, drY - 5, color);
		}


		//	Render::Line(endScreen.x - 10, endScreen.y, endScreen.x + 10, endScreen.y, Color(255, 0, 0, 255));
		//Render::Line(endScreen.x, endScreen.y - 10, endScreen.x, endScreen.y + 10, Color(255, 0, 0, 255));
	}
}