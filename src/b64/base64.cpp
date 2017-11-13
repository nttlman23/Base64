
#include <string>
#include <string.h>
#include <iostream>

// QUJDRA==

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void cnvrt826(unsigned char *arr8, unsigned char *arr6)
{
    arr6[0] = ((arr8[0] & 0xfc) >> 2);
    arr6[1] = ((arr8[0] & 0x03) << 4) + ((arr8[1] & 0xf0) >> 4);
    arr6[2] = ((arr8[1] & 0x0f) << 2) + ((arr8[2] & 0xc0) >> 6);
    arr6[3] = (arr8[2] & 0x3f);
}

void cnvrt628(unsigned char *arr6, unsigned char *arr8)
{
    arr8[0] = ((arr6[0] & 0x3f) << 2) + ((arr6[1] & 0x30) >> 4);
    arr8[1] = ((arr6[1] & 0x0f) << 4) + ((arr6[2] & 0x3c) >> 2);
    arr8[2] = ((arr6[2] & 0x03) << 6) + (arr6[3] & 0x3f);
}

std::string B64Encode(unsigned char const *data, int dataSize)
{
    std::string result = "";
    unsigned char arr6[4] = { 0 };
    unsigned char arr8[3] = { 0 };
    int i = 0;

    while (dataSize--)
    {
        arr8[i++] = *(data++);

        if (i == 3)
        {
            cnvrt826(arr8, arr6);
            
            for (i = 0;i < 4;i++)
            {
                result += base64_chars[arr6[i]];
            }
            i = 0;
        }
    }

    if (i)
    {
        int j = 0;
        for (j = i;j < 3;j++)
        {
            arr8[j] = '\0';
        }

        cnvrt826(arr8, arr6);

        for (j = 0;j < i + 1;j++)
        {
            result += base64_chars[arr6[j]];
        }
        
        while(i++ < 3)
        {
            result += '=';
        }
    }
    return result;
}

std::string B64Decode(unsigned char const *data, int dataSize)
{
    std::string result;
    unsigned char arr6[4] = { 0 };
    unsigned char arr8[3] = { 0 };
    int i = 0;
    int j;
    while((dataSize--) && (*data) != '=')
    {
        arr6[i++] = *(data++);
        
        if (i == 4)
        {            
            for ( j = 0; j < 4; j++)
            {
                arr6[j] = base64_chars.find(arr6[j]);
            }
            
            cnvrt628(arr6, arr8);
            
            for ( j = 0; j < 3; j++)
            {
                result += arr8[j];
            }
            i = 0;
        }
        
    }
    
    if (i)
    {
        for ( j = 0; j < i - 1; j++)
        {
            arr6[j] = base64_chars.find(arr6[j]);
        }
            
        cnvrt628(arr6, arr8);
           
        for ( j = 0; j < i - 1; j++)
        {
            result += arr8[j];
        }
    }

    return result;
}

