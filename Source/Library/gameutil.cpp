//#define	 INITGUID
#include "stdafx.h"
#include "../Core/game.h"
#include "../Core/MainFrm.h"
#include "../Core/Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include "audio.h"
#include "gameutil.h"
#include "gamecore.h"
#include "Shlwapi.h"
#include "../Game/config.h"
#include "../Game/mygame.h"
#include <filesystem>
#include <experimental/filesystem> // Header file for pre-standard implementation
#include <comdef.h>

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// CMovingBitmap: Moving Bitmap class
	// 這個 class 提供可以移動的圖形
	// 要懂得怎麼呼叫(運用)其各種能力，但是可以不懂下列的程式是什麼意思
	/////////////////////////////////////////////////////////////////////////////

	//! CMovingBitmap 建構子
	/*! 
		用於創立一個尚未讀取圖片的物件。
	*/
	CMovingBitmap::CMovingBitmap()
	{
		isBitmapLoaded = false;
	}

	//! 取得 CMovingBitmap 物件的圖片高度。
	/*!
		需要先載入圖片。
		\return 圖片高度，以像素為單位。
	*/
	int CMovingBitmap::GetHeight()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Height() is called !!!");
		return locations[frameIndex].bottom - locations[frameIndex].top;
	}

	//! 取得 CMovingBitmap 物件的左上角的 x 軸座標值。
	/*!
		需要先載入圖片。
		\return 圖片左上角的 x 軸座標值。
	*/
	int CMovingBitmap::GetLeft()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Left() is called !!!");
		return locations[frameIndex].left;
	}

	//! 讀取圖片資源。
	/*!
		透過資源編號 `IDB_BITMAP` 來讀取對應的圖片，並且過濾特定顏色 `color`。
		\param IDB_BITMAP 圖片資源編號
		\param color 欲過濾的顏色（預設為 `CLR_INVALID`，可利用 `RGB(<R>, <G>, <B>`) 來設置顏色）
	*/
	void CMovingBitmap::LoadBitmap(int IDB_BITMAP, COLORREF color)
	{
		CBitmap bitmap;
		BOOL rval = bitmap.LoadBitmap(IDB_BITMAP);
		//GAME_ASSERT(rval, "Load bitmap failed !!! Please check bitmap ID (IDB_XXX).");
		BITMAP bitmapSize;
		bitmap.GetBitmap(&bitmapSize);
		
		InitializeRectByBITMAP(bitmapSize);

		surfaceID.push_back(CDDraw::RegisterBitmap(IDB_BITMAP, color));
		filterColor = color;
		isBitmapLoaded = true;
	}

	//! 讀取圖片資源。
	/*!
		透過圖片相對路徑 `filepath` 來讀取對應的圖片，並且過濾特定顏色 `color`。
		\param filepath 圖片相對路徑
		\param color 欲過濾的顏色（預設為 `CLR_INVALID`，可利用 `RGB(<R>, <G>, <B>`) 來設置過濾顏色）
	*/
	void CMovingBitmap::LoadBitmap(char *filepath, COLORREF color)
	{
		HBITMAP hbitmap = (HBITMAP)LoadImage(NULL, filepath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hbitmap == NULL) {
			char error_msg[300];
			sprintf(error_msg, "Loading bitmap	from file \"%s\" failed !!!", filepath);
			GAME_ASSERT(false, error_msg);
		}

		CBitmap *bmp = CBitmap::FromHandle(hbitmap); // memory will be deleted automatically
		BITMAP bitmapSize;
		bmp->GetBitmap(&bitmapSize);

		InitializeRectByBITMAP(bitmapSize);

		surfaceID.push_back(CDDraw::RegisterBitmap(filepath, color));
		imageFileName = string(filepath);
		filterColor = color;
		isBitmapLoaded = true;

		bmp->DeleteObject();
	}

	//! 讀取圖片資源。
	/*!
		透過圖片相對路徑集 `filepaths` 來讀取多個圖片，並且過濾特定顏色 `color`。
		\param filepaths 圖片相對路徑集
		\param color 欲過濾的顏色（預設為 `CLR_INVALID`，可利用 `RGB(<R>, <G>, <B>`) 來設置過濾顏色）
		\sa https://en.cppreference.com/w/cpp/container/vector
	*/
	void CMovingBitmap::LoadBitmap(vector<char*> filepaths, COLORREF color)
	{
		for (int i = 0; i < (int)filepaths.size(); i++) {
			LoadBitmap(filepaths[i], color);
		}
	}

	//! 讀取圖片資源。
	/*!
		透過圖片相對路徑集 `filepaths` 來讀取多個圖片，並且過濾特定顏色 `color`。
		\param filepaths 圖片相對路徑集
		\param color 欲過濾的顏色（預設為 `CLR_INVALID`，可利用 `RGB(<R>, <G>, <B>`) 來設置過濾顏色）
		\sa https://en.cppreference.com/w/cpp/container/vector
	*/
	void CMovingBitmap::LoadBitmapByString(vector<string> filepaths, COLORREF color)
	{

		for (int i = 0; i < (int)filepaths.size(); i++) {
			LoadBitmap((char*)filepaths[i].c_str(), color);
		}
	}
	
	//! 讀取空白圖片資源。
	/*!
		讀取一個特定大小的白色點陣圖。
		\param height 圖片長度
		\param width 圖片寬度
	*/
	void CMovingBitmap::LoadEmptyBitmap(int height, int width) {
		HBITMAP hbitmap = CreateBitmap(width, height, 1, 32, NULL);
		CBitmap *bmp = CBitmap::FromHandle(hbitmap); // memory will be deleted automatically

		/* Fill white color to bitmap */
		HDC hdc = CreateCompatibleDC(NULL);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdc, hbitmap);
		PatBlt(hdc, 0, 0, width, height, WHITENESS);
		SelectObject(hdc, hOldBitmap);
		DeleteDC(hdc);

		BITMAP bitmapSize;
		bmp->GetBitmap(&bitmapSize);

		InitializeRectByBITMAP(bitmapSize);

		surfaceID.push_back(CDDraw::RegisterBitmapWithHBITMAP(hbitmap));
		isBitmapLoaded = true;

		bmp->DeleteObject();
	}
	
	//! 停止顯示圖片。
	/*!
		@deprecated 從 v1.0.0 版本後棄用，停止顯示圖片請在 `OnShow()` 時不呼叫 `ShowBitmap()` 即可
		\sa ShowBitmap()
	*/
	void CMovingBitmap::UnshowBitmap()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before SetTopLeft() is called !!!");
		isAnimation = false;
		this->ShowBitmap(0);
	}

	//! 設置圖片至畫布指定座標上。
	/*!
		將會把圖片左上角設置至指定座標上。
		\param x 左上角 x 座標
		\param y 左上角 y 座標
	*/
	void CMovingBitmap::SetTopLeft(int x, int y)
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before SetTopLeft() is called !!!");

		for (int i = 0; i < int(locations.size()); i++) {
			int dx = locations[i].left - x;
			int dy = locations[i].top - y;
			locations[i].left = x;
			locations[i].top = y;
			locations[i].right -= dx;
			locations[i].bottom -= dy;
		}
	}

	//! 設置圖片是否為動畫。
	/*!
		若 CMovingBitmap 讀入多個圖片，則可以使用此函數來設定物件為動畫。
		\param delay 動畫切換延遲（以毫秒為單位）
		\param once 動畫是否為一次性動畫，若是則需要以 `ToggleAnimation()` 來呼叫動畫啟動。
		\sa ToggleAnimation()
	*/
	void CMovingBitmap::SetAnimation(int delay, bool once) {
		if(!once) isAnimation = true;
		isOnce = once;
		delayCount = delay;
	}
	
	//! 顯示圖片。
	/*!
		僅能在 `onShow()` 時呼叫，且圖片需要被讀取。
	*/
	void CMovingBitmap::ShowBitmap()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		CDDraw::BltBitmapToBack(surfaceID[frameIndex], locations[frameIndex].left, locations[frameIndex].top);
		ShowBitmapBySetting();
	}

	//! 顯示圖片。
	/*!
		僅能在 `onShow()` 時呼叫，且圖片需要被讀取。
		\param factor 放大倍率，需要 VGA 顯卡的支援，否則會變得異常慢。
	*/
	void CMovingBitmap::ShowBitmap(double factor)
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before ShowBitmap() is called !!!");
		CDDraw::BltBitmapToBack(surfaceID[frameIndex], locations[frameIndex].left, locations[frameIndex].top, factor);
		ShowBitmapBySetting();
	}

	//! 設置當前圖片顯示幀的索引值。
	/*!
		圖片顯示幀的索引值以 0 開始。
		\param frameIndex 圖片顯示幀的索引值。
	*/
	void CMovingBitmap::SetFrameIndexOfBitmap(int frameIndex) {
		GAME_ASSERT(frameIndex < (int) surfaceID.size(), "選擇圖片時索引出界");
		this->frameIndex = frameIndex;
	}

	//! 取得當前圖片顯示幀的索引值。
	/*!
		\return 圖片顯示幀的索引值。
	*/
	int CMovingBitmap::GetFrameIndexOfBitmap() {
		return frameIndex;
	}

	//! 取得當前圖片左上角 y 軸的座標值。
	/*!
		\return 圖片左上角 y 軸的座標值。
	*/
	int CMovingBitmap::GetTop()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Top() is called !!!");
		return locations[frameIndex].top;
	}

	//! 取得當前圖片寬度。
	/*!
		\return 取得當前圖片寬度。
	*/
	int CMovingBitmap::GetWidth()
	{
		GAME_ASSERT(isBitmapLoaded, "A bitmap must be loaded before Width() is called !!!");
		return locations[frameIndex].right - locations[frameIndex].left;
	}
	
	//! 啟動單次動畫。
	/*!
		將動畫設為初始幀，並且初始化單次動畫的參數值。
	*/
	void CMovingBitmap::ToggleAnimation() {
		frameIndex = 0;
		isAnimation = true;
		isAnimationDone = false;
	}

	//! 物件是否為動畫物件。
	/*!
		\return 布林值，表示物件是否為動畫物件。
	*/
	bool CMovingBitmap::IsAnimation() {
		return isAnimation;
	}

	//! 動畫物件是否已執行完動畫。
	/*!
		\return 布林值，表示動畫物件是否已執行完動畫。
	*/
	bool CMovingBitmap::IsAnimationDone() {
		return isAnimationDone;
	}

	//! 動畫物件是否為單次動畫物件。
	/*!
		\return 布林值，表示動畫物件是否為單次動畫物件。
	*/
	bool CMovingBitmap::IsOnceAnimation() {
		return isOnce;
	}

	//! 物件是否已讀取點陣圖。
	/*!
		\return 布林值，表示物件是否已讀取點陣圖。
	*/
	bool CMovingBitmap::IsBitmapLoaded() {
		return isBitmapLoaded;
	}

	//! 回傳物件的幀數。
	/*!
		\return 回傳物件的幀數。
	*/
	int CMovingBitmap::GetFrameSizeOfBitmap() {
		return (int) surfaceID.size();
	}

	//! 根據 BITMAP 來初始化 CMovingBitmap 內的 location 物件。
	/*!
		\param bitmapSize 已初始化過的 BITMAP 物件，帶有點陣圖的高與寬
	*/
	void CMovingBitmap::InitializeRectByBITMAP(BITMAP bitmapSize) {
		const unsigned NX = 0;
		const unsigned NY = 0;
		CRect newCrect;
		newCrect.left = NX;
		newCrect.top = NY;
		newCrect.right = NX + bitmapSize.bmWidth;
		newCrect.bottom = NY + bitmapSize.bmHeight;
		locations.push_back(newCrect);
	}

	//! 根據使用者設定的參數來顯示圖片。
	void CMovingBitmap::ShowBitmapBySetting() {
		if (isAnimation == true && clock() - last_time >= delayCount) {
			frameIndex += 1;
			last_time = clock();
			if (frameIndex == surfaceID.size() && animationCount > 0) {
				animationCount -= 1;
			}
			if (frameIndex == surfaceID.size() && (isOnce || animationCount == 0)) {
				isAnimation = false;
				isAnimationDone = true;
				frameIndex = surfaceID.size() - 1;
				return;
			}
			frameIndex = frameIndex % surfaceID.size();
		}
	}

	//! 取得物件載入圖片名稱。
	/*!
		\return 回傳圖片名稱，若圖片尚未載入，則回傳一空字串。
	*/
	string CMovingBitmap::GetImageFileName() {
		return imageFileName;
	}

	//! 取得物件過濾顏色。
	/*!
		\return 回傳過濾顏色，若物件未設定過濾顏色，回傳 `CLR_INVALID`。
	*/
	COLORREF CMovingBitmap::GetFilterColor() {
		return filterColor;
	}

	//! 兩物件是否交疊。
	/*!
		\param bmp1 第一個 CMovingBitmap 物件
		\param bmp2 第二個 CMovingBitmap 物件
		\return 回傳布林值，代表兩物件是否交疊。
	*/
	bool CMovingBitmap::IsOverlap(CMovingBitmap bmp1, CMovingBitmap bmp2) {
		CRect rect;
		BOOL isOverlap = rect.IntersectRect(bmp1.locations[bmp1.GetFrameIndexOfBitmap()], bmp2.locations[bmp2.GetFrameIndexOfBitmap()]);
		return isOverlap;
	}

	/////////////////////////////////////////////////////////////////////////////
	// CTextDraw: The class provide the ability to draw the text.
	// 這個 class 提供文字的呈現
	// 要懂得怎麼呼叫(運用)其各種能力，但是可以不懂下列的程式是什麼意思
	/////////////////////////////////////////////////////////////////////////////

	//! 在畫面上印出文字。
	/*!
		我們將 pDC 取得的責任交給 caller，你必須要取得當前的 pDC 指標，然後記得釋放。
		\param pDC，一個 CDC 指標，可使用 `CDDraw::GetBackCDC()` 取得。
		\param x 文字顯示的左上角 x 座標
		\param y 文字顯示的左上角 y 座標
		\param str 欲顯示的文字
	*/
	void CTextDraw::Print(CDC *pDC, int x, int y, string str) {
		x = CDDraw::IsFullScreen() ? x + (RESOLUTION_X - SIZE_X) / 2 : x;
		y = CDDraw::IsFullScreen() ? y + (RESOLUTION_Y - SIZE_Y) / 2 : y;
		pDC->TextOut(x, y, str.c_str());
	}

	//! 設定當前文字的屬性。
	/*!
		我們將 pDC 取得的責任交給 caller，你必須要取得當前的 pDC 指標，然後記得釋放。
		此設定會在設定的 pDC 指標被釋放時失去設定值。
		\param pDC，一個 CDC 指標，可使用 `CDDraw::GetBackCDC()` 取得。
		\param size 文字的大小（以 pt 為單位，但因技術有限，並不一定會完全符合 pt 為單位的大小）
		\param fontName 字體名稱
		\param fontColor 字體顏色
		\param weight 字體粗度（預設為 500）
	*/
	void CTextDraw::ChangeFontLog(CDC *pDC, int size, string fontName, COLORREF fontColor, int weight) {
		CFont* fp;

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(fontColor);
		LOGFONT lf;
		CFont f;
		memset(&lf, 0, sizeof(lf));
		lf.lfHeight = -MulDiv(size, GetDeviceCaps(pDC->GetSafeHdc(), LOGPIXELSY), 96);
		lf.lfWeight = weight;
		strcpy(lf.lfFaceName, fontName.c_str());
		f.CreateFontIndirect(&lf);
		fp = pDC->SelectObject(&f);
	}
	void CMovingBitmap::C_Animation(int num) {
		switch (num)
		{
		case 0:
			//Sleep(10);
			if (pos == 0 && !stopanimate) {
				if (GetFrameIndexOfBitmap() == 2) {
					SetFrameIndexOfBitmap(0);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
				pos = num;
			}
			break;
		case 1:
			if (pos == 1) {
				if (GetFrameIndexOfBitmap() == 2) {
					SetFrameIndexOfBitmap(0);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(0);
			}
			pos = num;
			break;
		case 2:
			if (pos == 2) {
				if (GetFrameIndexOfBitmap() == 5) {
					SetFrameIndexOfBitmap(3);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(3);
			}
			pos = num;
			break;
		case 3:
			if (pos == 3) {
				if (GetFrameIndexOfBitmap() == 7) {
					SetFrameIndexOfBitmap(6);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(6);
			}
			pos = num;
			break;
		case 4:
			if (pos == 4) {
				SetFrameIndexOfBitmap(8);
			}
			else {
				SetFrameIndexOfBitmap(8);
			}
			pos = num;
			break;
		case 5:
			if (pos == 5) {
				if (GetFrameIndexOfBitmap() == 11) {
					SetFrameIndexOfBitmap(9);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(11);
			}
			pos = num;
			break;
		case 6:
			if (pos == 6) {
				if (GetFrameIndexOfBitmap() == 14) {
					SetFrameIndexOfBitmap(12);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(14);
			}
			pos = num;
			break;
		case 7:
			if (pos == 7) {
				if (GetFrameIndexOfBitmap() == 16) {
					SetFrameIndexOfBitmap(15);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(17);
			}
			pos = num;
			break;
		case 8:
			SetFrameIndexOfBitmap(17);
			pos = num;
			break;
		default:
			break;
		}
	}
	void CMovingBitmap::Animation(int num) {
		switch (num)
		{
		case 0:
			Sleep(10);
			if (pos == 0 && !stopanimate) {
				if (GetFrameIndexOfBitmap() == 2) {
					SetFrameIndexOfBitmap(0);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
				pos = num;
			}
			break;
		case 1:
			if (pos == 1) {
				if (GetFrameIndexOfBitmap() == 2) {
					SetFrameIndexOfBitmap(0);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(0);
			}
			pos = num;
			break;
		case 2:
			if (pos == 2) {
				if (GetFrameIndexOfBitmap() == 5) {
					SetFrameIndexOfBitmap(3);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(3);
			}
			pos = num;
			break;
		case 3:
			if (pos == 3) {
				if (GetFrameIndexOfBitmap() == 7) {
					SetFrameIndexOfBitmap(6);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(6);
			}
			pos = num;
			break;
		case 4:
			SetFrameIndexOfBitmap(8);
			pos = num;
			break;
		case 5:
			if (pos == 5) {
				if (GetFrameIndexOfBitmap() == 13) {
					SetFrameIndexOfBitmap(11);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(11);
			}
			pos = num;
			break;
		case 6:
			if (pos == 6) {
				if (GetFrameIndexOfBitmap() == 16) {
					SetFrameIndexOfBitmap(14);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(14);
			}
			pos = num;
			break;
		case 7:
			if (pos == 7) {
				if (GetFrameIndexOfBitmap() == 18) {
					SetFrameIndexOfBitmap(17);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(17);
			}
			pos = num;
			break;
		case 8:
			if (0) {
				if (GetFrameIndexOfBitmap() == 22) {
					SetFrameIndexOfBitmap(19);
				}
				else {
					SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
				}
			}
			else {
				SetFrameIndexOfBitmap(19);
			}
			pos = num;
			break;
		default:
			break;
		}
	}
	int CMovingBitmap::GetPos() {
		return pos;
	}
	void CMovingBitmap::StopAnimation() {
		stopanimate = true;
	}
	bool  CMovingBitmap::IsGround(int map[18][28], int p) {
		int x = GetLeft();
		int y = GetTop();
		if (map[y / 44 + 1][(x + p) / 40] == 0 || map[y / 44 + 1][(x + p) / 40] == 2 || map[y / 44 + 1][(x + p) / 40] == 5) {
			return true;
		}
		else {
			return false;
		}
	}
	bool CMovingBitmap::IsGold(int map[18][28]) {
		int x = GetLeft();
		int y = GetTop();
		if (map[y / 44][x / 40] == 4 && x % 40 == 0) {
			return true;
		}
		else {
			return false;
		}
	}
	void CMovingBitmap::SetCharacter(char set[18][28]) {
		for (int i = 0; i < 18; i++) {
			for (int j = 0; j < 28; j++) {
				if (set[i][j] == '?') {
					SetTopLeft(j * 40, i * 44);
				}
			}
		}
	}
	int CMovingBitmap::GetDirection() {
		return direction;
	}
	void CMovingBitmap::EnemyMove(CMovingBitmap character, int map[18][28]) {
		int left = 0;
		int right = 1;
		int x = GetLeft();
		int y = GetTop();
		bool down = false;
		bool up = false;
		downstair[0] = -1;
		downstair[1] = -1;
		upstair[0] = -1;
		upstair[1] = -1;
		if (y == character.GetTop() && ((x <= character.GetLeft() && character.GetLeft() <= x + GetWidth()) || (x <= character.GetLeft() + character.GetWidth() && character.GetLeft() + character.GetWidth() <= x + GetWidth()))) {
			catched = true;
		}
		if (GetPos() == 4) {
			if (map[y / 44 + 1][x / 40] != 0 && map[y / 44 + 1][x / 40] != 5){
				SetTopLeft(x, y + speed_y);
				Animation(4);
				direction = 2;
			}
			else {
				direction = 0;
				Animation(1);
			}
		}
		else if (GetPos() == 3 && y % 44 == 0 && direction == 1 && map[y / 44][x / 40] != 2) {
			direction = 0;
			Animation(1);
		}
		else if (direction == 1 && map[y / 44 + 1][x / 40] == 2 && y % 44 != 0 && map[(y - speed_y) / 44][x / 40] != 0 && map[(y - speed_y) / 44][x / 40] != 5) {
			SetTopLeft(x, y - speed_y);
			Animation(3);
			direction = 1;
		}
		else if (map[y / 44][x / 40] == 2 && x % 40 == 0 && (direction == 1 || direction == 2)) {
			if (direction == 1) {
				if (map[(y - speed_y) / 44][x / 40] == 0 || map[(y - speed_y) / 44][x / 40] == 5) {
					direction = 0;
				}
				else {
					SetTopLeft(x, y - speed_y);
					Animation(3);
					direction = 1;
				}
			}
			else if(direction = 2) {
				if (map[(y + speed_y) / 44 + 1][x / 40] == 0 || map[(y + speed_y) / 44 + 1][x / 40] == 5) {
					direction = 0;
				}
				else{
					SetTopLeft(x, y + speed_y);
					Animation(3);
					direction = 2;
				}
			}
		}
		else if (map[y / 44][x / 40] != 2 && map[y / 44][x / 40] != 3 && map[y / 44 + 1][x / 40] == 1 && x % 40 == 0) {
			Animation(4);
			direction = 2;
		}
		else if(map[y / 44][x / 40] == 2 && x % 40 == 0 && map[y / 44 + 1][x / 40] == 1){
			if (y < character.GetTop()) {
				if (map[y / 44 + 1][x / 40] == 2 || map[y / 44 + 1][x / 40] == 1) {
					if (map[y / 44 + 1][x / 40] == 1) {
						SetTopLeft(x, y + speed_y);
						Animation(4);
						direction = 2;
					}
					else if (map[y / 44 + 1][x / 40] == 2) {
						SetTopLeft(x, y + speed_y);
						Animation(3);
						direction = 2;
					}
				}
				else if (map[y / 44 - 1][x / 40] == 2) {
					SetTopLeft(x, y - speed_y);
					Animation(3);
					direction = 1;
				}
			}
			else {
				if (map[y / 44 - 1][x / 40] == 2) {
					SetTopLeft(x, y - speed_y);
					Animation(3);
					direction = 1;
				}
				else if (map[y / 44 + 1][x / 40] == 2 || map[y / 44 + 1][x / 40] == 1) {
					if (map[y / 44 + 1][x / 40] == 2) {
						SetTopLeft(x, y + speed_y);
						Animation(3);
						direction = 2;
					}
					else if (map[y / 44 + 1][x / 40] == 1) {
						SetTopLeft(x, y + speed_y);
						Animation(4);
						direction = 2;
					}
				}
			}
		}
		else if ((direction == 0 || direction == 2) && map[y / 44 + 1][x / 40] == 3) {
			if (map[(y + speed_y) / 44][x / 40] == 0 || map[(y + speed_y) / 44][x / 40] == 5) {
				direction = 0;
			}
			else {
				SetTopLeft(x, y + speed_y);
				Animation(3);
				direction = 2;
			}
		}
		else{
			while (map[y / 44][x / 40 - left] != 0 && map[y / 44][x / 40 - left] != 5 && (map[y / 44 ][x / 40 - left] == 3 || (map[y / 44 + 1][x / 40 - left] != 1 && map[y / 44 + 1][x / 40 - left] != 3 && map[y / 44 + 1][x / 40 - left] != 4)) && (downstair[0] == -1 || upstair[0] == -1) && ((x / 40 - left) >= 0)) {
				if (downstair[0] == -1 && map[y / 44 + 1][x / 40 - left] == 2) {
					downstair[0] = left;
					down = true;
				}
				if (upstair[0] == -1 && map[y / 44][x / 40 - left] == 2) {
					upstair[0] = left;
					up = true;
				}
				left++;
			}
			while (map[y / 44][x / 40 + right] != 0 && map[y / 44][x / 40 + right] != 5 && (map[y / 44][x / 40 + right] == 3 || map[y / 44 + 1][x / 40 + right] != 1 && map[y / 44 + 1][x / 40 + right] != 3 && map[y / 44 + 1][x / 40 + right] != 4) && (downstair[1] == -1 || upstair[1] == -1) && ((x / 40 + right) < 28)) {
				if (downstair[1] == -1 && map[y / 44 + 1][x / 40 + right] == 2) {
					downstair[1] = right;
					down = true;
				}
				if (upstair[1] == -1 && map[y / 44][x / 40 + right] == 2) {
					upstair[1] = right;
					up = true;
				}
				right++;
			}

			if (((y < character.GetTop() && down) || (y > character.GetTop() && !up && down)) && direction != 1) {
				if (downstair[0] == 0 && x % 40 == 0) {
					SetTopLeft(x, y + speed_y);
					Animation(3);
					direction = 2;
				}
				else if(downstair[0] == 0 && y % 44 == 0){
					if (GetPos() == 1 || GetPos() == 5) {
						SetTopLeft(x + speed_x, y);
						if (map[y / 44][(x + speed_x) / 40] == 3) {
							Animation(5);
							direction = 4;
						}
						else {
							Animation(1);
							direction = 4;
						}
					}
					else if (GetPos() == 2 || GetPos() == 6) {
						SetTopLeft(x - speed_x, y);
						if (map[y / 44][(x - speed_x) / 40] == 3) {
							Animation(6);
							direction = 3;
						}
						else {
							Animation(2);
							direction = 3;
						}
					}
				}
				else if (downstair[0] == -1 && y % 44 == 0) {
					SetTopLeft(x + speed_x, y);
					if (map[y / 44][(x + speed_x) / 40] == 3) {
						Animation(5);
						direction = 4;
					}
					else {
						Animation(1);
						direction = 4;
					}
				}
				else if (downstair[1] == -1 && y % 44 == 0) {
					SetTopLeft(x - speed_x, y);
					if (map[y / 44][(x - speed_x) / 40] == 3) {
						Animation(6);
						direction = 3;
					}
					else {
						Animation(2);
						direction = 3;
					}
				}
				else {
					if ((downstair[0] < downstair[1]) && y % 44 == 0) {
						SetTopLeft(x - speed_x, y);
						if (map[y / 44][(x - speed_x) / 40] == 3) {
							Animation(6);
							direction = 3;
						}
						else {
							Animation(2);
							direction = 3;
						}
					}
					else if ((downstair[0] > downstair[1]) && y % 44 == 0) {
						SetTopLeft(x + speed_x, y);
						if (map[y / 44][(x + speed_x ) / 40] == 3) {
							Animation(5);
							direction = 4;
						}
						else {
							Animation(1);
							direction = 4;
						}
					}
					else if(y % 44 == 0){
						if (x > character.GetLeft()) {
							SetTopLeft(x - speed_x, y);
							if (map[y / 44][(x - speed_x) / 40] == 3) {
								Animation(6);
								direction = 3;
							}
							else {
								Animation(2);
								direction = 3;
							}
						}
						else {
							SetTopLeft(x + speed_x, y);
							if (map[y / 44][(x + speed_x) / 40] == 3) {
								Animation(5);
								direction = 4;
							}
							else {
								Animation(1);
								direction = 4;
							}
						}
					}
				}
			}
			else if (((y > character.GetTop() && up) || (y < character.GetTop() && !down && up)) && direction != 2) {
				if (upstair[0] == 0 && x % 40 == 0  && map[(y - speed_y) / 44][x / 40] != 0 && map[(y - speed_y) / 44][x / 40] != 5) {
					SetTopLeft(x, y - speed_y);
					Animation(3);
					direction = 1;
				}
				else if (upstair[0] == 0 && y % 44 == 0) {
					if (GetPos() == 1 || GetPos() == 5) {
						SetTopLeft(x + speed_x, y);
						if (map[y / 44][(x + speed_x) / 40] == 3) {
							Animation(5);
							direction = 4;
						}
						else {
							Animation(1);
							direction = 4;
						}
					}
					else if(GetPos() == 2 || GetPos() == 6) {
						SetTopLeft(x - speed_x, y);
						if (map[y / 44][(x - speed_x) / 40] == 3) {
							Animation(6);
							direction = 3;
						}
						else {
							Animation(2);
							direction = 3;
						}
					}
				}
				else if (upstair[0] == -1 && y % 44 == 0) {
					SetTopLeft(x + speed_x, y);
					if (map[y / 44][(x + speed_x) / 40] == 3) {
						Animation(5);
						direction = 4;
					}
					else {
						Animation(1);
						direction = 4;
					}
				}
				else if (upstair[1] == -1 && y % 44 == 0) {
					SetTopLeft(x - speed_x, y);
					if (map[y / 44][(x - speed_x) / 40] == 3) {
						Animation(6);
						direction = 3;
					}
					else {
						Animation(2);
						direction = 3;
					}
				}
				else if(y % 44 == 0){
					if (upstair[0] < upstair[1]) {
						SetTopLeft(x - speed_x, y);
						if (map[y / 44][(x - speed_x) / 40] == 3) {
							Animation(6);
							direction = 3;
						}
						else {
							Animation(2);
							direction = 3;
						}
					}
					else if (upstair[0] > upstair[1]) {
						SetTopLeft(x + speed_x, y);
						if (map[y / 44][(x + speed_x) / 40] == 3) {
							Animation(5);
							direction = 4;
						}
						else {
							Animation(1);
							direction = 4;
						}
					}
					else {
						if (x > character.GetLeft()) {
							SetTopLeft(x - speed_x, y);
							if (map[y / 44][(x - speed_x) / 40] == 3) {
								Animation(6);
								direction = 3;
							}
							else {
								Animation(2);
								direction = 3;
							}
						}
						else {
							SetTopLeft(x + speed_x , y);
							if (map[y / 44][(x + speed_x) / 40] == 3) {
								Animation(5);
								direction = 4;
							}
							else {
								Animation(1);
								direction = 4;
							}
						}
					}
				}
			}
			else {
				if (!up && !down) {
					//Animation(1);
					//direction = 4;
					if (y == character.GetTop()) {
						if (x > character.GetLeft() && map[y / 44][(x - speed_x) / 40] != 0 && map[y / 44][(x - speed_x) / 40] != 5 && !(map[y / 44][(x - speed_x) / 40] != 3 && map[y / 44 + 1][(x - speed_x) / 40] == 1)) {
							SetTopLeft(x - speed_x, y);
							if (map[y / 44][(x - speed_x) / 40] == 3) {
								Animation(6);
								direction = 3;
							}
							else {
								Animation(2);
								direction = 3;
							}
						}
						else if (x < character.GetLeft() && map[y / 44][(x + speed_x) / 40 + 1] != 0 && map[y / 44][(x + speed_x) / 40 + 1] != 5 && !(map[y / 44][(x + speed_x) / 40 + 1] != 3 && map[y / 44 + 1][(x + speed_x) / 40 + 1] == 1)) {
							SetTopLeft(x + speed_x, y);
							if (map[y / 44][(x + speed_x) / 40] == 3) {
								Animation(5);
								direction = 4;
							}
							else {
								Animation(1);
								direction = 4;
							}
						}
					}
					else if (map[y / 44][x / 40] == 3) {
						if (map[(y + speed_y) / 44][x / 40] == 0 || map[(y + speed_y) / 44][x / 40] == 5) {
							direction = 0;
						}
						else {
							SetTopLeft(x, y + speed_y);
							Animation(3);
							direction = 2;
						}
					}
					else if (y % 44 != 0) {
						if (direction == 1) {
							SetTopLeft(x, y - speed_y);
							Animation(3);
						}
						else if (direction == 2) {
							SetTopLeft(x, y + speed_y);
							Animation(3);
						}
					}
					
				}
				else {
					if (y % 44 != 0) {
						if (direction == 1) {
							SetTopLeft(x, y - speed_y);
							Animation(3);
						}
						else if (direction == 2) {
							SetTopLeft(x, y + speed_y);
							Animation(3);
						}
					}
					else if (x > character.GetLeft() && map[y / 44][(x - speed_x) / 40] != 0 && map[y / 44][(x - speed_x) / 40] != 5) {
						SetTopLeft(x - speed_x, y);
						if (map[y / 44][(x - speed_x) / 40] == 3) {
							Animation(6);
							direction = 3;
						}
						else {
							Animation(2);
							direction = 3;
						}
					}
					else if(x < character.GetLeft() && map[y / 44][(x - speed_x) / 40] != 0 && map[y / 44][(x - speed_x) / 40] != 5){
						SetTopLeft(x + speed_x, y);
						if (map[y / 44][(x + speed_x) / 40] == 3) {
							Animation(5);
							direction = 4;
						}
						else {
							Animation(1);
							direction = 4;
						}
					}
				}
			}
		}
	}
	void CMovingBitmap::DigLeft() {
		if (first) {
			recover = clock();
			first = false;
		}
		if (clock() - time_dig >= 100) {
			SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
			time_dig = clock();
		}
	}
	void CMovingBitmap::DigRight() {
		if (first) {
			recover = clock();
			first = false;
		}
		if (clock() - time_dig >= 100) {
			SetFrameIndexOfBitmap(GetFrameIndexOfBitmap() + 1);
			time_dig = clock();
		}
	}
	bool CMovingBitmap::IsDig() {
		if (!first) {
			return true;
		}
		else {
			return false;
		}
	}
	void CMovingBitmap::DigReset() {
		if (clock() - recover >= 9000) {
			SetFrameIndexOfBitmap(0);
			first = true;
		}
		else if (clock() - recover >= 8000) {
			SetFrameIndexOfBitmap(9);
		}
	}
	bool CMovingBitmap::IsCatch() {
		return catched;
	}
	void CMovingBitmap::ResetCatch() {
		catched = false;
	}
	void CMovingBitmap::EnemyReset() {
		direction = 0;
		pos = 0;
		SetFrameIndexOfBitmap(0);
	}
}         

