//-----------------------------------------------------------------------------
//@file  
//	TemplateModule.cpp
//
//@author
//	Martin FLEURENT aka 'martignasse'
//
//@brief 
//	Implementation of the TemplateModule class.
//
//  A template include to start from for new user module.
//
//@historic 
//	2013/05/15
//    first release for Hollyhock CPP SDK 6.00.226 
//
//@IMPORTANT
//	This file is part of the Usine Hollyhock CPP SDK
//
//  Please, report bugs and patch to Usine forum :
//  http://www.sensomusic.com/wiki2/doku.php?id=hollyhock:bugsreport 
//
// All dependencies are under there own licence.
//
//@LICENCE
// Copyright (C) 2013 Sensomusic
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), 
// to deal in the Software without restriction, including without limitation 
// the rights to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to whom the Software 
// is furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
//     copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
// SOFTWARE.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------
#include "TemplateModule.h"

//----------------------------------------------------------------------------
// create, general info and destroy methodes
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Create
void CreateModule (void* &pModule, AinsiCharPtr optionalString, LongBool Flag, MasterInfo* pMasterInfo, AinsiCharPtr optionalContent)
{
	pModule = new TemplateModule();
}

// destroy
void DestroyModule(void* pModule) 
{
	// cast is important to call the good destructor
	delete ((TemplateModule*)pModule);
}

//-------------------------------------------------------------------------
// module constructors/destructors
//-------------------------------------------------------------------------

// constructor
TemplateModule::TemplateModule()
{
	rate = 0;
	mt.seed(static_cast<long int>(time(NULL)));
}

// destructor
TemplateModule::~TemplateModule()
{
	// 
}



//void TemplateModule::onCreate(AinsiCharPtr optionalString);
//void TemplateModule::onDestroy(); 
void TemplateModule::onGetModuleInfo (MasterInfo* pMasterInfo, ModuleInfo* pModuleInfo)
{
	pModuleInfo->Name				= "Mersenne";
	pModuleInfo->Description		= "Mersenne Twister";
	pModuleInfo->ModuleType         = mtSimple;
	pModuleInfo->BackColor          = sdkGetUsineColor(clDataModuleColor);
	pModuleInfo->NumberOfParams     = eNumParams;
	pModuleInfo->Version			= "1.0";
	pModuleInfo->DontProcess		= false;
}

//-----------------------------------------------------------------------------
// query system and init
int  TemplateModule::onGetNumberOfParams( int QIdx) {return 0;}
void TemplateModule::onAfterQuery (MasterInfo* pMasterInfo, ModuleInfo* pModuleInfo, int QIdx) {}
void TemplateModule::onInitModule (MasterInfo* pMasterInfo, ModuleInfo* pModuleInfo) {}

//-----------------------------------------------------------------------------
// parameters and process
void TemplateModule::onGetParamInfo (int ParamIndex, TParamInfo* pParamInfo)
{	
	switch (ParamIndex) 
	{
		// Add all parameters declared in the module class, example :

	case eRate:
		pParamInfo->ParamType			= ptListBox;
		pParamInfo->Caption				= "Rate";
		pParamInfo->IsInput				= TRUE;
		pParamInfo->IsOutput			= FALSE;
		pParamInfo->ListBoxStrings		= "\"Button\",\"Control\",\"Audio\"";
		pParamInfo->DefaultValue		= 0; 
		pParamInfo->ReadOnly			= FALSE;
		pParamInfo->IsStoredInPreset	= TRUE;
		pParamInfo->CallBackType		= ctImmediate;
		break;

	case eTrigger:
		pParamInfo->ParamType		= ptButton;
		pParamInfo->Caption			= "trigger";
		pParamInfo->IsInput			= TRUE;
		pParamInfo->IsOutput		= FALSE;
		break;

	case eOut:
		pParamInfo->ParamType		= ptArray;
		pParamInfo->Caption			= "out";
		pParamInfo->IsInput			= FALSE;
		pParamInfo->IsOutput		= TRUE;
		pParamInfo->ReadOnly		= TRUE;
		break;

		// default case
	default:
		break;
	}
}
void TemplateModule::onSetEventAddress (int ParamIndex, UsineEventPtr pEvent)
{
	switch (ParamIndex) 
	{
		// Initialyse all events adress declared in your module class, Example :

		// m_txtfExample
	case eRate:
		params[eRate] = pEvent;
		break;
	case eTrigger:
		params[eTrigger] = pEvent;
		break;
	case eOut:
		params[eOut] = pEvent;
		break;

		// default case
	default:
		break;
	}
}
void TemplateModule::onCallBack (UsineMessage *Message) {

	switch (Message->wParam) 
	{
	case eRate:
		rate = (int)sdkGetEvtData(params[eRate]);
		break;
	case eTrigger:
		if(Message->lParam == MSG_CHANGE && sdkGetEvtData(params[eRate]) == eButton){
			sdkSetEvtData(params[eOut], (float)mt());
		} 
		break;
	case eOut:
		//params[eOut] = pEvent;
		break;

		// default case
	default:
		break;
	}
}
void TemplateModule::onProcess () {

	if(sdkGetEvtData(params[eRate]) == eControl){
		sdkSetEvtSize(params[eOut], 1);
		sdkSetEvtData(params[eOut], (float)mt());
	}else if (sdkGetEvtData(params[eRate]) == eAudio){
		int sampleFrames = sdkGetBlocSize();
		sdkSetEvtSize(params[eOut], sampleFrames);
		for (int i = 0; i < sampleFrames; i++){   
			sdkSetEvtArrayData(params[eOut], i, (float)mt());
		}
	}
}

//-----------------------------------------------------------------------------
// midi out callbacks
void TemplateModule::onMidiSendOut (int DeviceID, UsineMidiCode Code) {}
void TemplateModule::onMidiSysexSendOut (int DeviceID, char* Sysex) {}

//-----------------------------------------------------------------------------
// chunk system
int  TemplateModule::onGetChunkLen (LongBool Preset) {return 0;}
void TemplateModule::onGetChunk (void* chunk, LongBool Preset) {}
void TemplateModule::onSetChunk (const void* chunk, int sizeInBytes, LongBool Preset) {}

//-----------------------------------------------------------------------------
// layout
void TemplateModule::onCreateSettings() {}
void TemplateModule::onSettingsHasChanged() {}
void TemplateModule::onPaint () {}

//-----------------------------------------------------------------------------
// mouse and multi touch interaction
void TemplateModule::onMouseMove(TShiftState Shift, float X, float Y) {}
void TemplateModule::onMouseDown(TMouseButton MouseButton, TShiftState Shift, float X,float Y) {}
void TemplateModule::onMouseUp (TMouseButton MouseButton, TShiftState Shift, float X,float Y) {}
void TemplateModule::onMouseMoveMultiProc(TShiftState Shift, UsineEventPtr X, UsineEventPtr Y) {}
void TemplateModule::onMouseDownMultiProc(TMouseButton MouseButton, TShiftState Shift, UsineEventPtr X, UsineEventPtr Y) {}
void TemplateModule::onMouseUpMultiProc (TMouseButton MouseButton, TShiftState Shift,UsineEventPtr X, UsineEventPtr Y) {}
void TemplateModule::onOpenEditor() {}
void TemplateModule::onBringToFront() {}

//-----------------------------------------------------------------------------
// audio setup update
void TemplateModule::onBlocSizeChange (int BlocSize) {}
void TemplateModule::onSampleRateChange (double SampleRate) {}

//-----------------------------------------------------------------------------
// recording 
void TemplateModule::onSetRecordedValue (TPrecision X, TPrecision Y, TPrecision Z) {}
