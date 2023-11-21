#include "SPCommon.h"
#include "SPMainGameDEF.H"
#include "SPGameObjectDEF.h"
#include "SPGameObject.h"
#include "SPStoryActor.h"
#include "SPStoryScene.h"

#include "SPLDTFile.h"
#include "SPLDTManager.h"
#include "SPResourceManager.h"
#include "SPWindowDEF.h"
#include "SPWindow.h"

//------------------------------------------------------------------------------------
SPStoryScene::SPStoryScene(SPWindow* pkWindow)
: m_pkWindow(pkWindow)
{
}

//------------------------------------------------------------------------------------
SPStoryScene::~SPStoryScene()
{
	Clear();
}

//------------------------------------------------------------------------------------
void SPStoryScene::Clear()
{
	vector< SPStoryActor* >::iterator iter = m_vActorList.begin();
	while( iter != m_vActorList.end() )
	{
		SAFE_DELETE(*iter);
		++iter;
	}
	m_vActorList.clear();
	
	m_vDefaultLayer.clear();
	m_vActorSpeakList.clear();
}

//------------------------------------------------------------------------------------
void SPStoryScene::AddActor(SPStoryActor* pkActor)
{
	m_vActorList.push_back(pkActor);
}

//------------------------------------------------------------------------------------
bool SPStoryScene::LoadDefaultLayerLDT(const char* pszFileName)
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName(pszFileName, &pLDTFile);
	if( pLDTFile == NULL )
		return false;

	m_vDefaultLayer.clear();

	int iItemID, iSize, i;
	LDT_Field LDTField;
	ACTOR_DEFAULT_LAYER Layer;

	iSize = pLDTFile->GetItemCount();
	for( i = 0; i < iSize; i++ )
	{
		ZeroMemory(&Layer, sizeof(ACTOR_DEFAULT_LAYER));

		iItemID = pLDTFile->GetPrimaryKey(i);

		pLDTFile->GetFieldFromLable(iItemID, "_Class", LDTField);
		Layer.iClassType = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Gender", LDTField);
		Layer.iGender = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Default_Hair", LDTField);
		Layer.iDefaultHair = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Default_Expression", LDTField);
		Layer.iDefaultExpression = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Default_Skin", LDTField);
		Layer.iDefaultSkin = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Default_Blouse", LDTField);
		Layer.iDefaultBlouse = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Default_Pants", LDTField);
		Layer.iDefaultPants = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Cap", LDTField);
		Layer.iCap = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Glasses", LDTField);
		Layer.iGlasses = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Earring", LDTField);
		Layer.iEarring = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Blouse", LDTField);
		Layer.iBlouse = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Pants", LDTField);
		Layer.iPants = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Cloak", LDTField);
		Layer.iCloak = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Glove", LDTField);
		Layer.iGlove = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Footwear", LDTField);
		Layer.iFootwear = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Stocking", LDTField);
		Layer.iStocking = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Makeup", LDTField);
		Layer.iMakeup = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Weapon", LDTField);
		Layer.iWeapon = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Shield", LDTField);
		Layer.iShield = LDTField.uData.lValue;

		m_vDefaultLayer.push_back(Layer);
	}
	
	return true;
}

//------------------------------------------------------------------------------------
bool SPStoryScene::LoadSpeekLDT(const char* pszFileName)
{
	SPLDTFile* pLDTFile = NULL;
	g_pResourceManager->GetLDTFromFileName(pszFileName, &pLDTFile);
	if( pLDTFile == NULL )
		return false;

	m_vActorSpeakList.clear();

	int iItemID, iSize, i;
	LDT_Field LDTField;
	ACTOR_SPEAK Speek;

	iSize = pLDTFile->GetItemCount();
	for( i = 0; i < iSize; i++ )
	{
		Speek.strHelpMessage.clear();
		Speek.strTalkMessage.clear();

		iItemID = pLDTFile->GetPrimaryKey(i);
		Speek.iNo = iItemID;

		pLDTFile->GetFieldFromLable(iItemID, "_Page", LDTField);
		Speek.iPage = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Cutin", LDTField);
		Speek.iImage = LDTField.uData.lValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Text", LDTField);
		Speek.strTalkMessage = LDTField.uData.pValue;

		pLDTFile->GetFieldFromLable(iItemID, "_Help", LDTField);
		Speek.strHelpMessage = LDTField.uData.pValue;

		m_vActorSpeakList.push_back(Speek);
	}

	return true;
}

