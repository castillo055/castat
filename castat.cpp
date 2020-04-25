#include <chrono>
#include <thread>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

#include "config.h"
const fs::path pathToModules{ modules_dir };

struct timer{
	typedef std::chrono::steady_clock clock;
	typedef std::chrono::milliseconds millis;

	void reset(){ start = clock::now(); }

	[[nodiscard]] unsigned long long millis_elapsed() const{
		return std::chrono::duration_cast<millis>( clock::now() - start).count();
	}

	private: clock::time_point start = clock::now();
};

void refreshStatus(unsigned long dt){
	for(const auto& entry : fs::directory_iterator(pathToModules)){
		const auto filenameStr = entry.path().filename().string();
		
	}
}

int main(){
	timer t;
	std::chrono::milliseconds delay(10);

	unsigned long dt = 0;
	while(true){
		std::this_thread::sleep_for(delay);
		dt = t.millis_elapsed();
		if(dt>=99){
			refreshStatus(dt);
			t.reset();
		}
	}
	return 0;
}
