/* Copyright(C)URARA-works 2007 */
/* ========================================================================= */
/* �t�@�C�����F	SboSoundData.h												 */
/* ���e�F		���֘ADLL ��`�t�@�C��										 */
/* �쐬�F		�N����N���t�����(URARA-works)								 */
/* �쐬�J�n���F	2007/05/03													 */
/* ========================================================================= */

#ifdef SOUNDDATA_EXPORTS
#define SOUNDDATA_API __declspec(dllexport)
#else
#define SOUNDDATA_API __declspec(dllimport)
#endif


/* ========================================================================= */
/* �萔�̒�`																 */
/* ========================================================================= */

/* ���ʉ� */
enum {
	SOUNDID_NONE				= 0,
	SOUNDID_OK_PI73				= 1,				/* ���艹(pi73.wav) */
	SOUNDID_FOOT_ASI_2			= 2,				/* ����(asi_2.wav) */
	SOUNDID_CURSORMOVE			= 3,				/* �J�[�\���ړ� */
	SOUNDID_CANCEL				= 4,				/* �L�����Z�� */
	SOUNDID_CHARI14_A			= 5,				/* �퓬�J�n */
	SOUNDID_SWING24_C			= 6,				/* ����U�� */
	SOUNDID_SWING25_C			= 7,				/* ����˂� */
	SOUNDID_HIT0				= 8,				/* �q�b�g0 */
	SOUNDID_HIT1				= 9,				/* �q�b�g1 */
	SOUNDID_DROP_MONEY			= 10,				/* �h���b�v:���� */
	SOUNDID_DROP_ITEM_SMALL		= 11,				/* �h���b�v:���A�C�e�� */
	SOUNDID_DROP_ITEM_MIDIUM	= 12,				/* �h���b�v:���A�C�e�� */
	SOUNDID_DROP_ITEM_LARGE		= 13,				/* �h���b�v:��A�C�e�� */
	SOUNDID_DROP_MATERIAL		= 14,				/* �h���b�v:�f�� */
	SOUNDID_DROP_WEAPON			= 15,				/* �h���b�v:���� */
	SOUNDID_DROP_CLOTH			= 16,				/* �h���b�v:���E���[�u */
	SOUNDID_CRASH20_D			= 17,				/* �M�����鉹 */
	SOUNDID_KUSA				= 18,				/* �������鉹 */
	SOUNDID_KAI					= 19,				/* �L�����鉹 */
	SOUNDID_CHAT				= 20,				/* �`���b�g */
	SOUNDID_W_CHAT				= 21,				/* �A�i�E���X */
	SOUNDID_CLAP02				= 22,				/* ����2 */
	SOUNDID_CLAP03				= 23,				/* ����3 */
	SOUNDID_CLAP0203			= 23,				/* ����2������3 */
	SOUNDID_BALLOON1			= 24,				/* ���o�� */
	SOUNDID_BIKKURI				= 25,				/* �т����� */
	SOUNDID_WHISTLES			= 26,				/* �z�C�b�X���r */
	SOUNDID_WHISTLEL			= 27,				/* �z�C�b�X���k */
	SOUNDID_GIMON				= 28,				/* �^�� */
	SOUNDID_NIKU				= 29,				/* �� */
	SOUNDID_KIZUKU				= 30,				/* �C�t�� */
	SOUNDID_PINPONPANPON		= 31,				/* �����O�̂��m�点�� */
	SOUNDID_SWOON				= 32,				/* �C�� */
	SOUNDID_URARA_WORKS			= 33,				/* �E�������[�N�X */
	SOUNDID_PUYON_ATACK			= 34,				/* �v�����U�� */
	SOUNDID_LIQUID				= 35,				/* �t�� */
	SOUNDID_BOW_ATACK			= 36,				/* �|�U�� */
	SOUNDID_MEAL				= 37,				/* �H�� */
	SOUNDID_DEFENSE				= 38,				/* �h�� */
	SOUNDID_SHIELD				= 39,				/* ���\�� */
	SOUNDID_PUYON_MOVE			= 40,				/* �v�����ړ� */
	SOUNDID_PUYON_DEAD			= 41,				/* �v�������S */
	SOUNDID_EXIST				= 42,				/* ���� */
	SOUNDID_TRASH				= 43,				/* �S�~���J���E�̂Ă� */
	SOUNDID_DROP_BIN			= 44,				/* �h���b�v�F�r�� */
	SOUNDID_OPEN_BAG			= 45,				/* �o�b�O�J�� */
	SOUNDID_MISS				= 46,				/* �~�X */
	SOUNDID_LVUP				= 47,				/* ���x���A�b�v */
	SOUNDID_CRITICAL1			= 48,				/* �a���N���e�B�J�� */
	SOUNDID_HIT2				= 49,				/* �a���q�b�g�P */
	SOUNDID_HIT3				= 50,				/* �a���q�b�g�Q */
	SOUNDID_OPEN_WINDOW			= 51,				/* ���J�� */
	SOUNDID_CRITICAL2			= 52,				/* �Ō��N���e�B�J�� */
	SOUNDID_CRITICAL3			= 53,				/* ��N���e�B�J�� */
	SOUNDID_HIT4				= 54,				/* ��q�b�g */
	SOUNDID_CHARGE				= 55,				/* ���� */
	SOUNDID_CHARGE2				= 56,				/* ���߂Q */
	SOUNDID_DR_BASS01			= 57,				/* �x�[�X�P */
	SOUNDID_DR_BASS02			= 58,				/* �x�[�X�Q */
	SOUNDID_DR_BASS05			= 59,				/* �x�[�X�T */
	SOUNDID_DR_CLAP01			= 60,				/* �N���b�v�P */
	SOUNDID_DR_SNE02			= 61,				/* �X�l�A�Q */
	SOUNDID_KIE_ATACK			= 62,				/* �L�[�U�� */
	SOUNDID_KIE_DEAD			= 63,				/* �L�[���S */
	SOUNDID_FLY					= 64,				/* �H�΂��� */
	SOUNDID_MAX
};


/* ========================================================================= */
/* �֐��̒�`																 */
/* ========================================================================= */

extern "C"{
SOUNDDATA_API int		GetSoundCount		(void);				/* ���ʉ������擾 */
SOUNDDATA_API int		GetSoundResourceID	(int nNo);			/* ���ʉ��̃��\�[�XID���擾 */
SOUNDDATA_API DWORD		GetSoundID			(int nNo);			/* ���ʉ�ID���擾 */
SOUNDDATA_API int		GetSoundNo			(DWORD dwSoundID);	/* ���ʉ��̃C���f�b�N�X���擾 */
SOUNDDATA_API LPCSTR	GetSoundName		(DWORD dwSoundID);	/* ���ʉ������擾 */
}

/* Copyright(C)URARA-works 2007 */
