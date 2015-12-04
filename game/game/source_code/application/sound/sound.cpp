//================================================================================
// サウンド処理 [sound.cpp]
// Author : TAKESI IWASAWA
//================================================================================

//================================================================================
// インクルード
//================================================================================
#include "Sound.h"
#include <tchar.h>

//================================================================================
// マクロ定義
//================================================================================
//#define _SOUND_OFF_

//================================================================================
// 静的変数実体化
//================================================================================
IXAudio2*					Sound::x_audio2_ = nullptr;					// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice*		Sound::mastering_voice_ = nullptr;			// マスターボイス
vector<Sound*>				Sound::sound_array_;						// サウンドクラスポインタ配列
int							Sound::count_sound_ = 0;					// サウンドクラスカウント

//================================================================================
// サウンド使用の準備
//================================================================================
void Sound::Setup(void)
{
	if(x_audio2_ != nullptr && mastering_voice_ != nullptr)
	{
		MessageBox(nullptr, _T("すでにサウンドの準備をしています！"), _T("警告！"), MB_ICONWARNING);
		return;
	}
	// 状態確認用変数
	HRESULT hr;
	// COMライブラリの初期化
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&x_audio2_, 0);
	if(FAILED(hr))
	{
		MessageBox(nullptr, _T("XAudio2オブジェクトの作成に失敗！"), _T("警告！"), MB_ICONWARNING);
		// COMライブラリの終了処理
		CoUninitialize();
		return;
	}
	// マスターボイスの生成
	hr = x_audio2_->CreateMasteringVoice(&mastering_voice_);
	if(FAILED(hr))
	{
		MessageBox(nullptr, _T("マスターボイスの生成に失敗！"), _T("警告！"), MB_ICONWARNING);
		// XAudio2オブジェクトの開放
		if(x_audio2_)
		{
			x_audio2_->Release();
			x_audio2_ = nullptr;
		}
		// COMライブラリの終了処理
		CoUninitialize();
		return;
	}
}

//================================================================================
// サウンドの全開放
//================================================================================
void Sound::End(void)
{
	// 全サウンド解放
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
	// マスターボイスの破棄
	if(mastering_voice_ != nullptr)
	{
		mastering_voice_->DestroyVoice();
		mastering_voice_ = nullptr;
	}
	// XAudio2オブジェクトの開放
	if(x_audio2_ != nullptr)
	{
		x_audio2_->Release();
		x_audio2_ = nullptr;
	}
	// COMライブラリの終了処理
	CoUninitialize();
}

//================================================================================
// サウンドのロード
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
// サウンドのロードと再生
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
// サウンドの解放
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
		(*dp_sound)->fade_flag_ = false;						// フェード
		(*dp_sound)->death_flag_ = true;						// デス
	}else
	{
		(*dp_sound)->fade_flag_ = true;						// フェード
		(*dp_sound)->death_flag_ = true;						// デス
		(*dp_sound)->fade_power_ = -((*dp_sound)->GetCurrentVolume() / static_cast<float>(fade_time));	// フェードパワー
	}
	// ポインタをnullptr化
	(*dp_sound) = nullptr;
}

//================================================================================
// サウンドの全解放
//================================================================================
void Sound::ReleaseAll(int fade_time)
{
	for(int loop = 0; loop < count_sound_; loop++)
	{
		if(sound_array_[loop] == nullptr)
		{
			continue;
		}
		// 解放方法チェック
		if(fade_time > 0)
		{
			sound_array_[loop]->fade_flag_ = false;						// フェード
			sound_array_[loop]->death_flag_ = true;						// デス
		}else
		{
			sound_array_[loop]->fade_flag_ = true;						// フェード
			sound_array_[loop]->death_flag_ = true;						// デス
			sound_array_[loop]->fade_power_ = -(sound_array_[loop]->GetCurrentVolume() / static_cast<float>(fade_time));	// フェードパワー
		}
	}
}

//================================================================================
// サウンドの再生
//================================================================================
void Sound::Play(bool loop_flag, int fade_time)
{
	// デバッグ用サウンドOFF
	#ifdef _DEBUG
		#ifdef _SOUND_OFF_
			return;
		#endif
	#endif
	// フェードチェック
	if(fade_time > 0)
	{
		// 現在のボリュームを０に
		SetCurrentVolume(0.0f);
		// フェードパワー設定
		fade_power_ = max_volume_ / static_cast<float>(fade_time);
		// フェードフラグON
		fade_flag_ = true;
	}else
	{
		// 現在のボリュームを最大に
		SetCurrentVolume(1.0f);
		// フェードフラグOFF
		fade_flag_ = false;
	}
	// 再生準備
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = size_audio_;
	buffer.pAudioData = data_audio_;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	// ループ設定
	buffer.LoopCount = (loop_flag)? XAUDIO2_LOOP_INFINITE:0;
	// 状態取得
	source_voice_->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{
		// 一時停止
		source_voice_->Stop(0);
		// オーディオバッファの削除
		source_voice_->FlushSourceBuffers();
	}
	// オーディオバッファの登録
	source_voice_->SubmitSourceBuffer(&buffer);
	// 再生
	source_voice_->Start(0);
}

