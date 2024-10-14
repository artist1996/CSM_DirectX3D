#include "pch.h"
#include "CStateMgr.h"

#include "CDirezieAirHitState.h"
#include "CDirezieAttack01State.h"
#include "CDirezieAttack02State.h"
#include "CDirezieAttack03State.h"
#include "CDirezieAttack04State.h"
#include "CDirezieAttack05State.h"
#include "CDirezieDeadState.h"
#include "CDirezieDownHitState.h"
#include "CDirezieFallState.h"
#include "CDirezieFlyState.h"
#include "CDirezieHideState.h"
#include "CDirezieHitBBQState.h"
#include "CDirezieHitState.h"
#include "CDirezieHoldingState.h"
#include "CDirezieIdleState.h"
#include "CDirezieStiffnessState.h"
#include "CDirezieTraceState.h"
#include "CDirezieWakeUpState.h"
#include "CHyungteoAirHitState.h"
#include "CHyungteoBuffEaterState.h"
#include "CHyungteoDeadState.h"
#include "CHyungteoDownHitState.h"
#include "CHyungteoEatState.h"
#include "CHyungteoFallState.h"
#include "CHyungteoFlyState.h"
#include "CHyungteoHitBBQState.h"
#include "CHyungteoHitState.h"
#include "CHyungteoIdleState.h"
#include "CHyungteoLookState.h"
#include "CHyungteoPunchState.h"
#include "CHyungteoRecoilState.h"
#include "CHyungteoStiffnessState.h"
#include "CHyungteoStingState.h"
#include "CHyungteoTraceState.h"
#include "CHyungteoWakeUpState.h"
#include "CJurisAttackState.h"
#include "CJurisBackAttackState.h"
#include "CJurisDeadState.h"
#include "CJurisHideState.h"
#include "CJurisHitState.h"
#include "CJurisIdleState.h"
#include "CJurisStiffnessState.h"
#include "CJurisTraceState.h"
#include "CMeltKnightAttackState.h"
#include "CMeltKnightDeadState.h"
#include "CMeltKnightIdleState.h"
#include "CMeltKnightStandByState.h"
#include "CMeltKnightTraceState.h"

void CStateMgr::GetStateInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CDirezieAirHitState");
	_vec.push_back(L"CDirezieAttack01State");
	_vec.push_back(L"CDirezieAttack02State");
	_vec.push_back(L"CDirezieAttack03State");
	_vec.push_back(L"CDirezieAttack04State");
	_vec.push_back(L"CDirezieAttack05State");
	_vec.push_back(L"CDirezieDeadState");
	_vec.push_back(L"CDirezieDownHitState");
	_vec.push_back(L"CDirezieFallState");
	_vec.push_back(L"CDirezieFlyState");
	_vec.push_back(L"CDirezieHideState");
	_vec.push_back(L"CDirezieHitBBQState");
	_vec.push_back(L"CDirezieHitState");
	_vec.push_back(L"CDirezieHoldingState");
	_vec.push_back(L"CDirezieIdleState");
	_vec.push_back(L"CDirezieStiffnessState");
	_vec.push_back(L"CDirezieTraceState");
	_vec.push_back(L"CDirezieWakeUpState");
	_vec.push_back(L"CHyungteoAirHitState");
	_vec.push_back(L"CHyungteoBuffEaterState");
	_vec.push_back(L"CHyungteoDeadState");
	_vec.push_back(L"CHyungteoDownHitState");
	_vec.push_back(L"CHyungteoEatState");
	_vec.push_back(L"CHyungteoFallState");
	_vec.push_back(L"CHyungteoFlyState");
	_vec.push_back(L"CHyungteoHitBBQState");
	_vec.push_back(L"CHyungteoHitState");
	_vec.push_back(L"CHyungteoIdleState");
	_vec.push_back(L"CHyungteoLookState");
	_vec.push_back(L"CHyungteoPunchState");
	_vec.push_back(L"CHyungteoRecoilState");
	_vec.push_back(L"CHyungteoStiffnessState");
	_vec.push_back(L"CHyungteoStingState");
	_vec.push_back(L"CHyungteoTraceState");
	_vec.push_back(L"CHyungteoWakeUpState");
	_vec.push_back(L"CJurisAttackState");
	_vec.push_back(L"CJurisBackAttackState");
	_vec.push_back(L"CJurisDeadState");
	_vec.push_back(L"CJurisHideState");
	_vec.push_back(L"CJurisHitState");
	_vec.push_back(L"CJurisIdleState");
	_vec.push_back(L"CJurisStiffnessState");
	_vec.push_back(L"CJurisTraceState");
	_vec.push_back(L"CMeltKnightAttackState");
	_vec.push_back(L"CMeltKnightDeadState");
	_vec.push_back(L"CMeltKnightIdleState");
	_vec.push_back(L"CMeltKnightStandByState");
	_vec.push_back(L"CMeltKnightTraceState");
}

