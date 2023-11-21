#ifndef __SPINDUN_GUILDROOM_H__
#define __SPINDUN_GUILDROOM_H__

class SPIndun ;
class SPIndunGuildRoom : public SPIndun
{
public:
	SPIndunGuildRoom() ;
	SPIndunGuildRoom(int indunID, UINT32 iInstanceID, INDUN_TYPE iType);
	virtual ~SPIndunGuildRoom();
	virtual bool Init();
	virtual void Clear();
	virtual void Process(float fTime);
	virtual void Render();

protected:
};


#endif