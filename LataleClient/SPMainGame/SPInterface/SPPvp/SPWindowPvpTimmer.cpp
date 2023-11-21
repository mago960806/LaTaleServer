#include "SPCommon.h"
#include "SPUtil.h"

#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceDef.h"
#include "SPResourceManager.h"

#include "SPSubGameManager.h"
#include "SPGameObject.h"
#include "SPManager.h"
#include "SPMouseCursor.h"
#include "SPIMEPool.h"
#include "SPWindowDEF.h"
#include "SPInterfaceManager.h"

#include "SPRenderModel.h"
#include "SPFXDEF.h"
#include "SPFXModel.h"
#include "SPFXUnit.h"
#include "SPFXGroupUnit.h"
#include "SPFXArchive.h"

#include "SPGameObject.h"
#include "SPGOBStatus.h"
#include "SPGOBClusterDef.h"
#include "SPGOBCluster.h"
#include "SPGobModelUnit.h"
#include "SPAvatarModelUnitDef.h"
#include "SPAvatarModelUnit.h"
#include "SPPlayer.h"
#include "SPGOBManager.h"

#include "SPSoundArchive.h"
#include "SPSoundManager.h"

#include "SPWindow.h"
#include "SPWindowStatic.h"
#include "SPWindowPvpTimmer.h"

#include "_Error_log.h"


SPWindowPvpTimmer::SPWindowPvpTimmer(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent /*= NULL*/)
: SPWindow(WNDID_PVP_TIMMER, InstanceID, iX, iY, iCX, iCY, pParent)
{
	Init();
}


SPWindowPvpTimmer::~SPWindowPvpTimmer()
{
	Clean();
}


void SPWindowPvpTimmer::Init()
{
	m_iSetTime = 0 ;
	m_iWarnTime = 0 ;
	m_fElapsedTime = 0 ;	// 시작후 누적 시간
	m_fSecondTime = 0 ;		// 1초

	SPWindowStatic* pStaticBG = NULL;
	SPWindowStatic* pStatic = NULL;

	// Start Effect
	m_pFXArchive	=	new SPFXArchive( NULL );

	// Background
	pStaticBG = new SPWindowStatic( WIID_PVP_TIMMER_BG , 0,  0,  150,  60, this);
	pStaticBG->SetImage("DATA/INTERFACE/CONCEPT/UI_WinBG_001.PNG", 322, 1) ;
	pStaticBG->Hide();

	pStatic = new SPWindowStatic( WIID_PVP_TIMMER_TITLE , 8,  6,  56,  7, pStaticBG);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 271, 93) ;
	pStatic->Hide();

	pStatic = new SPWindowStatic( WIID_PVP_TIMMER_COLON , 65,  20,  14,  28, pStaticBG);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 262, 106) ;
	pStatic->Hide();

	pStatic = new SPWindowStatic( WIID_PVP_TIMMER_MINUTE1 , 19,  12,  31,  42, pStaticBG);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 184) ;		// 0
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 141) ;		// 1
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 141) ;		// 2
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 141) ;		// 3
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 141) ;		// 4
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 141) ;		// 5
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 270) ;		// Warn 0
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 227) ;		// Warn 1
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 227) ;		// Warn 2
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 227) ;		// Warn 3
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 227) ;		// Warn 4
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 227) ;		// Warn 5

	
	pStatic->Hide();
	pStatic = new SPWindowStatic( WIID_PVP_TIMMER_MINUTE2 , 41,  12,  31,  42, pStaticBG);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 184) ;		// 0
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 141) ;		// 1
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 141) ;		// 2
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 141) ;		// 3
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 141) ;		// 4
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 141) ;		// 5
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 184) ;		// 6
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 184) ;		// 7
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 184) ;		// 8
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 184) ;		// 9
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 270) ;		// Warn 0
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 227) ;		// Warn 1
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 227) ;		// Warn 2
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 227) ;		// Warn 3
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 227) ;		// Warn 4
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 227) ;		// Warn 5
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 270) ;		// Warn 6
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 270) ;		// Warn 7
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 270) ;		// Warn 8
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 270) ;		// Warn 9
	pStatic->Hide();

	pStatic = new SPWindowStatic( WIID_PVP_TIMMER_SECOND1 , 73,  12,  31,  42, pStaticBG);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 184) ;		// 0
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 141) ;		// 1
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 141) ;		// 2
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 141) ;		// 3
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 141) ;		// 4
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 141) ;		// 5
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 270) ;		// Warn 0
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 227) ;		// Warn 1
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 227) ;		// Warn 2
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 227) ;		// Warn 3
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 227) ;		// Warn 4
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 227) ;		// Warn 5
	pStatic->Hide();

	pStatic = new SPWindowStatic( WIID_PVP_TIMMER_SECOND2 , 95,  12,  31,  42, pStaticBG);
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 184) ;		// 0
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 141) ;		// 1
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 141) ;		// 2
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 141) ;		// 3
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 141) ;		// 4
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 141) ;		// 5
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 184) ;		// 6
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 184) ;		// 7
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 184) ;		// 8
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 184) ;		// 9
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 270) ;		// Warn 0
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 227) ;		// Warn 1
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 227) ;		// Warn 2
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 227) ;		// Warn 3
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 227) ;		// Warn 4
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG", 129, 227) ;		// Warn 5
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",   1, 270) ;		// Warn 6
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  33, 270) ;		// Warn 7
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  65, 270) ;		// Warn 8
	pStatic->SetImage("DATA/INTERFACE/CONCEPT/UI_Object_000.PNG",  97, 270) ;		// Warn 9
	pStatic->Hide();

}



