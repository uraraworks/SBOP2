/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrData.h													 */
/* ���e			:�f�[�^�}�l�[�W���N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/11/04													 */
/* ========================================================================= */

#pragma once

class CMainFrame;
class CUraraSockTCPSBO;
class CTextOutput;
class CLibInfoAccount;
class CLibInfoCharSvr;
class CLibInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItemType;
class CLibInfoItem;
class CLibInfoItemWeapon;
class CLibInfoEffect;
class CLibInfoMotion;
class CLibInfoMotionType;
class CLibInfoEfcBalloon;
class CLibInfoSystem;
class CLibInfoSkill;
class CInfoFileList;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CMgrData
{
public:
			CMgrData();													/* �R���X�g���N�^ */
	virtual ~CMgrData();												/* �f�X�g���N�^ */

	void	Create				(CMainFrame *pMainFrame, CUraraSockTCPSBO *pSock);	/* �쐬 */
	void	Destroy				(void);												/* �j�� */

	void	Save				(void);											/* �f�[�^�̕ۑ� */
	void	Load				(void);											/* �f�[�^�̓ǂݍ��� */
	void	ReadHashList		(void);											/* �t�@�C�����X�g��ǂݍ��� */
	WORD	GetPort				(void)	{ return m_wPort;				}		/* �҂������|�[�g�ԍ����擾 */
	void	AddOnline			(void)	{ m_byOnline ++;				}		/* �I�����C������ǉ� */
	void	DecOnline			(void)	{ m_byOnline --;				}		/* �I�����C���������Z */
	BYTE	GetOnline			(void)	{ return m_byOnline;			}		/* �I�����C�������擾 */
	LPCSTR	GetAdminAccount		(void)	{ return m_strAdminAccount;		}		/* �Ǘ��Ҍ����A�J�E���g���擾 */
	LPCSTR	GetClientVersion	(void)	{ return m_strClientVersion;	}		/* �N���C�A���g�o�[�W�������擾 */
	void	SetClientVersion	(LPCSTR pszVersion);							/* �N���C�A���g�o�[�W������ݒ� */
	BYTE	GetLastSendClock	(void)	{ return m_byLastSendClock; 	}		/* �Ō�ɑ��M�����������擾 */
	void	SetLastSendClock	(BYTE byLastSendClock)	{ m_byLastSendClock = byLastSendClock; };	/* �Ō�ɑ��M����������ݒ� */
	LPCSTR	GetFtpAccount		(void)	{ return m_strFtpAccount;		}		/* FTP�A�J�E���g���擾 */
	LPCSTR	GetFtpPassword		(void)	{ return m_strFtpPassword;		}		/* FTP�p�X���[�h���擾 */
	LPCSTR	GetFtpServerAddr	(void)	{ return m_strFtpServerAddr;	}		/* �T�[�o�[�A�h���X���擾 */
	LPCSTR	GetFtpUploadPath	(void)	{ return m_strFtpUploadPath;	}		/* �A�b�v���[�h����擾 */

	CMainFrame			*GetMainFrame			(void)	{ return m_pMainFrame;		}	/* ���C���t���[�����擾 */
	CUraraSockTCPSBO	*GetSock				(void)	{ return m_pSock;			}	/* �ʐM�}�l�[�W�����擾 */

