// UIMainIngameWnd.h:  окошки-информаци€ в игре
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "UIProgressBar.h"
#include "UIGameLog.h"

#include "../alife_space.h"

#include "UICarPanel.h"
#include "UIMotionIcon.h"
#include "../hudsound.h"
// дл€ режима настройки HUD
extern int g_bHudAdjustMode;
extern float g_fHudAdjustValue;

class CUIPdaMsgListItem;
class CLAItem;
class CUIZoneMap;
class CUIArtefactPanel;
class CUIScrollView;
struct GAME_NEWS_DATA;
class CActor;
class CWeapon;
class CMissile;
class CInventoryItem;

class CUIMainIngameWnd : public CUIWindow
{
public:
	CUIMainIngameWnd();
	virtual ~CUIMainIngameWnd();

	virtual void Init();
	virtual void Draw();
	virtual void Update();

	bool OnKeyboardPress(int dik);

protected:
	CUIStatic UIStaticDiskIO;
	CUIStatic UIStaticHealth;
	CUIStatic UIStaticArmor;
	CUIStatic UIStaticQuickHelp;
	CUIProgressBar UIHealthBar;
	CUIProgressBar UIArmorBar;
	CUICarPanel UICarPanel;
	CUIMotionIcon UIMotionIcon;
	CUIZoneMap* UIZoneMap;

	// иконка, показывающа€ количество активных PDA
	CUIStatic UIPdaOnline;

	// изображение оружи€
	CUIStatic UIWeaponBack;
	CUIStatic UIWeaponSignAmmo;
	CUIStatic UIWeaponIcon;
	Frect UIWeaponIcon_rect;

public:
	CUIStatic* GetPDAOnline() { return &UIPdaOnline; };
	void OnSectorChanged(int sector);

protected:
	// 5 статиков дл€ отображени€ иконок:
	// - сломанного оружи€
	// - радиации
	// - ранени€
	// - голода
	// - усталости
	CUIStatic UIWeaponJammedIcon;
	CUIStatic UIRadiaitionIcon;
	CUIStatic UIWoundIcon;
	CUIStatic UIStarvationIcon;
	CUIStatic UIPsyHealthIcon;
	CUIStatic UIInvincibleIcon;
	//	CUIStatic			UISleepIcon;
	CUIStatic UIArtefactIcon;

	CUIScrollView* m_UIIcons;
	CUIWindow* m_pMPChatWnd;
	CUIWindow* m_pMPLogWnd;

public:
	CUIArtefactPanel* m_artefactPanel;

public:
	// ≈нумы соответсвующие предупреждающим иконкам
	enum EWarningIcons
	{
		ewiAll = 0,
		ewiWeaponJammed,
		ewiRadiation,
		ewiWound,
		ewiStarvation,
		ewiPsyHealth,
		ewiInvincible,
		//		ewiSleep,
		ewiArtefact,
	};

	void SetMPChatLog(CUIWindow* pChat, CUIWindow* pLog);

	// «адаем цвет соответствующей иконке
	void SetWarningIconColor(EWarningIcons icon, const u32 cl);
	void TurnOffWarningIcon(EWarningIcons icon);

	// ѕороги изменени€ цвета индикаторов, загружаемые из system.ltx
	typedef xr_map<EWarningIcons, xr_vector<float>> Thresholds;
	typedef Thresholds::iterator Thresholds_it;
	Thresholds m_Thresholds;

	// ≈нум перечислени€ возможных мигающих иконок
	enum EFlashingIcons
	{
		efiPdaTask = 0,
		efiMail
	};

	void SetFlashIconState_(EFlashingIcons type, bool enable);

	void AnimateContacts(bool b_snd);
	HUD_SOUND m_contactSnd;

	void ReceiveNews(GAME_NEWS_DATA* news);

protected:
	void SetWarningIconColor(CUIStatic* s, const u32 cl);
	void InitFlashingIcons(CUIXml* node);
	void DestroyFlashingIcons();
	void UpdateFlashingIcons();
	void UpdateActiveItemInfo();

	void SetAmmoIcon(const shared_str& sect_name);

	// first - иконка, second - анимаци€
	DEF_MAP(FlashingIcons, EFlashingIcons, CUIStatic*);
	FlashingIcons m_FlashingIcons;

	// дл€ текущего активного актера и оружи€
	CActor* m_pActor;
	CWeapon* m_pWeapon;
	CMissile* m_pGrenade;
	CInventoryItem* m_pItem;

	// ќтображение подсказок при наведении прицела на объект
	void RenderQuickInfos();

public:
	CUICarPanel& CarPanel() { return UICarPanel; };
	CUIMotionIcon& MotionIcon() { return UIMotionIcon; }
	void OnConnected();
	void reset_ui();

protected:
	CInventoryItem* m_pPickUpItem;
	CUIStatic UIPickUpItemIcon;

	float m_iPickUpItemIconX;
	float m_iPickUpItemIconY;
	float m_iPickUpItemIconWidth;
	float m_iPickUpItemIconHeight;

	void UpdatePickUpItem();

public:
	void SetPickUpItem(CInventoryItem* PickUpItem);
#ifdef DEBUG
	void draw_adjust_mode();
#endif
};
