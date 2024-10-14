#pragma once

class CConstBuffer;

class CDevice
	: public CSingleton<CDevice>
{
	SINGLE(CDevice);
private:
	HWND							m_hWnd;
	Vec2							m_vResolution;

	ComPtr<ID3D11Device>			m_Device;		// DX11 객체 생성, GPU 메모리 할당
	ComPtr<ID3D11DeviceContext>		m_Context;		// Rendering

	ComPtr<IDXGISwapChain>			m_SwapChain;
	
	Ptr<CTexture>					m_RTTex;

	Ptr<CTexture>					m_DSTex;

	ComPtr<ID3D11RasterizerState>	m_RSState[(UINT)RS_TYPE::END];
	ComPtr<ID3D11DepthStencilState> m_DSState[(UINT)DS_TYPE::END];
	ComPtr<ID3D11BlendState>	    m_BSState[(UINT)BS_TYPE::END];

	ComPtr<ID3D11SamplerState>      m_Sampler[2];


	CConstBuffer*					m_arrCB[(UINT)CB_TYPE::END];
	
private:
	int CreateSwapChain();
	int CreateView();
	int CreateConstBuffer();
	int CreateRasterizerState();
	int CreateDepthStencilState();
	int CreateBlendState();
	int CreateSamplerState();

public:
	int Init(HWND _hWnd, UINT _Width, UINT _Height);
	Vec2 GetResolution() { return m_vResolution; }

	void Present() { m_SwapChain->Present(0, 0); }

public:				     
	CConstBuffer*			 GetConstBuffer(CB_TYPE _Type) { return m_arrCB[(UINT)_Type]; }
	ID3D11Device*			 GetDevice()				   { return m_Device.Get(); }
	ID3D11DeviceContext*	 GetContext()				   { return m_Context.Get(); }
	ID3D11RasterizerState*	 GetRSState(RS_TYPE _Type)	   { return m_RSState[(UINT)_Type].Get(); }
	ID3D11DepthStencilState* GetDSState(DS_TYPE _Type)	   { return m_DSState[(UINT)_Type].Get(); }
	ID3D11BlendState*	     GetBSState(BS_TYPE _Type)     { return m_BSState[(UINT)_Type].Get(); }
};
