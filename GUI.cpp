/*
Tim's FlexBeta Framework 2017
*/

#include "GUI.h"

#include "RenderManager.h"
#include "MetaInfo.h"

#include <algorithm>
#include "tinyxml2.h"
#include "Controls.h"

#include "GUISPEC.h"

#include "Menu.h"

CUserCmd*		m_cmd;
Vector			m_position;
Vector			m_size;
IClientEntity*	m_player;


CGUI GUI;
CRadar *Radar = new CRadar;
float MenuAlpha = 0.f;

float Globals::MenuAlpha2 = MenuAlpha;

CGUI::CGUI()
{

}

// Draws all windows 
void CGUI::Draw()
{
	bool ShouldDrawCursor = false;

	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
		{
			ShouldDrawCursor = true;
			if (window == Windows[1])
			{
				DrawWindow(window, 2);
			}
			else
			{
				DrawWindow(window, 1);
			}

		}
	}

	if (ShouldDrawCursor)
	{
		int red = Menu::Window.MenuTab.MenuR.GetValue();
		int green = Menu::Window.MenuTab.MenuG.GetValue();
		int blue = Menu::Window.MenuTab.MenuB.GetValue();


		Render::Clear(Mouse.x + 1, Mouse.y, 1, 17, Color(red, green, blue, 255));
		for (int i = 0; i < 11; i++)
			Render::Clear(Mouse.x + 2 + i, Mouse.y + 1 + i, 1, 1, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 8, Mouse.y + 12, 5, 1, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 8, Mouse.y + 13, 1, 1, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 9, Mouse.y + 14, 1, 2, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 10, Mouse.y + 16, 1, 2, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 8, Mouse.y + 18, 2, 1, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 7, Mouse.y + 16, 1, 2, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 6, Mouse.y + 14, 1, 2, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 5, Mouse.y + 13, 1, 1, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 4, Mouse.y + 14, 1, 1, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 3, Mouse.y + 15, 1, 1, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 2, Mouse.y + 16, 1, 1, Color(red, green, blue, 255));
		for (int i = 0; i < 4; i++)
			Render::Clear(Mouse.x + 2 + i, Mouse.y + 2 + i, 1, 14 - (i * 2), Color(red - (i * 4), green - (i * 4), blue - (i * 4), 255));
		Render::Clear(Mouse.x + 6, Mouse.y + 6, 1, 8, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 7, Mouse.y + 7, 1, 9, Color(red, green, blue, 255));
		for (int i = 0; i < 4; i++)
			Render::Clear(Mouse.x + 8 + i, Mouse.y + 8 + i, 1, 4 - i, Color(red - (i * 4), green - (i * 4), blue - (i * 4), 255));
		Render::Clear(Mouse.x + 8, Mouse.y + 14, 1, 4, Color(red, green, blue, 255));
		Render::Clear(Mouse.x + 9, Mouse.y + 16, 1, 2, Color(red, green, blue, 255));
	}
}

