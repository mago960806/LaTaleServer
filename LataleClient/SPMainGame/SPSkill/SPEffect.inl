
//------------------------------------------------------------------------------------
inline void SPEffect::SetBuffInfo(const EFFECT_BUFF& Src)
{
	m_BuffInfo = Src;
}

//------------------------------------------------------------------------------------
inline void SPEffect::SetMissileInfo(const EFFECT_MISSILE& Src)
{
	m_MissileInfo = Src;
}

//------------------------------------------------------------------------------------
inline void SPEffect::SetFXSoundInfo(const EFFECT_FX_SOUND& Src)
{
	m_FXSoundInfo = Src;
}

//------------------------------------------------------------------------------------
inline void SPEffect::SetRequireInfo(const EFFECT_REQUIRE& Src)
{
	m_RequireInfo = Src;
}

//------------------------------------------------------------------------------------
inline void SPEffect::SetStatusInfo(const EFFECT_STATUS& Src)
{
	m_StatusInfo = Src;
}

//------------------------------------------------------------------------------------
inline void SPEffect::SetAttachFXGroup( int iAttachFXGroup )
{
	m_iAttachFXGroup	=	iAttachFXGroup;
}

//------------------------------------------------------------------------------------
inline void SPEffect::SetTransparency(int iTransparency)
{
	m_iTransparency = iTransparency;
}

//------------------------------------------------------------------------------------
inline void SPEffect::SetName(const char* pszName)
{
	if( pszName == NULL )
		return;

	m_strName	=	pszName;
}

//------------------------------------------------------------------------------------
inline void SPEffect::SetDescription(const char* pszDescription)
{
	if( pszDescription == NULL || (int)strlen(pszDescription) == 0 )
		return;

	m_strDescription = pszDescription;
}

//------------------------------------------------------------------------------------
inline int SPEffect::GetID()
{
	return m_iID;
}

//------------------------------------------------------------------------------------
inline EFFECT_TYPE SPEffect::GetType()
{
	return m_eType;
}

//------------------------------------------------------------------------------------
inline float SPEffect::GetApplyTime()
{
	return m_fApplyTime;
}

//------------------------------------------------------------------------------------
inline float SPEffect::GetApplyTimeElv()
{
	return m_fApplyTimeElv;
}

//------------------------------------------------------------------------------------
inline EFFECT_STATUS* SPEffect::GetStatusInfo()
{
	return &m_StatusInfo;
}

//------------------------------------------------------------------------------------
inline EFFECT_MISSILE* SPEffect::GetMissileInfo()
{
	return &m_MissileInfo;
}

//------------------------------------------------------------------------------------
inline EFFECT_FX_SOUND* SPEffect::GetFXSoundInfo()
{
	return &m_FXSoundInfo;
}

//------------------------------------------------------------------------------------
inline EFFECT_BUFF* SPEffect::GetBuff()
{
	return &m_BuffInfo;
}

//------------------------------------------------------------------------------------
inline EFFECT_REQUIRE* SPEffect::GetRequireInfo()
{
	return &m_RequireInfo;
}

//------------------------------------------------------------------------------------
inline int SPEffect::GetAttachFXGroup()
{
	return m_iAttachFXGroup;
}

//------------------------------------------------------------------------------------
inline int SPEffect::GetTransparency()
{
	return m_iTransparency;
}

//------------------------------------------------------------------------------------
inline const char* SPEffect::GetName()
{
	return m_strName.c_str();
}

//------------------------------------------------------------------------------------
inline const char* SPEffect::GetDescription()
{
	if( m_strDescription.empty() )
		return NULL;

	return m_strDescription.c_str();
}
