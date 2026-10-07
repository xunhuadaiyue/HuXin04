
// HuXinView.cpp: CHuXinView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HuXin.h"
#endif

#include "HuXinDoc.h"
#include "HuXinView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHuXinView

IMPLEMENT_DYNCREATE(CHuXinView, CView)

BEGIN_MESSAGE_MAP(CHuXinView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_COMMAND(ID_Quaternion_Interpolation, &CHuXinView::OnQuaternionInterpolation)
	ON_COMMAND(ID_Angle_Interpolation, &CHuXinView::OnAngleInterpolation)
	ON_COMMAND(ID_Spherical_Quaternion, &CHuXinView::OnSphericalQuaternion)
END_MESSAGE_MAP()

// CHuXinView 构造/析构

CHuXinView::CHuXinView() noexcept
{
	// TODO: 在此处添加构造代码
	this->m_GLPixelIndex = 0;
	this->m_hGLContext = NULL;

	m_xrotation = 0.0;
	m_yrotation = 90.0;
	m_zrotation = 0.0;

	m_xrotation_begin = 0.0;
	m_yrotation_begin = 90.0;
	m_zrotation_begin = 0.0;

	m_xrotation_end = 90.0;
	m_yrotation_end = 45.0;
	m_zrotation_end = 90.0;

	t = 0;

}

CHuXinView::~CHuXinView()
{
}

BOOL CHuXinView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);    //窗口风格


	return CView::PreCreateWindow(cs);
}

// CHuXinView 绘图

void CHuXinView::OnDraw(CDC* pDC)
{
	CHuXinDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(m_zrotation, 0.0, 0.0, 1.0);
	glRotatef(m_yrotation, 0.0, 1.0, 0.0);
	glRotatef(m_xrotation, 1.0, 0.0, 0.0);

	//////////////////////////////////////////////
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);

	float mat_ambient[] = { 0.1,0.1,0.1,1 };
	float mat_diffuse[] = { 0.5,0.3,0.1,1 };
	float mat_specular[] = { 0.9,0.9,0.9,1 };
	float mat_emission[] = { 0.5,0,0,1 };
	float mat_shiness[] = { 30 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiness);

	glColor3f(1.0, 0.0, 0.0);
	auxSolidTeapot(0.5);
	

	SwapBuffers(pDC->m_hDC);

}


// CHuXinView 打印

BOOL CHuXinView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHuXinView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHuXinView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

BOOL CHuXinView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR  pixelDesc = { sizeof(PIXELFORMATDESCRIPTOR), 1,
																			   PFD_DRAW_TO_WINDOW |
																			   PFD_SUPPORT_OPENGL |
																			   PFD_DOUBLEBUFFER,
																			   PFD_TYPE_RGBA,   24,  0,0,0,0,0,0,  0,  0,  0,
																			   0,0,0,0,  32,   0,  0, 0,  0,  0,0,0 };
	m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);

	if (m_GLPixelIndex == 0)
	{
		return FALSE;
	}

	if (SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CHuXinView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	if (m_hGLContext == NULL) //创建失败
	{
		return FALSE;
	}

	if (wglMakeCurrent(hDC, m_hGLContext) == FALSE)
	{//选为当前RC失败
		return FALSE;
	}
	return TRUE;
}


// CHuXinView 诊断

#ifdef _DEBUG
void CHuXinView::AssertValid() const
{
	CView::AssertValid();
}

void CHuXinView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHuXinDoc* CHuXinView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHuXinDoc)));
	return (CHuXinDoc*)m_pDocument;
}
#endif //_DEBUG


// CHuXinView 消息处理程序

int CHuXinView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	HWND hWnd = this->GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if (this->SetWindowPixelFormat(hDC) == FALSE)
	{
		return 0;
	}

	if (this->CreateViewGLContext(hDC) == FALSE)
	{
		return 0;
	}


	//////////////
	glewInit();
	glDrawBuffer(GL_BACK);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	/////////////

		////////////////////////////////////////
	//如果光照定义放在这里，光源位置相对视点固定
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float light_ambient[] = { 0.1,0.1,0.1,1 };
	float light_diffuse[] = { 1,1,1,1 };
	float light_specular[] = { 1,1,1,1 };
	float light_position[] = { 1,1,1,0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	////////////////////////////////////////

	SetTimer(0, 0.33, NULL);

	return 0;
}

void CHuXinView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (wglGetCurrentContext() != NULL)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (this->m_hGLContext != NULL)
	{
		wglDeleteContext(this->m_hGLContext);
		this->m_hGLContext = NULL;
	}

}

void CHuXinView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	glViewport(0, 0, cx, cy);
	GLdouble aspect;
	if (cy == 0)
	{
		aspect = (GLdouble)cx;
	}
	else
	{
		aspect = (GLdouble)cx / (GLdouble)cy;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-2.0 * aspect, 2.0 * aspect, -2.0, 2.0, -13.0, 13.0);
	Invalidate();

}

BOOL CHuXinView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CView::OnEraseBkgnd(pDC);

	return TRUE;
}

