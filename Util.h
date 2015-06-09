#if !defined(__Util_hdr__)
#define __Util_hdr__

#include <cassert>
#include <cstring>

class CUtil
{
public:
    template<typename T>
    static T float2int (float fInput)
    {
        if (fInput >= 0.F)
            return static_cast<T>(fInput + .5F);
        else
            return static_cast<T>(fInput - .5F);
    }
    template<typename T>
    static T double2int (double fInput)
    {
        if (fInput >= 0)
            return static_cast<T>(fInput + .5);
        else
            return static_cast<T>(fInput - .5);
    }

    static bool isPowOf2 (int n) 
    {
        return !(n & (n-1));
    }


    template<typename T>
    static void setZero (T *pInput, int iLength)
    {
        assert (iLength >= 0);
        assert (pInput);

        if (iLength > 0)
            memset (pInput, 0, sizeof(T)*iLength);
    }
    template<typename T>
    static void copyBuff (T *pDest, const T *pSource, int iLength)
    {
        assert (iLength >= 0);

        if (iLength > 0)
        {
            assert (pDest);
            assert (pSource);
            memcpy (pDest, pSource, sizeof(T)*iLength);
        }
    }
    template<typename T>
    static void moveBuff (T *pSrcDest, int iDestIdx, int iSrcIdx, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);

        if (iLength > 0)
            memmove (&pSrcDest[iDestIdx], &pSrcDest[iSrcIdx], sizeof(T)*iLength);
    }

    static void multBuffC (float *pSrcDest, float fScale, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);
        
        for (int i = 0; i < iLength; i++)
            pSrcDest[i] *= fScale;
    }

    static void multBuff (float *pSrcDest, const float *pSrc, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);
        assert (pSrc);

        for (int i = 0; i < iLength; i++)
            pSrcDest[i] *= pSrc[i];
    }

    static void addBuff (float *pSrcDest, const float *pSrc, int iLength)
    {
        assert (iLength >= 0);
        assert (pSrcDest);
        assert (pSrc);

        for (int i = 0; i < iLength; i++)
            pSrcDest[i] += pSrc[i];
    }

};
#endif // __Util_hdr__