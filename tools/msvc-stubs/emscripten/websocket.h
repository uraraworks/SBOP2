#pragma once

// ブラウザ版プリフライトの cl.exe 構文チェック専用スタブ。
// Common/UraraSockTCPWebSocket.h / .cpp が実際に使っている型・関数・定数
// だけを最小限で宣言する。リンクはしない（em++ 側の構文チェックのみ）ので
// 実装は空でよい。

#if defined(__cplusplus)
extern "C" {
#endif

typedef int EM_BOOL;
#define EM_TRUE  1
#define EM_FALSE 0

typedef int EMSCRIPTEN_WEBSOCKET_T;
#define EMSCRIPTEN_RESULT int
#define EMSCRIPTEN_RESULT_SUCCESS 0

typedef struct EmscriptenWebSocketCreateAttributes {
    const char *url;
    const char *protocols;
    EM_BOOL     createOnMainThread;
} EmscriptenWebSocketCreateAttributes;

typedef struct EmscriptenWebSocketOpenEvent {
    EMSCRIPTEN_WEBSOCKET_T socket;
} EmscriptenWebSocketOpenEvent;

typedef struct EmscriptenWebSocketMessageEvent {
    EMSCRIPTEN_WEBSOCKET_T socket;
    const unsigned char   *data;
    unsigned int           numBytes;
    EM_BOOL                 isText;
} EmscriptenWebSocketMessageEvent;

typedef struct EmscriptenWebSocketCloseEvent {
    EMSCRIPTEN_WEBSOCKET_T socket;
    int                     code;
    const char             *reason;
    EM_BOOL                 wasClean;
} EmscriptenWebSocketCloseEvent;

typedef struct EmscriptenWebSocketErrorEvent {
    EMSCRIPTEN_WEBSOCKET_T socket;
} EmscriptenWebSocketErrorEvent;

typedef EM_BOOL (*em_websocket_open_callback_func)(int eventType, const EmscriptenWebSocketOpenEvent *websocketEvent, void *userData);
typedef EM_BOOL (*em_websocket_message_callback_func)(int eventType, const EmscriptenWebSocketMessageEvent *websocketEvent, void *userData);
typedef EM_BOOL (*em_websocket_close_callback_func)(int eventType, const EmscriptenWebSocketCloseEvent *websocketEvent, void *userData);
typedef EM_BOOL (*em_websocket_error_callback_func)(int eventType, const EmscriptenWebSocketErrorEvent *websocketEvent, void *userData);

void emscripten_websocket_init_create_attributes(EmscriptenWebSocketCreateAttributes *attributes);
EMSCRIPTEN_WEBSOCKET_T emscripten_websocket_new(const EmscriptenWebSocketCreateAttributes *attributes);
int emscripten_websocket_close(EMSCRIPTEN_WEBSOCKET_T socket, unsigned short code, const char *reason);
int emscripten_websocket_delete(EMSCRIPTEN_WEBSOCKET_T socket);
int emscripten_websocket_send_binary(EMSCRIPTEN_WEBSOCKET_T socket, const void *data, unsigned int length);
int emscripten_websocket_set_onopen_callback(EMSCRIPTEN_WEBSOCKET_T socket, void *userData, em_websocket_open_callback_func callback);
int emscripten_websocket_set_onmessage_callback(EMSCRIPTEN_WEBSOCKET_T socket, void *userData, em_websocket_message_callback_func callback);
int emscripten_websocket_set_onclose_callback(EMSCRIPTEN_WEBSOCKET_T socket, void *userData, em_websocket_close_callback_func callback);
int emscripten_websocket_set_onerror_callback(EMSCRIPTEN_WEBSOCKET_T socket, void *userData, em_websocket_error_callback_func callback);

#if defined(__cplusplus)
}
#endif