// Handle all input etc
void CGUI::Update()
{

	static int bWasntHolding = false;
	static int bGrabbing = false;
	static int iOffsetX = 0, iOffsetY = 0;
	//Key Array
	std::copy(keys, keys + 255, oldKeys);
	for (int x = 0; x < 255; x++)
	{
		//oldKeys[x] = oldKeys[x] & keys[x];
		keys[x] = (GetAsyncKeyState(x));
	}

	POINT mp; GetCursorPos(&mp);
	ScreenToClient(GetForegroundWindow(), &mp);
	Mouse.x = mp.x; Mouse.y = mp.y;

	RECT Screen = Render::GetViewport();

	// Window Binds
	for (auto& bind : WindowBinds)
	{
		if (GetKeyPress(bind.first))
		{
			bind.second->Toggle();
		}
	}

	// Stop dragging
	if (IsDraggingWindow && !GetKeyState(VK_LBUTTON))
	{
		IsDraggingWindow = false;
		DraggingWindow = nullptr;

	}

	// If we are in the proccess of dragging a window
	if (IsDraggingWindow && GetKeyState(VK_LBUTTON) && !GetKeyPress(VK_LBUTTON))
	{
		if (DraggingWindow)
		{
			DraggingWindow->m_x = Mouse.x - DragOffsetX;
			DraggingWindow->m_y = Mouse.y - DragOffsetY;
		}

	}


	//bWasntHolding = Input->Hovering(x, y, width, 28) && !GetAsyncKeyState(VK_LBUTTON);

	// Process some windows
	for (auto window : Windows)
	{
		if (window->m_bIsOpen)
			MenuAlpha = min(MenuAlpha + 6, 255);
		else
			MenuAlpha = max(MenuAlpha - 6, 0);

		if (window->m_bIsOpen)
		{
			// Used to tell the widget processing that there could be a click
			bool bCheckWidgetClicks = false;

			// If the user clicks inside the window
			if (GetKeyPress(VK_LBUTTON) || GetKeyPress(VK_RETURN))
			{
				//if (IsMouseInRegion(window->m_x, window->m_y, window->m_x + window->m_iWidth, window->m_y + window->m_iHeight))
				//{
				// Is it inside the client area?
				if (IsMouseInRegion(window->GetClientArea()))
				{
					// User is selecting a new tab
					if (IsMouseInRegion(window->GetTabArea()))
					{
						// Loose focus on the control
						window->IsFocusingControl = false;
						window->FocusedControl = nullptr;

						int iTab = 0;
						int TabCount = window->Tabs.size();
						if (TabCount) // If there are some tabs
						{
							int TabSize = (window->m_iWidth - 4 - 12) / TabCount;
							int Dist = Mouse.x - (window->m_x + 8);
							while (Dist > TabSize)
							{
								if (Dist > TabSize)
								{
									iTab++;
									Dist -= TabSize;
								}
							}
							window->SelectedTab = window->Tabs[iTab];

							// Loose focus on the control
							bCheckWidgetClicks = false;
							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;
						}

					}
					else
						bCheckWidgetClicks = true;
				}
				if (IsMouseInRegion(window->GetDragArea()))
				{
					// Must be in the around the title or side of the window
					// So we assume the user is trying to drag the window
					IsDraggingWindow = true;

					DraggingWindow = window;

					DragOffsetX = Mouse.x - window->m_x;
					DragOffsetY = Mouse.y - window->m_y;

					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}

				//else
				//{
				//    // Loose focus on the control
				//    window->IsFocusingControl = false;
				//    window->FocusedControl = nullptr;
				//}
			}


			// Controls
			if (window->SelectedTab != nullptr)
			{
				// Focused widget
				bool SkipWidget = false;
				CControl* SkipMe = nullptr;

				// this window is focusing on a widget??
				if (window->IsFocusingControl)
				{
					if (window->FocusedControl != nullptr)
					{
						// We've processed it once, skip it later
						SkipWidget = true;
						SkipMe = window->FocusedControl;

						POINT cAbs = window->FocusedControl->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, window->FocusedControl->m_iWidth, window->FocusedControl->m_iHeight };
						window->FocusedControl->OnUpdate();

						if (window->FocusedControl->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
						{
							window->FocusedControl->OnClick();

							// If it gets clicked we loose focus
							window->IsFocusingControl = false;
							window->FocusedControl = nullptr;
							bCheckWidgetClicks = false;
						}
					}
				}

				// Itterate over the rest of the control
				for (auto control : window->SelectedTab->Controls)
				{
					if (control != nullptr)
					{
						if (SkipWidget && SkipMe == control)
							continue;

						POINT cAbs = control->GetAbsolutePos();
						RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
						control->OnUpdate();

						if (control->Flag(UIFlags::UI_Clickable) && IsMouseInRegion(controlRect) && bCheckWidgetClicks)
						{
							control->OnClick();
							bCheckWidgetClicks = false;

							// Change of focus
							if (control->Flag(UIFlags::UI_Focusable))
							{
								window->IsFocusingControl = true;
								window->FocusedControl = control;
							}
							else
							{
								window->IsFocusingControl = false;
								window->FocusedControl = nullptr;
							}

						}
					}
				}

				// We must have clicked whitespace
				if (bCheckWidgetClicks)
				{
					// Loose focus on the control
					window->IsFocusingControl = false;
					window->FocusedControl = nullptr;
				}
			}
		}
	}
}


// Returns 
bool CGUI::GetKeyPress(unsigned int key)
{
	if (keys[key] == true && oldKeys[key] == false)
		return true;
	else
		return false;
}

bool CGUI::GetKeyState(unsigned int key)
{
	return keys[key];
}

bool CGUI::IsMouseInRegion(int x, int y, int x2, int y2)
{
	if (Mouse.x > x && Mouse.y > y && Mouse.x < x2 && Mouse.y < y2)
		return true;
	else
		return false;
}

bool CGUI::IsMouseInRegion(RECT region)
{
	return IsMouseInRegion(region.left, region.top, region.left + region.right, region.top + region.bottom);
}

POINT CGUI::GetMouse()
{
	return Mouse;
}


