/// @file ProxyHeaderParser.h
/// @brief HTTPリクエストヘッダから X-Forwarded-For を取り出す純粋関数 定義ファイル
///
/// 本番がリバースプロキシ越しの場合、直接の接続元(WebSocketブリッジからは
/// loopback に見える)ではなく、プロキシが付与したヘッダに実クライアントIPが
/// 入っていることがある。ただしこのヘッダはプロキシを経由しない直接接続からは
/// 自由に偽装できてしまうため、呼び出し側で「直接の接続元が loopback である
/// 場合に限り」利用すること(ProxyIpRegistry::IsLoopbackIPv4 参照)。
/// この関数自体はその判断をせず、文字列を解析するだけの純粋関数にしてある。
///
/// [信頼モデル] 信頼する経路は「同一ホスト上のリバースプロキシ1段」だけという前提。
/// - X-Forwarded-For はプロキシがクライアントの申告値を消さずに末尾へ実IPを
///   付け足す仕様のため、右端(最後の要素)だけが1段先のプロキシが書いた値だと
///   保証できる。左側の要素はクライアントが自由に偽装できるので信用しない。
/// - X-Real-IP は IIS/ARR が付与しないヘッダーであり、クライアントが送った値が
///   そのまま素通しで届く。信用できる情報源ではないため一切参照しない。

#pragma once

#include <string>

namespace ProxyHeaderParser
{
    /// @brief HTTPリクエスト文字列(ヘッダ部を含む、"\r\n\r\n"まで受信済みのもの)から
    ///        実クライアントIPを取り出す。
    ///
    /// X-Forwarded-For ヘッダの右端(カンマ区切りの最後の要素、同名ヘッダーが
    /// 複数行ある場合は最後の行のもの)だけを採用する。信頼するプロキシは
    /// 直前の1段のみという前提のため、右端が IPv4 として不正な場合でも
    /// 左側の要素へはフォールバックしない。X-Real-IP は信用しないため参照しない。
    ///
    /// @param httpRequest 生のHTTPリクエスト文字列
    /// @param outIpNet    見つかった場合、ネットワークバイトオーダーのIPv4アドレスを格納
    /// @retval true  有効なIPv4アドレスが見つかった
    /// @retval false 見つからなかった(IPv6やホスト名など、この実装が非対応のものを含む)
    bool ExtractClientIp(const std::string &httpRequest, unsigned long &outIpNet);
}