//================================================================================
// セグメント停止
//================================================================================
void Sound::Stop(int fade_time)
{
	// 停止準備
	XAUDIO2_VOICE_STATE xa2state;
	source_voice_->GetState(&xa2state);
	// 再生状態チェック
	if(xa2state.BuffersQueued != 0)
	{
		if(fade_time > 0)
		{
			// フェードON
			fade_flag_ = true;
			// フェードパワー設定
			fade_power_ = -(GetCurrentVolume() / static_cast<float>(fade_time));
		}else
		{
			// 一時停止
			source_voice_->Stop(0);
			// オーディオバッファの削除
			source_voice_->FlushSourceBuffers();
		}
	}
}

//================================================================================
// 全サウンドの更新
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
		// 解放確認
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
// サウンドの更新
//================================================================================
void Sound::Update(void)
{
	// フェードチェック
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
	// １回終了確認
	if(master_flag_)
	{
		XAUDIO2_VOICE_STATE xa2state;
		source_voice_->GetState(&xa2state);
		// 再生状態チェック
		if(xa2state.BuffersQueued == 0)
		{
			death_flag_ = true;
			fade_flag_ = false;
		}
	}
	// デバッグ用サウンドOFF
	#ifdef _DEBUG
		#ifdef _SOUND_OFF_
			SetCurrentVolume(0.0f);
		#endif
	#endif
}

//================================================================================
// サウンドのコンストラクタ
//================================================================================
Sound::Sound(const char* pName)
{
	// 各種変数初期化
	source_voice_ = nullptr;			// ソースボイス
	data_audio_ = nullptr;				// オーディオデータ
	size_audio_ = 0;					// オーディオデータサイズ
	fade_flag_ = false;					// フェード
	fade_power_ = 0.0f;				// フェードパワー
	death_flag_ = false;					// デス

	HANDLE file_handle;
	DWORD dwChunkSize = 0;
	DWORD dwChunkPosition = 0;
	DWORD dwFiletype;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	// バッファのクリア
	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	// 状態確認用変数
	HRESULT hr;

	// サウンドデータファイルの生成
	file_handle = CreateFile(pName, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);
	if(file_handle == INVALID_HANDLE_VALUE)
	{
		//MessageBox(nullptr, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
		return;
	}
	if(SetFilePointer(file_handle, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// ファイルポインタを先頭に移動
		//MessageBox(nullptr, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
		return;
	}
	
	// WAVEファイルのチェック
	hr = CheckChunk(file_handle, 'FFIR', &dwChunkSize, &dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return;
	}
	hr = ReadChunkData(file_handle, &dwFiletype, sizeof(DWORD), dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return;
	}
	if(dwFiletype != 'EVAW')
	{
		//MessageBox(nullptr, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
		return;
	}
	
	// フォーマットチェック
	hr = CheckChunk(file_handle, ' tmf', &dwChunkSize, &dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
		return;
	}
	hr = ReadChunkData(file_handle, &wfx, dwChunkSize, dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
		return;
	}

	// オーディオデータ読み込み
	hr = CheckChunk(file_handle, 'atad', &size_audio_, &dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
		return;
	}
	data_audio_ = (BYTE*)malloc(size_audio_);
	hr = ReadChunkData(file_handle, data_audio_, size_audio_, dwChunkPosition);
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
		return;
	}
	
	// ソースボイスの生成
	hr = x_audio2_->CreateSourceVoice(&source_voice_, &(wfx.Format));
	if(FAILED(hr))
	{
		//MessageBox(nullptr, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
		return;
	}

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = size_audio_;
	buffer.pAudioData = data_audio_;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = 0;

	// オーディオバッファの登録
	source_voice_->SubmitSourceBuffer(&buffer);

	count_sound_++;
}

//================================================================================
// サウンドのデストラクタ
//================================================================================
Sound::~Sound(void)
{
	if(source_voice_)
	{
		// 一時停止
		source_voice_->Stop(0);
		// ソースボイスの破棄
		source_voice_->DestroyVoice();
		source_voice_ = nullptr;
		// オーディオデータの開放
		free(data_audio_);
		data_audio_ = nullptr;
	}

	count_sound_--;
}

//================================================================================
// チャンクチェック
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
		// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(file_handle, &dwChunkType, sizeof(DWORD), &dwRead, nullptr) == 0)
		{
			// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(file_handle, &dwChunkDataSize, sizeof(DWORD), &dwRead, nullptr) == 0)
		{
			// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(file_handle, &dwFileType, sizeof(DWORD), &dwRead, nullptr) == 0)
			{
				// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(file_handle, dwChunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				// ファイルポインタをチャンクデータ分移動
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
// チャンク読み込み
//================================================================================
HRESULT Sound::ReadChunkData(HANDLE file_handle, void *pBuffer, DWORD dw_buffersize, DWORD dw_bufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(file_handle, dw_bufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(file_handle, pBuffer, dw_buffersize, &dwRead, nullptr) == 0)
	{
		// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}