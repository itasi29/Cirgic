#pragma once
#include <list>

/// <summary>
/// BGM,SE�̍Đ����Ǘ�����V�X�e��
/// </summary>

class SoundSystem
{
private:
	struct SaveSeData
	{
		int handle;
		long long savePos;
	};

private:
	SoundSystem();

	SoundSystem(const SoundSystem& snd) = delete;
	void operator= (const SoundSystem& snd) = delete;

public:
	~SoundSystem();

	static SoundSystem& GetInstance();

	void Update();

	void StopNowPlaySe();
	void RestartStopedSe();

	/// <summary>
	/// BGM�𗬂�
	/// </summary>
	/// <param name="soundHnadle">BGM�n���h��</param>
	/// <param name="isLoop">���[�v���邩(�f�t�H��true)</param>
	/// /// <param name="isSoundPosSave">�Đ��ꏊ�̕ۑ���̍Đ���</param>
	void PlayBgm(int soundHnadle = -1, bool isLoop = true, bool isSoundPosSave = false);

	/// <summary>
	/// Fade�ݒ肠��BGM
	/// </summary>
	/// <param name="soundHandle">BGM�n���h�� : -�l���ƌ��ݗ����Ă���BGM�����̂܂ܕύX����</param>
	/// <param name="rate">���ʃp�[�Z���g</param>
	/// <param name="isLoop">���[�v���邩</param>
	void PlayFadeBgm(int soundHandle = -1, float rate = 1.0f, bool isLoop = true);

	/// <summary>
	/// SE�𗬂�
	/// </summary>
	/// <param name="seHandle">SE�n���h��</param>
	void PlaySe(int seHandle, bool isOption = false);

	void PlayFadeSe(int handle, float rate = 1.0f, bool isOption = false);

	/// <summary>
	/// �����̒�~
	/// </summary>
	/// /// <param name="soundHandle">�n���h��</param>
	/// /// <param name="isSoundPosSave">�Đ��ꏊ�̕ۑ����s����</param>
	void StopBGM(int soundHandle = -1, bool isSoundPosSave = false);
	
	void Stop(int handle);

	/// <summary>
	/// BGM�̃{�����[����ύX����
	/// </summary>
	/// <param name="val">�ύX������l �����̒l�����}����</param>
	bool ChangeBgmVol(int val, bool isApp = true);

	/// <summary>
	/// SE�̃{�����[����ύX����
	/// </summary>
	/// <param name="val">�ύX������l �����̒l�����}����</param>
	bool ChangeSeVol(int val);

	// Get�֐�
	int GetBgmVol() const { return m_bgmVolume; }
	int GetSeVol() const { return m_seVolume; }
	int GetMaxVol() const;
	bool IsNowPlay(int handle) const;
	/// <summary>
	/// BGM�̃{�����[���̊���(0.0f ~ 1.0f)���擾
	/// </summary>
	/// <returns>BgmVolume</returns>
	float GetBgmVolRate() const;
	/// <summary>
	/// SE�̃{�����[���̊���(0.0f ~ 1.0f)���擾
	/// </summary>
	/// <returns>SeVolum</returns>
	float GetSeVolRate() const;

private:

	/// <summary>
	/// �Đ�����BGM�̉��ʂ�ύX����
	/// </summary>
	void SetBgmVol(float rate = 1.0f);

	void ChangePlaySeVol(int handle, float rate);
	
private:
	// �{�����[������
	int m_bgmVolume;
	int m_seVolume;

	// ���ݍĐ�����BGM
	int m_nowPlayBgm;
	// �Ō�ɖ炵��SE
	int m_soundHandle;

	// 
	long long m_soundSavePos;

	bool m_isStop;
	std::list<SaveSeData> m_saveSeList;
};

