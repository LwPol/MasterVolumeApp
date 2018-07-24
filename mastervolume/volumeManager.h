#pragma once

#include <Windows.h>
#include <endpointvolume.h>

class DontYouDareToMute;

class VolumeManager {
	private:
		IAudioEndpointVolume *pEndpointVol;
		DontYouDareToMute *muteCallback;
	public:
		VolumeManager();

		~VolumeManager();

		int getMasterVolume() const;
		void setMasterVolume(int vol);
};