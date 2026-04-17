/// @file SoundDataTableBrowser.cpp
/// @brief Web版（Emscripten）専用 SOUNDID ↔ WAVファイル名マッピングテーブル 実装ファイル
///
/// SboSoundData.dll の代替として、SOUNDID と WAVファイル名の対応を静的テーブルで管理する。
/// WAVファイルは Emscripten の --preload-file で /WAVE/ にマウントされていることが前提。
#include "stdafx.h"
#include <cstdlib>  // rand
#include "SoundDataTableBrowser.h"
#include "SboSoundData.h"  // SOUNDID_* 定数

// -----------------------------------------------------------------------
// 内部テーブル定義
// -----------------------------------------------------------------------

namespace {

struct SoundEntry {
    DWORD       dwSoundID;    // SOUNDID_* 定数
    const char* pszWavFile;   // WAVファイル名（UTF-8）。/WAVE/ 以下のファイル名のみ。
};

// SboSoundData.cpp の g_aSoundDataTbl と SboSoundData.rc の IDR_WAVE_* → WAVファイル名
// の対応に基づいて構築した静的テーブル。
// 注意: SOUNDID_CLAP0203 は SOUNDID_CLAP03 と同じ enum 値(23)のため除外し、
//       GetSoundNo() でランダムに CLAP02/CLAP03 に振る。
static const SoundEntry g_aSoundTable[] = {
    { SOUNDID_OK_PI73,           u8"pi73.wav"                                  },
    { SOUNDID_FOOT_ASI_2,        u8"asi_2.wav"                                 },
    { SOUNDID_CURSORMOVE,        u8"select.wav"                                },
    { SOUNDID_CANCEL,            u8"push51_c.wav"                              },
    { SOUNDID_CHARI14_A,         u8"chari14_a.wav"                             },
    { SOUNDID_SWING24_C,         u8"swing24_c.wav"                             },
    { SOUNDID_SWING25_C,         u8"swing25_c.wav"                             },
    { SOUNDID_HIT0,              u8"hit0.wav"                                  },
    { SOUNDID_HIT1,              u8"hit1.wav"                                  },
    { SOUNDID_DROP_MONEY,        u8"\u304a\u91d1.wav"                          }, // お金.wav
    { SOUNDID_DROP_ITEM_SMALL,   u8"\u5c0f\u30a2\u30a4\u30c6\u30e0.wav"       }, // 小アイテム.wav
    { SOUNDID_DROP_ITEM_MIDIUM,  u8"\u4e2d\u30a2\u30a4\u30c6\u30e0.wav"       }, // 中アイテム.wav
    { SOUNDID_DROP_ITEM_LARGE,   u8"\u5927\u30a2\u30a4\u30c6\u30e0.wav"       }, // 大アイテム.wav
    { SOUNDID_DROP_MATERIAL,     u8"\u7d20\u6750.wav"                          }, // 素材.wav
    { SOUNDID_DROP_WEAPON,       u8"\u6b66\u5668.wav"                          }, // 武器.wav
    { SOUNDID_DROP_CLOTH,        u8"\u670d\u30fb\u30ed\u30fc\u30d6.wav"        }, // 服・ローブ.wav
    { SOUNDID_CRASH20_D,         u8"crash20_d.wav"                             },
    { SOUNDID_KUSA,              u8"kusa.wav"                                  },
    { SOUNDID_KAI,               u8"kai.wav"                                   },
    { SOUNDID_CHAT,              u8"chat.wav"                                  },
    { SOUNDID_W_CHAT,            u8"w_chat.wav"                                },
    { SOUNDID_CLAP02,            u8"clap02_loop.wav"                           },
    { SOUNDID_CLAP03,            u8"clap03_loop.wav"                           },
    { SOUNDID_BALLOON1,          u8"balloon1.wav"                              },
    { SOUNDID_BIKKURI,           u8"bikkuri.wav"                               },
    { SOUNDID_WHISTLES,          u8"whistleS.wav"                              },
    { SOUNDID_WHISTLEL,          u8"whistleL.wav"                              },
    { SOUNDID_GIMON,             u8"gimon.wav"                                 },
    { SOUNDID_NIKU,              u8"niku.wav"                                  },
    { SOUNDID_KIZUKU,            u8"kizuku.wav"                                },
    { SOUNDID_PINPONPANPON,      u8"\u30d4\u30f3\u30dd\u30f3\u30d1\u30f3\u30dd\u30fc\u30f3.wav" }, // ピンポンパンポーン.wav
    { SOUNDID_SWOON,             u8"\u6c17\u7d76.wav"                          }, // 気絶.wav
    { SOUNDID_URARA_WORKS,       u8"\uff08\u52a0\u5de5\u6e08\uff09\u30a6\u30e9\u30e9\u30ef\u30fc\u30af\u30b9(20050923).wav" }, // （加工済）ウララワークス(20050923).wav
    { SOUNDID_PUYON_ATACK,       u8"\u30d7\u30e8\u30f3\u653b\u6483.wav"       }, // プヨン攻撃.wav
    { SOUNDID_LIQUID,            u8"\u6db2\u4f53.wav"                          }, // 液体.wav
    { SOUNDID_BOW_ATACK,         u8"\u5f13\u653b\u6483.wav"                    }, // 弓攻撃.wav
    { SOUNDID_MEAL,              u8"\u98df\u4e8b.wav"                          }, // 食事.wav
    { SOUNDID_DEFENSE,           u8"\u9632\u5fa1.wav"                          }, // 防御.wav
    { SOUNDID_SHIELD,            u8"\u76fe\u69cb\u3048.wav"                    }, // 盾構え.wav
    { SOUNDID_PUYON_MOVE,        u8"\u30d7\u30e8\u30f3\u79fb\u52d5.wav"       }, // プヨン移動.wav
    { SOUNDID_PUYON_DEAD,        u8"\u30d7\u30e8\u30f3\u6b7b\u4ea1.wav"       }, // プヨン死亡.wav
    { SOUNDID_EXIST,             u8"\u6d88\u6ec5.wav"                          }, // 消滅.wav
    { SOUNDID_TRASH,             u8"\u30b4\u30df\u7b31\u958b\u304f\u30fb\u6368\u3066\u308b.wav" }, // ゴミ箱開く・捨てる.wav
    { SOUNDID_DROP_BIN,          u8"\u30c9\u30ed\u30c3\u30d7\uff1a\u30d3\u30f3.wav" }, // ドロップ：ビン.wav
    { SOUNDID_OPEN_BAG,          u8"\u30d0\u30c3\u30b0\u958b\u304f.wav"        }, // バッグ開く.wav
    { SOUNDID_MISS,              u8"\u30df\u30b9.wav"                          }, // ミス.wav
    { SOUNDID_LVUP,              u8"\u30ec\u30d9\u30eb\u30a2\u30c3\u30d7.wav"  }, // レベルアップ.wav
    { SOUNDID_CRITICAL1,         u8"\u65ac\u6483\u30af\u30ea\u30c6\u30a3\u30ab\u30eb.wav" }, // 斬撃クリティカル.wav
    { SOUNDID_HIT2,              u8"\u65ac\u6483\u30d2\u30c3\u30c8\uff11.wav"  }, // 斬撃ヒット１.wav
    { SOUNDID_HIT3,              u8"\u65ac\u6483\u30d2\u30c3\u30c8\uff12.wav"  }, // 斬撃ヒット２.wav
    { SOUNDID_OPEN_WINDOW,       u8"\u7a93\u958b\u304f.wav"                    }, // 窓開く.wav
    { SOUNDID_CRITICAL2,         u8"\u6253\u6483\u30af\u30ea\u30c6\u30a3\u30ab\u30eb.wav" }, // 打撃クリティカル.wav
    { SOUNDID_CRITICAL3,         u8"\u77e2\u30af\u30ea\u30c6\u30a3\u30ab\u30eb.wav" }, // 矢クリティカル.wav
    { SOUNDID_HIT4,              u8"\u77e2\u30d2\u30c3\u30c8.wav"              }, // 矢ヒット.wav
    { SOUNDID_CHARGE,            u8"\u6e9c\u3081.wav"                          }, // 溜め.wav
    { SOUNDID_CHARGE2,           u8"\u6e9c\u3081\uff12.wav"                    }, // 溜め２.wav
    { SOUNDID_DR_BASS01,         u8"dr-bass01.wav"                             },
    { SOUNDID_DR_BASS02,         u8"dr-bass02.wav"                             },
    { SOUNDID_DR_BASS05,         u8"dr-bass05.wav"                             },
    { SOUNDID_DR_CLAP01,         u8"dr-clap01.wav"                             },
    { SOUNDID_DR_SNE02,          u8"dr-sne02.wav"                              },
    { SOUNDID_KIE_ATACK,         u8"\u30ad\u30fc\u653b\u6483.wav"             }, // キー攻撃.wav
    { SOUNDID_KIE_DEAD,          u8"\u30ad\u30fc\u6b7b\u4ea1.wav"             }, // キー死亡.wav
    { SOUNDID_FLY,               u8"\u7fbd\u3070\u305f\u304d.wav"             }, // 羽ばたき.wav
    { SOUNDID_STEP03,            u8"step03.wav"                                },
};

// テーブルエントリ数（末尾番兵なし）
static const int g_nSoundTableCount =
    static_cast<int>(sizeof(g_aSoundTable) / sizeof(g_aSoundTable[0]));

}  // namespace (無名)

