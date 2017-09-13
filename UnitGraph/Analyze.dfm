object AnalyzeForm: TAnalyzeForm
  Left = 0
  Top = 0
  Caption = #1040#1085#1072#1083#1080#1079' '#1086#1096#1080#1073#1086#1082
  ClientHeight = 743
  ClientWidth = 1214
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsMDIChild
  Menu = MainMenu1
  OldCreateOrder = False
  Visible = True
  OnClose = FormClose
  DesignSize = (
    1214
    743)
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1217
    Height = 745
    ActivePage = TabSheet1
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    object TabSheet2: TTabSheet
      Caption = #1040#1085#1072#1083#1080#1079' '#1086#1096#1080#1073#1086#1082' '#1089#1082#1086#1088#1086#1089#1090#1077#1081
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1209
        717)
      object ChartAnalyzeXV: TChart
        Left = 3
        Top = 0
        Width = 1206
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          ' '#1054#1096#1080#1073#1082#1072' Wx, '#1091#1075#1083'. '#1089#1077#1082'/'#1089)
        BottomAxis.Title.Caption = #1059#1075#1083#1086#1074#1072#1103' '#1089#1082#1086#1088#1086#1089#1090#1100', '#1075#1088#1072#1076'/c'
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        TopAxis.Automatic = False
        TopAxis.AutomaticMaximum = False
        TopAxis.AutomaticMinimum = False
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartAnalyzeYV: TChart
        Left = -7
        Top = 240
        Width = 1213
        Height = 249
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          ' '#1054#1096#1080#1073#1082#1072' Wy, '#1091#1075#1083'. '#1089#1077#1082'/'#1089)
        BottomAxis.Title.Caption = #1059#1075#1083#1086#1074#1072#1103' '#1089#1082#1086#1088#1086#1089#1090#1100', '#1075#1088#1072#1076'/c'
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        TopAxis.Automatic = False
        TopAxis.AutomaticMaximum = False
        TopAxis.AutomaticMinimum = False
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartAnalyzeZV: TChart
        Left = 0
        Top = 480
        Width = 1206
        Height = 241
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          ' '#1054#1096#1080#1073#1082#1072' Wz, '#1091#1075#1083'. '#1089#1077#1082'/'#1089)
        BottomAxis.Title.Caption = #1059#1075#1083#1086#1074#1072#1103' '#1089#1082#1086#1088#1086#1089#1090#1100', '#1075#1088#1072#1076'/c'
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        TopAxis.Automatic = False
        TopAxis.AutomaticMaximum = False
        TopAxis.AutomaticMinimum = False
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
    end
    object TabSheet1: TTabSheet
      Caption = #1040#1085#1072#1083#1080#1079' '#1086#1096#1080#1073#1086#1082' '#1091#1075#1083#1086#1074
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1209
        717)
      object ChartAnalyzeErrorAl: TChart
        Left = -4
        Top = 0
        Width = 1213
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1087#1088#1103#1084#1086#1075#1086' '#1074#1086#1089#1093#1086#1078#1076#1077#1085#1080#1103', '#1091#1075#1083'. '#1089#1077#1082)
        BottomAxis.Title.Caption = #1059#1075#1083#1086#1074#1072#1103' '#1089#1082#1086#1088#1086#1089#1090#1100', '#1075#1088#1072#1076'/c'
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        TopAxis.Automatic = False
        TopAxis.AutomaticMaximum = False
        TopAxis.AutomaticMinimum = False
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartAnalyzeErrorDl: TChart
        Left = -4
        Top = 248
        Width = 1213
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1089#1082#1083#1086#1085#1077#1085#1080#1103', '#1091#1075#1083'. '#1089#1077#1082)
        BottomAxis.Title.Caption = #1059#1075#1083#1086#1074#1072#1103' '#1089#1082#1086#1088#1086#1089#1090#1100', '#1075#1088#1072#1076'/c'
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        TopAxis.Automatic = False
        TopAxis.AutomaticMaximum = False
        TopAxis.AutomaticMinimum = False
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartAnalyzeErrorAz: TChart
        Left = -4
        Top = 467
        Width = 1213
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1072#1079#1080#1084#1091#1090#1072', '#1091#1075#1083'. '#1089#1077#1082)
        BottomAxis.Title.Caption = #1059#1075#1083#1086#1074#1072#1103' '#1089#1082#1086#1088#1086#1089#1090#1100', '#1075#1088#1072#1076'/c'
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        TopAxis.Automatic = False
        TopAxis.AutomaticMaximum = False
        TopAxis.AutomaticMinimum = False
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        PrintMargins = (
          15
          39
          15
          39)
        ColorPaletteIndex = 13
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 1048
    object ChooseDirectories: TMenuItem
      Caption = #1042#1099#1073#1088#1072#1090#1100' '#1076#1080#1088#1077#1082#1090#1086#1088#1080#1080
      OnClick = ChooseDirectoriesClick
    end
    object SaveGraphs: TMenuItem
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      OnClick = SaveGraphsClick
    end
    object ClearGraphs: TMenuItem
      Caption = #1054#1095#1080#1089#1090#1080#1090#1100
      OnClick = ClearGraphsClick
    end
  end
  object FileOpenDialog1: TFileOpenDialog
    FavoriteLinks = <>
    FileTypes = <>
    Options = []
    Left = 1128
    Top = 32
  end
end
