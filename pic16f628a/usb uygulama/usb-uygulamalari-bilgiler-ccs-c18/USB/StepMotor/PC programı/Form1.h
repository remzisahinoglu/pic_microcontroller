#pragma once

#include <windows.h>	
#include <Dbt.h>		
#include "mpusbapi.h"	

#define DeviceVID_PID "vid_04d8&pid_0c00"
#define olcum_al  0x02					//ADC degerlerini oku
#define surum_oku 0x03					//Yazýlým sürümünü oku
#define motor_sur 0x04					

#define durdur     0
#define sol        1
#define sag        2

namespace UsbStepMotor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;	
	using namespace System::Runtime::InteropServices;  

	#ifdef UNICODE
	#define	Seeifdef	Unicode
	#else
	#define Seeifdef	Ansi
	#endif

#pragma region DLL imports
	//mpusbapi.dll içindeki USB fonksiyonlarý import ediliyor
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBGetDLLVersion")] 
	extern "C" DWORD MPUSBGetDLLVersion(void);
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBGetDeviceCount")] 
	extern "C" DWORD MPUSBGetDeviceCount(PCHAR pVID_PID);
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBOpen")]
	extern "C" HANDLE MPUSBOpen(DWORD instance,	//  Input
										PCHAR pVID_PID,	// Input
										PCHAR pEP,		// Input
										DWORD dwDir,	// Input
										DWORD dwReserved);// Input

	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBClose")] 
	extern "C" BOOL MPUSBClose(HANDLE handle);	//Input
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBRead")] 
	extern "C" DWORD MPUSBRead(HANDLE handle,	// Input
										PVOID pData,	// Output
										DWORD dwLen,	// Input
										PDWORD pLength,	// Output
										DWORD dwMilliseconds);// Input

	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBWrite")] 
	extern "C" DWORD MPUSBWrite(HANDLE handle,	// Input
										PVOID pData,	// Output
										DWORD dwLen,	// Input
										PDWORD pLength,	// Output
										DWORD dwMilliseconds);// Input
	[DllImport("MPUSBAPI.dll" , EntryPoint="_MPUSBReadInt")] 
	extern "C" DWORD MPUSBReadInt(HANDLE handle,	// Input
										PVOID pData,	// Output
										DWORD dwLen,	// Input
										PDWORD pLength,	// Output
										DWORD dwMilliseconds);// Input

	// WM_DEVICECHANGE mesajlarýnýn alýnabilmesi için user32.dll import edilmeli
	[DllImport("user32.dll" , CharSet = CharSet::Seeifdef, EntryPoint="RegisterDeviceNotification")]					
	extern "C" HDEVNOTIFY WINAPI RegisterDeviceNotificationUM(
		HANDLE hRecipient,
		LPVOID NotificationFilter,
		DWORD Flags);
