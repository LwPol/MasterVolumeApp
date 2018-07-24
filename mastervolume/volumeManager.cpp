#include "stdafx.h"
#include "volumeManager.h"
#include <mmdeviceapi.h>

class DontYouDareToMute : public IAudioEndpointVolumeCallback {
	LONG cRef;
	IAudioEndpointVolume *vol;
	public:
		DontYouDareToMute(IAudioEndpointVolume *volumeManager) :
			cRef(1),
			vol(volumeManager)
		{
		}

		ULONG STDMETHODCALLTYPE AddRef() {
			return InterlockedIncrement(&cRef);
		}

		ULONG STDMETHODCALLTYPE Release() {
			ULONG ulRef = InterlockedDecrement(&cRef);
			if (0 == ulRef) {
				delete this;
			}
			return ulRef;
		}

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID **ppvInterface) {
			if (IID_IUnknown == riid) {
				AddRef();
				*ppvInterface = (IUnknown*)this;
			}
			else if (__uuidof(IAudioEndpointVolumeCallback) == riid) {
				AddRef();
				*ppvInterface = (IAudioEndpointVolumeCallback*)this;
			}
			else {
				*ppvInterface = NULL;
				return E_NOINTERFACE;
			}
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnNotify(AUDIO_VOLUME_NOTIFICATION_DATA *pNotify) {
			if (pNotify == nullptr)
				return E_INVALIDARG;

			if (pNotify->bMuted)
				vol->SetMute(FALSE, nullptr);
			return S_OK;
		}
};

VolumeManager::VolumeManager() {
	bool bError = true;
	IMMDeviceEnumerator *pEnumerator;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
		__uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&pEnumerator));
	if (SUCCEEDED(hr)) {
		IMMDevice *pDevice;
		hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
		if (SUCCEEDED(hr)) {
			hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&pEndpointVol));
			if (SUCCEEDED(hr)) {
				bError = false;
				muteCallback = new DontYouDareToMute(pEndpointVol);
				pEndpointVol->RegisterControlChangeNotify(muteCallback);
			}
			pDevice->Release();
		}
		pEnumerator->Release();
	}

	if (bError)
		throw std::exception();
}

VolumeManager::~VolumeManager() {
	pEndpointVol->Release();
	muteCallback->Release();
}

int VolumeManager::getMasterVolume() const {
	float vol;
	pEndpointVol->GetMasterVolumeLevelScalar(&vol);
	return static_cast<int>(round(vol * 100));
}

void VolumeManager::setMasterVolume(int vol) {
	if (vol < 0)
		vol = 0;
	else if (vol > 100)
		vol = 100;

	pEndpointVol->SetMasterVolumeLevelScalar(static_cast<float>(vol) / 100, nullptr);
}
