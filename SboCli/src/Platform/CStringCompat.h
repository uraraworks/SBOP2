/// @file CStringCompat.h
/// @brief 非Windows環境向け CStringTCompat テンプレートクラス
/// @date 2026/04/07

#pragma once

#if !defined(_WIN32)

// -----------------------------------------------------------------------
// 大文字小文字変換ヘルパー（char / wchar_t 特殊化）
// -----------------------------------------------------------------------
template<typename TChar>
inline TChar BrowserCompatToLower(TChar ch);

template<>
inline char BrowserCompatToLower(char ch)
{
	return static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
}

template<>
inline wchar_t BrowserCompatToLower(wchar_t ch)
{
	return static_cast<wchar_t>(std::towlower(ch));
}

// -----------------------------------------------------------------------
// CStringTCompat<TChar>: MFC CStringT の互換実装
// -----------------------------------------------------------------------
template<typename TChar>
class CStringTCompat
{
public:
	typedef std::basic_string<TChar> string_type;

	CStringTCompat()
		: m_data(), m_buffer()
	{
	}

	CStringTCompat(const TChar *pszSrc)
		: m_data((pszSrc != NULL) ? pszSrc : TChar()), m_buffer()
	{
	}

	CStringTCompat(const string_type &strSrc)
		: m_data(strSrc), m_buffer()
	{
	}

	CStringTCompat(const CStringTCompat &strSrc)
		: m_data(strSrc.m_data), m_buffer()
	{
	}

	CStringTCompat &operator =(const CStringTCompat &strSrc)
	{
		if (this != &strSrc) {
			m_data = strSrc.m_data;
			m_buffer.clear();
		}
		return *this;
	}

	CStringTCompat &operator =(const TChar *pszSrc)
	{
		m_data = (pszSrc != NULL) ? pszSrc : string_type();
		m_buffer.clear();
		return *this;
	}

	CStringTCompat &operator +=(const CStringTCompat &strSrc)
	{
		m_data += strSrc.m_data;
		return *this;
	}

	CStringTCompat &operator +=(const TChar *pszSrc)
	{
		if (pszSrc != NULL) {
			m_data += pszSrc;
		}
		return *this;
	}

	bool operator ==(const TChar *pszSrc) const
	{
		return Compare(pszSrc) == 0;
	}

	bool operator !=(const TChar *pszSrc) const
	{
		return !(*this == pszSrc);
	}

	operator const TChar *() const
	{
		return m_data.c_str();
	}

	operator TChar *()
	{
		return GetBuffer(GetLength());
	}

	const TChar *GetString() const
	{
		return m_data.c_str();
	}

	int GetLength() const
	{
		return static_cast<int>(m_data.length());
	}

	bool IsEmpty() const
	{
		return m_data.empty();
	}

	void Empty()
	{
		m_data.clear();
		m_buffer.clear();
	}

	TChar *GetBuffer(int nMinBufferLength)
	{
		if (nMinBufferLength < 0) {
			nMinBufferLength = GetLength();
		}
		const size_t nBufferLength = static_cast<size_t>(nMinBufferLength) + 1;
		m_buffer.assign(nBufferLength, 0);
		if (!m_data.empty()) {
			const size_t nCopyLength = std::min(m_data.length(), nBufferLength - 1);
			std::copy_n(m_data.c_str(), nCopyLength, m_buffer.begin());
			m_buffer[nCopyLength] = 0;
		}
		return m_buffer.data();
	}

	void ReleaseBuffer(int nNewLength = -1)
	{
		if (m_buffer.empty()) {
			return;
		}
		if (nNewLength < 0) {
			nNewLength = static_cast<int>(std::char_traits<TChar>::length(m_buffer.data()));
		}
		if (nNewLength < 0) {
			nNewLength = 0;
		}
		m_data.assign(m_buffer.data(), static_cast<size_t>(nNewLength));
		m_buffer.clear();
	}

	void AppendChar(TChar ch)
	{
		m_data.push_back(ch);
	}

	int Compare(const TChar *pszSrc) const
	{
		const string_type strSrc = (pszSrc != NULL) ? pszSrc : string_type();
		if (m_data == strSrc) {
			return 0;
		}
		return (m_data < strSrc) ? -1 : 1;
	}

	int Compare(const CStringTCompat &strSrc) const
	{
		return Compare(strSrc.GetString());
	}

