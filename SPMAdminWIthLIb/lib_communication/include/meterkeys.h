#ifndef METERKEYS_H
#define METERKEYS_H

#include <QObject>

enum EnCustomer
{
    CUST_APDCL,
    CUST_INTELLI,
    CUST_JAMMU,
    CUST_KASHMIR,
    CUST_PANASONIC,
    CUST_APARVA,
    CUST_DEMO,
    CUST_OTHER,
    CUST_NOT_FOUND,
};

class MeterKeys
{
public:
    static constexpr int METER_NUMBER_LENGTH = 9;
private:
       EnCustomer customer = EnCustomer::CUST_JAMMU ;

public:
     void SetCustomer(EnCustomer newCustomer)
    {
        customer = newCustomer;
    }

     EnCustomer GetCustomer()
    {
        return customer;
    }

     QList<QString> GetHLSKeyList={"ASSOCIATION_UTST", "HLSKEY_INTELLI01", "HLSKEY_JANKASPDC", "HLSKEY_JANKASPDC", "HLSKEY_PANASONIC", "AparvaHIGHCcurHL", "aaaaaaaaaaaaaaaa", "aaaaaaaaaaaaaaaa", ""};
     QList<QString> GetAuthenticationKeyList = {"ENCRYPTION_UNICT", "AUTHENKEY_INTELI", "AUTHENKEY_JANKA1", "AUTHENKEY_JANKA1", "ENCRYP_PANASONIC", "AparvNcrypCcurEN", "AAAAAAAAAAAAAAAA", "AAAAAAAAAAAAAAAA", ""};
     QString GetHLSKey()
     {
         return GetHLSKeyList[GetCustomer()];
     }

     QList<QString> GetEncryptionKeyList={"ENCRYPTION_UNICT","AUTHENKEY_INTELI","AUTHENKEY_JANKA1","AUTHENKEY_JANKA1","ENCRYP_PANASONIC","AparvNcrypCcurEN","AAAAAAAAAAAAAAAA","AAAAAAAAAAAAAAAA",""};
     QString GetEncryptionKey()
     {
         return GetEncryptionKeyList[GetCustomer()];
     }
     QString GetAuthenticationKey()
     {
         return GetAuthenticationKeyList[GetCustomer()];
     }
     QList<int> GetNWChannelList = {0x06, 0x03, 0x4, 0x4, 0x05,  0x02,  0x01,0x06,0 };
     int GetNWChannel()
     {
        return GetNWChannelList[GetCustomer()];
     }
     QList<qint32> GetNWAddressList = {0X504F57, 0x302D35, 0x403C92, 0x403C92, 0x302D30, 0x252A25, 0x223344, 0X504F57, 0 };

     qint32 GetNWAddress()
     {
        return GetNWAddressList[GetCustomer()];
     }
     QList<QString> GetRFSecurityKeyList = {"ENCRYPTION_RFNET",  "RFENCRYPT_INTELI",  "RFENCRYPT_JNKNET", "RFENCRYPT_JNKNET",
                                            "RFENCRYPT_PANASC","RFENCRYPT_APARVA","RFENCRYPT_DEMO12", "ENCRYPTION_RFNET",  "" };
     QString GetRFSecurityKey()
     {
         return GetRFSecurityKeyList[GetCustomer()];
     }
};

#endif // METERKEYS_H
