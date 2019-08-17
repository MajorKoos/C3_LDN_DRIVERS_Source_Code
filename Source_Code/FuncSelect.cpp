//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "FuncSelect.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFuncSel *FuncSel;
TMainForm *Mform;

//---------------------------------------------------------------------------
__fastcall TFuncSel::TFuncSel(TComponent *Owner, TMainForm *Mf, union FUNC_UNION_COMMAND_LIST *Func, int FuncID) : TForm(Owner)
{
	Mform = Mf;
	FuncSel = this;
	LanguageSetStr();
	FuncSel->Left = (Screen->Width - FuncSel->Width) / 2;
	FuncSel->Top = (Screen->Height - FuncSel->Height) / 2;
	SetFunc = Func;
	IsSetFuncID = FuncID;

	for(int i = 0; i < Mform->PhyCfg.KeyScanCfg.Config.KeyMax; i++)
	{
		ComboBox9->Items->Add(i);
	}
	ComboBox9->ItemIndex = 0;

	for(int i = 0; i < LAYER_DEF_MACRO_ONE_GROUP_MAX_STEP; i++)
	{
		ComboBox12->Items->Add(i);
		ComboBox13->Items->Add(i);
		ComboBox15->Items->Add(i);
	}
	ComboBox12->ItemIndex = 0;
	ComboBox13->ItemIndex = 0;
	ComboBox15->ItemIndex = 0;

	//�����Ч
	for(int i = 0; i < Mform->CpRun.BklConfig.LedCfg.BklEffectMax + EFF_DEF_BKL_CUSTOM_MAX_EFFECT; i++)
	{
		ComboBox16->Items->Add(i);
	}
	ComboBox16->ItemIndex = 0;

	//�����Ч���ٶ�
	for(int i = 1; i <= EFF_DEF_ERR_MAX_SPEED; i++)
	{
		ComboBox17->Items->Add(i);
	}
	ComboBox17->ItemIndex = 0;

	switch(FuncID)
	{
		case 0: //���ܲ�����ҳ
		{
			//�������ҳ
			RadioButton1->Checked = true;
			RadioButton2->Visible = false;
			RadioButton3->Visible = false;
			Button10->Enabled = true;

			//��ý��ҳ
			RadioButton4->Checked = true;
			RadioButton5->Visible = false;
			RadioButton6->Visible = false;
			TabSheet4->TabVisible = false;
		} break;
		case 1: //������ҳ��
		{
			//�������ҳ
			RadioButton1->Checked = true;
			RadioButton2->Visible = false;
			RadioButton3->Visible = false;
			Button10->Visible = false;

			//��ý��ҳ
			RadioButton4->Checked = true;
			RadioButton5->Visible = false;
			RadioButton6->Visible = false;
			TabSheet4->TabVisible = false;
		} break;
		case 2: //��
		{
			//�������ҳ
			RadioButton1->Checked = true;
			RadioButton2->Visible = true;
			RadioButton3->Visible = true;
			Button10->Visible = false;

			//��ý��ҳ
			RadioButton4->Checked = true;
			RadioButton5->Visible = true;
			RadioButton6->Visible = true;
		} break;
		case 3: //������
		{
			TabSheet3->TabVisible = false;
			TabSheet4->TabVisible = false;
			TabSheet5->TabVisible = false;

			RadioButton1->Visible = false;
			RadioButton2->Visible = false;
			RadioButton3->Visible = false;
			Button10->Visible = false;

			//��ý��ҳ
			RadioButton4->Visible = false;
			RadioButton5->Visible = false;
			RadioButton6->Visible = false;
		} break;
	}

	MediaKeyAddItems();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::KEYSelectClick(TObject *Sender)
{
	//
	SetFuncForNormalKeyCode(dynamic_cast <TButton *> (Sender)->Tag);
	Close();
}

//---------------------------------------------------------------------------
uint8_t __fastcall TFuncSel::GetKbExCode(uint8_t idx)//������չ����
{
	//
	if(idx >= CP_DEF_NORMAL_KB_EX_CODE_MAX)
	{
		return (0);
	}
	return (KB_EX_CODE.Code[idx]);
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::ButtonGroup1ButtonClicked(TObject *Sender, int Index)
{
	//ȡ�ü���
	SetFuncForNormalKeyCode(GetKbExCode(Index));
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::SetFuncForNormalKeyCode(uint8_t Code)//������ͨ����Ϊ������
{
	//
	switch(IsSetFuncID)
	{
		case 0: //���ܲ�
		case 3: //������
		{
			SetFunc->NormalKey.FuncID = FUNC_DEF_CMD_NORMAL_KEY_CODE;
			SetFunc->NormalKey.KeyCode = Code;
			SetFunc->NormalKey.Status = 0;
			SetFunc->NormalKey.PressCount = 0;
		} break;
		case 1: //������
		{
			SetFunc->NormalKey.FuncID = FUNC_DEF_CMD_NORMAL_KEY_CODE;
			SetFunc->NormalKey.KeyCode = Code;
			SetFunc->NormalKey.Status = 0;
			SetFunc->NormalKey.PressCount = 0;
		} break;
		case 2: //��༭��
		{
			if(RadioButton1->Checked)
			{
				SetFunc->NormalKey.FuncID = FUNC_DEF_CMD_NORMAL_KEY_CODE;
				SetFunc->NormalKey.KeyCode = Code;
				SetFunc->NormalKey.Status = 0;
				SetFunc->NormalKey.PressCount = 0;
			}
			if(RadioButton2->Checked)//���´���
			{
				SetFunc->MacroPressNormalKey.FuncID = FUNC_DEF_CMD_MACRO_P_NORMAL_KEY;
				SetFunc->MacroPressNormalKey.KeyCode = Code;
			}
			if(RadioButton3->Checked)//�ͷŴ���
			{
				SetFunc->MacroReleaseNormalKey.FuncID = FUNC_DEF_CMD_MACRO_R_NORMAL_KEY;
				SetFunc->MacroReleaseNormalKey.KeyCode = Code;
			}
		} break;
	}

}

void __fastcall TFuncSel::Button10Click(TObject *Sender)
{
	//���ð�ť������
	if(IsSetFuncID == 0)//�ڹ��ܲ㣬���ʾ����һ�������������㲻ʹ��
	{
		SetFunc->NormalKey.FuncID = FUNC_DEF_CMD_NORMAL_KEY_DISABLE;
		SetFunc->NormalKey.KeyCode = 0;
		SetFunc->NormalKey.Status = 0;
		SetFunc->NormalKey.PressCount = 0;
	}
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::MediaKeyAddItems(void)//��Ӷ�ý�����ҳ�����Ŀ
{
	//
	int index;

	TGrpButtonItem *itm;

	//��ӵ������
	for(index = 0; index < Mform->CpRun.MediaKeyCode.Code->Count; index++)
	{
		itm = ButtonGroup2->Items->Add();
		itm->Caption = Mform->CpRun.MediaKeyCode.Str->Strings[index];
		itm->ImageIndex = Mform->CpRun.MediaKeyCode.Code->Strings[index].ToIntDef(0);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::ButtonGroup2ButtonClicked(TObject *Sender, int Index)
{
	//��ý����밴ť�鱻����
	switch(IsSetFuncID)
	{
		case 0: //���ܲ�
		case 3: //������
		{
			SetFunc->MediaKey.FuncID = FUNC_DEF_CMD_MEDIA_KEY_CODE; //��ý�����
			SetFunc->MediaKey.Status = 0;
			SetFunc->MediaKey.MediaKeyCode = ButtonGroup2->Items->Items[Index]->ImageIndex;
		} break;
		case 1: //������
		{
			SetFunc->MediaKey.FuncID = FUNC_DEF_CMD_MEDIA_KEY_CODE; //��ý�����
			SetFunc->MediaKey.Status = 0;
			SetFunc->MediaKey.MediaKeyCode = ButtonGroup2->Items->Items[Index]->ImageIndex;
		} break;
		case 2: //��
		{
			if(RadioButton4->Checked)
			{
				SetFunc->MediaKey.FuncID = FUNC_DEF_CMD_MEDIA_KEY_CODE; //��ý�����
				SetFunc->MediaKey.Status = 0;
				SetFunc->MediaKey.MediaKeyCode = ButtonGroup2->Items->Items[Index]->ImageIndex;
			}
			if(RadioButton5->Checked)//���´���
			{
				SetFunc->MacroPressMediaKey.FuncID = FUNC_DEF_CMD_MACRO_P_MEDIA_KEY;
				SetFunc->MacroPressMediaKey.MediaCode = ButtonGroup2->Items->Items[Index]->ImageIndex;
			}
			if(RadioButton6->Checked)//�ͷŴ���
			{
				SetFunc->MacroReleaseMediaKey.FuncID = FUNC_DEF_CMD_MACRO_R_MEDIA_KEY;
				SetFunc->MacroReleaseMediaKey.MediaCode = ButtonGroup2->Items->Items[Index]->ImageIndex;
			}
		} break;
	}
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::MediaSelectClick(TObject *Sender)//��ý�尴��ѡ��
{
	//
	switch(IsSetFuncID)
	{
		case 0: //���ܲ�
		case 3: //������
		{
			SetFunc->MediaKey.FuncID = FUNC_DEF_CMD_MEDIA_KEY_CODE; //��ý�����
			SetFunc->MediaKey.Status = 0;
			SetFunc->MediaKey.MediaKeyCode = dynamic_cast <TButton *> (Sender)->Tag;
		} break;
		case 1: //������
		{
			SetFunc->MediaKey.FuncID = FUNC_DEF_CMD_MEDIA_KEY_CODE; //��ý�����
			SetFunc->MediaKey.Status = 0;
			SetFunc->MediaKey.MediaKeyCode = dynamic_cast <TButton *> (Sender)->Tag;
		} break;
		case 2: //��
		{
			if(RadioButton4->Checked)
			{
				SetFunc->MediaKey.FuncID = FUNC_DEF_CMD_MEDIA_KEY_CODE; //��ý�����
				SetFunc->MediaKey.Status = 0;
				SetFunc->MediaKey.MediaKeyCode = dynamic_cast <TButton *> (Sender)->Tag;
			}
			if(RadioButton5->Checked)//���´���
			{
				SetFunc->MacroPressMediaKey.FuncID = FUNC_DEF_CMD_MACRO_P_MEDIA_KEY;
				SetFunc->MacroPressMediaKey.MediaCode = dynamic_cast <TButton *> (Sender)->Tag;
			}
			if(RadioButton6->Checked)//�ͷŴ���
			{
				SetFunc->MacroReleaseMediaKey.FuncID = FUNC_DEF_CMD_MACRO_R_MEDIA_KEY;
				SetFunc->MacroReleaseMediaKey.MediaCode = dynamic_cast <TButton *> (Sender)->Tag;
			}
		} break;
	}
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::Button127Click(TObject *Sender)
{
	//���ܲ㿪��-��
	SetFunc->FLayerEnable.FuncID = FUNC_DEF_CMD_ENABLE_FLAYER;
	SetFunc->FLayerEnable.LayerID = ComboBox1->ItemIndex;
	SetFunc->FLayerEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button128Click(TObject *Sender)
{
	//���ܲ㿪��-�ر�
	SetFunc->FLayerEnable.FuncID = FUNC_DEF_CMD_ENABLE_FLAYER;
	SetFunc->FLayerEnable.LayerID = ComboBox1->ItemIndex;
	SetFunc->FLayerEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button129Click(TObject *Sender)
{
	//���ܲ�����Ϊ������ȼ�
	SetFunc->FLayerSetTopPriority.FuncID = FUNC_DEF_CMD_SET_FLAYER_TOP_PRIORITY;
	SetFunc->FLayerSetTopPriority.LayerID = ComboBox2->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button130Click(TObject *Sender)
{
	//�������
	SetFunc->TLayerActive.FuncID = FUNC_DEF_CMD_ACTIVE_TLAYER;
	SetFunc->TLayerActive.TlayerID = ComboBox3->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button131Click(TObject *Sender)
{
	//fn�������ܿ���
	SetFunc->FnFuncEnable.FuncID = FUNC_DEF_CMD_FN_FUNC_ENABLE;
	SetFunc->FnFuncEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button132Click(TObject *Sender)
{
	//fn�������ܿ���
	SetFunc->FnFuncEnable.FuncID = FUNC_DEF_CMD_FN_FUNC_ENABLE;
	SetFunc->FnFuncEnable.Enable = 0;
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::Button133Click(TObject *Sender)
{
	//FN����������
	SetFunc->FnKeyEnable.FuncID = FUNC_DEF_CMD_FN_KEY_ENABLE;
	SetFunc->FnKeyEnable.Keyid = ComboBox4->ItemIndex;
	SetFunc->FnKeyEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button134Click(TObject *Sender)
{
	//FN����������
	SetFunc->FnKeyEnable.FuncID = FUNC_DEF_CMD_FN_KEY_ENABLE;
	SetFunc->FnKeyEnable.Keyid = ComboBox4->ItemIndex;
	SetFunc->FnKeyEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button135Click(TObject *Sender)
{
	//��ϼ��ܿ���
	SetFunc->ComboFuncEnable.FuncID = FUNC_DEF_CMD_COMBO_FUNC_ENABLE;
	SetFunc->ComboFuncEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button136Click(TObject *Sender)
{
	//��ϼ��ܿ���
	SetFunc->ComboFuncEnable.FuncID = FUNC_DEF_CMD_COMBO_FUNC_ENABLE;
	SetFunc->ComboFuncEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button143Click(TObject *Sender)
{
	//��ϼ��ֿ���
	SetFunc->ComboGroupEnable.FuncID = FUNC_DEF_CMD_COMBO_KEY_ENABLE;
	SetFunc->ComboGroupEnable.GroupID = ComboBox5->ItemIndex;
	SetFunc->ComboGroupEnable.Enable = 1;
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::Button144Click(TObject *Sender)
{
	//��ϼ��ֿ���
	SetFunc->ComboGroupEnable.FuncID = FUNC_DEF_CMD_COMBO_KEY_ENABLE;
	SetFunc->ComboGroupEnable.GroupID = ComboBox5->ItemIndex;
	SetFunc->ComboGroupEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button137Click(TObject *Sender)
{
	//�������ܿ���
	SetFunc->nHitFuncEnable.FuncID = FUNC_DEF_CMD_NHIT_FUNC_ENABLE;
	SetFunc->nHitFuncEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button138Click(TObject *Sender)
{
	//�������ܿ���
	SetFunc->nHitFuncEnable.FuncID = FUNC_DEF_CMD_NHIT_FUNC_ENABLE;
	SetFunc->nHitFuncEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button145Click(TObject *Sender)
{
	//�������ֿ���
	SetFunc->nHitKeyEnable.FuncID = FUNC_DEF_CMD_NHIT_KEY_ENABLE;
	SetFunc->nHitKeyEnable.Keyid = ComboBox6->ItemIndex;
	SetFunc->nHitKeyEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button146Click(TObject *Sender)
{
	//�������ֿ���
	SetFunc->nHitKeyEnable.FuncID = FUNC_DEF_CMD_NHIT_KEY_ENABLE;
	SetFunc->nHitKeyEnable.Keyid = ComboBox6->ItemIndex;
	SetFunc->nHitKeyEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button139Click(TObject *Sender)
{
	//����һ���ܿ���
	SetFunc->Two1FuncEnable.FuncID = FUNC_DEF_CMD_2IN1_FUNC_ENABLE;
	SetFunc->Two1FuncEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button140Click(TObject *Sender)
{
	//����һ���ܿ���
	SetFunc->Two1FuncEnable.FuncID = FUNC_DEF_CMD_2IN1_FUNC_ENABLE;
	SetFunc->Two1FuncEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button147Click(TObject *Sender)
{
	//����һ���ֿ���
	SetFunc->Two1KeyEnable.FuncID = FUNC_DEF_CMD_2IN1_KEY_ENABLE;
	SetFunc->Two1KeyEnable.Keyid = ComboBox7->ItemIndex;
	SetFunc->Two1KeyEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button148Click(TObject *Sender)
{
	//����һ���ֿ���
	SetFunc->Two1KeyEnable.FuncID = FUNC_DEF_CMD_2IN1_KEY_ENABLE;
	SetFunc->Two1KeyEnable.Keyid = ComboBox7->ItemIndex;
	SetFunc->Two1KeyEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button141Click(TObject *Sender)
{
	//���ּ��ܿ���
	SetFunc->HoldDownFuncEnable.FuncID = FUNC_DEF_CMD_HOLDDOWN_FUNC_ENABLE;
	SetFunc->HoldDownFuncEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button142Click(TObject *Sender)
{
	//���ּ��ܿ���
	SetFunc->HoldDownFuncEnable.FuncID = FUNC_DEF_CMD_HOLDDOWN_FUNC_ENABLE;
	SetFunc->HoldDownFuncEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button149Click(TObject *Sender)
{
	SetFunc->HoldDownKeyEnable.FuncID = FUNC_DEF_CMD_NORMAL_KEY_ENABLE;
	SetFunc->HoldDownKeyEnable.Keyid = ComboBox9->ItemIndex;
	SetFunc->HoldDownKeyEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button150Click(TObject *Sender)
{
	SetFunc->HoldDownKeyEnable.FuncID = FUNC_DEF_CMD_NORMAL_KEY_ENABLE;
	SetFunc->HoldDownKeyEnable.Keyid = ComboBox9->ItemIndex;
	SetFunc->HoldDownKeyEnable.Enable = 0;
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::ButtonGroup2MouseEnter(TObject *Sender)
{
	ButtonGroup2->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::Button153Click(TObject *Sender)
{
	//����һ���
	SetFunc->RunMacroGroup.FuncID = FUNC_DEF_CMD_RUN_MACRO;
	SetFunc->RunMacroGroup.GroupID = ComboBox10->ItemIndex;
	SetFunc->RunMacroGroup.StepID = ComboBox12->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button154Click(TObject *Sender)
{
	SetFunc->MacroStopRun.FuncID = FUNC_DEF_CMD_MACRO_STOP;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button155Click(TObject *Sender)
{
	//������
	SetFunc->RunKeyLockGroup.FuncID = FUNC_DEF_CMD_RUN_KEY_LOCK;
	SetFunc->RunKeyLockGroup.GroupID = ComboBox11->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button156Click(TObject *Sender)
{
	//����ת
	SetFunc->MacroJmp.FuncID = FUNC_DEF_CMD_MACRO_CURR_JMP;
	SetFunc->MacroJmp.JmpStepID = ComboBox13->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button157Click(TObject *Sender)
{
	//������ת
	SetFunc->MacroGroupJmp.FuncID = FUNC_DEF_CMD_MACRO_GROUP_JMP;
	SetFunc->MacroGroupJmp.GroupID = ComboBox14->ItemIndex;
	SetFunc->MacroGroupJmp.JmpStepID = ComboBox15->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Edit1KeyPress(TObject *Sender, System::WideChar &Key)
{
	if(!(((Key >= '0') && (Key <= '9')) || (Key == VK_BACK) || (Key == VK_DELETE)))
	{
		Key = 0;
	}
	else
	{
		if(!(Edit1->Text.Length() < 8 || Key == VK_BACK) || (Key == VK_DELETE))
		{
			Key = 0;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Edit1Change(TObject *Sender)
{
	int tmp = Edit1->Text.ToIntDef(20);

	if(tmp >= 1 && tmp <= 16777215)
	{
		Edit1->Font->Color = clGreen;
		Button168->Enabled = true;
	}
	else
	{
		Edit1->Font->Color = clRed;
		Button168->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button168Click(TObject *Sender)
{
	//�Զ�����ʱȷ��
	SetFunc->MacroDelay.FuncID = FUNC_DEF_CMD_MACRO_DELAY;
	SetFunc->MacroDelay.Timems = Edit1->Text.ToIntDef(20);
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button158Click(TObject *Sender)
{
	//��ʱ
	SetFunc->MacroDelay.FuncID = FUNC_DEF_CMD_MACRO_DELAY;
	SetFunc->MacroDelay.Timems = dynamic_cast <TButton *> (Sender)->Tag;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button169Click(TObject *Sender)
{
	//��һ�������Ч
	SetFunc->BklLastEff.FuncID = FUNC_DEF_CMD_BKL_LAST_EFF;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button170Click(TObject *Sender)
{
	//��һ�������Ч
	SetFunc->BklNextEff.FuncID = FUNC_DEF_CMD_BKL_NEXT_EFF;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button172Click(TObject *Sender)
{
	//�����ٶ�����
	SetFunc->BklSpeedFast.FuncID = FUNC_DEF_CMD_BKL_EFF_FAST;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button173Click(TObject *Sender)
{
	//�����ٶȽ���
	SetFunc->BklSpeedSlow.FuncID = FUNC_DEF_CMD_BKL_EFF_SLOW;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button175Click(TObject *Sender)
{
	//��������
	SetFunc->BklLedLigLevel.FuncID = FUNC_DEF_CMD_BKL_HIGH_ALL_LED_LIG_LEVEL;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button176Click(TObject *Sender)
{
	//��������
	SetFunc->BklLedLigLevel.FuncID = FUNC_DEF_CMD_BKL_LOW_ALL_LED_LIG_LEVEL;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button188Click(TObject *Sender)
{
	SetFunc->BklEnable.FuncID = FUNC_DEF_CMD_BKL_ENABLE;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button174Click(TObject *Sender)
{
	//ָ����Ч���ٶ�
	SetFunc->BklSpeedSet.FuncID = FUNC_DEF_CMD_BKL_EFF_SPEED_SET;
	SetFunc->BklSpeedSet.EffIndex = 0;
	SetFunc->BklSpeedSet.Speed = ComboBox17->ItemIndex + 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button171Click(TObject *Sender)
{
	//�л���ָ���ĵ�Ч
	SetFunc->BklSetEff.FuncID = FUNC_DEF_CMD_BKL_SET_EFF;
	SetFunc->BklSetEff.EffIndex = ComboBox16->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button177Click(TObject *Sender)
{
	//��WS�ĵ�Ч
	SetFunc->BklWsEnableGroup.FuncID = FUNC_DEF_CMD_WS_ENABLE_GROUP_FUNC;
	SetFunc->BklWsEnableGroup.GroupID = ComboBox18->ItemIndex;
	SetFunc->BklWsEnableGroup.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button178Click(TObject *Sender)
{
	//�ر�WS�ĵ�Ч
	SetFunc->BklWsEnableGroup.FuncID = FUNC_DEF_CMD_WS_ENABLE_GROUP_FUNC;
	SetFunc->BklWsEnableGroup.GroupID = ComboBox18->ItemIndex;
	SetFunc->BklWsEnableGroup.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button179Click(TObject *Sender)
{
	//ws�л�����һ����Ч
	SetFunc->BklWsSetGroupUDEff.FuncID = FUNC_DEF_CMD_WS_SET_GROUP_LAST_EFF;
	SetFunc->BklWsSetGroupUDEff.GroupID = ComboBox19->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button180Click(TObject *Sender)
{
	//ws�л�����һ����Ч
	SetFunc->BklWsSetGroupUDEff.FuncID = FUNC_DEF_CMD_WS_SET_GROUP_NEXT_EFF;
	SetFunc->BklWsSetGroupUDEff.GroupID = ComboBox20->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button181Click(TObject *Sender)
{
	//ָ����Ч
	SetFunc->BklWsSetGroupEffect.FuncID = FUNC_DEF_CMD_WS_SET_GROUP_EFFECT;
	SetFunc->BklWsSetGroupEffect.GroupID = ComboBox21->ItemIndex;
	SetFunc->BklWsSetGroupEffect.EffIndex = ComboBox22->ItemIndex;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button182Click(TObject *Sender)
{
	//����Caps����ָʾ��
	SetFunc->BklSetStLedEnable.FuncID = FUNC_DEF_CMD_ST_LED_ENABLE_CAPS;
	SetFunc->BklSetStLedEnable.Leds = 0;
	SetFunc->BklSetStLedEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button183Click(TObject *Sender)
{
	//����aps����ָʾ��
	SetFunc->BklSetStLedEnable.FuncID = FUNC_DEF_CMD_ST_LED_ENABLE_CAPS;
	SetFunc->BklSetStLedEnable.Leds = 0;
	SetFunc->BklSetStLedEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button184Click(TObject *Sender)
{
	//����SCROLL����ָʾ��
	SetFunc->BklSetStLedEnable.FuncID = FUNC_DEF_CMD_ST_LED_ENABLE_SCROLL;
	SetFunc->BklSetStLedEnable.Leds = 1;
	SetFunc->BklSetStLedEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button185Click(TObject *Sender)
{
	//����SCROLL����ָʾ��
	SetFunc->BklSetStLedEnable.FuncID = FUNC_DEF_CMD_ST_LED_ENABLE_SCROLL;
	SetFunc->BklSetStLedEnable.Leds = 1;
	SetFunc->BklSetStLedEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button186Click(TObject *Sender)
{
	//����NUM����ָʾ��
	SetFunc->BklSetStLedEnable.FuncID = FUNC_DEF_CMD_ST_LED_ENABLE_NUM;
	SetFunc->BklSetStLedEnable.Leds = 2;
	SetFunc->BklSetStLedEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button187Click(TObject *Sender)
{
	//����NUM����ָʾ��
	SetFunc->BklSetStLedEnable.FuncID = FUNC_DEF_CMD_ST_LED_ENABLE_NUM;
	SetFunc->BklSetStLedEnable.Leds = 2;
	SetFunc->BklSetStLedEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button189Click(TObject *Sender)
{
	//����CAPS�Ķ���ָʾ
	SetFunc->BklSetLedActionEnable.FuncID = FUNC_DEF_CMD_LED_ACTION_CAPS_ENABLE;
	SetFunc->BklSetLedActionEnable.Leds = 0;
	SetFunc->BklSetLedActionEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button190Click(TObject *Sender)
{
	//����CAPS�Ķ���ָʾ
	SetFunc->BklSetLedActionEnable.FuncID = FUNC_DEF_CMD_LED_ACTION_CAPS_ENABLE;
	SetFunc->BklSetLedActionEnable.Leds = 0;
	SetFunc->BklSetLedActionEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button191Click(TObject *Sender)
{
	//����Scroll�Ķ���ָʾ
	SetFunc->BklSetLedActionEnable.FuncID = FUNC_DEF_CMD_LED_ACTION_SCROLL_ENABLE;
	SetFunc->BklSetLedActionEnable.Leds = 1;
	SetFunc->BklSetLedActionEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button192Click(TObject *Sender)
{
	//����Scroll�Ķ���ָʾ
	SetFunc->BklSetLedActionEnable.FuncID = FUNC_DEF_CMD_LED_ACTION_SCROLL_ENABLE;
	SetFunc->BklSetLedActionEnable.Leds = 1;
	SetFunc->BklSetLedActionEnable.Enable = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button193Click(TObject *Sender)
{
	//����NUM�Ķ���ָʾ
	SetFunc->BklSetLedActionEnable.FuncID = FUNC_DEF_CMD_LED_ACTION_NUM_ENABLE;
	SetFunc->BklSetLedActionEnable.Leds = 2;
	SetFunc->BklSetLedActionEnable.Enable = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFuncSel::Button194Click(TObject *Sender)
{
	//����NUM�Ķ���ָʾ
	SetFunc->BklSetLedActionEnable.FuncID = FUNC_DEF_CMD_LED_ACTION_NUM_ENABLE;
	SetFunc->BklSetLedActionEnable.Leds = 2;
	SetFunc->BklSetLedActionEnable.Enable = 0;
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFuncSel::LanguageSetStr(void)//��������
{
	FuncSel->Caption = Mform->LanguageGetStr(630);
	TabSheet1->Caption = Mform->LanguageGetStr(500);
	TabSheet2->Caption = Mform->LanguageGetStr(501);
	TabSheet3->Caption = Mform->LanguageGetStr(502);
	TabSheet4->Caption = Mform->LanguageGetStr(503);
	TabSheet5->Caption = Mform->LanguageGetStr(504);

	RadioButton1->Caption = Mform->LanguageGetStr(510);
	RadioButton2->Caption = Mform->LanguageGetStr(511);
	RadioButton3->Caption = Mform->LanguageGetStr(512);

	RadioButton4->Caption = Mform->LanguageGetStr(520);
	RadioButton5->Caption = Mform->LanguageGetStr(521);
	RadioButton6->Caption = Mform->LanguageGetStr(522);

	//���ؿ���ҳ��
	GroupBox1->Caption = Mform->LanguageGetStr(540);
	Label1->Caption = Mform->LanguageGetStr(541);
	Label2->Caption = Mform->LanguageGetStr(542);
	Button127->Caption = Mform->LanguageGetStr(530); //��
	Button128->Caption = Mform->LanguageGetStr(531); //�ر�
	Button129->Caption = Mform->LanguageGetStr(532); //ȷ��

	GroupBox2->Caption = Mform->LanguageGetStr(543);
	Label3->Caption = Mform->LanguageGetStr(544);
	Button130->Caption = Mform->LanguageGetStr(532);

	GroupBox3->Caption = Mform->LanguageGetStr(545);
	Label4->Caption = Mform->LanguageGetStr(546);
	Label5->Caption = Mform->LanguageGetStr(547);
	Button131->Caption = Mform->LanguageGetStr(530); //��
	Button132->Caption = Mform->LanguageGetStr(531); //�ر�
	Button133->Caption = Mform->LanguageGetStr(530); //��
	Button134->Caption = Mform->LanguageGetStr(531); //��

	GroupBox4->Caption = Mform->LanguageGetStr(548);
	Label6->Caption = Mform->LanguageGetStr(549);
	Label7->Caption = Mform->LanguageGetStr(550);
	Button135->Caption = Mform->LanguageGetStr(530); //��
	Button136->Caption = Mform->LanguageGetStr(531); //�ر�
	Button143->Caption = Mform->LanguageGetStr(530); //��
	Button144->Caption = Mform->LanguageGetStr(531); //��

	GroupBox5->Caption = Mform->LanguageGetStr(551);
	Label8->Caption = Mform->LanguageGetStr(552);
	Label9->Caption = Mform->LanguageGetStr(553);
	Button137->Caption = Mform->LanguageGetStr(530); //��
	Button138->Caption = Mform->LanguageGetStr(531); //�ر�
	Button145->Caption = Mform->LanguageGetStr(530); //��
	Button146->Caption = Mform->LanguageGetStr(531); //��

	GroupBox10->Caption = Mform->LanguageGetStr(554);
	Label14->Caption = Mform->LanguageGetStr(555);
	Button151->Caption = Mform->LanguageGetStr(530); //��
	Button152->Caption = Mform->LanguageGetStr(531); //�ر�

	GroupBox6->Caption = Mform->LanguageGetStr(556);
	Label10->Caption = Mform->LanguageGetStr(557);
	Label11->Caption = Mform->LanguageGetStr(558);
	Button139->Caption = Mform->LanguageGetStr(530); //��
	Button140->Caption = Mform->LanguageGetStr(531); //�ر�
	Button147->Caption = Mform->LanguageGetStr(530); //��
	Button148->Caption = Mform->LanguageGetStr(531); //��

	GroupBox7->Caption = Mform->LanguageGetStr(559);
	Label12->Caption = Mform->LanguageGetStr(560);
	Label13->Caption = Mform->LanguageGetStr(561);
	Button141->Caption = Mform->LanguageGetStr(530); //��
	Button142->Caption = Mform->LanguageGetStr(531); //�ر�
	Button149->Caption = Mform->LanguageGetStr(530); //��
	Button150->Caption = Mform->LanguageGetStr(531); //��

	GroupBox8->Caption = Mform->LanguageGetStr(562);
	Label15->Caption = Mform->LanguageGetStr(563);
	Label16->Caption = Mform->LanguageGetStr(564);
	Button153->Caption = Mform->LanguageGetStr(532);
	Button154->Caption = Mform->LanguageGetStr(533);

	GroupBox9->Caption = Mform->LanguageGetStr(565);
	Label17->Caption = Mform->LanguageGetStr(566);
	Button155->Caption = Mform->LanguageGetStr(532);

	//�깦��ҳ��
	GroupBox11->Caption = Mform->LanguageGetStr(570);
	Label18->Caption = Mform->LanguageGetStr(571);
	Button156->Caption = Mform->LanguageGetStr(532);

	GroupBox12->Caption = Mform->LanguageGetStr(572);
	Label19->Caption = Mform->LanguageGetStr(573);
	Label20->Caption = Mform->LanguageGetStr(574);
	Button157->Caption = Mform->LanguageGetStr(532);

	GroupBox13->Caption = Mform->LanguageGetStr(575);
	Button158->Caption = Mform->LanguageGetStr(576);
	Button159->Caption = Mform->LanguageGetStr(577);
	Button160->Caption = Mform->LanguageGetStr(578);
	Button161->Caption = Mform->LanguageGetStr(579);
	Button162->Caption = Mform->LanguageGetStr(580);
	Button163->Caption = Mform->LanguageGetStr(581);
	Button164->Caption = Mform->LanguageGetStr(582);
	Button165->Caption = Mform->LanguageGetStr(583);
	Button166->Caption = Mform->LanguageGetStr(584);
	Button167->Caption = Mform->LanguageGetStr(585);
	Label21->Caption = Mform->LanguageGetStr(586);
	Button168->Caption = Mform->LanguageGetStr(532);

	//�������ҳ��
	GroupBox14->Caption = Mform->LanguageGetStr(590);
	Button169->Caption = Mform->LanguageGetStr(591);
	Button170->Caption = Mform->LanguageGetStr(592);
	Button172->Caption = Mform->LanguageGetStr(593);
	Button173->Caption = Mform->LanguageGetStr(594);
	Button175->Caption = Mform->LanguageGetStr(595);
	Button176->Caption = Mform->LanguageGetStr(596);
	Button188->Caption = Mform->LanguageGetStr(597);
	Button174->Caption = Mform->LanguageGetStr(532);
	Button171->Caption = Mform->LanguageGetStr(532);
	Label24->Caption = Mform->LanguageGetStr(598);
	Label23->Caption = Mform->LanguageGetStr(599);

	GroupBox15->Caption = Mform->LanguageGetStr(600);
	Label25->Caption = Mform->LanguageGetStr(601);
	Label26->Caption = Mform->LanguageGetStr(602);
	Label27->Caption = Mform->LanguageGetStr(602);
	Label28->Caption = Mform->LanguageGetStr(605);

	Button177->Caption = Mform->LanguageGetStr(530);
	Button178->Caption = Mform->LanguageGetStr(531);
	Button179->Caption = Mform->LanguageGetStr(603);
	Button180->Caption = Mform->LanguageGetStr(604);
	Button181->Caption = Mform->LanguageGetStr(532);

	//WS��Ч�ַ���
	ComboBox22->Items->Add(Mform->LanguageGetStr(107));
	ComboBox22->Items->Add(Mform->LanguageGetStr(108));
	ComboBox22->Items->Add(Mform->LanguageGetStr(109));
	ComboBox22->Items->Add(Mform->LanguageGetStr(110));
	ComboBox22->Items->Add(Mform->LanguageGetStr(111));
	ComboBox22->ItemIndex = 0;

	GroupBox16->Caption = Mform->LanguageGetStr(610);
	Button182->Caption = Mform->LanguageGetStr(611);
	Button183->Caption = Mform->LanguageGetStr(612);
	Button184->Caption = Mform->LanguageGetStr(613);
	Button185->Caption = Mform->LanguageGetStr(614);
	Button186->Caption = Mform->LanguageGetStr(615);
	Button187->Caption = Mform->LanguageGetStr(616);

	GroupBox17->Caption = Mform->LanguageGetStr(620);
	Button189->Caption = Mform->LanguageGetStr(621);
	Button190->Caption = Mform->LanguageGetStr(622);
	Button191->Caption = Mform->LanguageGetStr(623);
	Button192->Caption = Mform->LanguageGetStr(624);
	Button193->Caption = Mform->LanguageGetStr(625);
	Button194->Caption = Mform->LanguageGetStr(626);

}

