//*********************************************************************
//			概要	：バイナリファイルを扱うクラス
// 
//			制作日	：1月15日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once

#include <memory>
class BinaryFile
{
protected:

	// データ
	std::unique_ptr<char[]> m_data;

	// サイズ
	unsigned int m_size;

public:

	// ファイル名を指定してロード
	static BinaryFile LoadFile(const wchar_t* fileName);

	BinaryFile();

	// ムーブコンストラクタ
	BinaryFile(BinaryFile&& in);

	// アクセサ
	char* GetData() { return m_data.get(); }
	unsigned int GetSize() { return m_size; }
};
