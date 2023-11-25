#pragma once

#include <Windows.h>	//Definitions for various common and not so common types like DWORD, PCHAR, HANDLE, etc.
#include <Dbt.h>		//Need this for definitions of WM_DEVICECHANGE messages
#include <setupapi.h>	

#define Cihaz_ID  "Vid_04d8&Pid_0E00"	
#define LED_Kontrol   0x04
#define LCD_Yaz       0x05

namespace UsbHID {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

#pragma region DLL Imports
	using namespace System::Threading;	
	using namespace System::Runtime::InteropServices;  //Need this to support "unmanaged" code.


#ifdef UNICODE
#define	Seeifdef	Unicode
#else
#define Seeifdef	Ansi
#endif

	//Returns a HDEVINFO type for a device information set (WinUSB devices in
	//our case).  We will need the HDEVINFO as in input parameter for calling many of
	//the other SetupDixxx() functions.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetClassDevs")]		
	extern "C" HDEVINFO  SetupDiGetClassDevsUM(
		LPGUID  ClassGuid,					//Input: Supply the class GUID here. 
		PCTSTR  Enumerator,					//Input: Use NULL here, not important for our purposes
		HWND  hwndParent,					//Input: Use NULL here, not important for our purposes
		DWORD  Flags);						//Input: Flags describing what kind of filtering to use.

	//Gives us "PSP_DEVICE_INTERFACE_DATA" which contains the Interface specific GUID (different
	//from class GUID).  We need the interface GUID to get the device path.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInterfaces")]				
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInterfacesUM(
		HDEVINFO  DeviceInfoSet,			//Input: Give it the HDEVINFO we got from SetupDiGetClassDevs()
		PSP_DEVINFO_DATA  DeviceInfoData,	//Input (optional)
		LPGUID  InterfaceClassGuid,			//Input 
		DWORD  MemberIndex,					//Input: "Index" of the device you are interested in getting the path for.
		PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData);//Output: This function fills in an "SP_DEVICE_INTERFACE_DATA" structure.

	//SetupDiDestroyDeviceInfoList() frees up memory by destroying a DeviceInfoList
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiDestroyDeviceInfoList")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiDestroyDeviceInfoListUM(			
		HDEVINFO  DeviceInfoSet);			//Input: Give it a handle to a device info list to deallocate from RAM.

	//SetupDiEnumDeviceInfo() fills in an "SP_DEVINFO_DATA" structure, which we need for SetupDiGetDeviceRegistryProperty()
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiEnumDeviceInfo")]
	extern "C" WINSETUPAPI BOOL WINAPI  SetupDiEnumDeviceInfoUM(
		HDEVINFO  DeviceInfoSet,
		DWORD  MemberIndex,
		PSP_DEVINFO_DATA  DeviceInfoData);

	//SetupDiGetDeviceRegistryProperty() gives us the hardware ID, which we use to check to see if it has matching VID/PID
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetDeviceRegistryProperty")]
	extern "C"	WINSETUPAPI BOOL WINAPI  SetupDiGetDeviceRegistryPropertyUM(
		HDEVINFO  DeviceInfoSet,
		PSP_DEVINFO_DATA  DeviceInfoData,
		DWORD  Property,
		PDWORD  PropertyRegDataType,
		PBYTE  PropertyBuffer,   
		DWORD  PropertyBufferSize,  
		PDWORD  RequiredSize);

	//SetupDiGetDeviceInterfaceDetail() gives us a device path, which is needed before CreateFile() can be used.
	[DllImport("setupapi.dll" , CharSet = CharSet::Seeifdef, EntryPoint="SetupDiGetDeviceInterfaceDetail")]
	extern "C" BOOL SetupDiGetDeviceInterfaceDetailUM(
		HDEVINFO DeviceInfoSet,										//Input: Wants HDEVINFO which can be obtained from SetupDiGetClassDevs()
		PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData,				//Input: Pointer to an structure which defines the device interface.  
		PSP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData,	//Output: Pointer to a strucutre, which will contain the device path.
		DWORD DeviceInterfaceDetailDataSize,						//Input: Number of bytes to retrieve.
		PDWORD RequiredSize,										//Output (optional): Te number of bytes needed to hold the entire struct 
		PSP_DEVINFO_DATA DeviceInfoData);							//Output

	//Need this function for receiving all of the WM_DEVICECHANGE messages.  See MSDN documentation for
	//description of what this function does/how to use it. Note: name is remapped "RegisterDeviceNotificationUM" to
	//avoid possible build error conflicts.
	[DllImport("user32.dll" , CharSet = CharSet::Seeifdef, EntryPoint="RegisterDeviceNotification")]					
	extern "C" HDEVNOTIFY WINAPI RegisterDeviceNotificationUM(
		HANDLE hRecipient,
		LPVOID NotificationFilter,
		DWORD Flags);

