object FormAnimateSetting: TFormAnimateSetting
  Left = 0
  Top = 0
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080' '#1072#1085#1080#1084#1072#1094#1080#1080
  ClientHeight = 555
  ClientWidth = 368
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 353
    Height = 113
    Caption = #1058#1072#1073#1083#1080#1094#1072' '#1086#1073#1098#1077#1082#1090#1086#1074
    TabOrder = 0
    object Label1: TLabel
      Left = 40
      Top = 47
      Width = 134
      Height = 13
      Caption = #1051#1086#1082#1072#1083#1080#1079#1086#1074#1072#1085#1085#1099#1077' '#1086#1073#1098#1077#1082#1090#1099
    end
    object Label2: TLabel
      Left = 40
      Top = 66
      Width = 120
      Height = 13
      Caption = #1056#1072#1089#1087#1086#1079#1085#1072#1085#1085#1099#1077' '#1086#1073#1098#1077#1082#1090#1099
    end
    object ShapeColorLocObjTable: TShape
      Left = 180
      Top = 44
      Width = 16
      Height = 16
      OnMouseDown = ShapeColorLocObjTableMouseDown
    end
    object ShapeColorDetObjTable: TShape
      Left = 180
      Top = 66
      Width = 16
      Height = 16
      OnMouseDown = ShapeColorDetObjTableMouseDown
    end
    object CheckBoxFillTableObjects: TCheckBox
      Left = 18
      Top = 24
      Width = 142
      Height = 17
      Caption = #1047#1072#1082#1088#1072#1096#1080#1074#1072#1090#1100' '#1103#1095#1077#1081#1082#1080
      TabOrder = 0
      OnClick = CheckBoxFillTableObjectsClick
    end
    object CheckBoxApplyObjectsSeries: TCheckBox
      Left = 18
      Top = 85
      Width = 142
      Height = 17
      Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100' '#1082' '#1075#1088#1072#1092#1080#1082#1072#1084
      TabOrder = 1
      OnClick = CheckBoxApplyObjectsSeriesClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 127
    Width = 353
    Height = 122
    Caption = #1058#1072#1073#1083#1080#1094#1072' '#1092#1088#1072#1075#1084#1077#1085#1090#1086#1074
    TabOrder = 1
    object Label3: TLabel
      Left = 40
      Top = 70
      Width = 46
      Height = 13
      Caption = '1 '#1086#1073#1098#1077#1082#1090
    end
    object Label4: TLabel
      Left = 40
      Top = 51
      Width = 71
      Height = 13
      Caption = #1053#1077#1090' '#1086#1073#1098#1077#1082#1090#1086#1074
    end
    object Label5: TLabel
      Left = 234
      Top = 51
      Width = 52
      Height = 13
      Caption = '2 '#1086#1073#1098#1077#1082#1090#1072
    end
    object Label6: TLabel
      Left = 234
      Top = 70
      Width = 48
      Height = 13
      Caption = '3 '#1080' '#1073#1086#1083#1077#1077
    end
    object ShapeColorZeroObjTable: TShape
      Left = 180
      Top = 50
      Width = 16
      Height = 16
      OnMouseDown = ShapeColorZeroObjTableMouseDown
    end
    object ShapeColorOneObjTable: TShape
      Left = 180
      Top = 72
      Width = 16
      Height = 16
      OnMouseDown = ShapeColorOneObjTableMouseDown
    end
    object ShapeColorTwoObjTable: TShape
      Left = 292
      Top = 50
      Width = 16
      Height = 16
      OnMouseDown = ShapeColorTwoObjTableMouseDown
    end
    object ShapeColorThreeObjTable: TShape
      Left = 292
      Top = 72
      Width = 16
      Height = 16
      OnMouseDown = ShapeColorThreeObjTableMouseDown
    end
    object CheckBoxFillTableWindows: TCheckBox
      Left = 18
      Top = 28
      Width = 142
      Height = 17
      Caption = #1047#1072#1082#1088#1072#1096#1080#1074#1072#1090#1100' '#1103#1095#1077#1081#1082#1080
      TabOrder = 0
      OnClick = CheckBoxFillTableWindowsClick
    end
    object CheckBoxApplyWindowsSeries: TCheckBox
      Left = 18
      Top = 89
      Width = 142
      Height = 17
      Caption = #1055#1088#1080#1084#1077#1085#1080#1090#1100' '#1082' '#1075#1088#1072#1092#1080#1082#1072#1084
      TabOrder = 1
      OnClick = CheckBoxApplyWindowsSeriesClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 255
    Width = 352
    Height = 98
    Caption = #1044#1074#1080#1078#1077#1085#1080#1077' '#1079#1074#1077#1079#1076
    TabOrder = 2
    object CheckBoxLabelFrame: TCheckBox
      Left = 18
      Top = 24
      Width = 207
      Height = 17
      Caption = #1054#1090#1086#1073#1088#1072#1078#1072#1090#1100' '#1087#1086#1076#1087#1080#1089#1080' '#1092#1088#1072#1075#1084#1077#1085#1090#1086#1074
      TabOrder = 0
      OnClick = CheckBoxLabelFrameClick
    end
    object CheckBoxFrameMv: TCheckBox
      Left = 174
      Top = 47
      Width = 134
      Height = 17
      Caption = #1047#1074#1077#1079#1076#1085#1072#1103' '#1074#1077#1083#1080#1095#1080#1085#1072
      TabOrder = 1
    end
    object CheckBoxFrameSp: TCheckBox
      Left = 174
      Top = 70
      Width = 134
      Height = 17
      Caption = #1057#1087#1077#1082#1090#1088#1072#1083#1100#1085#1099#1081' '#1082#1083#1072#1089#1089
      TabOrder = 2
    end
    object CheckBoxFrameStarID: TCheckBox
      Left = 40
      Top = 70
      Width = 97
      Height = 17
      Caption = #1053#1086#1084#1077#1088' '#1079#1074#1077#1079#1076#1099
      TabOrder = 3
    end
    object CheckBoxFrameID: TCheckBox
      Left = 40
      Top = 47
      Width = 120
      Height = 17
      Caption = #1053#1086#1084#1077#1088' '#1092#1088#1072#1075#1084#1077#1085#1090#1072
      TabOrder = 4
    end
  end
  object GroupBox4: TGroupBox
    Left = 8
    Top = 359
    Width = 352
    Height = 108
    Caption = #1057#1074#1086#1076#1085#1099#1077' '#1075#1088#1072#1092#1080#1082#1080
    TabOrder = 3
    object CheckBoxCurrentTime: TCheckBox
      Left = 18
      Top = 48
      Width = 191
      Height = 17
      Caption = #1054#1090#1086#1073#1088#1072#1078#1072#1090#1100' '#1090#1077#1082#1091#1097#1077#1077' '#1080#1079#1084#1077#1088#1077#1085#1080#1077
      TabOrder = 0
    end
    object RadioButton1: TRadioButton
      Left = 40
      Top = 71
      Width = 113
      Height = 17
      Caption = #1083#1080#1085#1080#1103
      TabOrder = 1
    end
    object RadioButton2: TRadioButton
      Left = 180
      Top = 71
      Width = 113
      Height = 17
      Caption = #1090#1086#1095#1082#1072
      TabOrder = 2
    end
    object CheckBoxDateTime: TCheckBox
      Left = 18
      Top = 25
      Width = 231
      Height = 17
      Caption = #1054#1090#1086#1073#1088#1072#1078#1077#1085#1080#1077' '#1076#1072#1090#1099' '#1080' '#1074#1088#1077#1084#1077#1085#1080' '#1087#1086' '#1086#1089#1080' OX'
      TabOrder = 3
    end
  end
  object GroupBox5: TGroupBox
    Left = 8
    Top = 473
    Width = 352
    Height = 74
    Caption = #1042#1099#1073#1086#1088' '#1092#1072#1081#1083#1086#1074
    TabOrder = 4
    object Label7: TLabel
      Left = 18
      Top = 24
      Width = 44
      Height = 13
      Caption = #1055#1088#1077#1092#1080#1082#1089
    end
    object Label8: TLabel
      Left = 18
      Top = 46
      Width = 57
      Height = 13
      Caption = #1053#1072#1095#1080#1085#1072#1090#1100' '#1089
    end
    object EditFilePrefix: TEdit
      Left = 104
      Top = 16
      Width = 121
      Height = 21
      TabOrder = 0
      Text = 'Img'
    end
    object BeginFromEdit: TEdit
      Left = 104
      Top = 43
      Width = 121
      Height = 21
      NumbersOnly = True
      TabOrder = 1
    end
    object SkipFrameCheckBox: TCheckBox
      Left = 231
      Top = 16
      Width = 106
      Height = 17
      Caption = #1055#1088#1086#1087#1091#1089#1082#1072#1090#1100' '#1082#1072#1076#1088
      TabOrder = 2
    end
  end
  object ColorDialog1: TColorDialog
    Left = 392
    Top = 40
  end
end
