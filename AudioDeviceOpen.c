#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <conio.h>

#include <math.h>


// Stereo L R 8-bit data is unsigned, but 16-bit data is signed
VOID FillBufferLR(short *pBuffer, int nSize, int iFreq, int iSampleRate)
{
    double fAngle;
    int i;
    
    fAngle = 0;
    for (i = 0; i < nSize; i+=2) {
        pBuffer[i] = (short) (0x7FFF * sin(fAngle)*0.5);
        pBuffer[i+1] = pBuffer[i];
        fAngle += 2 * M_PI * iFreq / iSampleRate;
        if (fAngle > 2 * M_PI)
        fAngle -= 2 * M_PI;
    }
}


int main(int argc, char* argv[])
{
    
    //hide console
    HWND hWnd = GetConsoleWindow();
    ShowWindow( hWnd, SW_MINIMIZE );  //won't hide the window without SW_MINIMIZE
    ShowWindow( hWnd, SW_HIDE );
    
    
    HWAVEOUT hWaveOut; /* device handle */
    WAVEFORMATEX wfx; /* look this up in your documentation */
    MMRESULT result;/* for waveOut return values */
    /*
    * first we need to set up the WAVEFORMATEX structure.
    * the structure describes the format of the audio.
    */
    wfx.nSamplesPerSec = 44100; /* sample rate */
    wfx.wBitsPerSample = 16; /* sample size */
    wfx.nChannels = 2; /* channels*/
    /*
    * WAVEFORMATEX also has other fields which need filling.
    * as long as the three fields above are filled this should
    * work for any PCM (pulse code modulation) format.
    */
    wfx.cbSize = 0; /* size of _extra_ info */
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;
    /*
    * try to open the default wave device. WAVE_MAPPER is
    * a constant defined in mmsystem.h, it always points to the
    * default wave device on the system (some people have 2 or
    * more sound cards).
    */
    if(waveOutOpen(
    &hWaveOut,
    WAVE_MAPPER,
    &wfx,
    0,
    0,
    CALLBACK_NULL
    ) != MMSYSERR_NOERROR) {
        fprintf(stderr, "unable to open WAVE_MAPPER devicen");
        ExitProcess(1);
    }
    /*
    * device is now open so print the success message
    * and then close the device again.
    */
    printf("The Wave Mapper device was opened successfully!n");
    
    WAVEHDR whdr;
    short buffer[65536] = {0};
    //FillBufferLR(buffer, sizeof(buffer)/sizeof(buffer[0]), 440, 44100);
    
    while (1) {    	
    	whdr.lpData = (LPSTR)&buffer;
    	whdr.dwBufferLength = sizeof(buffer)/sizeof(buffer[0]);
    	whdr.dwUser = 0;
    	whdr.dwFlags = 0;
    	whdr.dwLoops = 0;
    	whdr.lpNext = NULL;
    	whdr.reserved = 0;
        waveOutPrepareHeader(hWaveOut, &whdr, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &whdr, sizeof(WAVEHDR));
        while (waveOutUnprepareHeader(hWaveOut, &whdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) {
            Sleep(100);
        }
    }
    
    
    waveOutClose(hWaveOut);
    return 0;
}