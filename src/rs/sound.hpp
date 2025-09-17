#pragma once
#include <miniaudio/miniaudio.h>
#include <string_view>

namespace rs {

	class Sound {

	public:
		Sound();

		void create(std::string_view fileName, ma_engine* audioEngine);
		void destroy();

		void play();
		void loop();
		void stop();

		bool isPlaying();
		bool isLooping();

	private:
		ma_sound source;
		bool playing, looping;
	};
}
