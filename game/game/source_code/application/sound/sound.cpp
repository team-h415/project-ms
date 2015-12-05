//================================================================================
// �T�E���h���� [sound.cpp]
// Author : TAKESI IWASAWA
//================================================================================

//================================================================================
// �C���N���[�h
//================================================================================
#include "Sound.h"
#include <tchar.h>

//================================================================================
// �}�N����`
//================================================================================
//#define _SOUND_OFF_

//================================================================================
// �ÓI�ϐ����̉�
//================================================================================
IXAudio2*					Sound::x_audio2_ = nullptr;					// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice*		Sound::mastering_voice_ = nullptr;			// �}�X�^�[�{�C�X
vector<Sound*>				Sound::sound_array_;						// �T�E���h�N���X�|�C���^�z��
int							Sound::count_sound_ = 0;					// �T�E���h�N���X�J�E���g

//================================================================================
// �T�E���h�g�p�̏���
//================================================================================
void Sound::Setup(void)
{
	if(x_audio2_ != nullptr && mastering_voice_ != nullptr)
	{
		MessageBox(nullptr, _T("���łɃT�E���h�̏��������Ă��܂��I"), _T("�x���I"), MB_ICONWARNING);
		return;
	}
	// ��Ԋm�F�p�ϐ�
	HRESULT hr;
	// COM���C�u�����̏�����
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&x_audio2_, 0);
	if(FAILED(hr))
	{
		MessageBox(nullptr, _T("XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I"), _T("�x���I"), MB_ICONWARNING);
		// COM���C�u�����̏I������
		CoUninitialize();
		return;
	}
	// �}�X�^�[�{�C�X�̐���
	hr = x_audio2_->CreateMasteringVoice(&mastering_voice_);
	if(FAILED(hr))
	{
		MessageBox(nullptr, _T("�}�X�^�[�{�C�X�̐����Ɏ��s�I"), _T("�x���I"), MB_ICONWARNING);
		// XAudio2�I�u�W�F�N�g�̊J��
		if(x_audio2_)
		{
			x_audio2_->Release();
			x_audio2_ = nullptr;
		}
		// COM���C�u�����̏I������
		CoUninitialize();
		return;
	}
}

//================================================================================
// �T�E���h�̑S�J��
//================================================================================
void Sound::End(void)
{
	// �S�T�E���h���
	int max = sound_array_.size();
	for(int loop = 0; loop < max; loop++)
	{
		if(sound_array_[loop] != nullptr)
		{
			delete sound_array_[loop];
			sound_array_[loop] = nullptr;
		}
	}
	sound_array_.clear();
	// �}�X�^�[�{�C�X�̔j��
	if(mastering_voice_ != nullptr)
	{
		mastering_voice_->DestroyVoice();
		mastering_voice_ = nullptr;
	}
	// XAudio2�I�u�W�F�N�g�̊J��
	if(x_audio2_ != nullptr)
	{
		x_audio2_->Release();
		x_audio2_ = nullptr;
	}
	// COM���C�u�����̏I������
	CoUninitialize();
}

//================================================================================
// �T�E���h�̃��[�h
//================================================================================
Sound* Sound::LoadSound(const char* sound_path, const float set_max)
{
	Sound* pNewSound = nullptr;
	pNewSound = new Sound(sound_path);
	pNewSound->SetMaxVolume(set_max);
	pNewSound->master_flag_ = false;
	sound_array_.push_back(pNewSound);
	return pNewSound;
}

//================================================================================
// �T�E���h�̃��[�h�ƍĐ�
//================================================================================
void Sound::LoadAndPlaySE(const char* sound_path, float set_volume)
{
	Sound* pNewSound = nullptr;
	pNewSound = new Sound(sound_path);
	pNewSound->SetMaxVolume(set_volume);
	pNewSound->SetCurrentVolume(set_volume);
	pNewSound->master_flag_ = true;
	pNewSound->Play(false);
	sound_array_.push_back(pNewSound);
}