#pragma endregion

#pragma region Genel Degiskenler


	//Globally Unique Identifier (GUID) for HID class devices.  Windows uses GUIDs to identify things.
	GUID InterfaceClassGuid = {0x4d1e55b2, 0xf16f, 0x11cf, 0x88, 0xcb, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30}; 

	//USB deðiþkenleri
	BOOL AttachedState = false;		
	BOOL AttachedButBroken = false;					
	PSP_DEVICE_INTERFACE_DETAIL_DATA DetailedInterfaceDataStructure = new SP_DEVICE_INTERFACE_DETAIL_DATA;
	HANDLE WriteHandleToUSBDevice = INVALID_HANDLE_VALUE;
	HANDLE ReadHandleToUSBDevice = INVALID_HANDLE_VALUE;


	BOOL Metin_Gonder = false;		
	BOOL LED_Yak = false;		
	unsigned char LED_durum = 0;			

#pragma endregion

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			//Additional constructor code			

			//Register for WM_DEVICECHANGE notifications.  This code uses these messages to detect plug and play connection/disconnection events for USB devices
			DEV_BROADCAST_DEVICEINTERFACE MyDeviceBroadcastHeader;// = new DEV_BROADCAST_HDR;
			MyDeviceBroadcastHeader.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
			MyDeviceBroadcastHeader.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
			MyDeviceBroadcastHeader.dbcc_reserved = 0;	//Reserved says not to use...
			MyDeviceBroadcastHeader.dbcc_classguid = InterfaceClassGuid;
			RegisterDeviceNotificationUM((HANDLE)this->Handle, &MyDeviceBroadcastHeader, DEVICE_NOTIFY_WINDOW_HANDLE);
		    if(CheckIfPresentAndGetUSBDevicePath())	//Cihazý ara
		    {
				 DWORD ErrorStatusWrite;
				 DWORD ErrorStatusRead;

				 WriteHandleToUSBDevice = CreateFile(DetailedInterfaceDataStructure->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				 ErrorStatusWrite = GetLastError();
				 ReadHandleToUSBDevice = CreateFile(DetailedInterfaceDataStructure->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
				 ErrorStatusRead = GetLastError();

				 if((ErrorStatusWrite == ERROR_SUCCESS) && (ErrorStatusRead == ERROR_SUCCESS))
				 {
					 AttachedState = TRUE;		
					 AttachedButBroken = FALSE;
					 toolStripStatusLabel2->Text = "Baðlantý kuruldu";
					 if(radioButton1->Checked)
					 {
						 groupBox1->Enabled= true;
						 groupBox2->Enabled= false;
					 }
					 else
					 {	
						 groupBox1->Enabled= false;
						 groupBox2->Enabled= true;
					 }
					 
			
				 }
				 else //yazma ya da okuma iþlemlerinden biri baþarasýz olursa
				 {
					 AttachedState = FALSE;		
					 AttachedButBroken = TRUE;	
					 if(ErrorStatusWrite == ERROR_SUCCESS)//Açýk baðlantýlarý kapat
						 CloseHandle(WriteHandleToUSBDevice);
					 if(ErrorStatusRead == ERROR_SUCCESS)
						 CloseHandle(ReadHandleToUSBDevice);

					 toolStripStatusLabel2->Text = "Baðlantý kesildi";
					 groupBox1->Enabled= false;
					 groupBox2->Enabled= false;
					 
				 }
			 }
			 else	
			 {
				 AttachedState = FALSE;
				 AttachedButBroken = FALSE;
				 toolStripStatusLabel2->Text = "Cihaz bulunamadý";
				 groupBox1->Enabled= false;
				 groupBox2->Enabled= false;
				 
			 }
					if(!ReadWriteThread->IsBusy)
						 ReadWriteThread->RunWorkerAsync();

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{


			//Close the read/write handles, if they are currently open.
			if(AttachedState == TRUE)
			{
				CloseHandle(WriteHandleToUSBDevice);
				CloseHandle(ReadHandleToUSBDevice);
			}

			if (components)
			{
				delete components;
			}
		}


#pragma region Components
	protected: 

	protected: 



	private: System::ComponentModel::BackgroundWorker^  ReadWriteThread;
	private: System::Windows::Forms::Timer^  FormUpdateTimer;

	private: System::Windows::Forms::RadioButton^  radioButton1;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::CheckBox^  checkBox2;
	private: System::Windows::Forms::CheckBox^  checkBox3;
	private: System::Windows::Forms::CheckBox^  checkBox4;
	private: System::Windows::Forms::CheckBox^  checkBox5;
	private: System::Windows::Forms::CheckBox^  checkBox6;
	private: System::Windows::Forms::CheckBox^  checkBox7;
	private: System::Windows::Forms::CheckBox^  checkBox8;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::TextBox^  textBox1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::StatusStrip^  statusStrip1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel1;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel3;










	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
#pragma endregion

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
			this->FormUpdateTimer = (gcnew System::Windows::Forms::Timer(this->components));
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox2 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox3 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox4 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox5 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox6 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox7 = (gcnew System::Windows::Forms::CheckBox());
			this->checkBox8 = (gcnew System::Windows::Forms::CheckBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel1 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->panel1->SuspendLayout();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->statusStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// ReadWriteThread
			// 
			this->ReadWriteThread->WorkerReportsProgress = true;
			this->ReadWriteThread->WorkerSupportsCancellation = true;
			this->ReadWriteThread->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::ReadWriteThread_DoWork);
			// 
			// FormUpdateTimer
			// 
			this->FormUpdateTimer->Enabled = true;
			this->FormUpdateTimer->Interval = 6;
			this->FormUpdateTimer->Tick += gcnew System::EventHandler(this, &Form1::FormUpdateTimer_Tick);
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Checked = true;
			this->radioButton1->Location = System::Drawing::Point(83, 3);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(46, 17);
			this->radioButton1->TabIndex = 1;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"LED";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton1_CheckedChanged);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(154, 3);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(46, 17);
			this->radioButton2->TabIndex = 2;
			this->radioButton2->Text = L"LCD";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->CheckedChanged += gcnew System::EventHandler(this, &Form1::radioButton1_CheckedChanged);
			// 
			// panel1
			// 
			this->panel1->BorderStyle = System::Windows::Forms::BorderStyle::Fixed3D;
			this->panel1->Controls->Add(this->radioButton2);
			this->panel1->Controls->Add(this->radioButton1);
			this->panel1->Location = System::Drawing::Point(32, 43);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(297, 27);
			this->panel1->TabIndex = 3;
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->Location = System::Drawing::Point(61, 44);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(15, 14);
			this->checkBox1->TabIndex = 4;
			this->checkBox1->UseVisualStyleBackColor = true;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox8_CheckedChanged);
			// 
			// checkBox2
			// 
			this->checkBox2->AutoSize = true;
			this->checkBox2->Location = System::Drawing::Point(82, 44);
			this->checkBox2->Name = L"checkBox2";
			this->checkBox2->Size = System::Drawing::Size(15, 14);
			this->checkBox2->TabIndex = 5;
			this->checkBox2->UseVisualStyleBackColor = true;
			this->checkBox2->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox8_CheckedChanged);
			// 
			// checkBox3
			// 
			this->checkBox3->AutoSize = true;
			this->checkBox3->Location = System::Drawing::Point(103, 44);
			this->checkBox3->Name = L"checkBox3";
			this->checkBox3->Size = System::Drawing::Size(15, 14);
			this->checkBox3->TabIndex = 6;
			this->checkBox3->UseVisualStyleBackColor = true;
			this->checkBox3->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox8_CheckedChanged);
			// 
			// checkBox4
			// 
			this->checkBox4->AutoSize = true;
			this->checkBox4->Location = System::Drawing::Point(124, 44);
			this->checkBox4->Name = L"checkBox4";
			this->checkBox4->Size = System::Drawing::Size(15, 14);
			this->checkBox4->TabIndex = 7;
			this->checkBox4->UseVisualStyleBackColor = true;
			this->checkBox4->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox8_CheckedChanged);
			// 
			// checkBox5
			// 
			this->checkBox5->AutoSize = true;
			this->checkBox5->Location = System::Drawing::Point(145, 44);
			this->checkBox5->Name = L"checkBox5";
			this->checkBox5->Size = System::Drawing::Size(15, 14);
			this->checkBox5->TabIndex = 8;
			this->checkBox5->UseVisualStyleBackColor = true;
			this->checkBox5->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox8_CheckedChanged);
			// 
			// checkBox6
			// 
			this->checkBox6->AutoSize = true;
			this->checkBox6->Location = System::Drawing::Point(166, 44);
			this->checkBox6->Name = L"checkBox6";
			this->checkBox6->Size = System::Drawing::Size(15, 14);
			this->checkBox6->TabIndex = 9;
			this->checkBox6->UseVisualStyleBackColor = true;
			this->checkBox6->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox8_CheckedChanged);
			// 
			// checkBox7
			// 
			this->checkBox7->AutoSize = true;
			this->checkBox7->Location = System::Drawing::Point(187, 44);
			this->checkBox7->Name = L"checkBox7";
			this->checkBox7->Size = System::Drawing::Size(15, 14);
			this->checkBox7->TabIndex = 10;
			this->checkBox7->UseVisualStyleBackColor = true;
			this->checkBox7->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox8_CheckedChanged);
			// 
			// checkBox8
			// 
			this->checkBox8->AutoSize = true;
			this->checkBox8->Location = System::Drawing::Point(208, 44);
			this->checkBox8->Name = L"checkBox8";
			this->checkBox8->Size = System::Drawing::Size(15, 14);
			this->checkBox8->TabIndex = 11;
			this->checkBox8->UseVisualStyleBackColor = true;
			this->checkBox8->CheckedChanged += gcnew System::EventHandler(this, &Form1::checkBox8_CheckedChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(63, 28);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(13, 13);
			this->label1->TabIndex = 12;
			this->label1->Text = L"0";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ImageAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->label2->Location = System::Drawing::Point(82, 28);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(13, 13);
			this->label2->TabIndex = 13;
			this->label2->Text = L"1";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(210, 28);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(13, 13);
			this->label3->TabIndex = 14;
			this->label3->Text = L"7";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(187, 28);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(13, 13);
			this->label4->TabIndex = 15;
			this->label4->Text = L"6";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(166, 28);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(13, 13);
			this->label5->TabIndex = 16;
			this->label5->Text = L"5";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(145, 28);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(13, 13);
			this->label6->TabIndex = 17;
			this->label6->Text = L"4";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(126, 28);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(13, 13);
			this->label7->TabIndex = 18;
			this->label7->Text = L"3";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(103, 28);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(13, 13);
			this->label8->TabIndex = 19;
			this->label8->Text = L"2";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->label8);
			this->groupBox1->Controls->Add(this->checkBox1);
			this->groupBox1->Controls->Add(this->label7);
			this->groupBox1->Controls->Add(this->checkBox2);
			this->groupBox1->Controls->Add(this->label6);
			this->groupBox1->Controls->Add(this->checkBox3);
			this->groupBox1->Controls->Add(this->label5);
			this->groupBox1->Controls->Add(this->checkBox4);
			this->groupBox1->Controls->Add(this->label4);
			this->groupBox1->Controls->Add(this->checkBox5);
			this->groupBox1->Controls->Add(this->label3);
			this->groupBox1->Controls->Add(this->checkBox6);
			this->groupBox1->Controls->Add(this->label2);
			this->groupBox1->Controls->Add(this->checkBox7);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Controls->Add(this->checkBox8);
			this->groupBox1->Enabled = false;
			this->groupBox1->Location = System::Drawing::Point(32, 92);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(297, 78);
			this->groupBox1->TabIndex = 20;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"LED";
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(197, 48);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(85, 23);
			this->button2->TabIndex = 21;
			this->button2->Text = L"Gonder";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(11, 22);
			this->textBox1->MaxLength = 32;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(271, 20);
			this->textBox1->TabIndex = 22;
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->button2);
			this->groupBox2->Controls->Add(this->textBox1);
			this->groupBox2->Enabled = false;
			this->groupBox2->Location = System::Drawing::Point(32, 189);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(297, 86);
			this->groupBox2->TabIndex = 23;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"LCD";
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {this->toolStripStatusLabel1, 
				this->toolStripStatusLabel2, this->toolStripStatusLabel3});
			this->statusStrip1->Location = System::Drawing::Point(0, 291);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(369, 22);
			this->statusStrip1->TabIndex = 24;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel1
			// 
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
			this->toolStripStatusLabel3->Size = System::Drawing::Size(260, 17);
			this->toolStripStatusLabel3->Text = L"tmcone (c) 2009";
			this->toolStripStatusLabel3->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// Form1
			// 
			this->AcceptButton = this->button2;
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(369, 313);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->panel1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"UsbHID Uygulamasi";
			this->TopMost = true;
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion


	BOOL	CheckIfPresentAndGetUSBDevicePath(void)
		{


			HDEVINFO DeviceInfoTable = INVALID_HANDLE_VALUE;
			PSP_DEVICE_INTERFACE_DATA InterfaceDataStructure = new SP_DEVICE_INTERFACE_DATA;
			SP_DEVINFO_DATA DevInfoData;

			DWORD InterfaceIndex = 0;
			DWORD StatusLastError = 0;
			DWORD dwRegType;
			DWORD dwRegSize;
			DWORD StructureSize = 0;
			PBYTE PropertyValueBuffer;
			bool MatchFound = false;
			DWORD ErrorStatus;
			BOOL BoolStatus = FALSE;
			DWORD LoopCounter = 0;

			String^ DeviceIDToFind = Cihaz_ID;

			//First populate a list of plugged in devices (by specifying "DIGCF_PRESENT"), which are of the specified class GUID. 
			DeviceInfoTable = SetupDiGetClassDevsUM(&InterfaceClassGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

			//Now look through the list we just populated.  We are trying to see if any of them match our device. 
			while(true)
			{
				InterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
				if(SetupDiEnumDeviceInterfacesUM(DeviceInfoTable, NULL, &InterfaceClassGuid, InterfaceIndex, InterfaceDataStructure))
				{
					ErrorStatus = GetLastError();
					if(ERROR_NO_MORE_ITEMS == GetLastError())	//Did we reach the end of the list of matching devices in the DeviceInfoTable?
					{	//Cound not find the device.  Must not have been attached.
						SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
						return FALSE;		
					}
				}
				else	//Else some other kind of unknown error ocurred...
				{
					ErrorStatus = GetLastError();
					SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
					return FALSE;	
				}

				//Now retrieve the hardware ID from the registry.  The hardware ID contains the VID and PID, which we will then 
				//check to see if it is the correct device or not.

				//Initialize an appropriate SP_DEVINFO_DATA structure.  We need this structure for SetupDiGetDeviceRegistryProperty().
				DevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
				SetupDiEnumDeviceInfoUM(DeviceInfoTable, InterfaceIndex, &DevInfoData);

				//First query for the size of the hardware ID, so we can know how big a buffer to allocate for the data.
				SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, NULL, 0, &dwRegSize);

				//Allocate a buffer for the hardware ID.
				PropertyValueBuffer = (BYTE *) malloc (dwRegSize);
				if(PropertyValueBuffer == NULL)	//if null, error, couldn't allocate enough memory
				{	//Can't really recover from this situation, just exit instead.
					SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
					return FALSE;		
				}

				//Retrieve the hardware IDs for the current device we are looking at.  PropertyValueBuffer gets filled with a 
				//REG_MULTI_SZ (array of null terminated strings).  To find a device, we only care about the very first string in the
				//buffer, which will be the "device ID".  The device ID is a string which contains the VID and PID, in the example 
				//format "Vid_04d8&Pid_003f".
				SetupDiGetDeviceRegistryPropertyUM(DeviceInfoTable, &DevInfoData, SPDRP_HARDWAREID, &dwRegType, PropertyValueBuffer, dwRegSize, NULL);

				//Now check if the first string in the hardware ID matches the device ID of my USB device.
#ifdef UNICODE
				String^ DeviceIDFromRegistry = gcnew String((wchar_t *)PropertyValueBuffer);
#else
				String^ DeviceIDFromRegistry = gcnew String((char *)PropertyValueBuffer);
#endif

				free(PropertyValueBuffer);		//No longer need the PropertyValueBuffer, free the memory to prevent potential memory leaks

				//Convert both strings to lower case.  This makes the code more robust/portable accross OS Versions
				DeviceIDFromRegistry = DeviceIDFromRegistry->ToLowerInvariant();	
				DeviceIDToFind = DeviceIDToFind->ToLowerInvariant();				
				//Now check if the hardware ID we are looking at contains the correct VID/PID
				MatchFound = DeviceIDFromRegistry->Contains(DeviceIDToFind);		
				if(MatchFound == true)
				{
					//Device must have been found.  Open WinUSB interface handle now.  In order to do this, we will need the actual device path first.
					//We can get the path by calling SetupDiGetDeviceInterfaceDetail(), however, we have to call this function twice:  The first
					//time to get the size of the required structure/buffer to hold the detailed interface data, then a second time to actually 
					//get the structure (after we have allocated enough memory for the structure.)
					DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
					//First call populates "StructureSize" with the correct value
					SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, NULL, NULL, &StructureSize, NULL);	
					DetailedInterfaceDataStructure = (PSP_DEVICE_INTERFACE_DETAIL_DATA)(malloc(StructureSize));		//Allocate enough memory
					if(DetailedInterfaceDataStructure == NULL)	//if null, error, couldn't allocate enough memory
					{	//Can't really recover from this situation, just exit instead.
						SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
						return FALSE;		
					}
					DetailedInterfaceDataStructure->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
					//Now call SetupDiGetDeviceInterfaceDetail() a second time to receive the goods.  
					SetupDiGetDeviceInterfaceDetailUM(DeviceInfoTable, InterfaceDataStructure, DetailedInterfaceDataStructure, StructureSize, NULL, NULL); 

					//We now have the proper device path, and we can finally open a device handle to the device.
					//WinUSB requires the device handle to be opened with the FILE_FLAG_OVERLAPPED attribute.
					SetupDiDestroyDeviceInfoListUM(DeviceInfoTable);	//Clean up the old structure we no longer need.
					return TRUE;
				}

				InterfaceIndex++;	
				//Keep looping until we either find a device with matching VID and PID, or until we run out of devices to check.
				//However, just in case some unexpected error occurs, keep track of the number of loops executed.
				//If the number of loops exceeds a very large number, exit anyway, to prevent inadvertent infinite looping.
				LoopCounter++;
				if(LoopCounter == 10000000)	//Surely there aren't more than 10 million devices attached to any forseeable PC...
				{
					return FALSE;
				}
			}
		}


