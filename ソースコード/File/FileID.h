#pragma once

namespace FileID
{
	namespace Img
	{
		const wchar_t* const LOGO = L"I_Logo";
		const wchar_t* const FRAME = L"I_Frame";
		const wchar_t* const FRAME_SELECT_LEFT = L"I_SelectFrameL";
		const wchar_t* const FRAME_SELECT_RIGHT = L"I_SelectFrameR";
		const wchar_t* const FRAME_NUM = L"I_NumFrame";
		const wchar_t* const FRAME_SELECT_NUM = L"I_SelectNumFrame";
		const wchar_t* const FRAME_STANDING_1 = L"I_StandingFrame1P";
		const wchar_t* const FRAME_STANDING_2 = L"I_StandingFrame2P";
		const wchar_t* const FRAME_STANDING_3 = L"I_StandingFrame3P";
		const wchar_t* const FRAME_STANDING_4 = L"I_StandingFrame4P";
		const wchar_t* const FRAME_STANDING_NPC = L"I_StandingFrameNPC";
		const wchar_t* const STANDING_1 = L"I_Standing1";
		const wchar_t* const STANDING_2 = L"I_Standing2";
		const wchar_t* const STANDING_3 = L"I_Standing3";
		const wchar_t* const STANDING_4 = L"I_Standing4";
		const wchar_t* const STANDING_5 = L"I_Standing5";
		const wchar_t* const STANDING_6 = L"I_Standing6";
		const wchar_t* const COUNT_DOWN_1 = L"I_CountDown1";
		const wchar_t* const COUNT_DOWN_2 = L"I_CountDown2";
		const wchar_t* const COUNT_DOWN_3 = L"I_CountDown3";
		const wchar_t* const COUNT_DOWN_GO = L"I_CountDownGo";
		const wchar_t* const COUNT_DOWN_FINISH = L"I_CountDownFinish";
		const wchar_t* const COUNT_DOWN_CURTAIN = L"I_CountDownCurtain";
		const wchar_t* const COIN = L"I_Coin";
		const wchar_t* const RAP = L"I_Rap";
		const wchar_t* const FRAME_BACK = L"I_BackFrame";
		const wchar_t* const FRAME_OPTION = L"I_OptionFrame";
		const wchar_t* const TEX_CAR_RED = L"I_CarTexRed";
		const wchar_t* const TEX_CAR_BULE = L"I_CarTexBule";
		const wchar_t* const TEX_CAR_YELLOW = L"I_CarTexYellow";
		const wchar_t* const TEX_CAR_PURPLE = L"I_CarTexPurple";
		const wchar_t* const TEX_SKY_DOME_1 = L"I_SkyDomeTex1";
		const wchar_t* const TEX_SKY_DOME_2 = L"I_SkyDomeTex2";
		const wchar_t* const TEX_SKY_DOME_3 = L"I_SkyDomeTex3";
		const wchar_t* const TEX_SKY_DOME_4 = L"I_SkyDomeTex4";
		const wchar_t* const FRAME_ITEM = L"I_ItemFrame";
		const wchar_t* const ITEM_KINOKO = L"I_ItemKinoko";
		const wchar_t* const ITEM_KINOKO_TIRPLE_1 = L"I_ItemKinokoTriple1";
		const wchar_t* const ITEM_KINOKO_TIRPLE_2 = L"I_ItemKinokoTriple2";
		const wchar_t* const ITEM_KINOKO_TIRPLE_3 = L"I_ItemKinokoTriple3";
		const wchar_t* const ITEM_CARAPACE_GREEN = L"I_ItemCarapaceGreen";
		const wchar_t* const ITEM_CARAPACE_RED = L"I_ItemCarapaceRed";
		const wchar_t* const ITEM_BANANA = L"I_ItemBanana";
		const wchar_t* const MINI_MAP = L"I_MiniMap";
		const wchar_t* const MAP_PLAYER_1 = L"I_MapPlayer1";
		const wchar_t* const MAP_PLAYER_2 = L"I_MapPlayer2";
		const wchar_t* const MAP_PLAYER_3 = L"I_MapPlayer3";
		const wchar_t* const MAP_PLAYER_4 = L"I_MapPlayer4";
	}
	namespace Mdl
	{
		const wchar_t* const SKY_DOME = L"M_SkyDome";
		const wchar_t* const CAR = L"M_Car";
		const wchar_t* const LOAD_STRAIGHT = L"M_Straight";
		const wchar_t* const LOAD_CURVE = L"M_Curve";
		const wchar_t* const LOAD_SCURVE = L"M_SCurve";
		const wchar_t* const GROUND = L"M_Ground";
		const wchar_t* const STAND = L"M_Stand";
		const wchar_t* const FENCE_1 = L"M_Fence_1";
		const wchar_t* const FENCE_2 = L"M_Fence_2";
		const wchar_t* const TIRE = L"M_Tire";
		const wchar_t* const ARCH_GOAL = L"M_ArchGoal";
		const wchar_t* const ARCH_NORMAL = L"M_ArchNormal";
		const wchar_t* const COIN = L"M_Coin";
		const wchar_t* const CRYSTAL = L"M_Crystal";
		const wchar_t* const PUDIUM = L"M_Podium";
	}
	namespace Snd
	{
		namespace SE
		{
			const wchar_t* const SELECT = L"S_SE_Select";
			const wchar_t* const CANCEL = L"S_SE_Cancel";
			const wchar_t* const CURSOR_MOVE = L"S_SE_CursoMove";
			const wchar_t* const NOT_CURSOR_MOVE = L"S_SE_NotCursoMove";
			const wchar_t* const COUNT_DOWN = L"S_SE_CountDown";
			const wchar_t* const RANKING_PUSH = L"S_SE_RankingPush";
			const wchar_t* const COIN = L"S_SE_Coin";
			const wchar_t* const ITEM_GET = L"S_SE_ItemGet";
			const wchar_t* const RAP_UPDATE = L"S_SE_RapUpdate";
			const wchar_t* const DASH = L"S_SE_Dash";
			const wchar_t* const CAR_ENGINE = L"S_SE_CarEngine";
			const wchar_t* const DRIFT = L"S_SE_Drift";
			const wchar_t* const GOAL = L"S_SE_Goal";
		}
		namespace BGM
		{
			const wchar_t* const TITLE = L"S_BGM_Title";
			const wchar_t* const MAIN = L"S_BGM_Main";
			const wchar_t* const RESULT = L"S_BGM_Result";
		}
	}
	namespace Eff
	{
		const wchar_t* const DRIFET_SMALE = L"EF_DriftSmale";
		const wchar_t* const DRIFET_LARGE = L"EF_DriftLarge";
		const wchar_t* const DASH = L"EF_Dash";
		const wchar_t* const SMOKE = L"EF_Smoke";
		const wchar_t* const CARAPCE_GREEN = L"EF_CARAPACE_GREEN";
		const wchar_t* const CARAPCE_RED = L"EF_CARAPACE_RED";
	}
}
