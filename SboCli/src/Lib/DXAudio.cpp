/// @file DXAudio.cpp
/// @brief SDL_audioベースのオーディオ実装（NO_DIRECTMUSICビルド用）
#include "stdafx.h"
#if defined(__EMSCRIPTEN__)
#include "DXAudio.h"

CDXAudio::CDXAudio() {
  m_pPerformance = nullptr;
  m_pDefAudioPath = nullptr;
  m_pDefAudioPath2 = nullptr;
  m_pLoader = nullptr;
  m_hResource = NULL;
  m_pBgmFmt = nullptr;
  m_pBgmAudio = nullptr;
  m_cbBgmAudio = 0;
  m_pBgmVoice = nullptr;
  m_fBgmVolume = 1.0f;
}

CDXAudio::~CDXAudio() {
}

BOOL CDXAudio::Create(void) { return TRUE; }
void CDXAudio::Destroy(void) {}
void CDXAudio::SetResourceHandle(void* hResource) { m_hResource = hResource; }
BOOL CDXAudio::GetSegFromRes(HRSRC hSrc, IDirectMusicSegment8 **pSeg, BOOL bMidi) { return FALSE; }
void CDXAudio::ReleaseSeg(IDirectMusicSegment8 *pSeg) {}
BOOL CDXAudio::PlayPrimary(IDirectMusicSegment8 *pSeg) { return FALSE; }
BOOL CDXAudio::PlaySecoundary(IDirectMusicSegment8 *pSeg) { return FALSE; }
void CDXAudio::SetVolPrimary(long lVol) {}
void CDXAudio::SetVolSecoundary(long lVol) {}
void CDXAudio::Stop(IDirectMusicSegment8 *pSeg, DWORD dwFlg) {}
void CDXAudio::SetLoopPoints(IDirectMusicSegment8 *pSeg, DWORD dwFlg) {}
BOOL CDXAudio::IsPlaying(IDirectMusicSegment8 *pSeg) { return FALSE; }
BOOL CDXAudio::PlayBGMFile(const char* path, BOOL bLoop, float volume) { return FALSE; }
void CDXAudio::StopBGM() {}
void CDXAudio::SetBGMVolume(float volume) { m_fBgmVolume = volume; }
void CDXAudio::FreeBgmResources() {}

#else
// -----------------------------------------------------------------------
// SDL_audio ベース実装（Windows ネイティブ向け）
// -----------------------------------------------------------------------
#include <windows.h>   // LoadResource / LockResource 等
#include <mmreg.h>     // WAVEFORMATEX
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include "third_party/stb_vorbis.c"
#include "DXAudio.h"

// -----------------------------------------------------------------------
// 内部構造体
// -----------------------------------------------------------------------

/// @brief 出力フォーマットに変換済みの PCM データを保持する
struct WAVSEG {
    Uint8*  audio;       ///< 変換済み PCM データ
    Uint32  audioBytes;  ///< データサイズ（バイト）
};

// -----------------------------------------------------------------------
// グローバルミキサー状態
// -----------------------------------------------------------------------

#define MAX_SE_CHANNELS   32
#define BGM_CHANNEL_INDEX MAX_SE_CHANNELS   ///< BGM 専用チャンネルのインデックス

/// @brief 1チャンネル分の再生状態
struct AudioChannel {
    const Uint8* data;    ///< PCM データへのポインタ
    Uint32       size;    ///< データサイズ
    Uint32       pos;     ///< 現在の再生位置
    float        volume;  ///< 音量（0.0 〜 1.0）
    SDL_bool     loop;    ///< ループ再生フラグ
    SDL_bool     active;  ///< 再生中フラグ
};

static AudioChannel     g_channels[MAX_SE_CHANNELS + 1];  ///< [0..MAX_SE_CHANNELS-1]=SE, [MAX_SE_CHANNELS]=BGM
static SDL_AudioDeviceID g_audioDevice = 0;
static SDL_AudioSpec    g_obtainedSpec;
static float            g_sfx_volume = 1.0f;   ///< SE 全体の音量（リニア）

