#include "common.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Buffer.hpp"
#include "Player.hpp"

using namespace std;

static const int length_in_seconds = 60;
static const int length_in_buffers = (length_in_seconds * globals::SAMPLE_RATE) / globals::SAMPLES_PER_BUFFER;


namespace wav {

    struct WavHeader
    {
        char chunk_id_0 = 'R';
        char chunk_id_1 = 'I';
        char chunk_id_2 = 'F';
        char chunk_id_3 = 'F';
        uint32_t chunk_size = 0;
        char format_0 = 'W';
        char format_1 = 'A';
        char format_2 = 'V';
        char format_3 = 'E';
        char subchunk1_id_0 = 'f';
        char subchunk1_id_1 = 'm';
        char subchunk1_id_2 = 't';
        char subchunk1_id_3 = 0x20;
        uint32_t subchunk1_size = 16;
        uint16_t audio_format = 1; // 1 = PCM
        uint16_t num_channels = 1;
        uint32_t sample_rate = 22050; 
        uint32_t bytes_per_second = 0;
        uint16_t bytes_per_frame = 0;
        uint16_t bits_per_sample = 0;
        char subchunk2_id_0 = 'd';
        char subchunk2_id_1 = 'a';
        char subchunk2_id_2 = 't';
        char subchunk2_id_3 = 'a';
        uint32_t subchunk2_size = 0;
    };


    static WavHeader wav_header;
    void write_wav(const char* filename, int16_t* data, int num_samples, bool only_8=true)
    {
        const int bytes_per_frame = only_8 ? 1 : 2;
        wav_header.chunk_size = bytes_per_frame * num_samples + 36;
        wav_header.subchunk2_size = bytes_per_frame * num_samples - 8;
        wav_header.bytes_per_second = bytes_per_frame * wav_header.sample_rate;
        wav_header.bytes_per_frame = bytes_per_frame;
        wav_header.bits_per_sample = bytes_per_frame * 8;
        FILE* fid = fopen(filename, "wb");
        fwrite(&wav_header, 1, sizeof(WavHeader), fid);
        if (only_8) {
            int8_t* wdata = (int8_t*)malloc(num_samples);
            for (int i=0; i<num_samples; i++) {
                int16_t val = data[i];
                val += 128;
                if (val<0) val = 0;
                if (val>255) val = 255;
                wdata[i] = val;
            }
            fwrite(wdata, 1, num_samples, fid);
            free(wdata);
        } else {
            fwrite(data, 2, num_samples, fid);
        }
        fclose(fid);
    }
}


int main(int argc, char* argv[])
{
    
    cout << "Rendering" << endl;
    

    std::array<Buffer, length_in_buffers> buffs;

    static Player player;
    for (auto& buff : buffs) player.render(buff);

    wav::write_wav("drop.wav", buffs[0].data(), length_in_buffers*globals::SAMPLES_PER_BUFFER, true);

    return 0;
}