#pragma endregion

	HANDLE  EP1INHandle = INVALID_HANDLE_VALUE;
	HANDLE  EP1OUTHandle = INVALID_HANDLE_VALUE;
	BOOL AttachedState = FALSE;	

	unsigned char yon = 0;	
	unsigned char motor = 1;	 // Surulecek motor	
	unsigned char sureMetot = 0; // Elle ya da zaman ayarlý

	bool durum = false;
	bool butDurum = false;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			CheckForIllegalCrossThreadCalls = false;
			GUID InterfaceClassGuid = {0xa5dcbf10, 0x6530, 0x11d2, 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED}; //Globally Unique Identifier (GUID) for USB peripheral devices
			
			//Register for WM_DEVICECHANGE notifications:
			DEV_BROADCAST_DEVICEINTERFACE MyDeviceBroadcastHeader;// = new DEV_BROADCAST_HDR;
			MyDeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
			MyDeviceBroadcastHeader.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
			MyDeviceBroadcastHeader.dbcc_reserved = 0;	//Reserved says not to use...
			MyDeviceBroadcastHeader.dbcc_classguid = InterfaceClassGuid;
			RegisterDeviceNotificationUM((HANDLE)this->Handle, &MyDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			//Programdan çýkarken açýk baðlantýlarý kapat
			if (EP1OUTHandle != INVALID_HANDLE_VALUE)
				MPUSBClose (EP1OUTHandle);
			if (EP1INHandle != INVALID_HANDLE_VALUE)
				MPUSBClose (EP1INHandle);

			if (components)
			{
				delete components;
			}
		}


	protected: 
	private: System::ComponentModel::BackgroundWorker^  ReadWriteThread;
	private: System::Windows::Forms::Timer^  timer1;


	private: System::Windows::Forms::Button^  button1;



	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel3;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel4;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::Button^  button4;
	private: System::Windows::Forms::TrackBar^  trackBar1;
	private: System::Windows::Forms::TrackBar^  trackBar2;
	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Splitter^  splitter1;


	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->ReadWriteThread = (gcnew System::ComponentModel::BackgroundWorker());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel4 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->splitter1 = (gcnew System::Windows::Forms::Splitter());
			this->statusStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->BeginInit();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// ReadWriteThread
			// 
			this->ReadWriteThread->WorkerReportsProgress = true;
			this->ReadWriteThread->WorkerSupportsCancellation = true;
			this->ReadWriteThread->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::ReadWriteThread_DoWork);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 8;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"MS Reference Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->button1->Location = System::Drawing::Point(26, 36);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(339, 27);
			this->button1->TabIndex = 14;
			this->button1->Text = L"B a ð l a n";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->toolStripStatusLabel1, 
				this->toolStripStatusLabel2, this->toolStripStatusLabel3, this->toolStripStatusLabel4});
			this->statusStrip1->Location = System::Drawing::Point(0, 326);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(385, 22);
			this->statusStrip1->TabIndex = 17;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
			this->toolStripStatusLabel1->AutoSize = false;
			this->toolStripStatusLabel1->Name = L"toolStripStatusLabel1";
			this->toolStripStatusLabel1->Size = System::Drawing::Size(45, 17);
			this->toolStripStatusLabel1->Text = L"Durum :";
			// 
			// toolStripStatusLabel2
			// 
			this->toolStripStatusLabel2->AutoSize = false;
			this->toolStripStatusLabel2->Name = L"toolStripStatusLabel2";
			this->toolStripStatusLabel2->Size = System::Drawing::Size(109, 17);
			// 
			// toolStripStatusLabel3
			// 
			this->toolStripStatusLabel3->AutoSize = false;
			this->toolStripStatusLabel3->Name = L"toolStripStatusLabel3";
			this->toolStripStatusLabel3->Size = System::Drawing::Size(109, 17);
			this->toolStripStatusLabel3->Text = L"Copyright  :";
			// 
			// toolStripStatusLabel4
			// 
			this->toolStripStatusLabel4->AutoSize = false;
			this->toolStripStatusLabel4->Name = L"toolStripStatusLabel4";
			this->toolStripStatusLabel4->Size = System::Drawing::Size(109, 17);
			this->toolStripStatusLabel4->Text = L"tmcone (c) 2009";
			// 
			// button2
			// 
			this->button2->Enabled = false;
			this->button2->Location = System::Drawing::Point(157, 143);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 18;
			this->button2->Text = L"Sol";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// button3
			// 
			this->button3->Enabled = false;
			this->button3->Location = System::Drawing::Point(239, 143);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 19;
			this->button3->Text = L"Sað";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &Form1::button3_Click);
			// 
			// button4
			// 
			this->button4->Enabled = false;
			this->button4->Location = System::Drawing::Point(157, 172);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(157, 23);
			this->button4->TabIndex = 20;
			this->button4->Text = L"Durdur";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &Form1::button4_Click);
			// 
			// trackBar1
			// 
			this->trackBar1->Location = System::Drawing::Point(16, 91);
			this->trackBar1->Maximum = 80;
			this->trackBar1->Minimum = 45;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->trackBar1->Size = System::Drawing::Size(45, 104);
			this->trackBar1->TabIndex = 21;
			this->trackBar1->Value = 65;
			// 
			// trackBar2
			// 
			this->trackBar2->Location = System::Drawing::Point(85, 91);
			this->trackBar2->Maximum = 250;
			this->trackBar2->Minimum = 30;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Orientation = System::Windows::Forms::Orientation::Vertical;
			this->trackBar2->Size = System::Drawing::Size(45, 104);
			this->trackBar2->TabIndex = 22;
			this->trackBar2->Value = 105;
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Checked = true;
			this->radioButton1->Location = System::Drawing::Point(157, 110);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(31, 17);
			this->radioButton1->TabIndex = 23;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"1";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton1_CheckedChanged);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(194, 110);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(31, 17);
			this->radioButton2->TabIndex = 24;
			this->radioButton2->Text = L"2";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton1_CheckedChanged);
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(239, 110);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(55, 17);
			this->radioButton3->TabIndex = 25;
			this->radioButton3->Text = L"1 ve 2";
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton1_CheckedChanged);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(191, 94);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(85, 13);
			this->label3->TabIndex = 26;
			this->label3->Text = L"Sürülecek Motor";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->label4);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Controls->Add(this->comboBox1);
			this->groupBox2->Controls->Add(this->label1);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->radioButton3);
			this->groupBox2->Controls->Add(this->radioButton2);
			this->groupBox2->Controls->Add(this->radioButton1);
			this->groupBox2->Controls->Add(this->trackBar2);
			this->groupBox2->Controls->Add(this->trackBar1);
			this->groupBox2->Controls->Add(this->button4);
			this->groupBox2->Controls->Add(this->button3);
			this->groupBox2->Controls->Add(this->button2);
			this->groupBox2->Controls->Add(this->splitter1);
			this->groupBox2->Location = System::Drawing::Point(26, 86);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(339, 218);
			this->groupBox2->TabIndex = 27;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"StepMotor";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(13, 22);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(123, 13);
			this->label4->TabIndex = 29;
			this->label4->Text = L"Motorun Çalýþma Süresi :";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(82, 75);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(40, 13);
			this->label2->TabIndex = 28;
			this->label2->Text = L"Zaman";
			// 
			// comboBox1
			// 
			this->comboBox1->DisplayMember = L"1";
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) {L"Elle", L"Zaman Ayarlý"});
			this->comboBox1->Location = System::Drawing::Point(157, 19);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(157, 21);
			this->comboBox1->TabIndex = 28;
			this->comboBox1->Text = L"Elle";
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::comboBox1_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(13, 75);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(22, 13);
			this->label1->TabIndex = 27;
			this->label1->Text = L"Hýz";
			// 
			// splitter1
			// 
			this->splitter1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->splitter1->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->splitter1->Location = System::Drawing::Point(3, 59);
			this->splitter1->Name = L"splitter1";
			this->splitter1->Size = System::Drawing::Size(333, 156);
			this->splitter1->TabIndex = 30;
			this->splitter1->TabStop = false;
			// 
			// Form1
			// 
			this->AcceptButton = this->button1;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(385, 348);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L" Usb StepMotor Uygulamasý ";
			this->TopMost = true;
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->trackBar2))->EndInit();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


