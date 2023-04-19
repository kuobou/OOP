#include "stdafx.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "../Library/audio.h"
#include "../Library/gameutil.h"
#include "../Library/gamecore.h"
#include "mygame.h"
#include <fstream>
#include <string>
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
	if (direction == 0) {
		int left, right, x;
		downstair[0] = -1;
		downstair[1] = -1;
		upstair[0] = -1;
		upstair[1] = -1;		
		if (monster1.GetTop() != character.GetTop() && character.GetTop() % 44 == 0) {
			x = monster1.GetLeft() / 40;
			left = x - 1;
			right = x + 1;
			while (monster1.GetLeft() > 0 && left >= 0) {
				if (map[monster1.GetTop() / 44][left] == 3 || map[monster1.GetTop() / 44 + 1][left] != 1) {
					if (map[monster1.GetTop() / 44][left] == 2) {
						upstair[0] = left;
					}
					if (map[monster1.GetTop() / 44 + 1][left] == 2) {
						downstair[0] = left;
					}
					left--;
				}
				else {
					break;
				}
			}
			while (monster1.GetLeft() <= 27 * 40 && right < 28) {
				if (map[monster1.GetTop() / 44][right] == 3 || map[monster1.GetTop() / 44 + 1][right] != 1) {
					if (map[monster1.GetTop() / 44][right] == 2) {
						upstair[1] = right;
					}
					if (map[monster1.GetTop() / 44 + 1][right] == 2) {
						downstair[1] = right;
					}
					right++;
				}
				else {
					break;
				}
			}
			if (monster1.GetTop() > character.GetTop()) {
				if (map[monster1.GetTop() / 44][x] == 2) {
					upstair[0] = x;
				}
				if (map[monster1.GetTop() / 44][x] == 2 && monster1.GetLeft() % 40 == 0) {
					monster1.SetTopLeft(monster1.GetLeft(), monster1.GetTop() - speed_y);
					direction = 1;
				}
				else if (upstair[0] != -1 || upstair[1] != -1) {
					if (upstair[0] != -1 && upstair[1] != -1) {
						if (character.GetLeft() > monster1.GetLeft()) {
							monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
							direction = 4;
						}
						else {
							monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
							direction = 3;
						}
					}
					else if (upstair[0] != -1) {
						monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
						direction = 3;
					}
					else {
						monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
						direction = 4;
					}
				}
				else {
					if (map[monster1.GetTop() / 44 + 1][x] == 2 && monster1.GetLeft() % 40 == 0) {
						monster1.SetTopLeft(monster1.GetLeft(), monster1.GetTop() + speed_y);
						direction = 2;
					}
					else if (downstair[0] != -1 || downstair[1] != -1) {
						if (downstair[0] != -1 && downstair[1] != -1) {
							if (character.GetLeft() > monster1.GetLeft()) {
								monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
								direction = 4;
							}
							else {
								monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
								direction = 3;
							}
						}
						else if (downstair[0] != -1) {
							monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
							direction = 3;
						}
						else {
							monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
							direction = 4;
						}
					}
				}
			}
			else {
				if (map[monster1.GetTop() / 44 + 1][x] == 2) {
					downstair[0] = x;
				}
				if (map[monster1.GetTop() / 44 + 1][x] == 2 && monster1.GetLeft() % 40 == 0) {
					monster1.SetTopLeft(monster1.GetLeft(), monster1.GetTop() + speed_y);
					direction = 2;
				}
				else if (downstair[0] != -1 || downstair[1] != -1) {
					if (downstair[0] != -1 && downstair[1] != -1) {
						if (character.GetLeft() > monster1.GetLeft()) {
							monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
							direction = 4;
						}
						else {
							monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
							direction = 3;
						}
					}
					else if (downstair[0] != -1) {
						monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
						direction = 3;
					}
					else {
						monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
						direction = 4;
					}
				}
				else {
					if (map[monster1.GetTop() / 44][x] == 2 && monster1.GetLeft() % 40 == 0) {
						monster1.SetTopLeft(monster1.GetLeft(), monster1.GetTop() - speed_y);
						direction = 1;
					}
					else if (upstair[0] != -1 || upstair[1] != -1) {
						if (upstair[0] != -1 && upstair[1] != -1) {
							if (character.GetLeft() > monster1.GetLeft()) {
								monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
								direction = 4;
							}
							else {
								monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
								direction = 3;
							}
						}
						else if (upstair[0] != -1) {
							monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
							direction = 3;
						}
						else {
							monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
							direction = 4;
						}
					}
				}
			}
		}
		else if(character.GetTop() % 44 == 0){
			if (monster1.GetLeft() > character.GetLeft()) {
				monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
			}
			else if (monster1.GetLeft() < character.GetLeft()) {
				monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
			}
			else {
				monster1.SetTopLeft(12 * 40, 44 * 3);
				direction = 0;
			}
		}
		else {
			monster1.SetTopLeft(12 * 40, 44 * 3);
			direction = 0;
			//角色在梯子怪物的移動模式待寫
		}
	}
	else {
		if (monster1.GetTop() % 44 != 0 && (direction == 1 || direction == 2)) {
			if (direction == 1) {
				monster1.SetTopLeft(monster1.GetLeft(), monster1.GetTop() - speed_y);
			}
			else if (direction == 2) {
				monster1.SetTopLeft(monster1.GetLeft(), monster1.GetTop() + speed_y);
			}
		}
		else if((character.GetTop() >= monster1.GetTop()) && (direction == 3 || direction == 4)){
			if (map[monster1.GetTop() / 44 + 1][monster1.GetLeft() / 40] == 2 && monster1.GetLeft() % 40 == 0) {
				direction = 0;
			}
			else if (downstair[0] == -1 && downstair[1] == -1) {
				if (map[monster1.GetTop() / 44][monster1.GetLeft() / 40] == 2 && monster1.GetLeft() % 40 == 0) {
					direction = 0;
				}
			}
			else if (direction == 3) {
				monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
			}
			else if (direction == 4) {
				monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
			}
		}
		else if ((character.GetTop() < monster1.GetTop()) && (direction == 3 || direction == 4)) {
			if (map[monster1.GetTop() / 44][monster1.GetLeft() / 40] == 2 && monster1.GetLeft() % 40 == 0) {
				direction = 0;
			}
			else if (upstair[0] == -1 && upstair[1] == -1) {
				if (map[monster1.GetTop() / 44 + 1][monster1.GetLeft() / 40] == 2 && monster1.GetLeft() % 40 == 0) {
					direction = 0;
				}
			}
			else if (direction == 3) {
				monster1.SetTopLeft(monster1.GetLeft() - speed_x, monster1.GetTop());
			}
			else if (direction == 4) {
				monster1.SetTopLeft(monster1.GetLeft() + speed_x, monster1.GetTop());
			}
		}
		else {
			direction = 0;
		}
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{

	ifstream ifs("map_entity/Stage" + to_string(stageid) + "_entity.txt");


	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 28; j++) {
			ifs >> map[i][j];
		}
	}

	ifs.close();
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 28; j++) {
			switch (map[i][j]) {
			case 0:
				stage[stageid - 1][i][j].LoadBitmapByString({ "resources/brick.bmp" });
				break;
			case 1:
				stage[stageid - 1][i][j].LoadBitmapByString({ "resources/black.bmp" });
				break;
			case 2:
				stage[stageid - 1][i][j].LoadBitmapByString({ "resources/ladder.bmp" });
				break;
			case 3:
				stage[stageid - 1][i][j].LoadBitmapByString({ "resources/rope.bmp" });
				break;
			case 4:
				stage[stageid - 1][i][j].LoadBitmapByString({ "resources/gold.bmp" });
				break;
			case 5:
				stage[stageid - 1][i][j].LoadBitmapByString({ "resources/block.bmp" });
				break;
			default:
				break;
			}
			stage[stageid - 1][i][j].SetTopLeft(j * 40, i * 44);
		}
	}
	for (int i = 0; i < 28; i++) {
		stage[stageid - 1][16][i].LoadBitmapByString({ "resources/ground.bmp" }, RGB(255, 255, 255));
		stage[stageid - 1][16][i].SetTopLeft(i * 40, 16 * 44);
		stage[stageid - 1][17][i].LoadBitmapByString({ "resources/brick.bmp" });
		stage[stageid - 1][17][i].SetTopLeft(i * 40, 17 * 44 - 22);
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
	character.SetTopLeft(40, 44 * 14);
	monster1.LoadBitmapByString({ 
									 "resources/monster1/01.bmp",
									 "resources/monster1/02.bmp",
									 "resources/monster1/03.bmp",
									 "resources/monster1/04.bmp",
									 "resources/monster1/05.bmp",
									 "resources/monster1/06.bmp",
									 "resources/monster1/08.bmp",
									 "resources/monster1/09.bmp",
									 "resources/monster1/10.bmp",
									 "resources/monster1/11.bmp",
									 "resources/monster1/12.bmp",
									 "resources/monster1/13.bmp",
									 "resources/monster1/14.bmp",
									 "resources/monster1/15.bmp",
									 "resources/monster1/16.bmp",
									 "resources/monster1/17.bmp",
									 "resources/monster1/18.bmp",
									 "resources/monster1/19.bmp",
									 "resources/monster1/20.bmp",
									 "resources/monster1/21.bmp",
									 "resources/monster1/22.bmp"
		}, RGB(0, 0, 0));
	monster1.SetTopLeft(120, 44 * 9);
	monster2.LoadBitmapByString({
									 "resources/monster1/01.bmp",
									 "resources/monster1/02.bmp",
									 "resources/monster1/03.bmp",
									 "resources/monster1/04.bmp",
									 "resources/monster1/05.bmp",
									 "resources/monster1/06.bmp",
									 "resources/monster1/08.bmp",
									 "resources/monster1/09.bmp",
									 "resources/monster1/10.bmp",
									 "resources/monster1/11.bmp",
									 "resources/monster1/12.bmp",
									 "resources/monster1/13.bmp",
									 "resources/monster1/14.bmp",
									 "resources/monster1/15.bmp",
									 "resources/monster1/16.bmp",
									 "resources/monster1/17.bmp",
									 "resources/monster1/18.bmp",
									 "resources/monster1/19.bmp",
									 "resources/monster1/20.bmp",
									 "resources/monster1/21.bmp",
									 "resources/monster1/22.bmp"
		}, RGB(0, 0, 0));
	monster2.SetTopLeft(160, 44 * 9);
	monster3.LoadBitmapByString({
									 "resources/monster1/01.bmp",
									 "resources/monster1/02.bmp",
									 "resources/monster1/03.bmp",
									 "resources/monster1/04.bmp",
									 "resources/monster1/05.bmp",
									 "resources/monster1/06.bmp",
									 "resources/monster1/08.bmp",
									 "resources/monster1/09.bmp",
									 "resources/monster1/10.bmp",
									 "resources/monster1/11.bmp",
									 "resources/monster1/12.bmp",
									 "resources/monster1/13.bmp",
									 "resources/monster1/14.bmp",
									 "resources/monster1/15.bmp",
									 "resources/monster1/16.bmp",
									 "resources/monster1/17.bmp",
									 "resources/monster1/18.bmp",
									 "resources/monster1/19.bmp",
									 "resources/monster1/20.bmp",
									 "resources/monster1/21.bmp",
									 "resources/monster1/22.bmp"
		}, RGB(0, 0, 0));
	monster3.SetTopLeft(200, 44 * 9);
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT || nChar == 0x41) {
		character.StopAnimation();
		int x = character.GetLeft();
		int y = character.GetTop() + character.GetHeight() / 2;
		if (map[y / 44][x / 40] == 3) {
			character.SetTopLeft(character.GetLeft() - speed_x, character.GetTop());
			character.Animation(6);
		}
		else if (map[y / 44][x / 40] != 0) {
			character.SetTopLeft(character.GetLeft() - speed_x, character.GetTop());
			character.Animation(2);
		}
	}
	else if (nChar == VK_DOWN || nChar == 0x53)
	{
		character.StopAnimation();
		int x = character.GetLeft() + character.GetWidth() / 2;
		int y = character.GetTop() + character.GetHeight();
		if (map[y / 44][x / 40] == 2) {
			character.SetTopLeft(character.GetLeft(), character.GetTop() + speed_y);
			character.Animation(3);
		}
	}
	else if (nChar == VK_RIGHT || nChar == 0x44) {
		character.StopAnimation();
		int x = character.GetLeft() + character.GetWidth();
		int y = character.GetTop() + character.GetHeight() / 2;
		if (map[y / 44][x / 40] == 3) {
			character.SetTopLeft(character.GetLeft() + speed_x, character.GetTop());
			character.Animation(5);
		}
		else if (map[y / 44][x / 40] != 0) {
			character.SetTopLeft(character.GetLeft() + speed_x, character.GetTop());
			character.Animation(1);
		}
	}
	else if (nChar == VK_UP || nChar == 0x57)
	{
		character.StopAnimation();
		int x = character.GetLeft();
		int y = character.GetTop() + character.GetHeight() - 1;
		if (map[y / 44][x / 40] == 2)
		{
			character.SetTopLeft(character.GetLeft(), character.GetTop() - speed_y);
			character.Animation(3);
		}
	}
	else if (nChar == VK_SPACE)
	{
		stageid++;
		ifstream ifs("map_entity/Stage" + to_string(stageid) + "_entity.txt");


		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 28; j++) {
				ifs >> map[i][j];
			}
		}

		ifs.close();
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 28; j++) {
				switch (map[i][j]) {
				case 0:
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/brick.bmp" });
					break;
				case 1:
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/black.bmp" });
					break;
				case 2:
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/ladder.bmp" });
					break;
				case 3:
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/rope.bmp" });
					break;
				case 4:
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/gold.bmp" });
					break;
				case 5:
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/block.bmp" });
					break;
				default:
					break;
				}
				stage[stageid - 1][i][j].SetTopLeft(j * 40, i * 44);
			}
		}
		for (int i = 0; i < 28; i++) {
			stage[stageid - 1][16][i].LoadBitmapByString({ "resources/ground.bmp" }, RGB(255, 255, 255));
			stage[stageid - 1][16][i].SetTopLeft(i * 40, 16 * 44);
			stage[stageid - 1][17][i].LoadBitmapByString({ "resources/brick.bmp" });
			stage[stageid - 1][17][i].SetTopLeft(i * 40, 17 * 44 - 22);
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
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 28; j++) {
			stage[stageid - 1][i][j].ShowBitmap();
		}
	}
	character.ShowBitmap();
	character.Animation(0);

	monster1.ShowBitmap();
	monster1.Animation(0);
	monster2.ShowBitmap();
	monster2.Animation(0);
	monster3.ShowBitmap();
	monster3.Animation(0);
	CDC *pDC = CDDraw::GetBackCDC();
	CTextDraw::Print(pDC, 300, 600, to_string(test));
	CDDraw::ReleaseBackCDC();
}





