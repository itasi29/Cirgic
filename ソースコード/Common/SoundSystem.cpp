#include <DxLib.h>
#include <cassert>
#include <algorithm>
#include "SoundSystem.h"

namespace
{
	constexpr int VOLUME_MAX = 255;
}

SoundSystem::SoundSystem() :
	m_bgmVolume(VOLUME_MAX),
	m_seVolume(VOLUME_MAX),
	m_nowPlayBgm(-1),
	m_soundHandle(-1),
	m_soundSavePos(0)
{
}

SoundSystem::~SoundSystem()
{
}

SoundSystem& SoundSystem::GetInstance()
{
	static SoundSystem snd;
	return snd;
}

void SoundSystem::Update()
{
	if (m_isStop) return;

	m_saveSeList.remove_if(
		[](const auto& info)
		{
			return !CheckSoundMem(info.handle);
		});
}

void SoundSystem::StopNowPlaySe()
{
	m_isStop = true;

	for (auto& info : m_saveSeList)
	{
		info.savePos = GetSoundCurrentPosition(info.handle);
		StopSoundMem(info.handle);
	}
}

void SoundSystem::RestartStopedSe()
{
	m_isStop = false;

	for (auto& info : m_saveSeList)
	{
		SetSoundCurrentTime(info.savePos, info.handle);
		ChangeNextPlayVolumeSoundMem(m_seVolume, info.handle);
		PlaySoundMem(info.handle, DX_PLAYTYPE_BACK, false);
	}
}

void SoundSystem::PlayBgm(int soundHandle, bool isLoop, bool isSoundPosSave)
{
	if (soundHandle < 0)
	{
		soundHandle = m_nowPlayBgm;
	}

	if (IsNowPlay(soundHandle)) return;

	// ���ʂ̕ύX
	ChangeNextPlayVolumeSoundMem(m_bgmVolume, soundHandle);

	if (isSoundPosSave)
	{
		SetSoundCurrentTime(m_soundSavePos, soundHandle);
		PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK, isLoop);
	}
	else
	{
		PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK, isLoop);
	}

	m_nowPlayBgm = soundHandle;
}

void SoundSystem::PlayFadeBgm(int soundHandle, float rate, bool isLoop)
{
	if (soundHandle < 0)
	{
		soundHandle = m_nowPlayBgm;
	}

	if (IsNowPlay(soundHandle))
	{
		SetBgmVol(rate);
		return;
	}

	// ���ʂ̕ύX
	ChangeNextPlayVolumeSoundMem(static_cast<int>(m_bgmVolume * rate), soundHandle);

	// ���[�v��ON�̏ꍇ��BGM���I������擪�ɖ߂�
	PlaySoundMem(soundHandle, DX_PLAYTYPE_BACK, isLoop);

	m_nowPlayBgm = soundHandle;
}

void SoundSystem::PlaySe(int seHandle, bool isOption)
{
	// ���ʂ̕ύX
	ChangeNextPlayVolumeSoundMem(m_seVolume, seHandle);

	auto err = PlaySoundMem(seHandle, DX_PLAYTYPE_BACK, true);
	assert(err != -1);
//	PlaySoundMem(seHandle, DX_PLAYTYPE_NORMAL, true);
	m_soundHandle = seHandle;

	if (!isOption)
	{
		m_saveSeList.emplace_back(SaveSeData{seHandle, 0});
	}
}

void SoundSystem::PlayFadeSe(int handle, float rate, bool isOption)
{
	if (IsNowPlay(handle))
	{
		ChangePlaySeVol(handle, rate);
		return;
	}

	ChangeNextPlayVolumeSoundMem(static_cast<int>(m_seVolume * rate), handle);

	PlaySoundMem(handle, DX_PLAYTYPE_BACK, true);

	if (!isOption)
	{
		m_saveSeList.emplace_back(SaveSeData{ handle, 0 });
	}
}

void SoundSystem::StopBGM(int soundHandle, bool isSoundPosSave)
{
	if (soundHandle < 0)
	{
		soundHandle = m_nowPlayBgm;
	}

	StopSoundMem(soundHandle);

	// �����Đ��ꏊ�̕ۑ�
	if (isSoundPosSave)
	{
		m_soundSavePos = GetSoundCurrentPosition(soundHandle);
	}
}

void SoundSystem::Stop(int handle)
{
	StopSoundMem(handle);
}

bool SoundSystem::ChangeBgmVol(int val, bool isApp)
{
	bool isChange = false;

	int temp = m_bgmVolume + val;
	if (temp > VOLUME_MAX)
	{
		m_bgmVolume = VOLUME_MAX;
		isChange = false;
	}
	else if (temp < 0)
	{
		m_bgmVolume = 0;
		isChange = false;
	}
	else
	{
		m_bgmVolume = temp;
		isChange = true;
	}
	
	if (isApp)
	{
		SetBgmVol();
	}

	return isChange;
}

bool SoundSystem::ChangeSeVol(int val)
{
	bool isChange = false;

	int temp = m_seVolume + val;
	if (temp > VOLUME_MAX)
	{
		m_seVolume = VOLUME_MAX;
		isChange = false;
	}
	else if (temp < 0)
	{
		m_seVolume = 0;
		isChange = false;
	}
	else
	{
		m_seVolume = temp;
		isChange = true;
	}

	PlaySe(m_soundHandle);

	return isChange;
}

int SoundSystem::GetMaxVol() const
{
	return VOLUME_MAX;
}

bool SoundSystem::IsNowPlay(int handle) const
{
	auto err = CheckSoundMem(handle);
	assert(err != -1);
	return err;
}

float SoundSystem::GetBgmVolRate() const
{
	return static_cast<float>(m_bgmVolume) / static_cast<float>(VOLUME_MAX);
}

float SoundSystem::GetSeVolRate() const
{
	return static_cast<float>(m_seVolume) / static_cast<float>(VOLUME_MAX);
}

void SoundSystem::SetBgmVol(float rate)
{
	// �Đ����łȂ���ΏI��
	if (!IsNowPlay(m_nowPlayBgm)) return;

	// �Y���Ă������萳�m�Ȃ������� GetSoundCurrentTime 
	LONGLONG soundPosition = GetSoundCurrentPosition(m_nowPlayBgm);

	// ��x�T�E���h���~�߂�
	StopSoundMem(m_nowPlayBgm);

	// �Đ��ʒu��ݒ�
	SetSoundCurrentTime(soundPosition, m_nowPlayBgm);

	// ���ʂ̒���
	ChangeNextPlayVolumeSoundMem(static_cast<int>(m_bgmVolume * rate), m_nowPlayBgm);

	// �ēx�Đ�
	PlaySoundMem(m_nowPlayBgm, DX_PLAYTYPE_BACK, false);
}

void SoundSystem::ChangePlaySeVol(int handle, float rate)
{
	LONGLONG soundPos = GetSoundCurrentPosition(handle);

	StopSoundMem(handle);

	SetSoundCurrentTime(soundPos, handle);

	// ���ʂ̒���
	ChangeNextPlayVolumeSoundMem(static_cast<int>(m_seVolume * rate), handle);

	// �ēx�Đ�
	PlaySoundMem(handle, DX_PLAYTYPE_BACK, false);
}
