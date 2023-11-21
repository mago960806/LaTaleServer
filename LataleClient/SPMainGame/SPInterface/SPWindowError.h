#ifndef __SPWINDOW_ERROR_H__
#define __SPWINDOW_ERROR_H__

#define		WIID_ERROR_SKIN_UPPER			(WIID_ERROR + 1)
#define		WIID_ERROR_SKIN_MIDDLE			(WIID_ERROR + 2)
#define		WIID_ERROR_SKIN_LOWER			(WIID_ERROR + 3)
#define		WIID_ERROR_OK					(WIID_ERROR + 4)

class SPWindowError : public SPWindow
{
public:
	SPWindowError(WND_ID WndClassID, INSTANCE_ID InstanceID);
	SPWindowError(INSTANCE_ID InstanceID, int iX, int iY, int iCX, int iCY, SPWindow* pParent = NULL);

	virtual ~SPWindowError();

	virtual void Init();
	virtual void Clean();

	virtual void Process(float fTime);
	virtual void Render(float fTime);

	virtual void Show(bool bWithChild = true);	

protected:
	SPIMESSAGE_OBJ_MAP_DECLARE()

	virtual int OnErrorMsg(WPARAM, LPARAM);
	virtual int OnErrorOk(WPARAM, LPARAM);

	void SetErrorMsg(const char* pszErrorMsg, int iType);

protected:
	int m_iErrorType;
	std::vector< std::string > m_vErrorMsg;
};

#endif