// XAudio2-backed minimal audio for NO_DIRECTMUSIC builds
#include "stdafx.h"
#include <windows.h>
#include <mmreg.h>
#include <xaudio2.h>
#include <cmath>
#include <cstdlib>
#include "third_party/stb_vorbis.c" // local bundled decoder (placeholder)
#include "DXAudio.h"

struct WAVSEG {
  IXAudio2SourceVoice* voice{nullptr};
  WAVEFORMATEX* format{nullptr};
  BYTE* audio{nullptr};
  DWORD audioBytes{0};
  XAUDIO2_BUFFER buffer{};
};

static IXAudio2* g_xaudio2 = nullptr;
static IXAudio2MasteringVoice* g_master = nullptr;
static float g_sfx_volume = 1.0f; // linear volume for secondary (SE)

static bool ParseWavFromMemory(const BYTE* data, DWORD size,
                               WAVEFORMATEX** outFmt,
                               BYTE** outAudio, DWORD* outBytes) {
  if (size < 12) return false;
  auto rd32 = [&](DWORD off)->DWORD { if (off+4>size) return 0; return *(DWORD const*)(data+off); };
  if (rd32(0) != 'FFIR' || rd32(8) != 'EVAW') return false; // 'RIFF' 'WAVE'
  DWORD pos = 12;
  const BYTE* audio = nullptr; DWORD audioBytes = 0; WAVEFORMATEX* fmt = nullptr;
  while (pos + 8 <= size) {
    DWORD id = rd32(pos);
    DWORD cb = rd32(pos+4);
    pos += 8;
    if (pos + cb > size) break;
    if (id == ' tmf') { // 'fmt '
      if (cb < sizeof(WAVEFORMATEX)) {
        // allocate and copy
        WAVEFORMATEX tmp{};
        memcpy(&tmp, data+pos, min<DWORD>(cb, sizeof(tmp)));
        fmt = (WAVEFORMATEX*)CoTaskMemAlloc(sizeof(WAVEFORMATEX));
        if (!fmt) return false;
        *fmt = tmp;
      } else {
        fmt = (WAVEFORMATEX*)CoTaskMemAlloc(sizeof(WAVEFORMATEX));
        if (!fmt) return false;
        memcpy(fmt, data+pos, sizeof(WAVEFORMATEX));
      }
    } else if (id == 'atad') { // 'data'
      audioBytes = cb;
      audio = data+pos;
    }
    pos += ((cb + 1) & ~1u); // align
  }
  if (!fmt || !audio || audioBytes == 0) { if (fmt) CoTaskMemFree(fmt); return false; }
  // make owned copy of audio
  BYTE* buf = (BYTE*)CoTaskMemAlloc(audioBytes);
  if (!buf) { CoTaskMemFree(fmt); return false; }
  memcpy(buf, audio, audioBytes);
  *outFmt = fmt; *outAudio = buf; *outBytes = audioBytes;
  return true;
}

static float DbToLinear(long dB) {
  if (dB > 10) dB = 10; // safety
  if (dB < -100) dB = -100;
  return powf(10.0f, (float)dB / 20.0f);
}

CDXAudio::CDXAudio() {
  m_hResource = NULL;
  m_pBgmFmt = nullptr;
  m_pBgmAudio = nullptr;
  m_cbBgmAudio = 0;
  m_pBgmVoice = nullptr;
  m_fBgmVolume = 1.0f;
}
CDXAudio::~CDXAudio() { Destroy(); }

