#pragma once

#include <windows.h>
#include <string>
#include <vector>

// FMF�t�@�C���w�b�_ (20 bytes)
typedef struct tag_FMFHeader{
	DWORD dwIdentifier;// �t�@�C�����ʎq 'FMF_'
	DWORD dwSize;      // �w�b�_���������f�[�^�T�C�Y
	DWORD dwWidth;     // �}�b�v�̉���
	DWORD dwHeight;    // �}�b�v�̍���
	BYTE  byChipWidth; // �}�b�v�`�b�v1�̕�(pixel)
	BYTE  byChipHeight;// �}�b�v�`�b�v�P�̍���(pixel)
	BYTE  byLayerCount;// ���C���[�̐�
	BYTE  byBitCount;  // ���C���f�[�^�̃r�b�g�J�E���g
}FMF_HEADER;

class FmfFile{
	FMF_HEADER fmfHeader;// File���^���
	std::vector<BYTE> data;// ���C���[�f�[�^
public:
	FmfFile();
	~FmfFile();

	void LoadData(std::string fileName);

	FMF_HEADER GetInfo();
	BYTE GetValue(BYTE layer, DWORD x, DWORD y);
};