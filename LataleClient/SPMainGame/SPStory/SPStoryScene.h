#ifndef __SPSTORY_SCENE_H__
#define __SPSTORY_SCENE_H__

class SPStoryActor;
class SPWindow;
struct ACTOR_DEFAULT_LAYER;

class SPStoryScene
{
public:
	SPStoryScene(SPWindow* pkWindow);
	~SPStoryScene();

	void Clear();
	void AddActor(SPStoryActor* pkActor);
	bool LoadDefaultLayerLDT(const char* pszFileName);
	bool LoadSpeekLDT(const char* pszFileName);

	void Process(float fTime);
	void Render(float fTime);

	void StartScene(int iSceneNo);
	void CompleteActorAction(const char* pszName, int iSceneNo);

	ACTOR_DEFAULT_LAYER* GetDefaultLayer(int iClassType, int iGender);
	ACTOR_SPEAK* GetActorSpeak(int iNo);

	void SetRenderOrder(const char* pszName, int iOrder);

	int SendGOBMessage(const char* pszName, SPGM uiMsg, LPARAM lParam);

private:
	SPStoryActor* GetActor(const char* pszName);

private:
	SPWindow* m_pkWindow;
	std::vector< SPStoryActor* > m_vActorList;
	std::vector< ACTOR_DEFAULT_LAYER > m_vDefaultLayer;
	std::vector< ACTOR_SPEAK > m_vActorSpeakList;
};

#endif