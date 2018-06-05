#pragma once

#include <string>
#include <unordered_map>

#include <HAPI_lib.h> // Include the HAPI header to get access to all of HAPIs interfaces
using namespace HAPISPACE; // HAPI itself is wrapped in the HAPISPACE namespace

//To store a sound clip for reuse
struct SoundInfo
{
public:
	unsigned int id;
	std::string filePath;
};

// To store a music file for reuse
struct MusicInfo
{
public:
	unsigned int id;
	std::string filePath;
	HAPISPACE::HAPI_TStreamedMediaOptions options;
	bool initialised;
};

/*The sound system is a blackbox class that deals with 
adding, loading and playing music and sound files.
The system is designed to be efficent and get rid of a lot of
ugly code.*/
class SoundSystem
{
	static SoundSystem *s_instance;

public:
	SoundSystem() = default;
	inline ~SoundSystem() {};

	// The inital loading of a clip, this should only be done once
	// per file.
	bool LoadSound(const std::string& fileName, const std::string& uniqueRefName);
	bool LoadMusic(const std::string& fileName, const std::string& uniqueRefName);

	// Add to the map storage
	void AddSound(const std::string& name, const std::string& filePath, unsigned int id);
	void AddMusic(const std::string& name, const std::string& filePath, unsigned int id);

	// Getter functions (for the map storage)
	SoundInfo GetSound(const std::string& name) const;
	MusicInfo GetMusic(const std::string& name) const;

	// Simply play using HAPI.PlaySound()
	bool PlaySound(const std::string& name);
	// Plays the music ensuring clips are Restarted instead of reloaded
	bool PlayMusic(const std::string& name);

	// Stop the current music (can be played again as we have the ID)
	bool StopCurrentMusic();


	// Get the instance, if none exists then creat a new one
	inline static SoundSystem *instance()
	{
		if (!s_instance)
			s_instance = new SoundSystem;
		return s_instance;
	}

private:
	unsigned int _idCount = 0;
	unsigned int _musicIdCount = 0;

	// make struct and store ID and FilePath so only have to say "Play("blaster")"
	std::unordered_map<std::string, SoundInfo> _sounds;
	std::unordered_map<std::string, MusicInfo> _music;

	MusicInfo* _currentlyPlayingMusic = nullptr;
};

