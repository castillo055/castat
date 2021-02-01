#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include <alsa/asoundlib.h>
#include <filesystem>
#include "ctpl_stl.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

static Display *dpy;
static int screen;
static Window root;

#define LENGTH(X) 	(sizeof X / sizeof X[0])

typedef struct {
	const char *path;
	double period;
} Module;

typedef struct {
	char cache[8096];
	double age = 0.0;
	string module_path;
	bool init = false;
	future<void> result;
	unsigned long exec_time;
} Cache;

#include "../config.h"

Cache cache[LENGTH(modules)];
ctpl::thread_pool pool(8);

struct timer{
	typedef chrono::steady_clock clock;
	typedef chrono::milliseconds millis;

	void reset(){ start = clock::now(); }

	[[nodiscard]] unsigned long long millis_elapsed() const{
		return chrono::duration_cast<millis>( clock::now() - start).count();
	}

	private: clock::time_point start = clock::now();
};

static bool debug = false;

static string run(string cmd) {
	string data;
	FILE* stream;
	const int max_buffer = 1024;
	char buffer[max_buffer];
	cmd.append(" 2>&1");

	stream = popen(cmd.c_str(), "r");
	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != nullptr) data.append(buffer);
		pclose(stream);
	}
	return data;
}

int getALSAVolume(){
	long min, max;
	snd_mixer_t *handle;
	snd_mixer_selem_id_t *sid;
	const char *card = "default";
	const char *selem_name = "Master";

	snd_mixer_open(&handle, 0);
	snd_mixer_attach(handle, card);
	snd_mixer_selem_register(handle, NULL, NULL);
	snd_mixer_load(handle);

	snd_mixer_selem_id_alloca(&sid);
	snd_mixer_selem_id_set_index(sid, 0);
	snd_mixer_selem_id_set_name(sid, selem_name);
	snd_mixer_elem_t* elem = snd_mixer_find_selem(handle, sid);

	snd_mixer_selem_get_playback_volume_range(elem, &min, &max);
	long vol;
	snd_mixer_selem_get_playback_volume(elem, SND_MIXER_SCHN_FRONT_LEFT, &vol);

	snd_mixer_close(handle);

	return (int)(vol*100/max);
}
int getBrightness(){
	string curBr;
	string maxBr;

	ifstream brightness("/sys/class/backlight/intel_backlight/brightness");
	ifstream max_brightness("/sys/class/backlight/intel_backlight/max_brightness");

	getline(brightness, curBr);
	getline(max_brightness, maxBr);

	brightness.close();
	max_brightness.close();

	int current = stoi(curBr);
	int max = stoi(maxBr);

	return current*100/max;
}

string full_cache;
static void set_bar(const string str){
	if(str == full_cache)
		return;

    full_cache = str;

	XStoreName(dpy, root, str.c_str());
	XFlush(dpy);
}

void refresh_module(string module_path, int id) {
    timer t;
    string tmp = run(module_path);
    strcpy(cache[id].cache, tmp.c_str());
    cache[id].exec_time = t.millis_elapsed();

    if(debug) {
        char exec_time[64];
        sprintf(exec_time, "^c#117711^%4dms ^A0,5,16,16,0,%d^^f18^^d^", cache[id].exec_time, (int)(360.0 * cache[id].exec_time * 0.001 / modules[id].period));
        strcat(cache[id].cache, exec_time);
    }
}

static void refresh_status(const unsigned long dt, timer* t){
    const Module *m;
	int i;
	string str;
	const double deltaT = (double)dt/1000.0;

	for(i = 0; i < LENGTH(modules); i++){
		m = &modules[i];
		Cache* c = &cache[i];
		c->module_path = m->path;

		string module_path(string(modules_dir) + m->path);

		if (!c->init || ((c->age += deltaT) >= m->period) && c->result.wait_for(chrono::seconds(0)) == future_status::ready) {
		    c->init = true;
            c->result = pool.push([module_path, i](int) mutable { refresh_module(module_path, i); });
            c->age = 0.0;
        }

        str += c->cache;
        str += delim;
	}

	str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    set_bar(str);
}

int main(int argc, char* argv[]){
    po::options_description desc("Options");
    desc.add_options()
            ("help,h", "Print usage")
            ("debug,d", "Enable debug graphics")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if (vm.count("help")) {
        cout << desc << "\n";
        return 0;
    }

    if (vm.count("debug")) {
        debug = true;
    }

	if(!(dpy = XOpenDisplay(nullptr))) {
	    cerr << "Could not connect to display." << endl;
	    exit(1);
	}
	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);

	timer t;
	chrono::milliseconds delay(30);

	unsigned long dt;
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
	while(true){
		this_thread::sleep_for(delay);
		dt = t.millis_elapsed();
		if(dt>=30){
            //t.reset();
            refresh_status(dt, &t);
			//cout << t.millis_elapsed() << endl;
			t.reset();
		}
	}
#pragma clang diagnostic pop

	return 0;
}
