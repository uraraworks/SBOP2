/* Copyright(C)URARA-works 2006 */
/* ========================================================================= */
/* ƒtƒ@ƒCƒ‹–¼	:MgrData.h													 */
/* “à—e			:ƒf[ƒ^ƒ}ƒl[ƒWƒƒƒNƒ‰ƒX ’è‹`ƒtƒ@ƒCƒ‹						 */
/* ì¬			:”N‚ª‚ç”N’†t‚¤‚ç‚ç(URARA-works)							 */
/* ì¬ŠJŽn“ú	:2006/11/04													 */
/* ========================================================================= */

#pragma once

class CMainFrame;
class CUraraSockTCPSBO;
class CTextOutput;
class CLibInfoAccount;
class CLibInfoCharSvr;
class CLibInfoDisable;
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
class CLibInfoTalkEvent;
class CInfoFileList;

/* ========================================================================= */
/* ƒNƒ‰ƒXéŒ¾																 */
/* ========================================================================= */

typedef class CMgrData
{
public:
			CMgrData();													/* ƒRƒ“ƒXƒgƒ‰ƒNƒ^ */
	virtual ~CMgrData();												/* ƒfƒXƒgƒ‰ƒNƒ^ */

	void	Create				(CMainFrame *pMainFrame, CUraraSockTCPSBO *pSock);	/* ì¬ */
	void	Destroy				(void);												/* ”jŠü */

	void	Save				(void);											/* ƒf[ƒ^‚Ì•Û‘¶ */
	void	Load				(void);											/* ƒf[ƒ^‚Ì“Ç‚Ýž‚Ý */
	void	ReadHashList		(void);											/* ƒtƒ@ƒCƒ‹ƒŠƒXƒg‚ð“Ç‚Ýž‚Ý */
	WORD	GetPort				(void)	{ return m_wPort;				}		/* ‘Ò‚¿‚¤‚¯ƒ|[ƒg”Ô†‚ðŽæ“¾ */
	void	AddOnline			(void)	{ m_byOnline ++;				}		/* ƒIƒ“ƒ‰ƒCƒ“”‚ð’Ç‰Á */
	void	DecOnline			(void)	{ m_byOnline --;				}		/* ƒIƒ“ƒ‰ƒCƒ“”‚ðŒ¸ŽZ */
	BYTE	GetOnline			(void)	{ return m_byOnline;			}		/* ƒIƒ“ƒ‰ƒCƒ“”‚ðŽæ“¾ */
	LPCSTR	GetAdminAccount		(void)	{ return m_strAdminAccount;		}		/* ŠÇ—ŽÒŒ ŒÀƒAƒJƒEƒ“ƒg‚ðŽæ“¾ */
	LPCSTR	GetClientVersion	(void)	{ return m_strClientVersion;	}		/* ƒNƒ‰ƒCƒAƒ“ƒgƒo[ƒWƒ‡ƒ“‚ðŽæ“¾ */
	void	SetClientVersion	(LPCSTR pszVersion);							/* ƒNƒ‰ƒCƒAƒ“ƒgƒo[ƒWƒ‡ƒ“‚ðÝ’è */
	BYTE	GetLastSendClock	(void)	{ return m_byLastSendClock; 	}		/* ÅŒã‚É‘—M‚µ‚½Žž‚ðŽæ“¾ */
	void	SetLastSendClock	(BYTE byLastSendClock)	{ m_byLastSendClock = byLastSendClock; };	/* ÅŒã‚É‘—M‚µ‚½Žž‚ðÝ’è */
	LPCSTR	GetFtpAccount		(void)	{ return m_strFtpAccount;		}		/* FTPƒAƒJƒEƒ“ƒg‚ðŽæ“¾ */
	LPCSTR	GetFtpPassword		(void)	{ return m_strFtpPassword;		}		/* FTPƒpƒXƒ[ƒh‚ðŽæ“¾ */
	LPCSTR	GetFtpServerAddr	(void)	{ return m_strFtpServerAddr;	}		/* ƒT[ƒo[ƒAƒhƒŒƒX‚ðŽæ“¾ */
	LPCSTR	GetFtpUploadPath	(void)	{ return m_strFtpUploadPath;	}		/* ƒAƒbƒvƒ[ƒhæ‚ðŽæ“¾ */

	CMainFrame			*GetMainFrame			(void)	{ return m_pMainFrame;		}	/* ƒƒCƒ“ƒtƒŒ[ƒ€‚ðŽæ“¾ */
	CUraraSockTCPSBO	*GetSock				(void)	{ return m_pSock;			}	/* ’ÊMƒ}ƒl[ƒWƒƒ‚ðŽæ“¾ */

	CTextOutput			*GetLog					(void)	{ return m_pLog;				}	/* ƒƒOƒNƒ‰ƒX‚ðŽæ“¾ */
	CLibInfoAccount		*GetLibInfoAccount		(void)	{ return m_pLibInfoAccount;		}	/* ƒAƒJƒEƒ“ƒgî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoCharSvr		*GetLibInfoChar			(void)	{ return m_pLibInfoChar;		}	/* ƒLƒƒƒ‰î•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoDisable		*GetLibInfoDisable		(void)	{ return m_pLibInfoDisable;		}	/* ‹‘”Ûî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoMapBase		*GetLibInfoMap			(void)	{ return m_pLibInfoMap;			}	/* ƒ}ƒbƒvî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoMapObject	*GetLibInfoMapObject	(void)	{ return m_pLibInfoMapObject;	}	/* ƒ}ƒbƒvƒIƒuƒWƒFƒNƒgî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoMapParts	*GetLibInfoMapParts		(void)	{ return m_pLibInfoMapParts;	}	/* ƒ}ƒbƒvƒp[ƒcî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoMapShadow	*GetLibInfoMapShadow	(void)	{ return m_pLibInfoMapShadow;	}	/* ƒ}ƒbƒv‰eî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoItemType	*GetLibInfoItemType		(void)	{ return m_pLibInfoItemType;	}	/* ƒAƒCƒeƒ€Ží•Êî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoItem		*GetLibInfoItem			(void)	{ return m_pLibInfoItem;		}	/* ƒAƒCƒeƒ€î•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoItemWeapon	*GetLibInfoItemWeapon	(void)	{ return m_pLibInfoItemWeapon;	}	/* •Šíî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoEffect		*GetLibInfoEffect		(void)	{ return m_pLibInfoEffect;		}	/* ƒGƒtƒFƒNƒgî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoMotion		*GetLibInfoMotion		(void)	{ return m_pLibInfoMotion;		}	/* ƒ‚[ƒVƒ‡ƒ“î•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoMotionType	*GetLibInfoMotionType	(void)	{ return m_pLibInfoMotionType;	}	/* ƒ‚[ƒVƒ‡ƒ“Ží•Êî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoEfcBalloon	*GetLibInfoEfcBalloon	(void)	{ return m_pLibInfoEfcBalloon;	}	/* •¬o‚µî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoSystem		*GetLibInfoSystem		(void)	{ return m_pLibInfoSystem;		}	/* ƒVƒXƒeƒ€î•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoSkill		*GetLibInfoSkill		(void)	{ return m_pLibInfoSkill;		}	/* ƒXƒLƒ‹î•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CLibInfoTalkEvent	*GetLibInfoTalkEvent	(void)	{ return m_pLibInfoTalkEvent;	}	/* ‰ï˜bƒCƒxƒ“ƒgî•ñƒ‰ƒCƒuƒ‰ƒŠ‚ðŽæ“¾ */
	CInfoFileList		*GetInfoFileList		(void)	{ return m_pInfoFileList;		}	/* ƒtƒ@ƒCƒ‹ƒŠƒXƒgî•ñ‚ðŽæ“¾ */


private:
	void	ReadIniData		(void);				/* iniƒtƒ@ƒCƒ‹‚©‚çÝ’è‚ð“Ç‚Ýž‚Ý */


private:
	WORD				m_wPort;				/* ‘Ò‚¿‚¤‚¯ƒ|[ƒg */
	BYTE				m_byOnline,				/* ƒIƒ“ƒ‰ƒCƒ“” */
						m_byLastSendClock;		/* ÅŒã‚É‘—M‚µ‚½Žž */
	CmyString			m_strAdminAccount,		/* ŠÇ—ŽÒŒ ŒÀƒAƒJƒEƒ“ƒg–¼ */
						m_strClientVersion,		/* ƒNƒ‰ƒCƒAƒ“ƒgƒo[ƒWƒ‡ƒ“ */
						m_strFtpAccount,		/* FTPƒAƒJƒEƒ“ƒg */
						m_strFtpPassword,		/* FTPƒpƒXƒ[ƒh */
						m_strFtpServerAddr,		/* ƒT[ƒo[ƒAƒhƒŒƒX */
						m_strFtpUploadPath;		/* ƒAƒbƒvƒ[ƒhæ */

	CMainFrame			*m_pMainFrame;			/* ƒƒCƒ“ƒtƒŒ[ƒ€ */
	CUraraSockTCPSBO	*m_pSock;				/* ’ÊMƒ}ƒl[ƒWƒƒ */
	CTextOutput			*m_pLog;				/* ƒƒOƒtƒ@ƒCƒ‹ */

	CLibInfoAccount		*m_pLibInfoAccount;		/* ƒAƒJƒEƒ“ƒgî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoCharSvr		*m_pLibInfoChar;		/* ƒLƒƒƒ‰î•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoDisable		*m_pLibInfoDisable;		/* ‹‘”Ûî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoMapBase		*m_pLibInfoMap;			/* ƒ}ƒbƒvî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoMapObject	*m_pLibInfoMapObject;	/* ƒ}ƒbƒvƒIƒuƒWƒFƒNƒgî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoMapParts	*m_pLibInfoMapParts;	/* ƒ}ƒbƒvƒp[ƒcî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoMapShadow	*m_pLibInfoMapShadow;	/* ƒ}ƒbƒv‰eî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoItemType	*m_pLibInfoItemType;	/* ƒAƒCƒeƒ€Ží•Êî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoItem		*m_pLibInfoItem;		/* ƒAƒCƒeƒ€î•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoItemWeapon	*m_pLibInfoItemWeapon;	/* •Šíî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoEffect		*m_pLibInfoEffect;		/* ƒGƒtƒFƒNƒgî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoMotion		*m_pLibInfoMotion;		/* ƒ‚[ƒVƒ‡ƒ“î•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoMotionType	*m_pLibInfoMotionType;	/* ƒ‚[ƒVƒ‡ƒ“Ží•Êî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoEfcBalloon	*m_pLibInfoEfcBalloon;	/* •¬o‚µî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoSystem		*m_pLibInfoSystem;		/* ƒVƒXƒeƒ€î•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoSkill		*m_pLibInfoSkill;		/* ƒXƒLƒ‹î•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CLibInfoTalkEvent	*m_pLibInfoTalkEvent;	/* ‰ï˜bƒCƒxƒ“ƒgî•ñƒ‰ƒCƒuƒ‰ƒŠ */
	CInfoFileList		*m_pInfoFileList;		/* ƒtƒ@ƒCƒ‹ƒŠƒXƒgî•ñ */
} CMgrData, *PCMgrData;

/* Copyright(C)URARA-works 2006 */