	int CompareNoCase(const TChar *pszSrc) const
	{
		const string_type strSrc = (pszSrc != NULL) ? pszSrc : string_type();
		size_t nMin = std::min(m_data.length(), strSrc.length());
		for (size_t i = 0; i < nMin; ++i) {
			const TChar chLeft = BrowserCompatToLower(m_data[i]);
			const TChar chRight = BrowserCompatToLower(strSrc[i]);
			if (chLeft == chRight) {
				continue;
			}
			return (chLeft < chRight) ? -1 : 1;
		}
		if (m_data.length() == strSrc.length()) {
			return 0;
		}
		return (m_data.length() < strSrc.length()) ? -1 : 1;
	}

	int CompareNoCase(const CStringTCompat &strSrc) const
	{
		return CompareNoCase(strSrc.GetString());
	}

	void FormatV(const TChar *pszFormat, va_list args)
	{
		if (pszFormat == NULL) {
			Empty();
			return;
		}

		va_list argCopy;
		va_copy(argCopy, args);
		int nLength = BrowserCompatVsnprintf(NULL, 0, pszFormat, argCopy);
		va_end(argCopy);
		if (nLength < 0) {
			Empty();
			return;
		}

		std::vector<TChar> buffer(static_cast<size_t>(nLength) + 1, 0);
		BrowserCompatVsnprintf(buffer.data(), buffer.size(), pszFormat, args);
		m_data.assign(buffer.data(), static_cast<size_t>(nLength));
	}

	void Format(const TChar *pszFormat, ...)
	{
		va_list args;
		va_start(args, pszFormat);
		FormatV(pszFormat, args);
		va_end(args);
	}

	CStringTCompat Mid(int nFirst, int nCount = -1) const
	{
		if (nFirst < 0) {
			nFirst = 0;
		}
		if (static_cast<size_t>(nFirst) >= m_data.length()) {
			return CStringTCompat();
		}
		size_t nPos = static_cast<size_t>(nFirst);
		size_t nLength = m_data.length() - nPos;
		if (nCount >= 0) {
			nLength = std::min(nLength, static_cast<size_t>(nCount));
		}
		return CStringTCompat(m_data.substr(nPos, nLength));
	}

	CStringTCompat Left(int nCount) const
	{
		if (nCount <= 0) {
			return CStringTCompat();
		}
		return CStringTCompat(m_data.substr(0, std::min(m_data.length(), static_cast<size_t>(nCount))));
	}

	CStringTCompat Right(int nCount) const
	{
		if (nCount <= 0) {
			return CStringTCompat();
		}
		const size_t nLength = std::min(m_data.length(), static_cast<size_t>(nCount));
		return CStringTCompat(m_data.substr(m_data.length() - nLength, nLength));
	}

	int Find(TChar ch, int nStart = 0) const
	{
		if (nStart < 0) {
			nStart = 0;
		}
		size_t nPos = m_data.find(ch, static_cast<size_t>(nStart));
		return (nPos == string_type::npos) ? -1 : static_cast<int>(nPos);
	}

	int Find(const TChar *pszSub, int nStart = 0) const
	{
		if (pszSub == NULL) {
			return -1;
		}
		if (nStart < 0) {
			nStart = 0;
		}
		size_t nPos = m_data.find(pszSub, static_cast<size_t>(nStart));
		return (nPos == string_type::npos) ? -1 : static_cast<int>(nPos);
	}

	int ReverseFind(TChar ch) const
	{
		size_t nPos = m_data.rfind(ch);
		return (nPos == string_type::npos) ? -1 : static_cast<int>(nPos);
	}

	TChar operator [](int nIndex) const
	{
		return m_data[static_cast<size_t>(nIndex)];
	}

private:
	// char 版 vsnprintf ラッパー
	static int BrowserCompatVsnprintf(char *pszDst, size_t nDstCount, const char *pszFormat, va_list args)
	{
		va_list argCopy;
		va_copy(argCopy, args);
		int nRet = vsnprintf(pszDst, nDstCount, pszFormat, argCopy);
		va_end(argCopy);
		return nRet;
	}

	// wchar_t 版 vswprintf ラッパー
	static int BrowserCompatVsnprintf(wchar_t *pszDst, size_t nDstCount, const wchar_t *pszFormat, va_list args)
	{
		va_list argCopy;
		va_copy(argCopy, args);
		int nRet = vswprintf(pszDst, nDstCount, pszFormat, argCopy);
		va_end(argCopy);
		return nRet;
	}

private:
	string_type m_data;
	std::vector<TChar> m_buffer;
};

// CString は wchar_t 版、CStringA は char 版
typedef CStringTCompat<wchar_t> CString;
typedef CStringTCompat<char> CStringA;

#endif // !_WIN32
