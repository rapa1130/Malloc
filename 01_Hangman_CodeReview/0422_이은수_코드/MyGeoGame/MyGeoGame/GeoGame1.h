#pragma once
#include "NzWndBase.h"

class DrawManager;

class GeoGame1 : public NzWndBase
{
public:
	GeoGame1() = default;
	~GeoGame1() override = default;

	bool Initialize();
	void Run();
	void Finalize();

private:
	void Update();
	void Render();

	void OnResize(int width, int height) override;
	void OnClose() override;

	void OnLButtonDown(int x, int y);
	void OnRButtonDown(int x, int y);

	// TODO : 필요시 FixedUpdate 추가
	void LogicUpdate();

private:
	HDC m_hFrontDC = nullptr;
	HDC m_hBackDC = nullptr;
	HBITMAP m_hBackBitmap = nullptr;
	HBITMAP m_hDefaultBitmap = nullptr;
	DrawManager* m_drawManager = nullptr;

	struct MOUSE_POS
	{
		int x = 0;
		int y = 0;

		bool operator!=(const MOUSE_POS& other) const
		{
			return (x != other.x || y != other.y);
		}
	};

	MOUSE_POS m_MousePos = { 0,0 };
	MOUSE_POS m_MousePosPrev = { 0,0 };
};