#pragma once
#include <list>

/// <summary>
/// BGM,SEの再生を管理するシステム
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
	/// BGMを流す
	/// </summary>
	/// <param name="soundHnadle">BGMハンドル</param>
	/// <param name="isLoop">ループするか(デフォでtrue)</param>
	/// /// <param name="isSoundPosSave">再生場所の保存後の再生か</param>
	void PlayBgm(int soundHnadle = -1, bool isLoop = true, bool isSoundPosSave = false);

	/// <summary>
	/// Fade設定ありBGM
	/// </summary>
	/// <param name="soundHandle">BGMハンドル : -値だと現在流しているBGMをそのまま変更する</param>
	/// <param name="rate">音量パーセント</param>
	/// <param name="isLoop">ループするか</param>
	void PlayFadeBgm(int soundHandle = -1, float rate = 1.0f, bool isLoop = true);

	/// <summary>
	/// SEを流す
	/// </summary>
	/// <param name="seHandle">SEハンドル</param>
	void PlaySe(int seHandle, bool isOption = false);

	void PlayFadeSe(int handle, float rate = 1.0f, bool isOption = false);

	/// <summary>
	/// 音声の停止
	/// </summary>
	/// /// <param name="soundHandle">ハンドル</param>
	/// /// <param name="isSoundPosSave">再生場所の保存を行うか</param>
	void StopBGM(int soundHandle = -1, bool isSoundPosSave = false);
	
	void Stop(int handle);

	/// <summary>
	/// BGMのボリュームを変更する
	/// </summary>
	/// <param name="val">変更させる値 引数の値だけ±する</param>
	bool ChangeBgmVol(int val, bool isApp = true);

	/// <summary>
	/// SEのボリュームを変更する
	/// </summary>
	/// <param name="val">変更させる値 引数の値だけ±する</param>
	bool ChangeSeVol(int val);

	// Get関数
	int GetBgmVol() const { return m_bgmVolume; }
	int GetSeVol() const { return m_seVolume; }
	int GetMaxVol() const;
	bool IsNowPlay(int handle) const;
	/// <summary>
	/// BGMのボリュームの割合(0.0f ~ 1.0f)を取得
	/// </summary>
	/// <returns>BgmVolume</returns>
	float GetBgmVolRate() const;
	/// <summary>
	/// SEのボリュームの割合(0.0f ~ 1.0f)を取得
	/// </summary>
	/// <returns>SeVolum</returns>
	float GetSeVolRate() const;

private:

	/// <summary>
	/// 再生中のBGMの音量を変更する
	/// </summary>
	void SetBgmVol(float rate = 1.0f);

	void ChangePlaySeVol(int handle, float rate);
	
private:
	// ボリューム調整
	int m_bgmVolume;
	int m_seVolume;

	// 現在再生中のBGM
	int m_nowPlayBgm;
	// 最後に鳴らしたSE
	int m_soundHandle;

	// 
	long long m_soundSavePos;

	bool m_isStop;
	std::list<SaveSeData> m_saveSeList;
};