//================================================================================
// �T�E���h�̉��
//================================================================================
void Sound::ReleaseSound(Sound** dp_sound, int fade_time)
{
	if(dp_sound == nullptr)
	{
		return;
	}
	if((*dp_sound) == nullptr)
	{
		return;
	}
	if(fade_time <= 0)
	{
		(*dp_sound)->fade_flag_ = false;						// �t�F�[�h
		(*dp_sound)->death_flag_ = true;						// �f�X
	}else
	{
		(*dp_sound)->fade_flag_ = true;						// �t�F�[�h
		(*dp_sound)->death_flag_ = true;						// �f�X
		(*dp_sound)->fade_power_ = -((*dp_sound)->GetCurrentVolume() / static_cast<float>(fade_time));	// �t�F�[�h�p���[
	}
	// �|�C���^��nullptr��
	(*dp_sound) = nullptr;
}

//================================================================================
// �T�E���h�̑S���
//================================================================================
void Sound::ReleaseAll(int fade_time)
{
	for(int loop = 0; loop < count_sound_; loop++)
	{
		if(sound_array_[loop] == nullptr)
		{
			continue;
		}
		// ������@�`�F�b�N
		if(fade_time > 0)
		{
			sound_array_[loop]->fade_flag_ = false;						// �t�F�[�h
			sound_array_[loop]->death_flag_ = true;						// �f�X
		}else
		{
			sound_array_[loop]->fade_flag_ = true;						// �t�F�[�h
			sound_array_[loop]->death_flag_ = true;						// �f�X
			sound_array_[loop]->fade_power_ = -(sound_array_[loop]->GetCurrentVolume() / static_cast<float>(fade_time));	// �t�F�[�h�p���[
		}
	}
}

//================================================================================
// �T�E���h�̍Đ�
//================================================================================
void Sound::Play(bool loop_flag, int fade_time)
{
	// �f�o�b�O�p�T�E���hOFF
	#ifdef _DEBUG
		#ifdef _SOUND_OFF_
			return;
		#endif
	#endif
	// �t�F�[�h�`�F�b�N
	if(fade_time > 0)
	{
		// ���݂̃{�����[�����O��
		SetCurrentVolume(0.0f);
		// �t�F�[�h�p���[�ݒ�
		fade_power_ = max_volume_ / static_cast<float>(fade_time);
		// �t�F�[�h�t���OON
		fade_flag_ = true;
	}else
	{
		// ���݂̃{�����[�����ő��
		SetCurrentVolume(1.0f);
		// �t�F�[�h�t���OOFF
		fade_flag_ = false;
	}
	// �Đ�����
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = size_audio_;
	buffer.pAudioData = data_audio_;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	// ���[�v�ݒ�
	buffer.LoopCount = (loop_flag)? XAUDIO2_LOOP_INFINITE:0;
	// ��Ԏ擾
	source_voice_->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{
		// �ꎞ��~
		source_voice_->Stop(0);
		// �I�[�f�B�I�o�b�t�@�̍폜
		source_voice_->FlushSourceBuffers();
	}
	// �I�[�f�B�I�o�b�t�@�̓o�^
	source_voice_->SubmitSourceBuffer(&buffer);
	// �Đ�
	source_voice_->Start(0);
}

//================================================================================
// �Z�O�����g��~
//================================================================================
void Sound::Stop(int fade_time)
{
	// ��~����
	XAUDIO2_VOICE_STATE xa2state;
	source_voice_->GetState(&xa2state);
	// �Đ���ԃ`�F�b�N
	if(xa2state.BuffersQueued != 0)
	{
		if(fade_time > 0)
		{
			// �t�F�[�hON
			fade_flag_ = true;
			// �t�F�[�h�p���[�ݒ�
			fade_power_ = -(GetCurrentVolume() / static_cast<float>(fade_time));
		}else
		{
			// �ꎞ��~
			source_voice_->Stop(0);
			// �I�[�f�B�I�o�b�t�@�̍폜
			source_voice_->FlushSourceBuffers();
		}
	}
}

//================================================================================
// �S�T�E���h�̍X�V
//================================================================================
void Sound::UpdateAll(void)
{
	int max = sound_array_.size();
	for(int loop = 0; loop < max; loop++)
	{
		if(sound_array_[loop] == nullptr)
		{
			continue;
		}
		sound_array_[loop]->Update();
		// ����m�F
		if(sound_array_[loop]->death_flag_ && !sound_array_[loop]->fade_flag_)
		{
			delete sound_array_[loop];
			sound_array_.erase(sound_array_.begin() + loop);
			loop--;
			max--;
		}
	}
}

