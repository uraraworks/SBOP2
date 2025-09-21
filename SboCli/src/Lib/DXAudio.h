// DXAudio.h - unified header with XAudio2 BGM support
#pragma once

#ifndef NO_DIRECTMUSIC
#include <dmusici.h>
#else
struct IDirectMusicSegment8;
struct IDirectMusicPerformance8;
struct IDirectMusicAudioPath8;
struct IDirectMusicLoader8;
#endif

typedef class CDXAudio
{
public:
            CDXAudio();
    virtual ~CDXAudio();

    BOOL Create            (void);
    void Destroy           (void);
    void SetResourceHandle (HMODULE hResource);

    BOOL GetSegFromRes     (HRSRC hSrc, IDirectMusicSegment8 **pSeg, BOOL bMidi = FALSE);
    void ReleaseSeg        (IDirectMusicSegment8 *pSeg);
    BOOL PlayPrimary       (IDirectMusicSegment8 *pSeg);
    BOOL PlaySecoundary    (IDirectMusicSegment8 *pSeg);
    void SetVolPrimary     (long lVol);
    void SetVolSecoundary  (long lVol);
    void Stop              (IDirectMusicSegment8 *pSeg, DWORD dwFlg);
    void SetLoopPoints     (IDirectMusicSegment8 *pSeg, DWORD dwFlg);
    BOOL IsPlaying         (IDirectMusicSegment8 *pSeg);

    // BGM (file from disk, decoded via Audiere, played by XAudio2)
    BOOL PlayBGMFile       (const char* path, BOOL bLoop, float volume);
    void StopBGM           ();
    void SetBGMVolume      (float volume);

private:
    IDirectMusicPerformance8 *m_pPerformance;
    IDirectMusicAudioPath8   *m_pDefAudioPath;
    IDirectMusicAudioPath8   *m_pDefAudioPath2;
    IDirectMusicLoader8      *m_pLoader;
    HMODULE                   m_hResource;

    // XAudio2-backed BGM state
    void*                     m_pBgmFmt;    // WAVEFORMATEX*
    unsigned char*            m_pBgmAudio;  // PCM data
    unsigned long             m_cbBgmAudio; // size in bytes
    void*                     m_pBgmVoice;  // IXAudio2SourceVoice*
    float                     m_fBgmVolume; // 0.0 - 1.0

    void FreeBgmResources();
} CDXAudio, *PCDXAudio;
