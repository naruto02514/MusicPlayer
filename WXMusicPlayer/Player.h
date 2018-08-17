#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <SDL2/SDL.h>
#include <mutex>
#include <thread>
#include <chrono>
#define __STDC_CONSTANT_MACROS

#ifdef _WIN32
//Windows
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswresample/swresample.h"
};
#else
//Linux...
#ifdef __cplusplus
extern "C"
{
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>

#ifdef __cplusplus
};
#endif
#endif

#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

static  Uint8  *audio_chunk;
static  Uint32  audio_len;
static  Uint8  *audio_pos;

/* The audio function callback takes the following parameters:
 * stream: A pointer to the audio buffer to be filled
 * len: The length (in bytes) of the audio buffer
*/
class Player{
public:
    std::string path;
    std::string name;
    double duration;
    Player();
    ~Player();
    int Music();
    void Play_force();
    void Stop();
    void Play();
    void Next();
    void Prev();
    int get_volume();
    void set_volume(int vol);
    void set_path(std::string input);
    int get_play_signal();
    std::string get_path();
private:
    std::thread Play_thread;
    int player_volume=128;
    std::mutex mutex;
    void set_play_signal(int signal);
	int play_signal=0; //0=stop 1=play 2=pause
};

extern Player player;
//int run_play(std::string input);
