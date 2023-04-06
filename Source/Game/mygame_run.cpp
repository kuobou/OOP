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
	/*int stage_map[16][28] = { {1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
							 {2, 2, 2, 2, 2, 2, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
							 {2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 1, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1},
							 {2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2},
							 {2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2},
							 {1, 1, 3, 1, 1, 1, 1, 1, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1, 1, 1, 1, 1, 1},
							 {2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2},
							 {2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2},
							 {1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2},
							 {2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2},
							 {2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 2, 2, 2, 2, 2, 2, 2},
							 {2, 2, 2, 2, 3, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3},
							 {2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3},
							 {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
							 {2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 1, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1},
							 {2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 1, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 3, 1, 1}
	};*/                                //1 是方塊 2是黑色的 3是梯子 4是繩索
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 28; j++) {
			switch (stage_map[i][j]) {
			case 1:
				stage[i][j].LoadBitmapByString({ "resources/brick.bmp" });
				break;
			case 2:
				stage[i][j].LoadBitmapByString({ "resources/black.bmp" });
				break;
			case 3:
				stage[i][j].LoadBitmapByString({ "resources/ladder.bmp" });
				break;
			case 4:
				stage[i][j].LoadBitmapByString({ "resources/rope.bmp" });
				break;
			default:
				break;
			}

			stage[i][j].SetTopLeft(j * 40, 80 + i * 44);
		}
	}
	character[0].LoadBitmapByString({
								"resources/character/redhat_01.bmp",
								"resources/character/redhat_02.bmp",
								"resources/character/redhat_03.bmp"/*,
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
								"resources/character/redhat_22.bmp"*/   //爬右
	}, RGB(255, 255, 255));
	character[1].LoadBitmapByString({
								"resources/character/redhat_04.bmp",
								"resources/character/redhat_05.bmp",
								"resources/character/redhat_06.bmp",    //爬左
		}, RGB(255, 255, 255));
	character[2].LoadBitmapByString({
								"resources/character/redhat_07.bmp",
								"resources/character/redhat_08.bmp",    //梯子
		}, RGB(255, 255, 255));
	character[3].LoadBitmapByString({
								"resources/character/redhat_09.bmp",
								"resources/character/redhat_10.bmp",
								"resources/character/redhat_11.bmp",    //繩索
		}, RGB(255, 255, 255));
	character[0].SetTopLeft(50, 44 * 14 - 10);
	character[1].SetTopLeft(50, 44 * 14 - 10);
	character[2].SetTopLeft(50, 44 * 14 - 10);
	character[3].SetTopLeft(50, 44 * 14 - 10);

	monster1[0].LoadBitmapByString({ "resources/monster1/01.bmp",
								     "resources/monster1/02.bmp",
								     "resources/monster1/03.bmp",
		}, RGB(0, 0, 0));
	monster1[1].LoadBitmapByString({ "resources/monster1/04.bmp",
							         "resources/monster1/05.bmp",
							         "resources/monster1/06.bmp",
		}, RGB(0, 0, 0));
	monster1[2].LoadBitmapByString({ "resources/monster1/07.bmp",
							         "resources/monster1/08.bmp",
		}, RGB(0, 0, 0));
	monster1[3].LoadBitmapByString({ "resources/monster1/09.bmp",
							         "resources/monster1/10.bmp",
							         "resources/monster1/11.bmp",
		}, RGB(0, 0, 0));
	monster1[0].SetTopLeft(130, 44 * 9 - 10);
	monster1[1].SetTopLeft(130, 44 * 9 - 10);
	monster1[2].SetTopLeft(130, 44 * 9 - 10);
	monster1[3].SetTopLeft(130, 44 * 9 - 10);

	monster2[0].LoadBitmapByString({ "resources/monster2/01.bmp",
								     "resources/monster2/02.bmp",
								     "resources/monster2/03.bmp",
		}, RGB(0, 0, 0));
	monster2[1].LoadBitmapByString({ "resources/monster2/04.bmp",
									 "resources/monster2/05.bmp",
									 "resources/monster2/06.bmp",
		}, RGB(0, 0, 0));
	monster2[2].LoadBitmapByString({ "resources/monster2/07.bmp",
									 "resources/monster2/08.bmp",
		}, RGB(0, 0, 0));
	monster2[3].LoadBitmapByString({ "resources/monster2/09.bmp",
									 "resources/monster2/10.bmp",
									 "resources/monster2/11.bmp",
		}, RGB(0, 0, 0));
	monster2[0].SetTopLeft(170, 44 * 9 - 10);
	monster2[1].SetTopLeft(170, 44 * 9 - 10);
	monster2[2].SetTopLeft(170, 44 * 9 - 10);
	monster2[3].SetTopLeft(170, 44 * 9 - 10);

	monster3[0].LoadBitmapByString({ "resources/monster3/01.bmp",
							         "resources/monster3/02.bmp",
							         "resources/monster3/03.bmp",
		}, RGB(0, 0, 0));
	monster3[1].LoadBitmapByString({ "resources/monster3/04.bmp",
									 "resources/monster3/05.bmp",
									 "resources/monster3/06.bmp",
		}, RGB(0, 0, 0));
	monster3[2].LoadBitmapByString({ "resources/monster3/07.bmp",
									 "resources/monster3/08.bmp",
		}, RGB(0, 0, 0));
	monster3[3].LoadBitmapByString({ "resources/monster3/09.bmp",
									 "resources/monster3/10.bmp",
									 "resources/monster3/11.bmp",
		}, RGB(0, 0, 0));
	monster3[0].SetTopLeft(200, 44 * 9 - 10);
	monster3[1].SetTopLeft(200, 44 * 9 - 10);
	monster3[2].SetTopLeft(200, 44 * 9 - 10);
	monster3[3].SetTopLeft(200, 44 * 9 - 10);

}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT || nChar == 0x41) {
		stopanime = true;
		character[1].SetTopLeft(character[num[0]].GetLeft() - speed, character[num[0]].GetTop());
		if (num[0] == 1) {
			if (character[num[0]].GetFrameIndexOfBitmap() == 2) {
				character[num[0]].SetFrameIndexOfBitmap(0);
			}
			else {
				character[num[0]].SetFrameIndexOfBitmap(character[num[0]].GetFrameIndexOfBitmap() + 1);
			}
		}
		num[0] = 1;
		
	}
	else if (nChar == VK_DOWN || nChar == 0x53) 
	{
		int x = character[num[0]].GetLeft() + character[num[0]].GetWidth() / 2;
		int y = character[num[0]].GetTop() + character[num[0]].GetHeight() + speed -18;
		if (stage_map[(y - 80) / 44][x / 40] == 3) {
			stopanime = true;
			character[2].SetTopLeft(character[num[0]].GetLeft(), character[num[0]].GetTop() + speed);
			if (num[0] == 2) {
				if (character[num[0]].GetFrameIndexOfBitmap() == 1) {
					character[num[0]].SetFrameIndexOfBitmap(0);
				}
				else {
					character[num[0]].SetFrameIndexOfBitmap(character[num[0]].GetFrameIndexOfBitmap() + 1);
				}
			}
			num[0] = 2;
		}
	}
	else if (nChar == VK_RIGHT || nChar == 0x44) {
		stopanime = true;
		character[0].SetTopLeft(character[num[0]].GetLeft() + speed, character[num[0]].GetTop());
		if (num[0] == 0) {
			if (character[num[0]].GetFrameIndexOfBitmap() == 2) {
				character[num[0]].SetFrameIndexOfBitmap(0);
			}
			else {
				character[num[0]].SetFrameIndexOfBitmap(character[num[0]].GetFrameIndexOfBitmap() + 1);
			}
		}
		num[0] = 0;
		
	}
	else if (nChar == VK_UP || nChar == 0x57)
	{
		
	int x = character[num[0]].GetLeft() + character[num[0]].GetWidth()/2;
	int y = character[num[0]].GetTop() + character[num[0]].GetHeight() - speed-18;
		if (stage_map[(y - 80) / 44][x / 40] == 3) 
		{
			stopanime = true;
			character[2].SetTopLeft(character[num[0]].GetLeft(), character[num[0]].GetTop() - speed);
			if (num[0] == 2) 
			{
				if (character[num[0]].GetFrameIndexOfBitmap() == 1) 
				{
					character[num[0]].SetFrameIndexOfBitmap(0);
				}
				else 
				{
					character[num[0]].SetFrameIndexOfBitmap(character[num[0]].GetFrameIndexOfBitmap() + 1);
				}
			}
			num[0] = 2;
		}
	}

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
	//CDC *pDC = CDDraw::GetBackCDC();
	//CTextDraw::Print(pDC, 300, 600, "Some text here.");
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 28; j++) {
			stage[i][j].ShowBitmap();
		}
	}
	character[num[0]].ShowBitmap();
	character[num[0]].SetAnimation(50, stopanime);

	monster1[num[1]].ShowBitmap();
	monster1[num[1]].SetAnimation(50, false);
	monster2[0].ShowBitmap();
	monster2[0].SetAnimation(50, false);
	
	
	monster3[0].ShowBitmap();
	monster3[0].SetAnimation(50, false);
	//int x = character[num[0]].GetLeft() + character[num[0]].GetWidth()/2;
	//int y = character[num[0]].GetTop() + character[num[0]].GetHeight()/2;
	//if (stage_map[(y - 80) / 44][x / 40] == 1) {
		//monster1[num[1]].SetTopLeft(monster1[num[1]].GetLeft(), monster1[num[1]].GetTop() - 5);
}
	
	/*int x = character[num[0]].GetLeft() + character[num[0]].GetWidth() //右邊
	int x = character[num[0]].GetLeft()  //左邊
	int y = character[num[0]].GetTop() + character[num[0]].GetHeight()  //底部
	int y = character[num[0]].GetTop()  //頭頂
	*/




