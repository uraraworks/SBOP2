/**
 * core/api.js
 * fetch ラッパ。app.js の fetchJson と同等のエラー整形・credentials 設定を ES Modules 化。
 *
 * レスポンス本文が JSON でない場合は { response, data: null, text } を返す。
 * HTTP エラー(4xx/5xx)は例外を投げず呼び出し元で response.ok を確認すること。
 */

const DEFAULT_HEADERS = {
  Accept: "application/json",
};

/**
 * JSON を取得する共通関数。
 * @param {string} url
 * @param {RequestInit} [options]
 * @returns {Promise<{ response: Response, data: unknown, text: string }>}
 */
export async function fetchJson(url, options = {}) {
  const merged = {
    credentials: "include",
    ...options,
    headers: {
      ...DEFAULT_HEADERS,
      ...(options.headers ?? {}),
    },
  };
  const response = await fetch(url, merged);
  const rawText = await response.text();
  let data = null;
  if (rawText) {
    try {
      data = JSON.parse(rawText);
    } catch {
      data = null;
    }
  }
  return { response, data, text: rawText };
}

/**
 * JSON ボディを送信する共通関数(POST/PUT/PATCH など)。
 * @param {string} url
 * @param {unknown} body
 * @param {RequestInit} [options]
 * @returns {Promise<{ response: Response, data: unknown, text: string }>}
 */
export async function postJson(url, body, options = {}) {
  return fetchJson(url, {
    method: "POST",
    ...options,
    headers: {
      "Content-Type": "application/json",
      ...(options.headers ?? {}),
    },
    body: JSON.stringify(body),
  });
}
