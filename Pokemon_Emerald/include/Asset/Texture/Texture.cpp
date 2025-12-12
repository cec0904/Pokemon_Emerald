#include "Texture.h"
#include "../../Device.h"

#ifdef _DEBUG
#pragma comment(lib, "../Bin/DirectXTex_Debug.lib")
#else
#pragma comment(lib, "../Bin/DirectXTex.lib")
#endif





CTexture::CTexture()
{
	mAssetType = EAssetType::Texture;
}

CTexture::~CTexture()
{
	//20251120 추가
	size_t Size = mTextureList.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(mTextureList[i]);
	}
}

bool CTexture::LoadTexture(const TCHAR* FileName)
{
	TCHAR FullPath[MAX_PATH] = {};
	//문자열 복사
	//strcpy
	lstrcpy(FullPath, gRootPath);
	//문자열 합치기
	//strcat
	lstrcat(FullPath, TEXT("Asset\\"));
	lstrcat(FullPath, FileName);

	return LoadTextureFullPath(FullPath);
}

bool CTexture::LoadTextureFullPath(const TCHAR* FullPath)
{
	// D:\KDT4\KDT4FrameWork\KDT4Framework\KDT4Framework\Bin\Asset\Texture\AA.png
	// Texture\AA.png

	FTextureInfo* Texture = new FTextureInfo;
	
	int Length = lstrlen(FullPath);

	// 파일 이름 가져오기
	for (int i = 0; i< Length; ++i)
	{
		if (gRootPath[i] != FullPath[i])
		{
			lstrcpy(Texture->FileName, &FullPath[i]);
			break;
		}
	}

	// 이미지 로드하기
	// 확장자로 이미지 불러오는게 달라진다.
	// 크게 DDS, TGA, 나머지(PNG, JPEG 등등 )

	// 파일 확장자를 구분하여 가져온다.
	TCHAR FileExt[_MAX_EXT] = {};

	_wsplitpath_s(FullPath,
		nullptr, 0,				//드라이브
		nullptr, 0,					//디렉토리
		nullptr, 0,				// 파일 이름
		FileExt, _MAX_EXT			//확장자
	);

	//대문자로 변경할려면 유니코드를 멀티바이트 문자열로 변환해주는 작업이 필요하다 .
	// 대문자로 만들어주는 함수 _strupr_s 멀티바이트 문자열만 받기때문에
	char ConvertExt[_MAX_EXT] = {};

	//변환에 필요한 버퍼크기를 계산
	int Count = WideCharToMultiByte(CP_ACP, 0, FileExt, -1, nullptr, 0, nullptr, nullptr);

	WideCharToMultiByte(CP_ACP, 0, FileExt, -1, ConvertExt, Count, nullptr, nullptr);

	// 확장자를 대문자로 통일시켜주다. 
	_strupr_s(ConvertExt);

	DirectX::ScratchImage* Image = new DirectX::ScratchImage;

	if (strcmp(ConvertExt, ".DDS") == 0)
	{
		if (FAILED(DirectX::LoadFromDDSFile(FullPath, DirectX::DDS_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Texture);
			SAFE_DELETE(Image);
			return false;
		}
	}
	else if (strcmp(ConvertExt, ".TGA") == 0)
	{
		if (FAILED(DirectX::LoadFromTGAFile(FullPath, nullptr, *Image)))
		{
			SAFE_DELETE(Texture);
			SAFE_DELETE(Image);
			return false;
		}
	}
	else
	{
		//PNG, JPG BMP 
		if (FAILED(DirectX::LoadFromWICFile(FullPath, DirectX::WIC_FLAGS_NONE, nullptr, *Image)))
		{
			SAFE_DELETE(Texture);
			SAFE_DELETE(Image);
			return false;
		}
	}

	Texture->Image = Image;
	Texture->Image->GetImages()->format;
	int RSVNumber = (int)mTextureList.size();

	mTextureList.emplace_back(Texture);
	//여기까지해서 이미지 정보를 불러왔다.

	

	return CreateResourceView(RSVNumber);
}

bool CTexture::LoadTexture(const vector<const TCHAR*>& FileName)
{
	size_t Size = FileName.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!LoadTexture(FileName[i]))
		{
			return false;
		}
	}

	return true;
}