private: System::Void ReadWriteThread_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
	
			unsigned char gelen_paket[64];
			unsigned char gond_paket[64];
			unsigned char hiz = 0;		 
		    unsigned char adim = 0;		 // Zaman (adým sayýsý) 
			DWORD ActualLength;
					
			while(AttachedState == TRUE)
			{
				if(butDurum)
				{
					//Bu parametleri motorunuza uygun deðerlerle deðiþtiriniz
					hiz  = (100 - ((unsigned char)trackBar1->Value ))/8;
					adim = (unsigned char)trackBar2->Value;
					
					gond_paket[0] = motor_sur;		//Gonderilecek komutu pakete yaz
					gond_paket[1] = adim;
					gond_paket[2] = hiz;
					gond_paket[3] = yon;
					gond_paket[4] = motor;
					gond_paket[5] = sureMetot;

					if(MPUSBWrite(EP1OUTHandle,gond_paket, 6, &ActualLength, 1000))	//Komutu cihaza gonder
					{
						if(MPUSBRead(EP1INHandle, gelen_paket, 1, &ActualLength, 1000))//Cihazdan gelen cevabý oku
						{
							if(gelen_paket[0]==gond_paket[0])				
								butDurum = false;
						}
					}
					
					else
					{
						//Gonderme baþarýsýz
						AttachedState=FALSE;
					}
				}
				Sleep(5);	
			}
			butDurum=false;
		}



private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(AttachedState)
			 {
				 
				 button1->Text = "B a ð l a n t ý y ý  K e s";
				
				 
			 }
			 else
			 {					 
				 if(durum)
				 {
					 toolStripStatusLabel2->Text = "Baðlantý kesildi.";
					 durum=false;
				 }
				 button1->Text = "B a ð l a n";				 
				 button2->Enabled = false;
				 button3->Enabled = false;
				 button4->Enabled = false;
				
			 }

		 }


