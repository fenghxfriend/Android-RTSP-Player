#ifndef _ANTS360_AUTOFILE_H_
#define _ANTS360_AUTOFILE_H_

#include <ants360typedefs.h>
#include <stdio.h>

namespace ants360 {


class CAutoFile
{
public:
    CAutoFile(LPCSTR szFilename, LPCSTR szMode = "rb")
    {
        m_hFile = fopen(szFilename, szMode);
    }

    ~CAutoFile()
    {
        if (m_hFile)
        {
            fclose(m_hFile);
        }
    }

    u32 write(const void *ptr, u32 bytes)
    {
        if (!ptr || !m_hFile)
        {
            return 0;
        }
        return fwrite(ptr, 1, bytes, m_hFile);
    }

    u32 read(void *ptr, u32 max_bytes)
    {
        if (!ptr || !m_hFile)
        {
            return 0;
        }
        return fread(ptr, 1, max_bytes, m_hFile);
    }

    BOOL32 isValid()
    {
        return m_hFile != NULL;
    }

    void flush()
    {
        if (!m_hFile)
        {
            return;
        }
        fflush(m_hFile);
    }

    operator FILE*()
    {
        return m_hFile;
    }

private:
    FILE *m_hFile;
};


} // namepsace ants360

#endif // _ANTS360_AUTOFILE_H_