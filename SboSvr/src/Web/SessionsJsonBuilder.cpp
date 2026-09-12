/// @file SessionsJsonBuilder.cpp
/// @brief 接続中プレイヤー一覧 JSON組み立て(純粋関数) 実装ファイル
/// @copyright Copyright(C)URARA-works
///
/// StdAfx.h(Windows/MFC由来の定義を含む) には依存しない。
/// SvrPlatform.h と JsonUtils.h だけを使い、SboSvrTest から単体でリンクできる
/// ようにするための制約(ProcessMetrics.cpp と同じ狙い)。

#include "SessionsJsonBuilder.h"

#include <cstdio>
#include <sstream>

#include "JsonUtils.h"
#include "../Platform/SvrPlatform.h"

namespace SessionsJsonBuilder
{
	std::string	Build(const std::vector<SboSessionRow> &rows,
			const std::string &strUpdatedAt, std::time_t tNowEpoch)
	{
		std::ostringstream oss;

		oss << "{\"updatedAt\":\"" << JsonUtils::Escape(strUpdatedAt) << "\",";
		oss << "\"count\":" << rows.size() << ',';
		oss << "\"sessions\":[";

		for (size_t i = 0; i < rows.size(); ++i) {
			const SboSessionRow &row = rows[i];
			if (i > 0) {
				oss << ',';
			}

			oss << "{";
			oss << "\"accountId\":" << row.nAccountId << ',';
			oss << "\"account\":\"" << JsonUtils::Escape(row.strAccount) << "\",";
			oss << "\"charId\":" << row.nCharId << ',';

			oss << "\"charName\":";
			if (row.bHasCharName) {
				oss << "\"" << JsonUtils::Escape(row.strCharName) << "\"";
			} else {
				oss << "null";
			}
			oss << ',';

			oss << "\"mapId\":";
			if (row.bHasMapId) {
				oss << row.nMapId;
			} else {
				oss << "null";
			}
			oss << ',';

			oss << "\"x\":";
			if (row.bHasPos) {
				oss << row.nX;
			} else {
				oss << "null";
			}
			oss << ',';

			oss << "\"y\":";
			if (row.bHasPos) {
				oss << row.nY;
			} else {
				oss << "null";
			}
			oss << ',';

			oss << "\"ip\":";
			if (row.bHasIp) {
				oss << "\"" << JsonUtils::Escape(row.strIp) << "\"";
			} else {
				oss << "null";
			}
			oss << ',';

			oss << "\"lastKeepaliveSec\":";
			if (row.bHasLastKeepalive) {
				long long nDiff = (long long)tNowEpoch - (long long)row.tLastKeepaliveEpoch;
				if (nDiff < 0) {
					nDiff = 0;
				}
				oss << nDiff;
			} else {
				oss << "null";
			}
			oss << ',';

			oss << "\"loginAt\":";
			if (row.bHasLoginAt) {
				oss << "\"" << FormatIso8601(row.tLoginAtEpoch) << "\"";
			} else {
				oss << "null";
			}

			oss << "}";
		}

		oss << "]}";
		return oss.str();
	}

	std::string	FormatIso8601(std::time_t tEpoch)
	{
		SboPlatform::GMTIME gmt;
		if (!SboPlatform::GmTimeUtc(tEpoch, &gmt)) {
			return "";
		}

		char szBuf[32];
		std::snprintf(szBuf, sizeof(szBuf), "%04d-%02d-%02dT%02d:%02d:%02dZ",
			gmt.nYear, gmt.nMonth, gmt.nDay, gmt.nHour, gmt.nMinute, gmt.nSecond);
		return szBuf;
	}

	void	ApplyLoginChars(std::vector<SboSessionRow> &rows,
			const std::vector<SboLoginCharRow> &loginChars)
	{
		for (size_t i = 0; i < rows.size(); ++i) {
			SboSessionRow &row = rows[i];

			const SboLoginCharRow *pMatch = NULL;

			// 1. セッションIDが一致するキャラを優先する
			for (size_t j = 0; j < loginChars.size(); ++j) {
				if (loginChars[j].nSessionId != 0 && loginChars[j].nSessionId == row.nSessionId) {
					pMatch = &loginChars[j];
					break;
				}
			}

			// 2. 見つからなければアカウントIDが一致するキャラで代用する
			if (pMatch == NULL) {
				for (size_t j = 0; j < loginChars.size(); ++j) {
					if (loginChars[j].nAccountId != 0 && loginChars[j].nAccountId == row.nAccountId) {
						pMatch = &loginChars[j];
						break;
					}
				}
			}

			// 一致するログイン中キャラが無い(キャラ未選択等)場合は行を変更しない
			if (pMatch == NULL) {
				continue;
			}

			row.nCharId = pMatch->nCharId;
			row.bHasCharName = true;
			row.strCharName = pMatch->strCharName;
			row.bHasMapId = true;
			row.nMapId = pMatch->nMapId;
			row.bHasPos = true;
			row.nX = pMatch->nX;
			row.nY = pMatch->nY;
		}
	}
}