#pragma region WndProc
protected: virtual void WndProc( Message% m ) override{
		 //Yeni bir cihaz takýlýr ya da çýkartýlýrsa istenen cihaz durumunu kontrol et

		 // Windows mesajlarýný takip et
		 if(m.Msg == WM_DEVICECHANGE)
		 {
			 if(((int)m.WParam == DBT_DEVICEARRIVAL) || ((int)m.WParam == DBT_DEVICEREMOVEPENDING) || ((int)m.WParam == DBT_DEVICEREMOVECOMPLETE) || ((int)m.WParam == DBT_CONFIGCHANGED) )
			 {

				if(MPUSBGetDeviceCount(DeviceVID_PID))	
				{
					if(AttachedState)
					{
						EP1OUTHandle = MPUSBOpen(0, DeviceVID_PID, "\\MCHP_EP1", MP_WRITE, 0);
						EP1INHandle = MPUSBOpen(0, DeviceVID_PID, "\\MCHP_EP1", MP_READ, 0);

						AttachedState = TRUE;
						toolStripStatusLabel2->Text = "Baðlantý kuruldu.";
						
					}
				
				}
				else	
				{
					if(MPUSBClose(EP1OUTHandle))
						EP1OUTHandle = INVALID_HANDLE_VALUE;
					if(MPUSBClose(EP1INHandle))
						EP1INHandle = INVALID_HANDLE_VALUE;

					AttachedState = FALSE;
					toolStripStatusLabel2->Text = "Baðlantý kurulamadý.";
				

				}
			 }
		 }

		 Form::WndProc( m );
	}
#pragma endregion

private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 if(!AttachedState) //Eðer cihaz, sisteme baðlý deðilse
			 {

				 if(MPUSBGetDeviceCount(DeviceVID_PID))	//Belirlediðimiz VID/PID deðerlerine sahip cihaz olup olmadýðý kontrol et
				 {
					 EP1OUTHandle = MPUSBOpen(0, DeviceVID_PID, "\\MCHP_EP1", MP_WRITE, 0);
					 EP1INHandle = MPUSBOpen(0, DeviceVID_PID, "\\MCHP_EP1", MP_READ, 0);

					 AttachedState = TRUE;				//kullanýma hazýr
					 toolStripStatusLabel2->Text = "Baðlantý kuruldu.";
					 durum=true;
					 button1->Text = "B a ð l a n t ý y ý  K e s";
					 button2->Enabled = true;
				     button3->Enabled = true;
				     button4->Enabled = true;

					 if(!ReadWriteThread->IsBusy)
						 ReadWriteThread->RunWorkerAsync();	

				 }
				 else	
				 {
					 toolStripStatusLabel2->Text = "Baðlantý kurulamadý.";
					 AttachedState = FALSE;
					
					 
				 }
			 }
			 else
			 {
				 AttachedState = FALSE;
				 ReadWriteThread->CancelAsync();							
				 button1->Text = "B a ð l a n";
				 

				 if (EP1OUTHandle != INVALID_HANDLE_VALUE)		//Açýk baðlantýlarý kapat
					 MPUSBClose (EP1OUTHandle);
				 if (EP1INHandle != INVALID_HANDLE_VALUE)
					 MPUSBClose (EP1INHandle);

				 toolStripStatusLabel2->Text = "Baðlantý kurulamadý.";
			 }
			 
		 }
private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
			 yon = sol;
			 button2->Enabled=false;
			 button3->Enabled=false;			 
			 button4->Enabled=true;
			 butDurum = true;
		 }
private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 yon = sag;
			 button2->Enabled=false;
			 button3->Enabled=false;
			 button4->Enabled=true;
			 butDurum = true;
		 }
private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e) {
			 button2->Enabled=true;
			 button3->Enabled=true;
			 button4->Enabled=false;
			 yon = durdur;
			 butDurum = true;
		 }
private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(radioButton1->Checked)
				 motor = 1;
			 if(radioButton2->Checked)
				 motor = 2;
			 if(radioButton3->Checked)
				 motor = 0;
		 }
private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
			 sureMetot = comboBox1->SelectedIndex==0 ? 0 : 1;
		 }
};
}

