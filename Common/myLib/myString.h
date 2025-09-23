/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ファイル名   :myString.h
*/
/* 内容                 :文字列クラス 定義ファイル                                                                       */
/* 作成                 :年がら年中春うらら(URARA-works)                                                         */
/* 作成開始日   :2006/05/25
*/
/* ========================================================================= */

#pragma once

#include <atlstr.h>

/* ========================================================================= */
/* クラス宣言
*/
/* ========================================================================= */

typedef class CmyString
{
public:
        CmyString();                                                    /* コンストラクタ */
        CmyString(const CmyString &strSrc);
        CmyString(LPCSTR szSrc);
        CmyString(LPCTSTR szSrc);
        ~CmyString();                                                   /* デストラクタ */

        void    Empty                   (void);                                                 /* 文字列を空にする */
        BOOL    IsEmpty                 (void) const;                                  /* 文字列が空か判定 */
        int             GetLength               (void) const;                                  /* 文字列長を取得 */
        void    Format                  (LPCSTR lpFormat, ...);                 /* 書式文字列で初期化 */
        void    Format                  (LPCTSTR lpFormat, ...);            /* 書式文字列で初期化 */

        void    operator =              (const CmyString &strSrc);                      /* 文字列を初期化 */
        void    operator =              (LPCSTR pszSrc);                                /* 文字列を初期化 */
        void    operator =              (LPCTSTR pszSrc);                               /* 文字列を初期化 */
        void    operator +=             (LPCSTR pszSrc);                                /* 文字列を追加 */
        void    operator +=             (LPCTSTR pszSrc);                               /* 文字列を追加 */
        BOOL    operator ==             (LPCSTR pszSrc) const;                 /* 文字列を比較 */
        BOOL    operator ==             (LPCTSTR pszSrc) const;                /* 文字列を比較 */
        BOOL    operator !=             (LPCSTR pszSrc) const;                 /* 文字列を比較 */
        BOOL    operator !=             (LPCTSTR pszSrc) const;                /* 文字列を比較 */
                        operator LPCTSTR        () const;                                               /* キャスト */
                        operator LPCTSTR        ();                                                     /* キャスト */
                        operator LPCSTR         () const;                                               /* UTF-8キャスト */
                        operator LPCSTR         ();                                                     /* UTF-8キャスト */

        int             CompareNoCase   (LPCSTR pszSrc) const;                 /* 文字列比較(大文字小文字区別無し) */
        int             CompareNoCase   (LPCTSTR pszSrc) const;                /* 文字列比較(大文字小文字区別無し) */

protected:
        void    RenewUtf8               (LPCSTR pszSrc);                               /* UTF-8文字列を更新 */
        void    RenewWide               (LPCTSTR pszSrc);                              /* Unicode文字列を更新 */
        void    UpdateUtf8Cache         () const;                                      /* UTF-8キャッシュを更新 */

protected:
        CString         m_strString;                                          /* 文字列データ */
        mutable CStringA        m_strUtf8Cache;                        /* UTF-8キャッシュ */
        mutable BOOL            m_bUtf8Dirty;                          /* キャッシュ更新フラグ */
} CmyString, *PCmyString;

/* Copyright(C)URARA-works 2006 */
