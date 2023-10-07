#include <Windows.h>
#include <Audioclient.h>
#include <endpointvolume.h>
#include <mmdeviceapi.h>
#include <string>
#include <iostream>
#include <fstream>
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"ole32.lib")

using namespace std;

class cFuncoes
{
private:

	IMMDeviceEnumerator* Enumerador = 0;
	IAudioEndpointVolume* Audio = 0;
	IMMDevice* Dispositivo = 0;

public:

	void InicializarInstancia()
	{
		CoInitializeEx(0, COINIT_MULTITHREADED);
		CoCreateInstance(__uuidof(MMDeviceEnumerator), 0, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&Enumerador);
	}

	void InicializarCliente()
	{
		/*
		* 
		* eRender e eCapture - Apenas os dispositivos de som que recebem os �udios atrav�s de aplicativos e outros recursos.
		* eMultimedia - �udios provenientes de m�sicas, filmes, narra��es e grava��es.
		* 
		* Outros sinalizadores e seus significados:
		* 
		* ERole: eCommunications - Comunica��o por voz.
		* ERole: eConsole - Jogos, sons do sistema e comandos por voz.
		*/
		Enumerador->GetDefaultAudioEndpoint(eRender, eMultimedia, (IMMDevice**)&Dispositivo);
		Dispositivo->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, 0, (void**)&Audio);
	}

	void ConfiguracoesDeVolume(float Valor)
	{
		Audio->SetMasterVolumeLevelScalar(Valor, 0);
	}

	void ObterNivelDeVolume()
	{
		float Nivel;
		Audio->GetMasterVolumeLevelScalar(&Nivel);

		//Formata a string para o formato correto, sem incluir o valor de float completo.
		//Esta formata��o remove os seguintes caracteres ( 0. ).
		string formatar = to_string(Nivel);
		formatar.erase(remove(formatar.begin(), formatar.end(), '0'),formatar.end());
		formatar.erase(remove(formatar.begin(), formatar.end(), '.'), formatar.end());

		cout << "N�vel atual do volume: " << formatar << '\n';
	}

	void Silenciar(BOOL Mutar)
	{
		Audio->SetMute(Mutar, 0);
	}

	void VerificarSilenciado()
	{
		BOOL Silenciado;
		Audio->GetMute(&Silenciado);
		if (Silenciado == FALSE)
			cout << "N�o silenciado\n";
		else
			cout << "Silenciado";
	}

}Funcoes;

int main()
{

	cout << "O assistente est� executando opera��es no sistema de �udio...";

	Funcoes.InicializarInstancia();
	Funcoes.InicializarCliente();
	Funcoes.ConfiguracoesDeVolume(0.50);//Ir� manter no n�vel 50 de volume.
	Funcoes.ConfiguracoesDeVolume(0.99);//Volume m�ximo.
	Funcoes.Silenciar(TRUE);
	Funcoes.VerificarSilenciado();
	Funcoes.Silenciar(FALSE);
	Funcoes.ObterNivelDeVolume();

	system("pause");
}