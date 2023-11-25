
#include "..\usb\typedefs.h"
#include "..\usb\usb.h"


#pragma romdata

/* Device Descriptor */
rom USB_DEV_DSC device_dsc=
{    
    sizeof(USB_DEV_DSC),    // Size of this descriptor in bytes
    DSC_DEV,                // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    EP0_BUFF_SIZE,          // Max packet size for EP0, see usbcfg.h
    0x04D8,                 // Vendor ID
    0x0C00,                 // Product ID
    0x0000,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x00,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
CFG01=
{
    /* Configuration Descriptor */
    sizeof(USB_CFG_DSC),    // Size of this descriptor in bytes
    DSC_CFG,                // CONFIGURATION descriptor type
    sizeof(cfg01),          // Total length of data for this cfg
    1,                      // Number of interfaces in this cfg
    1,                      // Index value of this configuration
    0,                      // Configuration string index
    _DEFAULT,               // Attributes, see usbdefs_std_dsc.h
    150,                     // Max power consumption (2X mA)
    
    /* Interface Descriptor */
    sizeof(USB_INTF_DSC),   // Size of this descriptor in bytes
    DSC_INTF,               // INTERFACE descriptor type
    0,                      // Interface Number
    0,                      // Alternate Setting Number
    2,                      // Number of endpoints in this intf
    0x00,                   // Class code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    0,                      // Interface string index
    
    /* Endpoint Descriptors */
    sizeof(USB_EP_DSC),DSC_EP,_EP01_OUT,_INT,USBGEN_EP_SIZE,32,
    sizeof(USB_EP_DSC),DSC_EP,_EP01_IN,_INT,USBGEN_EP_SIZE,32
};

rom struct{byte bLength;byte bDscType;word string[1];}sd000={
sizeof(sd000),DSC_STR,0x0409};

rom struct{byte bLength;byte bDscType;word string[26];}sd001={
sizeof(sd001),DSC_STR,
't','m','c','o','n','e',' ','(','C',') ',' ',
'T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'};

rom struct{byte bLength;byte bDscType;word string[40];}sd002={
sizeof(sd002),DSC_STR,
't','m','c','o','n','e',' ','(','C',')',' ','U','S','B',
' ','S','t','e','p','M','o','t','o','r',' ','U','y','g','u','l','a','m','a','s','i',
' ','2','0','0','9'};

rom const unsigned char *rom USB_CD_Ptr[]={&cfg01,&cfg01};
rom const unsigned char *rom USB_SD_Ptr[]={&sd000,&sd001,&sd002};


#pragma code
//end of file
