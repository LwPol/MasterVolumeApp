#pragma once

#include <chrono>
#include <limits>

class FPSCounter {
	private:
		std::chrono::time_point<std::chrono::system_clock> lastUpdate;
		float fFrameInterval;
	public:
		FPSCounter() :
			lastUpdate(std::chrono::system_clock::now()),
			fFrameInterval(0.f)
		{
		}

		void update() {
			auto current = std::chrono::system_clock::now();
			std::chrono::duration<float> timeElapsed = current - lastUpdate;
			lastUpdate = current;

			fFrameInterval = timeElapsed.count();
		}

		float sof(float fVal) {
			return fVal * fFrameInterval;
		}

		int getFPS() {
			if (fFrameInterval != 0.f)
				return static_cast<int>(1.f / fFrameInterval);

			return std::numeric_limits<int>::max();
		}
};