CState * CStateMgr::GetState(const wstring& _strStateName)
{
	if (L"CDirezieAirHitState" == _strStateName)
		return new CDirezieAirHitState;
	if (L"CDirezieAttack01State" == _strStateName)
		return new CDirezieAttack01State;
	if (L"CDirezieAttack02State" == _strStateName)
		return new CDirezieAttack02State;
	if (L"CDirezieAttack03State" == _strStateName)
		return new CDirezieAttack03State;
	if (L"CDirezieAttack04State" == _strStateName)
		return new CDirezieAttack04State;
	if (L"CDirezieAttack05State" == _strStateName)
		return new CDirezieAttack05State;
	if (L"CDirezieDeadState" == _strStateName)
		return new CDirezieDeadState;
	if (L"CDirezieDownHitState" == _strStateName)
		return new CDirezieDownHitState;
	if (L"CDirezieFallState" == _strStateName)
		return new CDirezieFallState;
	if (L"CDirezieFlyState" == _strStateName)
		return new CDirezieFlyState;
	if (L"CDirezieHideState" == _strStateName)
		return new CDirezieHideState;
	if (L"CDirezieHitBBQState" == _strStateName)
		return new CDirezieHitBBQState;
	if (L"CDirezieHitState" == _strStateName)
		return new CDirezieHitState;
	if (L"CDirezieHoldingState" == _strStateName)
		return new CDirezieHoldingState;
	if (L"CDirezieIdleState" == _strStateName)
		return new CDirezieIdleState;
	if (L"CDirezieStiffnessState" == _strStateName)
		return new CDirezieStiffnessState;
	if (L"CDirezieTraceState" == _strStateName)
		return new CDirezieTraceState;
	if (L"CDirezieWakeUpState" == _strStateName)
		return new CDirezieWakeUpState;
	if (L"CHyungteoAirHitState" == _strStateName)
		return new CHyungteoAirHitState;
	if (L"CHyungteoBuffEaterState" == _strStateName)
		return new CHyungteoBuffEaterState;
	if (L"CHyungteoDeadState" == _strStateName)
		return new CHyungteoDeadState;
	if (L"CHyungteoDownHitState" == _strStateName)
		return new CHyungteoDownHitState;
	if (L"CHyungteoEatState" == _strStateName)
		return new CHyungteoEatState;
	if (L"CHyungteoFallState" == _strStateName)
		return new CHyungteoFallState;
	if (L"CHyungteoFlyState" == _strStateName)
		return new CHyungteoFlyState;
	if (L"CHyungteoHitBBQState" == _strStateName)
		return new CHyungteoHitBBQState;
	if (L"CHyungteoHitState" == _strStateName)
		return new CHyungteoHitState;
	if (L"CHyungteoIdleState" == _strStateName)
		return new CHyungteoIdleState;
	if (L"CHyungteoLookState" == _strStateName)
		return new CHyungteoLookState;
	if (L"CHyungteoPunchState" == _strStateName)
		return new CHyungteoPunchState;
	if (L"CHyungteoRecoilState" == _strStateName)
		return new CHyungteoRecoilState;
	if (L"CHyungteoStiffnessState" == _strStateName)
		return new CHyungteoStiffnessState;
	if (L"CHyungteoStingState" == _strStateName)
		return new CHyungteoStingState;
	if (L"CHyungteoTraceState" == _strStateName)
		return new CHyungteoTraceState;
	if (L"CHyungteoWakeUpState" == _strStateName)
		return new CHyungteoWakeUpState;
	if (L"CJurisAttackState" == _strStateName)
		return new CJurisAttackState;
	if (L"CJurisBackAttackState" == _strStateName)
		return new CJurisBackAttackState;
	if (L"CJurisDeadState" == _strStateName)
		return new CJurisDeadState;
	if (L"CJurisHideState" == _strStateName)
		return new CJurisHideState;
	if (L"CJurisHitState" == _strStateName)
		return new CJurisHitState;
	if (L"CJurisIdleState" == _strStateName)
		return new CJurisIdleState;
	if (L"CJurisStiffnessState" == _strStateName)
		return new CJurisStiffnessState;
	if (L"CJurisTraceState" == _strStateName)
		return new CJurisTraceState;
	if (L"CMeltKnightAttackState" == _strStateName)
		return new CMeltKnightAttackState;
	if (L"CMeltKnightDeadState" == _strStateName)
		return new CMeltKnightDeadState;
	if (L"CMeltKnightIdleState" == _strStateName)
		return new CMeltKnightIdleState;
	if (L"CMeltKnightStandByState" == _strStateName)
		return new CMeltKnightStandByState;
	if (L"CMeltKnightTraceState" == _strStateName)
		return new CMeltKnightTraceState;
	return nullptr;
}

