object FormDevice: TFormDevice
  Left = 0
  Top = 0
  Caption = 'FormDevice'
  ClientHeight = 530
  ClientWidth = 798
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 16
    Width = 76
    Height = 13
    Caption = #1053#1086#1084#1077#1088' '#1087#1088#1080#1073#1086#1088#1072
  end
  object ComboBoxDeviceIndex: TComboBox
    Left = 120
    Top = 13
    Width = 145
    Height = 21
    ItemIndex = 0
    TabOrder = 0
    Text = '1'
    Items.Strings = (
      '1'
      '2'
      '3'
      '4')
  end
  object PageControl1: TPageControl
    Left = 8
    Top = 48
    Width = 782
    Height = 474
    ActivePage = TabSheetMshi
    TabOrder = 1
    object TabSheetMshi: TTabSheet
      Caption = #1052#1064#1048#1054#1056
      ExplicitHeight = 414
    end
    object TabSheetShtmi1: TTabSheet
      Caption = #1064#1058#1052#1048'1'
      ImageIndex = 1
      ExplicitHeight = 414
    end
    object TabSheetShtmi2: TTabSheet
      Caption = #1064#1058#1052#1048'2'
      ImageIndex = 2
      ExplicitHeight = 414
    end
    object TabSheetDtmi: TTabSheet
      Caption = #1044#1058#1052#1048
      ImageIndex = 3
      ExplicitHeight = 414
    end
  end
end
