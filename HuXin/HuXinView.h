
// HuXinView.h: CHuXinView 类的接口
//

#pragma once
#include "Quaternion.h"





class CHuXinView : public CView
{
protected: // 仅从序列化创建
	CHuXinView() noexcept;
	DECLARE_DYNCREATE(CHuXinView)


// 特性
public:
	CHuXinDoc* GetDocument() const;

	int m_GLPixelIndex;
	HGLRC m_hGLContext;

	///////////////////////////
	float m_xrotation;
	float m_yrotation;
	float m_zrotation;
	float m_xrotation_begin;
	float m_yrotation_begin;
	float m_zrotation_begin;
	float m_xrotation_end;
	float m_yrotation_end;
	float m_zrotation_end;
	Quaternion Quaternion_begin;
	Quaternion Quaternion_end;
	Quaternion Quaternion_middle;
	float t;
	int interpolation_mode;
	///////////////////////////

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);


// 实现
public:
	virtual ~CHuXinView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnQuaternionInterpolation();
	afx_msg void OnAngleInterpolation();
	afx_msg void OnSphericalQuaternion();
};

#ifndef _DEBUG  // HuXinView.cpp 中的调试版本
inline CHuXinDoc* CHuXinView::GetDocument() const
   { return reinterpret_cast<CHuXinDoc*>(m_pDocument); }
#endif

