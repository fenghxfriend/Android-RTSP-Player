#ifndef _ANTS360_SMARTPOINTER_H_
#define _ANTS360_SMARTPOINTER_H_

#include "ants360typedefs.h"

namespace ants360 {

class CRefBase
{
public:
    CRefBase()
        : m_dwRefCount(0)
    {
    }

    ~CRefBase()
    {
    }

    void AddReference()
    {
        ++m_dwRefCount;
    }

    void ReleaseReference()
    {
        --m_dwRefCount;
        if (m_dwRefCount == 0)
        {
            delete this;
        }
    }

public:
    u32 m_dwRefCount;
};

#define COMPARE(_op_)                                                  \
inline bool operator _op_ (const CSmartPointer<REF_TYPE>& o) const {   \
    return m_p _op_ o.m_p;                                             \
}                                                                      \
inline bool operator _op_ (const REF_TYPE* o) const {                  \
    return m_p _op_ o;                                                 \
}                                                                      \

template<typename REF_TYPE>
class CSmartPointer
{
public:
    inline CSmartPointer() : m_p(NULL) { }

    CSmartPointer(REF_TYPE* p) : m_p(p)
    {
        if (m_p)
        {
            m_p->AddReference();
        }
    }

    CSmartPointer(const CSmartPointer<REF_TYPE> &ap)
        : m_p(ap.m_p)
    {
        if (m_p)
        {
            m_p->AddReference();
        }
    }

    CSmartPointer& operator = (REF_TYPE* p)
    {
        if (m_p)
        {
            m_p->ReleaseReference();
        }

        m_p = p;
        if (m_p)
        {
            m_p->AddReference();
        }
        return *this;
    }

    CSmartPointer& operator = (const CSmartPointer &ap)
    {
        if (m_p)
        {
            m_p->ReleaseReference();
        }

        m_p = ap.m_p;
        if (m_p)
        {
            m_p->AddReference();
        }
        return *this;
    }

    ~CSmartPointer()
    {
        if (m_p)
        {
            m_p->ReleaseReference();
        }
    }

    inline REF_TYPE* get() const
    {
        return m_p;
    }

    inline REF_TYPE* operator->() const
    {
        return m_p;
    }

    inline REF_TYPE operator*() const
    {
        return *m_p;
    }

    COMPARE(==)
    COMPARE(!=)
    COMPARE(>)
    COMPARE(<)
    COMPARE(<=)
    COMPARE(>=)
    
public:
    REF_TYPE* m_p;
};

#undef COMPARE


} // namespace ants360

#endif // _ANTS360_SMARTPOINTER_H_