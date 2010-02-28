#pragma once

/* ========================================================================= */
/* �\���̒�`																 */
/* ========================================================================= */

/* �A�C�e����ʏ��\����(������) */
struct stInfoItemTypeArms {
	DWORD		dwWeaponInfoID;				/* ������ID */
	DWORD		dwValue;					/* �U���� */
	DWORD		dwMoveWait;					/* ���x */
	DWORD		dwMoveCount;				/* �򋗗� */

	stInfoItemTypeArms()
	: dwWeaponInfoID(0)
	, dwValue(0)
	, dwMoveWait(0)
	, dwMoveCount(0)
	{}
};

/* �A�C�e����ʏ��\����(HP����) */
struct stInfoItemTypeHp {
	DWORD		dwMin;						/* ����(�ŏ�) */
	DWORD		dwMax;						/* ����(�ő�) */

	stInfoItemTypeHp()
	: dwMin(0)
	, dwMax(0)
	{}
};

/* �A�C�e����ʏ��\����(����) */
struct stInfoItemTypeLight {
	DWORD		dwLevel;					/* ���背�x�� */
	DWORD		dwTime;						/* �������� */

	stInfoItemTypeLight()
	: dwLevel(0)
	, dwTime(0)
	{}
};

/* �A�C�e����ʏ��\���� */
struct stInfoItemType {
	BYTE		byDelAverage;				/* �g���ĂȂ��Ȃ�m�� */
	BYTE		byTarget;					/* �g�p�Ώ� */
	BYTE		byArea;						/* �g�p�͈� */
	BOOL		bPutOn;						/* ��ɒu���邩���� */
	WORD		wGrpIDMain;					/* �摜ID���C�� */
	WORD		wGrpIDSub;					/* �摜ID�T�u */
	DWORD		dwTypeID;					/* ���ID */
	DWORD		dwItemTypeID;				/* �A�C�e�����ID */
	DWORD		dwGrpID;					/* �n�ʂɂ��鎞�̉摜ID */
	DWORD		dwIconGrpID;				/* �o�b�N�p�b�N���̉摜ID */
	DWORD		dwDropSoundID;				/* �������Ƃ��̌��ʉ�ID */
	DWORD		dwUseEffectID;				/* �g�������ɍĐ�����G�t�F�N�gID */
	DWORD		dwUseSoundID;				/* �g�������ɍĐ�������ʉ�ID */
	CmyString	strName;					/* �A�C�e���� */

	union Type {
		stInfoItemTypeArms	Arms;			/* ������ */
		stInfoItemTypeHp	Hp;				/* HP���� */
		stInfoItemTypeLight	Light;			/* ���� */
	};

	stInfoItemType()
	: byDelAverage(0)
	, byTarget(0)
	, byArea(0)
	, bPutOn(0)
	, wGrpIDMain(0)
	, wGrpIDSub(0)
	, dwTypeID(0)
	, dwItemTypeID(0)
	, dwGrpID(0)
	, dwIconGrpID(0)
	, dwDropSoundID(0)
	, dwUseEffectID(0)
	, dwUseSoundID(0)
	{}
}
