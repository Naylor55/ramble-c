#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/parseutils.h>

void print_sei_data(const uint8_t *data, int size)
{
    // SEI 数据的打印逻辑
    printf("SEI Data (size %d):\n", size);
    for (int i = 0; i < size; i++)
    {
        printf("%02x ", data[i]);
        if ((i + 1) % 16 == 0)
        {
            printf("\n");
        }
    }
    printf("\n");
}

int main()
{
    const char *url = "rtmp://192.168.1.115:1935/live/home";
    AVFormatContext *fmt_ctx = NULL;
    AVCodecContext *codec_ctx = NULL;
    AVPacket packet;

    av_register_all();
    avformat_network_init();

    // 打开流
    if (avformat_open_input(&fmt_ctx, url, NULL, NULL) < 0)
    {
        fprintf(stderr, "Could not open input URL '%s'\n", url);
        return -1;
    }

    // 查找流信息
    if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
    {
        fprintf(stderr, "Could not find stream information\n");
        return -1;
    }

    // 查找视频流
    int video_stream_index = -1;
    for (int i = 0; i < fmt_ctx->nb_streams; i++)
    {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
            break;
        }
    }

    if (video_stream_index == -1)
    {
        fprintf(stderr, "Could not find video stream\n");
        return -1;
    }

    // 找到视频流的解码器
    AVCodecParameters *codecpar = fmt_ctx->streams[video_stream_index]->codecpar;
    AVCodec *codec = avcodec_find_decoder(codecpar->codec_id);
    if (!codec)
    {
        fprintf(stderr, "Codec not found\n");
        return -1;
    }

    // 初始化解码器
    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx)
    {
        fprintf(stderr, "Could not allocate video codec context\n");
        return -1;
    }

    if (avcodec_parameters_to_context(codec_ctx, codecpar) < 0)
    {
        fprintf(stderr, "Could not copy codec parameters to context\n");
        return -1;
    }

    if (avcodec_open2(codec_ctx, codec, NULL) < 0)
    {
        fprintf(stderr, "Could not open codec\n");
        return -1;
    }

    // 读取并处理包
    av_init_packet(&packet);
    while (av_read_frame(fmt_ctx, &packet) >= 0)
    {
        if (packet.stream_index == video_stream_index)
        {
            // 检查包中的 SEI 数据
            if (packet.size > 0)
            {
                // 假设 SEI 数据在 packet.data 中
                // 你可以解析 NAL 单元头来确定是否是 SEI 数据
                // 这里简单地打印所有数据
                print_sei_data(packet.data, packet.size);
            }
        }
        av_packet_unref(&packet);
    }

    // 清理
    avcodec_free_context(&codec_ctx);
    avformat_close_input(&fmt_ctx);
    avformat_network_deinit();

    return 0;
}