// -----------------------------------------------------------------------
// SoundDataBrowser API 実装
// -----------------------------------------------------------------------

namespace SoundDataBrowser {

int GetSoundCount(void)
{
    return g_nSoundTableCount;
}

DWORD GetSoundIDAt(int nNo)
{
    if (nNo < 0 || nNo >= g_nSoundTableCount) {
        return 0;
    }
    return g_aSoundTable[nNo].dwSoundID;
}

int GetSoundNo(DWORD dwSoundID)
{
    // SOUNDID_CLAP0203 は SOUNDID_CLAP03 と同じ値(23)だが、
    // SboSoundData.cpp の挙動に合わせてランダムに CLAP02/CLAP03 に振る
    if (dwSoundID == SOUNDID_CLAP0203) {
        dwSoundID = ((rand() % 100) > 50) ? SOUNDID_CLAP02 : SOUNDID_CLAP03;
    }

    for (int i = 0; i < g_nSoundTableCount; i++) {
        if (g_aSoundTable[i].dwSoundID == dwSoundID) {
            return i;
        }
    }
    return -1;
}

const char* GetWavFileName(int nNo)
{
    if (nNo < 0 || nNo >= g_nSoundTableCount) {
        return nullptr;
    }
    return g_aSoundTable[nNo].pszWavFile;
}

}  // namespace SoundDataBrowser
