#include "Memory.h"

Memory memory;
Memory::Memory(){srand(time(NULL));};
Memory::~Memory(){};
std::string Memory::get_name(){
    std::lock_guard<std::mutex> guard(mutex);
    return Name;
}

void Memory::set_name(std::string name){
    std::lock_guard<std::mutex> guard(mutex);
    Name = name;
}

std::string Memory::get_title(){
    std::lock_guard<std::mutex> guard(mutex);
    return Title;
}

void Memory::set_title(std::string title){
    std::lock_guard<std::mutex> guard(mutex);
    Title = title;
}

void Memory::set_update(int update){
    std::lock_guard<std::mutex> guard(mutex);
    Update=update;
}

int Memory::get_update(){
    std::lock_guard<std::mutex> guard(mutex);
    return Update;
}

void Memory::set_list_update(int update){
    std::lock_guard<std::mutex> guard(mutex);
    update_list=update;
}

int Memory::get_list_update(){
    std::lock_guard<std::mutex> guard(mutex);
    return update_list;
}

void Memory::append_list(std::string path,std::string name){
    std::lock_guard<std::mutex> guard(mutex);
    if(path.empty() || name.empty())
        return;
    std::replace(path.begin(), path.end(), '\\', '/');
    Play_List.push_back(name);
    Play_List_Path.push_back(path);
}

void Memory::append_list(const char* path_c,const char* name_c){
    std::lock_guard<std::mutex> guard(mutex);
    std::string path = path_c;
    std::string name = name_c;
    if(path.empty() || name.empty())
        return;
    std::replace(path.begin(), path.end(), '\\', '/');
    Play_List.push_back(name);
    Play_List_Path.push_back(path);

}

std::vector <std::string> Memory::get_play_list_path(){
    std::lock_guard<std::mutex> guard(mutex);
    return Play_List_Path;
}

std::string Memory::get_play_list_path(int index){
    std::lock_guard<std::mutex> guard(mutex);
    return Play_List_Path[index];
}

std::vector <std::string> Memory::get_play_list(){
    std::lock_guard<std::mutex> guard(mutex);
    return Play_List;
}

int Memory::get_play_list_size(){
    std::lock_guard<std::mutex> guard(mutex);
    return Play_List.size();
}

std::string Memory::get_list_path_index(int index){
    std::lock_guard<std::mutex> guard(mutex);
    return Play_List_Path[index];
}

void Memory::delete_list(int index){
    std::lock_guard<std::mutex> guard(mutex);
    Play_List.erase(Play_List.begin() + index);
    Play_List_Path.erase(Play_List_Path.begin() + index);
}

void Memory::clear_list( ){
    std::lock_guard<std::mutex> guard(mutex);
    Play_List.clear();
    Play_List_Path.clear();
}

void Memory::set_play_index(int index){
    std::lock_guard<std::mutex> guard(mutex);
    playing_index=index;
}

void Memory::next_play_index(){
    int max_int = get_play_list_size()-1;
    int now = get_play_index()+1;
    if(now>max_int)
        set_play_index(0);
    else
        set_play_index(now);
}

void Memory::prev_play_index(){
    int max_int = get_play_list_size()-1;
    int now = get_play_index()-1;
    if(now<0)
        set_play_index(max_int);
    else
        set_play_index(now);
}

int Memory::get_play_index(){
    std::lock_guard<std::mutex> guard(mutex);
    return playing_index;
}

void Memory::shuffle_play_list(){
    int size_list = get_play_list_size();
    int random = get_shuffle();
    std::lock_guard<std::mutex> guard(mutex);
    if(size_list>0){
        shuffle_list.clear();
        for(int i=0;i<size_list;i++){
            shuffle_list.push_back(i);
        }
        if(random==1){
            std::mt19937 rng(std::random_device{}());
            std::shuffle(shuffle_list.begin(),shuffle_list.end(),rng);
        }
    }
}

void Memory::shuffle_play_list(int input){
    int size_list = get_play_list_size();
    int random = get_shuffle();
    std::lock_guard<std::mutex> guard(mutex);
    if(size_list > 0){
        shuffle_list.clear();
        for(int i=0;i<size_list;i++){
                shuffle_list.push_back(i);
        }
        if(random==1){
            shuffle_list[input]=0;
            shuffle_list[0]=input;
            std::mt19937 rng(std::random_device{}());
            std::shuffle(shuffle_list.begin()+1,shuffle_list.end(),rng);
        }
    }
}

int Memory::get_shuffle_play_number(){
    int play_now = get_play_index();
    std::lock_guard<std::mutex> guard(mutex);
    return shuffle_list[play_now];
}

void Memory::save_img(std::vector <unsigned char> img_input){
    std::lock_guard<std::mutex> guard(mutex);
    cover_img = img_input;
}

std::vector <unsigned char> Memory::get_img(){
    std::lock_guard<std::mutex> guard(mutex);
    return cover_img;
}

void Memory::clear_img(){
    std::lock_guard<std::mutex> guard(mutex);
    cover_img.clear();
}

void Memory::set_repeat(int input){
    std::lock_guard<std::mutex> guard(mutex);
    repeat = input;
}

int Memory::get_repeat(){
    std::lock_guard<std::mutex> guard(mutex);
    return repeat;
}

void Memory::set_shuffle(int input){
    std::lock_guard<std::mutex> guard(mutex);
    shuffle = input;
}

int Memory::get_shuffle(){
    std::lock_guard<std::mutex> guard(mutex);
    return shuffle;
}

void Memory::set_duration_total(double input){
     std::lock_guard<std::mutex> guard(mutex);
     total_duration = input;
}

void Memory::set_duration(double input){
    std::lock_guard<std::mutex> guard(mutex);
    duration = input;
}

void Memory::reset_duration(){
    std::lock_guard<std::mutex> guard(mutex);
    duration=0.0;
}

long long int Memory::get_duration_total(){
     std::lock_guard<std::mutex> guard(mutex);
     long long d = (long long)(total_duration * 100);
     return d;
}
long long int Memory::get_duration_now(){
     std::lock_guard<std::mutex> guard(mutex);
        long long d = (long long)(duration * 100);
     return d;
}

double Memory::get_jump(){
    std::lock_guard<std::mutex> guard(mutex);
    //double ret=(double)jump/100.00;
    return jump;
}

void Memory::set_jump(long long int input){
    std::lock_guard<std::mutex> guard(mutex);
    jump = (double)input;
}

int Memory::get_jump_signal(){
    std::lock_guard<std::mutex> guard(mutex);
    return jump_signal;
}

void Memory::set_jump_signal(int input){
    std::lock_guard<std::mutex> guard(mutex);
    jump_signal=input;
}