bool CTexture::LoadTextureFullPath(const vector<const TCHAR*>& FullPath)
{
	size_t Size = FullPath.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (!LoadTextureFullPath(FullPath[i]))
		{
			return false;
		}
	}

	return true;

}

bool CTexture::LoadTexture(const TCHAR* FileName, const TCHAR* Ext, int Count)
{
	for (int i = 0 ; i< Count; ++i)
	{
		//파일이름 인덱스 . 확장자
		//Frame0.PNG
		TCHAR _FileName[MAX_PATH] = {};

		wsprintf(_FileName, TEXT("%s%d.%s"), FileName, i, Ext);

		if (!LoadTexture(_FileName))
		{
			return false;
		}
	}
	return true;
}

void CTexture::SetShader(int Register, int ShaderBufferType, int TextureIndex)
{
	// 1인자 : 레지스터 번호 
	// 2인자 : 몇개의 버퍼가 셋팅되어있는지
	// 3인자 : 셋팅할 버퍼의 주소

	if (ShaderBufferType & EShaderBufferType::Vertex)
	{

		CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);
	}
	//픽셀
	if (ShaderBufferType & EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);
	}
	//HULL
	if (ShaderBufferType & EShaderBufferType::Hull)
	{

		CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);
	}
	//Domain
	if (ShaderBufferType & EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);
	}
	//Geometry
	if (ShaderBufferType & EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);
	}

	//Comput
	if (ShaderBufferType & EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &mTextureList[TextureIndex]->SRV);
	}

}

void CTexture::ResetShader(int Register, int ShaderBufferType)
{
	ID3D11ShaderResourceView* SRV = nullptr;

	if (ShaderBufferType & EShaderBufferType::Vertex)
	{


		CDevice::GetInst()->GetContext()->VSSetShaderResources(Register, 1, &SRV);
	}
	//픽셀
	if (ShaderBufferType & EShaderBufferType::Pixel)
	{
		CDevice::GetInst()->GetContext()->PSSetShaderResources(Register, 1, &SRV);
	}
	//HULL
	if (ShaderBufferType & EShaderBufferType::Hull)
	{

		CDevice::GetInst()->GetContext()->HSSetShaderResources(Register, 1, &SRV);
	}
	//Domain
	if (ShaderBufferType & EShaderBufferType::Domain)
	{
		CDevice::GetInst()->GetContext()->DSSetShaderResources(Register, 1, &SRV);
	}
	//Geometry
	if (ShaderBufferType & EShaderBufferType::Geometry)
	{
		CDevice::GetInst()->GetContext()->GSSetShaderResources(Register, 1, &SRV);
	}

	//Comput
	if (ShaderBufferType & EShaderBufferType::Compute)
	{
		CDevice::GetInst()->GetContext()->CSSetShaderResources(Register, 1, &SRV);
	}
}

bool CTexture::CreateResourceView(int Index)
{
	if (FAILED(DirectX::CreateShaderResourceView(
		CDevice::GetInst()->GetDevice(),
		mTextureList[Index]->Image->GetImages(),
		mTextureList[Index]->Image->GetImageCount(),
		mTextureList[Index]->Image->GetMetadata(),
		&mTextureList[Index]->SRV)))
	{
		return false;
	}
	mTextureList[Index]->Width = static_cast<unsigned int>(mTextureList[Index]->Image->GetImages()[0].width);

	mTextureList[Index]->Height = static_cast<unsigned int>(mTextureList[Index]->Image->GetImages()[0].height);

	return true;
}
