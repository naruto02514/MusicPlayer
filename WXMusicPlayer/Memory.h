#include <string>
#include <iostream>
#include <stdio.h>
#include <mutex>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <string.h> // memcpy
class Memory{
public:
    Memory();
    ~Memory();
    std::string get_name();
    void set_name(std::string name);
    std::string get_title();
    void set_title(std::string title);
    void set_update(int update);
    int get_update();
    void append_list(std::string path,std::string name);
    void append_list(const char* path_c,const char* name_c);
    std::vector <std::string> get_play_list_path();
    std::vector <std::string> get_play_list();
    void delete_list(int index);
    void set_list_update(int update);
    int get_list_update();
    std::string get_list_path_index(int index);
    void set_play_index(int index);
    int get_play_index();
    void next_play_index();
    void prev_play_index();
    std::string get_play_list_path(int index);
    int get_play_list_size();
    void save_img(std::vector <unsigned char> img_input);
    void clear_img();
    std::vector <unsigned char> get_img();
    void clear_list();
    void set_repeat(int input);
    int get_repeat();
    void set_shuffle(int input);
    int get_shuffle();
    std::mutex mutex;
    void set_duration_total(double input);
    void add_duration(double input);
    long long int get_duration_total();
    long long int get_duration_now();
    void set_duration(double input);
    void reset_duration();

    double get_jump();
    void set_jump(long long int input);
    int get_jump_signal();
    void set_jump_signal(int input);
    void shuffle_play_list();
    void shuffle_play_list(int input);
    int get_shuffle_play_number();
private:
    std::vector <int> shuffle_list;
    std::vector <unsigned char> cover_img;
    std::string Name;
    std::string Title;
    int playing_index=0;
    double total_duration=0;
    double duration=0;
    int Update;
    int update_list=0;
    std::vector <std::string> Play_List;
    std::vector <std::string> Play_List_Path;
    int repeat=0;
    int shuffle=0;
    double jump=0;
    int jump_signal=0;
};

extern Memory memory;


