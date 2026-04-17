/// @file DXAudio.h
/// @brief SDL_audio ベースのオーディオ出力（効果音 + BGM） 定義ファイル
#pragma once

#include <map>

typedef class CDXAudio
{
public:
            CDXAudio();
    virtual ~CDXAudio();

    BOOL Create            (void);
    void Destroy           (void);
    void SetResourceHandle (void* hResource);

    // 効果音セグメント（内部実装は WAVSEG*。呼び出し側は不透明ポインタとして扱う）
    BOOL GetSegFromRes     (HRSRC hSrc, void **pSeg, BOOL bMidi = FALSE);
    void ReleaseSeg        (void *pSeg);
    BOOL PlayPrimary       (void *pSeg);
    BOOL PlaySecoundary    (void *pSeg);
    void SetVolPrimary     (long lVol);
    void SetVolSecoundary  (long lVol);
    void Stop              (void *pSeg, DWORD dwFlg);
    void SetLoopPoints     (void *pSeg, DWORD dwFlg);
    BOOL IsPlaying         (void *pSeg);

    // WAV ファイルから効果音セグメントを生成（Web版で使用）
    BOOL LoadWavFromFile   (const char* path, void** pSeg);

    // BGM (OGG/WAV ファイルを SDL_audio で再生)
    BOOL PlayBGMFile       (const char* path, BOOL bLoop, float volume);
    void StopBGM           ();
    void SetBGMVolume      (float volume);

    // BGM プリデコードキャッシュ（Emscripten版でのマップ切替無音解消用）
    // 指定パスのBGMを事前にデコードしてキャッシュに保存（keyで参照）
    BOOL PreloadBGM        (int key, const char* path);
    // プリロード済みBGMを再生（未プリロードなら FALSE を返す）
    BOOL PlayBGMCached     (int key, BOOL bLoop, float volume);

private:
    void*                     m_hResource;  // Windows版では (HMODULE) キャストして使用

    // BGM 状態
    unsigned char*            m_pBgmAudio;  // PCM data
    unsigned long             m_cbBgmAudio; // size in bytes
    float                     m_fBgmVolume; // 0.0 - 1.0

    void FreeBgmResources();

    // BGM プリデコードキャッシュ（key=BGMID, value={pcmData, pcmBytes}）
    struct BgmCacheEntry {
        unsigned char* pcmData;
        unsigned long  pcmBytes;
    };
    std::map<int, BgmCacheEntry> m_bgmCache;

    void FreeBgmCache();
} CDXAudio, *PCDXAudio;