bool CGUI::DrawWindow(CWindow* window, int menu)
{

	static int Clock = 0;
	static clock_t start_t = clock();
	double timeSoFar = (double)(clock() - start_t) / CLOCKS_PER_SEC;
	Clock++;

	if (MenuAlpha == 0 || MenuAlpha == 255)
		if (GetKeyPress(VK_INSERT))
		{
			if (window->m_bIsOpen)
			{
				if (MenuAlpha != 255)
					if (MenuAlpha > 255)
						MenuAlpha = 255;
					else
						MenuAlpha = timeSoFar * 255.f;
			}
			else if (MenuAlpha != 0)
			{
				if (MenuAlpha < 0)
					MenuAlpha = 0;
				else
					MenuAlpha = 255 - timeSoFar * 255.f;
			}

			Clock = 0;
			window->m_bIsOpen = !window->m_bIsOpen;
			std::string msg = "cl_mouseenable " + std::to_string(!window->m_bIsOpen);
		}


	//int red = Menu::Window.MenuTab.MenuClearR.GetValue();
	//int green = Menu::Window.MenuTab.MenuClearG.GetValue();
	//int blue = Menu::Window.MenuTab.MenuClearB.GetValue();

	int r = Menu::Window.MenuTab.TabareaR.GetValue();
	int g = Menu::Window.MenuTab.TabareaG.GetValue();
	int b = Menu::Window.MenuTab.TabareaB.GetValue();
	
	int red = Menu::Window.MenuTab.MenuR.GetValue();
	int green = Menu::Window.MenuTab.MenuG.GetValue();
	int blue = Menu::Window.MenuTab.MenuB.GetValue();


	{
		//Inner
		//Inner
		//Draw::Outline(window->m_x + 7, window->m_y + 1 + 27 * 2, window->m_iWidth - 4 - 10, window->m_iHeight - 2 - 6 - 26, Color(20, 20, 20, 80));
		Render::GradientV(window->m_x + 8 - 120, window->m_y + 1 + 28 * 2, window->m_iWidth - 4 - 12 + 120, window->m_iHeight - 2 - 8 - 26, Color(20, 20, 20, MenuAlpha), Color(30, 30, 30, MenuAlpha));
		Render::Clear(window->m_x + 8 - 120, window->m_y + 1 + 28 * 2 - 6, window->m_iWidth - 4 - 12 + 120, 6, Color(40, 40, 40, MenuAlpha));
		Render::Clear(window->m_x + 8 - 120, window->m_y + 1 + 28 * 2 + window->m_iHeight - 2 - 8 - 26, window->m_iWidth - 4 - 12 + 120, 6, Color(40, 40, 40, MenuAlpha));
		Render::Clear(window->m_x + 8 - 120 - 6, window->m_y + 1 + 28 * 2 - 6, 6, window->m_iHeight - 2 - 8 - 26 + 12, Color(40, 40, 40, MenuAlpha));
		Render::Clear(window->m_x + 8, window->m_y + 1 + 28 * 2 - 6, 6, window->m_iHeight - 2 - 8 - 26 + 12, Color(40, 40, 40, MenuAlpha));
		Render::Clear(window->m_x + 8 + window->m_iWidth - 4 - 12, window->m_y + 1 + 28 * 2 - 6, 6, window->m_iHeight - 2 - 8 - 26 + 12, Color(40, 40, 40, MenuAlpha));

		//Tab
		Render::Clear(window->m_x + 8 - 120, window->m_y + 1 + 28 * 2, 120, window->m_iHeight - 2 - 8 - 26, Color(15, 15, 15, MenuAlpha));

		Render::Outline(window->m_x + 8 - 120, window->m_y + 1 + 28 * 2, window->m_iWidth - 4 - 12 + 120, window->m_iHeight - 2 - 8 - 26, Color(48, 48, 48, MenuAlpha));
		//Draw::Clear(window->m_x + 8, window->m_y + 1 + 28 * 2, 1, window->m_iHeight - 2 - 8 - 26 - 1, Color(129, 129, 129, MenuAlpha));
		Render::Outline(window->m_x + 8 - 6 - 120, window->m_y + 1 + 28 * 2 - 6, window->m_iWidth - 4 - 12 + 120 + 12, window->m_iHeight - 2 - 8 - 26 + 12, Color(48, 48, 48, MenuAlpha));
		Render::Outline(window->m_x + 8 - 120, window->m_y + 1 + 28 * 2, 120, window->m_iHeight - 2 - 8 - 26, Color(48, 48, 48, MenuAlpha));

		//int iProportion = window->m_iWidth - 4 - 12 + 90 - 2;
		//iProportion = iProportion / 3;
		//Draw::GradientH(window->m_x + 8 - 90 + 1, window->m_y + 1 + 28 * 2 + 1, iProportion, 2, Color(137, 207, 240, MenuAlpha), Color(147, 112, 219, MenuAlpha));
		//Draw::GradientH(window->m_x + 8 - 90 + 1 + iProportion, window->m_y + 1 + 28 * 2 + 1, iProportion, 2, Color(147, 112, 219, MenuAlpha), Color(245, 245, 5, MenuAlpha));
		//Draw::GradientH(window->m_x + 8 - 90 + 1 + (iProportion * 2), window->m_y + 1 + 28 * 2 + 1, iProportion, 2, Color(245, 245, 5, MenuAlpha), Color(173, MenuAlpha, 47, MenuAlpha));

		int TabCount = window->Tabs.size();
		if (TabCount) // If there are some tabs
		{
			bool isOut = true;

			for (int i = 0; i < TabCount; i++)
			{
				CTab *tab = window->Tabs[i];
				CTab *lBotTab = window->Tabs[0];
				CTab *rBotTab = window->Tabs[1];
				CTab *advancedTab = window->Tabs[2];
				CTab *vTab = window->Tabs[3];
				CTab *miscTab = window->Tabs[4];

				if (window->SelectedTab == lBotTab)
				{
					Menu::Window.SetSize(800, 547);
				}
				else if (window->SelectedTab == vTab)
				{
					Menu::Window.SetSize(800, 547);
				}
				else if (window->SelectedTab == rBotTab)
				{
					Menu::Window.SetSize(800, 547);
				}
				else if (window->SelectedTab == advancedTab)
				{
					Menu::Window.SetSize(800, 547);
				}
				else if (window->SelectedTab == miscTab)
				{
					Menu::Window.SetSize(800, 547);
				}
				else
				{
					Menu::Window.SetSize(800, 547);
				}

				float xAxis;
				float yAxis;
				float yWinPos = window->m_y + 1 + 28 * 2;
				float yWinHeight = window->m_iHeight - 2 - 8 - 26;

				yAxis = yWinPos + 16 + (i * 66) - 10;

				RECT TabDrawArea = { window->m_x + 8 - 120 + 1, yWinPos + 6 + (i * 63) - 8, 120 - 1, 71 };

				RECT TextSize;
				TextSize = Render::GetTextSize(Render::Fonts::Tabs, tab->Title.c_str());

				RECT ClickTabArea = { xAxis,
					yAxis,
					TextSize.right,
					TextSize.bottom };

				if (GetAsyncKeyState(VK_LBUTTON))
				{
					if (IsMouseInRegion(TabDrawArea))
					{
						window->SelectedTab = window->Tabs[i];
					}
				}

				if (window->SelectedTab == tab) {
					xAxis = window->m_x + 8 - (45 + TextSize.right / 2);
						Render::GradientV(TabDrawArea.left, TabDrawArea.top, TabDrawArea.right, TabDrawArea.bottom, Color(28, 28, 28, MenuAlpha), Color(40, 40, 40, MenuAlpha));
						Render::Line(TabDrawArea.left, TabDrawArea.top, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top, Color(48, 48, 48, 255));
						Render::Line(TabDrawArea.left, TabDrawArea.top + TabDrawArea.bottom, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top + TabDrawArea.bottom, Color(48, 48, 48, MenuAlpha));
					Render::Text(TabDrawArea.left + 15, TabDrawArea.top + 10, Color(red, green, blue, MenuAlpha), Render::Fonts::Tabs, tab->Title.c_str());
				}
				else {
					xAxis = window->m_x + 8 - (45 + TextSize.right / 2);
					Render::GradientV(TabDrawArea.left, TabDrawArea.top, TabDrawArea.right, TabDrawArea.bottom, Color(40, 40, 40, MenuAlpha), Color(28, 28, 28, MenuAlpha));
					Render::Line(TabDrawArea.left, TabDrawArea.top, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top, Color(48, 48, 48, 255));
					Render::Line(TabDrawArea.left, TabDrawArea.top + TabDrawArea.bottom, TabDrawArea.left + TabDrawArea.right, TabDrawArea.top + TabDrawArea.bottom, Color(48, 48, 48, MenuAlpha));
					Render::Text(TabDrawArea.left + 15, TabDrawArea.top + 10, Color(150, 150, 150, MenuAlpha), Render::Fonts::Tabs, tab->Title.c_str());
				}

				//Draw::Clear(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, 2, Color(65, 55, 55, MenuAlpha));
				//Render::Text(TabDrawArea.left + 32, TabDrawArea.top + 8, Color(255, 255, 255, 255), Render::Fonts::Tabs, tab->Title.c_str());
			}
		}
	

		//Draw::Outline(window->m_x)
		// Controls 
		if (window->SelectedTab != nullptr)
		{
			// Focused widget
			bool SkipWidget = false;
			CControl* SkipMe = nullptr;

			// this window is focusing on a widget??
			if (window->IsFocusingControl)
			{
				if (window->FocusedControl != nullptr)
				{
					// We need to draw it last, so skip it in the regular loop
					SkipWidget = true;
					SkipMe = window->FocusedControl;
				}
			}


			// Itterate over all the other controls
			for (auto control : window->SelectedTab->Controls)
			{
				if (SkipWidget && SkipMe == control)
					continue;

				if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
				{
					POINT cAbs = control->GetAbsolutePos();
					RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
					bool hover = false;
					if (IsMouseInRegion(controlRect))
					{
						hover = true;
					}
					control->Draw(hover);
				}
			}

			// Draw the skipped widget last
			if (SkipWidget)
			{
				auto control = window->FocusedControl;

				if (control != nullptr && control->Flag(UIFlags::UI_Drawable))
				{
					POINT cAbs = control->GetAbsolutePos();
					RECT controlRect = { cAbs.x, cAbs.y, control->m_iWidth, control->m_iHeight };
					bool hover = false;
					if (IsMouseInRegion(controlRect))
					{
						hover = true;
					}
					control->Draw(hover);
				}
			}

		}


		{

			static int Clock = 0;
			double timeSoFar1 = (double)Clock / 70.f;
			Clock++;
			if (MenuAlpha == 0 || MenuAlpha == 255)
				if (GetKeyPress(VK_INSERT))
				{
					if (window->m_bIsOpen)
					{
						if (MenuAlpha != 255)
							if (MenuAlpha > 255)
								MenuAlpha = 255;
							else
								MenuAlpha = timeSoFar1 * 255.f;
					}
					else if (MenuAlpha != 0)
					{
						if (MenuAlpha < 0)
							MenuAlpha = 0;
						else
							MenuAlpha = 255 - (timeSoFar1 * 255.f);
					}

					Clock = 0;
					window->m_bIsOpen = !window->m_bIsOpen;
					std::string msg = "cl_mouseenable " + std::to_string(!window->m_bIsOpen);
				}



			return true;
		}
	}
}






