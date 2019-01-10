/*
 * vestige.h - simple header to allow VeSTige compilation and eventually work
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * This VeSTige header is included in this package in the good-faith
 * belief that it has been cleanly and legally reverse engineered
 * without reference to the official VST SDK and without its
 * developer(s) having agreed to the VST SDK license agreement.
 */

#ifndef __vestige__
#define __vestige__

#define kVstVersion 2400

#ifdef _WIN32
    typedef short vi16;
    typedef int vi32;
    typedef __int64 vi64;
#else
    #include <stdint.h>
    typedef int16_t vi16;
    typedef int32_t vi32;
    typedef int64_t vi64;
#endif

#if _WIN64 || __LP64__
    typedef vi64 viptr;
#else
    typedef vi32 viptr;
#endif

#if defined(__GNUC__)
    #pragma pack(push,8)
    #define VSTCALLBACK __cdecl
#elif defined(_WIN32) || defined(__FLAT__)
    #pragma pack(push)
    #pragma pack(8)
    #define VSTCALLBACK __cdecl
#else
    #define VSTCALLBACK
#endif

#undef CCONST
typedef struct AEffect AEffect;

typedef viptr (VSTCALLBACK *audioMasterCallback) (AEffect* effect, vi32 opcode, vi32 index, viptr value, void* ptr, float opt);
typedef viptr (VSTCALLBACK *AEffectDispatcherProc) (AEffect* effect, vi32 opcode, vi32 index, viptr value, void* ptr, float opt);
typedef void (VSTCALLBACK *AEffectProcessProc) (AEffect* effect, float** inputs, float** outputs, vi32 sampleFrames);
typedef void (VSTCALLBACK *AEffectProcessDoubleProc) (AEffect* effect, double** inputs, double** outputs, vi32 sampleFrames);
typedef void (VSTCALLBACK *AEffectSetParameterProc) (AEffect* effect, vi32 index, float parameter);
typedef float (VSTCALLBACK *AEffectGetParameterProc) (AEffect* effect, vi32 index);

#define CCONST(a, b, c, d) \
     ((((vi32)a) << 24) | (((vi32)b) << 16) | (((vi32)c) << 8) | (((vi32)d) << 0))

#define kEffectMagic CCONST ('V', 's', 't', 'P')

struct AEffect
{
    vi32 magic;
    AEffectDispatcherProc dispatcher;
    AEffectProcessProc process;
    AEffectSetParameterProc setParameter;
    AEffectGetParameterProc getParameter;
    vi32 numPrograms; 
    vi32 numParams;
    vi32 numInputs;
    vi32 numOutputs;
    vi32 flags;
    viptr resvd1;
    viptr resvd2;
    vi32 initialDelay;
    vi32 realQualities;
    vi32 offQualities;
    float ioRatio;
    void* object;
    void* user;
    vi32 uniqueID;
    vi32 version;
    AEffectProcessProc processReplacing;
    AEffectProcessDoubleProc processDoubleReplacing;
    char future[56];
};

enum
{
    effFlagsHasEditor = 1 << 0, effFlagsCanReplacing = 1 << 4, effFlagsProgramChunks = 1 << 5, effFlagsIsSynth = 1 << 8, effFlagsNoSoundInStop = 1 << 9, effFlagsCanDoubleReplacing = 1 << 12, effFlagsHasClip = 1 << 1, effFlagsHasVu = 1 << 2, effFlagsCanMono = 1 << 3, effFlagsExtIsAsync = 1 << 10, effFlagsExtHasBuffer = 1 << 11 
};

enum
{
    effOpen = 0, effClose, effSetProgram, effGetProgram, effSetProgramName, effGetProgramName, effGetParamLabel, effGetParamDisplay, effGetParamName, effGetVu, effSetSampleRate, effSetBlockSize, effMainsChanged, effEditGetRect, effEditOpen, effEditClose, effEditDraw, effEditMouse, effEditKey, effEditIdle, effEditTop, effEditSleep, effIdentify, effGetChunk, effSetChunk, effNumOpcodes
};

enum
{
    audioMasterAutomate = 0, audioMasterVersion, audioMasterCurrentId, audioMasterIdle, audioMasterPinConnected 
};

enum
{
    kVstMaxProgNameLen = 24, kVstMaxParamStrLen = 8, kVstMaxVendorStrLen = 64, kVstMaxProductStrLen = 64, kVstMaxEffectNameLen = 32 
};

struct ERect
{
    vi16 top;
    vi16 left;
    vi16 bottom;
    vi16 right;
};

enum
{
    kVstMaxNameLen = 64, kVstMaxLabelLen = 64, kVstMaxShortLabelLen = 8, kVstMaxCategLabelLen = 24, kVstMaxFileNameLen = 100    
};

typedef struct VstEvent
{
    vi32 type;
    vi32 byteSize;
    vi32 deltaFrames;
    vi32 flags;
    char data[16];
} VstEvent;

enum
{
    kVstMidiType = 1, kVstAudioType, kVstVideoType, kVstParameterType, kVstTriggerType, kVstSysExType   
};

