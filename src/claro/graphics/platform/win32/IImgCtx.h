/* 

Header based on MS Headers and strnghrs codeproject article
at http://www.codeproject.com/bitmap/JianImgCtxDecoder.asp

*/


#ifndef _IImgCtx_H
#define _IImgCtx_H

#define IMGCHG_SIZE         0x0001
#define IMGCHG_VIEW         0x0002
#define IMGCHG_COMPLETE     0x0004
#define IMGCHG_ANIMATE      0x0008
#define IMGCHG_MASK         0x000F

#define IMGLOAD_NOTLOADED   0x00100000  
#define IMGLOAD_LOADING     0x00200000  
#define IMGLOAD_STOPPED     0x00400000  
#define IMGLOAD_ERROR       0x00800000  
#define IMGLOAD_COMPLETE    0x01000000  
#define IMGLOAD_MASK        0x01F00000

#define IMGBITS_NONE        0x02000000
#define IMGBITS_PARTIAL     0x04000000
#define IMGBITS_TOTAL       0x08000000
#define IMGBITS_MASK        0x0E000000

#define IMGANIM_ANIMATED    0x10000000
#define IMGANIM_MASK        0x10000000

#define IMGTRANS_OPAQUE     0x20000000
#define IMGTRANS_MASK       0x20000000

#define DWN_COLORMODE       0x0000003F  
#define DWN_DOWNLOADONLY    0x00000040  
#define DWN_FORCEDITHER     0x00000080  
#define DWN_RAWIMAGE        0x00000100  
#define DWN_MIRRORIMAGE     0x00000200  

#undef INTERFACE
#define INTERFACE IImgCtx

typedef void (CALLBACK *PFNIMGCTXCALLBACK)(void *, void *);

DECLARE_INTERFACE_(IImgCtx, IUnknown)
{
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID FAR* ppvObj) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    STDMETHOD(Load)(THIS_ LPCWSTR pszUrl, DWORD dwFlags) PURE;
    STDMETHOD(SelectChanges)(THIS_ ULONG ulChgOn, ULONG ulChgOff, BOOL fSignal) PURE;
    STDMETHOD(SetCallback)(THIS_ PFNIMGCTXCALLBACK pfn, void * pvPrivateData) PURE;
    STDMETHOD(Disconnect)(THIS) PURE;

    STDMETHOD(GetUpdateRects)(THIS_ struct tagRECT FAR* prc, struct tagRECT FAR* prcImg, long FAR* pcrc) PURE;
    STDMETHOD(GetStateInfo)(THIS_ ULONG FAR* pulState, struct tagSIZE FAR* psize, BOOL fClearChanges) PURE;
    STDMETHOD(GetPalette)(THIS_ HPALETTE FAR* phpal) PURE;

    STDMETHOD(Draw)(THIS_ HDC hdc, struct tagRECT FAR* prcBounds) PURE;
    STDMETHOD(Tile)(THIS_ HDC hdc, struct tagPOINT FAR* pptBackOrg, struct tagRECT FAR* prcClip, struct tagSIZE FAR* psize) PURE;
    STDMETHOD(StretchBlt)(THIS_ HDC hdc, int dstX, int dstY, int dstXE, int dstYE, int srcX, int srcY, int srcXE, int srcYE, DWORD dwROP) PURE;
};
#endif
