// MemReader2.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <cstdio>
#include <Windows.h>
#include <TlHelp32.h>
#include <thread>
#include <Psapi.h>
#include <tchar.h>
using namespace std;



HANDLE PrintAllProcesses()
{
  DWORD pid = 0;
  int thread_cnt = 0;
  ZeroMemory(&thread_cnt, sizeof(thread_cnt));

  // Create toolhelp snapshot.
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 process;
  ZeroMemory(&process, sizeof(process));
  process.dwSize = sizeof(process);

  // Walkthrough all processes.
  if (Process32First(snapshot, &process))
  {
    do
    {
      // Compare process.szExeFile based on format of name, i.e., trim file path
      // trim .exe if necessary, etc.
      thread_cnt = thread_cnt + process.cntThreads;
      wchar_t* txt = process.szExeFile;
      wstring ws(txt);
      string proc_name(ws.begin(), ws.end());
      //cout << final << endl;
      //cout << process.cntThreads << endl;
      cout << "PROCESS-NAME:" << proc_name << endl;
      cout << "PROCESS-ID:" << (unsigned int)process.th32ProcessID << endl;

      DWORD proc_pid;


      HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, process.th32ProcessID);
      LPSTR procname = 0;
      HMODULE hMods[1024];
      DWORD cbNeeded;
      int i;

      GetProcessImageFileNameA(handle, procname, sizeof(procname));

      if (EnumProcessModules(handle, hMods, sizeof(hMods), &cbNeeded))
      {
        for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
          TCHAR szModName[MAX_PATH];

          // Get the full path to the module's file.

          if (GetModuleFileNameEx(handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
          {
            // Print the module name and handle value.

            _tprintf(TEXT("\t%s (0x%08X)\n"), szModName, hMods[i]);
          }

          MODULEINFO info;
          if (GetModuleInformation(handle, hMods[i], &info, sizeof(info)))
          {
            cout << "ENTRY POINT: 0x"<< info.EntryPoint << endl;
          }




        }
      }

    } while (Process32Next(snapshot, &process));
    cout << "TOTAL SYSTEM THREAD COUNT: " << thread_cnt << endl;

  }
  return NULL;

}

int main()
{
  std::cout << "Hello World!\n";

  while (true)
  {
    PrintAllProcesses();
    std::this_thread::sleep_for(chrono::seconds(1));
  }
  

}

// Programm ausführen: STRG+F5 oder Menüeintrag "Debuggen" > "Starten ohne Debuggen starten"
// Programm debuggen: F5 oder "Debuggen" > Menü "Debuggen starten"

// Tipps für den Einstieg: 
//   1. Verwenden Sie das Projektmappen-Explorer-Fenster zum Hinzufügen/Verwalten von Dateien.
//   2. Verwenden Sie das Team Explorer-Fenster zum Herstellen einer Verbindung mit der Quellcodeverwaltung.
//   3. Verwenden Sie das Ausgabefenster, um die Buildausgabe und andere Nachrichten anzuzeigen.
//   4. Verwenden Sie das Fenster "Fehlerliste", um Fehler anzuzeigen.
//   5. Wechseln Sie zu "Projekt" > "Neues Element hinzufügen", um neue Codedateien zu erstellen, bzw. zu "Projekt" > "Vorhandenes Element hinzufügen", um dem Projekt vorhandene Codedateien hinzuzufügen.
//   6. Um dieses Projekt später erneut zu öffnen, wechseln Sie zu "Datei" > "Öffnen" > "Projekt", und wählen Sie die SLN-Datei aus.
