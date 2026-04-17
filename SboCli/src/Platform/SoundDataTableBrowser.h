/// @file SoundDataTableBrowser.h
/// @brief Web版（Emscripten）専用 SOUNDID ↔ WAVファイル名マッピングテーブル 定義ファイル
#pragma once

namespace SoundDataBrowser {
    /// @brief 効果音の総数を返す
    int   GetSoundCount(void);
    /// @brief テーブルインデックス → SOUNDID
    DWORD GetSoundIDAt(int nNo);
    /// @brief SOUNDID → テーブルインデックス（SOUNDID_CLAP0203はランダムでCLAP02/CLAP03に振る）
    int   GetSoundNo(DWORD dwSoundID);
    /// @brief テーブルインデックス → WAVファイル名（UTF-8）
    const char* GetWavFileName(int nNo);
}