//================================================================================
// �T�E���h�̍X�V
//================================================================================
void Sound::Update(void)
{
	// �t�F�[�h�`�F�b�N
	if(fade_flag_)
	{
		float newVolume = GetCurrentVolume();
		newVolume += fade_power_;
		if(newVolume <= 0.0f || newVolume >= max_volume_)
		{
			fade_flag_ = false;
		}
		SetCurrentVolume(newVolume);
	}
	// �P��I���m�F
	if(master_flag_)
	{
		XAUDIO2_VOICE_STATE xa2state;
		source_voice_->GetState(&xa2state);
		// �Đ���ԃ`�F�b�N
		if(xa2state.BuffersQueued == 0)
		{
			death_flag_ = true;
			fade_flag_ = false;
		}
	}
	// �f�o�b�O�p�T�E���hOFF
	#ifdef _DEBUG
		#ifdef _SOUND_OFF_
			SetCurrentVolume(0.0f);
		#endif
	#endif
}

//================================================================================
// �T�E���h�̃R���X�g���N�^
//================================================================================
Sound::Sound(const char* pName)
{
	// �e��ϐ�������
	source_voice_ = nullptr;			// �\�[�X�{�C�X
	data_audio_ = nullptr;				// �I�[�f�B�I�f�[�^
	size_audio_ = 0;					// �I�[�f�B�I�f�[�^�T�C�Y
	fade_flag_ = false;					// �t�F�[�h
	fade_power_ = 0.0f;				// �t�F�[�h�p���[
	death_flag_ = false;					// �f�X

	HANDLE file_handle;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̃N���A
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// ��Ԋm�F�p�ϐ�
	HRESULT hr;

	// �T�E���h�f�[�^�t�@�C���̐���
	file_handle = CreateFile(pName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if(file_handle == INVALID_HANDLE_VALUE)
	{
		//MessageBox(nullptr, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return;
	}
	if(SetFilePointer(file_handle, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// �t�@�C���|�C���^��擪�Ɉړ�
		//MessageBox(nullptr, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return;
	}
	
	// WAVE�t�@�C���̃`�F�b�N
	hr = CheckChunk(file_handle, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return;
	}
	hr = ReadChunkData(file_handle, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return;
	}
	if(dwFiletype != 'EVAW')
	{
		//MessageBox(nullptr, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
		return;
	}
	
	// �t�H�[�}�b�g�`�F�b�N
	hr = CheckChunk(file_handle, ' tmf', &dwChunkSize, &dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return;
	}
	hr = ReadChunkData(file_handle, &wfx, dwChunkSize, dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return;
	}

	// �I�[�f�B�I�f�[�^�ǂݍ���
	hr = CheckChunk(file_handle, 'atad', &size_audio_, &dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
		return;
	}
	data_audio_ = (BYTE*)malloc(size_audio_);
	hr = ReadChunkData(file_handle, data_audio_, size_audio_, dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
		return;
	}
	
	// �\�[�X�{�C�X�̐���
	hr = x_audio2_->CreateSourceVoice(&source_voice_, &(wfx.Format));
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
		return;
	}

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = size_audio_;
	buffer.pAudioData = data_audio_;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	// �I�[�f�B�I�o�b�t�@�̓o�^
	source_voice_->SubmitSourceBuffer(&buffer);

	count_sound_++;
}

//================================================================================
// �T�E���h�̃f�X�g���N�^
//================================================================================
Sound::~Sound(void)
{
	if(source_voice_)
	{
		// �ꎞ��~
		source_voice_->Stop(0);
		// �\�[�X�{�C�X�̔j��
		source_voice_->DestroyVoice();
		source_voice_ = nullptr;
		// �I�[�f�B�I�f�[�^�̊J��
		free(data_audio_);
		data_audio_ = nullptr;
	}

	count_sound_--;
}

//================================================================================
// �`�����N�`�F�b�N
//================================================================================
HRESULT Sound::CheckChunk(HANDLE file_handle, DWORD format, DWORD *chunk_size, DWORD *chunk_data_position)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(file_handle, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(file_handle, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{
			// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(file_handle, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{
			// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(file_handle, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{
				// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(file_handle, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*chunk_size = dwChunkDataSize;
			*chunk_data_position = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//================================================================================
// �`�����N�ǂݍ���
//================================================================================
HRESULT Sound::ReadChunkData(HANDLE file_handle, void *pBuffer, DWORD dw_buffersize, DWORD dw_bufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(file_handle, dw_bufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(file_handle, pBuffer, dw_buffersize, &dwRead, nullptr) == 0)
	{
		// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}