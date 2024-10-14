#include "pch.h"
#include "SE_TextureView.h"

#include <Engine/CKeyMgr.h>
#include "SE_Info.h"

SE_TextureView::SE_TextureView()
	: m_Texture(nullptr)
	, m_WidthSize(0.f)
	, m_WheelScale(1.f)
	, m_Ratio(0.f)
	, m_ImageRectMin{}
	, m_MousePos{}
	, m_MouseLT{}
	, m_MouseRB{}
	, m_PixelID{}
{
}

SE_TextureView::~SE_TextureView()
{
}

void SE_TextureView::Init()
{
}

void SE_TextureView::Update()
{
	if (nullptr == m_Texture)
		return;

	WheelCheck();

	// 이미지	
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	// 실제 해상도 대비 출력 Image 의 비율
	m_Ratio = (m_WidthSize * m_WheelScale) / m_Texture->Width();

	ImGui::Image(m_Texture->GetSRV().Get(), ImVec2((m_WidthSize * m_WheelScale), m_Texture->Height() * m_Ratio)
		, uv_min, uv_max, tint_col, border_col);

	// Select Check
	SelectCheck();
	

	// Draw Rect
	DrawRect();
}

void SE_TextureView::Deactivate()
{
	m_Texture = nullptr;

	m_ImageRectMin = {}; // Image 위젯 좌상단 Pos

	m_MousePos = {};     // 현재 Mouse Pos

	m_MouseLT = {};      // 마우스 위치에 해당하는 Texture 의 좌상단 픽셀 좌표
	m_MouseRB = {};      // 마우스 위치에 해당하는 Texture 의 우하단 픽셀 좌표

	m_PixelID.clear();      // 등록된 적이 있는 픽셀인지 확인하는 용도

	m_WidthSize = 0.f;
	m_WheelScale = 1.f;   // Mouse Wheel
	m_Ratio = 0.f;        // 실제 해상도 대비 출력 이미지의 비율
}

void SE_TextureView::WheelCheck()
{
	if (0 < ImGui::GetIO().MouseWheel)
		m_WheelScale += 0.05f;
	if (0 > ImGui::GetIO().MouseWheel)
		m_WheelScale -= 0.05f;

	if (3.f < m_WheelScale)
		m_WheelScale = 3.f;
	if (0.1f > m_WheelScale)
		m_WheelScale = 0.1f;
}

void SE_TextureView::SelectCheck()
{
	// Image 위젯의 좌상단 좌표
	m_ImageRectMin = ImGui::GetItemRectMin();

	// Mouse LBTN Check
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowFocused())
	{
		// 현재 Mouse Pos에 따른 Image의 픽셀 좌표
		Vec2 vPixelPos = Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
		ImVec2 vDiff = ImVec2(vPixelPos.x - m_ImageRectMin.x, vPixelPos.y - m_ImageRectMin.y);
		// 비율 값을 이용 해 Rect를 고정
		vPixelPos = Vec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);
		
		if (0.f <= vPixelPos.x && vPixelPos.x < m_Texture->Width() 
		 && 0.f <= vPixelPos.y && vPixelPos.y < m_Texture->Height())
		{
			CalcSpriteSize(vPixelPos);
		}
	}

	// Mouse RBTN Check
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowFocused())
	{
		Vec2 vPixelPos = Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
		ImVec2 vDiff = ImVec2(vPixelPos.x - m_ImageRectMin.x, vPixelPos.y - m_ImageRectMin.y);
		m_MouseLT = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
	{
		Vec2 vPixelPos = Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
		ImVec2 vDiff = ImVec2(vPixelPos.x - m_ImageRectMin.x, vPixelPos.y - m_ImageRectMin.y);
		m_MouseRB = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);
	}
}

