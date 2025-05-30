package com.tans.tmediaplayer

import android.util.Log

@Suppress("ClassName")
object tMediaPlayerLog {

    var logLevel: LogLevel = LogLevel.Debug
        set(value) {
            synchronized(this) {
                field = value
            }
        }

    init {
        logLevel = if (BuildConfig.DEBUG) {
            LogLevel.Debug
        } else {
            LogLevel.Error
        }
    }

    internal inline fun d(tag: String, msgGetter: () -> String) {
        if (logLevel.ordinal <= LogLevel.Debug.ordinal) {
            Log.d(tag, msgGetter())
        }
    }

    internal inline fun e(tag: String, msgGetter: () -> String) {
        if (logLevel.ordinal <= LogLevel.Error.ordinal) {
            Log.e(tag, msgGetter())
        }
    }

    internal inline fun e(tag: String, msgGetter: () -> String, errorGetter: () -> Throwable?) {
        if (logLevel.ordinal <= LogLevel.Error.ordinal) {
            Log.e(tag, msgGetter(), errorGetter())
        }
    }

    enum class LogLevel {
        Debug,
        Error,
        NoLog
    }
}