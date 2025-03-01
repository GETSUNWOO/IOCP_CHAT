#pragma once
#include "Types.h"
#include <windows.h>
#include "DBBind.h"

namespace SP
{
	
    class InsertAccount : public DBBind<4,0>
    {
    public:
    	InsertAccount(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spInsertAccount(?,?,?,?)}") { }
    	void In_Type(int32& v) { BindParam(0, v); };
    	void In_Type(int32&& v) { _type = std::move(v); BindParam(0, _type); };
    	template<int32 N> void In_Name(WCHAR(&v)[N]) { BindParam(1, v); };
    	template<int32 N> void In_Name(const WCHAR(&v)[N]) { BindParam(1, v); };
    	void In_Name(WCHAR* v, int32 count) { BindParam(1, v, count); };
    	void In_Name(const WCHAR* v, int32 count) { BindParam(1, v, count); };
    	template<int32 N> void In_Password(WCHAR(&v)[N]) { BindParam(2, v); };
    	template<int32 N> void In_Password(const WCHAR(&v)[N]) { BindParam(2, v); };
    	void In_Password(WCHAR* v, int32 count) { BindParam(2, v, count); };
    	void In_Password(const WCHAR* v, int32 count) { BindParam(2, v, count); };
    	void In_CreateDate(TIMESTAMP_STRUCT& v) { BindParam(3, v); };
    	void In_CreateDate(TIMESTAMP_STRUCT&& v) { _createDate = std::move(v); BindParam(3, _createDate); };

    private:
    	int32 _type = {};
    	TIMESTAMP_STRUCT _createDate = {};
    };

    class GetAccount : public DBBind<1,5>
    {
    public:
    	GetAccount(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spGetAccount(?)}") { }
    	template<int32 N> void In_Name(WCHAR(&v)[N]) { BindParam(0, v); };
    	template<int32 N> void In_Name(const WCHAR(&v)[N]) { BindParam(0, v); };
    	void In_Name(WCHAR* v, int32 count) { BindParam(0, v, count); };
    	void In_Name(const WCHAR* v, int32 count) { BindParam(0, v, count); };
    	void Out_Id(OUT int32& v) { BindCol(0, v); };
    	void Out_Type(OUT int32& v) { BindCol(1, v); };
    	template<int32 N> void Out_Name(OUT WCHAR(&v)[N]) { BindCol(2, v); };
    	template<int32 N> void Out_Password(OUT WCHAR(&v)[N]) { BindCol(3, v); };
    	void Out_CreateDate(OUT TIMESTAMP_STRUCT& v) { BindCol(4, v); };

    private:
    };


     
};