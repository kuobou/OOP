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
	for (int i = 0;i < 18; i++) {
		for (int j = 0; j < 28; j++) {
			if (stage[stageid - 1][i][j].IsDig()) {
				stage[stageid - 1][i][j].DigReset();
				if (stage[stageid - 1][i][j].GetFrameIndexOfBitmap() == 0) {
					map[i][j] = 0;
				}
			}
		}
	}
	for (int i = 0; i < CountEnemy; i++) {
		if (monster[i].IsCatch()) {
			finished = true;
		}
	}
	
	if (gold == 0) {
		gold = 0;
		CountEnemy = 0;
		stageid++;
		ifstream ifs;
		ifs.open("map_entity/Stage" + to_string(stageid) + "_entity.txt");


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
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/break/down/hole_00.bmp" ,
															  "resources/break/down/hole_01.bmp" ,
															  "resources/break/down/hole_02.bmp" ,
															  "resources/break/down/hole_03.bmp" ,
															  "resources/break/down/hole_04.bmp" ,
															  "resources/break/down/hole_05.bmp" ,
															  "resources/break/down/hole_06.bmp" ,
															  "resources/break/down/hole_07.bmp" ,
															  "resources/break/down/hole_08.bmp" ,
															  "resources/break/down/hole_09.bmp" ,
															  "resources/break/down/hole_10.bmp" ,
															  "resources/break/down/hole_11.bmp" ,
															  "resources/break/down/hole_12.bmp" ,
															  "resources/break/down/hole_13.bmp" ,
															  "resources/break/down/hole_14.bmp" ,
															  "resources/break/down/hole_15.bmp" ,
															  "resources/break/down/hole_16.bmp" ,
															  "resources/break/down/hole_17.bmp" ,
																}, RGB(255, 255, 255));
					break;
				case 1:
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/black.bmp" });
					break;
				case 2:
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/ladder.bmp" });
					break;
				case 3:
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/rope.bmp"  });
					break;
				case 4:
					gold++;
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/gold.bmp" ,"resources/black.bmp" });
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
		ifs.open("map_set/Stage" + to_string(stageid) + "_set.txt");


		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 28; j++) {
				ifs >> set[i][j];
			}
		}

		ifs.close();
		character.SetCharacter(set);
		for (int i = 0; i < 18; i++) {
			for (int j = 0; j < 28; j++) {
				if (set[i][j] == 'M') {
					monster[CountEnemy].SetTopLeft(j * 40, i * 44);
					CountEnemy++;
				}
			}
		}
	}
	if (character.GetPos() == 8) {
		if (character.GetTop() % 44 == 0) {
			if (map[character.GetTop() / 44 + 1][(character.GetLeft()+ speed_x) / 40] == 1) {
				character.SetTopLeft(character.GetLeft(), character.GetTop() + speed_y);
			}
			else {
				character.C_Animation(1);
			}
		}
	}
	if (digright) {
		if (stage[stageid - 1][character.GetTop() / 44 + 1][character.GetLeft() / 40 + 1].GetFrameIndexOfBitmap() != 17) {
			stage[stageid - 1][character.GetTop() / 44 + 1][character.GetLeft() / 40 + 1].DigRight();
		}
		else {
			map[character.GetTop() / 44 + 1][character.GetLeft() / 40 + 1] = 1;
			digright = false;
		}
	}
	else if (digleft) {
		if (stage[stageid - 1][character.GetTop() / 44 + 1][character.GetLeft() / 40 - 1].GetFrameIndexOfBitmap() != 8) {
			stage[stageid - 1][character.GetTop() / 44 + 1][character.GetLeft() / 40 - 1].DigLeft();
		}
		else {
			map[character.GetTop() / 44 + 1][character.GetLeft() / 40 - 1] = 1;
			digleft = false;
		}
	}
	else if (keyleft) {
		int x = character.GetLeft() - speed_x;
		int y = character.GetTop();
		if (character.GetPos() == 0) {
			character.StopAnimation();
			character.C_Animation(2);
			character.SetTopLeft(character.GetLeft() - speed_x, character.GetTop());
		}
		else if (map[y / 44][x / 40] == 3 && y % 44 == 0) {
			character.SetTopLeft(character.GetLeft() - speed_x, character.GetTop());
			character.C_Animation(6);
		}
		else if (map[y / 44 + 1][(x + speed_x) / 40] == 1 && map[y / 44][x / 40] == 1 && x % 40 == 0) {
			character.SetTopLeft(character.GetLeft(), character.GetTop() + speed_y);
			character.C_Animation(8);
		}
		else if ((y % 44 == 0 && map[y / 44][x / 40] != 0) || (y % 44 != 0 && (map[y / 44 + 1][x / 40] != 0 && map[y / 44][x / 40] != 0)) && character.GetLeft() > 0) {
			character.SetTopLeft(character.GetLeft() - speed_x, character.GetTop());
			character.C_Animation(2);
		}
	}
	else if (keydown)
	{
		int x = character.GetLeft();
		int y = character.GetTop() + character.GetHeight();
		if (map[y / 44][x / 40] == 2 && x % 40 == 0) {
			character.SetTopLeft(character.GetLeft(), character.GetTop() + speed_y);
			character.C_Animation(3);
		}
		else if (map[(y - character.GetHeight()) / 44 + 1][x  / 40] == 1) {
			character.SetTopLeft(character.GetLeft(), character.GetTop() + speed_y);
			character.C_Animation(8);
			//不在正中間待修
		}
	}
	else if (keyright) {
		int x = character.GetLeft() + character.GetWidth();
		int y = character.GetTop();
		if (map[y / 44][x / 40] == 3 && y % 44 == 0) {
			character.SetTopLeft(character.GetLeft() + speed_x, character.GetTop());
			character.C_Animation(5);
		}
		else if (map[y / 44 + 1][(x - character.GetWidth() + speed_x) / 40] == 1 && map[y / 44][(x - character.GetWidth() + speed_x) / 40] == 1 && x % 40 == 0) {
			character.SetTopLeft(character.GetLeft() + speed_x, character.GetTop() + speed_y);
			character.C_Animation(8);
		}
		else if ((y % 44 == 0 && map[y / 44][x / 40] != 0) || (y % 44 != 0 && (map[y / 44 + 1][x / 40] != 0 && map[y / 44][x / 40] != 0))) {
			character.SetTopLeft(character.GetLeft() + speed_x, character.GetTop());
			character.C_Animation(1);
		}
	}
	else if (keyup)
	{
		int x = character.GetLeft();
		int y = character.GetTop() + character.GetHeight() - 1;
		if (map[y / 44][x / 40] == 2 && x % 40 == 0)
		{
			character.SetTopLeft(character.GetLeft(), character.GetTop() - speed_y);
			character.C_Animation(3);
		}
	}
	if (character.GetPos() == 8) {
		if (!character.IsGround(map)) {
			character.SetTopLeft(character.GetLeft(), character.GetTop() + speed_y);
		}
	}
	if (character.IsGold(map)) {
		map[character.GetTop() / 44][character.GetLeft() / 40] = 1;
		stage[stageid - 1][character.GetTop() / 44][character.GetLeft() / 40].SetFrameIndexOfBitmap(1);
		gold--;
	}
	if (character.GetPos() != 0) {
		for (int i = 0; i < CountEnemy; i++) {
			monster[i].EnemyMove(character, map);
		}
	}
}

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	int count = 0;
	ifstream ifs;
	ifs.open("map_entity/Stage" + to_string(stageid) + "_entity.txt");


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
				stage[stageid - 1][i][j].LoadBitmapByString({ "resources/break/down/hole_00.bmp" ,
															  "resources/break/down/hole_01.bmp" ,
															  "resources/break/down/hole_02.bmp" ,
															  "resources/break/down/hole_03.bmp" ,
															  "resources/break/down/hole_04.bmp" ,
															  "resources/break/down/hole_05.bmp" ,
															  "resources/break/down/hole_06.bmp" ,
															  "resources/break/down/hole_07.bmp" ,
															  "resources/break/down/hole_08.bmp" ,
															  "resources/break/down/hole_09.bmp" ,
															  "resources/break/down/hole_10.bmp" ,
															  "resources/break/down/hole_11.bmp" ,
															  "resources/break/down/hole_12.bmp" ,
															  "resources/break/down/hole_13.bmp" ,
															  "resources/break/down/hole_14.bmp" ,
															  "resources/break/down/hole_15.bmp" ,
															  "resources/break/down/hole_16.bmp" ,
															  "resources/break/down/hole_17.bmp" ,
															}, RGB(255, 255, 255));
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
				count++;
				stage[stageid - 1][i][j].LoadBitmapByString({ "resources/gold.bmp", "resources/black.bmp" });
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
	gold = count;
	for (int i = 0; i < 28; i++) {
		stage[stageid - 1][16][i].LoadBitmapByString({ "resources/ground.bmp" }, RGB(255, 255, 255));
		stage[stageid - 1][16][i].SetTopLeft(i * 40, 16 * 44);
		stage[stageid - 1][17][i].LoadBitmapByString({ "resources/brick.bmp" });
		stage[stageid - 1][17][i].SetTopLeft(i * 40, 17 * 44 - 22);
	}
	ifs.open("map_set/Stage" + to_string(stageid) + "_set.txt");

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 28; j++) {
			ifs >> set[i][j];
		}
	}

	ifs.close();
	character.LoadBitmapByString({
								"resources/character_new/runner_01.bmp",
								"resources/character_new/runner_02.bmp",
								"resources/character_new/runner_03.bmp",
								"resources/character_new/runner_04.bmp",
								"resources/character_new/runner_05.bmp",
								"resources/character_new/runner_06.bmp",
								"resources/character_new/runner_07.bmp",
								"resources/character_new/runner_08.bmp",
								"resources/character_new/runner_09.bmp",
								"resources/character_new/runner_10.bmp",
								"resources/character_new/runner_11.bmp",
								"resources/character_new/runner_12.bmp",
								"resources/character_new/runner_13.bmp",
								"resources/character_new/runner_14.bmp",
								"resources/character_new/runner_15.bmp",
								"resources/character_new/runner_16.bmp",
								"resources/character_new/runner_17.bmp",
								"resources/character_new/runner_18.bmp",
		}, RGB(0, 0, 0));
	character.SetCharacter(set);
	for (int i = 0; i < 10; i++) {
		monster[i].LoadBitmapByString({
									 "resources/monster/01.bmp",
									 "resources/monster/02.bmp",
									 "resources/monster/03.bmp",
									 "resources/monster/04.bmp",
									 "resources/monster/05.bmp",
									 "resources/monster/06.bmp",
									 "resources/monster/07.bmp",
									 "resources/monster/08.bmp",
									 "resources/monster/09.bmp",
									 "resources/monster/10.bmp",
									 "resources/monster/11.bmp",
									 "resources/monster/12.bmp",
									 "resources/monster/13.bmp",
									 "resources/monster/14.bmp",
									 "resources/monster/15.bmp",
									 "resources/monster/16.bmp",
									 "resources/monster/17.bmp",
									 "resources/monster/18.bmp",
									 "resources/monster/19.bmp",
									 "resources/monster/20.bmp",
									 "resources/monster/21.bmp",
									 "resources/monster/22.bmp"
			}, RGB(0, 0, 0));
	}
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 28; j++) {
			if (set[i][j] == 'M') {
				monster[CountEnemy].SetTopLeft(j * 40, i * 44);
				CountEnemy++;
			}
		}
	}
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{	
	if (character.GetPos() == 0) {
		character.StopAnimation();
		character.C_Animation(1);
	}
	else if (nChar == VK_LEFT || nChar == 0x41) {
		keyleft = true;
	}
	else if (nChar == VK_DOWN || nChar == 0x53)
	{
		keydown = true;
	}
	else if (nChar == VK_RIGHT || nChar == 0x44) {
		keyright = true;
	}
	else if (nChar == VK_UP || nChar == 0x57)
	{
		keyup = true;
	}
	else if (nChar == VK_SPACE)
	{
		gold = -1;
		int count = 0;
		CountEnemy = 0;
		stageid++;
		ifstream ifs;
		ifs.open("map_entity/Stage" + to_string(stageid) + "_entity.txt");
		
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
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/rope.bmp"  });
					break;
				case 4:
					count++;
					stage[stageid - 1][i][j].LoadBitmapByString({ "resources/gold.bmp", "resources/black.bmp" });
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
		gold = count;

		ifs.open("map_set/Stage" + to_string(stageid) + "_set.txt");


		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 28; j++) {
				ifs >> set[i][j];
			}
		}

		ifs.close();
		character.SetCharacter(set);
		for (int i = 0; i < 18; i++) {
			for (int j = 0; j < 28; j++) {
				if (set[i][j] == 'M') {
					monster[CountEnemy].SetTopLeft(j * 40, i * 44);
					CountEnemy++;
				}
			}
		}
	}
	else if (nChar == 0x51) {
		if (map[character.GetTop() / 44 + 1][character.GetLeft() / 40 - 1] == 0) {
			stage[stageid - 1][character.GetTop() / 44 + 1][character.GetLeft() / 40 - 1].SetFrameIndexOfBitmap(1);
			stage[stageid - 1][character.GetTop() / 44 + 1][character.GetLeft() / 40 - 1].DigLeft();
			digleft = true;
		}
	}
	else if (nChar == 0x58) {
		if (map[character.GetTop() / 44 + 1][character.GetLeft() / 40 - 1] == 0) {
			stage[stageid - 1][character.GetTop() / 44 + 1][character.GetLeft() / 40 + 1].SetFrameIndexOfBitmap(10);
			stage[stageid - 1][character.GetTop() / 44 + 1][character.GetLeft() / 40 + 1].DigRight();
			digright = true;
		}
	}
}


void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_LEFT || nChar == 0x41) {
		keyleft = false;
	}
	else if (nChar == VK_DOWN || nChar == 0x53)
	{
		keydown = false;
	}
	else if (nChar == VK_RIGHT || nChar == 0x44) {
		keyright = false;
	}
	else if (nChar == VK_UP || nChar == 0x57)
	{
		keyup = false;
	}
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
	if (!finished) {
		for (int i = 0; i < 18; i++) {
			for (int j = 0; j < 28; j++) {
				stage[stageid - 1][i][j].ShowBitmap();
				
			}
		}
		character.ShowBitmap();
		character.C_Animation(0);

		for (int i = 0; i < CountEnemy; i++) {
			monster[i].ShowBitmap();
			monster[i].Animation(0);
		}
	}
	else {
		CDC *pDC = CDDraw::GetBackCDC();
		CTextDraw::Print(pDC, 500, 380, "Game over");
		CDDraw::ReleaseBackCDC();
	}
	/*CDC *pDC = CDDraw::GetBackCDC();
	CTextDraw::Print(pDC, 300, 600, to_string(test));
	CDDraw::ReleaseBackCDC();*/
}





