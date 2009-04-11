/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* �t�@�C����	:MgrData.h													 */
/* ���e			:�f�[�^�}�l�[�W���N���X ��`�t�@�C��						 */
/* �쐬			:�N����N���t�����(URARA-works)							 */
/* �쐬�J�n��	:2006/09/24													 */
/* ========================================================================= */

#pragma once

#include "DataSlot.h"

class CMainFrame;
class CUraraSockTCPSBO;
class CMgrSound;
class CMgrGrpData;
class CMgrDraw;
class CMgrLayer;
class CMgrWindow;
class CMgrKeyInput;
class CInfoCharCli;
class CInfoMapBase;
class CInfoAccount;
class CInfoTalkEvent;
class CLibInfoCharCli;
class CLibInfoDisable;
class CLibInfoMapBase;
class CLibInfoMapObject;
class CLibInfoMapParts;
class CLibInfoMapShadow;
class CLibInfoItem;
class CLibInfoItemType;
class CLibInfoItemWeapon;
class CLibInfoEffect;
class CLibInfoMotion;
class CLibInfoMotionType;
class CLibInfoEfcBalloon;
class CLibInfoSystem;
class CLibInfoSkill;

/* ========================================================================= */
/* �N���X�錾																 */
/* ========================================================================= */

typedef class CMgrData : public CDataSlot
{
public:
			CMgrData();											/* �R���X�g���N�^ */
	virtual ~CMgrData();										/* �f�X�g���N�^ */

	void			Create				(CMainFrame *m_pMainFrame, CMgrGrpData *pMgrGrpData);	/* �쐬 */
	void			Destroy				(void);													/* �j�� */
	void			SaveIniData			(void);													/* �ݒ����ۑ� */

	void			SetWindowInfo		(HINSTANCE hInstance, HWND hWndMain);		/* �E�B���h�E����ݒ� */
	HINSTANCE		GetInstance			(void)	{ return m_hInstance;		}		/* �C���X�^���X�n���h�����擾 */
	HWND			GetMainWindow		(void)	{ return m_hWndMain;		}		/* ���C���E�B���h�E�n���h�����擾 */

	void				SetUraraSockTCP		(CUraraSockTCPSBO *pSock)	{ m_pSock = pSock;	}	/* �ʐM�}�l�[�W����ݒ� */
	CUraraSockTCPSBO	*GetUraraSockTCP	(void)						{ return m_pSock;	}	/* �ʐM�}�l�[�W�����擾 */

	void			SetAdminWindow		(HWND hWndAdmin)	{ m_hWndAdmin = hWndAdmin;		}	/* �Ǘ��҃E�B���h�E��ݒ� */
	HWND			GetAdminWindow		(void)				{ return m_hWndAdmin;			}	/* �Ǘ��҃E�B���h�E���擾 */
	void			SetAdminLevel		(int nAdminLevel)	{ m_nAdminLevel = nAdminLevel;	}	/* �Ǘ��Ҍ������x����ݒ� */
	int				GetAdminLevel		(void)				{ return m_nAdminLevel;			}	/* �Ǘ��Ҍ������x�����擾 */
	void			SetDrawMode			(int nDrawMode)		{ m_nDrawMode = nDrawMode;		}	/* �`�惂�[�h��ݒ� */
	int				GetDrawMode			(void)				{ return m_nDrawMode;			}	/* �`�惂�[�h���擾 */
	void			SetSetItemID		(DWORD dwItemID)	{ m_dwSetItemID = dwItemID;		}	/* �z�u����A�C�e��ID��ݒ� */
	DWORD			GetSetItemID		(void)				{ return m_dwSetItemID;			}	/* �z�u����A�C�e��ID���擾 */
	void			SetChatModeBack		(int nChatMode)		{ m_nChatModeBack = nChatMode;	}	/* �O��̃`���b�g��ʂ�ݒ� */
	int				GetChatModeBack		(void)				{ return m_nChatModeBack;		}	/* �O��̃`���b�g��ʂ��擾 */

	void			SetAdminNotifyTypeL		(int nType)		{ m_nAdminNotifyTypeL = nType;	}	/* �Ǘ��҃E�B���h�E�ւ̒ʒm���(���N���b�N)��ݒ� */
	int				GetAdminNotifyTypeL		(void)			{ return m_nAdminNotifyTypeL;	}	/* �Ǘ��҃E�B���h�E�ւ̒ʒm���(���N���b�N)���擾 */
	void			SetAdminNotifyTypeR		(int nType)		{ m_nAdminNotifyTypeR = nType;	}	/* �Ǘ��҃E�B���h�E�ւ̒ʒm���(�E�N���b�N)��ݒ� */
	int				GetAdminNotifyTypeR		(void)			{ return m_nAdminNotifyTypeR;	}	/* �Ǘ��҃E�B���h�E�ւ̒ʒm���(�E�N���b�N)���擾 */
	void			SetAdminNotifyTypeRR	(int nType)		{ m_nAdminNotifyTypeRR = nType;	}	/* �Ǘ��҃E�B���h�E�ւ̒ʒm���(�E�_�u���N���b�N)��ݒ� */
	int				GetAdminNotifyTypeRR	(void)			{ return m_nAdminNotifyTypeRR;	}	/* �Ǘ��҃E�B���h�E�ւ̒ʒm���(�E�_�u���N���b�N)���擾 */

	void			SetLastAccount		(LPCSTR pszAccount)		{ m_strLastAccount = pszAccount;	}	/* �ŏI�A�J�E���g����ݒ� */
	LPCSTR			GetLastAccount		(void)					{ return m_strLastAccount;			}	/* �ŏI�A�J�E���g�����擾 */
	void			SetLastPassword		(LPCSTR pszPassword)	{ m_strLastPassword = pszPassword;	}	/* �ŏI�p�X���[�h��ݒ� */
	LPCSTR			GetLastPassword		(void)					{ return m_strLastPassword;			}	/* �ŏI�p�X���[�h���擾 */

	void			SetEditMapPile			(BOOL bEditMapPile)		{ m_bEditMapPile = bEditMapPile;			}	/* �d�ˍ��킹�p�Ƃ��ă}�b�v�ҏW��ݒ� */
	BOOL			GetEditMapPile			(void)					{ return m_bEditMapPile;					}	/* �d�ˍ��킹�p�Ƃ��ă}�b�v�ҏW���擾 */
	void			SetMoveNoBlock			(BOOL bMoveNoBlock)		{ m_bMoveNoBlock = bMoveNoBlock;			}	/* �����蔻�薳����ݒ� */
	BOOL			GetMoveNoBlock			(void)					{ return m_bMoveNoBlock;					}	/* �����蔻�薳�����擾 */
	void			SetSavePassword			(BOOL bSavePassword)	{ m_bSavePassword = bSavePassword;			}	/* �p�X���[�h��ۑ������ݒ� */
	BOOL			GetSavePassword			(void)					{ return m_bSavePassword;					}	/* �p�X���[�h��ۑ�������擾 */
	void			SetDisableLogin			(BOOL bDisableLogin)	{ m_bDisableLogin = bDisableLogin;			}	/* ���O�C�����ۂ�ݒ� */
	BOOL			GetDisableLogin			(void)					{ return m_bDisableLogin;					}	/* ���O�C�����ۂ��擾 */
	void			SetSelectMapPartsID		(DWORD dwMapPartsID)	{ m_dwSelectMapPartsID = dwMapPartsID;		}	/* �I�𒆂̃}�b�v�p�[�cID��ݒ� */
	DWORD			GetSelectMapPartsID		(void)					{ return m_dwSelectMapPartsID;				}	/* �I�𒆂̃}�b�v�p�[�cID���擾 */
	void			SetSelectMapShadowID	(DWORD dwMapShadowID)	{ m_dwSelectMapShadowID = dwMapShadowID;	}	/* �I�𒆂̃}�b�v�eID��ݒ� */
	DWORD			GetSelectMapShadowID	(void)					{ return m_dwSelectMapShadowID;				}	/* �I�𒆂̃}�b�v�eID���擾 */
	void			SetViewGrid				(BYTE byViewGrid)		{ m_byViewGrid = byViewGrid;				}	/* �O���b�h�\����ݒ� */
	BYTE			GetViewGrid				(void)					{ return m_byViewGrid;						}	/* �O���b�h�\�����擾 */
	void			SetOptionTaskbar		(BOOL bOn)				{ m_bOptionTaskbar = bOn;					}	/* �������Ƀ^�X�N�o�[�`�J�`�J��ݒ� */
	BOOL			GetOptionTaskbar		(void)					{ return m_bOptionTaskbar;					}	/* �������Ƀ^�X�N�o�[�`�J�`�J���擾 */
	void			SetOptionViewChat		(BOOL bOn)				{ m_bOptionViewChat = bOn;					}	/* ������\�������ݒ� */
	BOOL			GetOptionViewChat		(void)					{ return m_bOptionViewChat;					}	/* ������\��������擾 */
	void			SetOptionViewItem		(BOOL bOn)				{ m_bOptionViewItem = bOn;					}	/* �A�C�e����\�������ݒ� */
	BOOL			GetOptionViewItem		(void)					{ return m_bOptionViewItem;					}	/* �A�C�e����\��������擾 */
	void			SetOptionViewItemName	(BOOL bOn)				{ m_bOptionViewItemName = bOn;				}	/* �A�C�e������\�������ݒ� */
	BOOL			GetOptionViewItemName	(void)					{ return m_bOptionViewItemName;				}	/* �A�C�e������\��������擾 */
	void			SetOptionViewHelpIcon	(BOOL bOn)				{ m_bOptionViewHelpIcon = bOn;				}	/* �w���v�A�C�R����\�����Ȃ���ݒ� */
	BOOL			GetOptionViewHelpIcon	(void)					{ return m_bOptionViewHelpIcon;				}	/* �w���v�A�C�R����\�����Ȃ����擾 */
	void			SetOptionBattleMsgLog	(BOOL bOn)				{ m_bOptionBattleMsgLog = bOn;				}	/* �퓬���b�Z�[�W�����O�Ɏc����ݒ� */
	BOOL			GetOptionBattleMsgLog	(void)					{ return m_bOptionBattleMsgLog;				}	/* �퓬���b�Z�[�W�����O�Ɏc�����擾 */
	void			SetOption60Frame		(BOOL bOn)				{ m_bOption60Frame = bOn;					}	/* 60�t���[���ŕ\�������ݒ� */
	BOOL			GetOption60Frame		(void)					{ return m_bOption60Frame;					}	/* 60�t���[���ŕ\��������擾 */

	LPCSTR			GetServerAddr		(void)	{ return m_strServerAddr;	}		/* �T�[�o�[�A�h���X���擾 */
	WORD			GetServerPort		(void)	{ return m_wServerPort;		}		/* �T�[�o�[�|�[�g�ԍ����擾 */

	CInfoCharCli	*GetPlayerChar		(void)	{ return m_pPlayerChar;		}						/* ���쒆�̃L���������擾 */
	void			SetPlayerChar		(CInfoCharCli *pInfoChar)	{ m_pPlayerChar = pInfoChar; }	/* ���쒆�̃L��������ݒ� */
	CInfoMapBase	*GetMap				(void)	{ return m_pMap;			}		/* �\�����̃}�b�v�����擾 */
	CInfoAccount	*GetAccount			(void)	{ return m_pInfoAccount;	}		/* �g�p���̃A�J�E���g�����擾 */
	void			SetMap				(CInfoMapBase *pMap)	{ m_pMap = pMap; }	/* �\�����̃}�b�v����ݒ� */
	void			SetAccount			(CInfoAccount *pInfoAccount);				/* �g�p���̃A�J�E���g����ݒ� */
	DWORD			GetAccountID		(void);										/* �g�p���̃A�J�E���gID���擾 */
	void			SetCharID			(DWORD dwCharID);							/* �g�p���̃L����ID��ݒ� */
	DWORD			GetCharID			(void);										/* �g�p���̃L����ID���擾 */
	DWORD			GetMapID			(void);										/* ���݂̃}�b�vID���擾 */
	LPCSTR			GetFamilyTypeName	(WORD wFamilyTypeID);						/* �푰�����擾 */
	LPCSTR			GetHairTypeName		(WORD wHairTypeID);							/* ���^�����擾 */
	LPCSTR			GetHairColorName	(WORD wHairColorID);						/* ���F�����擾 */
	LPCSTR			GetEyeColorName		(WORD wEyeColorID);							/* �ڐF�����擾 */
	LPCSTR			GetMotionName		(DWORD dwMotionID);							/* ���[�V���������擾 */
	LPCSTR			GetMotionProcName	(DWORD dwProcID);							/* �s�������擾 */
	LPCSTR			GetMapEventName		(int nMapEventType);						/* �}�b�v�C�x���g��ʖ����擾 */

	int				GetSystemMsgCount	(void);											/* �V�X�e�����b�Z�[�W�����擾 */
	PSTSYSTEMMSG	GetSystemMsg		(int nNo);										/* �V�X�e�����b�Z�[�W��ǉ� */
	void			AddSystemMsg		(BOOL bAddLog, LPCSTR pszMsg, COLORREF clMsg);	/* �V�X�e�����b�Z�[�W���擾 */
	void			DeleteAllSystemMsg	(void);											/* �V�X�e�����b�Z�[�W��S�č폜 */

	void			SetOnlineCount		(int nCount)	{ m_nOnlineCount = nCount;	}	/* �I�����C������ݒ� */
	int				GetOnlineCount		(void)			{ return m_nOnlineCount;	}	/* �I�����C�������擾 */
	void			SetCharCount		(int nCount)	{ m_nCharCount = nCount;	}	/* �L��������ݒ� */
	int				GetCharCount		(void)			{ return m_nCharCount;		}	/* �L���������擾 */
	void			SetPing				(int nPing)		{ m_nPing = nPing;			}	/* �T�[�o�[�Ƃ̃p�P�b�g�������Ԃ�ݒ� */
	int				GetPing				(void)			{ return m_nPing;			}	/* �T�[�o�[�Ƃ̃p�P�b�g�������Ԃ��擾 */
	int				GetSEVolume			(void)			{ return m_nSEVolume;		}	/* ���ʉ��ʂ��擾 */
	void			SetSEVolume			(int nSEVolume);								/* ���ʉ��ʂ�ݒ� */
	int				GetBGMVolume		(void)			{ return m_nBGMVolume;		}	/* BGM���ʂ��擾 */
	void			SetBGMVolume		(int nBGMVolume);								/* BGM���ʂ�ݒ� */
	GUID			GetInputGuid		(void)			{ return m_stInputGuid;		}	/* ���̓p�b�h��GUID���擾 */
	void			SetInputGuid		(GUID stGuid)	{ m_stInputGuid = stGuid;	}	/* ���̓p�b�h��GUID��ݒ� */
	int				GetSleepTimer		(void)			{ return m_nSleepTimer;		}	/* ���Ђ�˃^�C�}�[���擾 */
	void			SetSleepTimer		(int nSleepTimer) { m_nSleepTimer = nSleepTimer; }/* ���Ђ�˃^�C�}�[��ݒ� */
	SIZE			GetDlgMapPartsEdit	(void)			{ return m_sizeDlgMapPartsEdit;	}								/* �}�b�v�p�[�c�ҏW�_�C�A���O�T�C�Y���擾 */
	void			SetDlgMapPartsEdit	(SIZE sizeDlgMapPartsEdit)	{ m_sizeDlgMapPartsEdit = sizeDlgMapPartsEdit; }	/* �}�b�v�p�[�c�ҏW�_�C�A���O�T�C�Y��ݒ� */
	SIZE			GetWndMap			(void)						{ return m_sizeWndMap;	}							/* �}�b�v�E�B���h�E�T�C�Y���擾 */
	void			SetWndMap			(SIZE sizeWndMap)			{ m_sizeWndMap = sizeWndMap; }						/* �}�b�v�E�B���h�E�T�C�Y��ݒ� */
	int				GetLastSelectGrpNo	(void)						{ return m_nLastSelectGrpNo;	}					/* �Ō�ɑI�񂾉摜�ԍ����擾 */
	void			SetLastSelectGrpNo	(int nLastSelectGrpNo)		{ m_nLastSelectGrpNo = nLastSelectGrpNo; }			/* �Ō�ɑI�񂾉摜�ԍ���ݒ� */
	void			SetInfoTalkEvent	(CInfoTalkEvent *pSrc);						/* ��b�C�x���g���̐ݒ� */
	CInfoTalkEvent	*GetInfoTalkEvent	(void);										/* ��b�C�x���g���̎擾 */

	CMainFrame		*GetMainFrame		(void)	{ return m_pMainFrame;		}		/* ���C���t���[�����擾 */
	CMgrSound		*GetMgrSound		(void)	{ return m_pMgrSound;		}		/* �T�E���h�Ǘ����擾 */
	CMgrGrpData 	*GetMgrGrpData		(void)	{ return m_pMgrGrpData;		}		/* �O���t�B�b�N�f�[�^�Ǘ����擾 */
	CMgrDraw		*GetMgrDraw			(void)	{ return m_pMgrDraw;		}		/* �`��Ǘ����擾 */
	CMgrLayer		*GetMgrLayer		(void)	{ return m_pMgrLayer;		}		/* ���C���[�Ǘ����擾 */
	CMgrWindow		*GetMgrWindow		(void)	{ return m_pMgrWindow;		}		/* �E�B���h�E�Ǘ����擾 */
	CMgrKeyInput	*GetMgrKeyInput		(void)	{ return m_pMgrKeyInput;	}		/* �L�[���͊Ǘ����擾 */

	CLibInfoCharCli		*GetLibInfoChar			(void)	{ return m_pLibInfoChar;		}	/* �L������񃉃C�u�������擾 */
	CLibInfoDisable		*GetLibInfoDisable		(void)	{ return m_pLibInfoDisable;		}	/* ���ۏ�񃉃C�u�������擾 */
	CLibInfoMapBase		*GetLibInfoMap			(void)	{ return m_pLibInfoMap;			}	/* �}�b�v��񃉃C�u�������擾 */
	CLibInfoMapObject	*GetLibInfoMapObject	(void)	{ return m_pLibInfoMapObject;	}	/* �}�b�v�I�u�W�F�N�g��񃉃C�u�������擾 */
	CLibInfoMapParts	*GetLibInfoMapParts		(void)	{ return m_pLibInfoMapParts;	}	/* �}�b�v�p�[�c��񃉃C�u�������擾 */
	CLibInfoMapShadow	*GetLibInfoMapShadow	(void)	{ return m_pLibInfoMapShadow;	}	/* �}�b�v�e��񃉃C�u�������擾 */
	CLibInfoItem		*GetLibInfoItem			(void)	{ return m_pLibInfoItem;		}	/* �A�C�e����񃉃C�u�������擾 */
	CLibInfoItemType	*GetLibInfoItemType		(void)	{ return m_pLibInfoItemType;	}	/* �A�C�e����ʏ�񃉃C�u�������擾 */
	CLibInfoItemWeapon	*GetLibInfoItemWeapon	(void)	{ return m_pLibInfoItemWeapon;	}	/* �����񃉃C�u�������擾 */
	CLibInfoEffect		*GetLibInfoEffect		(void)	{ return m_pLibInfoEffect;		}	/* �G�t�F�N�g��񃉃C�u�������擾 */
	CLibInfoMotion		*GetLibInfoMotion		(void)	{ return m_pLibInfoMotion;		}	/* ���[�V������񃉃C�u�������擾 */
	CLibInfoMotionType	*GetLibInfoMotionType	(void)	{ return m_pLibInfoMotionType;	}	/* ���[�V������ʏ�񃉃C�u�������擾 */
	CLibInfoEfcBalloon	*GetLibInfoEfcBalloon	(void)	{ return m_pLibInfoEfcBalloon;	}	/* ���o����񃉃C�u�������擾 */
	CLibInfoSystem		*GetLibInfoSystem		(void)	{ return m_pLibInfoSystem;		}	/* �V�X�e����񃉃C�u�������擾 */
	CLibInfoSkill		*GetLibInfoSkill		(void)	{ return m_pLibInfoSkill;		}	/* �X�L����񃉃C�u�������擾 */

	CLibInfoCharCli		*GetLibInfoCharOnline	(void)	{ return m_pLibInfoCharOnline;	}	/* ���O�C�����L������񃉃C�u�������擾 */


private:
	void	ReadIniData	(void);						/* ini�t�@�C������ݒ��ǂݍ��� */


private:
	HINSTANCE			m_hInstance;				/* �C���X�^���X�n���h�� */
	HWND				m_hWndMain,					/* ���C���E�B���h�E�n���h�� */
						m_hWndAdmin;				/* �Ǘ��҃E�B���h�E�̃E�B���h�E�n���h�� */
	int					m_nOnlineCount,				/* �I�����C���� */
						m_nCharCount,				/* �����L������ */
						m_nPing,					/* �T�[�o�[�Ƃ̃p�P�b�g�������� */
						m_nAdminLevel,				/* �Ǘ��Ҍ������x�� */
						m_nDrawMode,				/* �`�惂�[�h */
						m_nChatModeBack,			/* �O��̃`���b�g��� */
						m_nAdminNotifyTypeL,		/* �Ǘ��҃E�B���h�E�ւ̒ʒm���(���N���b�N) */
						m_nAdminNotifyTypeR,		/* �Ǘ��҃E�B���h�E�ւ̒ʒm���(�E�N���b�N) */
						m_nAdminNotifyTypeRR,		/* �Ǘ��҃E�B���h�E�ւ̒ʒm���(�E�_�u���N���b�N) */
						m_nSEVolume,				/* ���ʉ��� */
						m_nBGMVolume,				/* BGM���� */
						m_nSleepTimer;				/* ���Ђ�˃^�C�}�[ */
	BOOL				m_bEditMapPile,				/* �d�ˍ��킹�p�Ƃ��ă}�b�v�ҏW */
						m_bMoveNoBlock,				/* �����蔻�薳�� */
						m_bSavePassword,			/* �p�X���[�h��ۑ�����H */
						m_bDisableLogin,			/* ���O�C������ */
						m_bOptionTaskbar,			/* �������Ƀ^�X�N�o�[���`�J�`�J */
						m_bOptionViewChat,			/* ������\������ */
						m_bOptionViewItem,			/* �A�C�e����\������ */
						m_bOptionViewItemName,		/* �A�C�e������\������ */
						m_bOptionViewHelpIcon,		/* �w���v�A�C�R����\������ */
						m_bOptionBattleMsgLog,		/* �퓬���b�Z�[�W�����O�Ɏc�� */
						m_bOption60Frame;			/* 60�t���[���ŕ\������ */
	BYTE				m_byViewGrid;				/* �O���b�h�\�� */
	GUID				m_stInputGuid;				/* ���̓p�b�h��GUID */
	SIZE				m_sizeDlgMapPartsEdit,		/* �}�b�v�p�[�c�ҏW�_�C�A���O�T�C�Y */
						m_sizeWndMap;				/* �}�b�v�E�B���h�E�T�C�Y */
	int					m_nLastSelectGrpNo;			/* �Ō�ɑI�񂾉摜�ԍ� */


	CmyString			m_strServerAddr,			/* �T�[�o�[�A�h���X */
						m_strLastAccount,			/* �Ō�Ɏg��ꂽ�A�J�E���g�� */
						m_strLastPassword;			/* �Ō�Ɏg��ꂽ�p�X���[�h */
	WORD				m_wServerPort;				/* �T�[�o�[�|�[�g�ԍ� */
	DWORD				m_dwSelectMapPartsID,		/* �I�𒆂̃}�b�v�p�[�cID(�}�b�v�ҏW�p) */
						m_dwSelectMapShadowID,		/* �I�𒆂̃}�b�v�eID(�}�b�v�ҏW�p) */
						m_dwSetItemID;				/* �z�u����A�C�e��ID */

	CInfoCharCli		*m_pPlayerChar;				/* ���쒆�̃L������� */
	CInfoMapBase		*m_pMap;					/* �\�����̃}�b�v��� */
	CInfoAccount		*m_pInfoAccount;			/* �g�p���̃A�J�E���g��� */
	CInfoTalkEvent		*m_pInfoTalkEvent;			/* ��b�C�x���g��� */

	CMainFrame			*m_pMainFrame;				/* ���C���t���[�� */
	CUraraSockTCPSBO	*m_pSock;					/* �ʐM�}�l�[�W�� */
	CMgrSound			*m_pMgrSound;				/* �T�E���h�Ǘ� */
	CMgrGrpData			*m_pMgrGrpData;				/* �O���t�B�b�N�f�[�^�Ǘ� */
	CMgrDraw			*m_pMgrDraw;				/* �`��Ǘ� */
	CMgrLayer			*m_pMgrLayer;				/* ���C���[�Ǘ� */
	CMgrWindow			*m_pMgrWindow;				/* �E�B���h�E�Ǘ� */
	CMgrKeyInput		*m_pMgrKeyInput;			/* �L�[���͊Ǘ� */

	CLibInfoCharCli		*m_pLibInfoChar;			/* �L������񃉃C�u���� */
	CLibInfoDisable		*m_pLibInfoDisable;			/* ���ۏ�񃉃C�u���� */
	CLibInfoMapBase		*m_pLibInfoMap;				/* �}�b�v��񃉃C�u���� */
	CLibInfoMapObject	*m_pLibInfoMapObject;		/* �}�b�v�I�u�W�F�N�g��񃉃C�u���� */
	CLibInfoMapParts	*m_pLibInfoMapParts;		/* �}�b�v�p�[�c��񃉃C�u���� */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;		/* �}�b�v�e��񃉃C�u���� */
	CLibInfoItem		*m_pLibInfoItem;			/* �A�C�e����񃉃C�u���� */
	CLibInfoItemType	*m_pLibInfoItemType;		/* �A�C�e����ʏ�񃉃C�u���� */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;		/* �����񃉃C�u���� */
	CLibInfoEffect		*m_pLibInfoEffect;			/* �G�t�F�N�g��񃉃C�u���� */
	CLibInfoMotion		*m_pLibInfoMotion;			/* ���[�V������񃉃C�u���� */
	CLibInfoMotionType	*m_pLibInfoMotionType;		/* ���[�V������ʏ�񃉃C�u���� */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;		/* ���o����񃉃C�u���� */
	CLibInfoSystem		*m_pLibInfoSystem;			/* �V�X�e����񃉃C�u���� */
	CLibInfoSkill		*m_pLibInfoSkill;			/* �X�L����񃉃C�u���� */
	CLibInfoCharCli		*m_pLibInfoCharOnline;		/* ���O�C�����L������񃉃C�u���� */

	CmyArray<PSTSYSTEMMSG, PSTSYSTEMMSG>	m_aSystemMsg;	/* �V�X�e�����b�Z�[�W */
} CMgrData, *PCMgrData;

/* Copyright(C)URARA-works 2006 */