bool CGUI::DrawRadar() {

	// Main Window
	Render::Clear(Radar->m_x + 8 - 90, Radar->m_y + 1 + 28 * 2, Radar->m_iWidth - 4 - 12 + 90, Radar->m_iHeight - 2 - 8 - 26, Color(28, 28, 28, MenuAlpha));
	Render::Clear(Radar->m_x + 8 - 90, Radar->m_y + 1 + 28 * 2 - 6, Radar->m_iWidth - 4 - 12 + 90, 6, Color(40, 40, 40, MenuAlpha));
	Render::Clear(Radar->m_x + 8 - 90, Radar->m_y + 1 + 28 * 2 + Radar->m_iHeight - 2 - 8 - 26, Radar->m_iWidth - 4 - 12 + 90, 6, Color(40, 40, 40, MenuAlpha));
	Render::Clear(Radar->m_x + 8 - 90 - 6, Radar->m_y + 1 + 28 * 2 - 6, 6, Radar->m_iHeight - 2 - 8 - 26 + 12, Color(40, 40, 40, MenuAlpha));
	Render::Clear(Radar->m_x + 8 + Radar->m_iWidth - 4 - 12, Radar->m_y + 1 + 28 * 2 - 6, 6, Radar->m_iHeight - 2 - 8 - 26 + 12, Color(40, 40, 40, MenuAlpha));

	Render::Clear(m_position.x, m_position.y, m_size.x, m_size.y, Color(40, 40, 40, 200));
	Render::Clear(m_position.x + m_size.x / 2, m_position.y, 1, m_size.y, Color(0, 0, 0, 200));
	Render::Clear(m_position.x, m_position.y + m_size.y / 2, m_size.x, 1, Color(0, 0, 0, 200));

	//Inner
	Render::Outline(Radar->m_x + 7, Radar->m_y + 1 + 26, Radar->m_iWidth - 4 - 10, Radar->m_iHeight - 2 - 6 - 26, Color(121, 181, 0, 255));
	Render::Clear(Radar->m_x + 8, Radar->m_y + 1 + 27, Radar->m_iWidth - 4 - 12, Radar->m_iHeight - 2 - 8 - 26, Color(0, 0, 0, 255));
	//D::DrawRectRainbow(window->m_x + 8, window->m_y + 1 + 27, window->m_iWidth - 4 - 12, 2, 0.003f, rainbow);

	if (!Interfaces::Engine->IsInGame()) {
		//Draw info
		//Render::Text(Radar->m_x + 1, Radar->m_y + 1, Color(255, 255, 255, 255), Render::Fonts::MenuBold, " ");
		return false;
	}

	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 2, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 2, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 12, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 12, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 22, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 22, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 32, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 32, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 42, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 42, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 52, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 52, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 62, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 62, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 72, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 72, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 82, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 82, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 92, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 92, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 102, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 102, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 112, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 112, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 122, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 122, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 132, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 132, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 142, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 142, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 152, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 152, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 162, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 162, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 172, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 172, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 182, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 182, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 192, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 192, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 202, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 202, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 212, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 212, Color(68, 68, 68, 255));
	//Render::Line(Radar->m_x + 7, Radar->m_y + 28 + 222, Radar->m_x + Radar->m_iWidth - 7, Radar->m_y + 28 + 222, Color(255, 255, 255, 50));

	Render::Line(Radar->m_x + 10, Radar->m_y + 28, Radar->m_x + 10, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 20, Radar->m_y + 28, Radar->m_x + 20, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 30, Radar->m_y + 28, Radar->m_x + 30, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 40, Radar->m_y + 28, Radar->m_x + 40, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 50, Radar->m_y + 28, Radar->m_x + 50, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 60, Radar->m_y + 28, Radar->m_x + 60, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 70, Radar->m_y + 28, Radar->m_x + 70, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 80, Radar->m_y + 28, Radar->m_x + 80, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 90, Radar->m_y + 28, Radar->m_x + 90, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 100, Radar->m_y + 28, Radar->m_x + 100, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 110, Radar->m_y + 28, Radar->m_x + 110, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 120, Radar->m_y + 28, Radar->m_x + 120, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 130, Radar->m_y + 28, Radar->m_x + 130, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 140, Radar->m_y + 28, Radar->m_x + 140, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 150, Radar->m_y + 28, Radar->m_x + 150, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 160, Radar->m_y + 28, Radar->m_x + 160, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 170, Radar->m_y + 28, Radar->m_x + 170, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 180, Radar->m_y + 28, Radar->m_x + 180, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 190, Radar->m_y + 28, Radar->m_x + 190, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 200, Radar->m_y + 28, Radar->m_x + 200, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 210, Radar->m_y + 28, Radar->m_x + 210, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 220, Radar->m_y + 28, Radar->m_x + 220, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 230, Radar->m_y + 28, Radar->m_x + 230, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	Render::Line(Radar->m_x + 240, Radar->m_y + 28, Radar->m_x + 240, Radar->m_y + Radar->m_iHeight - 8, Color(68, 68, 68, 255));
	//Render::Line(Radar->m_x + 1250, Radar->m_y + 28, Radar->m_x + 250, Radar->m_y + Radar->m_iHeight - 8, Color(255, 255, 255, 50));

	// Render::Line(Radar->m_x + 18, Radar->m_y + 28, Radar->m_x + 100, Radar->m_y + 28 + 82, Color(255, 255, 255, 255));
	// Render::Line(Radar->m_x + 100, Radar->m_y + 28 + 82, Radar->m_x + Radar->m_iWidth - 17, Radar->m_y + 26, Color(255, 255, 255, 255));
	Render::Line(Radar->m_x + 8, Radar->m_y + 6 + 122, Radar->m_x + Radar->m_iWidth - 8, Radar->m_y + 6 + 122, Color(155, 155, 155, 255));
	Render::Line(Radar->m_x + 122, Radar->m_y + 30, Radar->m_x + 122, Radar->m_y + Radar->m_iHeight - 12, Color(155, 155, 155, 255));


	IClientEntity* LocalPlayer = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());
	if (LocalPlayer) {
		auto flAngle(LocalPlayer->GetAbsAngles2().y);

		for (size_t EntityID = 1; EntityID < 64; EntityID++) {
			IClientEntity* Entity = Interfaces::EntList->GetClientEntity(EntityID);
			if (!Entity || Entity == LocalPlayer || !Entity->IsAlive() || Entity->IsDormant())
				continue;

			if (Menu::Window.VisualsTab.OtherRadarEnemyOnly.GetState()) {
				if (LocalPlayer->GetTeamNum() == Entity->GetTeamNum())
					continue;
			}

			bool Visible = true;

			if (Menu::Window.VisualsTab.OtherRadarVisibleOnly.GetState()) {
				Visible = false;

				if (GameUtils::IsVisible(LocalPlayer, Entity, (int)CSPlayerBones::head_0) ||
					GameUtils::IsVisible(LocalPlayer, Entity, (int)CSPlayerBones::arm_upper_L) ||
					GameUtils::IsVisible(LocalPlayer, Entity, (int)CSPlayerBones::arm_upper_R) ||
					GameUtils::IsVisible(LocalPlayer, Entity, (int)CSPlayerBones::hand_L) ||
					GameUtils::IsVisible(LocalPlayer, Entity, (int)CSPlayerBones::hand_R)) {
					Visible = true;
				}
			}

			int CenterX = Radar->m_x + Radar->m_iWidth / 2;
			int CenterY = Radar->m_y + Radar->m_iHeight / 2;

			float DeltaX = Entity->GetOrigin2().x - LocalPlayer->GetOrigin2().x;
			float DeltaY = Entity->GetOrigin2().y - LocalPlayer->GetOrigin2().y;

			float Yaw = (flAngle) * (PI / 180.0);
			float MainViewAngles_CosYaw = cos(Yaw);
			float MainViewAngles_SinYaw = sin(Yaw);

			float x = DeltaY * (-MainViewAngles_CosYaw) + DeltaX * MainViewAngles_SinYaw;
			float y = DeltaX * (-MainViewAngles_CosYaw) - DeltaY * MainViewAngles_SinYaw;

			float Scale = Menu::Window.VisualsTab.OtherRadarScale.GetValue();

			if (fabs(x) > Scale || fabs(y) > Scale) {
				if (y > x) {
					if (y > -x) {
						x = Scale * x / y;
						y = Scale;
					}
					else {
						y = -Scale * y / x;
						x = -Scale;
					}
				}
				else {
					if (y > -x) {
						y = Scale *y / x;
						x = Scale;
					}
					else {
						x = -Scale * x / y;
						y = -Scale;
					}
				}
			}

			int	ScreenX = CenterX + int(x / Scale * 98);
			int ScreenY = CenterY + int(y / Scale * 98);

			Color _Color;

			switch (Entity->GetTeamNum()) {
			case TEAM_CS_T:
				if (Visible) {
					_Color.SetColor(Menu::Window.ColorsTab.TColorVisR.GetValue(), Menu::Window.ColorsTab.TColorVisG.GetValue(), Menu::Window.ColorsTab.TColorVisB.GetValue(), 255);
					Render::Clear(ScreenX - 2, ScreenY - 2, 5, 5, _Color);
				}
				else {
					_Color.SetColor(Menu::Window.ColorsTab.TColorNoVisB.GetValue(), Menu::Window.ColorsTab.TColorNoVisG.GetValue(), Menu::Window.ColorsTab.TColorNoVisB.GetValue(), 255);
					Render::Clear(ScreenX - 2, ScreenY - 2, 5, 5, _Color);
				}
				break;

			case TEAM_CS_CT:
				if (Visible) {
					_Color.SetColor(Menu::Window.ColorsTab.CTColorVisR.GetValue(), Menu::Window.ColorsTab.CTColorVisG.GetValue(), Menu::Window.ColorsTab.CTColorVisB.GetValue(), 255);
					Render::Clear(ScreenX - 2, ScreenY - 2, 5, 5, _Color);
				}
				else {
					_Color.SetColor(Menu::Window.ColorsTab.CTColorNoVisR.GetValue(), Menu::Window.ColorsTab.CTColorNoVisG.GetValue(), Menu::Window.ColorsTab.CTColorNoVisB.GetValue(), 255);
					Render::Clear(ScreenX - 2, ScreenY - 2, 5, 5, _Color);
				}
				break;
			}
		}
	}



	return true;
}

