#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"

using namespace game_framework;

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g) : CGameState(g)
{
}

CGameStateRun::~CGameStateRun()
{
}

void CGameStateRun::OnBeginState()
{
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{	
	char stage_map[16][28] = { {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
							 {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
							 {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'A', 'A', 'C', 'B', 'B', 'B', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C', 'A', 'A'},
							 {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'A', 'A', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B'},
							 {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'A', 'A', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B'},
							 {'A', 'A', 'C', 'A', 'A', 'A', 'A', 'A', 'B', 'B', 'B', 'B', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C', 'A', 'A', 'A', 'A', 'A', 'A', 'A'},
							 {'B', 'B', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
							 {'B', 'B', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
							 {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
							 {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
							 {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B'},
							 {'B', 'B', 'B', 'B', 'C', 'A', 'A', 'A', 'A', 'A', 'A', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C'},
							 {'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C'},
							 {'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'A'},
							 {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'A', 'A', 'C', 'B', 'B', 'B', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C', 'A', 'A'},
							 {'B', 'B', 'B', 'B', 'B', 'B', 'B', 'C', 'B', 'B', 'B', 'B', 'A', 'A', 'C', 'B', 'B', 'B', 'A', 'A', 'A', 'A', 'A', 'A', 'A', 'C', 'A', 'A'}
							};
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 28; j++) {
			switch (stage_map[i][j]) {
			case 'A':
				stage[i][j].LoadBitmapByString({ "resources/brick.bmp" });
				break;
			case 'B':
				stage[i][j].LoadBitmapByString({ "resources/black.bmp" });
				break;
			case 'C':
				stage[i][j].LoadBitmapByString({ "resources/ladder.bmp" });
				break;
			case 'D':
				stage[i][j].LoadBitmapByString({ "resources/rope.bmp" });
				break;
			default:
				break;
			}
			
			stage[i][j].SetTopLeft(j * 40, 80 + i * 44);
		}
	}
	character.LoadBitmapByString({ 
								"resources/character/redhat_01.bmp",
								"resources/character/redhat_02.bmp",
								"resources/character/redhat_03.bmp",
								"resources/character/redhat_04.bmp",
								"resources/character/redhat_05.bmp",
								"resources/character/redhat_06.bmp",
								"resources/character/redhat_07.bmp",
								"resources/character/redhat_08.bmp",
								"resources/character/redhat_09.bmp",
								"resources/character/redhat_10.bmp",
								"resources/character/redhat_11.bmp",
								"resources/character/redhat_12.bmp",
								"resources/character/redhat_13.bmp",
								"resources/character/redhat_14.bmp",
								"resources/character/redhat_15.bmp",
								"resources/character/redhat_16.bmp",
								"resources/character/redhat_17.bmp",
								"resources/character/redhat_18.bmp",
								"resources/character/redhat_19.bmp",
								"resources/character/redhat_20.bmp",
								"resources/character/redhat_21.bmp",
								"resources/character/redhat_22.bmp"
	}, RGB(255, 255, 255));
	character.SetTopLeft(50, 44 * 14 - 10);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	
}

void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
}

void CGameStateRun::OnShow()
{
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 28; j++) {
			stage[i][j].ShowBitmap();
		}
	}
	character.ShowBitmap();
	if (character.GetFrameIndexOfBitmap() == 0) {
		character.SetFrameIndexOfBitmap(1);
	}
	else if (character.GetFrameIndexOfBitmap() == 1) {
		character.SetFrameIndexOfBitmap(2);
	}
	else if (character.GetFrameIndexOfBitmap() == 2) {
		character.SetFrameIndexOfBitmap(0);
	}
}
