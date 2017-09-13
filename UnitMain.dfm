object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = #1063#1090#1077#1085#1080#1077' '#1076#1072#1085#1085#1099#1093' '#1080#1079' flash-'#1087#1072#1084#1103#1090#1080
  ClientHeight = 825
  ClientWidth = 1184
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsMDIForm
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object MainMenu1: TMainMenu
    Left = 280
    object MenuLook: TMenuItem
      Caption = #1055#1088#1086#1089#1084#1086#1090#1088
      OnClick = MenuLookClick
    end
    object AnalyzeLook: TMenuItem
      Caption = #1040#1085#1072#1083#1080#1079
      OnClick = AnalyzeLookClick
    end
  end
end
