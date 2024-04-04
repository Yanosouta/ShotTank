// スカイドーム描画コンポーネント [skyRenderer.cpp]
#include "skyRenderer.h"
#include "renderer.h"

void SkyRenderer::Draw()
{
	Renderer::SetDepthEnable(false);
	ModelRenderer::Draw();
	Renderer::SetDepthEnable(true);
}
