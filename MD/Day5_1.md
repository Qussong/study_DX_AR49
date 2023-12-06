정리

## [ Device Class ]

### init( )
- CreateDevice()
    - <span style='color:yellow'>Device</span>
    - <span style='color:yellow'>DeviceContext</span>
- CreateSwapChain()
    - IDXGI Device
    - IDXGI Adapter
    - IDXGI Factory
    - <span style='color:yellow'>SwapChain</span>
- CreateTargetView()
    - RenderTarget Texture (from SwapChain)
    - <span style='color:yellow'>RenderTarget Texture View</span>
    - DepthStencil Texture (D3D11_TEXTURE_DESC.BindFlags = D3D11_BIND_DEPTH_STENCIL)
    - <span style='color:yellow'>DepthStencil Texture View</span>
    - <span style='color:violet'>CONTEXT->OMSetRenderTargets()</span>

### ClearRenderTarget( )
- <span style='color:violet'>CONTEXT->ClearRenderTargetView()</span>
- <span style='color:violet'>CONTEXT->ClearDepthStencilView()</span>
- viewport (D3D11_VIEWPORT)
- <span style='color:violet'>CONTEXT->RSSetViewports()</span>

### Present( )
- SWAPCHAIN->Present()

## [ Test Class ]

**_member :_**<br>
<span style='color:yellowgreen'>
    vertex[3]<br>
    VertexBuffer<br>
    VertexShaderBlob<br>
    VertexShader<br>
    layout<br>
    PixelShaderBlob<br>
    PixelShader<br>
    ErrorBlob<br>
</span>

### init( )
- Set Vertex of Triangle
- CreateVertexBuffer()
    - VertexBuffer
- CreateVertexShader()
    - D3DCompileFromFile()
    - VertexShaderBlob || ErrorBlob
    - VertexShader
- CreateInputLayout()
    - InpuyLayout (require VSBlob)
- CreatePixelShader()
    - D3DCompileFromFile()
    - PixelShaderBlob || ErrorBlob
    - PixelShader

### progress( )
- ClearRenderTarget()
    - <span style='color:violet'>CONTEXT->ClearRenderTargetView()</span>
    - <span style='color:violet'>CONTEXT->ClearDepthStencilView()</span>
    - <span style='color:violet'>CONTEXT->RSSetViewports()</span>
- <span style='color:violet'>CONTEXT->IASetVertexBuffer()</span>
- <span style='color:violet'>CONTEXT->IASetPrimitiveTopology()</span>
- <span style='color:violet'>CONTEXT->IASetInputLayout()</span>
- <span style='color:violet'>CONTEXT->VSSetShader()</span>
- <span style='color:violet'>CONTEXT->PSSetShader()</span>
- <span style='color:violet'>CONTEXT->Draw()</span>
- Present()
    - <span style='color:pink'>SWAPCHAIN->Present()</span>