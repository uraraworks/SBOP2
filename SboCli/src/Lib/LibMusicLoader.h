/* Copyright(C)URARA-works 2005 */
/* ========================================================================= */
/* �t�@�C�����F	LibMusicLoader.h											 */
/* ���e�F		AflMusic���C�u�����ǂݍ��݃N���X ��`�t�@�C��				 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2005/10/18													 */
/* ========================================================================= */

#pragma once

/* ========================================================================= */
/* �֐��|�C���^��`															 */
/* ========================================================================= */

typedef bool		(*LIBMUSICOpen1)(LPCSTR, LPCSTR);
typedef bool		(*LIBMUSICOpen2)(HMODULE, HRSRC, LPCSTR);
typedef bool		(*LIBMUSICClose)();
typedef bool		(*LIBMUSICFadeIn)(INT,INT);
typedef bool		(*LIBMUSICFadeOut)(INT);
typedef bool		(*LIBMUSICPlayTime)(DWORD);
typedef bool		(*LIBMUSICPlay1)(DWORDLONG);
typedef bool		(*LIBMUSICPlay2)();
typedef bool		(*LIBMUSICCont)();
typedef bool		(*LIBMUSICStop)();
typedef bool		(*LIBMUSICIsPlay)();
typedef void		(*LIBMUSICSetLoop)(bool);
typedef void		(*LIBMUSICSetRelativeVolume)(INT);
typedef LPCSTR		(*LIBMUSICGetTitle)();
typedef DWORDLONG	(*LIBMUSICGetAllTime)();
typedef DWORDLONG	(*LIBMUSICGetPlayTime)();


/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLibMusicLoader
{
public:
			CLibMusicLoader();					/* �R���X�g���N�^ */
	virtual	~CLibMusicLoader();					/* �f�X�g���N�^ */

	void	Load (void);						/* �ǂݍ��� */
	void	Free (void);						/* �J�� */

	BOOL		Open				(LPCSTR pFileName, LPCSTR pType = NULL);				/* �t�@�C������MIDI�f�[�^��ǂݍ��� */
	BOOL		Open				(HMODULE hResource, HRSRC hSrc, LPCSTR pType = NULL);	/* ���\�[�X����MIDI�f�[�^��ǂݍ��� */
	BOOL		Close				(void);													/* �f�[�^������������J�� */
	BOOL		FadeIn				(INT nTime, INT nVolume = -70);							/* �t�F�[�h�C�����s�� */
	BOOL		FadeOut				(INT nTime);											/* �t�F�[�h�A�E�g���s�� */
	BOOL		PlayTime			(DWORD dwTime);											/* �w�莞�Ԃ��炩�牉�t�J�n */
	BOOL		Play				(DWORDLONG dwlCount);									/* �w��ʒu���牉�t�J�n */
	BOOL		Play				(void);													/* ���t�J�n */
	BOOL		Cont				(void);													/* ���t�ĊJ */
	BOOL		Stop				(void);													/* ���t��~ */
	BOOL		IsPlay				(void);													/* ���t�������� */
	void		SetLoop				(BOOL bLoop);											/* ���[�v�����邩�w�� */
	void		SetRelativeVolume	(INT nVolume);											/* �{�����[���̐ݒ� */
	LPCSTR		GetTitle			(void);													/* �^�C�g�����擾 */
	DWORDLONG	GetAllTime			(void);													/* �����t���Ԃ��擾 */
	DWORDLONG	GetPlayTime			(void);													/* ���t���Ԃ��擾 */


protected:
	HMODULE				m_hLib;		/* DLL�̃��W���[���n���h�� */

	/* �ȉ��֐��|�C���^ */
	LIBMUSICOpen1				m_pOpen1;
	LIBMUSICOpen2				m_pOpen2;
	LIBMUSICClose				m_pClose;
	LIBMUSICFadeIn				m_pFadeIn;
	LIBMUSICFadeOut				m_pFadeOut;
	LIBMUSICPlayTime			m_pPlayTime;
	LIBMUSICPlay1				m_pPlay1;
	LIBMUSICPlay2				m_pPlay2;
	LIBMUSICCont				m_pCont;
	LIBMUSICStop				m_pStop;
	LIBMUSICIsPlay				m_pIsPlay;
	LIBMUSICSetLoop				m_pSetLoop;
	LIBMUSICSetRelativeVolume	m_pSetRelativeVolume;
	LIBMUSICGetTitle			m_pGetTitle;
	LIBMUSICGetAllTime			m_pGetAllTime;
	LIBMUSICGetPlayTime			m_pGetPlayTime;
} CLibMusicLoader, *PCLibMusicLoader;

/* Copyright(C)URARA-works 2005 */
