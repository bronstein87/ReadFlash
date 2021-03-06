object FormGraphOrient: TFormGraphOrient
  Left = 153
  Top = 48
  Anchors = [akTop]
  Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1086#1088#1080#1077#1085#1090#1072#1094#1080#1080
  ClientHeight = 799
  ClientWidth = 1271
  Color = clBtnFace
  Constraints.MinWidth = 825
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsMDIChild
  Menu = MainMenu1
  OldCreateOrder = False
  Visible = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnMouseWheelDown = FormMouseWheelDown
  OnMouseWheelUp = FormMouseWheelUp
  OnResize = FormResize
  DesignSize = (
    1271
    799)
  PixelsPerInch = 96
  TextHeight = 13
  object Label10: TLabel
    Left = 520
    Top = 771
    Width = 96
    Height = 13
    Margins.Bottom = 10
    Anchors = [akRight, akBottom]
    Caption = #1042#1088#1077#1084#1103' '#1087#1088#1080#1074#1103#1079#1082#1080', '#1089
    ExplicitLeft = 411
    ExplicitTop = 797
  end
  object Label11: TLabel
    Left = 762
    Top = 771
    Width = 44
    Height = 13
    Margins.Bottom = 10
    Anchors = [akRight, akBottom]
    Caption = #8470' '#1082#1072#1076#1088#1072
    ExplicitLeft = 653
    ExplicitTop = 797
  end
  object LabelFrameReport: TLabel
    Left = 16
    Top = 771
    Width = 3
    Height = 13
    Anchors = [akLeft, akBottom]
    ExplicitTop = 797
  end
  object LabelStatusReport: TLabel
    Left = 248
    Top = 776
    Width = 3
    Height = 13
    Anchors = [akLeft, akBottom]
  end
  object ButtonClear: TButton
    Left = 1167
    Top = 766
    Width = 75
    Height = 25
    Anchors = [akRight, akBottom]
    Caption = #1054#1095#1080#1089#1090#1080#1090#1100
    TabOrder = 0
    Visible = False
  end
  object EditSizeX: TEdit
    Left = 1040
    Top = 770
    Width = 50
    Height = 21
    Anchors = [akRight, akBottom]
    TabOrder = 1
    Text = '0'
  end
  object EditSizeY: TEdit
    Left = 1096
    Top = 770
    Width = 53
    Height = 21
    Anchors = [akRight, akBottom]
    TabOrder = 2
    Text = '220'
  end
  object EditTimeDev: TEdit
    Left = 622
    Top = 770
    Width = 121
    Height = 21
    Anchors = [akRight, akBottom]
    TabOrder = 3
    Text = '0'
  end
  object EditNumCadr: TEdit
    Left = 812
    Top = 770
    Width = 57
    Height = 21
    Anchors = [akRight, akBottom]
    NumbersOnly = True
    TabOrder = 4
    Text = '0'
    OnChange = EditNumCadrChange
  end
  object UpDown1: TUpDown
    Left = 869
    Top = 770
    Width = 16
    Height = 21
    Anchors = [akRight, akBottom]
    Associate = EditNumCadr
    Max = 99999999
    TabOrder = 5
    Thousands = False
  end
  object SortFileCheckBox: TCheckBox
    Left = 898
    Top = 771
    Width = 117
    Height = 20
    Alignment = taLeftJustify
    Anchors = [akRight, akBottom]
    BiDiMode = bdLeftToRight
    Caption = #1057#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1092#1072#1081#1083
    ParentBiDiMode = False
    TabOrder = 6
  end
  object PageControl1: TPageControl
    Left = -6
    Top = 8
    Width = 1281
    Height = 756
    ActivePage = TabSheetStatFrag2
    Anchors = [akLeft, akTop, akRight, akBottom]
    MultiLine = True
    TabOrder = 7
    object TabSheetAngles: TTabSheet
      Caption = #1059#1075#1083#1099' '#1086#1088#1080#1077#1085#1090#1072#1094#1080#1080
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartAl: TChart
        Left = 0
        Top = -1
        Width = 1270
        Height = 249
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1055#1088#1103#1084#1086#1077' '#1074#1086#1089#1093#1086#1078#1076#1077#1085#1080#1077', '#1075#1088#1072#1076)
        OnClickLegend = ChartOrientClickLegend
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
        PrintMargins = (
          15
          39
          15
          39)
        ColorPaletteIndex = 13
      end
      object ChartDl: TChart
        Left = 0
        Top = 236
        Width = 1270
        Height = 260
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1057#1082#1083#1086#1085#1077#1085#1080#1077', '#1075#1088#1072#1076)
        OnClickLegend = ChartOrientClickLegend
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
        PrintMargins = (
          15
          39
          15
          39)
        ColorPaletteIndex = 13
      end
      object ChartAz: TChart
        Left = 5
        Top = 481
        Width = 1265
        Height = 248
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1040#1079#1080#1084#1091#1090', '#1075#1088#1072#1076)
        OnClickLegend = ChartOrientClickLegend
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
    object TabSheetAnglesError: TTabSheet
      Caption = #1054#1096#1080#1073#1082#1072' '#1091#1075#1083#1086#1074' '#1086#1088#1080#1077#1085#1090#1072#1094#1080#1080
      ImageIndex = 10
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartAzError: TChart
        Left = 3
        Top = 472
        Width = 1259
        Height = 245
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1072#1079#1080#1084#1091#1090#1072', '#1091#1075#1083'. '#1089#1077#1082)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartDlError: TChart
        Left = 0
        Top = 240
        Width = 1262
        Height = 242
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1089#1082#1083#1086#1085#1077#1085#1080#1103', '#1091#1075#1083'. '#1089#1077#1082)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartAlError: TChart
        Left = 3
        Top = -3
        Width = 1267
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
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
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
    object TabSheetAxesError: TTabSheet
      Caption = #1054#1096#1080#1073#1082#1072' '#1086#1089#1077#1081' '#1055#1057#1050
      ImageIndex = 12
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartErrorOX: TChart
        Left = 3
        Top = -6
        Width = 1271
        Height = 242
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1087#1086' '#1086#1089#1080' '#1054#1061', '#1091#1075#1083'. '#1089#1077#1082)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
      object ChartErrorOY: TChart
        Left = 3
        Top = 229
        Width = 1271
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1087#1086' '#1086#1089#1080' '#1054'Y, '#1091#1075#1083'. '#1089#1077#1082)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
      object ChartErrorOZ: TChart
        Left = 3
        Top = 471
        Width = 1271
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1087#1086' '#1086#1089#1080' OZ, '#1091#1075#1083'. '#1089#1077#1082)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
    object TabSheetOmega: TTabSheet
      Caption = #1059#1075#1083#1086#1074#1072#1103' '#1089#1082#1086#1088#1086#1089#1090#1100
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartWx: TChart
        Left = 0
        Top = -3
        Width = 1270
        Height = 259
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'Wx, '#1091#1075#1083'. '#1089#1077#1082'/c')
        OnClickLegend = ChartOrientClickLegend
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        PrintMargins = (
          15
          39
          15
          39)
        ColorPaletteIndex = 13
      end
      object ChartWy: TChart
        Left = 0
        Top = 232
        Width = 1270
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'Wy, '#1091#1075#1083'. '#1089#1077#1082'/c')
        OnClickLegend = ChartOrientClickLegend
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartWz: TChart
        Left = 0
        Top = 465
        Width = 1274
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'Wz, '#1091#1075#1083'. '#1089#1077#1082'/c')
        OnClickLegend = ChartOrientClickLegend
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
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
    object TabSheetOmegaError: TTabSheet
      Caption = #1054#1096#1080#1073#1082#1072' '#1091#1075#1083#1086#1074#1086#1081' '#1089#1082#1086#1088#1086#1089#1090#1080
      ImageIndex = 11
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartWxError: TChart
        Left = 3
        Top = -6
        Width = 1267
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
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
      object ChartWyError: TChart
        Left = 3
        Top = 229
        Width = 1267
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' Wy, '#1091#1075#1083'. '#1089#1077#1082'/'#1089)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
      object ChartWzError: TChart
        Left = 3
        Top = 466
        Width = 1271
        Height = 254
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' Wz, '#1091#1075#1083'. '#1089#1077#1082'/'#1089)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
    object TabSheetMxy: TTabSheet
      Caption = #1056#1072#1089#1089#1086#1075#1083#1072#1089#1086#1074#1072#1085#1080#1103
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartMx: TChart
        Left = 3
        Top = 234
        Width = 1274
        Height = 259
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1089#1090#1072#1090#1086#1095#1085#1099#1077' '#1088#1072#1089#1089#1086#1075#1083#1072#1089#1086#1074#1072#1085#1080#1103' '#1087#1086' '#1086#1089#1080' OX, '#1084#1082#1084)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartMy: TChart
        Left = 3
        Top = 478
        Width = 1270
        Height = 232
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1089#1090#1072#1090#1086#1095#1085#1099#1077' '#1088#1072#1089#1089#1086#1075#1083#1072#1089#1086#1074#1072#1085#1080#1103' '#1087#1086' '#1086#1089#1080' OY, '#1084#1082#1084)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartMxy: TChart
        Left = 3
        Top = -4
        Width = 1270
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1089#1090#1072#1090#1086#1095#1085#1099#1077' '#1088#1072#1089#1089#1086#1075#1083#1072#1089#1086#1074#1072#1085#1080#1103', '#1084#1082#1084)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
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
    object TabSheetCountObjects: TTabSheet
      Caption = #1063#1080#1089#1083#1086' '#1086#1073#1098#1077#1082#1090#1086#1074
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartNumFrag: TChart
        Left = 3
        Top = -7
        Width = 1267
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1092#1088#1072#1075#1084#1077#1085#1090#1086#1074)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartNumLoc: TChart
        Left = 3
        Top = 231
        Width = 1271
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1083#1086#1082#1072#1083#1080#1079#1086#1074#1072#1085#1085#1099#1093' '#1086#1073#1098#1077#1082#1090#1086#1074)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartNumDet: TChart
        Left = 3
        Top = 465
        Width = 1267
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1088#1072#1089#1087#1086#1079#1085#1072#1085#1085#1099#1093' '#1079#1074#1077#1079#1076)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
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
    object TabSheetImage: TTabSheet
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1080#1079#1086#1073#1088#1072#1078#1077#1085#1080#1103
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartFone: TChart
        Left = 0
        Top = 232
        Width = 1270
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1092#1086#1085#1072', '#1045#1052#1056)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartNoise: TChart
        Left = -1
        Top = 470
        Width = 1274
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.CheckBoxes = True
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1096#1091#1084#1072' (1'#1057#1050#1054'), '#1045#1052#1056)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        Panning.MouseWheel = pmwNone
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartLevel: TChart
        Left = 0
        Top = -1
        Width = 1270
        Height = 250
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1055#1086#1088#1086#1075' '#1083#1086#1082#1072#1083#1080#1079#1072#1094#1080#1080', '#1045#1052#1056)
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        Panning.MouseWheel = pmwNone
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
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
      Caption = #1044#1086#1087#1086#1083#1085#1080#1090#1077#1083#1100#1085#1086
      ImageIndex = 17
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartTemp: TChart
        Left = 3
        Top = 230
        Width = 1270
        Height = 250
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' '#1050#1052#1054#1055'-'#1084#1072#1090#1088#1080#1094#1099)
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        Panning.MouseWheel = pmwNone
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartSunAngle: TChart
        Left = 3
        Top = -4
        Width = 1270
        Height = 250
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1075#1086#1083' '#1086#1087#1090#1080#1095#1077#1089#1082#1086#1081' '#1086#1089#1080' '#1082' '#1057#1086#1083#1085#1094#1091', '#1075#1088#1072#1076'.')
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        Panning.MouseWheel = pmwNone
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
    end
    object TabSheetStarsMotion: TTabSheet
      Caption = #1044#1074#1080#1078#1077#1085#1080#1077' '#1079#1074#1077#1079#1076
      ImageIndex = 5
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object Label4: TLabel
        Left = 344
        Top = 24
        Width = 32
        Height = 13
        Caption = 'Label4'
      end
      object Label3: TLabel
        Left = 344
        Top = 5
        Width = 32
        Height = 13
        Caption = 'Label3'
      end
      object Label2: TLabel
        Left = 23
        Top = 24
        Width = 32
        Height = 13
        Caption = 'Label2'
      end
      object Label1: TLabel
        Left = 23
        Top = 5
        Width = 32
        Height = 13
        Caption = 'Label1'
      end
      object Label5: TLabel
        Left = 664
        Top = 3
        Width = 32
        Height = 13
        Caption = 'Label5'
      end
      object Label6: TLabel
        Left = 664
        Top = 24
        Width = 32
        Height = 13
        Caption = 'Label6'
      end
      object Label12: TLabel
        Left = 571
        Top = 688
        Width = 46
        Height = 13
        Anchors = [akLeft, akBottom]
        Caption = #1052#1072#1089#1096#1090#1072#1073
        ExplicitTop = 709
      end
      object ChartMatrix: TChart
        AlignWithMargins = True
        Left = 3
        Top = 3
        Width = 1267
        Height = 704
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        LeftWall.Visible = False
        Legend.Alignment = laBottom
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.Shadow.Transparency = 0
        RightWall.Color = clWhite
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1044#1074#1080#1078#1077#1085#1080#1077' '#1079#1074#1105#1079#1076)
        OnClickLegend = ChartMatrixClickLegend
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.LabelStyle = talValue
        BottomAxis.Maximum = 672.835045698054700000
        BottomAxis.Minimum = -580.835045698054800000
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        Chart3DPercent = 1
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        Hover.Visible = False
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Axis.Color = 4210752
        LeftAxis.Grid.Color = 11119017
        LeftAxis.Inverted = True
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.Maximum = 7062.221492518250000000
        LeftAxis.Minimum = -6132.888159184916000000
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        Panning.MouseWheel = pmwNone
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        Shadow.Color = clWhite
        Shadow.Visible = False
        TopAxis.Automatic = False
        TopAxis.AutomaticMaximum = False
        TopAxis.AutomaticMinimum = False
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        View3DOptions.OrthoAngle = 0
        View3DOptions.Perspective = 0
        Zoom.MouseWheel = pmwNormal
        Align = alClient
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TTeeCanvas3D'
        PrintMargins = (
          15
          26
          15
          26)
        ColorPaletteIndex = 13
        object Label15: TLabel
          Left = 0
          Top = 0
          Width = 3
          Height = 13
        end
        object Series9: TBubbleSeries
          Marks.Font.Style = [fsBold]
          Marks.Frame.Visible = False
          Marks.Visible = True
          Marks.Arrow.Width = 0
          Marks.Arrow.Visible = False
          Marks.AutoPosition = False
          Marks.Callout.Arrow.Width = 0
          Marks.Callout.Arrow.Visible = False
          Marks.Callout.Distance = 20
          Marks.Tail.AutoAlign = False
          Marks.Tail.Align = ttaFar
          ClickableLine = False
          Pointer.Brush.Gradient.Angle = 38
          Pointer.Brush.Gradient.Direction = gdFromTopLeft
          Pointer.Brush.Gradient.EndColor = 10708548
          Pointer.Brush.Gradient.MidColor = 16059031
          Pointer.Brush.Gradient.StartColor = 14540754
          Pointer.Brush.Gradient.Visible = True
          Pointer.DarkPen = -42
          Pointer.Emboss.Transparency = 47
          Pointer.Emboss.Visible = True
          Pointer.Gradient.Angle = 38
          Pointer.Gradient.Direction = gdFromTopLeft
          Pointer.Gradient.EndColor = 10708548
          Pointer.Gradient.MidColor = 16059031
          Pointer.Gradient.StartColor = 14540754
          Pointer.Gradient.Visible = True
          Pointer.HorizSize = 13
          Pointer.InflateMargins = False
          Pointer.Pen.Color = 1358954496
          Pointer.Pen.Width = 0
          Pointer.Pen.Fill.Gradient.Direction = gdLeftRight
          Pointer.Pen.Fill.Gradient.Visible = True
          Pointer.Shadow.Color = clWhite
          Pointer.Shadow.HorizSize = 19
          Pointer.Shadow.SmoothBlur = 41
          Pointer.Shadow.Transparency = 100
          Pointer.Shadow.VertSize = 19
          Pointer.Shadow.Visible = True
          Pointer.Style = psCircle
          Pointer.VertSize = 13
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          RadiusValues.Name = 'Radius'
          RadiusValues.Order = loNone
        end
        object Series1: TLineSeries
          Marks.Emboss.Color = 8487297
          Marks.Font.Style = [fsBold]
          Marks.Frame.Visible = False
          Marks.Shadow.Color = 8487297
          Marks.Shadow.Visible = False
          Marks.Transparent = True
          Marks.Visible = True
          Marks.Arrow.Color = clBlack
          Marks.Arrow.Visible = False
          Marks.Callout.Arrow.Color = clBlack
          Marks.Callout.Arrow.Visible = False
          Marks.Callout.Distance = 5
          Marks.Callout.Length = 0
          SeriesColor = clYellow
          Brush.BackColor = clDefault
          LinePen.Color = clYellow
          LinePen.Visible = False
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Transparency = 42
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          Transparency = 47
        end
        object Series10: TArrowSeries
          Marks.Frame.Visible = False
          Marks.Transparent = True
          SeriesColor = clBlue
          ClickableLine = False
          Pointer.InflateMargins = False
          Pointer.Style = psRectangle
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          EndXValues.Name = 'EndX'
          EndXValues.Order = loNone
          EndYValues.Name = 'EndY'
          EndYValues.Order = loNone
          StartXValues.DateTime = False
          StartXValues.Name = 'X'
          StartXValues.Order = loAscending
          StartYValues.Name = 'Y'
          StartYValues.Order = loNone
        end
        object Series3: TArrowSeries
          Marks.Frame.Visible = False
          Marks.Transparent = True
          SeriesColor = clRed
          ClickableLine = False
          Pointer.Brush.Gradient.EndColor = clRed
          Pointer.Gradient.EndColor = clRed
          Pointer.InflateMargins = False
          Pointer.Style = psRectangle
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          EndXValues.Name = 'EndX'
          EndXValues.Order = loNone
          EndYValues.Name = 'EndY'
          EndYValues.Order = loNone
          StartXValues.DateTime = False
          StartXValues.Name = 'X'
          StartXValues.Order = loAscending
          StartYValues.Name = 'Y'
          StartYValues.Order = loNone
        end
      end
      object EditCountHistory: TEdit
        Left = 911
        Top = 686
        Width = 64
        Height = 21
        Anchors = [akLeft, akBottom]
        TabOrder = 1
        Text = '10'
      end
      object CheckBoxHistory: TCheckBox
        Left = 751
        Top = 688
        Width = 154
        Height = 17
        Anchors = [akLeft, akBottom]
        Caption = #1054#1090#1086#1073#1088#1072#1078#1072#1090#1100' '#1086#1073#1098#1077#1082#1090#1099' '#1087#1086' '
        TabOrder = 2
      end
      object EditScale: TEdit
        Left = 623
        Top = 686
        Width = 64
        Height = 21
        Anchors = [akLeft, akBottom]
        TabOrder = 3
        Text = '20'
      end
      object CheckBoxSaveScale: TCheckBox
        Left = 230
        Top = 688
        Width = 257
        Height = 15
        Anchors = [akLeft, akBottom]
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1084#1072#1089#1096#1090#1072#1073#1080#1088#1086#1074#1072#1085#1080#1077
        TabOrder = 4
      end
      object CheckBoxInterval: TCheckBox
        Left = 64
        Top = 686
        Width = 145
        Height = 17
        Anchors = [akLeft, akBottom]
        Caption = #1054#1090#1086#1073#1088#1072#1079#1080#1090#1100' '#1080#1085#1090#1077#1088#1074#1072#1083':'
        TabOrder = 5
      end
    end
    object TabSheetTableFrag: TTabSheet
      Caption = #1060#1088#1072#1075#1084#1077#1085#1090#1099
      ImageIndex = 6
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object Chart2: TChart
        Left = 3
        Top = 3
        Width = 516
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.Alignment = laBottom
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1092#1086#1085#1072', '#1045#1052#1056)
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
        object Series4: TBarSeries
          Marks.Visible = False
          Title = #1057#1088#1077#1076#1085#1077#1077
          Emboss.Color = 8882055
          Shadow.Color = 8882055
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object Chart3: TChart
        Left = 3
        Top = 500
        Width = 516
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.Alignment = laBottom
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1086#1073#1098#1077#1082#1090#1086#1074' '#1074' '#1086#1082#1085#1077)
        DepthAxis.Automatic = False
        DepthAxis.AutomaticMaximum = False
        DepthAxis.AutomaticMinimum = False
        DepthAxis.Maximum = 0.200000000000000000
        DepthAxis.Minimum = -0.800000000000000300
        DepthTopAxis.Automatic = False
        DepthTopAxis.AutomaticMaximum = False
        DepthTopAxis.AutomaticMinimum = False
        DepthTopAxis.Maximum = 0.200000000000000000
        DepthTopAxis.Minimum = -0.800000000000000300
        RightAxis.Automatic = False
        RightAxis.AutomaticMaximum = False
        RightAxis.AutomaticMinimum = False
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
        object Series6: TBarSeries
          Marks.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object Chart4: TChart
        Left = 3
        Top = 251
        Width = 516
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1096#1091#1084#1072', '#1045#1052#1056)
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
        object Series5: TBarSeries
          Marks.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object TableWindowsInfo: TStringGrid
        Left = 560
        Top = 32
        Width = 698
        Height = 689
        Anchors = [akLeft, akTop, akRight]
        ColCount = 8
        FixedCols = 0
        RowCount = 30
        TabOrder = 3
        OnDrawCell = TableWindowsInfoDrawCell
        ColWidths = (
          64
          64
          64
          64
          64
          64
          64
          64)
        RowHeights = (
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24)
      end
    end
    object TabSheetTableObject: TTabSheet
      Caption = #1054#1073#1098#1077#1082#1090#1099
      ImageIndex = 7
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object Chart5: TChart
        Left = 3
        Top = 3
        Width = 516
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1048#1085#1090#1077#1075#1088#1072#1083#1100#1085#1072#1103' '#1103#1088#1082#1086#1089#1090#1100', '#1045#1052#1056)
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
        object Series7: TBarSeries
          Marks.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object Chart6: TChart
        Left = 3
        Top = 251
        Width = 516
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1101#1083#1077#1084#1077#1085#1090#1086#1074' '#1074' '#1086#1073#1098#1077#1082#1090#1077)
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
        object Series8: TBarSeries
          Marks.Visible = False
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object TableObjectsInfo: TStringGrid
        Left = 560
        Top = 32
        Width = 698
        Height = 689
        Anchors = [akLeft, akTop, akRight]
        Color = clWhite
        ColCount = 8
        FixedCols = 0
        RowCount = 30
        TabOrder = 2
        OnDrawCell = TableObjectsInfoDrawCell
        ColWidths = (
          64
          64
          64
          64
          64
          64
          64
          64)
        RowHeights = (
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24
          24)
      end
    end
    object ShowFragmentTab: TTabSheet
      Caption = #1054#1090#1086#1073#1088#1072#1078#1077#1085#1080#1077' '#1092#1088#1072#1075#1084#1077#1085#1090#1086#1074
      ImageIndex = 8
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object FragmentShowScrollBox: TScrollBox
        Left = 0
        Top = 33
        Width = 1273
        Height = 677
        Align = alClient
        TabOrder = 0
        OnResize = FragmentShowScrollBoxResize
      end
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 1273
        Height = 33
        Align = alTop
        TabOrder = 1
        StyleElements = [seFont, seClient]
        object Label8: TLabel
          Left = 405
          Top = 12
          Width = 120
          Height = 13
          Margins.Bottom = 8
          Align = alCustom
          Caption = #1059#1074#1077#1083#1080#1095#1077#1085#1080#1077' '#1092#1088#1072#1075#1084#1077#1085#1090#1072
        end
        object Label9: TLabel
          Left = 611
          Top = 12
          Width = 84
          Height = 13
          Margins.Bottom = 8
          Caption = #1056#1072#1079#1084#1077#1088' '#1087#1080#1082#1089#1077#1083#1103
        end
        object Label7: TLabel
          Left = 769
          Top = 12
          Width = 81
          Height = 13
          Margins.Bottom = 8
          Caption = #1056#1072#1079#1084#1077#1088' '#1096#1088#1080#1092#1090#1072
        end
        object ContrastCheckBox: TCheckBox
          Left = 3
          Top = 10
          Width = 142
          Height = 17
          Align = alCustom
          Caption = #1057#1086#1093#1088#1072#1085#1103#1090#1100' '#1082#1086#1085#1090#1088#1072#1089#1090
          TabOrder = 0
        end
        object ScaleEdit: TEdit
          Left = 531
          Top = 9
          Width = 52
          Height = 21
          Margins.Bottom = 0
          Align = alCustom
          TabOrder = 1
          Text = '25'
          OnChange = ScaleEditChange
        end
        object UpDown2: TUpDown
          Left = 583
          Top = 9
          Width = 16
          Height = 21
          Margins.Bottom = 0
          Associate = ScaleEdit
          Position = 25
          TabOrder = 2
        end
        object PixelBrightCheckBox: TCheckBox
          Left = 143
          Top = 10
          Width = 114
          Height = 17
          Align = alCustom
          Caption = #1071#1088#1082#1086#1089#1090#1100' '#1087#1080#1082#1089#1077#1083#1103
          TabOrder = 3
          OnClick = PixelBrightCheckBoxClick
        end
        object PixelSizeEdit: TEdit
          Left = 701
          Top = 9
          Width = 40
          Height = 21
          Margins.Bottom = 0
          TabOrder = 4
          Text = '25'
          OnChange = PixelSizeEditChange
        end
        object UpDown3: TUpDown
          Left = 741
          Top = 9
          Width = 16
          Height = 21
          Margins.Bottom = 0
          Associate = PixelSizeEdit
          Position = 25
          TabOrder = 5
        end
        object FontSizeEdit: TEdit
          Left = 859
          Top = 9
          Width = 27
          Height = 21
          Margins.Bottom = 0
          TabOrder = 6
          Text = '10'
          OnChange = FontSizeEditChange
        end
        object UpDown4: TUpDown
          Left = 886
          Top = 9
          Width = 16
          Height = 21
          Margins.Bottom = 0
          Associate = FontSizeEdit
          Position = 10
          TabOrder = 7
        end
      end
      object CheckBoxLimit: TCheckBox
        Left = 280
        Top = 10
        Width = 97
        Height = 17
        Caption = #1042#1099#1096#1077' '#1087#1086#1088#1086#1075#1072
        TabOrder = 2
        OnClick = CheckBoxLimitClick
      end
    end
    object TabSheetStatFrag: TTabSheet
      Caption = #1057#1090#1072#1090#1080#1089#1090#1080#1082#1072' '#1087#1086' '#1092#1088#1072#1075#1084#1077#1085#1090#1072#1084
      ImageIndex = 9
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartFragErrX: TChart
        Left = 0
        Top = 16
        Width = 569
        Height = 361
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1087#1086' '#1086#1089#1080' OX, '#1084#1082#1084)
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartFragErrY: TChart
        Left = 3
        Top = 329
        Width = 566
        Height = 369
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1054#1096#1080#1073#1082#1072' '#1087#1086' '#1086#1089#1080' OY, '#1084#1082#1084)
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akBottom]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartFragBright: TChart
        Left = 696
        Top = 16
        Width = 569
        Height = 361
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1048#1085#1090#1077#1075#1088#1072#1083#1100#1085#1072#1103' '#1103#1088#1082#1086#1089#1090#1100', '#1045#1052#1056)
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        Anchors = [akTop, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartFragSizeEl: TChart
        Left = 696
        Top = 329
        Width = 566
        Height = 369
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1101#1083#1077#1084#1077#1085#1090#1086#1074' '#1074' '#1086#1073#1098#1077#1082#1090#1077)
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 3
        Anchors = [akRight, akBottom]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
    end
    object TabSheetStatFrag2: TTabSheet
      Caption = #1057#1090#1072#1090#1080#1089#1090#1080#1082#1072' '#1087#1086' '#1092#1088#1072#1075#1084#1077#1085#1090#1072#1084'-2'
      ImageIndex = 14
      DesignSize = (
        1273
        710)
      object ChartFragMean: TChart
        Left = 3
        Top = 4
        Width = 1270
        Height = 250
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1092#1086#1085#1072', '#1045#1052#1056)
        OnClickLegend = ChartFrag2ClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        Panning.MouseWheel = pmwNone
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartFragNoise: TChart
        Left = -1
        Top = 232
        Width = 1274
        Height = 250
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1096#1091#1084#1072' (1'#1057#1050#1054'), '#1045#1052#1056)
        OnClickLegend = ChartFrag2ClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        Panning.MouseWheel = pmwNone
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        Zoom.MouseWheel = pmwNormal
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartFragLevel: TChart
        Left = 3
        Top = 467
        Width = 1270
        Height = 250
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.CheckBoxes = True
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1055#1086#1088#1086#1075' '#1083#1086#1082#1072#1083#1080#1079#1072#1094#1080#1080', '#1045#1052#1056)
        OnClickLegend = ChartFrag2ClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        Panning.MouseWheel = pmwNone
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
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
    object TabSheetStatStars: TTabSheet
      Caption = #1057#1090#1072#1090#1080#1089#1090#1080#1082#1072' '#1087#1086' '#1079#1074#1077#1079#1076#1072#1084
      ImageIndex = 13
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      DesignSize = (
        1273
        710)
      object ChartBrightMv: TChart
        Left = 8
        Top = 24
        Width = 569
        Height = 361
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1047#1072#1074'-'#1090#1100' '#1080#1085#1090#1077#1075#1088'. '#1103#1088#1082#1086#1089#1090#1080' '#1086#1090' '#1079#1074'. '#1074#1077#1083'.')
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartBrightSize: TChart
        Left = 704
        Top = 24
        Width = 569
        Height = 361
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          ' '#1048#1085#1090#1077#1075#1088'. '#1103#1088#1082#1086#1089#1090#1100' '#1086#1090' '#1095#1080#1089#1083#1072' '#1101#1083'-'#1090#1086#1074)
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akTop, akRight]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartBrightSp: TChart
        Left = 704
        Top = 337
        Width = 566
        Height = 369
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1047#1072#1074'-'#1090#1100' '#1080#1085#1090#1077#1075#1088'. '#1103#1088#1082#1086#1089#1090#1080' '#1086#1090' '#1089#1087#1077#1082#1090#1088'. '#1082#1083#1072#1089#1089#1072)
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        Anchors = [akRight, akBottom]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartSizeMv: TChart
        Left = 11
        Top = 337
        Width = 566
        Height = 369
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1047#1072#1074'-'#1090#1100' '#1095#1080#1089#1083#1072' '#1101#1083'-'#1090#1086#1074' '#1086#1090' '#1079#1074'. '#1074#1077#1083'.')
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 3
        Anchors = [akLeft, akBottom]
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
    end
    object TabSheetTableStat: TTabSheet
      Caption = #1057#1074#1086#1076#1085#1072#1103' '#1089#1090#1072#1090#1080#1089#1090#1080#1082#1072
      ImageIndex = 15
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label13: TLabel
        Left = 1190
        Top = 712
        Width = 68
        Height = 13
        Caption = #1057#1086#1086#1090#1085#1086#1096#1077#1085#1080#1077
      end
      object TableStatInfo: TStringGrid
        Left = 3
        Top = 24
        Width = 700
        Height = 681
        FixedCols = 0
        TabOrder = 0
        ColWidths = (
          238
          61
          64
          64
          64)
        RowHeights = (
          24
          24
          24
          24
          24)
      end
      object TableStatusInfo: TStringGrid
        Left = 771
        Top = 24
        Width = 614
        Height = 677
        TabOrder = 1
        ColWidths = (
          64
          91
          64
          64
          64)
        RowHeights = (
          24
          29
          24
          24
          24)
      end
      object RatioEdit: TEdit
        Left = 1264
        Top = 707
        Width = 121
        Height = 21
        TabOrder = 2
      end
    end
    object TabSheetResStat: TTabSheet
      Caption = #1057#1090#1072#1090#1091#1089' '#1086#1088#1080#1077#1085#1090#1072#1094#1080#1080
      ImageIndex = 16
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object ChartStat: TChart
        Left = 3
        Top = 3
        Width = 678
        Height = 725
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        Legend.Visible = False
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1057#1090#1072#1090#1091#1089' '#1088#1077#1096#1077#1085#1080#1103' '#1079#1072#1076#1072#1095#1080)
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.LabelStyle = talMark
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
      object ChartCounter: TChart
        Left = 696
        Top = 3
        Width = 715
        Height = 725
        BackWall.Brush.Style = bsClear
        BackWall.Brush.Gradient.Direction = gdBottomTop
        BackWall.Brush.Gradient.EndColor = clWhite
        BackWall.Brush.Gradient.StartColor = 15395562
        BackWall.Brush.Gradient.Visible = True
        BackWall.Transparent = False
        Foot.Font.Color = clBlue
        Foot.Font.Name = 'Verdana'
        Gradient.Direction = gdBottomTop
        Gradient.EndColor = clWhite
        Gradient.MidColor = 15395562
        Gradient.StartColor = 15395562
        LeftWall.Color = 14745599
        Legend.Font.Name = 'Verdana'
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.Transparency = 0
        Legend.Shadow.VertSize = 0
        RightWall.Color = 14745599
        Title.Font.Height = -13
        Title.Font.Name = 'Verdana'
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1057#1095#1077#1090#1095#1080#1082#1080' '#1089#1090#1072#1090#1091#1089#1086#1074)
        OnClickLegend = ChartsFragClickLegend
        BottomAxis.Axis.Color = 4210752
        BottomAxis.Grid.Color = 11119017
        BottomAxis.LabelsFormat.Font.Name = 'Verdana'
        BottomAxis.TicksInner.Color = 11119017
        BottomAxis.Title.Font.Name = 'Verdana'
        DepthAxis.Axis.Color = 4210752
        DepthAxis.Grid.Color = 11119017
        DepthAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthAxis.TicksInner.Color = 11119017
        DepthAxis.Title.Font.Name = 'Verdana'
        DepthTopAxis.Axis.Color = 4210752
        DepthTopAxis.Grid.Color = 11119017
        DepthTopAxis.LabelsFormat.Font.Name = 'Verdana'
        DepthTopAxis.TicksInner.Color = 11119017
        DepthTopAxis.Title.Font.Name = 'Verdana'
        LeftAxis.Axis.Color = 4210752
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        LeftAxis.Grid.Color = 11119017
        LeftAxis.LabelsFormat.Font.Name = 'Verdana'
        LeftAxis.LabelStyle = talMark
        LeftAxis.TicksInner.Color = 11119017
        LeftAxis.Title.Font.Name = 'Verdana'
        RightAxis.Axis.Color = 4210752
        RightAxis.Grid.Color = 11119017
        RightAxis.LabelsFormat.Font.Name = 'Verdana'
        RightAxis.TicksInner.Color = 11119017
        RightAxis.Title.Font.Name = 'Verdana'
        TopAxis.Axis.Color = 4210752
        TopAxis.Grid.Color = 11119017
        TopAxis.LabelsFormat.Font.Name = 'Verdana'
        TopAxis.TicksInner.Color = 11119017
        TopAxis.Title.Font.Name = 'Verdana'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        DefaultCanvas = 'TTeeCanvas3D'
        ColorPaletteIndex = 13
      end
    end
  end
  object MainMenu1: TMainMenu
    Left = 1112
    Top = 192
    object MenuOpen: TMenuItem
      Caption = #1042#1099#1073#1088#1072#1090#1100' '#1092#1072#1081#1083
      object N1: TMenuItem
        Caption = #1055#1088#1086#1090#1086#1082#1086#1083#1099' '#1050#1048#1040
        object BOKZ60ParseProtocol: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052'60'
          OnClick = BOKZ60ParseProtocolClick
        end
        object BOKZM2VParseProtocol: TMenuItem
          Caption = #1084#1041#1054#1050#1047'-2'#1042
          OnClick = BOKZM2VParseProtocolClick
        end
        object BOKZM601000ParseProtocol: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052'60/1000'
          OnClick = BOKZM601000ParseProtocolClick
        end
        object BOKZMFParseProtocol: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052#1060
          OnClick = BOKZMFParseProtocolClick
        end
        object BOKZM2ParseProtocol: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052'2'
          OnClick = BOKZM2ParseProtocolClick
        end
        object BOKZMParse: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052
          OnClick = BOKZMParseClick
        end
        object BOKZM60MILParse: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052'60('#1052#1048#1051')'
        end
        object BOKZM60KondorParse: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052'60('#1050#1086#1085#1076#1086#1088')'
          OnClick = BOKZM60KondorParseClick
        end
        object N3: TMenuItem
          Caption = #1040#1048#1057'-'#1069
          OnClick = N3Click
        end
      end
      object N2: TMenuItem
        Caption = #1055#1088#1086#1090#1086#1082#1086#1083#1099' '#1058#1052#1048
        object MenuOpenEnergy: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052' ('#1069#1085#1077#1088#1075#1080#1103')'
          OnClick = MenuOpenEnergyClick
        end
        object MenuOpenSamspace: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052'60, M1000('#1057#1072#1084#1072#1088#1072')'
          OnClick = MenuOpenSamspaceClick
        end
        object MenuOpenArsenal: TMenuItem
          Caption = #1041#1054#1050#1047'-'#1052'60, '#1052'1000 ('#1052#1050#1054')'
          OnClick = MenuOpenMILClick
        end
        object MenuOpenEMKA: TMenuItem
          Caption = #1084#1041#1054#1050#1047'-2'#1042' ('#1042#1053#1048#1048#1069#1052')'
          OnClick = MenuOpenEMKAClick
        end
      end
      object ReadIKIFormat: TMenuItem
        Caption = #1044#1072#1085#1085#1099#1077' '#1080#1079' '#1092#1086#1088#1084#1072#1090#1072' '#1048#1050#1048
        object ChooseIKIFilesThenFolders: TMenuItem
          Caption = #1042#1099#1073#1088#1072#1090#1100' '#1092#1072#1081#1083#1099
          OnClick = ChooseIKIFilesThenFoldersClick
        end
        object ChooseIKIFolders: TMenuItem
          Caption = #1042#1099#1073#1088#1072#1090#1100' '#1087#1072#1087#1082#1080
          OnClick = ChooseIKIFoldersClick
        end
      end
      object MenuOpenFlash: TMenuItem
        Caption = #1044#1072#1085#1085#1099#1077' '#1080#1079' Flash-'#1087#1072#1084#1103#1090#1080' ('#1084#1041#1054#1050#1047'-2'#1042')'
        OnClick = MenuOpenFlashClick
      end
      object N21: TMenuItem
        Caption = #1058#1077#1083#1077#1084#1077#1090#1088#1080#1103' '#1041#1054#1050#1047'-'#1052'2 ('#1042#1086#1089#1090#1086#1095#1085#1099#1081')'
        OnClick = N21Click
      end
    end
    object MenuSave: TMenuItem
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
      OnClick = MenuSaveClick
    end
    object MenuOptions: TMenuItem
      Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1080
      OnClick = MenuOptionsClick
    end
    object MenuClear: TMenuItem
      Caption = #1054#1095#1080#1089#1090#1080#1090#1100
      OnClick = MenuClearClick
    end
  end
  object OpenDialog: TOpenDialog
    Left = 1112
    Top = 136
  end
  object ChartEditor1: TChartEditor
    GalleryHeight = 0
    GalleryWidth = 0
    Height = 0
    Width = 0
    Left = 1112
    Top = 304
  end
  object FileOpenDialog1: TFileOpenDialog
    FavoriteLinks = <>
    FileTypes = <>
    Options = []
    Left = 1112
    Top = 248
  end
  object PopupMenu1: TPopupMenu
    Left = 1112
    Top = 88
    object SaveSeriesData: TMenuItem
      Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100' '#1076#1072#1085#1085#1099#1077' '#1089' '#1075#1088#1072#1092#1080#1082#1072' '#1074' .txt'
      OnClick = SaveSeriesDataClick
    end
  end
end
