#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>

int main(int argc, char *argv[])
{
    const char *filename = "E:/RedMi_E盘_person/code/c/c_ffmpeg/1.mp4"; // 输入视频文件名
    AVFormatContext *fmt_ctx = NULL;

    // 初始化 FFmpeg 库
    avformat_network_init(); // 初始化网络组件（如果需要）

    // 打开输入文件
    if (avformat_open_input(&fmt_ctx, filename, NULL, NULL) < 0)
    {
        fprintf(stderr, "Could not open input file '%s'\n", filename);
        return -1;
    }

    // 打印输入文件的流信息
    av_dump_format(fmt_ctx, 0, filename, 0);

    // 关闭输入文件并释放资源
    avformat_close_input(&fmt_ctx);

    // 等待用户按任意键
    system("pause");

    return 0;
}