#pragma region WM_DeviceChange
	protected: virtual void WndProc( Message% m ) override{
				   if(m.Msg == WM_DEVICECHANGE)
				   {
					   if(((int)m.WParam == DBT_DEVICEARRIVAL) || ((int)m.WParam == DBT_DEVICEREMOVEPENDING) || ((int)m.WParam == DBT_DEVICEREMOVECOMPLETE) || ((int)m.WParam == DBT_CONFIGCHANGED) )
					   {

						   if(CheckIfPresentAndGetUSBDevicePath())	//Check and make sure at least one device with matching VID/PID is attached
						   {
							   //If executes to here, this means the device is currently attached and was found.
							   //This code needs to decide however what to do, based on whether or not the device was previously known to be
							   //attached or not.
							   if((AttachedState == FALSE) || (AttachedButBroken == TRUE))	//Check the previous attachment state
							   {
								   DWORD ErrorStatusWrite;
								   DWORD ErrorStatusRead;

								   //We obtained the proper device path (from CheckIfPresentAndGetUSBDevicePath() function call), and it
								   //is now possible to open read and write handles to the device.
								   WriteHandleToUSBDevice = CreateFile(DetailedInterfaceDataStructure->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
								   ErrorStatusWrite = GetLastError();
								   ReadHandleToUSBDevice = CreateFile(DetailedInterfaceDataStructure->DevicePath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
								   ErrorStatusRead = GetLastError();

								   if((ErrorStatusWrite == ERROR_SUCCESS) && (ErrorStatusRead == ERROR_SUCCESS))
								   {
									   AttachedState = TRUE;
									   AttachedButBroken = FALSE;
									   toolStripStatusLabel2->Text = "Baðlantý kuruldu";
						   			   if(radioButton1->Checked)
									   {
										   groupBox1->Enabled= true;
										   groupBox2->Enabled= false;
									   }
									   else
									   {	
										   groupBox1->Enabled= false;
										   groupBox2->Enabled= true;
									   }
									   
								   }
								   else //For some reason one or both of the read/write pipes couldn't be opened, even though the device was physically attached.  This could 
								   {	 //happen if some other application already opened handles to the device, but did not allow read or write sharing.
									   AttachedState = FALSE;		//Let the rest of this application know not to try reading/writing to the USB device
									   AttachedButBroken = TRUE;	//Let this WM_DEVICECHANGE handler know to retry to open handles the next time a WM_DEVICECHANGE message occurs.
									   if(ErrorStatusWrite == ERROR_SUCCESS)
										   CloseHandle(WriteHandleToUSBDevice);
									   if(ErrorStatusRead == ERROR_SUCCESS)
										   CloseHandle(ReadHandleToUSBDevice);

									   toolStripStatusLabel2->Text = "Baðlantý kesildi";
									   groupBox1->Enabled= false;
									   groupBox2->Enabled= false;
								   }
							   }
							   //else we did find the device, but AttachedState was already TRUE.  In this case, don't do anything to the read/write handles,
							   //since the WM_DEVICECHANGE message presumably wasn't caused by our USB device.  
						   }
						   else	//Device must not be connected (or not programmed with correct firmware)
						   {
							   if(AttachedState == TRUE)		//If it is currently set to TRUE, that means the device was just now disconnected
							   {
								   AttachedState = FALSE;
								   CloseHandle(WriteHandleToUSBDevice);
								   CloseHandle(ReadHandleToUSBDevice);
							   }
							   AttachedState = FALSE;
							   AttachedButBroken = FALSE;
							   toolStripStatusLabel2->Text = "Baðlantý kesildi";
							   groupBox1->Enabled= false;
							   groupBox2->Enabled= false;
							   
						   }				 
					   }
				   } 

				   Form::WndProc( m );
			   } 
#pragma endregion

	private: System::Void FormUpdateTimer_Tick(System::Object^  sender, System::EventArgs^  e) {;}

	private: System::Void ReadWriteThread_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {

				 unsigned char gond_paket[65];	//Elemen sayýsý, Uçnokta boyutu + 1
				 unsigned char gelen_paket[65];	 	
				 DWORD Gond_say = 0;				
				 DWORD Gelen_say = 0;

				 while(true)
				 {
					 if(AttachedState)
					 {
						 if(LED_Yak)//LED_Yak)
						 {
							 gond_paket[0] = 0;		// HID sýnýfýnda ilk eleman Rapor ID 'sini tutar, cihaza gönderilmez
							 gond_paket[1] = LED_Kontrol;
							 gond_paket[2] = LED_durum;

							 //Paketin kullanýlmayan elemanlarý 0xFF ile doldurulursa USB, NRZI kodlama kullandýðýndan
							 //usb kablosunun elektromanyetik etkileþimi azalýr

							 for(unsigned char i = 3; i <65; i++)
								 gond_paket[i] = 0xFF;


							 if(WriteFile(WriteHandleToUSBDevice, &gond_paket, 65, &Gond_say, 0))	
							 {
								 //gelen_paket[0] = 0;
								 //if(ReadFile(ReadHandleToUSBDevice,&gelen_paket,65,&Gelen_say,0))
								 LED_Yak =false;							
							 }


						 }
						 if(Metin_Gonder)
						 {

							 unsigned char i=0;							 

							 for(i = 3; i < (textBox1->Text->Length+3); i++)
								 gond_paket[i] = textBox1->Text[i-3];			

							 gond_paket[0] = 0;					
							 gond_paket[1] = LCD_Yaz;
							 gond_paket[2] = i-3;
							 if(WriteFile(WriteHandleToUSBDevice, &gond_paket, 65, &Gond_say, 0))	
								 Metin_Gonder = false;
						 }

						 else
						 {
							 Sleep(5);	
						 }
					 }
				 }

			 }


	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {


			


			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
				 Metin_Gonder = true;

			 }
	private: System::Void checkBox8_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 LED_Yak = true;
				 LED_durum = (checkBox1->Checked ? 1:0) | ((checkBox2->Checked ? 1:0) << 1)| ((checkBox3->Checked ? 1:0) << 2)| ((checkBox4->Checked ? 1:0) << 3)
					 | ((checkBox5->Checked ? 1:0) << 4)| ((checkBox6->Checked ? 1:0) << 5)| ((checkBox7->Checked ? 1:0) << 6)| ((checkBox8->Checked ? 1:0) << 7);
			 }
	private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
				 if(AttachedState)
				 {
					 if(radioButton1->Checked) 
						 groupBox1->Enabled = true;
					 else
						 groupBox1->Enabled = false;

					 if(radioButton2->Checked)
						 groupBox2->Enabled = true;
					 else
						 groupBox2->Enabled = false;
				 }
			 }
};
}