// -----------------------------------------------------------------------
// 音声コールバック
// -----------------------------------------------------------------------

/// @brief SDL 音声コールバック。全チャンネルをミックスして stream に書き出す。
static void SDLCALL AudioCallback(void* userdata, Uint8* stream, int len)
{
    (void)userdata;
    SDL_memset(stream, 0, len);  // まずサイレンスにする

    for (int i = 0; i < MAX_SE_CHANNELS + 1; i++) {
        AudioChannel* ch = &g_channels[i];
        if (!ch->active) continue;

        Uint32 remaining = ch->size - ch->pos;
        Uint32 toMix = (Uint32)len;
        if (toMix > remaining) toMix = remaining;

        int vol = (int)(ch->volume * SDL_MIX_MAXVOLUME);
        if (vol > SDL_MIX_MAXVOLUME) vol = SDL_MIX_MAXVOLUME;
        if (vol < 0)                 vol = 0;

        SDL_MixAudioFormat(stream, ch->data + ch->pos,
                           g_obtainedSpec.format, toMix, vol);
        ch->pos += toMix;

        if (ch->pos >= ch->size) {
            if (ch->loop) {
                ch->pos = 0;  // ループ先頭に戻す
            } else {
                ch->active = SDL_FALSE;
            }
        }
    }
}

// -----------------------------------------------------------------------
// WAV パーサー
// -----------------------------------------------------------------------

/// @brief メモリ上の WAV データを解析し、出力フォーマットに変換して WAVSEG を生成する。
/// @param data   WAV バイナリ
/// @param size   データサイズ
/// @param[out] outSeg  成功時に生成した WAVSEG を返す（失敗時は nullptr）
/// @return 成功なら true
static bool ParseWavFromMemory(const BYTE* data, DWORD size, WAVSEG** outSeg)
{
    if (!outSeg) return false;
    *outSeg = nullptr;

    if (size < 12) return false;

    // リトルエンディアン 4 バイト読み出しラムダ
    auto rd32 = [&](DWORD off) -> DWORD {
        if (off + 4 > size) return 0;
        return *(const DWORD*)(data + off);
    };

    // 'RIFF' / 'WAVE' チェック（リトルエンディアン比較）
    if (rd32(0) != 0x46464952u || rd32(8) != 0x45564157u) return false;  // "RIFF" "WAVE"

    DWORD pos = 12;
    const BYTE*  rawAudio = nullptr;
    DWORD        rawBytes = 0;
    WAVEFORMATEX wfx{};
    bool         hasFmt = false;

    while (pos + 8 <= size) {
        DWORD id = rd32(pos);
        DWORD cb = rd32(pos + 4);
        pos += 8;
        if (pos + cb > size) break;

        if (id == 0x20746D66u) {  // "fmt "
            DWORD copyBytes = (cb < sizeof(WAVEFORMATEX)) ? cb : sizeof(WAVEFORMATEX);
            memset(&wfx, 0, sizeof(wfx));
            memcpy(&wfx, data + pos, copyBytes);
            hasFmt = true;
        } else if (id == 0x61746164u) {  // "data"
            rawAudio = data + pos;
            rawBytes = cb;
        }
        pos += ((cb + 1) & ~1u);  // ワード境界アライン
    }

    if (!hasFmt || !rawAudio || rawBytes == 0) return false;

    // SDL_AudioCVT で出力フォーマットへ変換
    SDL_AudioCVT cvt;
    SDL_AudioFormat srcFmt = (wfx.wBitsPerSample == 8) ? AUDIO_U8 : AUDIO_S16LSB;
    int ret = SDL_BuildAudioCVT(&cvt,
        srcFmt, (Uint8)wfx.nChannels, (int)wfx.nSamplesPerSec,
        g_obtainedSpec.format, g_obtainedSpec.channels, g_obtainedSpec.freq);
    if (ret < 0) return false;  // 変換器の構築失敗

    // 変換バッファを確保
    Uint32 convertedSize = (cvt.len_mult > 0)
        ? (Uint32)(rawBytes * cvt.len_mult)
        : rawBytes;
    Uint8* buf = (Uint8*)malloc(convertedSize);
    if (!buf) return false;

    memcpy(buf, rawAudio, rawBytes);
    cvt.buf = buf;
    cvt.len = (int)rawBytes;

    if (ret > 0) {
        // 変換が必要な場合のみ SDL_ConvertAudio を呼ぶ
        if (SDL_ConvertAudio(&cvt) != 0) {
            free(buf);
            return false;
        }
    }

    WAVSEG* seg = (WAVSEG*)malloc(sizeof(WAVSEG));
    if (!seg) { free(buf); return false; }
    seg->audio      = buf;
    seg->audioBytes = (ret > 0) ? (Uint32)cvt.len_cvt : rawBytes;
    *outSeg = seg;
    return true;
}

