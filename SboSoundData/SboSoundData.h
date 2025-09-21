/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* ファイル名：	SboSoundData.h												 */
/* 内容：		音関連DLL 定義ファイル										 */
/* 作成：		年がら年中春うらら(URARA-works)								 */
/* 作成開始日：	2007/05/03													 */
/* ========================================================================= */

#ifdef SOUNDDATA_EXPORTS
#define SOUNDDATA_API __declspec(dllexport)
#else
#define SOUNDDATA_API __declspec(dllimport)
#endif


/* ========================================================================= */
/* 定数の定義																 */
/* ========================================================================= */

/* 効果音 */
enum {
	SOUNDID_NONE				= 0,
	SOUNDID_OK_PI73				= 1,				/* 決定音(pi73.wav) */
	SOUNDID_FOOT_ASI_2			= 2,				/* 足音(asi_2.wav) */
	SOUNDID_CURSORMOVE			= 3,				/* カーソル移動 */
	SOUNDID_CANCEL				= 4,				/* キャンセル */
	SOUNDID_CHARI14_A			= 5,				/* 戦闘開始 */
	SOUNDID_SWING24_C			= 6,				/* 武器振り */
	SOUNDID_SWING25_C			= 7,				/* 武器突き */
	SOUNDID_HIT0				= 8,				/* ヒット0 */
	SOUNDID_HIT1				= 9,				/* ヒット1 */
	SOUNDID_DROP_MONEY			= 10,				/* ドロップ:お金 */
	SOUNDID_DROP_ITEM_SMALL		= 11,				/* ドロップ:小アイテム */
	SOUNDID_DROP_ITEM_MIDIUM	= 12,				/* ドロップ:中アイテム */
	SOUNDID_DROP_ITEM_LARGE		= 13,				/* ドロップ:大アイテム */
	SOUNDID_DROP_MATERIAL		= 14,				/* ドロップ:素材 */
	SOUNDID_DROP_WEAPON			= 15,				/* ドロップ:武器 */
	SOUNDID_DROP_CLOTH			= 16,				/* ドロップ:服・ローブ */
	SOUNDID_CRASH20_D			= 17,				/* 樽が壊れる音 */
	SOUNDID_KUSA				= 18,				/* 草を刈る音 */
	SOUNDID_KAI					= 19,				/* 貝が壊れる音 */
	SOUNDID_CHAT				= 20,				/* チャット */
	SOUNDID_W_CHAT				= 21,				/* アナウンス */
	SOUNDID_CLAP02				= 22,				/* 拍手2 */
	SOUNDID_CLAP03				= 23,				/* 拍手3 */
	SOUNDID_CLAP0203			= 23,				/* 拍手2か拍手3 */
	SOUNDID_BALLOON1			= 24,				/* 噴出し */
	SOUNDID_BIKKURI				= 25,				/* びっくり */
	SOUNDID_WHISTLES			= 26,				/* ホイッスルＳ */
	SOUNDID_WHISTLEL			= 27,				/* ホイッスルＬ */
	SOUNDID_GIMON				= 28,				/* 疑問 */
	SOUNDID_NIKU				= 29,				/* 肉 */
	SOUNDID_KIZUKU				= 30,				/* 気付く */
	SOUNDID_PINPONPANPON		= 31,				/* 放送前のお知らせ音 */
	SOUNDID_SWOON				= 32,				/* 気絶 */
	SOUNDID_URARA_WORKS			= 33,				/* ウララワークス */
	SOUNDID_PUYON_ATACK			= 34,				/* プヨン攻撃 */
	SOUNDID_LIQUID				= 35,				/* 液体 */
	SOUNDID_BOW_ATACK			= 36,				/* 弓攻撃 */
	SOUNDID_MEAL				= 37,				/* 食事 */
	SOUNDID_DEFENSE				= 38,				/* 防御 */
	SOUNDID_SHIELD				= 39,				/* 盾構え */
	SOUNDID_PUYON_MOVE			= 40,				/* プヨン移動 */
	SOUNDID_PUYON_DEAD			= 41,				/* プヨン死亡 */
	SOUNDID_EXIST				= 42,				/* 消滅 */
	SOUNDID_TRASH				= 43,				/* ゴミ箱開く・捨てる */
	SOUNDID_DROP_BIN			= 44,				/* ドロップ：ビン */
	SOUNDID_OPEN_BAG			= 45,				/* バッグ開く */
	SOUNDID_MISS				= 46,				/* ミス */
	SOUNDID_LVUP				= 47,				/* レベルアップ */
	SOUNDID_CRITICAL1			= 48,				/* 斬撃クリティカル */
	SOUNDID_HIT2				= 49,				/* 斬撃ヒット１ */
	SOUNDID_HIT3				= 50,				/* 斬撃ヒット２ */
	SOUNDID_OPEN_WINDOW			= 51,				/* 窓開く */
	SOUNDID_CRITICAL2			= 52,				/* 打撃クリティカル */
	SOUNDID_CRITICAL3			= 53,				/* 矢クリティカル */
	SOUNDID_HIT4				= 54,				/* 矢ヒット */
	SOUNDID_CHARGE				= 55,				/* 溜め */
	SOUNDID_CHARGE2				= 56,				/* 溜め２ */
	SOUNDID_DR_BASS01			= 57,				/* ベース１ */
	SOUNDID_DR_BASS02			= 58,				/* ベース２ */
	SOUNDID_DR_BASS05			= 59,				/* ベース５ */
	SOUNDID_DR_CLAP01			= 60,				/* クラップ１ */
	SOUNDID_DR_SNE02			= 61,				/* スネア２ */
	SOUNDID_KIE_ATACK			= 62,				/* キー攻撃 */
	SOUNDID_KIE_DEAD			= 63,				/* キー死亡 */
	SOUNDID_FLY					= 64,				/* 羽ばたき */
	SOUNDID_STEP03				= 65,				/* 階段移動 */
	SOUNDID_MAX
};


/* ========================================================================= */
/* 関数の定義																 */
/* ========================================================================= */

extern "C"{
SOUNDDATA_API int		GetSoundCount		(void);				/* 効果音数を取得 */
SOUNDDATA_API int		GetSoundResourceID	(int nNo);			/* 効果音のリソースIDを取得 */
SOUNDDATA_API DWORD		GetSoundID			(int nNo);			/* 効果音IDを取得 */
SOUNDDATA_API int		GetSoundNo			(DWORD dwSoundID);	/* 効果音のインデックスを取得 */
SOUNDDATA_API LPCSTR	GetSoundName		(DWORD dwSoundID);	/* 効果音名を取得 */
}

/* Copyright(C)URARA-works 2007 */