void CGUI::RegisterWindow(CWindow* window)
{
	Windows.push_back(window);

	// Resorting to put groupboxes at the start
	for (auto tab : window->Tabs)
	{
		for (auto control : tab->Controls)
		{
			if (control->Flag(UIFlags::UI_RenderFirst))
			{
				CControl * c = control;
				tab->Controls.erase(std::remove(tab->Controls.begin(), tab->Controls.end(), control), tab->Controls.end());
				tab->Controls.insert(tab->Controls.begin(), control);
			}
		}
	}
}

void CGUI::BindWindow(unsigned char Key, CWindow* window)
{
	if (window)
		WindowBinds[Key] = window;
	else
		WindowBinds.erase(Key);
}

void CGUI::SaveWindowState(CWindow* window, std::string Filename)
{
	// Create a whole new document and we'll just save over top of the old one
	tinyxml2::XMLDocument Doc;

	// Root Element is called "ayy"
	tinyxml2::XMLElement *Root = Doc.NewElement("ayy");
	Doc.LinkEndChild(Root);

	Utilities::Log("Saving Window %s", window->Title.c_str());

	// If the window has some tabs..
	if (Root && window->Tabs.size() > 0)
	{
		for (auto Tab : window->Tabs)
		{
			// Add a new section for this tab
			tinyxml2::XMLElement *TabElement = Doc.NewElement(Tab->Title.c_str());
			Root->LinkEndChild(TabElement);

			Utilities::Log("Saving Tab %s", Tab->Title.c_str());

			// Now we itterate the controls this tab contains
			if (TabElement && Tab->Controls.size() > 0)
			{
				for (auto Control : Tab->Controls)
				{
					// If the control is ok to be saved
					if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
					{
						// Create an element for the control
						tinyxml2::XMLElement *ControlElement = Doc.NewElement(Control->FileIdentifier.c_str());
						TabElement->LinkEndChild(ControlElement);

						Utilities::Log("Saving control %s", Control->FileIdentifier.c_str());

						if (!ControlElement)
						{
							Utilities::Log("Errorino :("); // s0 cute
							return;
						}

						CCheckBox* cbx = nullptr;
						CComboBox* cbo = nullptr;
						CKeyBind* key = nullptr;
						CSlider* sld = nullptr;

						// Figure out what kind of control and data this is
						switch (Control->FileControlType)
						{
						case UIControlTypes::UIC_CheckBox:
							cbx = (CCheckBox*)Control;
							ControlElement->SetText(cbx->GetState());
							break;
						case UIControlTypes::UIC_ComboBox:
							cbo = (CComboBox*)Control;
							ControlElement->SetText(cbo->GetIndex());
							break;
						case UIControlTypes::UIC_KeyBind:
							key = (CKeyBind*)Control;
							ControlElement->SetText(key->GetKey());
							break;
						case UIControlTypes::UIC_Slider:
							sld = (CSlider*)Control;
							ControlElement->SetText(sld->GetValue());
							break;
						}
					}
				}
			}
		}
	}

	//Save the file
	if (Doc.SaveFile(Filename.c_str()) != tinyxml2::XML_NO_ERROR)
	{
		MessageBox(NULL, "Failed To Save Config File!", "FlexBeta", MB_OK);
	}

}

