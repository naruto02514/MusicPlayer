#include "Player.h"
#include "Memory.h"

Player player;

void  fill_audio(void *udata,Uint8 *stream,int len){
	//SDL 2.0
	SDL_memset(stream, 0, len);
	if(audio_len<=0)
		return;
	len=(len>audio_len?audio_len:len);	/*  Mix  as  much  data  as  possible  */
	SDL_MixAudio(stream,audio_pos,len,player.get_volume());
	audio_pos += len;
	audio_len -= len;
}

Player::Player():Play_thread(){
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER))
    {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return;
    }
    Play_thread = std::thread(&Player::Music,this);
}

Player::~Player(){
    Stop();
    Play_thread.join();
    SDL_Quit();
}

int Player::get_volume(){
    std::lock_guard<std::mutex> guard(memory.mutex);
    return player_volume;
}

void Player::set_volume(int vol){
    std::lock_guard<std::mutex> guard(memory.mutex);
    player_volume = vol;
}

int Player::get_play_signal(){
    std::lock_guard<std::mutex> guard(memory.mutex);
    return play_signal;
}

void Player::set_play_signal(int signal){
    std::lock_guard<std::mutex> guard(memory.mutex);
    play_signal=signal;
}

void Player::Stop(){
    set_play_signal(0);
}

void Player::Next(){
    set_play_signal(4);
}

void Player::Prev(){
    set_play_signal(3);
}

void Player::Play_force(){
    int play_index=memory.get_shuffle_play_number();
    set_path(memory.get_play_list_path(play_index));
    set_play_signal(5);
}

void Player::Play(){
    int signal = get_play_signal();
    if(signal==2 || signal==0){
        set_play_signal(1);
    }
    else
        set_play_signal(2);
}

void Player::set_path(std::string input){
    std::lock_guard<std::mutex> guard(memory.mutex);
    path = input;
}

std::string Player::get_path(){
    std::lock_guard<std::mutex> guard(memory.mutex);
    return path;
}

