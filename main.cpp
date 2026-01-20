#include <windows.h>
#include <windowsx.h>

// Идентификаторы элементов управления
#define IDC_EDIT     1001
#define IDC_BUTTON   1002
#define IDC_BUTTON_CLEAR 1003

// Идентификаторы меню
#define IDR_MENU1    101
#define IDM_EXIT     40001
#define IDM_ABOUT    40002

// Глобальные переменные для дескрипторов окон
static HWND hEdit;
static HWND hStatic;

// Обработчик сообщений главного окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Точка входа программы
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	// Регистрация класса окна
	const char CLASS_NAME[] = "SimpleWindowClass";
	
	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;               // Обработчик сообщений
	wc.hInstance = hInstance;               // Экземпляр приложения
	wc.lpszClassName = CLASS_NAME;          // Имя класса
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Курсор
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Цвет фона
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1); // Меню из ресурсов
	wc.style = CS_HREDRAW | CS_VREDRAW;     // Стиль класса
	
	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, "Ошибка регистрации класса окна!", "Ошибка", MB_ICONERROR);
		return 0;
	}
	
	// Создание главного окна
	HWND hwnd = CreateWindowEx(
							   0,                                  // Дополнительные стили
							   CLASS_NAME,                         // Имя класса окна
							   "WinAPI приложение",                // Заголовок окна
							   WS_OVERLAPPEDWINDOW,                // Стиль окна
							   
							   // Позиция и размер
							   CW_USEDEFAULT, CW_USEDEFAULT, 
							   320, 250,
							   
							   NULL,                               // Родительское окно
							   NULL,                               // Меню
							   hInstance,                          // Экземпляр приложения
							   NULL                                // Дополнительные данные
							   );
	
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Ошибка создания окна!", "Ошибка", MB_ICONERROR);
		return 0;
	}
	
	// Отображение окна
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	
	// Цикл сообщений
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	
	return (int)msg.wParam;
}

// Обработчик сообщений главного окна
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// Сообщение создания окна
	case WM_CREATE:
	{
		// Статический текст
		hStatic = CreateWindow("STATIC", "Введите текст:", 
							   WS_CHILD | WS_VISIBLE | SS_LEFT,
							   20, 20, 100, 20, hwnd, NULL, NULL, NULL);
		
		// Текстовое поле для ввода
		hEdit = CreateWindow("EDIT", "",
							 WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL,
							 20, 50, 250, 20, hwnd, (HMENU)IDC_EDIT, NULL, NULL);
		
		// Кнопка действия
		CreateWindow("BUTTON", "Показать текст",
					 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					 20, 80, 120, 30, hwnd, (HMENU)IDC_BUTTON, NULL, NULL);
		
		// Кнопка очистки
		CreateWindow("BUTTON", "Очистить",
					 WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					 150, 80, 120, 30, hwnd, (HMENU)IDC_BUTTON_CLEAR, NULL, NULL);
		break;
	}
		
		// Сообщение команды
	case WM_COMMAND:
		{
			// Определение элемента, который вызвал команду
			int cmd = LOWORD(wParam);
			
			switch (cmd)
			{
				// Нажата кнопка "Показать текст"
			case IDC_BUTTON:
			{
				char buffer[256];
				// Получаем текст из поля ввода
				GetWindowText(hEdit, buffer, sizeof(buffer));
				
				// Показываем сообщение
				MessageBox(hwnd, buffer, "Введенный текст:", MB_OK | MB_ICONINFORMATION);
				break;
			}
				
				// Кнопка "Очистить"
			case IDC_BUTTON_CLEAR:
				{
					// Очищаем текстовое поле
					SetWindowText(hEdit, "");
					break;
				}
				
				// Пункт меню "Выход"
			case IDM_EXIT:
				DestroyWindow(hwnd);
				break;
				
				// Пункт меню "О программе"
			case IDM_ABOUT:
				{
					MessageBox(hwnd, 
							   "Практическая работа 1\n"
							   "Задание 3: Работа с WinAPI\n"
							   "Простое окно с элементами управления",
							   "О программе",
							   MB_OK | MB_ICONINFORMATION);
					break;
				}
			}
			break;
		}
		
		// Отрисовка
	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);
			
			// Рамка вокруг области
			Rectangle(hdc, 10, 10, 290, 130);
			
			// Текст внизу окна
			TextOut(hdc, 90, 140, "Окно через WinAPI", 17);
			
			EndPaint(hwnd, &ps);
			break;
		}
		
		// Закрытие
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
		
		// Уничтожение
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
		
		// Все остальные сообщения
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
