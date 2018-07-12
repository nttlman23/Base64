
#include <string>
#include <string.h>
#include <iostream>
#include <assert.h>

#include <base64.h>

// QUJDRA==
const uint8_t base64_chars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";
        
static void progress_draw(int percentage)
{
    int blsq = percentage / 5;
    int whtsq = 20 - blsq;
    int i;
    
    fprintf(stdout, "\rProcessing ");
    
    for (i = 0;i < blsq;i++)
    {
        fprintf(stdout, "%s", BLACK_SQUARE);
    }
            
    for (i = 0;i < whtsq;i++)
    {
        fprintf(stdout, "%s", WHITE_SQUARE);
    }
            
    fprintf(stdout, " %d %%", percentage);
    fflush(stdout);
}

// static void progress_draw(int percentage)
// {
//     int blsq = percentage / 5;
//     int whtsq = 20 - blsq;
//     int i;
//     char buffer[100] = { 0 };
//     int len = 0;
//     
//     sprintf(buffer + len, "\rProcessing ");
//     
//     len = strlen(buffer);
//     
//     for (i = 0;i < blsq;i++)
//     {
//         sprintf(buffer + len, "%s", BLACK_SQUARE);
//         len++;
//     }
//     
//     for (i = 0;i < whtsq;i++)
//     {
//         sprintf(buffer + len, "%s", WHITE_SQUARE);
//         len++;
//     }
//     
//     sprintf(buffer + len, " %d %%", percentage);
//     
//     fprintf(stdout, "%s", buffer);
//     fflush(stdout);
// }

void hexDump(const char *desc, void *addr, int len, int offset)
{
    assert(addr);
    int i;
    unsigned char buff[17];
    unsigned char *pc = (unsigned char*)addr;
    
    if (desc != NULL)
    {
        printf ("%s:\n", desc);
    }
    
    for (i = 0; i < len; i++)
    {
        if ((i % 16) == 0)
        {
            if (i != 0)
            {
                printf("  %s\n", buff);
            }
            printf("%08x ", i + (offset * 0x200));
        }
        printf(" %02x", pc[i]);
        if ((pc[i] < 0x20) || (pc[i] > 0x7e))
        {
            buff[i % 16] = '.';
        }
        else
        {
            buff[i % 16] = pc[i];
        }
        
        buff[(i % 16) + 1] = '\0';
    }
    while ((i % 16) != 0)
    {
        printf("   ");
        i++;
    }
    printf("  %s\n", buff);
}

static inline unsigned char b64_lookup(unsigned char c)
{
    if(c >='A' && c <='Z')
    {
        return c - 'A';
    }
    if(c >='a' && c <='z')
    {
        return c - 71;
    }
    if(c >='0' && c <='9')
    {
        return c + 4;
    }
    if(c == '+')
    {
        return 62;
    }
    if(c == '/')
    {
        return 63;
    }
    return 255;
}

void cnvrt826(unsigned char *arr8, unsigned char *arr6)
{
    arr6[0] = ((arr8[0] & 0xfc) >> 2);
    arr6[1] = ((arr8[0] & 0x03) << 4) + ((arr8[1] & 0xf0) >> 4);
    arr6[2] = ((arr8[1] & 0x0f) << 2) + ((arr8[2] & 0xc0) >> 6);
    arr6[3] = (arr8[2] & 0x3f);
}

void cnvrt628(unsigned char *arr6, unsigned char *arr8)
{
    arr8[0] = (arr6[0] << 2) + ((arr6[1] & 0x30) >> 4);
    arr8[1] = ((arr6[1] & 0xf) << 4) + ((arr6[2] & 0x3c) >> 2);
    arr8[2] = ((arr6[2] & 0x3) << 6) + arr6[3];
}

std::string B64Encode(unsigned char const *data, int dataSize)
{
    assert(data);
    std::string result = "";
    unsigned char arr6[4] = { 0 };
    unsigned char arr8[3] = { 0 };
    int i = 0;
    int fullDataSize = dataSize;

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
        
        float prct = ((float)(fullDataSize - dataSize) / (float)fullDataSize) * 100.0;
        
        progress_draw((int)prct);
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
    
    progress_draw(100);
    fprintf(stdout, "\n");
    
    return result;
}

std::string B64Decode(unsigned char const *data, int dataSize)
{
    assert(data);
    std::string result;
    int fullDataSize = dataSize;
    unsigned char arr6[4] = { 0 };
    unsigned char arr8[3] = { 0 };
    int i = 0;
    int j;

    while(dataSize--)
    {
        if ((*data) == '=')
        {
            break;
        }
        if (b64_lookup(*data) == 255)
        {
            data++;
            continue;
        }
            
        arr6[i++] = *(data++);
        
        if (i == 4)
        {
            for ( j = 0; j < 4; j++)
            {
                arr6[j] = b64_lookup(arr6[j]);
            }
            
            cnvrt628(arr6, arr8);
            
            for ( j = 0; j < 3; j++)
            {
                result += arr8[j];
            }
            i = 0;
        }
        
        float prct = ((float)(fullDataSize - dataSize) / (float)fullDataSize) * 100.0;
        
        progress_draw((int)prct);
    }
    
    if (i)
    {
        for (j = i; j < 4;j++)
        {
            arr6[j] = '\0';
        }
        
        for ( j = 0; j < 4; j++)
        {
            arr6[j] = b64_lookup(arr6[j]);
        }

        cnvrt628(arr6, arr8);
        
        for ( j = 0; j < i - 1; j++)
        {
            result += arr8[j];
        }
    }
    
    progress_draw(100);
    fprintf(stdout, "\n");
    
    return result;
}

