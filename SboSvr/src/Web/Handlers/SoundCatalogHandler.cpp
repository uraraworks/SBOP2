#include "StdAfx.h"
#include "SoundCatalogHandler.h"

#include <sstream>
#include <string>

// ---------------------------------------------------------------------------
// サウンドエントリテーブル
//
// !! SboSoundData/resource.h の IDR_WAVE_* と同期が必須 !!
//    resource.h を変更した場合は必ずこのテーブルも更新すること。
//
// id   : resource.h の #define 値
// pszKey  : IDR_WAVE_* の定義名そのまま
// pszLabel: 将来的に和名へ差し替え可能。現在は IDR_WAVE_ を除いた名前を機械生成。
//           ここに直接日本語文字列を書けば上書き表示される。
// ---------------------------------------------------------------------------

namespace
{

struct SSoundEntry
{
    int         nId;
    const char *pszKey;
    const char *pszLabel; // NULL の場合は key から IDR_WAVE_ を除いた名前を生成
};

// IDR_WAVE1/IDR_WAVE2/IDR_WAVE3/IDR_WAVE5/IDR_WAVE6 はエイリアスなので除外。
// IDR_PINPONPANPON も IDR_WAVE1 の別名なので除外。
// !! SboSoundData/resource.h と同期必須 !!
static const SSoundEntry kSoundEntries[] = {
    { 101, "IDR_WAVE_PI73",          NULL },
    { 102, "IDR_WAVE_ASI_2",         NULL },
    { 103, "IDR_WAVE_PUSH51_C",      NULL },
    { 104, "IDR_WAVE_SELECT",        NULL },
    { 105, "IDR_WAVE_ATACK_1",       NULL },
    { 106, "IDR_WAVE_CHARI14_A",     NULL },
    { 107, "IDR_WAVE_SWING24_C",     NULL },
    { 108, "IDR_WAVE_SWING25_C",     NULL },
    { 109, "IDR_WAVE_HIT0",          NULL },
    { 110, "IDR_WAVE_HIT1",          NULL },
    { 111, "IDR_WAVE_MONEY",         NULL },
    { 112, "IDR_WAVE_ITEM_SMALL",    NULL },
    { 113, "IDR_WAVE_ITEM_MIDIUM",   NULL },
    { 114, "IDR_WAVE_ITEM_LARGE",    NULL },
    { 115, "IDR_WAVE_MATERIAL",      NULL },
    { 116, "IDR_WAVE_WEAPON",        NULL },
    { 117, "IDR_WAVE_CLOTH",         NULL },
    { 118, "IDR_WAVE_CRASH20_D",     NULL },
    { 119, "IDR_WAVE_KUSA",          NULL },
    { 120, "IDR_WAVE_KAI",           NULL },
    { 121, "IDR_WAVE_CHAT",          NULL },
    { 122, "IDR_WAVE_W_CHAT",        NULL },
    { 123, "IDR_WAVE_CLAP02",        NULL },
    { 124, "IDR_WAVE_CLAP03",        NULL },
    { 125, "IDR_WAVE_BALLOON1",      NULL },
    { 126, "IDR_WAVE_BIKKURI",       NULL },
    { 127, "IDR_WAVE_WHISTLES",      NULL },
    { 128, "IDR_WAVE_WHISTLEL",      NULL },
    { 129, "IDR_WAVE_GIMON",         NULL },
    { 130, "IDR_WAVE_NIKU",          NULL },
    { 131, "IDR_WAVE_KIZUKU",        NULL },
    // 132 は IDR_WAVE1 / IDR_PINPONPANPON のエイリアスなので除外
    { 133, "IDR_WAVE_SWOON",         NULL },
    { 134, "IDR_WAVE_URARA_WORKS",   NULL },
    { 135, "IDR_WAVE_PUYON_ATACK",   NULL },
    { 137, "IDR_WAVE_LIQUID",        NULL },
    { 138, "IDR_WAVE_BOW_ATACK",     NULL },
    { 139, "IDR_WAVE_MEAL",          NULL },
    { 140, "IDR_WAVE_DEFENSE",       NULL },
    // 141 は IDR_WAVE2 のエイリアスだが IDR_WAVE_SHIELD が正規名
    { 141, "IDR_WAVE_SHIELD",        NULL },
    { 142, "IDR_WAVE_PUYON_MOVE",    NULL },
    { 143, "IDR_WAVE_PUYON_DEAD",    NULL },
    // 144 は IDR_WAVE5 のエイリアスだが IDR_WAVE_EXIST が正規名
    { 144, "IDR_WAVE_EXIST",         NULL },
    { 145, "IDR_WAVE_DROP_BIN",      NULL },
    { 146, "IDR_WAVE_OPEN_BAG",      NULL },
    { 147, "IDR_WAVE_CRITICAL2",     NULL },
    { 149, "IDR_WAVE_HIT2",          NULL },
    { 150, "IDR_WAVE_OPEN_WINDOW",   NULL },
    { 151, "IDR_WAVE_CRITICAL3",     NULL },
    { 152, "IDR_WAVE_CRITICAL1",     NULL },
    { 153, "IDR_WAVE_LVUP",          NULL },
    { 154, "IDR_WAVE_MISS",          NULL },
    { 155, "IDR_WAVE_HIT4",          NULL },
    { 156, "IDR_WAVE_TRASH",         NULL },
    { 157, "IDR_WAVE_CHARGE",        NULL },
    { 158, "IDR_WAVE_HIT3",          NULL },
    { 159, "IDR_WAVE_CHARGE2",       NULL },
    { 160, "IDR_WAVE_DR_BASS01",     NULL },
    { 161, "IDR_WAVE_DR_BASS02",     NULL },
    { 162, "IDR_WAVE_DR_BASS05",     NULL },
    { 163, "IDR_WAVE_DR_CLAP01",     NULL },
    { 164, "IDR_WAVE_DR_SNE02",      NULL },
    { 165, "IDR_WAVE_KIE_ATACK",     NULL },
    { 166, "IDR_WAVE_KIE_DEAD",      NULL },
    { 167, "IDR_WAVE_FLY",           NULL },
    { 168, "IDR_WAVE_STEP03",        NULL },
};

static const size_t kSoundEntryCount = sizeof(kSoundEntries) / sizeof(kSoundEntries[0]);

// IDR_WAVE_ プレフィクスを除いた文字列を生成するヘルパー
std::string MakeDefaultLabel(const char *pszKey)
{
    const char *kPrefix = "IDR_WAVE_";
    const size_t kPrefixLen = 9; // strlen("IDR_WAVE_")
    if (pszKey == NULL) {
        return std::string();
    }
    const std::string key(pszKey);
    if (key.size() > kPrefixLen &&
        key.compare(0, kPrefixLen, kPrefix) == 0) {
        return key.substr(kPrefixLen);
    }
    return key;
}

// 簡易 JSON エスケープ
std::string EscapeJson(const char *pszText)
{
    std::string out;
    if (pszText == NULL) {
        return out;
    }
    for (const char *p = pszText; *p != '\0'; ++p) {
        unsigned char ch = static_cast<unsigned char>(*p);
        switch (ch) {
        case '"':  out += "\\\""; break;
        case '\\': out += "\\\\"; break;
        case '\b': out += "\\b";  break;
        case '\f': out += "\\f";  break;
        case '\n': out += "\\n";  break;
        case '\r': out += "\\r";  break;
        case '\t': out += "\\t";  break;
        default:
            if (ch < 0x20) {
                char buf[8];
                sprintf_s(buf, sizeof(buf), "\\u%04x", ch);
                out += buf;
            } else {
                out += static_cast<char>(ch);
            }
            break;
        }
    }
    return out;
}

} // namespace

// ---------------------------------------------------------------------------
// CSoundCatalogHandler::Handle
// ---------------------------------------------------------------------------

void CSoundCatalogHandler::Handle(const HttpRequest & /*request*/, HttpResponse &response)
{
    std::ostringstream oss;
    oss << '[';

    for (size_t i = 0; i < kSoundEntryCount; ++i) {
        if (i > 0) {
            oss << ',';
        }
        const SSoundEntry &e = kSoundEntries[i];

        // label: pszLabel が設定されていればそれを使用、なければ key から生成
        std::string label;
        if (e.pszLabel != NULL && e.pszLabel[0] != '\0') {
            label = e.pszLabel;
        } else {
            label = MakeDefaultLabel(e.pszKey);
        }

        oss << '{'
            << "\"id\":"     << e.nId << ','
            << "\"key\":\""  << EscapeJson(e.pszKey) << "\","
            << "\"label\":\"" << EscapeJson(label.c_str()) << "\""
            << '}';
    }

    oss << ']';

    response.statusLine = "HTTP/1.1 200 OK";
    response.SetJsonBody(oss.str());
}
