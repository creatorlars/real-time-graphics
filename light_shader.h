#pragma once

class direct3d;
class object;
class camera;
class light;

class light_shader
{
public:
	explicit light_shader(direct3d const&);

	light_shader() = delete;
	~light_shader() = default;

	explicit light_shader(light_shader const&) = default;
	explicit light_shader(light_shader&&) = default;

	light_shader& operator=(light_shader const&) = default;
	light_shader& operator=(light_shader&&) = default;

	template<typename T>
	void render(T const &object, std::shared_ptr<camera> const &camera,
		std::vector<std::shared_ptr<light>> const &lights) const
	{
		HRESULT result{};
		auto const context = d3d_.context();

		//object->render();

		// Lock the constant buffer so it can be written to.
		auto mapped_resource = D3D11_MAPPED_SUBRESOURCE{};
		result = context->Map(matrix_buffer_.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U,
			&mapped_resource);
		if (FAILED(result))
		{
			throw "";
		}

		// Get a pointer to the data in the constant buffer.
		auto const matrix_buffer = static_cast<MATRICES*>(mapped_resource.pData);

		// Get matrices.
		auto const projection_matrix = XMLoadFloat4x4(&d3d_.projection_matrix());
		//auto const object_matrix = XMLoadFloat4x4(&object->matrix());
		auto const camera_matrix = XMLoadFloat4x4(&camera->matrix());

		// Copy the matrices into the constant buffer.
		matrix_buffer->projection = XMMatrixTranspose(projection_matrix);
		matrix_buffer->world = XMMatrixTranspose(object->render());
		matrix_buffer->view = XMMatrixTranspose(camera_matrix);

		// Unlock the constant buffer.
		context->Unmap(matrix_buffer_.Get(), 0U);

		// finally set the constant buffer in the vertex shader
		context->VSSetConstantBuffers(0U, 1U, matrix_buffer_.GetAddressOf());






		// Set shader texture resource in the pixel shader.
		auto const texture = object->model()->view();
		context->PSSetShaderResources(0U, 1U, texture.GetAddressOf());

		// Lock the transparent constant buffer so it can be written to.
		result = context->Map(light_buffer_.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U,
			&mapped_resource);
		if (FAILED(result))
		{
			throw "";
		}

		auto const light_buffer = static_cast<LightBufferType*>(mapped_resource.pData);
		light_buffer->count = static_cast<int>(lights.size());
		for (auto i = 0U; i < lights.size(); ++i)
		{
			// Get vectors
			auto const ambient_color = XMLoadFloat4(&lights[i]->ambient());
			auto const diffuse_color = XMLoadFloat4(&lights[i]->diffuse());
			auto const light_direction = XMLoadFloat3(&lights[i]->direction());

			// Copy the matrices into the constant buffer
			light_buffer->ambient[i] = ambient_color;
			light_buffer->diffuse[i] = diffuse_color;
			light_buffer->direction[i] = light_direction;
		}

		// Unlock the buffer.
		context->Unmap(light_buffer_.Get(), 0U);

		// Now set the texture translation constant buffer in the pixel shader with
		// the updated values.
		context->PSSetConstantBuffers(0U, 1U, light_buffer_.GetAddressOf());





		result = context->Map(light_position_buffer_.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U,
			&mapped_resource);
		if (FAILED(result))
		{
			throw "";
		}

		// Get a pointer to the data in the constant buffer.
		auto const light_position_buffer = static_cast<LightPositionBufferType*>(mapped_resource.pData);

		// Get positions.
		auto constexpr test = XMFLOAT4{ 3.f, 0.f, 0.f, 0.f };
		auto const test_position = XMLoadFloat4(&test);

		auto constexpr test2 = XMFLOAT4{ 1.f, 0.f, 0.f, 0.f };
		auto const test_position2 = XMLoadFloat4(&test2);

		// Copy the matrices into the constant buffer.
		light_position_buffer->position[0] = test_position;
		light_position_buffer->position[1] = test_position2;
		light_position_buffer->count = 2;

		// Unlock the constant buffer.
		context->Unmap(light_position_buffer_.Get(), 0U);

		// finally set the constant buffer in the vertex shader
		context->VSSetConstantBuffers(1U, 1U, light_position_buffer_.GetAddressOf());





		// Set the vertex input layout.
		context->IASetInputLayout(layout_.Get());

		// Set the vertex and pixel shaders used to render this triangle.
		context->VSSetShader(vertex_shader_.Get(), nullptr, 0U);
		context->PSSetShader(pixel_shader_.Get(), nullptr, 0U);

		// Set the sampler state in the pixel shader.
		context->PSSetSamplers(0U, 1U, sampler_state_.GetAddressOf());

		// Render the triangle.
		context->DrawIndexed(object->model()->index_count(), 0U, 0);
	}

private:
	struct MATRICES
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

#define MAXLIGHTS 10
	struct LightPositionBufferType
	{
		XMVECTOR position[MAXLIGHTS];
		int count;
	};

	struct LightBufferType
	{
		XMVECTOR ambient[MAXLIGHTS];
		XMVECTOR diffuse[MAXLIGHTS];
		XMVECTOR direction[MAXLIGHTS];
		int count;
	};

	ComPtr<ID3D11VertexShader> vertex_shader_ = nullptr;
	ComPtr<ID3D11PixelShader> pixel_shader_ = nullptr;
	ComPtr<ID3D11InputLayout> layout_ = nullptr;
	ComPtr<ID3D11SamplerState> sampler_state_ = nullptr;
	ComPtr<ID3D11Buffer> matrix_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> light_buffer_ = nullptr;
	ComPtr<ID3D11Buffer> light_position_buffer_ = nullptr;

	direct3d const& d3d_;
};