	CTextOutput			*GetLog					(void)	{ return m_pLog;				}	/* ���O�N���X���擾 */
	CLibInfoAccount		*GetLibInfoAccount		(void)	{ return m_pLibInfoAccount;		}	/* �A�J�E���g��񃉃C�u�������擾 */
	CLibInfoCharSvr		*GetLibInfoChar			(void)	{ return m_pLibInfoChar;		}	/* �L������񃉃C�u�������擾 */
	CLibInfoMapBase		*GetLibInfoMap			(void)	{ return m_pLibInfoMap;			}	/* �}�b�v��񃉃C�u�������擾 */
	CLibInfoMapObject	*GetLibInfoMapObject	(void)	{ return m_pLibInfoMapObject;	}	/* �}�b�v�I�u�W�F�N�g��񃉃C�u�������擾 */
	CLibInfoMapParts	*GetLibInfoMapParts		(void)	{ return m_pLibInfoMapParts;	}	/* �}�b�v�p�[�c��񃉃C�u�������擾 */
	CLibInfoMapShadow	*GetLibInfoMapShadow	(void)	{ return m_pLibInfoMapShadow;	}	/* �}�b�v�e��񃉃C�u�������擾 */
	CLibInfoItemType	*GetLibInfoItemType		(void)	{ return m_pLibInfoItemType;	}	/* �A�C�e����ʏ�񃉃C�u�������擾 */
	CLibInfoItem		*GetLibInfoItem			(void)	{ return m_pLibInfoItem;		}	/* �A�C�e����񃉃C�u�������擾 */
	CLibInfoItemWeapon	*GetLibInfoItemWeapon	(void)	{ return m_pLibInfoItemWeapon;	}	/* �����񃉃C�u�������擾 */
	CLibInfoEffect		*GetLibInfoEffect		(void)	{ return m_pLibInfoEffect;		}	/* �G�t�F�N�g��񃉃C�u�������擾 */
	CLibInfoMotion		*GetLibInfoMotion		(void)	{ return m_pLibInfoMotion;		}	/* ���[�V������񃉃C�u�������擾 */
	CLibInfoMotionType	*GetLibInfoMotionType	(void)	{ return m_pLibInfoMotionType;	}	/* ���[�V������ʏ�񃉃C�u�������擾 */
	CLibInfoEfcBalloon	*GetLibInfoEfcBalloon	(void)	{ return m_pLibInfoEfcBalloon;	}	/* ���o����񃉃C�u�������擾 */
	CLibInfoSystem		*GetLibInfoSystem		(void)	{ return m_pLibInfoSystem;		}	/* �V�X�e����񃉃C�u�������擾 */
	CLibInfoSkill		*GetLibInfoSkill		(void)	{ return m_pLibInfoSkill;		}	/* �X�L����񃉃C�u�������擾 */
	CInfoFileList		*GetInfoFileList		(void)	{ return m_pInfoFileList;		}	/* �t�@�C�����X�g�����擾 */


private:
	void	ReadIniData		(void);				/* ini�t�@�C������ݒ��ǂݍ��� */


private:
	WORD				m_wPort;				/* �҂������|�[�g */
	BYTE				m_byOnline,				/* �I�����C���� */
						m_byLastSendClock;		/* �Ō�ɑ��M�������� */
	CmyString			m_strAdminAccount,		/* �Ǘ��Ҍ����A�J�E���g�� */
						m_strClientVersion,		/* �N���C�A���g�o�[�W���� */
						m_strFtpAccount,		/* FTP�A�J�E���g */
						m_strFtpPassword,		/* FTP�p�X���[�h */
						m_strFtpServerAddr,		/* �T�[�o�[�A�h���X */
						m_strFtpUploadPath;		/* �A�b�v���[�h�� */

	CMainFrame			*m_pMainFrame;			/* ���C���t���[�� */
	CUraraSockTCPSBO	*m_pSock;				/* �ʐM�}�l�[�W�� */
	CTextOutput			*m_pLog;				/* ���O�t�@�C�� */

	CLibInfoAccount		*m_pLibInfoAccount;		/* �A�J�E���g��񃉃C�u���� */
	CLibInfoCharSvr		*m_pLibInfoChar;		/* �L������񃉃C�u���� */
	CLibInfoMapBase		*m_pLibInfoMap;			/* �}�b�v��񃉃C�u���� */
	CLibInfoMapObject	*m_pLibInfoMapObject;	/* �}�b�v�I�u�W�F�N�g��񃉃C�u���� */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* �}�b�v�p�[�c��񃉃C�u���� */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	/* �}�b�v�e��񃉃C�u���� */
	CLibInfoItemType	*m_pLibInfoItemType;	/* �A�C�e����ʏ�񃉃C�u���� */
	CLibInfoItem		*m_pLibInfoItem;		/* �A�C�e����񃉃C�u���� */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	/* �����񃉃C�u���� */
	CLibInfoEffect		*m_pLibInfoEffect;		/* �G�t�F�N�g��񃉃C�u���� */
	CLibInfoMotion		*m_pLibInfoMotion;		/* ���[�V������񃉃C�u���� */
	CLibInfoMotionType	*m_pLibInfoMotionType;	/* ���[�V������ʏ�񃉃C�u���� */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;	/* ���o����񃉃C�u���� */
	CLibInfoSystem		*m_pLibInfoSystem;		/* �V�X�e����񃉃C�u���� */
	CLibInfoSkill		*m_pLibInfoSkill;		/* �X�L����񃉃C�u���� */
	CInfoFileList		*m_pInfoFileList;		/* �t�@�C�����X�g��� */
} CMgrData, *PCMgrData;

/* Copyright(C)URARA-works 2006 */
