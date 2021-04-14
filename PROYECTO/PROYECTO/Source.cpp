#include <Windows.h>
#include <fstream>
#include "resource.h"
#include <string.h>
#include <Commctrl.h>
#include <ctime>
#include <time.h>
#include <mmsystem.h> 
#pragma comment(lib, "winmm.lib")
#define ID_5SECONDS 100 
UINT TimmerID1 = 0; 
UINT TimmerID2 = 0;
using namespace std;
time_t actualTime;
struct tm *timeinfo;
bool visto = true;
//MENDOZA ARREDONDO JULIO ANGEL		1635986		GRUPO:04	3ERA OPORTUNIDAD
HINSTANCE hInstance = 0;
HWND hDlg = 0, ghDlg=0;
char DiaHoy[_MAX_PATH] = "";
char MesHoy[_MAX_PATH] = "";
char YYYYHoy[_MAX_PATH] = "";
char HoraHoy[_MAX_PATH] = "";
char MinutosHoy[_MAX_PATH] = "";
//char dias[] = { "07,14,21,18" };
int dias[] = {07,14,21,28};
char VistaMes[_MAX_PATH] = "";
char VistaAnio[_MAX_PATH] = "";
//seleccinoes de dia semanal
int nselec, nselec2, nselec3;
int d5 = 0, d6 = 0, d7 = 0;

char semana[20] = "";
char semanaseleccion[20];
char semanacambio[7][20] = { "Lunes","Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo" };
//termino de dias seleccionados
struct nodos {
	nodos*anterior;
	char usuario[50] = "";
	char Direccion[50] = "";
	char correo[50] = "";
	char telefono[10] = "";
	char casa[8] = "";
	char foto1[_MAX_PATH] = "";
	char foto2[_MAX_PATH] = "";
	char foto3[_MAX_PATH] = "";
	char categoria[2] = "";
	char favorita[_MAX_PATH] = "";
	char sexo[2] = "";
	nodos*siguiente;
};
struct alarmas {
	alarmas*ant;
	alarmas*sig;
	char dia[_MAX_PATH] = "";
	char mes[_MAX_PATH] = "";
	char anio[_MAX_PATH] = "";
	char hora[_MAX_PATH] = "";
	char minutos[_MAX_PATH] = "";
	char asunto[_MAX_PATH] = "";
	char nombre[30] = "";
	char sonido[_MAX_PATH] = "";
	char vista[_MAX_PATH]="";
};
alarmas*inicial=0;
alarmas*AlAuxiliar=0;
alarmas*AlFin = 0;
nodos*inicio = 0;
nodos*fin = 0;
nodos*auxiliar = 0;
BOOL CALLBACK MENUproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ADD_CONTACTOSproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK VISTAproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK ADD_CITASproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EditProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK EditCitaProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK VISTA_SEMANALproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK VISTA_EDBORRARproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK VISTA_MENSUALproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

void Guardado(),Precarga(),Validacion(),Desplegar(HWND hDlg,char*Categoria);
void busqueda(HWND hDlg, bool n, char*nombre);
void Semana(HWND hDlg,int hoy);
int AlarmaVer(HWND hDlg,char*dia,char*mes,char*anio,char*minutos,char*hora);
void CONTACTOS(char *nombre, char*direccion, char*correo, char*tel1, char*casa, char*categoria, char*cfavorita, char*sexo, char*foto1, char*foto2, char*foto3);
void Alarma(char *dia, char*mes, char*anio, char*asunto, char*minuto, char*hora,char*nombre,char*sonido);
void CargaSemanal(HWND hDlg, HWND L, HWND M, HWND MM, HWND J, HWND V, HWND S, HWND D);
char * file(char filtro[3], HWND hDlg, char tag[3], char*punte, int uno);
void EliminarNodos(char* n);
void Perdidas();
void EliminarAlarmas(char* n,char*a);
void FechaMenor(int*op,int*D,int*M,int*Y,int*H,int*mm, int*D2, int*M2, int*Y2, int*H2, int*mm2);
void CARGAGRUPOS(HWND hDlg, HWND H);
void CargarSemana(HWND DL, HWND DM, HWND DMM, HWND DJ, HWND DV, HWND DS, HWND DD, HWND hDlg,char*dia,char*mes,char*anio);
void Mensual(HWND hDlg);
void DiaSemana(char fecha[20]);
void DiasMes();
void VacioMensual();
void aumentardia();