CState * CStateMgr::GetState(UINT _iStateType)
{
	switch (_iStateType)
	{
	case (UINT)STATE_TYPE::DIREZIEAIRHITSTATE:
		return new CDirezieAirHitState;
		break;
	case (UINT)STATE_TYPE::DIREZIEATTACK01STATE:
		return new CDirezieAttack01State;
		break;
	case (UINT)STATE_TYPE::DIREZIEATTACK02STATE:
		return new CDirezieAttack02State;
		break;
	case (UINT)STATE_TYPE::DIREZIEATTACK03STATE:
		return new CDirezieAttack03State;
		break;
	case (UINT)STATE_TYPE::DIREZIEATTACK04STATE:
		return new CDirezieAttack04State;
		break;
	case (UINT)STATE_TYPE::DIREZIEATTACK05STATE:
		return new CDirezieAttack05State;
		break;
	case (UINT)STATE_TYPE::DIREZIEDEADSTATE:
		return new CDirezieDeadState;
		break;
	case (UINT)STATE_TYPE::DIREZIEDOWNHITSTATE:
		return new CDirezieDownHitState;
		break;
	case (UINT)STATE_TYPE::DIREZIEFALLSTATE:
		return new CDirezieFallState;
		break;
	case (UINT)STATE_TYPE::DIREZIEFLYSTATE:
		return new CDirezieFlyState;
		break;
	case (UINT)STATE_TYPE::DIREZIEHIDESTATE:
		return new CDirezieHideState;
		break;
	case (UINT)STATE_TYPE::DIREZIEHITBBQSTATE:
		return new CDirezieHitBBQState;
		break;
	case (UINT)STATE_TYPE::DIREZIEHITSTATE:
		return new CDirezieHitState;
		break;
	case (UINT)STATE_TYPE::DIREZIEHOLDINGSTATE:
		return new CDirezieHoldingState;
		break;
	case (UINT)STATE_TYPE::DIREZIEIDLESTATE:
		return new CDirezieIdleState;
		break;
	case (UINT)STATE_TYPE::DIREZIESTIFFNESSSTATE:
		return new CDirezieStiffnessState;
		break;
	case (UINT)STATE_TYPE::DIREZIETRACESTATE:
		return new CDirezieTraceState;
		break;
	case (UINT)STATE_TYPE::DIREZIEWAKEUPSTATE:
		return new CDirezieWakeUpState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOAIRHITSTATE:
		return new CHyungteoAirHitState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOBUFFEATERSTATE:
		return new CHyungteoBuffEaterState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEODEADSTATE:
		return new CHyungteoDeadState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEODOWNHITSTATE:
		return new CHyungteoDownHitState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOEATSTATE:
		return new CHyungteoEatState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOFALLSTATE:
		return new CHyungteoFallState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOFLYSTATE:
		return new CHyungteoFlyState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOHITBBQSTATE:
		return new CHyungteoHitBBQState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOHITSTATE:
		return new CHyungteoHitState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOIDLESTATE:
		return new CHyungteoIdleState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOLOOKSTATE:
		return new CHyungteoLookState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOPUNCHSTATE:
		return new CHyungteoPunchState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEORECOILSTATE:
		return new CHyungteoRecoilState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOSTIFFNESSSTATE:
		return new CHyungteoStiffnessState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOSTINGSTATE:
		return new CHyungteoStingState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOTRACESTATE:
		return new CHyungteoTraceState;
		break;
	case (UINT)STATE_TYPE::HYUNGTEOWAKEUPSTATE:
		return new CHyungteoWakeUpState;
		break;
	case (UINT)STATE_TYPE::JURISATTACKSTATE:
		return new CJurisAttackState;
		break;
	case (UINT)STATE_TYPE::JURISBACKATTACKSTATE:
		return new CJurisBackAttackState;
		break;
	case (UINT)STATE_TYPE::JURISDEADSTATE:
		return new CJurisDeadState;
		break;
	case (UINT)STATE_TYPE::JURISHIDESTATE:
		return new CJurisHideState;
		break;
	case (UINT)STATE_TYPE::JURISHITSTATE:
		return new CJurisHitState;
		break;
	case (UINT)STATE_TYPE::JURISIDLESTATE:
		return new CJurisIdleState;
		break;
	case (UINT)STATE_TYPE::JURISSTIFFNESSSTATE:
		return new CJurisStiffnessState;
		break;
	case (UINT)STATE_TYPE::JURISTRACESTATE:
		return new CJurisTraceState;
		break;
	case (UINT)STATE_TYPE::MELTKNIGHTATTACKSTATE:
		return new CMeltKnightAttackState;
		break;
	case (UINT)STATE_TYPE::MELTKNIGHTDEADSTATE:
		return new CMeltKnightDeadState;
		break;
	case (UINT)STATE_TYPE::MELTKNIGHTIDLESTATE:
		return new CMeltKnightIdleState;
		break;
	case (UINT)STATE_TYPE::MELTKNIGHTSTANDBYSTATE:
		return new CMeltKnightStandByState;
		break;
	case (UINT)STATE_TYPE::MELTKNIGHTTRACESTATE:
		return new CMeltKnightTraceState;
		break;
	}
	return nullptr;
}