void SE_TextureView::DrawRect()
{
	// 현재 픽셀 좌표의 좌상단, 우하단 값에 비율값을 적용 후 이미지의 좌상단 값을 더해준다.
	ImVec2 MouseLT = ImVec2(m_MouseLT.x * m_Ratio + m_ImageRectMin.x, m_MouseLT.y * m_Ratio + m_ImageRectMin.y);
	ImVec2 MouseRB = ImVec2(m_MouseRB.x * m_Ratio + m_ImageRectMin.x, m_MouseRB.y * m_Ratio + m_ImageRectMin.y);

	ImGui::GetWindowDrawList()->AddRect(MouseLT, MouseRB,
										ImGui::GetColorU32(ImVec4(0.f, 1.f, 0.f, 1.f)), 0.f, 0, 2.f);
}

void SE_TextureView::CalcSpriteSize(Vec2 _PixelPos)
{
	// 각 상황에 맞는 최악의 경우를 미리 계산
	float left = m_Texture->Width() - 1.f;
	float top = m_Texture->Height() - 1.f;
	float right = 0.f;
	float bot = 0.f;

	// 등록 아이디 검사용 Set Clear
	m_PixelID.clear();

	// Queue에 클릭한 최초 좌표를 넣어준다.
	list<Vec2> queue;
	queue.push_back(_PixelPos);
	
	while (!queue.empty())
	{
		Vec2 vPixelPos = queue.front();
		queue.pop_front();

		if (vPixelPos.x < left)
			left = vPixelPos.x;
		if (vPixelPos.x > right)
			right = vPixelPos.x;
		if (vPixelPos.y < top)
			top = vPixelPos.y;
		if (vPixelPos.y > bot)
			bot = vPixelPos.y;

		Vec2 vUp    = vPixelPos + Vec2(0.f, 1.f);   // 위
		Vec2 vDown  = vPixelPos + Vec2(0.f, -1.f);	// 아래
		Vec2 vLeft  = vPixelPos + Vec2(-1.f, 0.f);	// 좌
		Vec2 vRight = vPixelPos + Vec2(1.f, 0.f);	// 우

		if (IsPixelOk(vUp))
		{
			queue.push_back(vUp);
			m_PixelID.insert(vUp);
		}
		if (IsPixelOk(vDown))
		{
			queue.push_back(vDown);
			m_PixelID.insert(vDown);
		}
		if (IsPixelOk(vLeft))
		{
			queue.push_back(vLeft);
			m_PixelID.insert(vLeft);
		}
		if (IsPixelOk(vRight))
		{
			queue.push_back(vRight);
			m_PixelID.insert(vRight);
		}
	}

	m_MouseLT = ImVec2(left, top);
	m_MouseRB = ImVec2(right, bot);

	GetInfo()->SetInfo(tSEInfo{ Vec2(m_MouseLT.x, m_MouseLT.y), Vec2(m_MouseRB.x - m_MouseLT.x, m_MouseRB.y - m_MouseLT.y), Vec2(0.f,0.f), Vec2(0.f,0.f) });
}

bool SE_TextureView::IsPixelOk(Vec2 _PixelPos)
{
	// 해당 픽셀이 Texture 의 해상도를 벗어나는 경우
	if (0.f > _PixelPos.x || m_Texture->Width() <= _PixelPos.x
	 || 0.f > _PixelPos.y || m_Texture->Height() <= _PixelPos.y)
		return false;

	// 이미 등록 된적 있는 Pixel 이라면
	if (m_PixelID.end() != m_PixelID.find(_PixelPos))
		return false;
	
	// 픽셀의 알파 값이 0 이라면
	int PixelIdx = m_Texture->Width() * (int)_PixelPos.y + (int)_PixelPos.x;

	if (0 == m_Texture->GetPixels()[PixelIdx].a)
		return false;

	return true;
}

void SE_TextureView::SetInfo()
{
}

void SE_TextureView::SetTexture(Ptr<CTexture> _Texture)
{
	if (m_Texture == _Texture)
		return;

	m_Texture = _Texture;

	m_WidthSize = (float)m_Texture->Width();
}