// Copyright (C) metalgeni
//***************************************************************************
// System Name : 
// Comment     : 주의 !: lItemID는 0이 아니라 1부터 시작한다. 
//					그리고, DB에 따라 항상 1,2,3 은 아님. 하지만 DB 내에선 Unique한 숫자임.
// Modified    : metalgeni 2004-08-29 오후 2:32:54 
//***************************************************************************


#pragma once

#include <vector>
#include <hash_map>
#include <string>

// .NET2003 전용 해시맵 사용때문에.. 
#ifdef _DEFINE_DEPRECATED_HASH_CLASSES
#undef _DEFINE_DEPRECATED_HASH_CLASSES 
#define _DEFINE_DEPRECATED_HASH_CLASSES 0	
#endif

#pragma warning(disable:4786)	// vector warning disable

using namespace std;
using namespace stdext;

class SPLDTManager;

class SPStream;

enum LDTfldTypes {
	fldNA = 0,
	fldString = 1,
	fldTF = 2,
	fldNum = 3,
	fldPer = 4,
	fldFID = 5,
	fldAlias = 6,
	fldNum64 = 7,
	fldPrimaryKey=255
};

struct LDT_STR64 {
	char	String[64];
};

struct LDT_Field {
	void Init() {
		pName=NULL;
		nSize=0;
		nFldTyp=0;
		uData.lValue=0;
	}
	char*		pName;
	long        nFldTyp;
	union
	{
		char*		pValue;
		long		lValue;
		float		fValue;
		__int64*	pValue64;		  
	} uData;
	int	nSize;
};

struct LDT_Header {
	long		nDB_ID;
	long		nFields;
	long		nData;
	LDT_STR64	FieldNAM[128];
	long		FieldTYP[128];
};

class SPLDTFile
{
public:	
	virtual ~SPLDTFile();	
	
	void		Clean();	

	// item ID와 field 인덱스를 넣으면 field 구조체를 세팅해준다
	bool		GetField(long lItemID, long lFieldNum, LDT_Field& rLDTField);
	// 해당 Field에 Data가 있는지 여부 파악
	bool		CheckLDTField(LDT_Field& stField) ;

	// item ID와 field label을 넣으면 field 구조체를 세팅해준다
	bool		GetFieldFromLable(long lItemID, const char* cFieldLabel, LDT_Field& rLDTField);					

	long		GetItemCount() { return (long)(m_LDTprimaryidx.size()); }
	long		GetFieldCount() { return m_nFieldCount; }
	const char* GetFieldLabel( long lField ) { return m_Label[lField].String; }	
	long		GetFieldNum( const char* pszFieldLabel);

	// 레코드 인덱스로부터 그 레코드의 primary key (lItemID)를 얻는다.
	long		GetPrimaryKey(long lIndex); 				

	// 해당 primary key (lItemID) 의 레코드 인덱스를 되돌린다. //lPrimaryKey == ItemID이다
	long		GetIDXprimary(long lPrimaryKey);			

	long		GetDB_ID()				{ return m_lDB_ID; }
	const char* GetFileName()	{ return m_strFileName.c_str(); }
	void		Release();

	// SAVE Interface 추가 [200611:duragon]
	bool		SaveLDTFile( LPCSTR pszLDTFile);			///< LDT File로 저장
	bool		SaveToTXT(const char* pszFileName) ;		///< TXT File로 저장
	bool		PushBack(std::string& strLDTField);			///< Data Add --> type : size : data (순서엄수!!)

protected:	
	friend SPLDTManager;	
	
	// 생성은 항상 SPLDTManager에서다.
	SPLDTFile();
	bool		LoadLDTFile( LPCSTR pszLDTFile);		//SPLDTManager만 호출해야함.
	bool		LoadLDTFile( SPStream* pLDTStream);		//SPLDTManager만 호출해야함.
	
	int			GetRefCount();	
	void		IncreaseRef();

	bool		SaveLDTFile( SPStream* pLDTStream);		//[200611:duragon]SPLDTManager만 호출해야함.
														// 일단은 Stream부분만 Protect로
	

private:

	vector<LDT_Field>		m_LDTLinearDB;		// DB
	hash_map<long, long>	m_LDTprimaryidx;	// Item ID to Index
	vector<LDT_STR64>		m_Label;			// 구분자

	hash_map<std::string, long> m_hmLabel;	

	long m_lDB_ID;
	long m_nFieldCount;
	long m_nDataCount;

	string m_strFileName;

	int m_iRefCount;
	bool m_bIsLoad ;			///< LDT파일이 로딩되어 있는지 여부 (최소 Header가 있어야 함)

};




