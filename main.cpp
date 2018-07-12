
#include <iostream>
#include <string.h>
#include <base64.h>
#include <getopt.h>
#include <stdio.h>
#include <errno.h>
#include <fstream>
#include <assert.h>
#include <unistd.h>

#define BLACK_SQUARE        "▪"
#define WHITE_SQUARE        "▫"

#define TIMEOUT             100000

void progress_draw(int percentage)
{
    int blsq = percentage / 5;
    int whtsq = 20 - blsq;
    int i;
    
    fprintf(stdout, "\rLoading ");
    
    for (i = 0;i < blsq;i++)
    {
        fprintf(stdout, "%s", BLACK_SQUARE);
    }
    
    for (i = 0;i < whtsq;i++)
    {
        fprintf(stdout, "%s", WHITE_SQUARE);
    }
    
    fprintf(stdout, " %d%%", percentage);
    fflush(stdout);
}

void hexDump(char *desc, void *addr, int len, int offset) 
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

void release_res(std::fstream &ifile, std::fstream &ofile, char *buffer)
{
    if (ifile.is_open())
    {
        ifile.close();
    }
    
    if (ofile.is_open())
    {
        ofile.close();
    }
    
    if (buffer)
    {
        delete [] buffer;
    }
}

char *_stringncpy(char *dest, std::string &src, size_t n)
{
    size_t i;
    
    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    
    for ( ; i < n; i++)
    {
        dest[i] = '\0';
    }
    
    return dest;
}

char *read_from_stdin(int &data_size)
{
    std::string data_str;    
    char *buffer = NULL;
    data_size = 0;
    
    fprintf(stdout, "Source string (Ctrl+D - stop):\n");
    
    while (!std::cin.eof())
    {
        std::string line;
        getline(std::cin, line);
        
        if (std::cin.fail())
        {
            //error
            break;
        }
        
        data_str += line;
    }
    
    data_size = data_str.length();
    
    if (data_size)
    {
        buffer = new char[data_size];
        memset(buffer, 0, data_size);
        strncpy(buffer, data_str.c_str(), data_size);
    }
    
    return buffer;
}

char *read_from_file(std::fstream &in, int &data_size)
{
    std::string data_str;    
    char *buffer = NULL;
    data_size = 0;
    
    in.seekg (0, in.end);
    data_size = in.tellg();
    in.seekg (0, in.beg);
    
    buffer = new char[data_size];
    
    in.read(buffer, data_size);
    
    if (in)
    {
        std::cout << "all characters read successfully." << std::endl;
    }
    else
    {
        std::cout << "error: only " << in.gcount() <<
                                " could be read" << std::endl;
        delete [] buffer;
        return NULL;
    }
    
    return buffer;
}

int main(int argc, char** argv)
{
    
//     for (int i = 0;i <= 100;i++)
//     {
//         progress_draw(i);
//         usleep(TIMEOUT);
//     }
    
//     fprintf(stdout, "\n\n( ͡° ͜ʖ ͡°)\n\n");
    
    char *buffer = NULL;
    int stdin_io = 1;
    int stdout_io = 1;
    int data_size = 0;
    
    std::fstream in;
    std::fstream out;
    
    int decode = 0;
    
    const char* const short_opts = "i:o:d";
    const struct option long_opts[] = {
        {"input",       required_argument,  NULL, 'i'},
        {"output",      required_argument,  NULL, 'o'},
        {"decode",      required_argument,  NULL, 'd'},
        {NULL,          0,                  NULL,  0}
    };
    
    int res;
    int option_index;
    
    while ((res = getopt_long(argc, argv, short_opts, 
        long_opts, &option_index)) != -1)
    {
        switch(res)
        {
            case 'i':
            {
                stdin_io = 0;
                in.open(optarg, std::fstream::in);
                
                if (!in.is_open())
                {
                    fprintf(stdout, "Unable to open '%s': %s\n",
                            optarg, strerror(errno));
                    return -1;
                }
                break;
            }
            case 'o':
            {
                stdout_io = 0;
                out.open(optarg, std::fstream::out);
                
                if (!out.is_open())
                {
                    fprintf(stdout, "Unable to open '%s': %s\n",
                            optarg, strerror(errno));
                    return -1;
                }
                break;
            }
            case 'd':
            {
                decode = 1;
                break;
            }
        }
    }
    
    if (stdin_io)
    {
        buffer = read_from_stdin(data_size);
    }
    else
    {
        buffer = read_from_file(in, data_size);
    }
    
    std::string dest_str;
    
    if (decode)
    {
        dest_str = B64Decode((unsigned char const *)buffer, data_size);
    }
    else
    {
        dest_str = B64Encode((unsigned char const *)buffer, data_size);
    }
    
    if (out.is_open())
    {
        out.write (dest_str.c_str(), dest_str.length());
    }
    else
    {
        hexDump("Result", (void *)dest_str.c_str(), dest_str.length(), 0);
        std::cout << std::endl << "Result string: " << dest_str.c_str() << std::endl;
    }
    
    release_res(in, out, buffer);
   
    return 0;
}
