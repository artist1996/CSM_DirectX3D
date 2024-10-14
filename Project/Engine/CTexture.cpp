#include "pch.h"
#include "CTexture.h"
#include "CDevice.h"

CTexture::CTexture()
	: CAsset(ASSET_TYPE::TEXTURE)
	, m_Desc{}
	, m_RecentBindingRegisterNum(0)
{
}

CTexture::~CTexture()
{
}

int CTexture::Load(const wstring& _FilePath)
{
	path Filepath = _FilePath;
	path Extension = Filepath.extension();

	HRESULT hr = S_OK;

	// *.dds
	if (Extension == L".dds" || Extension == L".DDS")
	{
		hr = LoadFromDDSFile(_FilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
	}

	// *.tga
	else if (Extension == L".tga" || Extension == L".TGA")
	{
		hr = LoadFromTGAFile(_FilePath.c_str(), nullptr, m_Image);
	}

	// Window Image Component (*.bmp, *.png, *.jpg, *.jpeg )
	else if (Extension == L".bmp" || Extension == L".BMP"
		  || Extension == L".png" || Extension == L".PNG"
		  || Extension == L".jpg" || Extension == L".JPG"
		  || Extension == L".jpeg"|| Extension == L".JPEG")
	{
		hr = LoadFromWICFile(_FilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	// 알 수 없는 포맷일 경우
	else
	{
		return E_FAIL;
	}

	// 로딩이 실패했다면
	if (FAILED(hr))
	{
		errno_t ErrNum = GetLastError();
		wchar_t szBuff[255] = {};
		wsprintf(szBuff, L"Error Num : %d", ErrNum);
		MessageBox(nullptr, szBuff, L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	// 쉐이더 리소스뷰 생성
	CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	// 쉐이더 리소스뷰로 Texture 2D 가져오기
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());

	// Texture 2D 에서 Desc 정보 가져오기
	m_Tex2D->GetDesc(&m_Desc);
	SetKey(_FilePath);

	return S_OK;
}

int CTexture::Create(UINT _Width, UINT _Height, DXGI_FORMAT _PixelFormat, UINT _Flags, D3D11_USAGE _Usage)
{
	m_Desc.Width = _Width;			// DepthStencil 텍스쳐는 렌더타겟 해상도와 반드시 일치해야한다.
	m_Desc.Height = _Height;
	m_Desc.Format = _PixelFormat;	// Depth 24bit, Stencil 8bit
	m_Desc.ArraySize = 1;
	m_Desc.BindFlags = _Flags;

	m_Desc.Usage = _Usage;			// System Memory 와의 연계 설정
	
	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == _Usage)
	{
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	else
	{
		m_Desc.CPUAccessFlags = 0;
	}

	m_Desc.MiscFlags = 0;
	m_Desc.MipLevels = 1;			// 열화버전 해상도 이미지 추가 생성

	m_Desc.SampleDesc.Count = 1;
	m_Desc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf())))
	{
		return E_FAIL;
	}

	// View 생성
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	}
	
	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());
	}
	
	return S_OK;
}

int CTexture::Create(ComPtr<ID3D11Texture2D> _Tex2D)
{
	m_Tex2D = _Tex2D;
	m_Tex2D->GetDesc(&m_Desc);

	// View 생성
	if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
	{
		DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
	{
		DEVICE->CreateShaderResourceView(m_Tex2D.Get(), nullptr, m_SRV.GetAddressOf());
	}

	if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
	{
		DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), nullptr, m_UAV.GetAddressOf());
	}

	return S_OK;
}

void CTexture::Binding(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Binding_CS_SRV(UINT _RegisterNum)
{
	m_RecentBindingRegisterNum = _RegisterNum;
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::Binding_CS_UAV(UINT _RegisterNum)
{
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
	m_RecentBindingRegisterNum = _RegisterNum;
}

void CTexture::Clear(UINT _RegisterNum)
{
	ID3D11ShaderResourceView* pRSV = nullptr;
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pRSV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pRSV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pRSV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pRSV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pRSV);
}

void CTexture::Clear_CS_SRV()
{
}

void CTexture::Clear_CS_UAV()
{
	UINT i = -1;
	ID3D11UnorderedAccessView* pUAV  = nullptr;
	CONTEXT->CSSetUnorderedAccessViews(m_RecentBindingRegisterNum, 1, &pUAV, &i);
}

int CTexture::Save(const wstring& _FilePath)
{
	return 0;
}
