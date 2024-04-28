#include "tmediaaudiotrack.h"


void playerBufferQueueCallback(SLAndroidSimpleBufferQueueItf bq, void *context) {

    // TODO: Callback.
//    if (context == nullptr) {
//        return;
//    }
//    tMediaAudioTrackContext *audioTrackContext = reinterpret_cast<tMediaAudioTrackContext *>(context);
//    if (bq != audioTrackContext->playerBufferQueueInterface) {
//        return;
//    }
    return;
}

tMediaOptResult tMediaAudioTrackContext::prepare(int bufferSize) {
    // region Init sl engine
    SLresult result = slCreateEngine(&engineObject, 0, nullptr, 0, nullptr, nullptr);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Create sl engine object fail: %d", result);
        return OptFail;
    }
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Realize sl engine object fail: %d", result);
        return OptFail;
    }
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineInterface);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Get sl engine interface fail: %d", result);
        return OptFail;
    }
    // endregion

    // region Init output mix
    const SLInterfaceID outputMixIds[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean outputMixReq[1] = {SL_BOOLEAN_FALSE};
    result = (*engineInterface)->CreateOutputMix(engineInterface, &outputMixObject, 1, outputMixIds, outputMixReq);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Create output mix object fail: %d", result);
        return OptFail;
    }
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Realize output mix object fail: %d", result);
        return OptFail;
    }
    // endregion

    // region Create player

    // Audio source configure
    SLDataLocator_AndroidSimpleBufferQueue audioInputQueue = {SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE, inputQueueSize};
    SLDataFormat_PCM audioInputFormat = {SL_DATAFORMAT_PCM, inputSampleChannels, inputSampleRate,
                                         inputSampleFormat, inputSampleFormat,
                                         SL_SPEAKER_FRONT_CENTER, SL_BYTEORDER_LITTLEENDIAN};
    SLDataSource audioInputSource = {&audioInputQueue, &audioInputFormat};

    // Audio sink configure
    SLDataLocator_OutputMix outputMix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSink = {&outputMix, NULL};

    const SLInterfaceID playerIds[3] = {SL_IID_BUFFERQUEUE, SL_IID_VOLUME, SL_IID_EFFECTSEND};
    const SLboolean playerReq[3] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engineInterface)->CreateAudioPlayer(engineInterface, &playerObject, &audioInputSource, &audioSink, 2, playerIds, playerReq);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Create audio player object fail: %d", result);
        return OptFail;
    }
    result = (*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Realize audio player fail: %d", result);
        return OptFail;
    }
    result = (*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerInterface);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Get audio player interface fail: %d", result);
        return OptFail;
    }
    result = (*playerObject)->GetInterface(playerObject, SL_IID_BUFFERQUEUE, &playerBufferQueueInterface);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Get audio buffer queue interface fail: %d", result);
        return OptFail;
    }
    result = (*playerBufferQueueInterface)->RegisterCallback(playerBufferQueueInterface, playerBufferQueueCallback, this);
    if (result != SL_RESULT_SUCCESS) {
        LOGE("Register audio queue callback fail: %d", result);
        return OptFail;
    }
    // endregion

    LOGD("Prepare audio track success!!");

    return OptSuccess;
}

tMediaOptResult tMediaAudioTrackContext::play() {
    SLresult result = (*playerInterface)->SetPlayState(playerInterface, SL_PLAYSTATE_PLAYING);
    if (result == SL_RESULT_SUCCESS) {
        return OptSuccess;
    } else {
        return OptFail;
    }
}

tMediaOptResult tMediaAudioTrackContext::pause() {
    SLresult result = (*playerInterface)->SetPlayState(playerInterface, SL_PLAYSTATE_PAUSED);
    if (result == SL_RESULT_SUCCESS) {
        return OptSuccess;
    } else {
        return OptFail;
    }
}

tMediaOptResult tMediaAudioTrackContext::enqueueBuffer(tMediaAudioBuffer *buffer) {
    // TODO:
}

tMediaOptResult tMediaAudioTrackContext ::clearBuffers() {
    // TODO:
}


void tMediaAudioTrackContext::release() {
    // TODO:
}