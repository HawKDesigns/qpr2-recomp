"""Run the existing Windows XAudio2 ring regression with API declarations only.
The production backend and original test drive every slot and pending sample.
"""
from pathlib import Path
import subprocess,tempfile
root=Path(__file__).resolve().parents[1]
windows=r'''
#pragma once
#include <stdint.h>
#include <string.h>
typedef int32_t HRESULT;typedef uint32_t DWORD,UINT32;typedef unsigned char BYTE;
#define STDMETHODCALLTYPE
#define S_OK 0
#define S_FALSE 1
#define E_FAIL ((HRESULT)0x80004005)
#define RPC_E_CHANGED_MODE ((HRESULT)0x80010106)
#define SUCCEEDED(x) ((x)>=0)
#define FAILED(x) ((x)<0)
#define COINIT_MULTITHREADED 0
#define WAVE_FORMAT_PCM 1
typedef struct {uint16_t wFormatTag,nChannels;uint32_t nSamplesPerSec,nAvgBytesPerSec;uint16_t nBlockAlign,wBitsPerSample,cbSize;} WAVEFORMATEX;
static DWORD GetTickCount(void){static DWORD t;t+=21;return t;}
'''
xaudio=r'''
#pragma once
#include "windows.h"
#define XAUDIO2_DEFAULT_PROCESSOR 0
#define XAUDIO2_DEFAULT_FREQ_RATIO 2.0f
#define XAUDIO2_COMMIT_NOW 0
#define XAUDIO2_VOICE_NOSAMPLESPLAYED 0x100
struct IXAudio2MasteringVoice;struct IXAudio2SourceVoice;
typedef struct {void (*DestroyVoice)(struct IXAudio2MasteringVoice*);} IXAudio2MasteringVoiceVtbl;
typedef struct {void (*DestroyVoice)(struct IXAudio2SourceVoice*);} IXAudio2SourceVoiceVtbl;
typedef struct IXAudio2MasteringVoice {IXAudio2MasteringVoiceVtbl*lpVtbl;} IXAudio2MasteringVoice;
typedef struct IXAudio2SourceVoice {IXAudio2SourceVoiceVtbl*lpVtbl;} IXAudio2SourceVoice;
typedef struct {void*lpVtbl;} IXAudio2;
typedef struct {void*pCurrentBufferContext;UINT32 BuffersQueued;uint64_t SamplesPlayed;} XAUDIO2_VOICE_STATE;
typedef struct {UINT32 Flags,AudioBytes;const BYTE*pAudioData;UINT32 PlayBegin,PlayLength,LoopBegin,LoopLength,LoopCount;void*pContext;} XAUDIO2_BUFFER;
'''
with tempfile.TemporaryDirectory(prefix='qpr2-audio-queue-') as d:
 p=Path(d);(p/'windows.h').write_text(windows);(p/'xaudio2.h').write_text(xaudio)
 exe=p/'test'
 subprocess.run(['gcc','-std=gnu11','-O1','-D_WIN32=1','-Werror=implicit-function-declaration',
  '-fsanitize=undefined','-fno-sanitize-recover=all','-I'+str(p),
  str(root/'third_party/xboxrecomp/tests/xaudio2/test_main.c'),'-o',str(exe)],check=True)
 subprocess.run([str(exe)],check=True,timeout=10)
