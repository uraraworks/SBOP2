// Placeholder for stb_vorbis implementation
// To enable OGG decoding, replace this file with the official stb_vorbis.c
// from https://github.com/nothings/stb (stb_vorbis, public domain / MIT).
// This stub compiles but returns failure so .ogg won't play until replaced.

#ifdef __cplusplus
extern "C" {
#endif

int stb_vorbis_decode_filename(const char* filename, int* channels, int* sample_rate, short** output)
{
    (void)filename; (void)channels; (void)sample_rate; (void)output;
    return 0; // failure
}

#ifdef __cplusplus
}
#endif

