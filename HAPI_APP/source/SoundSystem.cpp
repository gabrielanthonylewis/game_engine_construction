#include "SoundSystem.h"


bool SoundSystem::LoadSound(const std::string& fileName, const std::string& uniqueRefName)
{
	AddSound(uniqueRefName, fileName, _idCount);
	_idCount++;
	return HAPI.LoadSound(fileName);
}

bool SoundSystem::LoadMusic(const std::string& fileName, const std::string& uniqueRefName)
{
	AddMusic(uniqueRefName, fileName, _musicIdCount);
	_musicIdCount++;
	return true;
}


bool SoundSystem::PlaySound(const std::string& name)
{
	return HAPI.PlaySound(GetSound(name).filePath);
}

bool SoundSystem::PlayMusic(const std::string& name)
{
	if (StopCurrentMusic() == false)
		return false;

	_currentlyPlayingMusic = &_music[name];
	
	//if never played do this
	if (_music[name].initialised == false)
	{
		_music[name].initialised = true;
		return HAPI.PlayStreamedMedia(_music[name].filePath, _music[name].options, _music[name].id);
	}
	//else restart music
	else
		return HAPI.PlayStreamedMedia(_music[name].id);

}

bool SoundSystem::StopCurrentMusic()
{
	if (_currentlyPlayingMusic != nullptr)
	{
		MusicInfo* temp = _currentlyPlayingMusic;
		_currentlyPlayingMusic = nullptr;
		
		return HAPI.StopStreamedMedia(temp->id);
	}

	return true;
}

void SoundSystem::AddSound(const std::string& name, const std::string& filePath, unsigned int id)
{
	SoundInfo info;
	info.id = id;
	info.filePath = filePath;
	_sounds[name] = info;
}

void SoundSystem::AddMusic(const std::string& name, const std::string& filePath, unsigned int id)
{
	MusicInfo info;
	info.id = id;
	info.initialised = false;
	info.filePath = filePath;
	info.options.loop = true;
	_music[name] = info;
}

SoundInfo SoundSystem::GetSound(const std::string& name) const
{
	if (_sounds.find(name) == _sounds.end())
	{
		HAPI.UserMessage("Sound '" + name + "' not found! (attempting to access it)", "Error");
		return SoundInfo();
	}
	else
		return _sounds.at(name);
}

MusicInfo SoundSystem::GetMusic(const std::string& name) const
{
	if (_music.find(name) == _music.end())
	{
		HAPI.UserMessage("Music '" + name + "' not found! (attempting to access it)", "Error");
		return MusicInfo();
	}
	else
		return _music.at(name);
}