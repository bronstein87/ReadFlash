object FormGraphOrient: TFormGraphOrient
  Left = 153
  Top = 48
  Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1086#1088#1080#1077#1085#1090#1072#1094#1080#1080
  ClientHeight = 825
  ClientWidth = 1184
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
  DesignSize = (
    1184
    825)
  PixelsPerInch = 96
  TextHeight = 13
  object Label10: TLabel
    Left = 139
    Top = 796
    Width = 96
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = #1042#1088#1077#1084#1103' '#1087#1088#1080#1074#1103#1079#1082#1080', '#1089
  end
  object Label11: TLabel
    Left = 426
    Top = 796
    Width = 44
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = #8470' '#1082#1072#1076#1088#1072
  end
  object ButtonAdd: TButton
    Left = 1040
    Top = 600
    Width = 75
    Height = 25
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100'...'
    TabOrder = 0
    Visible = False
  end
  object ButtonClear: TButton
    Left = 1097
    Top = 792
    Width = 75
    Height = 25
    Caption = #1054#1095#1080#1089#1090#1080#1090#1100
    TabOrder = 1
    Visible = False
  end
  object PageControl2: TPageControl
    Left = 16
    Top = 8
    Width = 1160
    Height = 777
    ActivePage = ShowFragmentTab
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
    object TabSheet5: TTabSheet
      Caption = #1059#1075#1083#1099' '#1086#1088#1080#1077#1085#1090#1072#1094#1080#1080
      DesignSize = (
        1152
        749)
      object ChartAl: TChart
        Left = 16
        Top = 6
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1055#1088#1103#1084#1086#1077' '#1074#1086#1089#1093#1086#1078#1076#1077#1085#1080#1077', '#1075#1088#1072#1076)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartDl: TChart
        Left = 16
        Top = 252
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1057#1082#1083#1086#1085#1077#1085#1080#1077', '#1075#1088#1072#1076)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartAz: TChart
        Left = 16
        Top = 497
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1040#1079#1080#1084#1091#1090', '#1075#1088#1072#1076)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
    end
    object TabSheet6: TTabSheet
      Caption = #1059#1075#1083#1086#1074#1072#1103' '#1089#1082#1086#1088#1086#1089#1090#1100
      ImageIndex = 1
      DesignSize = (
        1152
        749)
      object ChartWx: TChart
        Left = 16
        Top = 9
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'Wx, '#39'/c')
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartWy: TChart
        Left = 16
        Top = 252
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'Wy, '#39'/c')
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartWz: TChart
        Left = 16
        Top = 497
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'Wz, '#39'/c')
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
    end
    object TabSheet7: TTabSheet
      Caption = #1056#1072#1089#1089#1086#1075#1083#1072#1089#1086#1074#1072#1085#1080#1103
      ImageIndex = 2
      DesignSize = (
        1152
        749)
      object ChartMx: TChart
        Left = 16
        Top = 9
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'mx, '#1084#1082#1084)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartMy: TChart
        Left = 16
        Top = 252
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'my, '#1084#1082#1084)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartNst: TChart
        Left = 16
        Top = 497
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -15
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          'm_cur, '#1084#1082#1084)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
    end
    object TabSheet8: TTabSheet
      Caption = #1063#1080#1089#1083#1086' '#1086#1073#1098#1077#1082#1090#1086#1074
      ImageIndex = 3
      DesignSize = (
        1152
        749)
      object ChartFoc: TChart
        Left = 16
        Top = 9
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1092#1088#1072#1075#1072#1084#1077#1085#1090#1086#1074)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartX0: TChart
        Left = 16
        Top = 252
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1083#1086#1082#1072#1083#1080#1079#1086#1074#1072#1085#1085#1099#1093' '#1086#1073#1098#1077#1082#1090#1086#1074)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartY0: TChart
        Left = 16
        Top = 497
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1088#1072#1089#1087#1086#1079#1085#1072#1085#1085#1099#1093' '#1079#1074#1077#1079#1076)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
    end
    object TabSheet9: TTabSheet
      Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1080#1079#1086#1073#1088#1072#1078#1077#1085#1080#1103
      ImageIndex = 4
      DesignSize = (
        1152
        749)
      object ChartFone: TChart
        Left = 16
        Top = 8
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1092#1086#1085#1072', '#1045#1052#1056)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartNoise: TChart
        Left = 16
        Top = 252
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1096#1091#1084#1072' (1'#1057#1050#1054'), '#1045#1052#1056)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
      object ChartNloc: TChart
        Left = 16
        Top = 498
        Width = 1117
        Height = 250
        BackWall.Brush.Style = bsClear
        Legend.LegendStyle = lsSeries
        Legend.Shadow.HorizSize = 0
        Legend.Shadow.VertSize = 0
        Title.Font.Height = -13
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1058#1077#1084#1087#1077#1088#1072#1090#1091#1088#1072' '#1050#1052#1054#1055'-'#1084#1072#1090#1088#1080#1094#1099)
        BottomAxis.Title.Caption = #1042#1088#1077#1084#1103', '#1089
        LeftAxis.AxisValuesFormat = '#,##0.#####'
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        Anchors = [akLeft, akRight]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
      end
    end
    object TabSheet10: TTabSheet
      Caption = #1044#1074#1080#1078#1077#1085#1080#1077' '#1079#1074#1077#1079#1076
      ImageIndex = 5
      DesignSize = (
        1152
        749)
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
      object Chart1: TChart
        Left = 3
        Top = 41
        Width = 1146
        Height = 705
        Legend.Visible = False
        Title.Text.Strings = (
          'TChart')
        BottomAxis.Automatic = False
        BottomAxis.AutomaticMaximum = False
        BottomAxis.AutomaticMinimum = False
        BottomAxis.Maximum = 1024.000000000000000000
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.Maximum = 1024.000000000000000000
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        Anchors = [akLeft, akTop, akRight, akBottom]
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series9: TBubbleSeries
          Marks.Frame.Visible = False
          ClickableLine = False
          Pointer.Brush.Gradient.EndColor = 6144242
          Pointer.Gradient.EndColor = 6144242
          Pointer.HorizSize = 69
          Pointer.InflateMargins = False
          Pointer.Style = psCircle
          Pointer.VertSize = 69
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          RadiusValues.Name = 'Radius'
          RadiusValues.Order = loNone
        end
        object Series2: TLineSeries
          SeriesColor = clBlue
          Brush.BackColor = clDefault
          LinePen.Color = clBlue
          LinePen.Visible = False
          Pointer.Brush.Gradient.EndColor = clBlue
          Pointer.Gradient.EndColor = clBlue
          Pointer.InflateMargins = True
          Pointer.Style = psCircle
          Pointer.Visible = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
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
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          EndXValues.Name = 'EndX'
          EndXValues.Order = loNone
          EndYValues.Name = 'EndY'
          EndYValues.Order = loNone
          StartXValues.Name = 'X'
          StartXValues.Order = loAscending
          StartYValues.Name = 'Y'
          StartYValues.Order = loNone
        end
        object Series1: TLineSeries
          Marks.Emboss.Color = 8487297
          Marks.Shadow.Color = 8487297
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
      end
    end
    object TabSheet1: TTabSheet
      Caption = #1060#1088#1072#1075#1084#1077#1085#1090#1099
      ImageIndex = 6
      DesignSize = (
        1152
        749)
      object Chart2: TChart
        Left = 3
        Top = 3
        Width = 516
        Height = 250
        Legend.Alignment = laBottom
        Legend.Visible = False
        Title.Font.Color = clBlack
        Title.Font.Height = -15
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1092#1086#1085#1072', '#1045#1052#1056)
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series4: TBarSeries
          Marks.Visible = False
          SeriesColor = 16744576
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
        Legend.Alignment = laBottom
        Legend.Visible = False
        Title.Font.Color = clBlack
        Title.Font.Height = -15
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
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series6: TBarSeries
          Marks.Visible = False
          SeriesColor = 16744576
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
        Legend.Visible = False
        Title.Font.Color = clBlack
        Title.Font.Height = -15
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1059#1088#1086#1074#1077#1085#1100' '#1096#1091#1084#1072', '#1045#1052#1056)
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 2
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series5: TBarSeries
          Marks.Visible = False
          SeriesColor = 16744576
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object TableWindowsInfo: TStringGrid
        Left = 560
        Top = 32
        Width = 577
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
    object TabSheet2: TTabSheet
      Caption = #1054#1073#1098#1077#1082#1090#1099
      ImageIndex = 7
      DesignSize = (
        1152
        749)
      object Chart5: TChart
        Left = 3
        Top = 3
        Width = 516
        Height = 250
        Legend.Visible = False
        Title.Font.Color = clBlack
        Title.Font.Height = -15
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1048#1085#1090#1077#1075#1088#1072#1083#1100#1085#1072#1103' '#1103#1088#1082#1086#1089#1090#1100', '#1045#1052#1056)
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 0
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series7: TBarSeries
          Marks.Visible = False
          SeriesColor = 16744576
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
        Legend.Visible = False
        Title.Font.Color = clBlack
        Title.Font.Height = -15
        Title.Font.Style = [fsBold]
        Title.Text.Strings = (
          #1063#1080#1089#1083#1086' '#1101#1083#1077#1084#1077#1085#1090#1086#1074' '#1074' '#1086#1073#1098#1077#1082#1090#1077)
        View3D = False
        BevelOuter = bvNone
        Color = clWhite
        TabOrder = 1
        DefaultCanvas = 'TGDIPlusCanvas'
        ColorPaletteIndex = 13
        object Series8: TBarSeries
          Marks.Visible = False
          SeriesColor = 16744576
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Bar'
          YValues.Order = loNone
        end
      end
      object TableObjectsInfo: TStringGrid
        Left = 560
        Top = 32
        Width = 577
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
      object FragmentShowScrollBox: TScrollBox
        Left = 0
        Top = 0
        Width = 1152
        Height = 749
        Align = alClient
        TabOrder = 0
        OnResize = FragmentShowScrollBoxResize
      end
    end
  end
  object EditSizeX: TEdit
    Left = 1067
    Top = 5
    Width = 50
    Height = 21
    Anchors = [akTop, akRight]
    TabOrder = 3
    Text = '620'
  end
  object EditSizeY: TEdit
    Left = 1123
    Top = 5
    Width = 53
    Height = 21
    Anchors = [akTop, akRight]
    TabOrder = 4
    Text = '220'
  end
  object EditTimeDev: TEdit
    Left = 275
    Top = 796
    Width = 121
    Height = 21
    Anchors = [akLeft, akBottom]
    TabOrder = 5
    Text = '0'
  end
  object EditNumCadr: TEdit
    Left = 482
    Top = 796
    Width = 57
    Height = 21
    Anchors = [akLeft, akBottom]
    NumbersOnly = True
    TabOrder = 6
    Text = '0'
    OnChange = EditNumCadrChange
  end
  object UpDown1: TUpDown
    Left = 539
    Top = 796
    Width = 16
    Height = 21
    Anchors = [akLeft, akBottom]
    Associate = EditNumCadr
    Max = 10000
    TabOrder = 7
    OnClick = UpDown1Click
  end
  object SortFileCheckBox: TCheckBox
    Left = 944
    Top = 8
    Width = 117
    Height = 20
    Alignment = taLeftJustify
    BiDiMode = bdLeftToRight
    Caption = #1057#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1092#1072#1081#1083
    ParentBiDiMode = False
    TabOrder = 8
  end
  object Button1: TButton
    Left = 968
    Top = 792
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 9
  end
  object OpenDialogG: TOpenDialog
    Left = 1060
    Top = 792
  end
  object MainMenu1: TMainMenu
    Left = 56
    Top = 8
    object MenuOpen: TMenuItem
      Caption = #1042#1099#1073#1088#1072#1090#1100' '#1092#1072#1081#1083
      object MenuOpenFlash: TMenuItem
        Caption = #1044#1072#1085#1085#1099#1077' '#1080#1079' Flash-'#1087#1072#1084#1103#1090#1080' ('#1084#1041#1054#1050#1047'-2'#1042')'
        OnClick = MenuOpenFlashClick
      end
      object MenuOpenTMI: TMenuItem
        Caption = #1055#1088#1086#1090#1086#1082#1086#1083' '#1090#1077#1083#1077#1084#1077#1090#1088#1080#1080
        OnClick = MenuOpenTMIClick
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
  object OpenDialog1: TOpenDialog
    Filter = 'dat|*.dat'
    Left = 48
    Top = 64
  end
  object OpenDialog2: TOpenDialog
    Filter = 'txt|.txt'
    Left = 120
    Top = 72
  end
end