/// @brief dB 値（整数）をリニア音量に変換する
static float DbToLinear(long dB)
{
    if (dB >   10) dB =   10;
    if (dB < -100) dB = -100;
    return powf(10.0f, (float)dB / 20.0f);
}

// -----------------------------------------------------------------------
// CDXAudio 実装
// -----------------------------------------------------------------------

CDXAudio::CDXAudio()
    : m_pPerformance(nullptr)
    , m_pDefAudioPath(nullptr)
    , m_pDefAudioPath2(nullptr)
    , m_pLoader(nullptr)
    , m_hResource(NULL)
    , m_pBgmFmt(nullptr)
    , m_pBgmAudio(nullptr)
    , m_cbBgmAudio(0)
    , m_pBgmVoice(nullptr)
    , m_fBgmVolume(1.0f)
{
}

CDXAudio::~CDXAudio()
{
    Destroy();
}

BOOL CDXAudio::Create(void)
{
    if (g_audioDevice != 0) return TRUE;  // 既に初期化済み

    SDL_AudioSpec desired;
    SDL_memset(&desired, 0, sizeof(desired));
    desired.freq     = 44100;
    desired.format   = AUDIO_S16LSB;
    desired.channels = 2;
    desired.samples  = 2048;
    desired.callback = AudioCallback;
    desired.userdata = nullptr;

    g_audioDevice = SDL_OpenAudioDevice(
        NULL, 0, &desired, &g_obtainedSpec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
    if (g_audioDevice == 0) return FALSE;

    SDL_memset(g_channels, 0, sizeof(g_channels));
    SDL_PauseAudioDevice(g_audioDevice, 0);  // 再生開始
    return TRUE;
}

void CDXAudio::Destroy(void)
{
    // グローバルエンジンはアプリ全体で共有するため、ここでは閉じない
}

void CDXAudio::SetResourceHandle(void* hResource)
{
    m_hResource = hResource;
}

BOOL CDXAudio::GetSegFromRes(HRSRC hSrc, IDirectMusicSegment8 **pSeg, BOOL /*bMidi*/)
{
    if (!pSeg || !m_hResource || g_audioDevice == 0) return FALSE;

    // m_hResource は void* だが Windows 版でしか到達しないため HMODULE にキャストして使用
    HGLOBAL hRes = LoadResource((HMODULE)m_hResource, hSrc);
    if (!hRes) return FALSE;
    DWORD sz = SizeofResource((HMODULE)m_hResource, hSrc);
    const BYTE* mem = (const BYTE*)LockResource(hRes);
    if (!mem || sz == 0) return FALSE;

    WAVSEG* seg = nullptr;
    if (!ParseWavFromMemory(mem, sz, &seg)) return FALSE;

    *pSeg = (IDirectMusicSegment8*)seg;
    return TRUE;
}

void CDXAudio::ReleaseSeg(IDirectMusicSegment8 *pSeg)
{
    if (!pSeg) return;
    WAVSEG* seg = (WAVSEG*)pSeg;
    if (seg->audio) { free(seg->audio); seg->audio = nullptr; }
    free(seg);
}

BOOL CDXAudio::PlayPrimary(IDirectMusicSegment8 *pSeg)
{
    if (!pSeg || g_audioDevice == 0) return FALSE;
    WAVSEG* seg = (WAVSEG*)pSeg;

    SDL_LockAudioDevice(g_audioDevice);
    // 空きチャンネルを探す
    for (int i = 0; i < MAX_SE_CHANNELS; i++) {
        if (!g_channels[i].active) {
            g_channels[i].data   = seg->audio;
            g_channels[i].size   = seg->audioBytes;
            g_channels[i].pos    = 0;
            g_channels[i].volume = g_sfx_volume;
            g_channels[i].loop   = SDL_FALSE;
            g_channels[i].active = SDL_TRUE;
            SDL_UnlockAudioDevice(g_audioDevice);
            return TRUE;
        }
    }
    SDL_UnlockAudioDevice(g_audioDevice);
    return FALSE;  // 空きチャンネルなし
}

BOOL CDXAudio::PlaySecoundary(IDirectMusicSegment8 *pSeg)
{
    return PlayPrimary(pSeg);
}

void CDXAudio::SetVolPrimary(long lVol)
{
    // プライマリ（マスター相当）：全チャンネルの基準音量に反映
    // SDL にはマスターボリュームの直接 API がないため g_sfx_volume で代用
    g_sfx_volume = DbToLinear(lVol);
}

void CDXAudio::SetVolSecoundary(long lVol)
{
    g_sfx_volume = DbToLinear(lVol);
}

void CDXAudio::Stop(IDirectMusicSegment8 *pSeg, DWORD /*dwFlg*/)
{
    if (!pSeg || g_audioDevice == 0) return;
    WAVSEG* seg = (WAVSEG*)pSeg;

    SDL_LockAudioDevice(g_audioDevice);
    for (int i = 0; i < MAX_SE_CHANNELS; i++) {
        if (g_channels[i].active && g_channels[i].data == seg->audio) {
            g_channels[i].active = SDL_FALSE;
        }
    }
    SDL_UnlockAudioDevice(g_audioDevice);
}

void CDXAudio::SetLoopPoints(IDirectMusicSegment8 *pSeg, DWORD dwRepeats)
{
    // SDL ミキサーでのループは AudioChannel::loop フラグで制御する。
    // GetSegFromRes 時点ではループ設定を保持しておらず、
    // 実際に Play する際に設定する必要があるが、現状の呼び出し側は
    // Play の前に SetLoopPoints を呼ぶ設計ではないため、ここでは無視する。
    (void)pSeg;
    (void)dwRepeats;
}

BOOL CDXAudio::IsPlaying(IDirectMusicSegment8 *pSeg)
{
    if (!pSeg || g_audioDevice == 0) return FALSE;
    WAVSEG* seg = (WAVSEG*)pSeg;

    SDL_LockAudioDevice(g_audioDevice);
    for (int i = 0; i < MAX_SE_CHANNELS; i++) {
        if (g_channels[i].active && g_channels[i].data == seg->audio) {
            SDL_UnlockAudioDevice(g_audioDevice);
            return TRUE;
        }
    }
    SDL_UnlockAudioDevice(g_audioDevice);
    return FALSE;
}

// -----------------------------------------------------------------------
// BGM 関連
// -----------------------------------------------------------------------

void CDXAudio::FreeBgmResources()
{
    if (g_audioDevice != 0) {
        SDL_LockAudioDevice(g_audioDevice);
        g_channels[BGM_CHANNEL_INDEX].active = SDL_FALSE;
        g_channels[BGM_CHANNEL_INDEX].data   = nullptr;
        SDL_UnlockAudioDevice(g_audioDevice);
    }
    if (m_pBgmAudio) {
        free(m_pBgmAudio);
        m_pBgmAudio = nullptr;
    }
    m_pBgmFmt    = nullptr;  // WAVEFORMATEX* は持たない（SDL_audio では不要）
    m_pBgmVoice  = nullptr;  // 使用しない
    m_cbBgmAudio = 0;
}

BOOL CDXAudio::PlayBGMFile(const char* path, BOOL bLoop, float volume)
{
    if (!path || g_audioDevice == 0) return FALSE;

    // ファイル全体を読み込む
    FILE* fp = fopen(path, "rb");
    if (!fp) return FALSE;
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (fsize <= 0) { fclose(fp); return FALSE; }

    BYTE* mem = (BYTE*)malloc((size_t)fsize);
    if (!mem) { fclose(fp); return FALSE; }
    if (fread(mem, 1, (size_t)fsize, fp) != (size_t)fsize) {
        free(mem); fclose(fp); return FALSE;
    }
    fclose(fp);

    Uint8* pcmData  = nullptr;
    Uint32 pcmBytes = 0;

    // WAV を試みる
    WAVSEG* seg = nullptr;
    if (ParseWavFromMemory(mem, (DWORD)fsize, &seg)) {
        free(mem);
        pcmData  = seg->audio;
        pcmBytes = seg->audioBytes;
        free(seg);  // WAVSEG 構造体のみ解放（pcmData は保持）
    } else {
        free(mem);
        // OGG を試みる（stb_vorbis 使用）
        int ch = 0, rate = 0;
        short* pcm = nullptr;
        int frames = stb_vorbis_decode_filename(path, &ch, &rate, &pcm);
        if (frames <= 0 || !pcm || ch <= 0 || rate <= 0) {
            if (pcm) free(pcm);
            return FALSE;
        }

        // stb_vorbis の出力（16bit PCM）を SDL_AudioCVT で変換
        Uint32 rawBytes = (Uint32)frames * (Uint32)ch * sizeof(short);
        SDL_AudioCVT cvt;
        int ret = SDL_BuildAudioCVT(&cvt,
            AUDIO_S16LSB, (Uint8)ch, rate,
            g_obtainedSpec.format, g_obtainedSpec.channels, g_obtainedSpec.freq);
        if (ret < 0) { free(pcm); return FALSE; }

        Uint32 bufSize = (cvt.len_mult > 0)
            ? (Uint32)(rawBytes * cvt.len_mult)
            : rawBytes;
        Uint8* buf = (Uint8*)malloc(bufSize);
        if (!buf) { free(pcm); return FALSE; }
        memcpy(buf, pcm, rawBytes);
        free(pcm);

        cvt.buf = buf;
        cvt.len = (int)rawBytes;
        if (ret > 0 && SDL_ConvertAudio(&cvt) != 0) {
            free(buf); return FALSE;
        }
        pcmData  = buf;
        pcmBytes = (ret > 0) ? (Uint32)cvt.len_cvt : rawBytes;
    }

    if (!pcmData || pcmBytes == 0) { if (pcmData) free(pcmData); return FALSE; }

    // 既存 BGM を停止してリソースを解放
    FreeBgmResources();

    m_pBgmAudio  = pcmData;
    m_cbBgmAudio = (unsigned long)pcmBytes;
    m_fBgmVolume = volume;

    SDL_LockAudioDevice(g_audioDevice);
    g_channels[BGM_CHANNEL_INDEX].data   = pcmData;
    g_channels[BGM_CHANNEL_INDEX].size   = pcmBytes;
    g_channels[BGM_CHANNEL_INDEX].pos    = 0;
    g_channels[BGM_CHANNEL_INDEX].volume = volume;
    g_channels[BGM_CHANNEL_INDEX].loop   = bLoop ? SDL_TRUE : SDL_FALSE;
    g_channels[BGM_CHANNEL_INDEX].active = SDL_TRUE;
    SDL_UnlockAudioDevice(g_audioDevice);

    return TRUE;
}

void CDXAudio::StopBGM()
{
    FreeBgmResources();
}

void CDXAudio::SetBGMVolume(float volume)
{
    m_fBgmVolume = volume;
    if (g_audioDevice != 0) {
        SDL_LockAudioDevice(g_audioDevice);
        g_channels[BGM_CHANNEL_INDEX].volume = volume;
        SDL_UnlockAudioDevice(g_audioDevice);
    }
}

#endif
