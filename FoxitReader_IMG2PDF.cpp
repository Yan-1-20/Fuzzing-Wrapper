#include <windows.h>
#include <iostream>
#include <time.h>

struct gg
{
	wchar_t input[260];
	wchar_t output[260];
	wchar_t unknown[10240];
};

typedef int(__stdcall *func1)(int foxit_product, const wchar_t *language);
typedef int(__stdcall *func2)(int foxit_product); 
typedef int(__stdcall *func3)(LPWSTR pPrinterName);
typedef int(__stdcall *func4)(gg* lpFileName, int a3, int a4, int a5, int a6, int a7, int a8, int a9);
typedef int(__stdcall *func5)();
typedef void*(__stdcall *CreateFXPDFConvertor)();

typedef struct vtable
{
	func4 func4;
	func2 func2;
	func3 func3;
	func1 func1;
	void* func_;
	void* func__;
	void* func___;
	func5 func5;
} *Vtable;

wchar_t *GetWC(char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t convertedChars = 0;
	mbstowcs(wc, c, cSize);

	return wc;
}


extern "C" __declspec(dllexport) int fuzz(Vtable *a, gg &xxx);

int fuzz(Vtable *a, gg &xxx)
{
	register Vtable * ecx asm("ecx");
	memset(&xxx, 0, 520);
	wchar_t *infile = GetWC(__argv[1]);
	wcscpy(xxx.input, infile);
	delete infile;
	
	ecx = a;
	(*a)->func4(&xxx, 0, 0, 0, 0, 0, 0, 0);
	
	
	return 0;
}

int main()
{
	HINSTANCE handle = LoadLibrary("ConvertToPDF_x86.dll");
	//HINSTANCE handle = LoadLibrary("C:\\Program Files (x86)\\Foxit Software\\Foxit Reader\\plugins\\Creator\\x86\\ConvertToPDF_x86.dll");

	if (!handle) 
	{
		return 0;
	}

	int t = time(NULL);
	CreateFXPDFConvertor funci = (CreateFXPDFConvertor)GetProcAddress(handle, "CreateFXPDFConvertor");
	
	if (!funci)
	{
		return 0;
	}
	
	void** log_object = (void**)(funci + 0x52ecd8);
	printf("%p %p %x\n", fuzz, main, *log_object);
	
	//*log_object = 0;
	
	register Vtable * ecx asm("ecx");
	Vtable *a = (Vtable*)funci();
	gg xxx;
	wcscpy(xxx.output, L"output.pdf");
	
	ecx = a;
	(*a)->func1(2, L"en-US");
	ecx = a;
	(*a)->func2(2);
	ecx = a;
	(*a)->func3(L"Foxit Reader PDF Printer");
	
	fuzz(a, xxx);
	
	ecx = a;
	int f = (*a)->func5();	
		
	printf("%d\n", time(NULL) - t);
	FreeLibrary(handle);
	return 1;
}
