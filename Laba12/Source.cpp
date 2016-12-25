#include <afxwin.h>
#define RANDOM_COLOR RGB(rand() % 256, rand() % 256, rand() % 256)
#define PI 3.14159265
#define BACK_COLOR RGB(255, 255, 255)

double doubleRand(double max, unsigned precision = 8) {
	unsigned random = rand() % precision;
	return max / precision * random;
}

class CMainWnd : public CFrameWnd {
public:
	int oX = 0;
	int oY = 0;
	int x1, y1, x2, y2;
	CMainWnd();//конструктор по умолчанию
	afx_msg void OnPaint(); //вызов обработчика сообщений
	afx_msg void OnKeyDown(UINT, UINT, UINT);
	afx_msg void OnLButtonDown(UINT, CPoint);
	afx_msg void OnRButtonDown(UINT, CPoint);
	CDC m_memDC; //переменная состояния; указатель контекста рисования
	CBitmap m_bmp; // второй указатель контекста точечного рисования
	CBrush m_bkbrush; //объявление кисти
	void ClearWindow(COLORREF color = BACK_COLOR);
private:
	DECLARE_MESSAGE_MAP(); //макрос,т.е. данный класс будет реагировать на выше перечисленное
};

BEGIN_MESSAGE_MAP(CMainWnd, CFrameWnd) //включить макрокоманду в очередь сообщений
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
END_MESSAGE_MAP() // конец формирования очереди сообщений

void CMainWnd::ClearWindow(COLORREF color) {
	RECT clientRect; //инициализация экземпляра
	this->GetClientRect(&clientRect);

	CPen pen(PS_NULL, 0, RGB(0, 0, 0)); // создание кисти
	m_memDC.SelectObject(pen); //выбор логического объекта для CDC

	CBrush brush;
	brush.CreateSolidBrush(color); //установка цвета кисти
	m_memDC.SelectObject(brush);
	m_memDC.Rectangle(0, 0, clientRect.right, clientRect.bottom); //рисование прямоугольника x. y ширина, высота
	this->InvalidateRect(0, FALSE); //перерисовка всего окна, т.к. FALSE фон останется прежним
}

CMainWnd::CMainWnd() {
	Create(NULL, L"Lab12", WS_OVERLAPPEDWINDOW, rectDefault, NULL, NULL);// создание окна, имеющее стили
	int maxX = GetSystemMetrics(SM_CXSCREEN); //размеры элементов отображения Windows; ширина экрана
	int maxY = GetSystemMetrics(SM_CYSCREEN);
	CClientDC dc(this); //объект, обращающися к клиентской базе
	m_memDC.CreateCompatibleDC(&dc); //создание контекста памяти совместимое с DC
	m_bmp.CreateCompatibleBitmap(&dc, maxX, maxY);
	m_memDC.SelectObject(&m_bmp);
	m_bkbrush.CreateStockObject(WHITE_BRUSH); //создание кисти с заданным цветом
	m_memDC.SelectObject(&m_bkbrush);
	m_memDC.PatBlt(0, 0, maxX, maxY, PATCOPY);//окрашивает заданный прямоугольник, используя кисть, которая в текущий момент выбрана в заданный контекст устройства
	ClearWindow();
}

void CMainWnd::OnPaint() {
	CPaintDC paintDC(this);
	RECT clientRect;
	this->GetClientRect(&clientRect);
	paintDC.BitBlt(0, 0, clientRect.right, clientRect.bottom, &m_memDC, 0, 0, SRCCOPY); //выполняет передачу битовых блоков данных о цвете, соответствующих прямоугольнику пикселей из заданного исходного контекста Копирует исходный прямоугольник непосредственно в целевой прямоугольник.
}

void CMainWnd::OnKeyDown(UINT ch, UINT, UINT) {
		RECT clientRect;
		this->GetClientRect(&clientRect);

		int penWidth = rand() % 3 + 1;
		COLORREF penColor = RANDOM_COLOR;
		CPen pen(PS_SOLID, penWidth, penColor);
		m_memDC.SelectObject(pen);

		CBrush brush;
		COLORREF brushColor = RANDOM_COLOR;
		brush.CreateSolidBrush(brushColor);
		m_memDC.SelectObject(brush);

		int width = rand() % 150 + 50;
		int height = rand() % 150 + 50;
		x1 = oX;
		y1 = oY;
		x2 = oX + width;
		y2 = oY + height;
		m_memDC.Rectangle(x1, y1, x2, y2);
		double x = (double)width / 2;
		double y = (double)height / 2;
		oX = x1 + (int) x;
		oY = y1 + (int) y;
	this->InvalidateRect(0, FALSE);
}

void CMainWnd::OnLButtonDown(UINT, CPoint) {
	ClearWindow();
}

void CMainWnd::OnRButtonDown(UINT, CPoint) {
	ClearWindow();
	RECT clientRect;
	this->GetClientRect(&clientRect);

	int x = clientRect.right/2;
	int y = clientRect.bottom/2;

	m_memDC.SetTextColor(RANDOM_COLOR);
	m_memDC.SetBkMode(TRANSPARENT);//установливает режим смешивания фона указанного контекста устройства; Фоновый режим не изменен перед рисунком
	m_memDC.SelectObject(CreateFont(rand()%50 + 80,  // высота шрифта
	rand()%32 + 32, //средняя ширина символа
	rand()%30, //угол наклона
	0,  // угол ориентации базисной линии
	(rand()%9)*100, // = 0 толщина шрифта
	rand()%1, // описатель параметра курсивного шрифта
	FALSE, // описатель параметра подчеркивания
	FALSE,// описатель параметра зачеркивания
	ANSI_CHARSET, //идентификатор набора символов
	OUT_DEFAULT_PRECIS,  // по умолчанию точность ввода
	CLIP_DEFAULT_PRECIS, //точность отсечения
	rand()%5, // приглаживание = качество ввода
	DEFAULT_PITCH, //шаг между символами шрифта и семейство
	NULL)); //имя гарнитуры шрифта

	RECT position;
	position.left = -max(x, clientRect.right - x);
	position.right = 2 * x - position.left;
	position.top = -max(y, clientRect.bottom - y);
	position.bottom = 2 * y - position.top;

	m_memDC.DrawText(L"Samira", &position, DT_SINGLELINE | DT_CENTER | DT_VCENTER); //текст только на 1-ой строке
	this->InvalidateRect(0, FALSE);
}

class CMyApp : public CWinApp {
public:
	CMyApp();
	virtual BOOL InitInstance();
};

CMyApp::CMyApp() {}

BOOL CMyApp::InitInstance() {
	m_pMainWnd = new CMainWnd();
	ASSERT(m_pMainWnd);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

CMyApp theApp;