struct VstEvents
{
    vi32 numEvents;
    viptr reserved;
    VstEvent* events[2];
};

struct VstMidiEvent
{
    vi32 type;
    vi32 byteSize;
    vi32 deltaFrames;
    vi32 flags;
    vi32 noteLength;
    vi32 noteOffset;
    char midiData[4];
    char detune;
    char noteOffVelocity;
    char reserved1;
    char reserved2;
};

enum
{
    kVstMidiEventIsRealtime = 1 << 0    
};

struct VstMidiSysexEvent
{
    vi32 type;
    vi32 byteSize;
    vi32 deltaFrames;
    vi32 flags;
    vi32 dumpBytes;
    viptr resvd1;
    char* sysexDump;
    viptr resvd2;
};

struct VstTimeInfo
{
    double samplePos;
    double sampleRate;
    double nanoSeconds;
    double ppqPos;
    double tempo;
    double barStartPos;
    double cycleStartPos;
    double cycleEndPos;
    vi32 timeSigNumerator;
    vi32 timeSigDenominator;
    vi32 smpteOffset;
    vi32 smpteFrameRate;
    vi32 samplesToNextClock;
    vi32 flags;
};

enum
{
    kVstTransportChanged = 1, kVstTransportPlaying = 1 << 1, kVstTransportCycleActive = 1 << 2, kVstTransportRecording = 1 << 3, kVstAutomationWriting = 1 << 6, kVstAutomationReading = 1 << 7, kVstNanosValid = 1 << 8, kVstPpqPosValid = 1 << 9, kVstTempoValid = 1 << 10, kVstBarsValid = 1 << 11, kVstCyclePosValid = 1 << 12, kVstTimeSigValid = 1 << 13, kVstSmpteValid = 1 << 14, kVstClockValid = 1 << 15  
};

enum
{
    kVstSmpte24fps = 0, kVstSmpte25fps = 1, kVstSmpte2997fps = 2, kVstSmpte30fps = 3, kVstSmpte2997dfps = 4, kVstSmpte30dfps = 5, kVstSmpteFilm16mm = 6, kVstSmpteFilm35mm = 7, kVstSmpte239fps = 10, kVstSmpte249fps = 11, kVstSmpte599fps = 12, kVstSmpte60fps = 13   
};

enum
{
    audioMasterWantMidi = audioMasterPinConnected + 2, audioMasterGetTime, audioMasterProcessEvents, audioMasterSetTime, audioMasterTempoAt, audioMasterGetNumAutomatableParameters, audioMasterGetParameterQuantization, audioMasterIOChanged, audioMasterNeedIdle, audioMasterSizeWindow, audioMasterGetSampleRate, audioMasterGetBlockSize, audioMasterGetInputLatency, audioMasterGetOutputLatency, audioMasterGetPreviousPlug, audioMasterGetNextPlug, audioMasterWillReplaceOrAccumulate, audioMasterGetCurrentProcessLevel, audioMasterGetAutomationState, audioMasterOfflineStart, audioMasterOfflineRead, audioMasterOfflineWrite, audioMasterOfflineGetCurrentPass, audioMasterOfflineGetCurrentMetaPass, audioMasterSetOutputSampleRate, audioMasterGetOutputSpeakerArrangement, audioMasterGetVendorString, audioMasterGetProductString, audioMasterGetVendorVersion, audioMasterVendorSpecific, audioMasterSetIcon, audioMasterCanDo, audioMasterGetLanguage, audioMasterOpenWindow, audioMasterCloseWindow, audioMasterGetDirectory, audioMasterUpdateDisplay, audioMasterBeginEdit, audioMasterEndEdit, audioMasterOpenFileSelector, audioMasterCloseFileSelector, audioMasterEditFile, audioMasterGetChunkFile, audioMasterGetInputSpeakerArrangement   
};

enum
{
    effProcessEvents = effSetChunk + 1, effCanBeAutomated, effString2Parameter, effGetNumProgramCategories, effGetProgramNameIndexed, effCopyProgram, effConnectInput, effConnectOutput, effGetInputProperties, effGetOutputProperties, effGetPlugCategory, effGetCurrentPosition, effGetDestinationBuffer, effOfflineNotify, effOfflinePrepare, effOfflineRun, effProcessVarIo, effSetSpeakerArrangement, effSetBlockSizeAndSampleRate, effSetBypass, effGetEffectName, effGetErrorText, effGetVendorString, effGetProductString, effGetVendorVersion, effVendorSpecific, effCanDo, effGetTailSize, effIdle, effGetIcon, effSetViewPosition, effGetParameterProperties, effKeysRequired, effGetVstVersion, effEditKeyDown, effEditKeyUp, effSetEditKnobMode, effGetMidiProgramName, effGetCurrentMidiProgram, effGetMidiProgramCategory, effHasMidiProgramsChanged, effGetMidiKeyName, effBeginSetProgram, effEndSetProgram, effGetSpeakerArrangement, effShellGetNextPlugin, effStartProcess, effStopProcess, effSetTotalSampleToProcess, effSetPanLaw, effBeginLoadBank, effBeginLoadProgram, effSetProcessPrecision, effGetNumMidiInputChannels, effGetNumMidiOutputChannels
};

