#include <VideoPlayer.h>
#include <raylib.h> 
#include "../debug/debug.h"
#include <switch.h>
// Load frame data to image
// load texture from image
// render texture
bool ffinit = false;
bool stop = false;
bool ispause = false;
AVFormatContext* ctx_format;
AVCodecContext* ctx_codec;
AVCodec* codec;
AVFrame* frame;
AVFrame* rgbframe;
int stream_idx;
SwsContext* ctx_sws;
AVStream *vid_stream;
AVPacket* pkt;
int ret;
int iffw = 1;
int counter = 0;

void Player::playbackInit(string VideoPath)
{
    if(!ffinit)
    {
        frame = av_frame_alloc();
        rgbframe = av_frame_alloc();
        pkt = av_packet_alloc();
        av_register_all();
        avcodec_register_all();
        av_log_set_level(AV_LOG_QUIET);
        ret = avformat_open_input(&ctx_format, VideoPath.c_str(), NULL, NULL);
        if (ret != 0) {
            char error_message[1024];
            av_strerror(ret, error_message, sizeof(error_message));
            Player::playbackThrowError(std::string("Error opening file: ") + error_message);
        }
        if(avformat_find_stream_info(ctx_format, NULL) < 0) Player::playbackThrowError("Error finding stream info.");
        av_dump_format(ctx_format, 0, VideoPath.c_str(), false);
        for(int i = 0; i < ctx_format->nb_streams; i++) if(ctx_format->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            stream_idx = i;
            vid_stream = ctx_format->streams[i];
            break;
        }
        if(!vid_stream) Player::playbackThrowError("Error getting video stream.");
        codec = avcodec_find_decoder(vid_stream->codecpar->codec_id);
        if(!codec) Player::playbackThrowError("Error finding a decoder (strange)");
        ctx_codec = avcodec_alloc_context3(codec);
        if(avcodec_parameters_to_context(ctx_codec, vid_stream->codecpar) < 0) Player::playbackThrowError("Error sending parameters to codec context.");
        if(avcodec_open2(ctx_codec, codec, NULL) < 0) Player::playbackThrowError("Error opening codec with context.");
        ffinit = true;
    }
}
void Player::playbackThrowError(string Error)
{
    Player::playbackExit();
    debugLog("Player was closed due to an error: %s" , Error.c_str());

}
bool Player::playbackLoop()
{
    if(ffinit)
    {
       
            if(stop) return false;
            if(pause)
            {
                 if(GetGamepadButtonPressed() == GAMEPAD_BUTTON_MIDDLE_LEFT) Player::playbackPauseResume();
                 else if(GetGamepadButtonPressed() == GAMEPAD_BUTTON_MIDDLE_RIGHT) Player::playbackStop();
            }
            else if(av_read_frame(ctx_format, pkt) >= 0)
            {
                 if(pkt->stream_index == stream_idx)
                {
                    ret = avcodec_send_packet(ctx_codec, pkt);
                    if(ret < 0 || ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) return false;
                    if(ret >= 0)
                    {
                        ret = avcodec_receive_frame(ctx_codec, frame);
                        if(ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) return false;
                        if(GAMEPAD_BUTTON_RIGHT_FACE_RIGHT == GetGamepadButtonPressed()) Player::playbackPauseResume();
                        else if((GAMEPAD_BUTTON_MIDDLE_RIGHT == GetGamepadButtonPressed()) || (GAMEPAD_BUTTON_MIDDLE_LEFT == GetGamepadButtonPressed())) Player::playbackStop();
                        else if(GAMEPAD_BUTTON_RIGHT_FACE_UP == GetGamepadButtonPressed())
                        {
                            int newffd = 1;
                            if(iffw == 1) newffd = 2;
                            else if(iffw == 2) newffd = 4;
                            else if(iffw == 4) newffd = 8;
                            else if(iffw == 8) newffd = 1;
                            Player::playbackSetFastForward(newffd);
                        }
                        counter++;
                        if(counter >= iffw) counter = 0;
    
                         ctx_sws = sws_getContext(frame->width, frame->height, AV_PIX_FMT_YUV420P, frame->width, frame->height, AV_PIX_FMT_RGB24, SWS_BILINEAR, 0, 0, 0);
                        rgbframe->width = frame->width;
                        rgbframe->height = frame->height;
                        rgbframe->format = AV_PIX_FMT_RGB24;
                        av_image_alloc(rgbframe->data, rgbframe->linesize, rgbframe->width, rgbframe->height, (AVPixelFormat)rgbframe->format, 32);
                        sws_scale(ctx_sws, frame->data, frame->linesize, 0, frame->height, rgbframe->data, rgbframe->linesize);
                        Image img;
                        img.data = (unsigned char*)rgbframe->data[0];
                        img.width = rgbframe->width; img.height = rgbframe->height; img.format = rgbframe->format; img.mipmaps = 1; 
                        Texture2D texture = LoadTextureFromImage(img);
                        DrawTexture(texture, 0, 0, WHITE);
                        av_freep(rgbframe->data);
                    }
                }
            }
              else stop = true;
        }
        if(stop) Player::playbackExit();
        return false;
    
}

void Player::playbackPause()
{
    if(!ispause) ispause = true;
}

void Player::playbackResume()
{
    if(ispause) ispause = false;
}

void Player::playbackPauseResume()
{
    ispause = !ispause;
}

void Player::playbackStop()
{
    if(!stop) stop = true;
}






void Player::playbackExit()
{
    if(ffinit)
    {
        avformat_close_input(&ctx_format);
        av_packet_unref(pkt);
        av_frame_unref(rgbframe);
        av_frame_unref(frame);
        avcodec_free_context(&ctx_codec);
        avformat_free_context(ctx_format);
        if(stop) stop = false;
        if(ispause) ispause = false;
        iffw = 1;
        ffinit = false;
    }
     debugLog("Player was closed.");
}
void Player::playbackSetFastForward(int Power)
{
    iffw = Power;
}
