/**
 * core/api.js
 * fetch ラッパ。app.js の fetchJson と同等のエラー整形・credentials 設定を ES Modules 化。
 *
 * レスポンス本文が JSON でない場合は { response, data: null, text } を返す。
 * HTTP エラー(4xx/5xx)は例外を投げず呼び出し元で response.ok を確認すること。
 *
 * 401 共通処理:
 *   setUnauthorizedHandler() で登録したフックが、401 応答を受けるたびに呼ばれる。
 *   実際の登録は app.js 側(ログイン画面の表示)で行う(main.js から動的 import)。
 *   各 view 個別の 401 分岐(role-management / account-create / image-editor 等)は
 *   このフックに一本化し、view 側では削除すること。
 *
 * 成功時トースト:
 *   options.toast に文字列(または { message, type })を渡すと、
 *   response.ok の時だけ components/toast.js で通知を出す。
 *   type 省略時は "success"。
 */

import { showToast } from "../components/toast.js";

const DEFAULT_HEADERS = {
  Accept: "application/json",
};

/** @type {(() => void)|null} */
let _unauthorizedHandler = null;

/**
 * 401 応答を受けた時に呼ぶ共通フックを登録する。
 * @param {() => void} handler
 */
export function setUnauthorizedHandler(handler) {
  _unauthorizedHandler = typeof handler === "function" ? handler : null;
}

function notifyUnauthorized() {
  if (!_unauthorizedHandler) {
    return;
  }
  try {
    _unauthorizedHandler();
  } catch (err) {
    console.error("[api] unauthorized handler failed:", err);
  }
}

function notifyToast(toastOption) {
  if (!toastOption) {
    return;
  }
  if (typeof toastOption === "string") {
    showToast(toastOption, "success");
    return;
  }
  showToast(toastOption.message, toastOption.type ?? "success", toastOption);
}

/**
 * JSON を取得する共通関数。
 * @param {string} url
 * @param {RequestInit & { toast?: string | { message: string, type?: "success"|"error"|"info" } }} [options]
 * @returns {Promise<{ response: Response, data: unknown, text: string }>}
 */
export async function fetchJson(url, options = {}) {
  const { toast, ...requestOptions } = options;
  const merged = {
    credentials: "include",
    ...requestOptions,
    headers: {
      ...DEFAULT_HEADERS,
      ...(requestOptions.headers ?? {}),
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
  if (response.status === 401) {
    notifyUnauthorized();
  } else if (response.ok) {
    notifyToast(toast);
  }
  return { response, data, text: rawText };
}

/**
 * JSON ボディを送信する共通関数(POST/PUT/PATCH など)。
 * @param {string} url
 * @param {unknown} body
 * @param {RequestInit & { toast?: string | { message: string, type?: "success"|"error"|"info" } }} [options]
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

/**
 * JSON ボディを PUT する共通関数。
 * @param {string} url
 * @param {unknown} body
 * @param {RequestInit & { toast?: string | { message: string, type?: "success"|"error"|"info" } }} [options]
 * @returns {Promise<{ response: Response, data: unknown, text: string }>}
 */
export async function putJson(url, body, options = {}) {
  return fetchJson(url, {
    method: "PUT",
    ...options,
    headers: {
      "Content-Type": "application/json",
      ...(options.headers ?? {}),
    },
    body: JSON.stringify(body),
  });
}

/**
 * DELETE リクエストを送る共通関数。body は省略可(渡した場合は JSON として送信)。
 * @param {string} url
 * @param {unknown} [body]
 * @param {RequestInit & { toast?: string | { message: string, type?: "success"|"error"|"info" } }} [options]
 * @returns {Promise<{ response: Response, data: unknown, text: string }>}
 */
export async function deleteJson(url, body, options = {}) {
  const hasBody = body !== undefined;
  return fetchJson(url, {
    method: "DELETE",
    ...options,
    headers: hasBody
      ? { "Content-Type": "application/json", ...(options.headers ?? {}) }
      : (options.headers ?? {}),
    body: hasBody ? JSON.stringify(body) : undefined,
  });
}
