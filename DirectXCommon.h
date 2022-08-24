#pragma once

#include<wrl.h>
#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include"WinApp.h"
#include"imgui/imgui.h"
#include"imgui/imgui_impl_win32.h"
#include"imgui/imgui_impl_dx12.h"

class DirectXCommon
{
private: // �G�C���A�X
// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	void Initialize(WinApp* winApp);
	void InitializeDevice();
	void InitializeCommand();
	bool initializeImg();
	void InitializeSwapchain();
	void InitializeRenderTargetView();
	void InitializeDepthBuffer();
	void InitializeFence();
	void PreDraw();
	void PostDraw();

	ID3D12Device* GetDev() { return dev.Get(); }
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }
private:
	WinApp* winApp = nullptr;
	//�f�o�C�X
	ComPtr<ID3D12Device> dev;
	//DXGI�t�@�N�g��
	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<IDXGISwapChain4>swapchain;
	ComPtr<IDXGISwapChain1>swapchain1;
	ComPtr<ID3D12CommandAllocator>cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList>cmdList;
	ComPtr<ID3D12CommandQueue>cmdQueue;
	ComPtr<ID3D12DescriptorHeap>rtvHeaps;
	ComPtr<ID3D12DescriptorHeap>dsvHeap;
	ComPtr<ID3D12Resource> depthBuffer;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	//�t�F���X�̐���
	ComPtr<ID3D12Fence>fence;
	UINT64 fenceVal = 0;
	UINT bbIndex;

	//imgui�p�̃q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap>DescriptorHeapImg;
	//�q�[�v�ێ��p
	ComPtr<ID3D12DescriptorHeap>_heapForImg;

};