void SPWindowPvpTimmer::Clean()
{
	SAFE_DELETE(m_pFXArchive);
	SPWindow::Clean();
}


void SPWindowPvpTimmer::Show(bool bWithChild /*= true*/)
{
	// for Test set SECONDS
	// SetTestData(3515, 3500) ;

	SPWindow::Show(bWithChild);
}


void SPWindowPvpTimmer::Hide(bool bSendServer /*= true*/)
{
	if( m_pFXArchive )
		m_pFXArchive->Clean();

	SPWindow::Hide(bSendServer);
}


void SPWindowPvpTimmer::Process(float fTime)
{
	SPWindow* pStatic = NULL ;
	int iWarnParam = 0 ;
	if(!m_bShow)			return;

	if( m_pFXArchive != NULL )
	{
		m_pFXArchive->Process( fTime );
	}

	// Time Over
	if( m_iSetTime <= (int)m_fElapsedTime )
	{
		pStatic = Find(WIID_PVP_TIMMER_MINUTE1, true) ;
		if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
		pStatic = Find(WIID_PVP_TIMMER_MINUTE2, true) ;
		if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
		pStatic = Find(WIID_PVP_TIMMER_SECOND1, true) ;
		if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
		pStatic = Find(WIID_PVP_TIMMER_SECOND2, true) ;
		if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
		return ;
	}

	m_fElapsedTime += fTime ;
	m_fSecondTime  += fTime ;
	if( m_fSecondTime > 1.0f )
	{
		m_fSecondTime -= 1.0f ;
		int iRemainTime = (m_iSetTime - (int)m_fElapsedTime) ;
		int iMin1, iMin2, iSec1, iSec2 ;
		iMin1 = iRemainTime / 600 ;
		iMin2 = ( iRemainTime / 60 ) % 10 ;
		iSec1 = ( iRemainTime % 60 ) / 10;
		iSec2 = iRemainTime % 10 ;

		if( m_iWarnTime >= iRemainTime )		{
			iWarnParam = 1 ;
			SetTimerEffect(0, PVP_WARN_SOUND_EFFECT) ;
		}
		pStatic = Find(WIID_PVP_TIMMER_MINUTE1, true) ;
		if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iMin1+iWarnParam*6) ;
		pStatic = Find(WIID_PVP_TIMMER_MINUTE2, true) ;
		if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iMin2+iWarnParam*10) ;
		pStatic = Find(WIID_PVP_TIMMER_SECOND1, true) ;
		if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iSec1+iWarnParam*6) ;
		pStatic = Find(WIID_PVP_TIMMER_SECOND2, true) ;
		if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, iSec2+iWarnParam*10) ;
		
	}
	
	SPWindow::Process(fTime);
}


void SPWindowPvpTimmer::Render(float fTime)
{
	if(!m_bShow)		return;
	
	SPWindow::Render(fTime);

	if( m_pFXArchive != NULL )
		m_pFXArchive->Render( fTime );
}


/**
	For Test 
 */