void CGUI::LoadWindowState(CWindow* window, std::string Filename)
{
	// Lets load our meme
	tinyxml2::XMLDocument Doc;
	if (Doc.LoadFile(Filename.c_str()) == tinyxml2::XML_NO_ERROR)
	{
		tinyxml2::XMLElement *Root = Doc.RootElement();

		// The root "ayy" element
		if (Root)
		{
			// If the window has some tabs..
			if (Root && window->Tabs.size() > 0)
			{
				for (auto Tab : window->Tabs)
				{
					// We find the corresponding element for this tab
					tinyxml2::XMLElement *TabElement = Root->FirstChildElement(Tab->Title.c_str());
					if (TabElement)
					{
						// Now we itterate the controls this tab contains
						if (TabElement && Tab->Controls.size() > 0)
						{
							for (auto Control : Tab->Controls)
							{
								// If the control is ok to be saved
								if (Control && Control->Flag(UIFlags::UI_SaveFile) && Control->FileIdentifier.length() > 1 && Control->FileControlType)
								{
									// Get the controls element
									tinyxml2::XMLElement *ControlElement = TabElement->FirstChildElement(Control->FileIdentifier.c_str());

									if (ControlElement)
									{
										CCheckBox* cbx = nullptr;
										CComboBox* cbo = nullptr;
										CKeyBind* key = nullptr;
										CSlider* sld = nullptr;

										// Figure out what kind of control and data this is
										switch (Control->FileControlType)
										{
										case UIControlTypes::UIC_CheckBox:
											cbx = (CCheckBox*)Control;
											cbx->SetState(ControlElement->GetText()[0] == '1' ? true : false);
											break;
										case UIControlTypes::UIC_ComboBox:
											cbo = (CComboBox*)Control;
											cbo->SelectIndex(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_KeyBind:
											key = (CKeyBind*)Control;
											key->SetKey(atoi(ControlElement->GetText()));
											break;
										case UIControlTypes::UIC_Slider:
											sld = (CSlider*)Control;
											sld->SetValue(atof(ControlElement->GetText()));
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}