void CHuXinView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (interpolation_mode == 1)
	{
		if (m_xrotation <= m_xrotation_end && m_yrotation >= m_yrotation_end && m_zrotation <= m_zrotation_end)
		{
			m_xrotation += 0.005 * (m_xrotation_end - m_xrotation_begin);
			m_yrotation += 0.005 * (m_yrotation_end - m_yrotation_begin);
			m_zrotation += 0.005 * (m_zrotation_end - m_zrotation_begin);
		}
	}

	else if (interpolation_mode == 2)
	{
		///////////////////////////////////////////////////////////////////////四元数插值
		Quaternion_begin.SetEulerAngle(m_xrotation_begin, m_yrotation_begin, m_zrotation_begin);
		Quaternion_end.SetEulerAngle(m_xrotation_end, m_yrotation_end, m_zrotation_end);

		float temp = Quaternion_begin.qw * Quaternion_end.qw + Quaternion_begin.qx * Quaternion_end.qx + Quaternion_begin.qy * Quaternion_end.qy + Quaternion_begin.qz * Quaternion_end.qz;
		if (temp < 0)
		{
			Quaternion_end.qw *= -1;
			Quaternion_end.qx *= -1;
			Quaternion_end.qy *= -1;
			Quaternion_end.qz *= -1;
		}

		if (t < 1.0)
			t += 0.005;

		Quaternion_middle.qw = (1 - t) * Quaternion_begin.qw + t * Quaternion_end.qw;
		Quaternion_middle.qx = (1 - t) * Quaternion_begin.qx + t * Quaternion_end.qx;
		Quaternion_middle.qy = (1 - t) * Quaternion_begin.qy + t * Quaternion_end.qy;
		Quaternion_middle.qz = (1 - t) * Quaternion_begin.qz + t * Quaternion_end.qz;

		Quaternion_middle.qw /= sqrt(Quaternion_middle.qw * Quaternion_middle.qw + Quaternion_middle.qx * Quaternion_middle.qx + Quaternion_middle.qy * Quaternion_middle.qy + Quaternion_middle.qz * Quaternion_middle.qz);
		Quaternion_middle.qx /= sqrt(Quaternion_middle.qw * Quaternion_middle.qw + Quaternion_middle.qx * Quaternion_middle.qx + Quaternion_middle.qy * Quaternion_middle.qy + Quaternion_middle.qz * Quaternion_middle.qz);
		Quaternion_middle.qy /= sqrt(Quaternion_middle.qw * Quaternion_middle.qw + Quaternion_middle.qx * Quaternion_middle.qx + Quaternion_middle.qy * Quaternion_middle.qy + Quaternion_middle.qz * Quaternion_middle.qz);
		Quaternion_middle.qz /= sqrt(Quaternion_middle.qw * Quaternion_middle.qw + Quaternion_middle.qx * Quaternion_middle.qx + Quaternion_middle.qy * Quaternion_middle.qy + Quaternion_middle.qz * Quaternion_middle.qz);

		Quaternion_middle.GetEulerAngle(m_xrotation, m_yrotation, m_zrotation);
		/////////////////////////////////////////////////////////////////////////////////
	}

	else if (interpolation_mode == 3)
	{
		Quaternion_begin.SetEulerAngle(m_xrotation_begin, m_yrotation_begin, m_zrotation_begin);
		Quaternion_end.SetEulerAngle(m_xrotation_end, m_yrotation_end, m_zrotation_end);

		float temp = Quaternion_begin.qw * Quaternion_end.qw + Quaternion_begin.qx * Quaternion_end.qx
			+ Quaternion_begin.qy * Quaternion_end.qy + Quaternion_begin.qz * Quaternion_end.qz;
		if (temp < 0)
		{
			Quaternion_end.qw *= -1; Quaternion_end.qx *= -1;
			Quaternion_end.qy *= -1; Quaternion_end.qz *= -1;
			temp = -temp;
		}

		if (temp > 1.0f) temp = 1.0f;
		if (temp < -1.0f) temp = -1.0f;

		float theta = acos(temp);
		float sin_theta = sin(theta);

		if (t < 1.0) t += 0.005;

		float s0, s1;
		if (sin_theta > 0.0001f)
		{
			s0 = sin((1.0f - t) * theta) / sin_theta;
			s1 = sin(t * theta) / sin_theta;
		}
		else
		{
			s0 = 1.0f - t;
			s1 = t;
		}

		Quaternion_middle.qw = s0 * Quaternion_begin.qw + s1 * Quaternion_end.qw;
		Quaternion_middle.qx = s0 * Quaternion_begin.qx + s1 * Quaternion_end.qx;
		Quaternion_middle.qy = s0 * Quaternion_begin.qy + s1 * Quaternion_end.qy;
		Quaternion_middle.qz = s0 * Quaternion_begin.qz + s1 * Quaternion_end.qz;

		float norm = sqrt(Quaternion_middle.qw * Quaternion_middle.qw + Quaternion_middle.qx * Quaternion_middle.qx
			+ Quaternion_middle.qy * Quaternion_middle.qy + Quaternion_middle.qz * Quaternion_middle.qz);
		if (norm > 0.0001f)
		{
			Quaternion_middle.qw /= norm;
			Quaternion_middle.qx /= norm;
			Quaternion_middle.qy /= norm;
			Quaternion_middle.qz /= norm;
		}

		Quaternion_middle.GetEulerAngle(m_xrotation, m_yrotation, m_zrotation);
	}

	/////////////////////////////////////////////////////////////////////////////////

	Invalidate(TRUE);


	CView::OnTimer(nIDEvent);

}

void CHuXinView::OnQuaternionInterpolation()
{
	// TODO: 在此添加命令处理程序代码
	t = 0;
	
	interpolation_mode = 2;

}

void CHuXinView::OnAngleInterpolation()
{
	// TODO: 在此添加命令处理程序代码
	m_xrotation = m_xrotation_begin;
	m_yrotation = m_yrotation_begin;
	m_zrotation = m_zrotation_begin;

	interpolation_mode = 1;
}

void CHuXinView::OnSphericalQuaternion()
{
	// TODO: 在此添加命令处理程序代码
	t = 0;

	interpolation_mode = 3;
}
