#include <rs/sound.hpp>
#include <util.hpp>

using namespace rs;

Sound::Sound() : source({}), playing(false), looping(false) {}

void Sound::create(std::string_view fileName, ma_engine* audioEngine) {
	ma_result result = ma_sound_init_from_file(audioEngine, fileName.data(), 0, NULL, NULL, &source);
	CRITICAL_ASSERT(result == MA_SUCCESS, "Failed to create a sound from %s", fileName.data());
}

void Sound::destroy() {
	ma_sound_uninit(&source);
	source = {};
	playing = false;
	looping = false;
}

void Sound::play() {
	stop();
	playing = true;
	ma_sound_start(&source);
}

void Sound::loop() {
	stop();
	playing = true;
	looping = true;
	ma_sound_set_looping(&source, MA_TRUE);
	ma_sound_start(&source);
}

void Sound::stop() {
	playing = false;
	looping = false;

	ma_sound_stop(&source);
	ma_sound_set_looping(&source, MA_FALSE);
	ma_sound_seek_to_pcm_frame(&source, 0);
}

bool Sound::isPlaying() {
	playing = ma_sound_is_playing(&source) == MA_TRUE;
	return playing;
}

bool Sound::isLooping() {
	return looping;
}