BOOL CDXAudio::Create(void) {
  if (!g_xaudio2) {
    if (FAILED(XAudio2Create(&g_xaudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) return FALSE;
    if (FAILED(g_xaudio2->CreateMasteringVoice(&g_master))) return FALSE;
  }
  return TRUE;
}

void CDXAudio::Destroy(void) {
  // keep global engine alive for app lifetime; nothing to do here
}

void CDXAudio::SetResourceHandle(HMODULE hResource) { m_hResource = hResource; }

BOOL CDXAudio::GetSegFromRes(HRSRC hSrc, IDirectMusicSegment8 **pSeg, BOOL /*bMidi*/) {
  if (!g_xaudio2 || !pSeg || !m_hResource) return FALSE;
  HGLOBAL hRes = LoadResource(m_hResource, hSrc);
  if (!hRes) return FALSE;
  DWORD sz = SizeofResource(m_hResource, hSrc);
  const BYTE* mem = (const BYTE*)LockResource(hRes);
  if (!mem || !sz) return FALSE;
  WAVEFORMATEX* fmt=nullptr; BYTE* audio=nullptr; DWORD bytes=0;
  if (!ParseWavFromMemory(mem, sz, &fmt, &audio, &bytes)) return FALSE;
  IXAudio2SourceVoice* v=nullptr;
  if (FAILED(g_xaudio2->CreateSourceVoice(&v, fmt))) { CoTaskMemFree(fmt); CoTaskMemFree(audio); return FALSE; }
  WAVSEG* seg = new WAVSEG();
  seg->voice = v; seg->format = fmt; seg->audio = audio; seg->audioBytes = bytes;
  ZeroMemory(&seg->buffer, sizeof(seg->buffer));
  seg->buffer.AudioBytes = bytes;
  seg->buffer.pAudioData = audio;
  seg->buffer.Flags = XAUDIO2_END_OF_STREAM;
  *pSeg = (IDirectMusicSegment8*)seg;
  return TRUE;
}

void CDXAudio::ReleaseSeg(IDirectMusicSegment8 *pSeg) {
  if (!pSeg) return;
  WAVSEG* seg = (WAVSEG*)pSeg;
  if (seg->voice) { seg->voice->DestroyVoice(); seg->voice = nullptr; }
  if (seg->format) { CoTaskMemFree(seg->format); seg->format = nullptr; }
  if (seg->audio) { CoTaskMemFree(seg->audio); seg->audio = nullptr; }
  delete seg;
}

BOOL CDXAudio::PlayPrimary(IDirectMusicSegment8 *pSeg) {
  if (!pSeg) return FALSE;
  WAVSEG* seg = (WAVSEG*)pSeg;
  seg->voice->Stop();
  seg->voice->FlushSourceBuffers();
  if (FAILED(seg->voice->SubmitSourceBuffer(&seg->buffer))) return FALSE;
  seg->voice->SetVolume(g_sfx_volume);
  return SUCCEEDED(seg->voice->Start());
}

BOOL CDXAudio::PlaySecoundary(IDirectMusicSegment8 *pSeg) {
  return PlayPrimary(pSeg);
}

void CDXAudio::SetVolPrimary(long lVol) {
  if (!g_master) return;
  float vol = DbToLinear(lVol);
  g_master->SetVolume(vol);
}

void CDXAudio::SetVolSecoundary(long lVol) {
  // This would ideally adjust per-voice volume; left to caller to manage per segment
  g_sfx_volume = DbToLinear(lVol);
}

void CDXAudio::Stop(IDirectMusicSegment8 *pSeg, DWORD /*dwFlg*/) {
  if (!pSeg) return;
  WAVSEG* seg = (WAVSEG*)pSeg;
  seg->voice->Stop();
  seg->voice->FlushSourceBuffers();
}

void CDXAudio::SetLoopPoints(IDirectMusicSegment8 *pSeg, DWORD dwRepeats) {
  if (!pSeg) return;
  WAVSEG* seg = (WAVSEG*)pSeg;
  // Re-submit buffer with loop settings
  seg->buffer.LoopCount = (dwRepeats == 0 ? XAUDIO2_LOOP_INFINITE : (dwRepeats > 1 ? dwRepeats-1 : 0));
}

BOOL CDXAudio::IsPlaying(IDirectMusicSegment8 *pSeg) {
  if (!pSeg) return FALSE;
  WAVSEG* seg = (WAVSEG*)pSeg;
  XAUDIO2_VOICE_STATE st{};
  seg->voice->GetState(&st);
  return st.BuffersQueued > 0;
}

void CDXAudio::FreeBgmResources() {
  if (m_pBgmVoice) {
    ((IXAudio2SourceVoice*)m_pBgmVoice)->Stop();
    ((IXAudio2SourceVoice*)m_pBgmVoice)->FlushSourceBuffers();
    ((IXAudio2SourceVoice*)m_pBgmVoice)->DestroyVoice();
    m_pBgmVoice = nullptr;
  }
  if (m_pBgmFmt) {
    CoTaskMemFree(m_pBgmFmt);
    m_pBgmFmt = nullptr;
  }
  if (m_pBgmAudio) {
    CoTaskMemFree(m_pBgmAudio);
    m_pBgmAudio = nullptr;
  }
  m_cbBgmAudio = 0;
}

BOOL CDXAudio::PlayBGMFile(const char* path, BOOL bLoop, float volume) {
  if (!g_xaudio2 || !path) return FALSE;
  // Load whole file
  HANDLE hf = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (hf == INVALID_HANDLE_VALUE) return FALSE;
  DWORD sz = GetFileSize(hf, nullptr);
  if (sz == INVALID_FILE_SIZE || sz == 0) { CloseHandle(hf); return FALSE; }
  BYTE* mem = (BYTE*)malloc(sz);
  if (!mem) { CloseHandle(hf); return FALSE; }
  DWORD rd=0; BOOL ok = ReadFile(hf, mem, sz, &rd, nullptr);
  CloseHandle(hf);
  if (!ok || rd != sz) { free(mem); return FALSE; }

  // Try WAV first
  WAVEFORMATEX* fmt = nullptr; BYTE* audio = nullptr; DWORD bytes = 0;
  BOOL parsed = ParseWavFromMemory(mem, sz, &fmt, &audio, &bytes);
  if (!parsed) {
    // Not WAV. Try OGG via stb_vorbis
    int ch = 0, rate = 0;
    short* pcm = nullptr;
    int frames = 0;
    // include local decoder wrapper
    // Note: This expects third_party/stb_vorbis.c to provide implementation.
    frames = stb_vorbis_decode_filename(path, &ch, &rate, &pcm);
    if (frames <= 0 || !pcm || ch <= 0 || rate <= 0) { free(mem); return FALSE; }
    DWORD outBytes = (DWORD)frames * ch * sizeof(short);
    fmt = (WAVEFORMATEX*)CoTaskMemAlloc(sizeof(WAVEFORMATEX));
    if (!fmt) { free(pcm); free(mem); return FALSE; }
    ZeroMemory(fmt, sizeof(*fmt));
    fmt->wFormatTag = WAVE_FORMAT_PCM;
    fmt->nChannels = (WORD)ch;
    fmt->nSamplesPerSec = (DWORD)rate;
    fmt->wBitsPerSample = 16;
    fmt->nBlockAlign = (fmt->nChannels * fmt->wBitsPerSample) / 8;
    fmt->nAvgBytesPerSec = fmt->nSamplesPerSec * fmt->nBlockAlign;
    BYTE* xa = (BYTE*)CoTaskMemAlloc(outBytes);
    if (!xa) { CoTaskMemFree(fmt); free(pcm); free(mem); return FALSE; }
    memcpy(xa, pcm, outBytes);
    free(pcm);
    audio = xa; bytes = outBytes;
  }
  free(mem);

  IXAudio2SourceVoice* voice = nullptr;
  if (FAILED(g_xaudio2->CreateSourceVoice(&voice, fmt))) {
    CoTaskMemFree(fmt); CoTaskMemFree(audio); return FALSE; }

  FreeBgmResources();

  XAUDIO2_BUFFER buf{};
  buf.AudioBytes = bytes;
  buf.pAudioData = audio;
  buf.Flags = XAUDIO2_END_OF_STREAM;
  if (bLoop) {
    buf.LoopBegin = 0;
    buf.LoopLength = 0; // loop entire buffer
    buf.LoopCount = XAUDIO2_LOOP_INFINITE;
  }
  if (FAILED(voice->SubmitSourceBuffer(&buf))) {
    voice->DestroyVoice(); CoTaskMemFree(fmt); CoTaskMemFree(audio); return FALSE; }

  m_pBgmFmt = fmt;
  m_pBgmAudio = audio;
  m_cbBgmAudio = bytes;
  m_pBgmVoice = voice;
  m_fBgmVolume = volume;
  ((IXAudio2SourceVoice*)m_pBgmVoice)->SetVolume(m_fBgmVolume);
  if (FAILED(((IXAudio2SourceVoice*)m_pBgmVoice)->Start())) {
    FreeBgmResources(); return FALSE; }
  return TRUE;
}

void CDXAudio::StopBGM() {
  FreeBgmResources();
}

void CDXAudio::SetBGMVolume(float volume) {
  m_fBgmVolume = volume;
  if (m_pBgmVoice) {
    ((IXAudio2SourceVoice*)m_pBgmVoice)->SetVolume(m_fBgmVolume);
  }
}