const wchar_t * CStateMgr::GetStateName(CState * _pState)
{
	switch ((STATE_TYPE)_pState->GetStateType())
	{
	case STATE_TYPE::DIREZIEAIRHITSTATE:
		return L"CDirezieAirHitState";
		break;

	case STATE_TYPE::DIREZIEATTACK01STATE:
		return L"CDirezieAttack01State";
		break;

	case STATE_TYPE::DIREZIEATTACK02STATE:
		return L"CDirezieAttack02State";
		break;

	case STATE_TYPE::DIREZIEATTACK03STATE:
		return L"CDirezieAttack03State";
		break;

	case STATE_TYPE::DIREZIEATTACK04STATE:
		return L"CDirezieAttack04State";
		break;

	case STATE_TYPE::DIREZIEATTACK05STATE:
		return L"CDirezieAttack05State";
		break;

	case STATE_TYPE::DIREZIEDEADSTATE:
		return L"CDirezieDeadState";
		break;

	case STATE_TYPE::DIREZIEDOWNHITSTATE:
		return L"CDirezieDownHitState";
		break;

	case STATE_TYPE::DIREZIEFALLSTATE:
		return L"CDirezieFallState";
		break;

	case STATE_TYPE::DIREZIEFLYSTATE:
		return L"CDirezieFlyState";
		break;

	case STATE_TYPE::DIREZIEHIDESTATE:
		return L"CDirezieHideState";
		break;

	case STATE_TYPE::DIREZIEHITBBQSTATE:
		return L"CDirezieHitBBQState";
		break;

	case STATE_TYPE::DIREZIEHITSTATE:
		return L"CDirezieHitState";
		break;

	case STATE_TYPE::DIREZIEHOLDINGSTATE:
		return L"CDirezieHoldingState";
		break;

	case STATE_TYPE::DIREZIEIDLESTATE:
		return L"CDirezieIdleState";
		break;

	case STATE_TYPE::DIREZIESTIFFNESSSTATE:
		return L"CDirezieStiffnessState";
		break;

	case STATE_TYPE::DIREZIETRACESTATE:
		return L"CDirezieTraceState";
		break;

	case STATE_TYPE::DIREZIEWAKEUPSTATE:
		return L"CDirezieWakeUpState";
		break;

	case STATE_TYPE::HYUNGTEOAIRHITSTATE:
		return L"CHyungteoAirHitState";
		break;

	case STATE_TYPE::HYUNGTEOBUFFEATERSTATE:
		return L"CHyungteoBuffEaterState";
		break;

	case STATE_TYPE::HYUNGTEODEADSTATE:
		return L"CHyungteoDeadState";
		break;

	case STATE_TYPE::HYUNGTEODOWNHITSTATE:
		return L"CHyungteoDownHitState";
		break;

	case STATE_TYPE::HYUNGTEOEATSTATE:
		return L"CHyungteoEatState";
		break;

	case STATE_TYPE::HYUNGTEOFALLSTATE:
		return L"CHyungteoFallState";
		break;

	case STATE_TYPE::HYUNGTEOFLYSTATE:
		return L"CHyungteoFlyState";
		break;

	case STATE_TYPE::HYUNGTEOHITBBQSTATE:
		return L"CHyungteoHitBBQState";
		break;

	case STATE_TYPE::HYUNGTEOHITSTATE:
		return L"CHyungteoHitState";
		break;

	case STATE_TYPE::HYUNGTEOIDLESTATE:
		return L"CHyungteoIdleState";
		break;

	case STATE_TYPE::HYUNGTEOLOOKSTATE:
		return L"CHyungteoLookState";
		break;

	case STATE_TYPE::HYUNGTEOPUNCHSTATE:
		return L"CHyungteoPunchState";
		break;

	case STATE_TYPE::HYUNGTEORECOILSTATE:
		return L"CHyungteoRecoilState";
		break;

	case STATE_TYPE::HYUNGTEOSTIFFNESSSTATE:
		return L"CHyungteoStiffnessState";
		break;

	case STATE_TYPE::HYUNGTEOSTINGSTATE:
		return L"CHyungteoStingState";
		break;

	case STATE_TYPE::HYUNGTEOTRACESTATE:
		return L"CHyungteoTraceState";
		break;

	case STATE_TYPE::HYUNGTEOWAKEUPSTATE:
		return L"CHyungteoWakeUpState";
		break;

	case STATE_TYPE::JURISATTACKSTATE:
		return L"CJurisAttackState";
		break;

	case STATE_TYPE::JURISBACKATTACKSTATE:
		return L"CJurisBackAttackState";
		break;

	case STATE_TYPE::JURISDEADSTATE:
		return L"CJurisDeadState";
		break;

	case STATE_TYPE::JURISHIDESTATE:
		return L"CJurisHideState";
		break;

	case STATE_TYPE::JURISHITSTATE:
		return L"CJurisHitState";
		break;

	case STATE_TYPE::JURISIDLESTATE:
		return L"CJurisIdleState";
		break;

	case STATE_TYPE::JURISSTIFFNESSSTATE:
		return L"CJurisStiffnessState";
		break;

	case STATE_TYPE::JURISTRACESTATE:
		return L"CJurisTraceState";
		break;

	case STATE_TYPE::MELTKNIGHTATTACKSTATE:
		return L"CMeltKnightAttackState";
		break;

	case STATE_TYPE::MELTKNIGHTDEADSTATE:
		return L"CMeltKnightDeadState";
		break;

	case STATE_TYPE::MELTKNIGHTIDLESTATE:
		return L"CMeltKnightIdleState";
		break;

	case STATE_TYPE::MELTKNIGHTSTANDBYSTATE:
		return L"CMeltKnightStandByState";
		break;

	case STATE_TYPE::MELTKNIGHTTRACESTATE:
		return L"CMeltKnightTraceState";
		break;

	}
	return nullptr;
}