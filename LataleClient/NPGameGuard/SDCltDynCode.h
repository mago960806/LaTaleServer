#ifndef SDCLTDYNCODE_H
#define SDCLTDYNCODE_H

#pragma once

#ifndef SDAPI
#define SDAPI   __stdcall
#endif

/* ====================================================================
 * ISDCltDynCode: client side dynamic code component. ISDCltDynCode provides
 * dynamic encrypt and decrypt methods.
 *
 * Usage: 
 * 1. Create ISDCltDynCode component, call SDCreateCltDynCode
 * 2. Call ISDCltDynCode.SetDynCode to when getting dynamic code from server
 * 3. To encrypt: call ISDCltDynCode::Encode
 * 4. To decrypt: call ISDCltDynCode::Decode
 * 5. when finish, call ISDCltDynCode::Release to release the component
 */

#define SDUPDATE_SYNC true
#define SDUPDATE_ASYNC false
#define SDUPDATE_ERROR 0
#define SDUPDATE_OK 1
#define SDUPDATE_RUNNING 2

namespace SGDP{

class ISDCltDynCode
{
public:
    //Function: set dynamic code
    //Return: true if successful
    //Remark: this will overwrite the dynamic doe set before
    virtual bool SDAPI SetDynCode(const unsigned char* lpDynCode, int nLen) = 0;

    //Function: encrypt the data, it will return false if called without setting dynamic code
    //Parameter: lpData, the original data, it is used as a [in, out] parameter
    //		       nLen, the data length, it will not change after encryption
    //Return: true if successful
    virtual bool SDAPI Encode(unsigned char * lpData, unsigned long nLen) = 0;

    //Function: decrypt the data, it will return false if called without setting dynamic code
    //Parameter: lpData, the original data to be decrypted, it is used as a [in, out] parameter
    //		       nLen, the data length, it will not change after decryption
    //Return: true if successful
    virtual bool SDAPI Decode(unsigned char * lpData, unsigned long nLen) = 0;

	//Function: Release the component
    virtual void SDAPI Release() = 0;
};

//Function: Create ISDCltDynCode component
ISDCltDynCode * SDAPI SDCreateCltDynCode();

//Function: 检查更新客户端组件,
//Parameter:szURL,更新文件存放的IP地址或者域名。可以是http或者ftp方式
//			bsync,是否同步，SDUPDATE_SYNC为同步，SDUPDATE_ASYNC为异步，为防止阻塞建议用SDUPDATE_ASYNC
//Return:true if successful.
bool SDAPI SDDynCodeCheckUpdate(const char* szUrl,bool bsync);


//Function:检查更新状态
//Return:SDUPDATE_OK,SDUPDATE_ERROR,SDUPDATE_RUNNING
int SDAPI GetSDDynStatus();

}

#endif