//------------------------------------------------------------------------------------
ACTOR_DEFAULT_LAYER* SPStoryScene::GetDefaultLayer(int iClassType, int iGender)
{
	vector< ACTOR_DEFAULT_LAYER >::iterator iter = m_vDefaultLayer.begin();
	while( iter != m_vDefaultLayer.end() )
	{
		if( ((*iter).iClassType == iClassType) &&
			((*iter).iGender == iGender) )
			return &(*iter);

		++iter;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
ACTOR_SPEAK* SPStoryScene::GetActorSpeak(int iNo)
{
	vector< ACTOR_SPEAK >::iterator iter = m_vActorSpeakList.begin();
	while( iter != m_vActorSpeakList.end() )
	{
		if( (*iter).iNo == iNo )
			return &(*iter);

		++iter;
	}
	return NULL;
}

//------------------------------------------------------------------------------------
SPStoryActor* SPStoryScene::GetActor(const char* pszName)
{
	vector< SPStoryActor* >::iterator iter = m_vActorList.begin();
	while( iter != m_vActorList.end() )
	{
		if( strcmp((*iter)->GetName(), pszName) == 0 )
			return (*iter);

		++iter;
	}

	return NULL;
}

//------------------------------------------------------------------------------------
void SPStoryScene::SetRenderOrder(const char* pszName, int iOrder)
{
	SPStoryActor* pkActor = GetActor(pszName);
	if( pkActor == NULL )
		return;

	pkActor->SetRenderOrder(iOrder);
}

//------------------------------------------------------------------------------------
int SPStoryScene::SendGOBMessage(const char* pszName, SPGM uiMsg, LPARAM lParam)
{
	SPStoryActor* pkActor = GetActor(pszName);
	if( pkActor == NULL )
		return 0;

	return pkActor->SendGOBMessage(uiMsg, lParam);
}

//------------------------------------------------------------------------------------
void SPStoryScene::StartScene(int iSceneNo)
{
	vector< SPStoryActor* >::iterator iter = m_vActorList.begin();
	while( iter != m_vActorList.end() )
	{
		(*iter)->StartScene(iSceneNo);
		++iter;
	}
}

//------------------------------------------------------------------------------------
void SPStoryScene::CompleteActorAction(const char* pszName, int iSceneNo)
{
	if( m_pkWindow == NULL )
		return;

	m_pkWindow->SPSendMessage(SPIM_ACTOR_SCENE_COMPLETE, (WPARAM)pszName, iSceneNo);
}

//------------------------------------------------------------------------------------
void SPStoryScene::Process(float fTime)
{
	vector< SPStoryActor* >::iterator iter = m_vActorList.begin();
	while( iter != m_vActorList.end() )
	{
		(*iter)->Process(fTime);
		++iter;
	}
}

//------------------------------------------------------------------------------------
void SPStoryScene::Render(float fTime)
{
	int i, iRenderOrder = 0;
	vector< SPStoryActor* >::iterator iter;
	for( i = 0; i < 2; i++ )
	{
		iter = m_vActorList.begin();
		while( iter != m_vActorList.end() )
		{
			if( (*iter)->GetRenderOrder() == iRenderOrder )
			{
				(*iter)->Render(fTime);
				g_pVideo->Flush();
			}

			++iter;
		}

		++iRenderOrder;
	}
}