int Player::Music(){
    while(1){
Start:
        int signal = get_play_signal();
        memory.reset_duration();
        while(signal==0){
            signal = get_play_signal();
            std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        }
        audio_chunk = 0;
        //Audio buffer length
        audio_len = 0;
        audio_pos = 0;
        AVFormatContext	*pFormatCtx;
        int				i, audioStream;
        AVCodecContext	*pCodecCtx;
        AVCodec			*pCodec;
        AVPacket		*packet;
        uint8_t			*out_buffer;
        AVFrame			*pFrame;
        SDL_AudioSpec wanted_spec;
        int ret;
        uint32_t len = 0;
        int64_t in_channel_layout;
        struct SwrContext *au_convert_ctx;
        int out_nb_samples;
        int out_sample_rate;
        int out_channels;
        int out_buffer_size;
        int got_picture;
        printf("Music Start\n");
        int index = 0;
        len = 0;
        av_register_all();
        avformat_network_init();
        std::string url = get_path();
        std::replace(url.begin(), url.end(), '\\', '/');
        //const char *url = path.c_str();
        pFormatCtx = avformat_alloc_context();
        //Open
        printf("avopen_input\n");
        if(avformat_open_input(&pFormatCtx,url.c_str(),NULL,NULL)!=0){
            printf("Couldn't open input stream.\n");
            avformat_close_input(&pFormatCtx);
            set_play_signal(0);
            goto Start;
        }
        // Retrieve stream information
        printf("avopen_stream info\n");
        if(avformat_find_stream_info(pFormatCtx,NULL)<0){
            printf("Couldn't find stream information.\n");
            avformat_close_input(&pFormatCtx);
            set_play_signal(0);
            goto Start;
        }
        // Dump valid information onto standard error
        av_dump_format(pFormatCtx, 0, url.c_str(), false);

        // Find the first audio stream
        audioStream=-1;
        for(i=0; i < pFormatCtx->nb_streams; i++)
            if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
                audioStream=i;
                break;
            }
        printf("avopen stream info 2\n");
        if(audioStream==-1){
            printf("Didn't find a audio stream.\n");
            avformat_close_input(&pFormatCtx);
            set_play_signal(0);
            goto Start;
        }
        for(i=0; i < pFormatCtx->nb_streams; i++)
            if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
                audioStream=i;
                break;
            }
        if(audioStream==-1){
            printf("Didn't find a audio stream.\n");
            avformat_close_input(&pFormatCtx);
            set_play_signal(0);
            goto Start;
        }
        printf("avopen_get meta\n");
        AVDictionaryEntry *artist = av_dict_get(pFormatCtx->metadata, "artist", NULL, 0);
        AVDictionaryEntry *title = av_dict_get(pFormatCtx->metadata, "title", NULL, 0);
        std::string sname,stitle;
        if (artist && artist->value){
            sname = artist->value;
            sname+="   ";
        }
        else{
            sname = "Unknown";
        }
        if (title && title->value){
            stitle = title->value;
            stitle+="   ";
        }
        else{
            stitle = "Unknown";
        }
        memory.set_name(sname);
        memory.set_title(stitle);

        // Get a pointer to the codec context for the audio stream
        pCodecCtx=pFormatCtx->streams[audioStream]->codec;

        // Find the decoder for the audio stream
        pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
        if(pCodec==NULL){
            printf("Codec not found.\n");
            avformat_close_input(&pFormatCtx);
            set_play_signal(0);
            goto Start;
        }

        // Open codec
        if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
            printf("Could not open codec.\n");
            avformat_close_input(&pFormatCtx);
            set_play_signal(0);
            goto Start;
        }

        packet=(AVPacket *)av_malloc(sizeof(AVPacket));
        av_init_packet(packet);

        //Out Audio Param
        uint64_t out_channel_layout=AV_CH_LAYOUT_STEREO;
        AVSampleFormat out_sample_fmt=AV_SAMPLE_FMT_S16;
        //Out Buffer Size
        out_nb_samples=pCodecCtx->frame_size;
        out_sample_rate=pCodecCtx->sample_rate;
        out_channels=av_get_channel_layout_nb_channels(out_channel_layout);
        if(out_nb_samples<=0){
            //sample_freq * channels * bits_per_sample / 8
            if(strcmp(pCodecCtx->codec->name,"alac")==0){
                out_nb_samples = 4096;
            }
            else if(strcmp(pCodecCtx->codec->name,"ape")==0){
                out_nb_samples = 4608;
            }
            else{
                out_nb_samples = 1024;
            }
            //out_nb_samples=3000;
        }
        memory.set_duration_total(pFormatCtx->duration * av_q2d(AV_TIME_BASE_Q));
        out_buffer_size=av_samples_get_buffer_size(NULL,out_channels ,out_nb_samples,out_sample_fmt, 1);
        out_buffer=(uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);
        pFrame=av_frame_alloc();
    //SDL------------------
        wanted_spec.freq = out_sample_rate;
        wanted_spec.format = AUDIO_S16SYS;
        wanted_spec.channels = out_channels;
        wanted_spec.silence = 0;
        wanted_spec.samples = out_nb_samples;
        wanted_spec.callback = fill_audio;
        wanted_spec.userdata = pCodecCtx;

        printf("sdl open audio\n");
        if (SDL_OpenAudio(&wanted_spec, NULL)<0){
            printf("can't open audio.\n");
            avformat_close_input(&pFormatCtx);
            set_play_signal(0);
            goto Start;
        }

        in_channel_layout=av_get_default_channel_layout(pCodecCtx->channels);
        au_convert_ctx = swr_alloc();
        au_convert_ctx=swr_alloc_set_opts(au_convert_ctx,out_channel_layout, out_sample_fmt, out_sample_rate,
            in_channel_layout,pCodecCtx->sample_fmt , pCodecCtx->sample_rate,0, NULL);
        swr_init(au_convert_ctx);

        //read album art
        AVPacket pkt;
        av_read_frame(pFormatCtx, &pkt);
        if(pkt.stream_index > 0){
            std::vector <unsigned char> tmp_img(pkt.data,pkt.data+pkt.size);
            memory.save_img(tmp_img);
        }
        else{
            memory.clear_img();
        }
        memory.set_update(1);
        av_free_packet(&pkt);
        signal=get_play_signal();
        //std::this_thread::sleep_for(std::chrono::microseconds(100));
        SDL_PauseAudio(0);

        while(signal!=0 && signal!=3 && signal!=4 && signal!=5){
            if(memory.get_jump_signal()==1){
                audio_chunk = 0;
                audio_len = 0;
                audio_pos = 0;
                memory.set_jump_signal(0);
                double jump = memory.get_jump();
                av_seek_frame(pFormatCtx,-1,((double)jump/(double)1000)*AV_TIME_BASE,AVSEEK_FLAG_BACKWARD);
            }
            if(av_read_frame(pFormatCtx, packet)<0){
                break;
            }
            if(packet->stream_index==audioStream){
                ret = avcodec_decode_audio4( pCodecCtx, pFrame,&got_picture, packet);
                if ( ret < 0 ) {
                    printf("Error in decoding audio frame.\n");
                    av_free_packet(packet);
                    set_play_signal(0);
                }
                if ( got_picture > 0 ){
                    swr_convert(au_convert_ctx,&out_buffer, MAX_AUDIO_FRAME_SIZE,(const uint8_t **)pFrame->data , pFrame->nb_samples);
                    double timeframe = packet->pts * av_q2d(pFormatCtx->streams[packet->stream_index]->time_base);
                    memory.set_duration(timeframe); //pts * av_q2d(st->time_base)
                    printf("index:%5d\t pts:%lld\t packet size:%d time_frame: %f\n",index,packet->pts,packet->size,timeframe);
                    index++;
                }
                while(audio_len>0)//Wait until finish
                    SDL_Delay(1);
                //Set audio buffer (PCM data)
                audio_chunk = (Uint8 *) out_buffer;
                //Audio buffer length
                audio_len = out_buffer_size;
                audio_pos = audio_chunk;

            }
            av_free_packet(packet);

            while(signal==2){
                signal = get_play_signal();
                std::this_thread::sleep_for (std::chrono::nanoseconds(10));
            }
            signal=get_play_signal();
        }
        swr_free(&au_convert_ctx);
        av_free(out_buffer);
        avcodec_close(pCodecCtx);
        avformat_close_input(&pFormatCtx);
        SDL_CloseAudio();//Close SDL
        int play_index=0;
        if(signal==1){
            memory.next_play_index();
            play_index = memory.get_shuffle_play_number();
            set_path(memory.get_play_list_path()[play_index]);
            if(memory.get_repeat()==0 && play_index==0){
                set_play_signal(0);
            }
        }
        else if(signal==3){
            memory.prev_play_index();
            play_index = memory.get_shuffle_play_number();
            set_path(memory.get_play_list_path()[play_index]);
            set_play_signal(1);
        }
        else if(signal==4){
            memory.next_play_index();
            play_index = memory.get_shuffle_play_number();
            set_path(memory.get_play_list_path()[play_index]);
            set_play_signal(1);
        }
        else if(signal==5){
            set_play_signal(1);
        }
    }
    printf("end\n");
}

