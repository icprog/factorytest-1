
#ifndef _FT_TYPE_H_
#define _FT_TYPE_H_

#define FT_SUCCESS  ( 0)
#define FT_FAIL     (-1)

#define FT_STATUS_NORMAL    (0)
#define FT_STATUS_OK        (1)
#define FT_STATUS_FAIL      (2)

#define FT_N_ELEMENTS(arr)  (sizeof(arr) / sizeof((arr)[0]))

enum
{
    FT_ITEM_VER = 0,
    FT_ITEM_KEY,
    FT_ITEM_LED,
    FT_ITEM_MOTOR,
    FT_ITEM_SPK,
    FT_ITEM_LOOP,
    FT_ITEM_HEADSET,
    FT_ITEM_LCD,
    FT_ITEM_MATRIX,
    FT_ITEM_ADC,
    FT_ITEM_CAMERA,
    FT_ITEM_GSM,
    FT_ITEM_IMEI_SN,
    FT_ITEM_BT,
    FT_ITEM_WIFI,
    FT_ITEM_FM,
    FT_ITEM_GPS,
    FT_ITEM_SD,
    FT_ITEM_G_SENS,
    FT_ITEM_INFRA_SENS,
    FT_ITEM_OPTI_SENS,
};

#endif/*_FT_TYPE_H_*/
