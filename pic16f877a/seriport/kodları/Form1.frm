VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form Form1 
   Caption         =   "seri iletiþim alýcý verici - Remzi Þahinoðlu"
   ClientHeight    =   7035
   ClientLeft      =   9240
   ClientTop       =   1710
   ClientWidth     =   7560
   LinkTopic       =   "Form1"
   ScaleHeight     =   7035
   ScaleWidth      =   7560
   Begin VB.CheckBox Check1 
      Caption         =   "sayý deðeri gönder"
      Height          =   375
      Left            =   3720
      TabIndex        =   19
      Top             =   4920
      Width           =   1815
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   3600
      Top             =   5760
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      DTREnable       =   -1  'True
   End
   Begin VB.CommandButton Command4 
      Caption         =   "BAÐLANTIYI KES"
      Height          =   375
      Left            =   6000
      TabIndex        =   17
      Top             =   6240
      Width           =   1455
   End
   Begin VB.CommandButton Command3 
      Caption         =   "BAÐLAN"
      Height          =   375
      Left            =   6000
      TabIndex        =   16
      Top             =   5760
      Width           =   1455
   End
   Begin VB.Frame Frame2 
      Caption         =   "port seçimi"
      Height          =   1935
      Left            =   2040
      TabIndex        =   11
      Top             =   4800
      Width           =   1335
      Begin VB.OptionButton Option8 
         Caption         =   "COM4"
         Height          =   195
         Left            =   120
         TabIndex        =   15
         Top             =   1440
         Width           =   855
      End
      Begin VB.OptionButton Option7 
         Caption         =   "COM3"
         Height          =   195
         Left            =   120
         TabIndex        =   14
         Top             =   1080
         Width           =   855
      End
      Begin VB.OptionButton Option6 
         Caption         =   "COM2"
         Height          =   195
         Left            =   120
         TabIndex        =   13
         Top             =   720
         Width           =   855
      End
      Begin VB.OptionButton Option5 
         Caption         =   "COM1"
         Height          =   195
         Left            =   120
         TabIndex        =   12
         Top             =   360
         Width           =   975
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "saniyedeki bit sayýsý"
      Height          =   1935
      Left            =   120
      TabIndex        =   6
      Top             =   4800
      Width           =   1695
      Begin VB.OptionButton Option4 
         Caption         =   "9600"
         Height          =   195
         Left            =   240
         TabIndex        =   10
         Top             =   1440
         Width           =   855
      End
      Begin VB.OptionButton Option3 
         Caption         =   "4800"
         Height          =   195
         Left            =   240
         TabIndex        =   9
         Top             =   1080
         Width           =   855
      End
      Begin VB.OptionButton Option2 
         Caption         =   "2400"
         Height          =   195
         Left            =   240
         TabIndex        =   8
         Top             =   720
         Width           =   855
      End
      Begin VB.OptionButton Option1 
         Caption         =   "1200"
         Height          =   195
         Left            =   240
         TabIndex        =   7
         Top             =   360
         Width           =   855
      End
   End
   Begin VB.CommandButton Command2 
      Caption         =   "temizle"
      Height          =   1935
      Left            =   6600
      TabIndex        =   5
      Top             =   2760
      Width           =   735
   End
   Begin VB.Timer Timer1 
      Interval        =   1
      Left            =   3600
      Top             =   6480
   End
   Begin VB.TextBox Text1 
      Height          =   2055
      Left            =   120
      MultiLine       =   -1  'True
      TabIndex        =   2
      Text            =   "Form1.frx":0000
      Top             =   360
      Width           =   6255
   End
   Begin VB.TextBox Text2 
      Height          =   1935
      Left            =   120
      MultiLine       =   -1  'True
      TabIndex        =   1
      Top             =   2760
      Width           =   6255
   End
   Begin VB.CommandButton Command1 
      Caption         =   "veriyi gönder"
      Height          =   2055
      Left            =   6600
      TabIndex        =   0
      Top             =   360
      Width           =   735
   End
   Begin VB.Label Label3 
      Caption         =   "durum"
      Height          =   255
      Left            =   6360
      TabIndex        =   18
      Top             =   6720
      Width           =   615
   End
   Begin VB.Shape Shape1 
      BackColor       =   &H00C0C0C0&
      BackStyle       =   1  'Opaque
      Height          =   255
      Left            =   6000
      Shape           =   2  'Oval
      Top             =   6720
      Width           =   255
   End
   Begin VB.Label Label2 
      Caption         =   "alýcý"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   2520
      Width           =   375
   End
   Begin VB.Label Label1 
      Caption         =   "verici"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   495
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim port As Integer

Private Sub Command1_Click()
    If MSComm1.PortOpen = True Then
        If Check1.Value = 1 Then
            MSComm1.Output = Chr(Val(Text1.Text))    'veriyi sayý deðeri olarakgönder
        Else
            MSComm1.Output = Text1.Text         'veriyi metin olarak gönder
        End If
    End If
End Sub

Private Sub Command2_Click()
    Text2.Text = ""
End Sub

Private Sub Command3_Click()
    Select Case port
        Case 1
            MSComm1.CommPort = 1
            MSComm1.PortOpen = True
        Case 2
            MSComm1.CommPort = 2
            MSComm1.PortOpen = True
        Case 3
            MSComm1.CommPort = 3
            MSComm1.PortOpen = True
        Case 4
            MSComm1.CommPort = 4
            MSComm1.PortOpen = True
    End Select
    Command3.Enabled = False
    Command4.Enabled = True
    Shape1.BackColor = vbGreen
    Timer1.Enabled = True
    Frame2.Enabled = False
End Sub

Private Sub Command4_Click()
    Select Case port
        Case 1
            MSComm1.PortOpen = False
        Case 2
            MSComm1.PortOpen = False
        Case 3
            MSComm1.PortOpen = False
        Case 4
            MSComm1.PortOpen = False
    End Select
    Command3.Enabled = True
    Command4.Enabled = False
    Shape1.BackColor = vbRed
    Timer1.Enabled = False
    Frame2.Enabled = True
End Sub

Private Sub Form_Load()
    Timer1.Enabled = False
    Timer1.Interval = 1
    Option4.Value = True
    Option5.Value = True
    Shape1.BackColor = vbRed
    Command4.Enabled = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    If MSComm1.PortOpen = True Then MSComm1.PortOpen = False       ' portu kapa
End Sub

Private Sub Option1_Click()
    MSComm1.Settings = "1200,N,8,1"
End Sub

Private Sub Option2_Click()
    MSComm1.Settings = "2400,N,8,1"
End Sub

Private Sub Option3_Click()
    MSComm1.Settings = "4800,N,8,1"
End Sub

Private Sub Option4_Click()
    MSComm1.Settings = "9600,N,8,1"
End Sub

Private Sub Option5_Click()
    port = 1
End Sub

Private Sub Option6_Click()
    port = 2
End Sub

Private Sub Option7_Click()
    port = 3
End Sub

Private Sub Option8_Click()
    port = 4
End Sub

Private Sub Timer1_Timer()
    Text2.Text = Text2.Text & MSComm1.Input  'veriyi al
End Sub
