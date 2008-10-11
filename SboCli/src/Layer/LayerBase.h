/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:LayerBase.h												 */
/* ���e			:���C���[�`����N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

class CImg32;
class CMgrData;
class CMgrGrpData;
class CMgrDraw;
class CLibInfoCharCli;
class CLibInfoMapBase;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CLayerBase
{
public:
			CLayerBase();								/* �R���X�g���N�^ */
	virtual ~CLayerBase();								/* �f�X�g���N�^ */

	virtual void Create		(CMgrData *pMgrData);				/* �쐬 */
	virtual void Destroy	(void);								/* �j�� */

	virtual void Draw		(CImg32 *pDst);						/* �`�� */
	virtual BOOL TimerProc	(void);								/* ���ԏ��� */

	int		GetID		(void)	{ return m_nID; }				/* ID���擾 */


protected:
	void	TextOut1	(HDC hDC, int x, int y, LPCSTR pStr, COLORREF color);		/* �����`�� */
	void	TextOut2	(HDC hDC, int x, int y, LPCSTR pStr, COLORREF color);		/* ����蕶���`�� */


protected:
	int				m_nID;								/* ���C���[ID */
	HFONT			m_hFont;							/* �`��Ɏg���t�H���g */
	CImg32			*m_pDib,							/* �o�b�N�o�b�t�@ */
					*m_pDibBase;						/* �o�b�N�o�b�t�@(���T�C�Y) */
	CImg32			*m_pDibSystem;						/* �V�X�e���摜 */

	CMgrData		*m_pMgrData;						/* �f�[�^�Ǘ� */
	CMgrGrpData		*m_pMgrGrpData;						/* �O���t�B�b�N�f�[�^�Ǘ� */
	CMgrDraw		*m_pMgrDraw;						/* �`��Ǘ� */

	CLibInfoCharCli	*m_pLibInfoChar;					/* �L������񃉃C�u���� */
	CLibInfoMapBase	*m_pLibInfoMap;						/* �}�b�v��񃉃C�u���� */
} CLayerBase, *PCLayerBase;
typedef CmyArray<PCLayerBase, PCLayerBase>	  ARRAYLAYERBASE;
typedef CmyArray<PCLayerBase, PCLayerBase>	*PARRAYLAYERBASE;

/* Copyright(C)URARA-works 2006 */