enum
{
    kVstProcessPrecision32 = 0, kVstProcessPrecision64  
};

struct VstPinProperties
{
    char label[kVstMaxLabelLen];
    vi32 flags;
    vi32 arrangementType;
    char shortLabel[kVstMaxShortLabelLen];
    char future[48];
};

enum
{
    kVstPinIsActive = 1 << 0, kVstPinIsStereo = 1 << 1, kVstPinUseSpeaker = 1 << 2  
};

enum VstPlugCategory
{
    kPlugCategUnknown = 0, kPlugCategEffect, kPlugCategSynth, kPlugCategAnalysis, kPlugCategMastering, kPlugCategSpacializer, kPlugCategRoomFx, kPlugSurroundFx, kPlugCategRestoration, kPlugCategOfflineProcess, kPlugCategShell, kPlugCategGenerator, kPlugCategMaxCount  
};

struct MidiProgramName
{
    vi32 thisProgramIndex;
    char name[kVstMaxNameLen];
    char midiProgram;
    char midiBankMsb;
    char midiBankLsb;
    char reserved;
    vi32 parentCategoryIndex;
    vi32 flags;
};

enum
{
    kMidiIsOmni = 1 
};

struct MidiProgramCategory
{
    vi32 thisCategoryIndex;
    char name[kVstMaxNameLen];
    vi32 parentCategoryIndex;
    vi32 flags;
};

struct MidiKeyName
{
    vi32 thisProgramIndex;
    vi32 thisKeyNumber;
    char keyName[kVstMaxNameLen];
    vi32 reserved;
    vi32 flags;
};

typedef struct VstSpeakerProperties
{
    float azimuth;
    float elevation;
    float radius;
    float reserved;
    char name[kVstMaxNameLen];
    vi32 type;
    char future[28];
} VstSpeakerProperties;

struct VstSpeakerArrangement
{
    vi32 type;
    vi32 numChannels;
    VstSpeakerProperties speakers[8];
};

enum
{
    kSpeakerUndefined = 0x7fffffff, kSpeakerM = 0, kSpeakerL, kSpeakerR, kSpeakerC, kSpeakerLfe, kSpeakerLs, kSpeakerRs, kSpeakerLc, kSpeakerRc, kSpeakerS, kSpeakerCs = kSpeakerS, kSpeakerSl, kSpeakerSr, kSpeakerTm, kSpeakerTfl, kSpeakerTfc, kSpeakerTfr, kSpeakerTrl, kSpeakerTrc, kSpeakerTrr, kSpeakerLfe2
};

enum
{
    kSpeakerU32 = -32, kSpeakerU31, kSpeakerU30, kSpeakerU29, kSpeakerU28, kSpeakerU27, kSpeakerU26, kSpeakerU25, kSpeakerU24, kSpeakerU23, kSpeakerU22, kSpeakerU21, kSpeakerU20, kSpeakerU19, kSpeakerU18, kSpeakerU17, kSpeakerU16, kSpeakerU15, kSpeakerU14, kSpeakerU13, kSpeakerU12, kSpeakerU11, kSpeakerU10, kSpeakerU9, kSpeakerU8, kSpeakerU7, kSpeakerU6, kSpeakerU5, kSpeakerU4, kSpeakerU3, kSpeakerU2, kSpeakerU1
};

enum
{
    kSpeakerArrUserDefined = -2, kSpeakerArrEmpty = -1, kSpeakerArrMono = 0, kSpeakerArrStereo, kSpeakerArrStereoSurround, kSpeakerArrStereoCenter, kSpeakerArrStereoSide, kSpeakerArrStereoCLfe, kSpeakerArr30Cine, kSpeakerArr30Music, kSpeakerArr31Cine, kSpeakerArr31Music, kSpeakerArr40Cine, kSpeakerArr40Music, kSpeakerArr41Cine, kSpeakerArr41Music, kSpeakerArr50, kSpeakerArr51, kSpeakerArr60Cine, kSpeakerArr60Music, kSpeakerArr61Cine, kSpeakerArr61Music, kSpeakerArr70Cine, kSpeakerArr70Music, kSpeakerArr71Cine, kSpeakerArr71Music, kSpeakerArr80Cine, kSpeakerArr80Music, kSpeakerArr81Cine, kSpeakerArr81Music, kSpeakerArr102, kNumSpeakerArr
};

struct VstPatchChunkInfo
{
    vi32 version;
    vi32 pluginUniqueID;
    vi32 pluginVersion;
    vi32 numElements;
    char future[48];
};

enum
{
    kLinearPanLaw = 0, kEqualPowerPanLaw    
};

enum
{
    kVstProcessLevelUnknown = 0, kVstProcessLevelUser, kVstProcessLevelRealtime, kVstProcessLevelPrefetch, kVstProcessLevelOffline  
};

enum
{
    kVstAutomationUnsupported = 0, kVstAutomationOff, kVstAutomationRead, kVstAutomationWrite, kVstAutomationReadWrite  
};

#pragma pack(pop)

#endif // __vestige__