void SPWindowPvpTimmer::SetTestData(int iSeconds, int iWarns)
{
	SPWindow* pStatic = NULL ;
	m_iSetTime = iSeconds ;
	m_iWarnTime = iWarns ;
	m_fElapsedTime = 0 ;
	m_fSecondTime = 0 ;	

	pStatic = Find(WIID_PVP_TIMMER_MINUTE1, true) ;
	if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
	pStatic = Find(WIID_PVP_TIMMER_MINUTE2, true) ;
	if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
	pStatic = Find(WIID_PVP_TIMMER_SECOND1, true) ;
	if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
	pStatic = Find(WIID_PVP_TIMMER_SECOND2, true) ;
	if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
}



/**
	PVP Start Effect & Sound
 */
void SPWindowPvpTimmer::SetTimerEffect(int iEffectID, int iSoundEffect)
{
	SPPlayer*	pLocalPlayer = g_pGOBManager->GetLocalPlayer();

	// FX
	if( iEffectID > 0 )	{
		m_pFXArchive->Clean();
		m_pFXArchive->InsertFXGroup( (SPID_FX_GROUP)iEffectID );
	}

	// Sound
	SPSoundArchive* pSoundArchive	=	NULL;
	pLocalPlayer->SPGOBPerformMessage( SPGM_GETSOUNDARCHIVE , (LPARAM)&pSoundArchive );
	if( pSoundArchive != NULL )
	{
		SOUND_TABLE_INFO* pInfo = SPSoundManager::GetInstance()->GetSoundTable( (unsigned int)iSoundEffect );
		if( pInfo != NULL )
		{
			std::string strFullName;

			if( !pInfo->strFileName[0].empty() )
			{
				strFullName = "DATA/SOUND/";
				strFullName += pInfo->strFileName[0];

				pSoundArchive->InsertItem( strFullName.c_str() , pInfo->fDelayTime[0] , pInfo->bLoop[0] );
			}
		}
	}
}


SPIMESSAGE_OBJ_MAP_BEGIN	( SPWindowPvpTimmer )

SPIMESSAGE_COMMAND(SPIM_PVP_SET_TIMELIMIT												, OnSetPvpTime	)
SPIMESSAGE_CONTROL_RANGE(SPIM_LBUTTON_UP	, WIID_PVP_TIMMER, WIID_PVP_TIMMER+20		, OnClose	)

SPIMESSAGE_OBJ_MAP_END	( SPWindow	)


int SPWindowPvpTimmer::OnClose( int iID, WPARAM wParam, LPARAM lParam ) 
{
	//Hide() ;
	return 1;
}


/**
	PVP Timer에서 사용될 시간값 셋팅
	@param wParam : 셋팅될 시간값 (MAX : MAX_PVP_TIME_VALUE)
	@param lParam : Warning이 되어야 할 시간
 */
int SPWindowPvpTimmer::OnSetPvpTime(WPARAM wParam, LPARAM lParam )
{
	SPWindow* pStatic = NULL ;
	m_iSetTime = (int)wParam ;
	m_iWarnTime = (int)lParam ;

	if( m_iSetTime > MAX_PVP_TIME_VALUE )
		m_iSetTime = MAX_PVP_TIME_VALUE ;
	if( m_iWarnTime >= m_iSetTime )
		m_iWarnTime = m_iSetTime ;

	if( m_iSetTime < 0 || m_iWarnTime < 0 )
	{
		ErrorFLogs->Error_Log("TimeSet Value minus Fail... Time[%d], Warn[%d]", m_iSetTime, m_iWarnTime) ;
		m_iSetTime = 0 ;
		m_iWarnTime = 0 ;
		return 0 ;
	}
	ErrorFLogs->Error_Log("TimeSet Value Time[%d], Warn[%d]", m_iSetTime, m_iWarnTime) ;

	m_fElapsedTime = 0 ;
	m_fSecondTime = 0 ;	

	pStatic = Find(WIID_PVP_TIMMER_MINUTE1, true) ;
	if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
	pStatic = Find(WIID_PVP_TIMMER_MINUTE2, true) ;
	if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
	pStatic = Find(WIID_PVP_TIMMER_SECOND1, true) ;
	if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;
	pStatic = Find(WIID_PVP_TIMMER_SECOND2, true) ;
	if( pStatic )	pStatic->SPSendMessage(SPIM_SET_IMAGEINDEX, 0) ;

	SetTimerEffect(PVP_START_FX_EFFECT, PVP_START_SOUND_EFFECT) ;
	Show() ;

	return 1 ;
}