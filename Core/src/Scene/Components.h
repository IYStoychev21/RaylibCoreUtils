#pragma once
#include "pch.h"
#include <glm/glm.hpp>
#include "Script/NativeScript.h"

namespace Core {
	enum class ShapeType
	{
		None = 0,
		Rectangle,
		Circle,
	};
	
	/// @brief Tag component for entities
	struct TagComponent
	{
		std::string Tag;

		TagComponent(std::string tag)
		{
			Tag = tag;
		}
	};

	/// @brief Transform component for entities (position and rotation)
	struct TransformComponent
	{
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		float Rotation = 0.0f;
		glm::vec3 Scale = {1.0f, 1.0f, 1.0f};
		ShapeType Shape = ShapeType::None;

		TransformComponent(glm::vec3 position, glm::vec3 scale, float rotation, ShapeType shape = ShapeType::None)
		{
			Position = position;
			Scale = scale;
			Rotation = rotation;
			Shape = shape;
		}
	};

	struct NativeScriptComponent
	{
		std::shared_ptr<NativeScript> Instance = nullptr;

		NativeScriptComponent(std::shared_ptr<NativeScript> instance, std::shared_ptr<Entity> entity)
		{
			Instance = instance;
			Instance->m_Entity = entity;
		}
	};

	struct ColorComponent
	{
		glm::vec4 Color = { 255, 255, 255, 255 };

		ColorComponent(glm::vec4 color)
		{
			Color = color;
		}
	};
}
