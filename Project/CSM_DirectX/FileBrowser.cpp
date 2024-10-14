#include "pch.h"
#include "FileBrowser.h"

#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>

#include "TreeUI.h"

FileBrowser::FileBrowser()
	: m_CurrentDirectory{}
{
	SetMove(true);
}

FileBrowser::~FileBrowser()
{
}

void FileBrowser::Init()
{
	m_CurrentDirectory = CPathMgr::GetInst()->GetContentPath();
	m_strContentPath   = CPathMgr::GetInst()->GetContentPath();
	m_BrowserTex	   = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\filebrowser.png");
	m_IconTex		   = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\fileicon.png");
}

void FileBrowser::Update()
{
	if (m_CurrentDirectory != std::filesystem::path(m_strContentPath))
	{
		if (ImGui::Button(".."))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}
	}
	
	static float padding = 16.f;
	static float thumbnailSize = 60.f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& iter : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = iter.path();
		auto RelativePath = std::filesystem::relative(path, m_strContentPath);
		//string strRelativePath = RelativePath.string();
		string strFileName = RelativePath.filename().string();
	
		//Ptr<CTexture> Icon = iter.is_directory() ? m_BrowserTex : m_IconTex;

		if(iter.is_directory())
			ImGui::ImageButton((void*)m_BrowserTex->GetSRV().Get(), { thumbnailSize ,thumbnailSize }, { 0,0 }, { 1,1 });
		else
		{
			if (L".png" == RelativePath.extension() || L".bmp" == RelativePath.extension()
				|| L".jpg" == RelativePath.extension() || L".TGA" == RelativePath.extension())
			{
				Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(RelativePath.wstring());
				ImGui::ImageButton((void*)pTex->GetSRV().Get(), { thumbnailSize ,thumbnailSize }, { 0,0 }, { 1,1 });
			}
			else
				ImGui::ImageButton((void*)m_IconTex->GetSRV().Get(), { thumbnailSize ,thumbnailSize }, { 0,0 }, { 1,1 });
		}

		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* szItem = RelativePath.c_str();

			ImGui::SetDragDropPayload("FILE_BROWSER_ITEM", szItem, wcslen(szItem) * sizeof(const wchar_t*));

			ImGui::EndDragDropSource();
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (iter.is_directory())
				m_CurrentDirectory /= path.filename();
		}

		ImGui::TextWrapped(strFileName.c_str());

		ImGui::NextColumn();
	}

	ImGui::Columns(1);

	//ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	//ImGui::SliderFloat("Padding", &padding, 0, 32);
}

ASSET_TYPE FileBrowser::FindAssetType(const std::filesystem::path& _Path)
{
	path ext = _Path.extension();

	if (ext == L".mesh")
		return ASSET_TYPE::MESH;
	//else if (ext == L".mdat")
		//CAssetMgr::GetInst()->Load<CMeshData>(_Path, _Path);
	else if (ext == L".mtrl")
		return ASSET_TYPE::MATERIAL;
	else if (ext == L".pref")
		return ASSET_TYPE::PREFAB;
	else if (ext == L".png" || ext == L".jpg" || ext == L".jpeg" || ext == L".bmp" || ext == L".dds" || ext == L".tga"
		|| ext == L".PNG" || ext == L".JPG" || ext == L".JPEG" || ext == L".BMP" || ext == L".DDS" || ext == L".TGA")
		return ASSET_TYPE::TEXTURE;
	else if (ext == L".mp3" || ext == L".mp4" || ext == L".ogg" || ext == L".wav"
		|| ext == L".MP3" || ext == L".MP4" || ext == L".OGG" || ext == L".WAV")
		return ASSET_TYPE::SOUND;
	else if (ext == L".sprite")
		return ASSET_TYPE::SPRITE;
	else if (ext == L".anim")
		return ASSET_TYPE::ANIMATION;
}