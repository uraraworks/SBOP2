/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名	:Packet.h													 */
/* 内容			:パケットクラス定義ファイル									 */
/* 作成			:年がら年中春うらら(URARA-works)							 */
/* 作成開始日	:2006/11/05													 */
/* ========================================================================= */

#pragma once

#include "VERSION/PacketVERSION_REQ_VERSIONCHECK.h"		/* バージョン系:バージョンチェック要求 */
#include "VERSION/PacketVERSION_RES_VERSIONCHECK.h"		/* 				バージョンチェック応答 */
#include "VERSION/PacketVERSION_REQ_FILELISTCHECK.h"	/* 				ファイルリストチェック要求 */
#include "VERSION/PacketVERSION_RES_FILELISTCHECK.h"	/* 				ファイルリストチェック応答 */
#include "VERSION/PacketVERSION_REQ_FILELIST.h"			/* 				ファイルリスト要求 */
#include "VERSION/PacketVERSION_RES_FILELIST.h"			/* 				ファイルリスト応答 */
#include "VERSION/PacketVERSION_REQ_FILE.h"				/*				ファイル要求 */
#include "VERSION/PacketVERSION_RES_FILE.h"				/*				ファイル応答 */
#include "CONNECT/PacketCONNECT_REQ_LOGIN.h"			/* 接続系:	ログイン要求 */
#include "CONNECT/PacketCONNECT_RES_LOGIN.h"			/* 			ログイン応答 */
#include "CONNECT/PacketCONNECT_REQ_PLAY.h"				/* 			ゲーム開始要求 */
#include "CONNECT/PacketCONNECT_RES_PLAY.h"				/* 			ゲーム開始応答 */
#include "CONNECT/PacketCONNECT_KEEPALIVE.h"			/* 			生存確認通知 */
#include "ACCOUNT/PacketACCOUNT_REQ_ACCOUNTINFO.h"		/* アカウント系:アカウント情報要求 */
#include "ACCOUNT/PacketACCOUNT_RES_ACCOUNTINFO.h"		/* 				アカウント情報応答 */
#include "ACCOUNT/PacketACCOUNT_REQ_MAKECHAR.h"			/* 				キャラ作成要求 */
#include "ACCOUNT/PacketACCOUNT_RES_MAKECHAR.h"			/* 				キャラ作成応答 */
#include "CHAR/PacketCHAR_REQ_CHARINFO.h"				/* キャラ系:キャラ情報要求 */
#include "CHAR/PacketCHAR_RES_CHARINFO.h"				/* 			キャラ情報応答 */
#include "CHAR/PacketCHAR_MOVEPOS.h"					/* 			移動通知 */
#include "CHAR/PacketCHAR_STATE.h"						/* 			状態通知 */
#include "CHAR/PacketCHAR_CHARINFO.h"					/* 			キャラ情報通知 */
#include "CHAR/PacketCHAR_REQ_CHAT.h"					/* 			チャット要求 */
#include "CHAR/PacketCHAR_CHAT.h"						/* 			チャット通知 */
#include "CHAR/PacketCHAR_CHARID.h"						/* 			キャラID通知 */
#include "CHAR/PacketCHAR_REQ_CHARINFO2.h"				/* 			キャラ情報要求(複数) */
#include "CHAR/PacketCHAR_REQ_PUTGET.h"					/* 			アイテムを拾う置く要求 */
#include "CHAR/PacketCHAR_RES_PUTGET.h"					/* 			アイテムを拾う置く応答 */
#include "CHAR/PacketCHAR_REQ_USEITEM.h"				/* 			アイテムを使う要求 */
#include "CHAR/PacketCHAR_REQ_DRAGITEM.h"				/* 			アイテム位置変更要求 */
#include "CHAR/PacketCHAR_SET_EFFECT.h"					/* 			エフェクト指定 */
#include "CHAR/PacketCHAR_ITEMINFO.h"					/* 			アイテム情報通知 */
#include "CHAR/PacketCHAR_REQ_PUSH.h"					/* 			押す要求 */
#include "CHAR/PacketCHAR_REQ_TAIL.h"					/* 			付いて行く要求 */
#include "CHAR/PacketCHAR_RES_TAIL.h"					/* 			付いて行く応答 */
#include "CHAR/PacketCHAR_REQ_MODIFY_PARAM.h"			/* 			パラメータ変更要求 */
#include "CHAR/PacketCHAR_MODIFY_PARAM.h"				/* 			パラメータ変更通知 */
#include "CHAR/PacketCHAR_REQ_EQUIP.h"					/* 			装備要求 */
#include "CHAR/PacketCHAR_GRP.h"						/* 			画像通知 */
#include "CHAR/PacketCHAR_MOTION.h"						/* 			モーション情報通知 */
#include "CHAR/PacketCHAR_MOTIONTYPE.h"					/* 			モーション種別情報通知 */
#include "CHAR/PacketCHAR_SET_EFCBALLOON.h"				/* 			噴出し指定 */
#include "CHAR/PacketCHAR_PARA1.h"						/* 			汎用[パラメータ1個] */
#include "CHAR/PacketCHAR_STATUS.h"						/* 			ステータス情報通知 */
#include "CHAR/PacketCHAR_TEXTEFFECT.h"					/* 			文字エフェクト通知 */
#include "CHAR/PacketCHAR_RES_TALKEVENT.h"				/* 			会話イベント情報応答 */
#include "MAP/PacketMAP_REQ_MAPINFO.h"					/* マップ系:マップ情報要求 */
#include "MAP/PacketMAP_RES_MAPINFO.h"					/* 			マップ情報応答 */
#include "MAP/PacketMAP_ONLINE.h"						/* 			オンライン数通知 */
#include "MAP/PacketMAP_SYSTEMMSG.h"					/* 			システムメッセージ通知 */
#include "MAP/PacketMAP_FORMATMSG.h"					/* 			フォーマットメッセージ通知 */
#include "MAP/PacketMAP_MAPOBJECT.h"					/* 			マップオブジェクト情報通知 */
#include "MAP/PacketMAP_MAPOBJECTDATA.h"				/* 			マップオブジェクト配置データ通知 */
#include "MAP/PacketMAP_MAPPARTS.h"						/* 			マップパーツ情報通知 */
#include "MAP/PacketMAP_SETPARTS.h"						/* 			マップパーツ配置 */
#include "MAP/PacketMAP_RENEWMAPSIZE.h"					/* 			マップサイズ更新 */
#include "MAP/PacketMAP_DELETEPARTS.h"					/* 			マップパーツ削除 */
#include "MAP/PacketMAP_MAPSHADOW.h"					/* 			マップ影情報通知 */
#include "MAP/PacketMAP_SETMAPSHADOW.h"					/* 			マップ影配置 */
#include "MAP/PacketMAP_DELETEMAPSHADOW.h"				/* 			マップ影削除 */
#include "MAP/PacketMAP_MAPNAME.h"						/* 			マップ名通知 */
#include "MAP/PacketMAP_MAPEVENT.h"						/* 			マップイベント情報通知 */
#include "MAP/PacketMAP_PARA1.h"						/* 			汎用[パラメータ1個] */
#include "ADMIN/PacketADMIN_CHARINFO.h"					/* 管理者系:キャラ情報通知 */
#include "ADMIN/PacketADMIN_MAP_RENEWMAPOBJECT.h"		/* 			マップオブジェクト更新 */
#include "ADMIN/PacketADMIN_MAP_RENEWOBJECTDATA.h"		/* 			マップオブジェクト配置データ更新 */
#include "ADMIN/PacketADMIN_RENEWMAPPARTS.h"			/* 			マップパーツ更新 */
#include "ADMIN/PacketADMIN_MAP_SETPARTS.h"				/* 			マップパーツ配置 */
#include "ADMIN/PacketADMIN_MAP_RENEWMAPSIZE.h"			/* 			マップサイズ更新 */
#include "ADMIN/PacketADMIN_MAP_DELETEPARTS.h"			/* 			マップパーツ削除 */
#include "ADMIN/PacketADMIN_MAP_COPYPARTS.h"			/* 			マップパーツコピー */
#include "ADMIN/PacketADMIN_MAP_SETMAPNAME.h"			/* 			マップ名変更 */
#include "ADMIN/PacketADMIN_RENEWMAPSHADOW.h"			/* 			マップ影更新 */
#include "ADMIN/PacketADMIN_MAP_SETMAPSHADOW.h"			/* 			マップ影配置 */
#include "ADMIN/PacketADMIN_MAP_DELETEMAPSHADOW.h"		/* 			マップ影削除 */
#include "ADMIN/PacketADMIN_MAP_EVENT.h"				/*			マップイベント情報更新 */
#include "ADMIN/PacketADMIN_SKILL_RENEWSKILL.h"			/*			スキル情報更新 */
#include "ADMIN/PacketADMIN_REQ_ADMINLEVEL.h"			/* 			管理者権限レベル要求 */
#include "ADMIN/PacketADMIN_RES_ADMINLEVEL.h"			/* 			管理者権限レベル応答 */
#include "ADMIN/PacketADMIN_RENEWADMINLEVEL.h"			/* 			管理者権限レベル更新 */
#include "ADMIN/PacketADMIN_SERVER_SAVEINFO.h"			/* 			サーバー情報保存 */
#include "ADMIN/PacketADMIN_ITEM_ADD.h"					/* 			アイテム情報追加 */
#include "ADMIN/PacketADMIN_ITEM_COPY.h"				/* 			アイテム情報コピー */
#include "ADMIN/PacketADMIN_ITEM_DELETE.h"				/* 			アイテム情報削除 */
#include "ADMIN/PacketADMIN_ITEMTYPE_ADD.h"				/* 			アイテム種別情報追加 */
#include "ADMIN/PacketADMIN_ITEMTYPE_COPY.h"			/* 			アイテム種別情報コピー */
#include "ADMIN/PacketADMIN_ITEMTYPE_DELETE.h"			/* 			アイテム種別情報削除 */
#include "ADMIN/PacketADMIN_ITEMWEAPON_ADD.h"			/* 			武器情報追加 */
#include "ADMIN/PacketADMIN_ITEMWEAPON_RENEW.h"			/* 			武器情報更新 */
#include "ADMIN/PacketADMIN_CHAR_ADDNPC.h"				/* 			NPCの追加 */
#include "ADMIN/PacketADMIN_CHAR_MODIFYITEM.h"			/* 			所持アイテムの変更 */
#include "ADMIN/PacketADMIN_CHAR_RENEWMOTION.h"			/* 			キャラモーション情報の更新 */
#include "ADMIN/PacketADMIN_CHAR_ADDMOTIONTYPE.h"		/* 			キャラモーション種別情報の追加 */
#include "ADMIN/PacketADMIN_CHAR_RENEWMOTIONTYPE.h"		/* 			キャラモーション種別情報の更新 */
#include "ADMIN/PacketADMIN_CHAR_RENEWSTATUS.h"			/* 			ステータス情報更新 */
#include "ADMIN/PacketADMIN_CHAR_RES_ONLINE.h"			/* 			オンライン中キャラ一覧応答 */
#include "ADMIN/PacketADMIN_EFC_RENEWBALLOON.h"			/* 			噴出し情報の更新 */
#include "ADMIN/PacketADMIN_EFC_RENEWEFFECT.h"			/* 			エフェクト情報の更新 */
#include "ADMIN/PacketADMIN_REQ_PLAYSOUND.h"			/* 			効果音の再生要求 */
#include "ADMIN/PacketADMIN_PLAYSOUND.h"				/* 			効果音の再生 */
#include "ADMIN/PacketADMIN_CHAR_REQ_ACCOUNT.h"			/*			アカウント情報要求 */
#include "ADMIN/PacketADMIN_CHAR_RES_ACCOUNT.h"			/*			アカウント情報応答 */
#include "ADMIN/PacketADMIN_CHAR_RENEW_ACCOUNT.h"		/*			アカウント情報更新 */
#include "ADMIN/PacketADMIN_CHAR_RENEW_TALKEVENT.h"		/* 			会話イベント情報更新 */
#include "ADMIN/PacketADMIN_RENEW_CLIENTVERSION.h"		/*			クライアントバージョン更新 */
#include "ADMIN/PacketADMIN_PARA2.h"					/* 			汎用[パラメータ2個] */
#include "ADMIN/PacketADMIN_SYSTEM_RENEWINFO.h"			/*			システム情報の更新 */
#include "MSGCMD/PacketMSGCMD_CHGFACE.h"				/* メッセージコマンド系:表情変更 */
#include "MSGCMD/PacketMSGCMD_CHGCLOTH.h"				/*						服装変更 */
#include "MSGCMD/PacketMSGCMD_CHGACCE.h"				/*						アクセサリ変更 */
#include "MSGCMD/PacketMSGCMD_CHGCOLOR.h"				/*						色変更 */
#include "MSGCMD/PacketMSGCMD_CHGARMS.h"				/*						持ち物変更 */
#include "MSGCMD/PacketMSGCMD_MAKEITEM.h"				/*						アイテム作成 */
#include "MSGCMD/PacketMSGCMD_PARA1.h"					/*						汎用[パラメータ1個] */
#include "ITEM/PacketITEM_REQ_ITEMINFO.h"				/* アイテム系:	アイテム情報要求 */
#include "ITEM/PacketITEM_RES_ITEMINFO.h"				/* 				アイテム情報応答 */
#include "ITEM/PacketITEM_ITEMINFO.h"					/* 				アイテム情報通知 */
#include "ITEM/PacketITEM_DELETEITEMINFO.h"				/* 				アイテム情報削除 */
#include "ITEM/PacketITEM_RENEWITEMINFO.h"				/* 				アイテム情報更新 */
#include "ITEM/PacketITEM_REQ_ITEMTYPEINFO.h"			/* 				アイテム種別情報要求 */
#include "ITEM/PacketITEM_RES_ITEMTYPEINFO.h"			/* 				アイテム種別情報応答 */
#include "ITEM/PacketITEM_ITEMTYPEINFO.h"				/* 				アイテム種別情報通知 */
#include "ITEM/PacketITEM_DELETEITEMTYPEINFO.h"			/* 				アイテム種別情報削除 */
#include "ITEM/PacketITEM_RENEWITEMTYPEINFO.h"			/* 				アイテム種別情報更新 */
#include "ITEM/PacketITEM_REQ_ITEMWEAPONINFO.h"			/* 				武器情報要求 */
#include "ITEM/PacketITEM_RES_ITEMWEAPONINFO.h"			/* 				武器情報応答 */
#include "ITEM/PacketITEM_ITEMWEAPONINFO.h"				/* 				武器情報通知 */
#include "EFFECT/PacketEFFECT_REQ_EFFECTINFO.h"			/* エフェクト系:エフェクト情報要求 */
#include "EFFECT/PacketEFFECT_RES_EFFECTINFO.h"			/* 				エフェクト情報応答 */
#include "EFFECT/PacketEFFECT_EFFECTINFO.h"				/* 				エフェクト情報通知 */
#include "EFFECT/PacketEFFECT_DELETEEFFECTINFO.h"		/* 				エフェクト情報削除 */
#include "EFFECT/PacketEFFECT_REQ_BALLOONINFO.h"		/* 				噴出し情報要求 */
#include "EFFECT/PacketEFFECT_BALLOONINFO.h"			/* 				噴出し情報通知 */
#include "EFFECT/PacketEFFECT_DELETEBALLOONINFO.h"		/* 				噴出し情報削除 */
#include "BATTLE/PacketBATTLE_REQ_ATACK.h"				/* 戦闘系:	攻撃要求 */
#include "SYSTEM/PacketSYSTEM_INFO.h"					/* システム系:	システム情報通知 */
#include "SKILL/PacketSKILL_PARA2.h"					/* スキル系:	汎用[パラメータ2個] */
#include "SKILL/PacketSKILL_SKILLINFO.h"				/* 				スキル情報通知 */

/* Copyright(C)URARA-works 2006 */
