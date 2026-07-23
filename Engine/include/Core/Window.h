#pragma once

#include <string>

namespace AtlasEngine
{

	struct WindowProperties
	{
		std::string Title = "Atlas Engine";
		unsigned int Width = 1280;
		unsigned int Height = 720;
	};

	class Window
	{
	public:

		Window();
		~Window();

		bool Create(const WindowProperties& properties);

		void Update();

		bool ShouldClose() const;

	private:

	};

}