int obteners(int año);
int obtenerm(int mes);
void VaciadoListas(HWND hDlg, int num);
	int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, PSTR cmd, int show) {
		Precarga();
		
		ghDlg = CreateDialog(
			hInstance, // Application instance.
			MAKEINTRESOURCE(IDD_MENU), // Dialog resource ID.
			0, // Parent window--null for no parent.
			MENUproc); // Dialog window procedure.

					   // Show the dialog.
		ShowWindow(ghDlg, show);
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));
		while (GetMessage(&msg, 0, 0, 0))
		{

			if (ghDlg == 0 || !IsDialogMessage(ghDlg, &msg))
			{
				TimmerID1 = SetTimer(ghDlg, ID_5SECONDS, 5000, NULL);
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		return (int)msg.wParam;

}
	BOOL CALLBACK MENUproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) { 
		static HWND HORA = 0, DIA = 0;
		static char buffer[80] = "";
		static char buffer2[_MAX_PATH] = "";
		static char segundos[2] = "";
		char NDiaHoy[_MAX_PATH] = "";
		char NMesHoy[_MAX_PATH] = "";
		char NAnioHoy[_MAX_PATH] = "";
		char NHoraHoy[_MAX_PATH] = "";
		char NMinutosHoy[_MAX_PATH] = "";
		switch (msg) {
		case WM_INITDIALOG:
			HORA = GetDlgItem(hDlg, IDC_DATETIMEPICKERHORAMENU);
			DIA = GetDlgItem(hDlg, IDC_DATETIMEPICKERFECHAMENU);

			return true;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_ADD:
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_CITAS), hDlg, ADD_CITASproc);
				return true;

			case IDC_ADD_CONTACTOS:
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_CONTACTOS), hDlg, ADD_CONTACTOSproc);
				return true;

			

			case IDC_CONTACTOS:
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_VISTA), hDlg, VISTAproc);
				auxiliar = fin;
				return true;

			case IDC_SALIDA:
				Guardado();
				PostQuitMessage(0);
				return true;
			case ID_VISTASEMANAL:
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_VISTA_SEMANAL), hDlg, VISTA_SEMANALproc);
				return true;
			case ID_VISTAMENSUAL:
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_VISTA_MENSUAL), hDlg, VISTA_MENSUALproc);
				return true;
			case ID_CITAS_BORRARCITA: //-BORRAR
				DialogBox(hInstance, MAKEINTRESOURCE(IDD_EDBORRADO), hDlg, VISTA_EDBORRARproc);
					return true;
				case ID_CITAS_MODIFI: //- MODIFICAR
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_EDBORRADO), hDlg, VISTA_EDBORRARproc);
						return true;
			}
			return true;

		case WM_DESTROY:
			PostQuitMessage(0);
			return true;
		case WM_TIMER:
			time_t rawtime; 
			struct tm * timeinfo;
			time(&rawtime); 
			timeinfo = localtime(&rawtime);
			//strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
			GetWindowText(DIA, (LPSTR)buffer, 80);
		/*	DiaHoy[0] = buffer[0];
			DiaHoy[1] = buffer[1];
			MesHoy[0] = buffer[3];
			MesHoy[0] = buffer[4];
			AnioHoy[0] = buffer[6];
			AnioHoy[1] = buffer[7];
			AnioHoy[2] = buffer[8];
			AnioHoy[3] = buffer[9];
			strftime(buffer2, 80, "%I:%M:%S", timeinfo);
			HoraHoy[0] = buffer2[0];
			HoraHoy[1] = buffer2[1];
			MinutosHoy[0] = buffer2[3];
			MinutosHoy[1] = buffer2[4];*/
			strftime(buffer2, 260, "%d", timeinfo);
			strcpy(NDiaHoy, buffer2);
			strcpy(DiaHoy, buffer2);
			
			strftime(buffer2, 260, "%m", timeinfo);
			strcpy(NMesHoy, buffer2);
			strcpy(MesHoy, buffer2);
			strftime(YYYYHoy, 260, "%Y", timeinfo);
		
			strftime(buffer2, 260, "%H", timeinfo);
			strcpy(NHoraHoy, buffer2);
			strcpy(HoraHoy, buffer2);
		
			strftime(buffer2, 260, "%M", timeinfo);
			strcpy(NMinutosHoy, buffer2);
			strcpy(MinutosHoy, buffer2);
			
			strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeinfo);
			AlarmaVer(hDlg, DiaHoy, NMesHoy, YYYYHoy, MinutosHoy, HoraHoy);

			//MessageBox(0,buffer2, "Segundos cada 5", 0);
			if (visto) {
				Perdidas();
				visto = false;
			}
			return true;
		}

		
		return false;
	}
	BOOL CALLBACK ADD_CONTACTOSproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) { 
		bool v[9] = { false };
		static int cursor = 0;
		static char categoria[2] = "";
		static char sexo[2] = "";
		strcpy(sexo, "0");
		static char nombre[50] = "";
		static char numero1[10] = "";
		static char numero2[8] = "";
		static char correo[50] = "";
		static char direccion[50] = "";
		static char favorita[_MAX_PATH] = "";
		static char foto1[_MAX_PATH] = "";
		static char foto2[_MAX_PATH] = "";
		static char foto3[_MAX_PATH] = "";
		
		char ncursor[30] = "";

		static HWND combo = 0;
		switch (msg) {
		case WM_INITDIALOG:
			CheckDlgButton(hDlg, IDC_CHECK_HOMBRE, BST_CHECKED);
			SendDlgItemMessage(hDlg, IDC_STATIC__FOTO1, WM_SETTEXT, 0, LPARAM(""));
			SendDlgItemMessage(hDlg, IDC_STATIC__FOTO2, WM_SETTEXT, 0, LPARAM(""));
			SendDlgItemMessage(hDlg, IDC_STATIC_FOTO3, WM_SETTEXT, 0, LPARAM(""));
			SendDlgItemMessage(hDlg, IDC_STATIC_CANCION, WM_SETTEXT, 0, LPARAM(""));
			combo = GetDlgItem(hDlg, IDC_COMBO1);
			CARGAGRUPOS(hDlg, combo);
			return true;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_COMBO1:
				switch (HIWORD(wParam)) {
				case CBN_SELENDOK:
					//cursor = SendMessage(SCombo,CB_GETCURSEL, 0, 0);
					cursor = SendMessage(combo, CB_GETCURSEL, 0, 0);
					//_itoa_s(cursor, categoria, 1);
					//_itoa_s(cursor, categoria, 10);
					if (cursor == 0)
						strcpy(categoria, "0");
					else
						if(cursor==1)
							strcpy(categoria, "1");
						else
							if(cursor==2)
								strcpy(categoria, "2");
							else
								if(cursor==3)
									strcpy(categoria, "3");
					return true;
				}
				return true;
			case IDC_CHECK_HOMBRE:
				CheckDlgButton(hDlg, IDC_CHECK_MUJER, BST_UNCHECKED);
				strcpy(sexo, "0");
				return true;
			case IDC_CHECK_MUJER:
				CheckDlgButton(hDlg, IDC_CHECK_HOMBRE, BST_UNCHECKED);
				strcpy(sexo, "1");
				return true;
			case IDC_EXAMINAR:
				if (IsDlgButtonChecked(hDlg, IDC_RADIO_FOTO1) == BST_CHECKED)
					file("BMP", hDlg, "FO1",foto1,0);
				else
					if (IsDlgButtonChecked(hDlg, IDC_RADIO_FOTO2) == BST_CHECKED)
						file("BMP", hDlg, "FO2",foto2,0);
					else
						if (IsDlgButtonChecked(hDlg, IDC_RADIO_FOTO3) == BST_CHECKED)
							file("BMP", hDlg, "FO3",foto3,0);
						else
							if (IsDlgButtonChecked(hDlg, IDC_RADIO_MP3) == BST_CHECKED)
								file("MP3", hDlg, "MP3",favorita,0);
							else
								MessageBox(hDlg, "No ha seleccionado un filtro", "AVISO", MB_OK);
				return true;

			case IDC_ACEPTAR:


				SendDlgItemMessage(hDlg, IDC_EDIT1, WM_GETTEXT, 50, LPARAM(nombre));
				if (strcmp(nombre, "") == 0)
					v[0] = false;
				else
					v[0] = true;
				SendDlgItemMessage(hDlg,IDC_EDIT2, WM_GETTEXT, 10, LPARAM(numero1));
				if (strcmp(numero1, "") == 0)
					v[1] = false;
				else
					v[1] = true;
				SendDlgItemMessage(hDlg, IDC_EDIT3, WM_GETTEXT, 8, LPARAM(numero2));
				if (strcmp(numero2, "") == 0)
					v[2] = false;
				else
					v[2] = true;
				SendDlgItemMessage(hDlg, IDC_EDIT4, WM_GETTEXT, 50, LPARAM(correo));
				if (strcmp(correo, "") == 0)
					v[3] = false;
				else
					v[3] = true;
				SendDlgItemMessage(hDlg, IDC_EDIT5, WM_GETTEXT, 50, LPARAM(direccion));
				if (strcmp(direccion, "") == 0)
					v[4] = false;
				else
					v[4] = true;
				//SendDlgItemMessage(hDlg, IDC_STATIC_EDCANCION, WM_GETTEXT, _MAX_PATH, LPARAM(favorita));
				if (strcmp(favorita, "") == 0)
					v[5] = false;
				else
					v[5] = true;
				//SendDlgItemMessage(hDlg, IDC_STATIC__EDFOTO1, WM_GETTEXT, _MAX_PATH, LPARAM(foto1));
				if (strcmp(foto1, "") == 0)
					v[6] = false;
				else
					v[6] = true;
				//SendDlgItemMessage(hDlg, IDC_STATIC__EDFOTO2, WM_GETTEXT, _MAX_PATH, LPARAM(foto2));
				if (strcmp(foto2, "") == 0)
					v[7] = false;
				else
					v[7] = true;
				//SendDlgItemMessage(hDlg, IDC_STATIC_EDFOTO3, WM_GETTEXT, _MAX_PATH, LPARAM(foto3));
				if (strcmp(foto3, "") == 0)
					v[8] = false;
				else
					v[8] = true;


				if (v[0] == true && v[1] == true && v[2] == true && v[3] == true && v[4] == true && v[5] == true
					&& v[6] == true && v[7] == true && v[8] == true) {

					CONTACTOS(nombre, direccion, correo, numero1, numero2, categoria, favorita, sexo, foto1, foto2, foto3);
					MessageBox(hDlg, "Contacto Agregado", "AVISO", MB_OK);
					EndDialog(hDlg, 0);

				}
				else {
					MessageBox(hDlg, "Llena correctamente los campos solicitados", "AVISO", MB_OK);
				}
				return true;

			case IDC_CANCEL:
				EndDialog(hDlg, 0);
				return true;


				return true;
			}
		}
		return false; 
	}
	BOOL CALLBACK VISTAproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) { 
		static HWND CATEGORIA = 0, LISTA = 0;
		static int cursor = 0, contacto = 0, contadorFoto=0;
		static char ListaNombre[50] = "";
		static char categoria[2] = "";
		//static char CategoriaNombre[30] = "", auxcursor[2]="";
		switch (msg) {
		case WM_INITDIALOG:
			static HBITMAP bmp1, bmp2;
			//static HWND combo = GetDlgItem(hDlg, IDC_COMBO2);
			CATEGORIA=GetDlgItem(hDlg,IDC_COMBO2);
			LISTA = GetDlgItem(hDlg, IDC_LIST1);
			CARGAGRUPOS(hDlg, CATEGORIA);
			return true;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_SELECTIONS:
				SendMessage(LISTA, LB_RESETCONTENT, 0, 0);
				//cursor = SendMessage(CATEGORIA, CB_GETCURSEL, 0, 0);
				auxiliar = inicio;
				while (inicio != 0) {
					if (strcmp(auxiliar->categoria,categoria) == 0) {
						SendMessage(LISTA,LB_INSERTSTRING,0,(LPARAM)auxiliar->usuario);
					}
					if (auxiliar == fin)
						break;
					else
						auxiliar = auxiliar->siguiente;
				}
				return true;
			case IDC_COMBO2:
				switch (HIWORD(wParam)) {
				case CBN_SELENDOK:
					SendMessage(LISTA, LB_RESETCONTENT, 0, 0);
					strcpy(ListaNombre, "");
					cursor = SendMessage(CATEGORIA, CB_GETCURSEL, 0, 0);//cursor de grupo
					//SendMessage(CATEGORIA, WM_GETTEXT, (WPARAM)cursor, LPARAM(categoria));
					//_itoa_s(cursor,auxcursor, 10);
					if (cursor == 0)
						strcpy(categoria, "0");
					else
						if (cursor == 1)
							strcpy(categoria, "1");
						else
							if (cursor == 2)
								strcpy(categoria, "2");
							else
								if (cursor == 3)
									strcpy(categoria, "3");
					
					//SendMessage(LISTA, LB_ADDSTRING, 1, LPARAM(ListaNombre));
					return true;
				}

				return true;

			case IDC_BUTTON1:
				if (contadorFoto == 5)
					contadorFoto = 0;
				
				if (contadorFoto <1/*||contadorFoto==1*/)
					contadorFoto = 3;
				else
					contadorFoto--;

				if (strcmp(ListaNombre, "") == 0) {
					MessageBox(hDlg, "No ha seleccionado un contacto", "Aviso", MB_OK);
				}
				else {
					if (contadorFoto == 3) {
						bmp1 = (HBITMAP)SendDlgItemMessage(hDlg, IDC_FOTO1, STM_GETIMAGE, IMAGE_BITMAP, 0);
						bmp2 = (HBITMAP)LoadImage(NULL, auxiliar->foto3, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
						SendDlgItemMessage(hDlg, IDC_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
						//contadorFoto = 2;
					}
					
						if (contadorFoto == 2) {
							bmp1 = (HBITMAP)SendDlgItemMessage(hDlg, IDC_FOTO1, STM_GETIMAGE, IMAGE_BITMAP, 0);
							bmp2 = (HBITMAP)LoadImage(NULL, auxiliar->foto2, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
							SendDlgItemMessage(hDlg, IDC_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
							//contadorFoto = 1;
						}
						
							if (contadorFoto == 1) {
								bmp1 = (HBITMAP)SendDlgItemMessage(hDlg, IDC_FOTO1, STM_GETIMAGE, IMAGE_BITMAP, 0);
								bmp2 = (HBITMAP)LoadImage(NULL, auxiliar->foto1, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
								SendDlgItemMessage(hDlg, IDC_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
								contadorFoto = 0;
							}

						
					
				}
					return true;

			case IDC_BUTTON2:
				if (contadorFoto == 5)
					contadorFoto = 1;

				if (contadorFoto > 2)
					contadorFoto = 1;
				else
					contadorFoto++;
				//
				
				if (strcmp(ListaNombre, "") == 0) {
					MessageBox(hDlg, "No ha seleccionado un contacto", "Aviso", MB_OK);
				}
				else {
					if (contadorFoto == 3) {
						bmp1 = (HBITMAP)SendDlgItemMessage(hDlg, IDC_FOTO1, STM_GETIMAGE, IMAGE_BITMAP, 0);
						bmp2 = (HBITMAP)LoadImage(NULL, auxiliar->foto3, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
						SendDlgItemMessage(hDlg, IDC_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
						//contadorFoto = 2;
					}

					if (contadorFoto == 2) {
						bmp1 = (HBITMAP)SendDlgItemMessage(hDlg, IDC_FOTO1, STM_GETIMAGE, IMAGE_BITMAP, 0);
						bmp2 = (HBITMAP)LoadImage(NULL, auxiliar->foto2, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
						SendDlgItemMessage(hDlg, IDC_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
						//contadorFoto = 1;
					}

					if (contadorFoto == 1) {
						bmp1 = (HBITMAP)SendDlgItemMessage(hDlg, IDC_FOTO1, STM_GETIMAGE, IMAGE_BITMAP, 0);
						bmp2 = (HBITMAP)LoadImage(NULL, auxiliar->foto1, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
						SendDlgItemMessage(hDlg, IDC_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
						//contadorFoto = 0;
					}



				}


					//siguiente
					return true;
			case IDC_LIST1:
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
					contacto = SendMessage(LISTA, LB_GETCURSEL, 0, 0); //cursor de lista
					SendMessage(LISTA, LB_GETTEXT, (WPARAM)contacto, LPARAM(ListaNombre));
					/*bmp2 = (HBITMAP)LoadImage(NULL, NULL, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);*/
					//
					
					//
					auxiliar = inicio;
					while (inicio != 0) {
						if (strcmp(auxiliar->usuario, ListaNombre) == 0) {
							break;
						}
						if (auxiliar == fin)
							break;
						else
							auxiliar = auxiliar->siguiente;
					}

					//foto aqui al clic NEL
					bmp1 = (HBITMAP)SendDlgItemMessage(hDlg, IDC_FOTO1, STM_GETIMAGE, IMAGE_BITMAP, 0);
					bmp2 = (HBITMAP)LoadImage(NULL, auxiliar->foto1, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					SendDlgItemMessage(hDlg, IDC_FOTO1, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp2);
					contadorFoto=5;
					
					//
					return true;
				}

				return true;

			case IDC_VISTA_SALIR:
				strcpy(ListaNombre, "");
				cursor = 0;
				contacto = 0;
				EndDialog(hDlg, 0);
				contacto = 0;
				
				return true;
			case IDC_EDCONTACTO:
				if (strcmp(ListaNombre,"") == 0) {
					MessageBox(hDlg, "No ha sido seleccionado ningun nombre","AVISO",MB_OK);
				}
				else {
					DialogBox(hInstance, MAKEINTRESOURCE(IDD_EDITAR), hDlg, EditProc);
					strcpy(ListaNombre, "");
					cursor = 0;
					contacto = 0;
					auxiliar = fin;
					SendMessage(LISTA, LB_RESETCONTENT, 0, 0);
					//EndDialog(hDlg, 0);
				}
				return true;
			}
			return true;
		}
		return false; 
	}
	BOOL CALLBACK ADD_CITASproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) { 
		 static HWND lista = 0,combo = 0,hDatePicker = 0,hLabelDate = 0;
		static int cursor = 0;
		static char Nombre[30] = "";
		static char categoria[2] = "";
		static char buffer[80]="";
		static char buffer2[_MAX_PATH] = "";
		static char dia[_MAX_PATH] = "";
		static char mes[_MAX_PATH] = "";
		static char aanio[_MAX_PATH] = "";
		static char hora[_MAX_PATH] = "";
		static char minutos[_MAX_PATH] = "";
		static char asunto[_MAX_PATH] = "";
		static char cancion[_MAX_PATH] = "";
		static int categoria2 = 0;
		bool REPRODUCCION = true;
		char REP[_MAX_PATH] = "";
		char*p1 = 0, *p2 = 0, *p3 = 0, *p4 = 0, *p5 = 0, *p6 = 0, *p7 = 0, *p8 = 0;
		int fecha[5] = { 0 };
		int op = 0;
		int hhoy[5] = { 0 };
		//string Day="", Month="", Year="";
		switch (msg) {
		case WM_INITDIALOG:
			lista = GetDlgItem(hDlg, IDC_LIST2);
			hDatePicker = GetDlgItem(hDlg, IDC_DATETIMEPICKER11);
			hLabelDate = GetDlgItem(hDlg, IDC_DATETIMEPICKER2);
			CheckDlgButton(hDlg, IDC_RADIO4, BST_CHECKED);
			combo = GetDlgItem(hDlg, IDC_COMBO4);
			CARGAGRUPOS(hDlg,combo);
			strcpy(Nombre, "");
			strcpy(cancion, "");
			return true;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_LIST2:
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
					cursor = SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETCURSEL, 0, 0);
					SendDlgItemMessage(hDlg, IDC_LIST2, LB_GETTEXT, (WPARAM)cursor, (LPARAM)Nombre);
					return true;
				}
			case IDC_COMBO4:
				switch (HIWORD(wParam)) {
				case CBN_SELENDOK:
					categoria2 = SendMessage(combo, CB_GETCURSEL, 0, 0);
					if (categoria2 == 0)
						strcpy(categoria, "0");
					else
						if (categoria2 == 1)
							strcpy(categoria, "1");
						else
							if (categoria2 == 2)
								strcpy(categoria, "2");
							else
								if (categoria2 == 3)
									strcpy(categoria, "3");
					auxiliar = inicio;
					if (inicio != 0) {
						auxiliar = inicio;
						SendDlgItemMessage(hDlg, IDC_LIST2, LB_RESETCONTENT, 0, 0);
						while (inicio != 0) {
							if (strcmp(auxiliar->categoria, categoria) == 0)
								SendDlgItemMessage(hDlg, IDC_LIST2, LB_ADDSTRING, 0, (LPARAM)auxiliar->usuario);//CAMBIO
							if (auxiliar == fin)
								break;
							else
								auxiliar = auxiliar->siguiente;
						}

					}
					return true;
				}
				return true;

			case IDC_ALARMA:
				if (strcmp(Nombre, "") == 0) {
					MessageBox(hDlg, "No haz seleccionado un usuario en la lista par la cita", "AVISO", MB_OK);

				}
				else {
					GetWindowText(hDatePicker, (LPSTR)buffer, 80); //<------ AQUI
					//str

					//fecha


					dia[0] = buffer[0];
					dia[1] = buffer[1];
					mes[0] = buffer[3];
					mes[1] = buffer[4];
					aanio[0] = buffer[6];
					aanio[1] = buffer[7];
					aanio[2] = buffer[8];
					aanio[3] = buffer[9];
					if (strcmp(aanio, "2017") == 0) {
						strcpy(aanio, "2017");
					}
					//se corrompe
				   // hora
				   //strcat(Nombre, buffer);
					GetWindowText(hLabelDate, (LPSTR)buffer2, _MAX_PATH); //<------ AQUI
					hora[0] = buffer2[0];
					hora[1] = buffer2[1];
					minutos[0] = buffer2[3];
					minutos[1] = buffer2[4];

					hhoy[0] = atoi(DiaHoy);
					//mes
					hhoy[1] = atoi(MesHoy);
					//anio
					hhoy[2] = atoi(YYYYHoy);
					//horas
					hhoy[3] = atoi(HoraHoy);
					//MINUTOS
					hhoy[4] = atoi(MinutosHoy);

					
					fecha[0] = atoi(dia);
					//mes ALARMA

					fecha[1] = atoi(mes);
					//anio ALARMA

					fecha[2] = atoi(aanio);
					//FECHA
					//HORAS ALARMA

					fecha[3] = atoi(hora);
					//MINUTOS ALARMA

					fecha[4] = atoi(minutos);


					//alarmas
					FechaMenor(&op, &fecha[0], &fecha[1], &fecha[2], &fecha[3], &fecha[4], &hhoy[0], &hhoy[1], &hhoy[2], &hhoy[3], &hhoy[4]);

										//ararmas
					if (op==0){
						if (inicio != 0) {
							while (inicio != 0) {
								if (strcmp(auxiliar->usuario, Nombre) == 0) {
									break;
								}
								if (auxiliar == fin) {
									break;
								}
								else {
									auxiliar = auxiliar->siguiente;
								}
							}
						}
					SendDlgItemMessage(hDlg, IDC_EDIT_ASUNTO12, WM_GETTEXT, _MAX_PATH, LPARAM(asunto));
					if (strcmp(asunto, "") == 0) {
						MessageBox(hDlg, "ASUNTO VACIO", "Aviso", MB_OK);
					}
					else {
						if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
							strcpy(cancion, "C:\\temp\\SIRENA.MP3");
						else
							if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
								strcpy(cancion, "C:\\temp\\BEEP.MP3");
							else
								if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
									strcpy(cancion, "C:\\temp\\NATURALEZA.MP3");
								else
									if (IsDlgButtonChecked(hDlg, IDC_RADIO4) == BST_CHECKED)
										strcpy(cancion, auxiliar->favorita);

						p1 = dia;
						p2 = mes;
						p3 = aanio;
						p4 = asunto;
						p5 = minutos;
						p6 = hora;
						p7 = Nombre;
						p8 = cancion;
						Alarma(p1, p2, p3, p4, p5, p6, p7, p8);
					}

					MessageBox(hDlg, Nombre, "Nueva cita", MB_OK);
					}
					else {
						MessageBox(hDlg, "La Fecha y/o hora introducidas ya pasaron", "AVISO", MB_OK);
					}
			}
					//Alarma()
				return true;
				case IDC_RADIO1:
					//mciSendString("close alarma", NULL, 0, 0);
					if (REPRODUCCION)
						mciSendString("close alarma", NULL, 0, 0);

					strcpy(cancion, "C:\\temp\\SIRENA.MP3");
					
					return true;
				case IDC_RADIO2:
					if (REPRODUCCION)
						mciSendString("close alarma", NULL, 0, 0);

					strcpy(cancion, "C:\\temp\\BEEP.MP3");
					return true;
				case IDC_RADIO3:
					
					if(REPRODUCCION)
						mciSendString("close alarma", NULL, 0, 0);
					strcpy(cancion, "C:\\temp\\NATURALEZA.MP3");
					return true;
				case IDC_RADIO4:
					
					//mciSendString("close alarma", NULL, 0, 0);
					if (inicio != 0) {
						SendMessage(lista, LB_GETTEXT, WPARAM(cursor), LPARAM(Nombre));
						auxiliar = inicio;
						while (inicio != 0) {
							if (strcmp(auxiliar->usuario, Nombre) == 0)
								break;

							if (auxiliar == fin)
								break;
							else
								auxiliar = auxiliar->siguiente;
						}

					}
					if (REPRODUCCION)
						mciSendString("close alarma", NULL, 0, 0);

					strcpy(cancion, auxiliar->favorita);
					return true;

		case IDC_BUTTON2://STOP
			mciSendString("close alarma", NULL, 0, 0);
			return true;
		case IDC_BUTTON6: //PLAY
			strcat(REP, "open "); //agrega open
			strcat(REP, cancion); //agrega direccion
			strcat(REP, " alias alarma");
			mciSendString(REP, NULL, 0, 0);
			mciSendString("play alarma", NULL, 0, 0);
			REPRODUCCION = true;

			if (strcmp(Nombre, "") == 0) {
				MessageBox(hDlg, "No ha seleccionado un nombre de contacto", "AVISO", 0);
			}
			else {
				if (inicio != 0) {
					SendMessage(lista, LB_GETTEXT, WPARAM(cursor), LPARAM(Nombre));
					auxiliar = inicio;
					while (inicio != 0) {
						if (strcmp(auxiliar->usuario, Nombre) == 0)
							break;

						if (auxiliar == fin)
							break;
						else
							auxiliar = auxiliar->siguiente;
					}

				}
				strcpy(REP, "");
				strcat(REP, "open "); //agrega open
				strcat(REP, auxiliar->favorita); //agrega direccion
				strcat(REP, " alias alarma");
				mciSendString(REP, NULL, 0, 0);
				mciSendString("play alarma", NULL, 0, 0);
			}
			
			return true;
		case IDC_BUTTON7://PAUSE
			mciSendString("pause alarma", NULL, 0, 0);
			return true;
			case IDC_CANCELED:
				if (REPRODUCCION)
					mciSendString("close alarma", NULL, 0, 0);
				auxiliar = fin;
				EndDialog(hDlg, 0);
				return true;

			}
			return true;

		}
		return false;
	}
	BOOL CALLBACK EditProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) { 
		char SeleccionNombre[50] = ""; static int cursor = 0;
		static char Nombre[50] = "", numero[8] = "", numero2[10] = "",Dir[50]="",correo[50]="",categoria[2]="";
		static  char favorita[_MAX_PATH] = "", foto1[_MAX_PATH] = "", foto2[_MAX_PATH] = "", foto3[_MAX_PATH] = "";
		strcpy(SeleccionNombre, auxiliar->usuario);
		static char sexo[2] ="";
		static HWND COMBO = 0;
	
		/*char *foto1=0/*, *foto2, *foto3, *favorita*/;
		switch (msg) {
		case WM_INITDIALOG:
			COMBO = GetDlgItem(hDlg, IDC_COMBO5);
			CARGAGRUPOS(hDlg, COMBO);
			
			SendDlgItemMessage(hDlg, IDC_EDIT_CONTACTO, WM_SETTEXT, 0, LPARAM(auxiliar->usuario));
			SendDlgItemMessage(hDlg, IDC_EDIT_MOVIL, WM_SETTEXT, 0, LPARAM(auxiliar->telefono));
			SendDlgItemMessage(hDlg, IDC_EDIT_FIJO, WM_SETTEXT, 0, LPARAM(auxiliar->casa));
			SendDlgItemMessage(hDlg, IDC_EDIT__CORREO, WM_SETTEXT, 0, LPARAM(auxiliar->correo));
			SendDlgItemMessage(hDlg, IDC_EDIT_DIR, WM_SETTEXT, 0, LPARAM(auxiliar->Direccion));

			if (strcmp(auxiliar->sexo, "0") == 0) {
				CheckDlgButton(hDlg, IDC_CHECK_HOMBRE1, BST_CHECKED);
				strcpy(sexo, "0");
			}
			else {
				CheckDlgButton(hDlg, IDC_CHECK_MUJER1, BST_CHECKED);
				strcpy(sexo, "1");
			}
			SendDlgItemMessage(hDlg, IDC_STATIC__EDFOTO1, WM_SETTEXT, 0, LPARAM(auxiliar->foto1));
			SendDlgItemMessage(hDlg, IDC_STATIC__EDFOTO2, WM_SETTEXT, 0, LPARAM(auxiliar->foto2));
			SendDlgItemMessage(hDlg, IDC_STATIC_EDFOTO3, WM_SETTEXT, 0, LPARAM(auxiliar->foto3));
			SendDlgItemMessage(hDlg, IDC_STATIC_EDCANCION, WM_SETTEXT, 0, LPARAM(auxiliar->favorita));
			
			if(strcmp(auxiliar->categoria,"0")==0)
			SendDlgItemMessage(hDlg, IDC_COMBO5, CB_SETCURSEL, 0, 0);
			else
				if (strcmp(auxiliar->categoria, "1") == 0)
					SendDlgItemMessage(hDlg, IDC_COMBO5, CB_SETCURSEL, 1, 0);
				else
					if (strcmp(auxiliar->categoria, "2") == 0)
						SendDlgItemMessage(hDlg, IDC_COMBO5, CB_SETCURSEL, 2, 0);
					else
						if (strcmp(auxiliar->categoria, "3") == 0)
							SendDlgItemMessage(hDlg, IDC_COMBO5, CB_SETCURSEL, 3, 0);
			strcpy(categoria, auxiliar->categoria); //copiar valor inicializado


			
			//busqueda(hDlg, 0, SeleccionNombre);
			return true;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_COMBO5:
				switch (HIWORD(wParam)) {
				case CBN_SELENDOK:
					cursor = SendDlgItemMessage(hDlg,IDC_COMBO5, CB_GETCURSEL, 0, 0);
					SendDlgItemMessage(hDlg, IDC_COMBO5, CB_GETLBTEXT, 0, LPARAM(SeleccionNombre));
					if (cursor == 0)
						strcpy(categoria,"0");
						else
							if(cursor==1)
								strcpy(categoria,"1");
							else
								if (cursor == 2)
									strcpy(categoria,"2");
								else
									if (cursor == 3)
										strcpy(categoria,"3");
					return true;
				}
				return true;

			case IDC_ED_BUSCADOR:
				if (IsDlgButtonChecked(hDlg, IDC_RADIO_FOTO11) == BST_CHECKED)
					file("BMP", hDlg, "FO1", foto1,1);
				else
					if (IsDlgButtonChecked(hDlg, IDC_RADIO_FOTO22) == BST_CHECKED)
						file("BMP", hDlg, "FO2", foto2,1);
					else
						if (IsDlgButtonChecked(hDlg, IDC_RADIO_FOTO33) == BST_CHECKED)
							file("BMP", hDlg, "FO3", foto3,1);
						else
							if (IsDlgButtonChecked(hDlg, IDC_RADIO_MP33) == BST_CHECKED)
								file("MP3", hDlg, "MP3", favorita,1);
							else
								MessageBox(hDlg, "No ha seleccionado un filtro", "AVISO", MB_OK);
				return true;

			case IDC_CHECK_HOMBRE1:
				CheckDlgButton(hDlg, IDC_CHECK_MUJER1, BST_UNCHECKED);
				strcpy(sexo, "0");
				return true;

			case IDC_CHECK_MUJER1:
				CheckDlgButton(hDlg, IDC_CHECK_HOMBRE1, BST_UNCHECKED);
				strcpy(sexo, "1");
				return true;

			case IDC_EDIT_CANCEL:
				EndDialog(hDlg, 0);
				auxiliar = fin;
				return true;

			case IDC_EDBORRAR:
				EliminarNodos(auxiliar->usuario);
				auxiliar = fin;
				EndDialog(hDlg, 0);
				
				//funcion borrar
				return true;

			case IDC_ED_EDITAR:
				
				SendDlgItemMessage(hDlg, IDC_EDIT_CONTACTO, WM_GETTEXT, 50, LPARAM(Nombre));
				SendDlgItemMessage(hDlg, IDC_EDIT_MOVIL, WM_GETTEXT, 10, LPARAM(numero2));
				SendDlgItemMessage(hDlg, IDC_EDIT_FIJO, WM_GETTEXT, 8, LPARAM(numero));
				SendDlgItemMessage(hDlg, IDC_EDIT_DIR, WM_GETTEXT, 50, LPARAM(Dir));
				SendDlgItemMessage(hDlg, IDC_EDIT__CORREO, WM_GETTEXT, 50, LPARAM(correo));
				SendDlgItemMessage(hDlg, IDC_STATIC__EDFOTO1, WM_GETTEXT, _MAX_PATH, LPARAM(foto1));
				SendDlgItemMessage(hDlg, IDC_STATIC__EDFOTO2, WM_GETTEXT, _MAX_PATH, LPARAM(foto2));
				SendDlgItemMessage(hDlg, IDC_STATIC_EDFOTO3, WM_GETTEXT, _MAX_PATH, LPARAM(foto3));
				SendDlgItemMessage(hDlg, IDC_STATIC_EDCANCION, WM_GETTEXT, _MAX_PATH, LPARAM(favorita));
				if (strlen(Nombre) >0 && strlen(numero2) >0 && strlen(numero) >0 && strlen(correo) >0
					&& strlen(Dir) >0 && strlen(foto1) >0 && strlen(foto2) >0
					&& strlen(foto3) >0 && strlen(favorita) >0) {
					strcpy(auxiliar->usuario, Nombre);
					strcpy(auxiliar->telefono,numero2);
					strcpy(auxiliar->casa, numero);
					strcpy(auxiliar->correo,correo);
					strcpy(auxiliar->foto1,foto1);
					strcpy(auxiliar->foto2,foto2);
					strcpy(auxiliar->foto3,foto3);
					strcpy(auxiliar->favorita,favorita);
					strcpy(auxiliar->Direccion,Dir);
					strcpy(auxiliar->sexo,sexo);
					strcpy(auxiliar->categoria,categoria);
					MessageBox(hDlg, "bien", "AVISO", MB_OK);
					auxiliar = fin;
					EndDialog(hDlg, 0);
				}
				else {
					MessageBox(hDlg, "Falta de llenar campos", "AVISO", MB_OK);
				}

	/*
		static char Nombre[50] = "", numero[8] = "", numero2[10] = "",Dir[50]="",correo[50]="",categoria[2]="";
		static  char favorita[_MAX_PATH] = "", foto1[_MAX_PATH] = "", foto2[_MAX_PATH] = "", foto3[_MAX_PATH] = "";
	*/
													
	//devolver auxiliar a su lugar

				return true;
			}

			return true;
		}
		return false; 
	}
	BOOL CALLBACK EditCitaProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) { 
		static HWND DATE1=0, DATE2 = 0, LISTA=0,COMBO=0;
		static int cursor = 0, CLISTA = 0, fecha[5] = { 0 }, hhoy[5] = { 0 };
		static char buffer[_MAX_PATH] = "";
		static char motivo[_MAX_PATH] = "";
		static char cancion[_MAX_PATH] = "";
		static char FAV[_MAX_PATH] = "";
		static char USUARIO[50] = "";
		static char aux[2] = "";
		static char aux2[2] = "";
		static char aux3[4] = "";
		static char buffer2[_MAX_PATH] = ""; //hoy fecha
		static char buffer3[_MAX_PATH] = ""; // hoy hora
		static char buffer4[_MAX_PATH] = ""; // hoy hora
		static char verdadero[_MAX_PATH] = ""; // hoy hora
		static char dia[_MAX_PATH]="", mes[_MAX_PATH] = "", aanio[_MAX_PATH] = "", hora[_MAX_PATH] = "", minutos[_MAX_PATH] = "";
		int verdad = 0;
		int *op;
	/*	DATE1 = GetDlgItem(hDlg, IDC_DATETIMEPICKER1EDC);
		DATE2 = GetDlgItem(hDlg, IDC_DATETIMEPICKER2EDC);*/
	
		//strcpy(FAV, AlAuxiliar->sonido);
		switch (msg) {
		case WM_INITDIALOG:
			DATE1 = GetDlgItem(hDlg,IDC_DATETIMEPICKER1EDC);
			DATE2 = GetDlgItem(hDlg, IDC_DATETIMEPICKER2EDC);
			LISTA = GetDlgItem(hDlg, IDC_LIST1_CITAED);
			COMBO = GetDlgItem(hDlg, IDC_COMBOED);
			CARGAGRUPOS(hDlg, COMBO);

			SendDlgItemMessage(hDlg, IDC_EDIT_DIA, WM_SETTEXT, _MAX_PATH, LPARAM(""));
			SendDlgItemMessage(hDlg, IDC_EDIT_MES, WM_SETTEXT, _MAX_PATH, LPARAM(""));
			SendDlgItemMessage(hDlg, IDC_EDIT_ANIO, WM_SETTEXT, _MAX_PATH, LPARAM(""));
			SendDlgItemMessage(hDlg, IDC_EDIT_HORA, WM_SETTEXT, _MAX_PATH, LPARAM(""));
			SendDlgItemMessage(hDlg, IDC_EDIT_MINUTOS, WM_SETTEXT, _MAX_PATH, LPARAM(""));

			SendDlgItemMessage(hDlg, IDC_EDIT1_CITAMOT,WM_SETTEXT,_MAX_PATH,LPARAM(AlAuxiliar->asunto));

			SendDlgItemMessage(hDlg, IDC_EDIT_DIA, WM_SETTEXT, _MAX_PATH, LPARAM(AlAuxiliar->dia));
			SendDlgItemMessage(hDlg, IDC_EDIT_MES, WM_SETTEXT, _MAX_PATH, LPARAM(AlAuxiliar->mes));
			SendDlgItemMessage(hDlg, IDC_EDIT_ANIO, WM_SETTEXT, _MAX_PATH, LPARAM(AlAuxiliar->anio));
			SendDlgItemMessage(hDlg, IDC_EDIT_HORA, WM_SETTEXT, _MAX_PATH, LPARAM(AlAuxiliar->hora));
			SendDlgItemMessage(hDlg, IDC_EDIT_MINUTOS, WM_SETTEXT, _MAX_PATH, LPARAM(AlAuxiliar->minutos));
			GetWindowText(DATE1, (LPSTR)buffer2, _MAX_PATH);
			GetWindowText(DATE2, (LPSTR)buffer3, _MAX_PATH);

			strcat(buffer, AlAuxiliar->dia);
			strcat(buffer, "/");
			strcat(buffer, AlAuxiliar->mes);
			strcat(buffer, "/");
			strcat(buffer, AlAuxiliar->anio);
			
			
			strcpy(buffer4, "");
			strcat(buffer4, AlAuxiliar->hora);
			strcat(buffer4, ":");
			strcat(buffer4, AlAuxiliar->minutos);
			strcat(buffer4, ":00");

			//DATOSPICKER(DATE1,DATE2,buffer,buffer4);
		
			
			if (strcmp(AlAuxiliar->sonido, "C:\\temp\\SIRENA.MP3") == 0) {
				//CheckDlgButton(hDlg,);
				CheckDlgButton(hDlg, IDC_RADIO1, BST_CHECKED);
				
			}
			else {
				if (strcmp(AlAuxiliar->sonido, "C:\\temp\\BEEP.MP3")==0) {
					CheckDlgButton(hDlg, IDC_RADIO2, BST_CHECKED);
					
				}
				else {
					if (strcmp(AlAuxiliar->sonido, "C:\\temp\\NATURALEZA.MP3") == 0) {
						CheckDlgButton(hDlg, IDC_RADIO3, BST_CHECKED);
					
					
					}
					else {
						CheckDlgButton(hDlg, IDC_RADIO4, BST_CHECKED);
						
					
					}
				
				}
			
			}
			
			return true;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_COMBOED:
				switch (HIWORD(wParam)) {
				case CBN_SELENDOK:
					SendMessage(LISTA, LB_RESETCONTENT, 0, 0);
					cursor = SendMessage(COMBO, CB_GETCURSEL, 0, 0);
					if (cursor == 0)
						strcpy(aux, "0");
					else
						if (cursor == 1)
							strcpy(aux, "1");
						else
							if (cursor == 2)
								strcpy(aux, "2");
							else
								if (cursor == 3)
									strcpy(aux, "3");
					auxiliar = inicio;
					while (inicio != 0) {
						if (strcmp(auxiliar->categoria, aux) == 0)
							SendMessage(LISTA, LB_ADDSTRING, -1, LPARAM(auxiliar->usuario));

						if (auxiliar == fin)
							break;
						else
							auxiliar = auxiliar->siguiente;
					}

					return true;
				}
				return true;

			case IDC_LIST1_CITAED:
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
					CLISTA = SendMessage(LISTA, LB_GETCURSEL, 0, 0);
					SendMessage(LISTA, LB_GETTEXT, (WPARAM)CLISTA, (LPARAM)USUARIO);
					return true;
				}

				return true;

			case IDC_EDITARCITA:
				SendDlgItemMessage(hDlg, IDC_EDIT_DIA, WM_GETTEXT, _MAX_PATH, LPARAM(dia));
				SendDlgItemMessage(hDlg, IDC_EDIT_MES, WM_GETTEXT, _MAX_PATH, LPARAM(mes));
				SendDlgItemMessage(hDlg, IDC_EDIT_ANIO, WM_GETTEXT, _MAX_PATH, LPARAM(aanio));
				SendDlgItemMessage(hDlg, IDC_EDIT_HORA, WM_GETTEXT, _MAX_PATH, LPARAM(hora));
				SendDlgItemMessage(hDlg, IDC_EDIT_MINUTOS, WM_GETTEXT, _MAX_PATH, LPARAM(minutos));
				if (strcmp(dia, "") == 0 || strcmp(mes, "") == 0 || strcmp(aanio, "") == 0 || strcmp(hora, "") == 0 || strcmp(minutos, "") == 0)
					MessageBox(hDlg, "Hay campos vacios", "Aviso", 0);
				else
					if (strlen(dia) != 2 || strlen(mes) != 2 || strlen(aanio) != 4 || strlen(hora) != 2 || strlen(minutos) != 2)
						MessageBox(hDlg, "Hay campos con mas de 2 carecteres en Fecha y/o Hora", "Aviso", 0);
					else {
						fecha[0] = atoi(dia);
						//mes ALARMA

						fecha[1] = atoi(mes);
						//anio ALARMA

						fecha[2] = atoi(aanio);

						//FECHA
						//HORAS ALARMA

						fecha[3] = atoi(hora);
						//MINUTOS ALARMA

						fecha[4] = atoi(minutos);
						//cambie &&
						if (fecha[0] == 0 || fecha[0] > 31 || fecha[1] == 0 || fecha[1] > 12 || fecha[2] == 0 || fecha[2] > 2099 || fecha[3] > 23 || fecha[4] > 59 || (fecha[1] == 2 && (fecha[0] >= 30|| fecha[0]==0))) {
						MessageBox(hDlg, "Formato incorrecto en carecteres en Fecha y/o Hora", "Aviso", 0);

						//cierre bueno
					}
						else {

						if (strcmp(USUARIO, "") == 0) {
							MessageBox(hDlg, "No haz seleccionado un contacto", "AVISO", MB_OK);
						}
						else {
							SendDlgItemMessage(hDlg, IDC_EDIT1_CITAMOT, WM_GETTEXT, _MAX_PATH, LPARAM(motivo));
							if (strcmp(motivo, "") == 0) {
								MessageBox(hDlg, "Falta llenar ASUNTO de CITA", "AVISO", MB_OK);
							}
							else {
								if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
									strcpy(cancion, "C:\\temp\\SIRENA.MP3");
								else
									if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
										strcpy(cancion, "C:\\temp\\BEEP.MP3");
									else
										if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
											strcpy(cancion, "C:\\temp\\NATURALEZA.MP3");
										else {
											auxiliar = inicio;
											while (inicio != 0) {
												//SendMessage(LISTA, LB_ADDSTRING, -1, LPARAM(auxiliar->usuario));
												if (strcmp(auxiliar->usuario, USUARIO) == 0) {
													strcpy(cancion, auxiliar->favorita);
													break;
												}
												if (auxiliar == fin)
													break;
												else
													auxiliar = auxiliar->siguiente;
											}

										}
										//fecha[0] = atoi(buffer);

										//dia ALARMA

										//fecha[0] = atoi(dia);
										////mes ALARMA

										//fecha[1] = atoi(mes);
										////anio ALARMA

										//fecha[2] = atoi(aanio);

										////FECHA
										////HORAS ALARMA

										//fecha[3] = atoi(hora);
										////MINUTOS ALARMA

										//fecha[4] = atoi(minutos);


										//hoy

										hhoy[0] = atoi(DiaHoy);
										//mes

										hhoy[1] = atoi(MesHoy);
										//anio


										hhoy[2] = atoi(YYYYHoy);
										//horas

										hhoy[3] = atoi(HoraHoy);
										//MINUTOS

										hhoy[4] = atoi(MinutosHoy);



										op = &verdad;
										FechaMenor(op, &fecha[0], &fecha[1], &fecha[2], &fecha[3], &fecha[4], &hhoy[0], &hhoy[1], &hhoy[2], &hhoy[3], &hhoy[4]);
										//AQUI ME QUEDE

										if (verdad == 0) {
											GetWindowText(DATE1, (LPSTR)buffer, _MAX_PATH);

									
									
											strcpy(AlAuxiliar->nombre, USUARIO);
											strcpy(AlAuxiliar->asunto, motivo);
											strcpy(AlAuxiliar->sonido, cancion);
											strcpy(AlAuxiliar->vista, "0");
											strcpy(AlAuxiliar->dia, dia);
											strcpy(AlAuxiliar->mes, mes);
											strcpy(AlAuxiliar->anio, aanio);
											strcpy(AlAuxiliar->hora, hora);
											strcpy(AlAuxiliar->minutos, minutos);
											MessageBox(hDlg, "CORRECTO", "AVISO", 0);
											auxiliar = fin;
											AlAuxiliar = AlFin;
											EndDialog(hDlg, 0);

										}
										else {
											MessageBox(hDlg, "La fecha seleccionada y hora ya pasaron", "AVISO", MB_OK);
										}
							}
						}

					}
			}
				return true;

			case IDC_CANCELCITA:
				EndDialog(hDlg, 0);
				return true;
			}

			return true;
		}
		return false; 
	}
	BOOL CALLBACK VISTA_SEMANALproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
		static HWND LUNES = 0, MARTES = 0, MIERCOLES = 0, JUEVES = 0, VIERNES = 0, SABADO = 0, DOMINGO = 0;
		static HWND FECHA = 0;
		static char buffer[80] = "";
		static char Selbufer2[20] = "";
		static int hoy = 0;
		static char nDiaHoy[2] = "";
		static char nMesHoy[2] = "";
		static char nAnio[4] = "";
		string fecha1;
		string Dia1;
		string Año1;
		string Mes1;
		//int hoy;
		switch (msg) {
			
		case WM_INITDIALOG: {
			FECHA = GetDlgItem(hDlg, IDC_DATETIMEPICKER3);
			GetWindowText(FECHA, (LPSTR)buffer, 80);
			nDiaHoy[0] = buffer[0];
			nDiaHoy[1] = buffer[1];
			nMesHoy[0] = buffer[3];
			nMesHoy[1] = buffer[4];
			nAnio[0] = buffer[6];
			nAnio[1] = buffer[7];
			nAnio[2] = buffer[8];
			nAnio[3] = buffer[9];
			hoy = atoi(DiaHoy);

			LUNES = GetDlgItem(hDlg, IDC_STATIC_DIA1);
			MARTES = GetDlgItem(hDlg, IDC_STATIC_DIA2);
			MIERCOLES = GetDlgItem(hDlg, IDC_STATIC_DIA3);
			JUEVES = GetDlgItem(hDlg, IDC_STATIC_DIA4);
			VIERNES = GetDlgItem(hDlg, IDC_STATIC_DIA5);
			SABADO = GetDlgItem(hDlg, IDC_STATIC_DIA6);
			DOMINGO = GetDlgItem(hDlg, IDC_STATIC_DIA7);
			VaciadoListas(hDlg, 0);
			//GetWindowText(GetDlgItem(IDC_DATETIMEPICKER3), (LSPTR)buffer2, 80);
			SendDlgItemMessage(hDlg, IDC_DATETIMEPICKER3, WM_GETTEXT, 20, LPARAM(Selbufer2));
			string fecha1(Selbufer2);
			Dia1 = fecha1.substr(0, 2);
			nselec = atoi(Dia1.c_str());
			Mes1 = fecha1.substr(3, 2);
			nselec2 = atoi(Mes1.c_str());
			Año1 = fecha1.substr(6, 4);
			nselec3 = atoi(Año1.c_str());
			//obtener dia de semana
			DiaSemana(Selbufer2);
		
			//guardar dia
			strcpy(semanaseleccion, semana); // guardar que dia de la semana es la seleccion
			CargarSemana(LUNES, MARTES, MIERCOLES, JUEVES, VIERNES, SABADO, DOMINGO, hDlg, nDiaHoy, nMesHoy, nAnio);
			/*OBTENER DIA*/
			return true; }
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_VISTADEREGRESO:
				EndDialog(hDlg, 0);
				return true;
			
			case IDC_SELECCION2:
				VaciadoListas(hDlg, 0);
				//GetWindowText(GetDlgItem(IDC_DATETIMEPICKER3), (LSPTR)buffer2, 80);
				SendDlgItemMessage(hDlg, IDC_DATETIMEPICKER3, WM_GETTEXT, 20, LPARAM(Selbufer2));
				string fecha2(Selbufer2);
				string Dia2 = fecha2.substr(0, 2);
				nselec = atoi(Dia2.c_str());
				string Mes2 = fecha2.substr(3, 2);
				nselec2 = atoi(Mes2.c_str());
				string Año2 = fecha2.substr(6, 4);
				nselec3 = atoi(Año2.c_str());
				//obtener dia de semana
				DiaSemana(Selbufer2);
				//guardar dia
				strcpy(semanaseleccion, semana); // guardar que dia de la semana es la seleccion
				nDiaHoy[0] = Selbufer2[0];
				nDiaHoy[1] = Selbufer2[1];
				nMesHoy[0] = Selbufer2[3];
				nMesHoy[1] = Selbufer2[4];
				nAnio[0] = Selbufer2[6];
				nAnio[1] = Selbufer2[7];
				nAnio[2] = Selbufer2[8];
				nAnio[3] = Selbufer2[9];
				CargarSemana(LUNES, MARTES, MIERCOLES, JUEVES, VIERNES, SABADO, DOMINGO, hDlg, nDiaHoy, nMesHoy, nAnio);
												 //comenzarLunes(); 
				//cargar dias en lb

				return true;
			}
			
			return true;

		}
		return false;
	}
	BOOL CALLBACK VISTA_EDBORRARproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
		static int cursor = 0;
		static char seleccion[30]="";
		static char seleccion2[_MAX_PATH] = "";
		static HWND LISTA1 = 0, LISTA2 = 0, LISTA3 = 0, LISTA4 = 0;
		switch (msg) {
		case WM_INITDIALOG:
			//CheckDlgButton(hDlg, IDC_CHECK1, BST_CHECKED);
			LISTA1 = GetDlgItem(hDlg, IDC_FECHA);
			LISTA2 = GetDlgItem(hDlg, IDC_ASUNTOLIST);
			LISTA3 = GetDlgItem(hDlg, IDC_CITALIST);
			LISTA4 = GetDlgItem(hDlg, IDC_HORALIST);
//			CheckDlgButton(hDlg, IDC_CHECK1, BST_CHECKED);
			
			
			strcpy(seleccion, "");

				return true;
		
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			case IDC_BUTTON1: //borrar
				SendMessage(LISTA3, LB_GETTEXT, (WPARAM)cursor, (LPARAM)seleccion);
				SendMessage(LISTA2, LB_GETTEXT, (WPARAM)cursor, (LPARAM)seleccion2);
				if (inicial == 0) {
					MessageBox(hDlg, "No existen alarmas", "AVISO", 0);
				}
				else {

					//ELIMINAR ALARMAS
					if (strcmp(seleccion, "") == 0) {
					MessageBox(hDlg, "No hay nada seleccionado", "AVISO", 0);
				}
					else {
					EliminarAlarmas(seleccion,seleccion2);
					strcpy(seleccion, "");
					cursor = 0;
					SendMessage(LISTA1, LB_RESETCONTENT, 0, 0);
					SendMessage(LISTA2, LB_RESETCONTENT, 0, 0);
					SendMessage(LISTA3, LB_RESETCONTENT, 0, 0);
					SendMessage(LISTA4, LB_RESETCONTENT, 0, 0);
					
					strcpy(seleccion, "");
					strcpy(seleccion2, "");
				}
				}
				return true;
			case IDC_BUTTON2: //editar
				if (inicial == 0 && strcmp(seleccion, "") == 0) {
					MessageBox(hDlg, "No existen alarmas", "AVISO", 0);
				}
				else {
					if (IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED){
						MessageBox(hDlg, "No puedes modificar este tipo de alarmas", "AVISO", 0);//aqui
				}
				else {
					SendMessage(LISTA3, LB_GETTEXT, (WPARAM)cursor, (LPARAM)seleccion);
					SendMessage(LISTA2, LB_GETTEXT, (WPARAM)cursor, (LPARAM)seleccion2);
					if (strcmp(seleccion, "") == 0 && strcmp(seleccion2, "") == 0) {
						MessageBox(hDlg, "No ha seleccionado alguna alarma", "AVISO", 0);
					}
					else {
						AlAuxiliar = inicial;
						while (inicial != 0) {
							if (strcmp(AlAuxiliar->nombre, seleccion) == 0 && strcmp(AlAuxiliar->asunto, seleccion2) == 0 
								&& strcmp(AlAuxiliar->vista,"0") == 0)
								break;

							if (AlAuxiliar == AlFin)
								break;
							else
								AlAuxiliar = AlAuxiliar->sig;

						}
						DialogBox(hInstance, MAKEINTRESOURCE(IDD_EDITCITA), hDlg, EditCitaProc);
						AlAuxiliar = AlFin;
						strcpy(seleccion, "");
						strcpy(seleccion2, "");
						cursor = 0;
						SendMessage(LISTA1, LB_RESETCONTENT, 0, 0);
						SendMessage(LISTA2, LB_RESETCONTENT, 0, 0);
						SendMessage(LISTA3, LB_RESETCONTENT, 0, 0);
						SendMessage(LISTA4, LB_RESETCONTENT, 0, 0);
						
						strcpy(seleccion, "");
						//EndDialog(hDlg, 0);
					}
				}
			}
				return true;
			case IDC_BUTTON3: //cancelar
				AlAuxiliar = AlFin;
				auxiliar = fin;
				strcpy(seleccion, "");
				strcpy(seleccion2, "");
				EndDialog(hDlg, 0);
				return true;
				//listas
			case IDC_FECHA:
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
					cursor = SendMessage(LISTA1, LB_GETCURSEL, 0, 0);
					SendMessage(LISTA2, LB_SETCURSEL, WPARAM(cursor), 0);
					SendMessage(LISTA3, LB_SETCURSEL, WPARAM(cursor), 0);
					SendMessage(LISTA4, LB_SETCURSEL, WPARAM(cursor), 0);
					return true;
				}
				return true;

			case IDC_CITALIST:
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
					cursor = SendMessage(LISTA3, LB_GETCURSEL, 0, 0);
					SendMessage(LISTA2, LB_SETCURSEL, WPARAM(cursor), 0);
					SendMessage(LISTA1, LB_SETCURSEL, WPARAM(cursor), 0);
					SendMessage(LISTA4, LB_SETCURSEL, WPARAM(cursor), 0);

					return true;
				}
				return true;

			case IDC_HORALIST:
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
					cursor = SendMessage(LISTA4, LB_GETCURSEL, 0, 0);
					SendMessage(LISTA2, LB_SETCURSEL, WPARAM(cursor), 0);
					SendMessage(LISTA3, LB_SETCURSEL, WPARAM(cursor), 0);
					SendMessage(LISTA1, LB_SETCURSEL, WPARAM(cursor), 0);
					return true;
				}
				return true;
			case IDC_ASUNTOLIST:
				switch (HIWORD(wParam)) {
				case LBN_SELCHANGE:
					cursor = SendMessage(LISTA2, LB_GETCURSEL, 0, 0);
					SendMessage(LISTA1, LB_SETCURSEL, WPARAM(cursor), 0);
					SendMessage(LISTA3, LB_SETCURSEL, WPARAM(cursor), 0);
					SendMessage(LISTA4, LB_SETCURSEL, WPARAM(cursor), 0);
					return true;
				}
				return true;
				//fin de listas y cursores
			case IDC_CHECK1: //VENCIDAS
				strcpy(seleccion, "");
				strcpy(seleccion2, "");
				SendMessage(LISTA1, LB_RESETCONTENT, 0, 0);
				SendMessage(LISTA2, LB_RESETCONTENT, 0, 0);
				SendMessage(LISTA3, LB_RESETCONTENT, 0, 0);
				SendMessage(LISTA4, LB_RESETCONTENT, 0, 0);

				CheckDlgButton(hDlg, IDC_CHECK1, BST_CHECKED);
				CheckDlgButton(hDlg, IDC_CHECK2, BST_UNCHECKED);
	// antes de ciclo
				AlAuxiliar = inicial;
				if (inicial != 0) {
					while (inicial != 0) {
						if (strcmp(AlAuxiliar->vista,"1")==0) {
						char aux[_MAX_PATH] = "";
						SendMessage(LISTA2, LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
						SendMessage(LISTA3, LB_ADDSTRING, -1, LPARAM(AlAuxiliar->nombre));
						strcat(aux, AlAuxiliar->dia);
						strcat(aux, "/");
						strcat(aux, AlAuxiliar->mes);
						strcat(aux, "/");
						strcat(aux, AlAuxiliar->anio);
						SendMessage(LISTA1, LB_ADDSTRING, -1, LPARAM(aux));
						strcpy(aux, "");
						strcat(aux, AlAuxiliar->hora);
						strcat(aux, ":");
						strcat(aux, AlAuxiliar->minutos);
						SendMessage(LISTA4, LB_ADDSTRING, -1, LPARAM(aux));
					}
						if (AlAuxiliar == AlFin)
							break;
						else
							AlAuxiliar = AlAuxiliar->sig;
					}
				}
				else {
					MessageBox(hDlg, "No hay alarmas", "AVISO", MB_OK);
				}
				AlAuxiliar = AlFin;
	//}
				//despues de ciclo
				return true;

			case IDC_CHECK2:
				strcpy(seleccion, "");
				strcpy(seleccion2, "");
				SendMessage(LISTA1, LB_RESETCONTENT, 0, 0);
				SendMessage(LISTA2, LB_RESETCONTENT, 0, 0);
				SendMessage(LISTA3, LB_RESETCONTENT, 0, 0);
				SendMessage(LISTA4, LB_RESETCONTENT, 0, 0);
				CheckDlgButton(hDlg, IDC_CHECK1, BST_UNCHECKED);
				CheckDlgButton(hDlg, IDC_CHECK2, BST_CHECKED);
				//antes
				// antes de ciclo
				AlAuxiliar = inicial;
				if (inicial != 0) {
					while (inicial != 0) {
						if (strcmp(AlAuxiliar->vista, "0") == 0) {
							char aux[_MAX_PATH] = "";
							SendMessage(LISTA2, LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
							SendMessage(LISTA3, LB_ADDSTRING, -1, LPARAM(AlAuxiliar->nombre));
							strcat(aux, AlAuxiliar->dia);
							strcat(aux, "/");
							strcat(aux, AlAuxiliar->mes);
							strcat(aux, "/");
							strcat(aux, AlAuxiliar->anio);
							SendMessage(LISTA1, LB_ADDSTRING, -1, LPARAM(aux));
							strcpy(aux, "");
							strcat(aux, AlAuxiliar->hora);
							strcat(aux, ":");
							strcat(aux, AlAuxiliar->minutos);
							SendMessage(LISTA4, LB_ADDSTRING, -1, LPARAM(aux));
						}
						if (AlAuxiliar == AlFin)
							break;
						else
							AlAuxiliar = AlAuxiliar->sig;
					}
				}
				else {
					MessageBox(hDlg, "No hay alarmas", "AVISO", MB_OK);
				}
				AlAuxiliar = AlFin;
	//}
	//despues de ciclo
				//despues
				return true;
				
			}
			
			return true;
		
		
		}
		return false;
	}
	BOOL CALLBACK VISTA_MENSUALproc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (msg) {
		case WM_INITDIALOG:
			//Mensual(hDlg);
			Mensual(hDlg);
			return true;
		case WM_COMMAND:
			switch (LOWORD(wParam)) {
			
			case IDC_MENSUAL_VOLVER:
				EndDialog(hDlg, 0);
				return true;
			
			} //case de command
			return true;
		
		}
	
		return false;
	}

	void Guardado() {
		if (inicio != 0) {
			ofstream archi;
			archi.open("C:\\temp\\Usuarios.txt", ios_base::binary | ios::trunc);
			if (archi.is_open()) {
				auxiliar = inicio;
				while (inicio != 0) {
					archi.write(auxiliar->usuario, 50);
					archi.write(auxiliar->Direccion, 50);
					archi.write(auxiliar->correo, 50);
					archi.write(auxiliar->telefono, 10);
					archi.write(auxiliar->casa, 8);
					archi.write(auxiliar->foto1, _MAX_PATH);
					archi.write(auxiliar->foto2, _MAX_PATH);
					archi.write(auxiliar->foto3, _MAX_PATH);
					archi.write(auxiliar->categoria, 2);
					archi.write(auxiliar->favorita, _MAX_PATH);
					archi.write(auxiliar->sexo, 2);
					//
					if (auxiliar == fin)
						break;
					else
						auxiliar = auxiliar->siguiente;
				}
			}
			archi.close();
		}
		
		if (inicial != 0) {
			
			ofstream AL;
			AL.open("C:\\temp\\Alarmas.txt", ios_base::binary | ios::trunc);
			if (AL.is_open()) {
				AlAuxiliar = inicial;
				while (inicial!=0) {
					AL.write(AlAuxiliar->dia, _MAX_PATH);
					AL.write(AlAuxiliar->mes, _MAX_PATH);
					AL.write(AlAuxiliar->anio, _MAX_PATH);
					AL.write(AlAuxiliar->hora, _MAX_PATH);
					AL.write(AlAuxiliar->minutos, _MAX_PATH);
					AL.write(AlAuxiliar->nombre, 30);
					AL.write(AlAuxiliar->asunto, _MAX_PATH);
					AL.write(AlAuxiliar->vista, _MAX_PATH);
					AL.write(AlAuxiliar->sonido, _MAX_PATH);
					if (AlAuxiliar == AlFin)
						break;
					else
						AlAuxiliar = AlAuxiliar->sig;
				}
			}
			delete inicial;
			AL.close();
		}

	}
	void Validacion() {
		auxiliar = inicio;
		nodos*borrar = 0;
		while (inicio != 0) {
			if (strcmp(auxiliar->usuario, "") == 0) {
				borrar = auxiliar;
				auxiliar = auxiliar->anterior;
				auxiliar->siguiente = NULL;
				delete borrar;
				break;
			}
			else {
				auxiliar = auxiliar->siguiente;

			}

		}
		fin = auxiliar;


		
	}
	void Precarga() {
		ifstream archivo;
		archivo.open("C:\\temp\\Usuarios.txt", ios_base::binary); //GetCurrentDirectory

		if (archivo.is_open()) {
			while (!archivo.eof()) {
				if (inicio == 0) {
					nodos*nuevo = new nodos;
				
					archivo.read(nuevo->usuario, 50);
					archivo.read(nuevo->Direccion, 50);
					archivo.read(nuevo->correo, 50);
					archivo.read(nuevo->telefono, 10);
					archivo.read(nuevo->casa, 8);
					archivo.read(nuevo->foto1, _MAX_PATH);
					archivo.read(nuevo->foto2, _MAX_PATH);
					archivo.read(nuevo->foto3, _MAX_PATH);
					archivo.read(nuevo->categoria, 2);
					archivo.read(nuevo->favorita, _MAX_PATH);
					archivo.read(nuevo->sexo, 2);
					nuevo->anterior = NULL;
					nuevo->siguiente = NULL;
					inicio = fin = auxiliar = nuevo;
				}
				else {
					nodos*nuevo = new nodos;

					auxiliar->siguiente = nuevo;
					archivo.read(nuevo->usuario, 50);
					archivo.read(nuevo->Direccion, 50);
					archivo.read(nuevo->correo, 50);
					archivo.read(nuevo->telefono, 10);
					archivo.read(nuevo->casa, 8);
					archivo.read(nuevo->foto1, _MAX_PATH);
					archivo.read(nuevo->foto2, _MAX_PATH);
					archivo.read(nuevo->foto3, _MAX_PATH);
					archivo.read(nuevo->categoria, 2);
					archivo.read(nuevo->favorita, _MAX_PATH);
					archivo.read(nuevo->sexo, 2);

					nuevo->anterior = auxiliar;

					nuevo->siguiente = NULL;
					fin = auxiliar = nuevo;
				}
			}
		}
		archivo.close();

		

		ifstream AL;
		AL.open("C:\\temp\\Alarmas.txt", ios_base::binary);
		if (AL.is_open()) {
			while (!AL.eof()) {
				if (inicial == 0) {
					alarmas*lectura = new alarmas;
					AL.read(lectura->dia, _MAX_PATH);
					AL.read(lectura->mes, _MAX_PATH);
					AL.read(lectura->anio, _MAX_PATH);
					AL.read(lectura->hora, _MAX_PATH);
					AL.read(lectura->minutos, _MAX_PATH);
					AL.read(lectura->nombre, 30);
					AL.read(lectura->asunto, _MAX_PATH);
					AL.read(lectura->vista, _MAX_PATH);
					AL.read(lectura->sonido, _MAX_PATH);
					lectura->sig = NULL;
					lectura->ant = NULL;
					inicial =AlFin=AlAuxiliar=lectura;
				}
				else {
				alarmas*lectura = new alarmas;
					AL.read(lectura->dia, _MAX_PATH);
					AL.read(lectura->mes, _MAX_PATH);
					AL.read(lectura->anio, _MAX_PATH);
					AL.read(lectura->hora, _MAX_PATH);
					AL.read(lectura->minutos, _MAX_PATH);
					AL.read(lectura->nombre, 30);
					AL.read(lectura->asunto, _MAX_PATH);
					AL.read(lectura->vista, _MAX_PATH);
					AL.read(lectura->sonido, _MAX_PATH);
					lectura->ant = AlAuxiliar;
					lectura->sig = NULL;
					AlAuxiliar->sig = lectura;
					AlFin = AlAuxiliar = lectura;

				}
			}
		}
		AL.close();
		alarmas*BORRANDO = 0;

		AlAuxiliar = inicial;
		while (inicial != 0) {
			if (strcmp(AlAuxiliar->nombre, "") == 0) {
				BORRANDO = AlAuxiliar;
				AlAuxiliar = AlAuxiliar->ant;
				AlAuxiliar->sig = NULL;
				delete BORRANDO;
				break;

				if (AlAuxiliar = AlFin)
					break;
				else
					AlAuxiliar = AlAuxiliar->sig;

			}
			else {
				AlAuxiliar = AlAuxiliar->sig;
			}

		}
		AlFin = AlAuxiliar;
		Validacion();
	}
	void Desplegar(HWND hDlg, char*categoria) {
		auxiliar = inicio;
		while (inicio != 0) {
			if (strcmp(auxiliar->categoria, categoria) == 0) {
				SendMessage(hDlg, LB_ADDSTRING, _MAX_PATH, (LPARAM)auxiliar->usuario);
			}
			if (auxiliar == fin)
				break;
			else
				auxiliar = auxiliar->siguiente;
		}
		auxiliar = fin;

	}
	void CARGAGRUPOS(HWND hDlg, HWND H) {
		
		SendMessage(H, CB_ADDSTRING, 1, (LPARAM)"AMIGOS");
		SendMessage(H, CB_ADDSTRING, 1, (LPARAM)"COMPAÑEROS DE ESCUELA");
		SendMessage(H, CB_ADDSTRING, 1, (LPARAM)"COMPAÑEROS DE TRABAJO");
		SendMessage(H, CB_ADDSTRING, 1, (LPARAM)"FAMILIA");
	}
	char * file(char filtro[3], HWND hDlg, char tag[3], char*punte,int uno) {
		OPENFILENAME ofn;
		char szFileName[MAX_PATH] = "";
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW 
		ofn.hwndOwner = hDlg;
		if (filtro == "MP3")
			ofn.lpstrFilter = "Musica (.mp3)\0*.mp3\0"; //Musica (.mp3)\0*.mp3\0
		else
			if (filtro == "BMP")
				ofn.lpstrFilter = "Imagen (.bmp)\0*.bmp\0";


		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = "txt";

		if (GetOpenFileName(&ofn) == TRUE) {
			//SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), ofn.lpstrFile);
			if (uno == 0) {
				if (strcmp(tag, "FO1") == 0) {
					SetWindowText(GetDlgItem(hDlg, IDC_STATIC__FOTO1), ofn.lpstrFile); strcpy(punte, ofn.lpstrFile);
				}
				else {
					if (strcmp(tag, "FO2") == 0) {
						SetWindowText(GetDlgItem(hDlg, IDC_STATIC__FOTO2), ofn.lpstrFile); strcpy(punte, ofn.lpstrFile);
					}
					else {
						if (strcmp(tag, "FO3") == 0) {
							SetWindowText(GetDlgItem(hDlg, IDC_STATIC_FOTO3), ofn.lpstrFile); strcpy(punte, ofn.lpstrFile);
						}
						else {
							if (strcmp(tag, "MP3") == 0) {
								SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CANCION), ofn.lpstrFile);
								strcpy(punte, ofn.lpstrFile);
							}
						}
					}
				}
			}
			else {
				//editar
				if (strcmp(tag, "FO1") == 0) {
					SetWindowText(GetDlgItem(hDlg, IDC_STATIC__EDFOTO1), ofn.lpstrFile); strcpy(punte, ofn.lpstrFile);
				}
				else {
					if (strcmp(tag, "FO2") == 0) {
						SetWindowText(GetDlgItem(hDlg, IDC_STATIC__EDFOTO2), ofn.lpstrFile); strcpy(punte, ofn.lpstrFile);
					}
					else {
						if (strcmp(tag, "FO3") == 0) {
							SetWindowText(GetDlgItem(hDlg, IDC_STATIC_EDFOTO3), ofn.lpstrFile); strcpy(punte, ofn.lpstrFile);
						}
						else {
							if (strcmp(tag, "MP3") == 0) {
								SetWindowText(GetDlgItem(hDlg, IDC_STATIC_EDCANCION), ofn.lpstrFile);
								strcpy(punte, ofn.lpstrFile);
							}
						}
					}
				}
				
				//fin
			
			}
			
			}
		else{
			MessageBox(hDlg, "No eligió archivo", "Aviso", MB_OK | MB_ICONINFORMATION);
		}
		

		return false;
}
	void busqueda(HWND hDlg, bool n, char*nombre) {
		if (n) {
			auxiliar = inicio;
			while (inicio != 0) {
				if (strcmp(nombre, auxiliar->usuario) == 0) {
					SetWindowText(GetDlgItem(hDlg, IDC_EDIT1), auxiliar->usuario);
					SetWindowText(GetDlgItem(hDlg, IDC_EDIT2), auxiliar->telefono);
					SetWindowText(GetDlgItem(hDlg, IDC_EDIT3), auxiliar->casa);
					SetWindowText(GetDlgItem(hDlg, IDC_EDIT4), auxiliar->correo);
					SetWindowText(GetDlgItem(hDlg, IDC_EDIT5), auxiliar->Direccion);
					if (auxiliar->sexo == 0)
						CheckDlgButton(hDlg, IDC_CHECK_HOMBRE, BST_CHECKED);
					else
						CheckDlgButton(hDlg, IDC_CHECK_MUJER, BST_CHECKED);

					SetWindowText(GetDlgItem(hDlg, IDC_STATIC_CANCION1), auxiliar->favorita);
					SetWindowText(GetDlgItem(hDlg, IDC_STATIC__FOTO1E), auxiliar->foto1);
					SetWindowText(GetDlgItem(hDlg, IDC_STATIC__FOTO2E), auxiliar->foto2);
					SetWindowText(GetDlgItem(hDlg, IDC_STATIC_FOTO3E), auxiliar->foto3);

				}
				if (auxiliar == fin) {
					break;
				}

			}


		}
		else {
			//ALARMAS EDITAR

		}


	}
	void CONTACTOS(char *nombre, char*direccion, char*correo, char*tel1, char*casa, char*categoria, char*cfavorita, char*sexo, char*foto1, char*foto2, char*foto3) {

		if (inicio == 0) {

			nodos*nuevo = new nodos;
			nuevo->anterior = NULL;
			strcpy(nuevo->usuario, nombre);
			strcpy(nuevo->Direccion, direccion);
			strcpy(nuevo->correo, correo);
			strcpy(nuevo->telefono, tel1);
			strcpy(nuevo->casa, casa);
			strcpy(nuevo->categoria, categoria);
			strcpy(nuevo->favorita, cfavorita);
			strcpy(nuevo->sexo, sexo);
			strcpy(nuevo->foto1, foto1);
			strcpy(nuevo->foto2, foto2);
			strcpy(nuevo->foto3, foto3);
			//strcpy(nuevo->categoria,categoria);

			nuevo->siguiente = NULL;
			inicio = fin = auxiliar = nuevo;

		}
		else {
			nodos*nuevo = new nodos;
			nuevo->anterior = auxiliar;
			auxiliar->siguiente = nuevo;
			strcpy(nuevo->usuario, nombre);
			strcpy(nuevo->Direccion, direccion);
			strcpy(nuevo->correo, correo);
			strcpy(nuevo->telefono, tel1);
			strcpy(nuevo->casa, casa);
			strcpy(nuevo->categoria, categoria);
			//nuevo->categoria = categoria;
			strcpy(nuevo->favorita, cfavorita);
			strcpy(nuevo->sexo, sexo);
			//nuevo->sexo = sexo;
			strcpy(nuevo->foto1, foto1);
			strcpy(nuevo->foto2, foto2);
			strcpy(nuevo->foto3, foto3);
			nuevo->siguiente = NULL;
			fin = auxiliar = nuevo;
		}

	}
	void EliminarNodos(char* n) {
		//considerar 3 tipos de casos en eliminar
		// primer nodo, ultimo nodo, nodo de en medio
		//si es el nodo de enmedio se ocuparan 2 Auxiliares

		nodos*AuxiliarAntes=0;
		nodos*AuxiliarDespues=0;
		nodos*borra = 0;
		
		bool encontrado = false;
		//aqui num

		auxiliar = inicio;
		//if (num == 0) {
			if (inicio != 0) {
				//la lista no esta vacia
				//cout << "usuario a borrar\n";
				//cin >> borrar;
				while (inicio != 0) {
					if (/*auxiliar->usuario == n*/ strcmp(auxiliar->usuario, n) == 0) {
						if (inicio == fin) {
							//solo existe un nodo
							borra = auxiliar; //<-
							delete[]borra;
							inicio = fin = auxiliar = 0;
							encontrado = true;
							break;
						}
						else {
							if (auxiliar == inicio) {
								inicio = inicio->siguiente;
								borra = auxiliar;
								auxiliar = inicio;
								delete[] borra;
								encontrado = true;
								break;
							}
							else {
								if (auxiliar == fin) {
									fin = fin->anterior;
									borra = auxiliar;
									auxiliar = fin;
									delete[]borra;
									encontrado = true;
									break;
								}
								else {
									if (strcmp(auxiliar->usuario, n) == 0) {
										//en este punto significa que es uno de los nodos
										//de enmedio
										AuxiliarAntes = auxiliar->anterior;
										AuxiliarDespues = auxiliar->siguiente;
										//entrelazar los nodos
										AuxiliarAntes->siguiente = AuxiliarDespues;
										AuxiliarDespues->anterior = AuxiliarAntes;
										//eliminar dato encontrado
										borra = auxiliar;
										delete[]borra;
										auxiliar = fin;
										encontrado = true;
										break;
									}

								}

							}

						}

					}
					if (auxiliar != fin) {
						//no ha recorrido hasta el ultimo nodo
						auxiliar = auxiliar->siguiente;
					}
					else {
						//en caso de acabar de recorrer la lista salir
						break;
					}
				}
			}
			else {
				//cout << "La lista se encuentra vacia" << endl;
				MessageBox(0, "La Lista Esta Vacia", "Aviso", MB_OK + MB_ICONINFORMATION);
			}
		//}
	
		
			if (encontrado) {
				MessageBox(0, "Usuario Borrado", "Aviso", MB_OK + MB_ICONINFORMATION);
				//cout << "nodo eliminado\n";
			}
			else {
				MessageBox(0, "No se encontro usuario", "Aviso", MB_OK + MB_ICONINFORMATION);
				//cout << "El nodo no existe\n";
			}
		
			

		
		//regresar el auxiliar a su lugar
		auxiliar = fin;

	}
	void Alarma(char *dia, char*mes, char*anio, char*asunto, char*minuto, char*hora,char*nombre,char*sonido) {
		//char aniocorrompe[4] = "";
		char vista[_MAX_PATH] = "";
		strcpy(vista, "0");
		if (inicial == 0) {
			
			alarmas*nuevo = new alarmas;
			nuevo->ant = NULL;
			nuevo->sig = NULL;
			strcpy(nuevo->asunto, asunto);
			strcpy(nuevo->dia, dia);
			strcpy(nuevo->mes, mes);
		/*	aniocorrompe[0] = anio[0];
			aniocorrompe[1] = anio[1];
			aniocorrompe[2] = anio[2];
			aniocorrompe[3] = anio[3];
*/
			//strcmp(anio, aniocorrompe);
			strcpy(nuevo->anio, anio);
			strcpy(nuevo->hora, hora);
			strcpy(nuevo->minutos, minuto);
			strcpy(nuevo->nombre, nombre);
			strcpy(nuevo->sonido, sonido);
			strcpy(nuevo->vista,vista);
			AlAuxiliar = inicial = AlFin = nuevo;
		}
		else {
			//
			alarmas*nuevo = new alarmas;
			strcpy(nuevo->asunto, asunto);
			strcpy(nuevo->dia, dia);
			strcpy(nuevo->mes, mes);
			//aniocorrompe[0] = anio[0];
			//aniocorrompe[1] = anio[1];
			//aniocorrompe[2] = anio[2];
			//aniocorrompe[3] = anio[3];
			//strcmp(anio, aniocorrompe);
			strcpy(nuevo->anio, anio);
			strcpy(nuevo->hora, hora);
			strcpy(nuevo->minutos, minuto);
			strcpy(nuevo->nombre, nombre);
			strcpy(nuevo->sonido, sonido);
			strcpy(nuevo->vista, vista);
			nuevo->ant = AlAuxiliar;
			AlAuxiliar->sig = nuevo;
			nuevo->sig = NULL;
			AlAuxiliar = AlFin = nuevo;
			//
		}
	}
	void Semana(HWND hDlg,int hoy) {
		if (hoy <= dias[0]) {
			//MessageBox(0, (char*)hoy, "Hoy es", 0);
			SendDlgItemMessage(hDlg, IDC_STATIC_DIA1, WM_SETTEXT, 0, LPARAM("1"));
			SendDlgItemMessage(hDlg, IDC_STATIC_DIA2, WM_SETTEXT, 0, LPARAM("2"));
			SendDlgItemMessage(hDlg, IDC_STATIC_DIA3, WM_SETTEXT, 0, LPARAM("3"));
			SendDlgItemMessage(hDlg, IDC_STATIC_DIA4, WM_SETTEXT, 0, LPARAM("4"));
			SendDlgItemMessage(hDlg, IDC_STATIC_DIA5, WM_SETTEXT, 0, LPARAM("5"));
			SendDlgItemMessage(hDlg, IDC_STATIC_DIA6, WM_SETTEXT, 0, LPARAM("6"));
			SendDlgItemMessage(hDlg, IDC_STATIC_DIA7, WM_SETTEXT, 0, LPARAM("7"));
		}
		else {
			if (hoy <= dias[1]) {
				//MessageBox(0, (char*)hoy, "Hoy es", 0);
				SendDlgItemMessage(hDlg, IDC_STATIC_DIA1, WM_SETTEXT, 0, LPARAM("8"));
				SendDlgItemMessage(hDlg, IDC_STATIC_DIA2, WM_SETTEXT, 0, LPARAM("9"));
				SendDlgItemMessage(hDlg, IDC_STATIC_DIA3, WM_SETTEXT, 0, LPARAM("10"));
				SendDlgItemMessage(hDlg, IDC_STATIC_DIA4, WM_SETTEXT, 0, LPARAM("11"));
				SendDlgItemMessage(hDlg, IDC_STATIC_DIA5, WM_SETTEXT, 0, LPARAM("12"));
				SendDlgItemMessage(hDlg, IDC_STATIC_DIA6, WM_SETTEXT, 0, LPARAM("13"));
				SendDlgItemMessage(hDlg, IDC_STATIC_DIA7, WM_SETTEXT, 0, LPARAM("14"));
			}
			else {

				if (hoy <= dias[2]) {
					//	MessageBox(0, (char*)hoy, "Hoy es", 0);
					SendDlgItemMessage(hDlg, IDC_STATIC_DIA1, WM_SETTEXT, 0, LPARAM("15"));
					SendDlgItemMessage(hDlg, IDC_STATIC_DIA2, WM_SETTEXT, 0, LPARAM("16"));
					SendDlgItemMessage(hDlg, IDC_STATIC_DIA3, WM_SETTEXT, 0, LPARAM("17"));
					SendDlgItemMessage(hDlg, IDC_STATIC_DIA4, WM_SETTEXT, 0, LPARAM("18"));
					SendDlgItemMessage(hDlg, IDC_STATIC_DIA5, WM_SETTEXT, 0, LPARAM("19"));
					SendDlgItemMessage(hDlg, IDC_STATIC_DIA6, WM_SETTEXT, 0, LPARAM("20"));
					SendDlgItemMessage(hDlg, IDC_STATIC_DIA7, WM_SETTEXT, 0, LPARAM("21"));

				}
				else {

					if (hoy <= dias[3]) {
						//	MessageBox(0, (char*)hoy, "Hoy es", 0);
						SendDlgItemMessage(hDlg, IDC_STATIC_DIA1, WM_SETTEXT, 0, LPARAM("22"));
						SendDlgItemMessage(hDlg, IDC_STATIC_DIA2, WM_SETTEXT, 0, LPARAM("23"));
						SendDlgItemMessage(hDlg, IDC_STATIC_DIA3, WM_SETTEXT, 0, LPARAM("24"));
						SendDlgItemMessage(hDlg, IDC_STATIC_DIA4, WM_SETTEXT, 0, LPARAM("25"));
						SendDlgItemMessage(hDlg, IDC_STATIC_DIA5, WM_SETTEXT, 0, LPARAM("26"));
						SendDlgItemMessage(hDlg, IDC_STATIC_DIA6, WM_SETTEXT, 0, LPARAM("27"));
						SendDlgItemMessage(hDlg, IDC_STATIC_DIA7, WM_SETTEXT, 0, LPARAM("28"));
					}
					else {

						if (hoy <= dias[4]) {
							//	MessageBox(0, (char*)hoy, "Hoy es", 0);
							SendDlgItemMessage(hDlg, IDC_STATIC_DIA1, WM_SETTEXT, 0, LPARAM("29"));
							SendDlgItemMessage(hDlg, IDC_STATIC_DIA2, WM_SETTEXT, 0, LPARAM("30"));
							SendDlgItemMessage(hDlg, IDC_STATIC_DIA3, WM_SETTEXT, 0, LPARAM("31"));

						}
						else {

						}
					}
				}
			}
		}


} //BAN PAPU
	int AlarmaVer(HWND hDlg,char*dia, char*mes, char*anio, char*minutos, char*hora) {
		/*char anioCompompe[4] = "";
		anioCompompe[0] = anio[0];
		anioCompompe[1] = anio[1];
		anioCompompe[2] = anio[2];
		anioCompompe[3] = anio[3];*/
		char vista[_MAX_PATH] = "";
		strcpy(vista, "0");
		char visita[_MAX_PATH] = "";
		char asun[_MAX_PATH] = "";
		strcpy(visita, "1");
		int aceptar = 0;
		AlAuxiliar = inicial;
		if (inicial != 0) {
			while (inicial != 0) {
				
					
					if(strcmp(AlAuxiliar->dia,dia) == 0 && strcmp(AlAuxiliar->mes,mes) == 0 && 
						strcmp(AlAuxiliar->vista, vista)==0 && strcmp(AlAuxiliar->anio, YYYYHoy) == 0 
						&& strcmp(AlAuxiliar->minutos, MinutosHoy)==0 && strcmp(AlAuxiliar->hora, HoraHoy) == 0)
				{
					strcpy(AlAuxiliar->vista,"1"); //alarma atendida
					char cancion[_MAX_PATH] = "";
					char asunto[_MAX_PATH] = "";
					strcat(asunto, "Cita con: ");
					strcat(asunto, AlAuxiliar->nombre);
					strcat(asun, "ASUNTO: ");
					strcat(asun, AlAuxiliar->asunto);
					strcat(cancion, "open "); //agrega open
					strcat(cancion, AlAuxiliar->sonido); //agrega direccion
					strcat(cancion, " alias alarma");
					mciSendString(cancion, NULL, 0, 0);
					mciSendString("play alarma", NULL, 0, 0);
					//aceptar = MessageBox(hDlg, AlAuxiliar->asunto, asunto, MB_OK+MB_ICONINFORMATION);
					if (aceptar = MessageBox(hDlg, asun, asunto, MB_OK + MB_ICONINFORMATION)) {

						mciSendString("stop alarma", NULL, 0, 0);
						mciSendString("close alarma", NULL, 0, 0);
						return false;
					}


				}

				if (AlAuxiliar == AlFin) {
				break;
			}
				else {
					AlAuxiliar = AlAuxiliar->sig;
				}
		
			}
			
		}
		

		return true;
	}
	void CargaSemanal(HWND hDlg, HWND L, HWND M, HWND MM, HWND J, HWND V, HWND S, HWND D) {
		int lunes=0;
		char SEMANADIA[_MAX_PATH] = "";
		SendMessage(L, WM_GETTEXT, _MAX_PATH, LPARAM(SEMANADIA));
		lunes = atoi(SEMANADIA);
		AlAuxiliar = inicial;
		while (AlAuxiliar != 0) {
		if(strcmp(AlAuxiliar->dia,SEMANADIA)==0 && strcmp(AlAuxiliar->mes,"05") == 0&& strcmp(AlAuxiliar->anio,"2017") == 0)
			if (AlAuxiliar == AlFin)
				break;
			else
				AlAuxiliar = AlAuxiliar->sig;
		}
	
	
	}
	void Perdidas() {
		int op = 0;
		int aceptar=0;
		char asunto[_MAX_PATH] = "";
		char lahora[_MAX_PATH] = "";
		int fecha[5] = { 0 };

		int hhoy[5] = { 0 };
		hhoy[0] = atoi(DiaHoy);
		//mes
		hhoy[1] = atoi(MesHoy);
		//anio
		hhoy[2] = atoi(YYYYHoy);
		//horas
		hhoy[3] = atoi(HoraHoy);
		//MINUTOS
		hhoy[4] = atoi(MinutosHoy);



		char vista[_MAX_PATH] = "";
		strcpy(vista, "0");






		AlAuxiliar = inicial;
		while (inicial != 0) {
			if (strcmp(AlAuxiliar->vista, vista) == 0) {

				fecha[0] = atoi(AlAuxiliar->dia);
				//mes ALARMA
				fecha[1] = atoi(AlAuxiliar->mes);
				//anio ALARMA
				fecha[2] = atoi(AlAuxiliar->anio);
				//HORAS ALARMA
				fecha[3] = atoi(AlAuxiliar->hora);
				//MINUTOS ALARMA
				fecha[4] = atoi(AlAuxiliar->minutos);


				FechaMenor(&op, &fecha[0], &fecha[1], &fecha[2], &fecha[3], &fecha[4], &hhoy[0], &hhoy[1], &hhoy[2], &hhoy[3], &hhoy[4]);
				if (op == 1) {
					
					strcat(asunto, "Cita con: ");
					strcat(asunto, AlAuxiliar->nombre);
					strcat(asunto, ". ASUNTO: ");
					strcat(asunto, AlAuxiliar->asunto);
					strcat(lahora, "Alarma Perdida: ");
					strcat(lahora, AlAuxiliar->dia);
					strcat(lahora, "/");
					strcat(lahora, AlAuxiliar->mes);
					strcat(lahora, "/");
					strcat(lahora, AlAuxiliar->anio);
					strcat(lahora, " Hora:");
					strcat(lahora, AlAuxiliar->hora);
					strcat(lahora, ":");
					strcat(lahora, AlAuxiliar->minutos);
					strcpy(AlAuxiliar->vista, "1");
					
					if (aceptar = MessageBox(hDlg,asunto,lahora, MB_OK + MB_ICONINFORMATION))
						strcpy(AlAuxiliar->vista, "1");
						//nada
				}
			}
			if (AlAuxiliar == AlFin)
				break;
			else
				AlAuxiliar = AlAuxiliar->sig;
		
		}
		AlAuxiliar = AlFin;
	}
	void EliminarAlarmas(char* n,char*a) {
		//considerar 3 tipos de casos en eliminar
		// primer nodo, ultimo nodo, nodo de en medio
		//si es el nodo de enmedio se ocuparan 2 Auxiliares

		alarmas*AuxiliarAntes = 0;
		alarmas*AuxiliarDespues = 0;
		alarmas*borra = 0;

		bool encontrado = false;
		//aqui num

		AlAuxiliar = inicial;
		//if (num == 0) {
		if (inicio != 0) {
			//la lista no esta vacia
			//cout << "usuario a borrar\n";
			//cin >> borrar;
			while (inicial != 0) {
				if (strcmp(AlAuxiliar->asunto, a) == 0 && strcmp(AlAuxiliar->nombre, n) == 0) {
					if (inicial == AlFin) {
						//solo existe un nodo
						borra = AlAuxiliar; //<-
						delete[]borra;
						inicial = AlFin = AlAuxiliar = 0;
						encontrado = true;
						break;
					}
					else {
						if (AlAuxiliar == inicial) {
							inicial = inicial->sig;
							borra = AlAuxiliar;
							AlAuxiliar = inicial;
							delete[] borra;
							encontrado = true;
							break;
						}
						else {
							if (AlAuxiliar == AlFin) {
								AlFin = AlFin->ant;
								borra = AlAuxiliar;
								AlAuxiliar = AlFin;
								delete[]borra;
								encontrado = true;
								break;
							}
							else {
								if (strcmp(AlAuxiliar->asunto, a) == 0 && strcmp(AlAuxiliar->nombre, n) == 0) {
									//en este punto significa que es uno de los nodos
									//de enmedio
									AuxiliarAntes = AlAuxiliar->ant;
									AuxiliarDespues = AlAuxiliar->sig;
									//entrelazar los nodos
									AuxiliarAntes->sig = AuxiliarDespues;
									AuxiliarDespues->ant = AuxiliarAntes;
									//eliminar dato encontrado
									borra = AlAuxiliar;
									delete[]borra;
									AlAuxiliar = AlFin;
									encontrado = true;
									break;
								}

							}

						}

					}

				}
				if (AlAuxiliar != AlFin) {
					//no ha recorrido hasta el ultimo nodo
					AlAuxiliar = AlAuxiliar->sig;
				}
				else {
					//en caso de acabar de recorrer la lista salir
					break;
				}
			}
		}
		else {
			//cout << "La lista se encuentra vacia" << endl;
			MessageBox(0, "La Lista Esta Vacia", "Aviso", MB_OK + MB_ICONINFORMATION);
		}
		//}


		if (encontrado) {
			MessageBox(0, "Cita Borrada", "Aviso", MB_OK + MB_ICONINFORMATION);
			//cout << "nodo eliminado\n";
		}
		else {
			MessageBox(0, "No se encontrola cita", "Aviso", MB_OK + MB_ICONINFORMATION);
			//cout << "El nodo no existe\n";
		}




		//regresar el auxiliar a su lugar
		auxiliar = fin;

	}
	void FechaMenor(int*op, int*D, int*M, int*Y, int*H, int*mm, int*D2, int*M2, int*Y2, int*H2, int*mm2) {
	//int*D2, int*M2, int*Y2, int*H2, int*mm2 dia y fecha de hoy
		*op = 1;
		int resultado = 0;

		//falsas
		//if (D < D2	&&	M < M2	&&	Y < Y2) //fecha menor
		//	*op = 1;

		//if (H < H2	&&	D==D2 && M==M2 && Y==Y2	) //hora menor de este dia
		//	*op = 1;

		//if (D < D2	&&	Y < Y2) //dia y año menor
		//	*op = 1;

		//if (Y < Y2) //año es menor
		//	*op = 1;

		//if(H == H2 && mm<mm2	&&	D == D2 && M == M2 && Y == Y2)
		//	*op = 1;
		
		//verdaderas

		if (*H == *H2 && *mm>*mm2	&&	*D == *D2 && *M == *M2 && *Y == *Y2) //Este *Dia, misma *Hora, aun no concuer*Da los minutos *De alarma
			*op = 0;
		if (*M > *M2	&&	*Y == *Y2) //mes ma*Yor, año el mismo
			*op = 0;
		if(*Y>*Y2)		//año ma*Yor
			*op = 0;
		if(*D>*D2 && *M==*M2 && *Y==*Y2)
			*op = 0;

		if(*M>*M2 && *Y==*Y2)
			*op = 0;
		if(*D==*D2 && *M==*M2 && *Y==*Y2 && *H==*H2&& *mm>*mm2)
			*op = 0;

		if (*D == *D2 && *M == *M2 && *Y == *Y2 && *H > *H2)
			*op = 0;

	

	
	}
	void CargarSemana(HWND DL, HWND DM, HWND DMM, HWND DJ, HWND DV, HWND DS, HWND DD, HWND hDlg, char*dia, char*mes, char*anio) {
		int seguridad = atoi(dia);
		int Vista = 0;
		char aniovisto[_MAX_PATH] = "";
		aniovisto[0] = anio[0];
		aniovisto[1] = anio[1];
		aniovisto[2] = anio[2];
		aniovisto[3] = anio[3];
		strcpy(anio, "");
		strcpy(anio, aniovisto);
		bool bisiesto = false;
		char bisiestoN[2] = "";
		bisiestoN[0] = anio[2];
		bisiestoN[1] = anio[3]; //ultimos 2 digitos
		int aux;
		aux = atoi(bisiestoN);
		if (aux % 4 == 0)
			bisiesto = true;	//visualizando mes bisiesto				
		else
			bisiesto = false;
		int maximo = 0; //si se acaba el mes en la vista semnal

		if (bisiesto&&strcmp(mes, "02") == 0)		//si visualizo mes de febrero y este es biciesto
			maximo = 29;
		else
			if (bisiesto == false && strcmp(mes, "02") == 0)
				maximo = 28;
		
		if (strcmp(semanaseleccion, "Lunes") == 0) {
			Vista = atoi(dia);
		}

		if (strcmp(semanaseleccion, "Martes") == 0 && seguridad >= 1) {
			Vista = atoi(dia);
			Vista -= 1;
		}
		else {
			if (strcmp(semanaseleccion, "Martes") == 0) {
				//cuando el dia es 1 en martes
			//Vista = 1; REVISADO
				if (strcmp(mes, "01") == 0) { //un a;o hacia atras
					strcpy(mes, "12");
					int NuevoAnio = atoi(aniovisto);
					NuevoAnio--;
					char n[_MAX_PATH] = "";
					_itoa(NuevoAnio, n, 10);
					anio[0] = n[0];
					anio[1] = n[1];
					anio[2] = n[2];
					anio[3] = n[3];

					Vista = 31;
				}
				else {
					if (strcmp(mes, "12") == 0) {
						strcpy(mes, "11");
						//int NuevoAnio = atoi(aniovisto);
						//NuevoAnio--;
						Vista = 30;
					}
					else {
						if (strcmp(mes, "11") == 0) {
							strcpy(mes, "10");
							Vista = 30;
						}
						else {
							if (strcmp(mes, "10") == 0) {
								strcpy(mes, "09");
								Vista = 30;
							}
							else {
								if (strcmp(mes, "09") == 0) {
									strcpy(mes, "08");
									Vista = 31;
								}
								else {
									if (strcmp(mes, "08") == 0) {
										strcpy(mes, "07");
										Vista = 31;
									}
									else {
										if (strcmp(mes, "07") == 0) {
											strcpy(mes, "06");
											Vista = 30;
										}
										else {
											if (strcmp(mes, "06") == 0) {
												strcpy(mes, "05");
												Vista = 31;
											}
											else {
												if (strcmp(mes, "05") == 0) {
													strcpy(mes, "04");
													Vista = 30;
												}
												else {
													if (strcmp(mes, "04") == 0) {
														strcpy(mes, "03");
														Vista = 31;
													}
													else {
														if (strcmp(mes, "03") == 0) {
															strcpy(mes, "02");
															if (bisiesto)
																Vista = 29;
															else
																Vista = 28;
														}
														else {
															if (strcmp(mes, "02") == 0) {
																strcpy(mes, "01");
																Vista = 31;
															}
															//no cagarla
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	
			if (strcmp(semanaseleccion, "Miercoles") == 0&&seguridad > 2) {
				Vista = atoi(dia);
				Vista -= 2;
			}
			else { //REVISADO
				if (strcmp(semanaseleccion, "Miercoles") == 0) {
					if (seguridad == 2) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 31;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 30;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 31;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 30;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 31;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 31;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 30;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 31;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 30;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 31;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 29;
																	else
																		Vista = 28;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 31;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					else {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 30;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 29;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 30;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 29;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 30;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 30;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 29;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 30;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 29;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 30;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 28;
																	else
																		Vista = 27;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 30;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
				//28nuevo lunes
		//jueves
			if (strcmp(semanaseleccion, "Jueves") == 0 && seguridad > 3) {//holo
				Vista = atoi(dia);
				Vista -= 3;
			}
			else { //29 28 REVISADO
				if (strcmp(semanaseleccion, "Jueves") == 0) {
					//si es 1
					if (seguridad == 1) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 29;
						}

						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 28;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 29;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 28;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 29;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 29;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 28;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 29;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 28;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 29;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 27;
																	else
																		Vista = 26;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 29;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}

					if (seguridad == 2) {

						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 30;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 29;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 30;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 29;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 30;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 30;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 29;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 30;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 29;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 30;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 28;
																	else
																		Vista = 27;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 30;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}

					if (seguridad == 3) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 31;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 30;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 31;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 30;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 31;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 31;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 30;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 31;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 30;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 31;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 29;
																	else
																		Vista = 28;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 31;
																	}
																}
															}
														}
													}
												}
											}

										}
									}
								}
							}
						}
					}
				}
			}
				
			//viernes aqui
			if (strcmp(semanaseleccion, "Viernes") == 0 && seguridad > 4) {
				Vista = atoi(dia);
				Vista -= 4;
			}
			else { //28 27 REVISADO
				if (strcmp(semanaseleccion, "Viernes") == 0) {
					if (seguridad == 1) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 28;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 27;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 28;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 27;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 28;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 28;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 27;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 28;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 27;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 28;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 26;
																	else
																		Vista = 25;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 28;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}

					} //secguridad1
					if (seguridad == 2) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 29;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 28;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 29;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 28;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 29;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 29;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 28;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 29;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 28;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 29;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 27;
																	else
																		Vista = 26;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 29;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					} //cierre seguridad2
					if (seguridad == 3) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 30;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 29;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 30;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 29;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 30;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 30;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 29;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 30;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 29;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 30;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 28;
																	else
																		Vista = 27;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 30;
																	}

																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}

					}//cierre seguridad 3
					if (seguridad == 4) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 31;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 30;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 31;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 30;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 31;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 31;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 30;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 31;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 30;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 31;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 29;
																	else
																		Vista = 28;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 31;
																	}

																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}


					}//cierre seguridad4
				}
			}
					//sabado aqui	
			if (strcmp(semanaseleccion, "Sabado") == 0&&seguridad>5) {
								Vista = atoi(dia);
								Vista -= 5;
			}
			else { //27 26 REVISADO
				if (strcmp(semanaseleccion, "Sabado") == 0) {
					if (seguridad == 1) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 27;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 26;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 27;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 26;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 27;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 27;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 26;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 27;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 26;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 27;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 25;
																	else
																		Vista = 24;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 27;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}//cierre seguridad1
					if (seguridad == 2) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 28;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 27;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 28;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 27;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 28;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 28;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 27;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 28;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 27;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 28;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 26;
																	else
																		Vista = 25;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 28;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}//cierre 2
					if (seguridad == 3) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 29;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 28;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 29;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 28;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 29;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 29;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 28;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 29;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 28;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 29;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 27;
																	else
																		Vista = 26;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 29;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
					if (seguridad == 4) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 30;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 29;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 30;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 29;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 30;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 30;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 29;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 30;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 29;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 30;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 28;
																	else
																		Vista = 27;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 30;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					} //cierre
					if (seguridad == 5) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 31;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 30;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 31;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 30;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 31;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 31;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 30;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 31;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 30;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 31;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 29;
																	else
																		Vista = 28;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 31;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}//cierre
				
			}
			}

			//domingo
			if (strcmp(semanaseleccion, "Domingo") == 0&& seguridad>6) {
								Vista = atoi(dia);
								Vista -= 6;  //recorrer del domingo al lunes para mostar semana
			}
			else { //26 25 REVISADO
				if (strcmp(semanaseleccion, "Domingo") == 0) {
					if (seguridad == 1) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 26;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 25;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 26;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 25;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 26;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 26;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 25;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 26;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 25;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 26;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 26;
																	else
																		Vista = 25;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 26;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}//cierre 1
					if (seguridad == 2) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 27;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 26;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 27;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 26;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 27;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 27;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 26;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 27;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 26;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 27;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 25;
																	else
																		Vista = 24;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 27;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}//cierre 2
					if (seguridad == 3) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 28;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 27;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 28;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 27;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 28;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 28;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 27;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 28;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 27;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 28;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 26;
																	else
																		Vista = 25;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 28;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}//cierre 3
					if (seguridad == 4) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 29;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 28;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 29;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 28;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 29;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 29;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 28;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 29;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 28;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 29;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 27;
																	else
																		Vista = 26;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 29;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}//cierre4
					if (seguridad == 5) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 30;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 29;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 30;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 29;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 30;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 30;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 29;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 30;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 29;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 30;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 28;
																	else
																		Vista = 27;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 30;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}///cierre seguidad
					if (seguridad == 6) {
						if (strcmp(mes, "01") == 0) { //un a;o hacia atras
							strcpy(mes, "12");
							int NuevoAnio = atoi(aniovisto);
							NuevoAnio--;
							Vista = 31;
						}
						else {
							if (strcmp(mes, "12") == 0) {
								strcpy(mes, "11");
								Vista = 30;
							}
							else {
								if (strcmp(mes, "11") == 0) {
									strcpy(mes, "10");
									Vista = 31;
								}
								else {
									if (strcmp(mes, "10") == 0) {
										strcpy(mes, "09");
										Vista = 30;
									}
									else {
										if (strcmp(mes, "09") == 0) {
											strcpy(mes, "08");
											Vista = 31;
										}
										else {
											if (strcmp(mes, "08") == 0) {
												strcpy(mes, "07");
												Vista = 31;
											}
											else {
												if (strcmp(mes, "07") == 0) {
													strcpy(mes, "06");
													Vista = 30;
												}
												else {
													if (strcmp(mes, "06") == 0) {
														strcpy(mes, "05");
														Vista = 31;
													}
													else {
														if (strcmp(mes, "05") == 0) {
															strcpy(mes, "04");
															Vista = 30;
														}
														else {
															if (strcmp(mes, "04") == 0) {
																strcpy(mes, "03");
																Vista = 31;
															}
															else {
																if (strcmp(mes, "03") == 0) {
																	strcpy(mes, "02");
																	if (bisiesto)
																		Vista = 29;
																	else
																		Vista = 28;
																}
																else {
																	if (strcmp(mes, "02") == 0) {
																		strcpy(mes, "01");
																		Vista = 31;
																	}
																}
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}//cierre seguridad6
				
			}
		}
		
	

	
		//if(mes)
		if (strcmp(mes, "01") == 0 || strcmp(mes, "03") == 0 || strcmp(mes, "05") == 0 || strcmp(mes, "07") == 0 || strcmp(mes, "08") == 0 || strcmp(mes, "10") == 0 || strcmp(mes, "12") == 0)
			maximo = 31;
		if (strcmp(mes, "04") == 0 || strcmp(mes, "06") == 0 || strcmp(mes, "09") == 0 || strcmp(mes, "11") == 0)
			maximo = 30;
		//
		int vista2 = Vista;
		for (int i = 0; i < 7; i++) {
		
			char DiaChar[_MAX_PATH] = "";
			_itoa_s(vista2, DiaChar, 10);

			if (i == 0)
				SendMessage(DL, WM_SETTEXT, 0, LPARAM(DiaChar)); //el dia fue recorrido a lunes
			else
				if (i == 1)
					SendMessage(DM, WM_SETTEXT, 0, LPARAM(DiaChar));
				else
					if(i==2)
						SendMessage(DMM, WM_SETTEXT, 0, LPARAM(DiaChar));
					else
						if (i == 3)
							SendMessage(DJ, WM_SETTEXT, 0, LPARAM(DiaChar));
						else
							if (i == 4)
								SendMessage(DV, WM_SETTEXT, 0, LPARAM(DiaChar));
								else
									if (i == 5)
									SendMessage(DS, WM_SETTEXT, 0, LPARAM(DiaChar));
									else
										if(i==6)
											SendMessage(DD, WM_SETTEXT, 0, LPARAM(DiaChar));

			vista2++;
			if (vista2 > 31 && (strcmp(mes, "01") == 0 || strcmp(mes, "03") == 0 || strcmp(mes, "05") == 0 || strcmp(mes, "07") == 0 || strcmp(mes, "08") == 0 || strcmp(mes, "10") == 0 || strcmp(mes, "12") == 0))
				vista2 = 1;
			else
				if (vista2 > 30 && (strcmp(mes, "04") == 0 || strcmp(mes, "06") == 0 || strcmp(mes, "09") == 0 || strcmp(mes, "11") == 0))
					vista2 = 1;
				else
					if (vista2 > 28 && strcmp(mes, "02") == 0 && bisiesto)
						vista2 = 29;
					else
						if (vista2 > 29 && strcmp(mes, "02") == 0 && bisiesto == false)
							vista2 = 1;
		}
		//poner fechas en calendario semanal
	
	
		AlAuxiliar = inicial;
		if (inicial != 0) {
			int diaaux = Vista; //conviertye a entero el dia que vi
			int contador2 = 0;
			for (int contador=0; contador < 7; contador++) {
				if(contador!=0)
				diaaux =diaaux + 1; //dia entero lunes + avace de dia
				else
				diaaux = diaaux + 0;
				
				AlAuxiliar = inicial;
				_itoa(diaaux, dia, 10);
				if (diaaux == 1)
					strcpy(dia, "01");
				else
					if (diaaux == 2)
						strcpy(dia, "02");
					else
						if (diaaux == 3)
							strcpy(dia, "03");
						else
							if (diaaux == 4)
								strcpy(dia, "04");
							else
								if (diaaux == 5)
									strcpy(dia, "05");
								else
									if (diaaux == 6)
										strcpy(dia, "06");
									else
										if (diaaux == 7)
											strcpy(dia, "07");
										else
											if (diaaux == 8)
												strcpy(dia, "08");
											else
												if (diaaux == 9)
													strcpy(dia, "09");
											
					

				//conversion para comparar
				while (AlAuxiliar!=NULL) {
				

					if (strcmp(AlAuxiliar->anio, anio) == 0 && strcmp(AlAuxiliar->mes, mes) == 0 && strcmp(AlAuxiliar->dia, dia) == 0) {

						if (contador==0) {
							//lunes
							if (strcmp(AlAuxiliar->hora, "10") == 0) //10
								SendDlgItemMessage(hDlg, IDC_LUNES_10, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
							else
								if (strcmp(AlAuxiliar->hora, "11") == 0) //11
									SendDlgItemMessage(hDlg, IDC_LUNES_11, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
								else
									if (strcmp(AlAuxiliar->hora, "12") == 0) //12
										SendDlgItemMessage(hDlg, IDC_LUNES_12, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
									else
										if (strcmp(AlAuxiliar->hora, "13") == 0) //01
											SendDlgItemMessage(hDlg, IDC_LUNES_01, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
										else
											if (strcmp(AlAuxiliar->hora, "14") == 0) //02
												SendDlgItemMessage(hDlg, IDC_LUNES_02, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
											else
												if (strcmp(AlAuxiliar->hora, "15") == 0) //03
													SendDlgItemMessage(hDlg, IDC_LUNES_03, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
												else
													if (strcmp(AlAuxiliar->hora, "16") == 0) //04
														SendDlgItemMessage(hDlg, IDC_LUNES_04, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
													else
														if (strcmp(AlAuxiliar->hora, "17") == 0) //05
															SendDlgItemMessage(hDlg, IDC_LUNES_05, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
														else
															if (strcmp(AlAuxiliar->hora, "18") == 0) //06
																SendDlgItemMessage(hDlg, IDC_LUNES_06, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
															else
																if (strcmp(AlAuxiliar->hora, "19") == 0) //07
																	SendDlgItemMessage(hDlg, IDC_LUNES_07, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));

						}

						else {
							//martes
							if (contador == 1) {
								//martes inicio
								//lunes
								if (strcmp(AlAuxiliar->hora, "10") == 0) //10
									SendDlgItemMessage(hDlg, IDC_MARTES_10, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
								else
									if (strcmp(AlAuxiliar->hora, "11") == 0) //11
										SendDlgItemMessage(hDlg, IDC_MARTES_11, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
									else
										if (strcmp(AlAuxiliar->hora, "12") == 0) //12
											SendDlgItemMessage(hDlg, IDC_MARTES_12, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
										else
											if (strcmp(AlAuxiliar->hora, "13") == 0) //01
												SendDlgItemMessage(hDlg, IDC_MARTES_01, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
											else
												if (strcmp(AlAuxiliar->hora, "14") == 0) //02
													SendDlgItemMessage(hDlg, IDC_MARTES_02, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
												else
													if (strcmp(AlAuxiliar->hora, "15") == 0) //03
														SendDlgItemMessage(hDlg, IDC_MARTES_03, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
													else
														if (strcmp(AlAuxiliar->hora, "16") == 0) //04
															SendDlgItemMessage(hDlg, IDC_MARTES_04, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
														else
															if (strcmp(AlAuxiliar->hora, "17") == 0) //05
																SendDlgItemMessage(hDlg, IDC_MARTES_05, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
															else
																if (strcmp(AlAuxiliar->hora, "18") == 0) //06
																	SendDlgItemMessage(hDlg, IDC_MARTES_06, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																else
																	if (strcmp(AlAuxiliar->hora, "19") == 0) //07
																		SendDlgItemMessage(hDlg, IDC_MARTES_07, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
								//martes fin
							}
							else {
								if (contador == 2) {
									//lunes
									if (strcmp(AlAuxiliar->hora, "10") == 0) //10
										SendDlgItemMessage(hDlg, IDC_MIERCOLES_10, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
									else
										if (strcmp(AlAuxiliar->hora, "11") == 0) //11
											SendDlgItemMessage(hDlg, IDC_MIERCOLES_11, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
										else
											if (strcmp(AlAuxiliar->hora, "12") == 0) //12
												SendDlgItemMessage(hDlg, IDC_MIERCOLES_12, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
											else
												if (strcmp(AlAuxiliar->hora, "13") == 0) //01
													SendDlgItemMessage(hDlg, IDC_MIERCOLES_01, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
												else
													if (strcmp(AlAuxiliar->hora, "14") == 0) //02
														SendDlgItemMessage(hDlg, IDC_MIERCOLES_02, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
													else
														if (strcmp(AlAuxiliar->hora, "15") == 0) //03
															SendDlgItemMessage(hDlg, IDC_MIERCOLES_03, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
														else
															if (strcmp(AlAuxiliar->hora, "16") == 0) //04
																SendDlgItemMessage(hDlg, IDC_MIERCOLES_04, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
															else
																if (strcmp(AlAuxiliar->hora, "17") == 0) //05
																	SendDlgItemMessage(hDlg, IDC_MIERCOLES_05, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																else
																	if (strcmp(AlAuxiliar->hora, "18") == 0) //06
																		SendDlgItemMessage(hDlg, IDC_MIERCOLES_06, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																	else
																		if (strcmp(AlAuxiliar->hora, "19") == 0) //07
																			SendDlgItemMessage(hDlg, IDC_MIERCOLES_07, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
								}//MIERCOLES
								else {
									//jueves
									if (contador == 3) {
										if (strcmp(AlAuxiliar->hora, "10") == 0) //10
											SendDlgItemMessage(hDlg, IDC_JUEVES_10, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
										else
											if (strcmp(AlAuxiliar->hora, "11") == 0) //11
												SendDlgItemMessage(hDlg, IDC_JUEVES_11, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
											else
												if (strcmp(AlAuxiliar->hora, "12") == 0) //12
													SendDlgItemMessage(hDlg, IDC_JUEVES_12, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
												else
													if (strcmp(AlAuxiliar->hora, "13") == 0) //01
														SendDlgItemMessage(hDlg, IDC_JUEVES_01, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
													else
														if (strcmp(AlAuxiliar->hora, "14") == 0) //02
															SendDlgItemMessage(hDlg, IDC_JUEVES_02, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
														else
															if (strcmp(AlAuxiliar->hora, "15") == 0) //03
																SendDlgItemMessage(hDlg, IDC_JUEVES_03, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
															else
																if (strcmp(AlAuxiliar->hora, "16") == 0) //04
																	SendDlgItemMessage(hDlg, IDC_JUEVES_04, LB_ADDSTRING, 0, LPARAM(AlAuxiliar->asunto));
																else
																	if (strcmp(AlAuxiliar->hora, "17") == 0) //05
																		SendDlgItemMessage(hDlg, IDC_JUEVES_05, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																	else
																		if (strcmp(AlAuxiliar->hora, "18") == 0) //06
																			SendDlgItemMessage(hDlg, IDC_JUEVES_06, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																		else
																			if (strcmp(AlAuxiliar->hora, "19") == 0) //07
																				SendDlgItemMessage(hDlg, IDC_JUEVES_07, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
									}
									else {
										if (contador == 4) {
											//lunes
											if (strcmp(AlAuxiliar->hora, "10") == 0) //10
												SendDlgItemMessage(hDlg, IDC_VIERNES_10, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
											else
												if (strcmp(AlAuxiliar->hora, "11") == 0) //11
													SendDlgItemMessage(hDlg, IDC_VIERNES_11, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
												else
													if (strcmp(AlAuxiliar->hora, "12") == 0) //12
														SendDlgItemMessage(hDlg, IDC_VIERNES_12, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
													else
														if (strcmp(AlAuxiliar->hora, "13") == 0) //01
															SendDlgItemMessage(hDlg, IDC_VIERNES_01, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
														else
															if (strcmp(AlAuxiliar->hora, "14") == 0) //02
																SendDlgItemMessage(hDlg, IDC_VIERNES_02, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
															else
																if (strcmp(AlAuxiliar->hora, "15") == 0) //03
																	SendDlgItemMessage(hDlg, IDC_VIERNES_03, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																else
																	if (strcmp(AlAuxiliar->hora, "16") == 0) //04
																		SendDlgItemMessage(hDlg, IDC_VIERNES_04, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																	else
																		if (strcmp(AlAuxiliar->hora, "17") == 0) //05
																			SendDlgItemMessage(hDlg, IDC_VIERNES_05, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																		else
																			if (strcmp(AlAuxiliar->hora, "18") == 0) //06
																				SendDlgItemMessage(hDlg, IDC_VIERNES_06, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																			else
																				if (strcmp(AlAuxiliar->hora, "19") == 0) //07
																					SendDlgItemMessage(hDlg, IDC_VIERNES_07, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));




										}
										else {
											if (contador == 5) {
												//lunes
												if (strcmp(AlAuxiliar->hora, "10") == 0) //10
													SendDlgItemMessage(hDlg, IDC_SABADO_10, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
												else
													if (strcmp(AlAuxiliar->hora, "11") == 0) //11
														SendDlgItemMessage(hDlg, IDC_SABADO_11, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
													else
														if (strcmp(AlAuxiliar->hora, "12") == 0) //12
															SendDlgItemMessage(hDlg, IDC_SABADO_12, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
														else
															if (strcmp(AlAuxiliar->hora, "13") == 0) //01
																SendDlgItemMessage(hDlg, IDC_SABADO_01, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
															else
																if (strcmp(AlAuxiliar->hora, "14") == 0) //02
																	SendDlgItemMessage(hDlg, IDC_SABADO_02, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																else
																	if (strcmp(AlAuxiliar->hora, "15") == 0) //03
																		SendDlgItemMessage(hDlg, IDC_SABADO_03, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																	else
																		if (strcmp(AlAuxiliar->hora, "16") == 0) //04
																			SendDlgItemMessage(hDlg, IDC_SABADO_04, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																		else
																			if (strcmp(AlAuxiliar->hora, "17") == 0) //05
																				SendDlgItemMessage(hDlg, IDC_SABADO_05, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																			else
																				if (strcmp(AlAuxiliar->hora, "18") == 0) //06
																					SendDlgItemMessage(hDlg, IDC_SABADO_06, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																				else
																					if (strcmp(AlAuxiliar->hora, "19") == 0) //07
																						SendDlgItemMessage(hDlg, IDC_SABADO_07, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));


											}
											else {
												if (contador == 6) {
													//lunes
													if (strcmp(AlAuxiliar->hora, "10") == 0) //10
														SendDlgItemMessage(hDlg, IDC_DOMINGO_10, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
													else
														if (strcmp(AlAuxiliar->hora, "11") == 0) //11
															SendDlgItemMessage(hDlg, IDC_DOMINGO_11, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
														else
															if (strcmp(AlAuxiliar->hora, "12") == 0) //12
																SendDlgItemMessage(hDlg, IDC_DOMINGO_12, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
															else
																if (strcmp(AlAuxiliar->hora, "13") == 0) //01
																	SendDlgItemMessage(hDlg, IDC_DOMINGO_01, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																else
																	if (strcmp(AlAuxiliar->hora, "14") == 0) //02
																		SendDlgItemMessage(hDlg, IDC_DOMINGO_02, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																	else
																		if (strcmp(AlAuxiliar->hora, "15") == 0) //03
																			SendDlgItemMessage(hDlg, IDC_DOMINGO_03, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																		else
																			if (strcmp(AlAuxiliar->hora, "16") == 0) //04
																				SendDlgItemMessage(hDlg, IDC_DOMINGO_04, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																			else
																				if (strcmp(AlAuxiliar->hora, "17") == 0) //05
																					SendDlgItemMessage(hDlg, IDC_DOMINGO_05, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																				else
																					if (strcmp(AlAuxiliar->hora, "18") == 0) //06
																						SendDlgItemMessage(hDlg, IDC_DOMINGO_06, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));
																					else
																						if (strcmp(AlAuxiliar->hora, "19") == 0) //07
																							SendDlgItemMessage(hDlg, IDC_DOMINGO_07, LB_ADDSTRING, _MAX_PATH, LPARAM(AlAuxiliar->asunto));


												}

											}

										}
									}

								}


							}// miercoles

						}
						//
					}
					//cuidado de mes y dia desplazar
				
				
									
									//
	
						AlAuxiliar = AlAuxiliar->sig;

				} //CIERRE WHILE
				if (diaaux >= 31 && (strcmp(mes, "01") == 0 || strcmp(mes, "03") == 0 || strcmp(mes, "05") == 0 || strcmp(mes, "07") == 0 || strcmp(mes, "08") == 0 || strcmp(mes, "10") == 0 || strcmp(mes, "12") == 0)) {
					strcpy(dia, "01");
					diaaux = 0;
					//contador2 = 0; //al  final se suma1
								   //avanzar de mes
					if (strcmp(mes, "01") == 0) // si es 1 mover a febrero
						strcpy(mes, "02");
					else
						if (strcmp(mes, "03") == 0) //si es marzo mover a abril
							strcpy(mes, "04");
						else
							if (strcmp(mes, "05") == 0) //si es mayo mover a junio
								strcpy(mes, "06");
							else
								if (strcmp(mes, "07") == 0) //si es junio mover a agosto
									strcpy(mes, "08");
								else
									if (strcmp(mes, "08") == 0)//si es agosto mover a septiembre
										strcpy(mes, "09");
									else
										if (strcmp(mes, "10") == 0) //si es octubre mover a noviembre
											strcpy(mes, "11");
										else
											if (strcmp(mes, "12") == 0)//si es diciembre mover a enero
												strcpy(mes, "01");
				}
				else {
					if (diaaux >= 30 && (strcmp(mes, "04") == 0 || strcmp(mes, "06") == 0 || strcmp(mes, "09") == 0 || strcmp(mes, "11") == 0)) {
						strcpy(dia, "01"); //dia 1 del siguiente mes
						diaaux = 0;
										   //contador2 = 0;			//se hace uno al final
						if (strcmp(mes, "04") == 0) //abril moverme a mayo
							strcpy(mes, "05");
						else
							if (strcmp(mes, "06") == 0)
								strcpy(mes, "07");
							else
								if (strcmp(mes, "09") == 0)
									strcpy(mes, "10");
								else
									if (strcmp(mes, "11") == 0)
										strcpy(mes, "12");
					}
					else {
						if (diaaux >= 28 && strcmp(mes, "02") == 0 && bisiesto) {
							strcpy(mes, "03");
							strcpy(dia, "01");
							diaaux = 0;
							//contador2 = 0;
						}
						else {
							if (diaaux >= 29 && strcmp(mes, "02") == 0 && bisiesto == false) {
								//vista2 = 1;
								strcpy(mes, "03");
								strcpy(dia, "01");
								diaaux = 0;
								//contador2 = 0;

							}
						}
					}
				}

				
			}//cierre for

		}
		//lo bueno
	}
	void Mensual(HWND hDlg) {
		int dia = 0;
		AlAuxiliar = inicial;
		if (inicial != 0) {
			while (inicial != 0){
				if (strcmp(AlAuxiliar->mes,"05") == 0 && strcmp(AlAuxiliar->anio, "2017") == 0) {
					dia = atoi(AlAuxiliar->dia);
					if (dia == 1)
						SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA1), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
					else
						if (dia == 2)
							SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA2), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
						else
							if (dia == 3)
								SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA3), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
							else
								if (dia == 4)
									SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA4), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
								else
									if (dia == 5)
										SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA5), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
									else
										if (dia == 6)
											SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA6), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
										else
											if (dia == 7)
												SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA7), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
											else
												if (dia == 8)
													SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA8), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
												else
													if (dia == 9)
														SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA9), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
													else
														if (dia == 10)
															SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA10), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
														else
															if (dia == 11)
																SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA11), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
															else
																if (dia == 12)
																	SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA12), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																else
																	if (dia == 13)
																		SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA13), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																	else
																		if (dia == 14)
																			SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA14), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																		else
																			if (dia == 15)
																				SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA15), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																			else
																				if (dia == 16)
																					SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA16), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																				else
																					if (dia == 17)
																						SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA17), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																					else
																						if (dia == 18)
																							SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA18), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																						else
																							if (dia == 19)
																								SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA19), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																							else
																								if (dia == 20)
																									SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA20), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																								else
																									if (dia == 21)
																										SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA21), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																									else
																										if (dia == 22)
																											SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA22), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																										else
																											if (dia == 23)
																												SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA23), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																											else
																												if (dia == 24)
																													SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA24), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																												else
																													if (dia == 25)
																														SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA25), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																													else
																														if (dia == 26)
																															SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA26), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																														else
																															if (dia == 27)
																																SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA27), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																															else
																																if (dia == 28)
																																	SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA28), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
																																else
																																	if (dia == 29)																						SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA29), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));						else
																																		if (dia == 30)
																																			SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA30), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));					else
																																			SendMessage(GetDlgItem(hDlg, IDC_LIST_DIA31), LB_ADDSTRING, -1, LPARAM(AlAuxiliar->asunto));
				}


			if (AlAuxiliar == AlFin)
				break;
			else
				AlAuxiliar = AlAuxiliar->sig;

		}
			}
		else {
			MessageBox(hDlg, "No existen alarmas", "AVISO", 0);
		}
		AlAuxiliar = AlFin;
	}
	void DiaSemana(char fecha[20])
	{
		string cutMonth(fecha);
		string Day = cutMonth.substr(0, cutMonth.find_first_of("/"));
		int num = atoi(Day.c_str());
		string Mes = cutMonth.substr(3, cutMonth.find_first_of("/"));
		int mes = atoi(Mes.c_str());
		string Año = cutMonth.substr(6, 4);
		string Año2 = cutMonth.substr(8, 2);
		int añocorto = atoi(Año2.c_str());
		int año = atoi(Año.c_str());

		int mesraro = obtenerm(mes);
		int añoraro = obteners(año);

		int divicionaño = añocorto / 4;

		int x = num + mesraro + añocorto + divicionaño + añoraro;

		int nsemana = x % 7;

		switch (nsemana)
		{
		case 0: strcpy(semana, "Domingo");
			break;
		case 1: strcpy(semana, "Lunes");
			break;
		case 2: strcpy(semana, "Martes");
			break;
		case 3: strcpy(semana, "Miercoles");
			break;
		case 4: strcpy(semana, "Jueves");
			break;
		case 5: strcpy(semana, "Viernes");
			break;
		case 6: strcpy(semana, "Sabado");
			break;
		}
	}
	int obteners(int año)
	{
		if (año > 1600 && año < 1699)
		{
			año = 6;
		}
		else
		{
			if (año > 1700 && año < 1799)
			{
				año = 4;
			}
			else
			{
				if (año > 1800 && año < 1899)
				{
					año = 2;
				}
				else
				{
					if (año > 1900 && año < 1999)
					{
						año = 0;
					}
					else
					{
						if (año > 2000 && año < 2099)
						{
							año = 6;
						}
					}
				}
			}
		}
		return año;
	}
	int obtenerm(int mes)
	{
		switch (mes)
		{
		case 1: mes = 0; break;
		case 2: mes = 3; break;
		case 3: mes = 3; break;
		case 4: mes = 6; break;
		case 5: mes = 1; break;
		case 6: mes = 4; break;
		case 7: mes = 6; break;
		case 8: mes = 2; break;
		case 9: mes = 5; break;
		case 10: mes = 0; break;
		case 11: mes = 3; break;
		case 12: mes = 5; break;
		}
		return mes;
	}
	void VaciadoListas(HWND hDlg, int num) {
	//0 semana //1 mes
		if (num == 0) {
			//DIAS
			SendMessage(GetDlgItem(hDlg, IDC_STATIC_DIA1), WM_SETTEXT, 0, LPARAM(""));
			SendMessage(GetDlgItem(hDlg, IDC_STATIC_DIA2), WM_SETTEXT, 0, LPARAM(""));
			SendMessage(GetDlgItem(hDlg, IDC_STATIC_DIA3), WM_SETTEXT, 0, LPARAM(""));
			SendMessage(GetDlgItem(hDlg, IDC_STATIC_DIA4), WM_SETTEXT, 0, LPARAM(""));
			SendMessage(GetDlgItem(hDlg, IDC_STATIC_DIA5), WM_SETTEXT, 0, LPARAM(""));
			SendMessage(GetDlgItem(hDlg, IDC_STATIC_DIA6), WM_SETTEXT, 0, LPARAM(""));
			SendMessage(GetDlgItem(hDlg, IDC_STATIC_DIA7), WM_SETTEXT, 0, LPARAM(""));
			//LUNES
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_10), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_11), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_12), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_01), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_02), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_03), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_04), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_05), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_06), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_LUNES_07), LB_RESETCONTENT, 0, 0);
			//MARTES
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_10), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_11), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_12), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_01), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_02), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_03), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_04), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_05), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_06), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MARTES_07), LB_RESETCONTENT, 0, 0);
			//MIERCOLES
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_10), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_11), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_12), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_01), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_02), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_03), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_04), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_05), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_06), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_MIERCOLES_07), LB_RESETCONTENT, 0, 0);
			//JUEVES
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_10), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_11), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_12), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_01), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_02), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_03), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_04), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_05), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_06), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_JUEVES_07), LB_RESETCONTENT, 0, 0);
			//VIERNES
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_10), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_11), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_12), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_01), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_02), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_03), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_04), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_05), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_06), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_VIERNES_07), LB_RESETCONTENT, 0, 0);
			//SABADO
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_10), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_11), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_12), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_01), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_02), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_03), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_04), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_05), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_06), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_SABADO_07), LB_RESETCONTENT, 0, 0);
			//DOMINGO
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_10), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_11), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_12), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_01), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_02), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_03), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_04), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_05), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_06), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hDlg, IDC_DOMINGO_07), LB_RESETCONTENT, 0, 0);
		
		}
		else {
		
		//DESPUES
		
		
		}
	
	
	
	}