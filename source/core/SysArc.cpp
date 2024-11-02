#include<SysArc.h>

void* DecompresSceneFile(void* data, unsigned int dataSize, unsigned int* outputSize)
{
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    strm.avail_in = dataSize;
    strm.next_in = (Bytef*)data;

    inflateInit(&strm);

    unsigned char* decompressedData = new unsigned char[dataSize * 2]; // allocate space for decompressed data
    strm.avail_out = dataSize * 2;
    strm.next_out = decompressedData;

    int ret = inflate(&strm, Z_NO_FLUSH);
    if (ret != Z_STREAM_END) {
        // handle error
    }

    inflateEnd(&strm);

    *outputSize = strm.total_out;

    return decompressedData;
}
