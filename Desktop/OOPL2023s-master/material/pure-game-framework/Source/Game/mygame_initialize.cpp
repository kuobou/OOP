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
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g) : CGameState(g)
{
}

void CGameStateInit::OnInit()
{
	//
	// ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
	//     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
	//
	ShowInitProgress(0, "Start Initialize...");	// �@�}�l��loading�i�׬�0%
	load_background();
	//
	// �}�l���J���
	//
	//Sleep(1000);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
	//
	// ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
	//
}

void CGameStateInit::OnBeginState()
{
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{
	GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}

void CGameStateInit::OnShow()
{
	background.ShowBitmap();
}
void CGameStateInit::load_background() {
	background.LoadBitmapByString({
		"resources/image/video0010.bmp",
		"resources/image/video0001.bmp",
		"resources/image/video0002.bmp",
		"resources/image/video0003.bmp",
		"resources/image/video0004.bmp",
		"resources/image/video0005.bmp",
		"resources/image/video0006.bmp",
		"resources/image/video0007.bmp",
		"resources/image/video0008.bmp",
		"resources/image/video0009.bmp",
		"resources/image/video0010.bmp"
	});
	background.SetTopLeft(0, 0);
	//background.SetAnimation(40, FALSE);
	//background.SetAnimation(40, TRUE);
}