#pragma once

#include <windows.h>	
#include <Dbt.h>		
#include "mpusbapi.h"	

#define DeviceVID_PID "vid_04d8&pid_0c00"
#define olcum_al  0x02					//ADC degerlerini oku
#define surum_oku 0x03					//Yazýlým sürümünü oku


namespace UsbADC {

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
	unsigned int ADC_deger1 = 0;
	unsigned int ADC_deger2 = 0;
	bool durum = false;
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();

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

	private: System::Windows::Forms::ProgressBar^  progressBar1;
	protected: 
	private: System::ComponentModel::BackgroundWorker^  ReadWriteThread;
	private: System::Windows::Forms::Timer^  timer1;

	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::ProgressBar^  progressBar2;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel3;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel4;

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
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->ReadWriteThread = (gcnew System::ComponentModel::BackgroundWorker());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->progressBar2 = (gcnew System::Windows::Forms::ProgressBar());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel4 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->groupBox1->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// progressBar1
			// 
			this->progressBar1->BackColor = System::Drawing::Color::White;
			this->progressBar1->ForeColor = System::Drawing::Color::Red;
			this->progressBar1->Location = System::Drawing::Point(53, 28);
			this->progressBar1->Maximum = 1024;
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(298, 23);
			this->progressBar1->Step = 50;
			this->progressBar1->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar1->TabIndex = 9;
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
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label2->Location = System::Drawing::Point(6, 28);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(30, 15);
			this->label2->TabIndex = 13;
			this->label2->Text = L"AN0";
			// 
			// button1
			// 
			this->button1->Font = (gcnew System::Drawing::Font(L"MS Reference Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->button1->Location = System::Drawing::Point(67, 25);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(298, 27);
			this->button1->TabIndex = 14;
			this->button1->Text = L"B a ð l a n";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// progressBar2
			// 
			this->progressBar2->Location = System::Drawing::Point(53, 57);
			this->progressBar2->Maximum = 1024;
			this->progressBar2->Name = L"progressBar2";
			this->progressBar2->Size = System::Drawing::Size(298, 23);
			this->progressBar2->Step = 50;
			this->progressBar2->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
			this->progressBar2->TabIndex = 15;
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->progressBar2);
			this->groupBox1->Controls->Add(this->progressBar1);
			this->groupBox1->Location = System::Drawing::Point(14, 68);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(370, 92);
			this->groupBox1->TabIndex = 16;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Ölçülen Gerilimler";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(162)));
			this->label1->Location = System::Drawing::Point(6, 57);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(30, 15);
			this->label1->TabIndex = 17;
			this->label1->Text = L"AN1";
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->toolStripStatusLabel1, 
				this->toolStripStatusLabel2, this->toolStripStatusLabel3, this->toolStripStatusLabel4});
			this->statusStrip1->Location = System::Drawing::Point(0, 172);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(411, 22);
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
			this->toolStripStatusLabel3->Text = L"Sürüm :";
			// 
			// toolStripStatusLabel4
			// 
			this->toolStripStatusLabel4->AutoSize = false;
			this->toolStripStatusLabel4->Name = L"toolStripStatusLabel4";
			this->toolStripStatusLabel4->Size = System::Drawing::Size(109, 17);
			// 
			// Form1
			// 
			this->AcceptButton = this->button1;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(411, 194);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->button1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"USB ADC Uygulamasý";
			this->TopMost = true;
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


private: System::Void ReadWriteThread_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
	
			unsigned char paket[64];
			DWORD ActualLength;
			paket[0] = surum_oku;			//Yazýlým sürümünü oku

			if(MPUSBWrite(EP1OUTHandle, paket, 1, &ActualLength, 1000))	
			{
				if(MPUSBRead(EP1INHandle, paket, 3, &ActualLength, 1000))
				 {
					 if(paket[0]==surum_oku)
						toolStripStatusLabel4->Text = " " + paket[2] + "." + paket[1];
					 else
						toolStripStatusLabel4->Text = "Bilinmiyor" ;
				 }
			}	
			
			
			while(AttachedState == TRUE)
			{
			
				paket[0] = olcum_al;		//Gonderilecek komutu pakete yaz


				if(MPUSBWrite(EP1OUTHandle, paket, 1, &ActualLength, 1000))	//Komutu cihaza gonder
				{
					if(MPUSBRead(EP1INHandle, paket, 5, &ActualLength, 1000))//Cihazdan gelen cevabý oku
					{
						ADC_deger1 = (paket[2] << 8) +  paket[1];
						ADC_deger2 = (paket[4] << 8) +  paket[3];
					}
				}
				
				else
				{
					ADC_deger1 = 0;
					ADC_deger2 = 0;
					AttachedState=FALSE;
				}
				Sleep(5);	
			}
		}



private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) //Progressbarlarý günceller
		 {
			 if(AttachedState)
			 {
				 toolStripStatusLabel2->Text = "Baðlantý kuruldu.";
				 button1->Text = "B a ð l a n t ý y ý  K e s";
				 if(ADC_deger1 > (unsigned int)progressBar1->Maximum)
				 {
					 ADC_deger1 = progressBar1->Maximum;
				 }

				 if(ADC_deger2 > (unsigned int)progressBar2->Maximum)
				 {
					 ADC_deger2 = progressBar2->Maximum;
				 }

				 progressBar1->Value = ADC_deger1;
				 progressBar2->Value = ADC_deger2;
			 }
			 else
			 {					 
				 if(durum)
				 {
					 toolStripStatusLabel2->Text = "Baðlantý kesildi.";
					 durum=false;
				 }
				 button1->Text = "B a ð l a n";
				 progressBar1->Value = 0;
				 progressBar2->Value = 0;
			 }

		 }


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
					progressBar1->Value = 0;
					progressBar2->Value = 0;

				}
			 }
		 }

		 Form::WndProc( m );
	}


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

					 if(!ReadWriteThread->IsBusy)
						 ReadWriteThread->RunWorkerAsync();	//ADC okuma fonksiyonu için ayrý bir thread çalýþtýrýlmalý

				 }
				 else	
				 {
					 toolStripStatusLabel2->Text = "Baðlantý kurulamadý.";
					 AttachedState = FALSE;
					 progressBar1->Value = 0;
					 progressBar2->Value = 0;
					 
				 }
			 }
			 else
			 {
				 AttachedState = FALSE;
				 ReadWriteThread->CancelAsync();				//ADC okuma durduluyor
				 progressBar1->Value = 0;
				 progressBar2->Value = 0;
				 button1->Text = "B a ð l a n";
				 

				 if (EP1OUTHandle != INVALID_HANDLE_VALUE)		//Açýk baðlantýlarý kapat
					 MPUSBClose (EP1OUTHandle);
				 if (EP1INHandle != INVALID_HANDLE_VALUE)
					 MPUSBClose (EP1INHandle);

				 toolStripStatusLabel2->Text = "Baðlantý kurulamadý.";
			 }
			 
		 }
};
}

